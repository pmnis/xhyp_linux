/*
 * mm/fault.c
 *
 * Fault handling code
 *
 * Author: Pierre Morel <pmorel@mnis.fr>
 *
 * $LICENSE:
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <linux/module.h>
#include <linux/signal.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/signal.h>
#include <linux/io.h>
#include <linux/sched.h>

#include <asm/processor.h>
#include <xhyp/xhyp.h>

#define FSR_WRITE (1 << 11)

extern int do_page_fault(struct pt_regs *regs, unsigned long address,
			unsigned long write);

void do_DataAbort(unsigned long addr, unsigned int fsr)
{
	struct pt_regs *regs;
	int write = fsr & FSR_WRITE;
	register unsigned long sp asm ("sp");

	debugp("fsr: 0x%03x at addr 0x%08lx\n",fsr, addr);

	debugp("sp: %08lx, xhyp_sp: %08lx\n", sp, xhyp_sp);
	debugp("current->threadinfo: %p\n", current_thread_info());

	regs = (struct pt_regs *)&xhyp_sp->ctx;

	debugp("regs at %p cpsr = %08lx\n", regs, regs->ARM_cpsr);

//debugp("PHYS_OFFSET: %08lx\n", PHYS_OFFSET);
//debugp("PAGE_OFFSET: %08lx\n", PAGE_OFFSET);
//debugp("Shared page: %08lx\n", xhyp_sp);
//debugp("current: %p\n", current);

	do_page_fault(regs, addr, write);

	xhyp_abrt_ret();
debugp("WAIT\n");
while(1);
}
/*
void do_PrefetchAbort(unsigned long addr, unsigned int ifsr, struct pt_regs *regs)
{
	struct siginfo info;

	printk(KERN_ALERT "Unhandled prefetch abort: %s (0x%03x) at 0x%08lx\n",
		"PrefetchError", ifsr, addr);

	info.si_signo = SIGSEGV;
	info.si_errno = 0;
	info.si_code  = 0;
	info.si_addr  = (void __user *)addr;
	xhyp_notify_die("", regs, &info, ifsr, 0);
}
*/
