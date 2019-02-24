/*
 *  linux/arch/m68k/mm/fault.c
 *
 *  Copyright (C) 1995  Hamish Macdonald
 */

#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/ptrace.h>
#include <linux/interrupt.h>
#include <linux/module.h>

#include <asm/setup.h>
#include <asm/traps.h>
#include <asm/uaccess.h>
#include <asm/pgalloc.h>
#include <xhyp/xhyp.h>

int send_fault_sig(struct pt_regs *regs)
{
	siginfo_t siginfo = { 0, 0, 0, };

	siginfo.si_signo = current->thread.trap_no;
	siginfo.si_code = current->thread.error_code;
	siginfo.si_addr = (void *)current->thread.address;

	force_sig_info(siginfo.si_signo, &siginfo, current);

	return 1;
}

/*
 * This routine handles page faults.  It determines the problem, and
 * then passes it off to one of the appropriate routines.
 *
 * error_code:
 *	bit 0 == 0 means no page found, 1 means protection fault
 *	bit 1 == 0 means read, 1 means write
 *
 * If this routine detects a bad access, it returns 1, otherwise it
 * returns 0.
 */
int do_page_fault(struct pt_regs *regs, unsigned long address,
			      unsigned long write)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct * vma;
	int fault;
	unsigned int flags = FAULT_FLAG_ALLOW_RETRY | FAULT_FLAG_KILLABLE;

	/*
 	* 	if address >= TASK_SIZE we are in kernel
 	*/
	debugp("address %08lx write %d\n", address, write);
	debugp("current->threadinfo: %p\n", current_thread_info());
	if (address >= TASK_SIZE) {
		if ((unsigned long)address < PAGE_SIZE)
			printk(KERN_ALERT "Unable to handle kernel NULL pointer dereference");
		else
			printk(KERN_ALERT "Unable to handle kernel access");
		printk(" at virtual address %p\n", address);
		xhyp_die("Oops", regs, 0, 0);
		do_exit(SIGKILL);
	}

	/*
	 * If we're in an interrupt or have no user
	 * context, we must not take the fault..
	 */
	if (in_atomic() || !mm) {
		debugp("atomic %d mm %p\n", in_atomic(), mm);
		goto no_context;
	}
retry:
	//debugp("before down_read\n");
	down_read(&mm->mmap_sem);

	vma = find_vma(mm, address);
	if (!vma)
		goto map_err;
	if (vma->vm_flags & VM_IO)
		goto acc_err;
	if (vma->vm_start <= address)
		goto good_area;
	if (!(vma->vm_flags & VM_GROWSDOWN))
		goto map_err;
	if (expand_stack(vma, address))
		goto map_err;

/*
 * Ok, we have a good vm_area for this memory access, so
 * we can handle it..
 */
good_area:
	//debugp("good area\n");
	if (write) {
		if (!(vma->vm_flags & VM_WRITE))
			goto acc_err;
		flags |= FAULT_FLAG_WRITE;
	}

	//debugp("flags: %08lx\n", flags),
	fault = handle_mm_fault(mm, vma, address, flags);
	//debugp("fault: %08lx\n", fault);

	if ((fault & VM_FAULT_RETRY) && fatal_signal_pending(current))
		return 0;

	if (unlikely(fault & VM_FAULT_ERROR)) {
		if (fault & VM_FAULT_OOM)
			goto out_of_memory;
		else if (fault & VM_FAULT_SIGBUS)
			goto bus_err;
		BUG();
	}
//debugp("\n");
	/*
	 * Major/minor page fault accounting is only done on the
	 * initial attempt. If we go through a retry, it is extremely
	 * likely that the page will be found in page cache at that point.
	 */
	if (flags & FAULT_FLAG_ALLOW_RETRY) {
		if (fault & VM_FAULT_MAJOR)
			current->maj_flt++;
		else
			current->min_flt++;
		if (fault & VM_FAULT_RETRY) {
			/* Clear FAULT_FLAG_ALLOW_RETRY to avoid any risk
			 * of starvation. */
			flags &= ~FAULT_FLAG_ALLOW_RETRY;

			/*
			 * No need to up_read(&mm->mmap_sem) as we would
			 * have already released it in __lock_page_or_retry
			 * in mm/filemap.c.
			 */

			goto retry;
		}
	}

//debugp("before up_read\n");
	up_read(&mm->mmap_sem);
	return 0;

/*
 * We ran out of memory, or some other thing happened to us that made
 * us unable to handle the page fault gracefully.
 */
out_of_memory:
debugp("out_of_memory\n");
	up_read(&mm->mmap_sem);
	if (!user_mode(regs))
		goto no_context;
	pagefault_out_of_memory();
	return 0;

no_context:
debugp("no_context\n");
	current->thread.trap_no = SIGBUS;
	current->thread.address = address;
	return send_fault_sig(regs);

bus_err:
debugp("bus_err\n");
	current->thread.trap_no = SIGBUS;
	current->thread.error_code = BUS_ADRERR;
	current->thread.address = address;
	goto send_sig;

map_err:
debugp("map_err\n");
	current->thread.trap_no = SIGSEGV;
	current->thread.error_code = SEGV_MAPERR;
	current->thread.address = address;
	goto send_sig;

acc_err:
debugp("acc_err\n");
	current->thread.trap_no = SIGSEGV;
	current->thread.error_code = SEGV_ACCERR;
	current->thread.address = address;

send_sig:
debugp("up_read and sendsig\n");
	up_read(&mm->mmap_sem);
	return send_fault_sig(regs);
}
