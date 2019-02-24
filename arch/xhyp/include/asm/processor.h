/*
 *  arch/arm/include/asm/processor.h
 *
 *  Copyright (C) 1995-1999 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Copyright (C) 2012-2013 Pierre Morel
 *  	Changes for adaptatoin to a virtual ARM
 *  	mostly add processor flags here
 */

#ifndef __ASM_XHYP_PROCESSOR_H
#define __ASM_XHYP_PROCESSOR_H

/*
 * Processor flags
 */
#define USR_MODE	0x00000010
#define FIQ_MODE	0x00000011
#define IRQ_MODE	0x00000012
#define SVC_MODE	0x00000013
#define ABT_MODE	0x00000017
#define UND_MODE	0x0000001b
#define SYSTEM_MODE	0x0000001f
#define MODE_MASK	0x0000001f

#define PSR_T_BIT	0x00000020

#define PSR_F_BIT	0x00000040
#define PSR_I_BIT	0x00000080
#define IRQ_MASK	0x000000c0

#define PSR_A_BIT	0x00000100
#define PSR_E_BIT	0x00000200
#define PSR_J_BIT	0x01000000
#define PSR_Q_BIT	0x08000000
#define PSR_V_BIT	0x10000000
#define PSR_C_BIT	0x20000000
#define PSR_Z_BIT	0x40000000

/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 */
#define current_text_addr() ({ __label__ _l; _l: &&_l;})

#ifndef __KERNEL__
struct pt_regs {
        long uregs[18];
};
#else /* __KERNEL__ */

#include <asm/ptrace.h>
#include <asm/types.h>

#define STACK_TOP	((current->personality & ADDR_LIMIT_32BIT) ? \
			 TASK_SIZE : TASK_SIZE_26)
#define STACK_TOP_MAX	TASK_SIZE

struct debug_info {
#ifdef CONFIG_HAVE_HW_BREAKPOINT
	struct perf_event	*hbp[ARM_MAX_HBP_SLOTS];
#endif
};

struct thread_struct {
							/* fault info	  */
	unsigned long		address;
	unsigned long		trap_no;
	unsigned long		error_code;
							/* debugging	  */
	struct debug_info	debug;
};


#define INIT_SP	 (sizeof(init_stack) + (unsigned long) &init_stack)

#define INIT_THREAD  { }

#ifdef CONFIG_MMU
#define nommu_start_thread(regs) do { } while (0)
#else
#define nommu_start_thread(regs) regs->ARM_r10 = current->mm->start_data
#endif

extern void start_thread(struct pt_regs *regs,unsigned long pc,unsigned long sp);

/* Forward declaration, a strange C thing */
struct task_struct;

/* Free all resources held by a thread. */
extern void release_thread(struct task_struct *);

unsigned long get_wchan(struct task_struct *p);

#if __LINUX_ARM_ARCH__ == 6 || defined(CONFIG_ARM_ERRATA_754327)
#define cpu_relax()			smp_mb()
#else
#define cpu_relax()			barrier()
#endif

/*
 * Create a new kernel thread
 */
extern int kernel_thread(int (*fn)(void *), void *arg, unsigned long flags);

#define task_pt_regs(p) \
	((struct pt_regs *)(THREAD_START_SP + task_stack_page(p)) - 1)

#define KSTK_EIP(tsk)	task_pt_regs(tsk)->ARM_pc
#define KSTK_ESP(tsk)	task_pt_regs(tsk)->ARM_sp

/*
 * Prefetching support - only ARMv5.
 */
#if __LINUX_ARM_ARCH__ >= 5

#define ARCH_HAS_PREFETCH
static inline void prefetch(const void *ptr)
{
	__asm__ __volatile__(
		"pld\t%a0"
		:
		: "p" (ptr)
		: "cc");
}

#define ARCH_HAS_PREFETCHW
#define prefetchw(ptr)	prefetch(ptr)

#define ARCH_HAS_SPINLOCK_PREFETCH
#define spin_lock_prefetch(x) do { } while (0)

#endif /* MMU */

#define HAVE_ARCH_PICK_MMAP_LAYOUT

#endif /* KERNEL */

#endif /* __ASM_XHYP_PROCESSOR_H */
