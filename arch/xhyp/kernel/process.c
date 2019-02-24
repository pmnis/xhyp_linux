#include <linux/module.h>
#include <linux/sched.h>
#include <linux/elf.h>
#include <linux/personality.h>
#include <xhyp/xhyp.h>

#define kernel_thread_exit	do_exit
extern void kernel_thread_helper(void);

int elf_check_arch(const struct elf32_hdr *x)
{
        unsigned int eflags;

	debugp("Start at: %08lx\n", x->e_entry);
	
	return 1;
}
EXPORT_SYMBOL(elf_check_arch);

void elf_set_personality(const struct elf32_hdr *x)
{
	unsigned int eflags = x->e_flags;
	unsigned int personality = current->personality & ~PER_MASK;

	personality |= PER_LINUX;
	personality |= ADDR_LIMIT_32BIT;
	set_personality(personality);
}


void start_thread(struct pt_regs *regs,unsigned long pc,unsigned long sp)
{
	unsigned long *stack = (unsigned long *)sp; 

	memset(regs->uregs, 0, sizeof(regs->uregs));

	regs->ARM_cpsr = USR_MODE;

	if (elf_hwcap & HWCAP_THUMB && pc & 1)
		regs->ARM_cpsr |= PSR_T_BIT;

	regs->ARM_cpsr |= PSR_ENDSTATE; 
	regs->ARM_pc = pc & ~1;		/* pc */
	regs->ARM_sp = sp;		/* sp */
	regs->ARM_r2 = stack[2];	/* r2 (envp) */ 
	regs->ARM_r1 = stack[1];	/* r1 (argv) */ 
	regs->ARM_r0 = stack[0];	/* r0 (argc) */ 
	debugp("pc: %08lx sp %08lx argc %08lx argv %08lx\n", pc, sp, stack[0], stack[1]);
	debugp("current->comm: %s mm: %p\n", current->comm, current->mm);
	debugp("current->threadinfo: %p\n", current_thread_info());
        {
        int i;
        unsigned long *p = (unsigned long *)regs;
        debugp("regs at......: %p\n", p);
        debugp("ARM_R1.......: %p\n", &regs->ARM_r1);
        debugp("ARM_R1.......: %08lx\n", regs->ARM_r1);
        debugp("ARM_R1.......: %08lx\n", stack[1]);
        for (i = 0; i < 37; i++, p++)
                debugp("reg[%02d]...: %08lx\n", i, *p);
        } 
}

pid_t kernel_thread(int (*fn)(void *), void *arg, unsigned long flags)
{
	struct pt_regs regs;

//debugp("\n");
	memset(&regs, 0, sizeof(regs));

	regs.ARM_r4 = (unsigned long)arg;
	regs.ARM_r5 = (unsigned long)fn;
	regs.ARM_r6 = (unsigned long)kernel_thread_exit;
	regs.ARM_r7 = SVC_MODE | PSR_ENDSTATE;
	regs.ARM_pc = (unsigned long)kernel_thread_helper;
	regs.ARM_cpsr = regs.ARM_r7 | PSR_I_BIT;

//debugp("PC: %08lx\n", regs.ARM_pc);
	return do_fork(flags|CLONE_VM|CLONE_UNTRACED, 0, &regs, 0, NULL, NULL);
}

EXPORT_SYMBOL(kernel_thread);

extern void __back_to_user(struct pt_regs *regs);

