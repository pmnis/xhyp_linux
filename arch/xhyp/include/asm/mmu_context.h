#ifndef __ASM_XHYP_MMU_CONTEXT_H
#define __ASM_XHYP_MMU_CONTEXT_H

#include <asm-generic/mm_hooks.h>

extern int init_new_context(struct task_struct *tsk, struct mm_struct *mm);
extern void destroy_context(struct mm_struct *mm);
extern void switch_mm(struct mm_struct *prev, struct mm_struct *next,
                      struct task_struct *tsk);

#define deactivate_mm(tsk, mm)  do { } while (0)

#define activate_mm(prev, next) switch_mm((prev), (next), NULL)

/* current active pgd - this is similar to other processors pgd
 *  * registers like cr3 on the i386
 *   */

extern volatile pgd_t *current_pgd;   /* defined in arch/openrisc/mm/fault.c */

static inline void enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk)
{
}

#define init_new_context(tsk,mm)        0
#define destroy_context(mm)             do { } while(0)


#endif
