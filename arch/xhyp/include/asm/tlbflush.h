#ifndef __ASM_XHYP_TLBFLUSH_H
#define __ASM_XHYP_TLBFLUSH_H

#include <linux/mm.h>
#include <asm/processor.h>
#include <asm/pgtable.h>
#include <asm/pgalloc.h>
#include <asm/current.h>
#include <linux/sched.h>

#include <xhyp/xhyp.h>

void flush_tlb_all(void );

void flush_tlb_mm(struct mm_struct *mm);
void flush_tlb_page(struct vm_area_struct *vma, unsigned long addr);
void flush_tlb_range(struct vm_area_struct *vma, unsigned long start, unsigned long end);


static inline void flush_tlb(void)
{
	debugp("\n");
        flush_tlb_mm(current->mm);
}

static inline void flush_tlb_kernel_range(unsigned long start,
                                          unsigned long end)
{
	debugp("\n");
        flush_tlb_range(NULL, start, end);
}

#endif