void ret_from_fork(struct thread_info *ti_p, struct thread_info *ti_n)
{
	register unsigned long sp asm ("sp");
	struct task_struct *p = ti_p->task;
	struct task_struct *n = ti_n->task;
	struct pt_regs *regs;

	//debugp("sp: %08lx\n", sp);
	//debugp("thread_info: %p\n", ti_p);
	//debugp("task_struct: %p\n", p);
	//debugp("thread_info: %p\n", ti_n);
	//debugp("task_struct: %p\n", n);
	regs = task_pt_regs(n);
	//debugp("regs: %08lx\n", regs);
	//debugp("R4  : %08lx\n", regs->ARM_r4);
	//debugp("R5  : %08lx\n", regs->ARM_r5);
	//debugp("R6  : %08lx\n", regs->ARM_r6);
	//debugp("R7  : %08lx\n", regs->ARM_r7);
	//debugp("PC  : %08lx\n", regs->ARM_pc);
	schedule_tail(p);
	//debugp("================================\n");
	__back_to_user(regs);
	debugp("=======NEVER REACHED============\n");
	while(1);
}

extern void __switch_to2(struct thread_info *ti_p,struct thread_info *ti_n);

void __switch_to(struct task_struct *p, struct task_struct *n,struct task_struct *l)
{
	struct thread_info *ti_p, *ti_n;

	//debugp("p: %s\n", p->comm);
	//debugp("n: %s\n", n->comm);
	//debugp("l: %s\n", l->comm);
//debugp("ti(p): %p\n", task_thread_info(p));
//debugp("ti(n): %p\n", task_thread_info(n));

ti_n = task_thread_info(n);
ti_n = (struct thread_info *) task_thread_info(n);
ti_p = (struct thread_info *) task_thread_info(p);

//debugp("SP: %08lx\n", ti_p->cpu_context.sp);
//debugp("PC: %08lx\n", ti_p->cpu_context.pc);
//debugp("SP: %08lx\n", ti_n->cpu_context.sp);
//debugp("PC: %08lx\n", ti_n->cpu_context.pc);
	__switch_to2(ti_p, ti_n);
	l = p;
}

int
copy_thread(unsigned long clone_flags, unsigned long stack_start,
            unsigned long stk_sz, struct task_struct *p, struct pt_regs *regs)
{
	struct thread_info *thread = task_thread_info(p);
	struct pt_regs *childregs = task_pt_regs(p);

	//debugp("clone_flags: %08lx\n", clone_flags);
	//debugp("stack_start: %08lx\n", stack_start);
	//debugp("stk_sz     : %08lx\n", stk_sz);
	//debugp("task_struct: %p\n", p);
	//debugp("regs       : %p\n", regs);
	//debugp("thread     : %p\n", thread);
	//debugp("childregs  : %p\n", childregs);

	*childregs = *regs;
	childregs->ARM_r0 = 0;
	childregs->ARM_sp = stack_start;

	memset(&thread->cpu_context, 0, sizeof(struct cpu_context_save));
	thread->cpu_context.sp = (unsigned long)childregs;
	thread->cpu_context.pc = (unsigned long)ret_from_fork;

	if (clone_flags & CLONE_SETTLS)
		thread->tp_value = regs->ARM_r3;

	return 0;
}

void exit_thread(void)
{
	debugp("current: %d - %s\n", current->pid, current->comm);
/* TBD Install a thread notifier */
	//while(1);
}


unsigned long get_wchan(struct task_struct *p)
{
	debugp("\n");
	while(1);
	return 0;
}

void flush_thread(void)
{
	struct thread_info *thread = current_thread_info();
	struct task_struct *tsk = current;

	memset(thread->used_cp, 0, sizeof(thread->used_cp));
	memset(&tsk->thread.debug, 0, sizeof(struct debug_info));
	memset(&thread->fpstate, 0, sizeof(union fp_state));
}
void release_thread(struct task_struct *dead_task)
{
	//debugp("\n");
	//while(1);
}
/*
struct task_struct *__switch_to(struct task_struct *prev, struct task_struct *next)
{
debugp("from %p to %p\n", prev, next);
debugp("from %s to %s\n", prev->comm, next->comm);
while(1);
	return NULL;
}
*/
/*
unsigned long arch_randomize_brk(struct mm_struct *mm)
{
	unsigned long range_end = mm->brk + 0x02000000;
	return randomize_range(mm->brk, range_end, 0) ? : mm->brk;
}
*/

