/*
 * arch/xhyp/kernel/traps.c
 *
 * trap code
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
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/hardirq.h>
#include <linux/kdebug.h>

#include <asm/pgtable.h>
#include <asm/traps.h>

#include <xhyp/xhyp.h>

void __pte_error(const char *file, int line, pte_t pte)
{
	printk("%s:%d: bad pte %08llx.\n", file, line, (long long)pte_val(pte));
}

void __pmd_error(const char *file, int line, pmd_t pmd)
{
	printk("%s:%d: bad pmd %08llx.\n", file, line, (long long)pmd_val(pmd));
}

void __pgd_error(const char *file, int line, pgd_t pgd)
{
	printk("%s:%d: bad pgd %08llx.\n", file, line, (long long)pgd_val(pgd));
}

asmlinkage void __div0(void)
{
	printk("Division by zero in kernel.\n");
	dump_stack();
}
EXPORT_SYMBOL(__div0);

static inline void dump_backtrace(struct pt_regs *regs, struct task_struct *tsk)
{
}

void dump_stack(void)
{
	dump_backtrace(NULL, NULL);
}

EXPORT_SYMBOL(dump_stack);


void show_stack(struct task_struct *tsk, unsigned long *sp)
{
	dump_backtrace(NULL, tsk);
	barrier();
}

void xhyp_die(const char *str, struct pt_regs *regs, int err, int trap)
{
	unsigned long flags;
	int ret;

	oops_enter();
	local_irq_save(flags);

	console_verbose();
	ret = notify_die(DIE_OOPS, str, regs, err, trap, SIGSEGV);

	local_irq_restore(flags);
	oops_exit();

	if (in_interrupt())
		panic("Fatal exception in interrupt");
	if (panic_on_oops)
		panic("Fatal exception");
	if (ret != NOTIFY_STOP)
		do_exit(SIGSEGV);
}

void xhyp_notify_die(const char *str, struct pt_regs *regs,
		struct siginfo *info, unsigned long err, unsigned long trap)
{
debugp("regs: %p\n", regs);
while(1);
	if (user_mode(regs)) {
debugp("\n");
while(1);
		current->thread.error_code = err;
		current->thread.trap_no = trap;

		force_sig_info(info->si_signo, info, current);
	} else {
debugp("\n");
while(1);
		xhyp_die(str, regs, err, trap);
	}
}

/*
 * Called by start_kernel
 */
void trap_init(void)
{
        xhyp_pgfault_request(do_DataAbort, irq_stack+512);
// TBD VOIR POUR LA PILE....
	//debugp("do_DataAbort %p %p\n", do_DataAbort, irq_stack+512);
}

