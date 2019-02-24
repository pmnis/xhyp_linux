/*
 * XHYP Linux
 *
 * Linux architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __ASM_XHYP_TLB_H__
#define __ASM_XHYP_TLB_H__

#define HAVE_ARCH_UNMAPPED_AREA

//#define tlb_start_vma(tlb, vma) do { } while (0)
//#define tlb_end_vma(tlb, vma) do { } while (0)
//#define __tlb_remove_tlb_entry(tlb, ptep, address) do { } while (0)

//#define tlb_flush(tlb) flush_tlb_mm((tlb)->mm)
#include <linux/pagemap.h>
#include <asm-generic/tlb.h>

extern void tlb_start_vma(struct mmu_gather *tlb, struct vm_area_struct *vma);
extern void tlb_end_vma(struct mmu_gather *tlb, struct vm_area_struct *vma);
extern void __tlb_remove_tlb_entry(struct mmu_gather *tlb, pte_t * ptep, unsigned long address);

extern void tlb_flush(struct mmu_gather *tlb);

#endif /* __ASM_XHYP_TLB_H__ */
