#include <linux/module.h>
#include <linux/sched.h>
#include <asm/tlb.h>
#include <asm/tlbflush.h>

void flush_tlb_range(struct vm_area_struct *vma,
                     unsigned long start, unsigned long end)
{
	debugp("start %08lx end %08lx\n", start, end);
}

void flush_tlb_page(struct vm_area_struct *vma, unsigned long uaddr)
{
	debugp("uaddr %08lx\n", uaddr);
}

void flush_tlb_mm(struct mm_struct *mm)
{
	debugp("mm %p mm->pgd %p\n", mm, mm->pgd);
}

void __tlb_remove_tlb_entry(struct mmu_gather *tlb, pte_t * ptep, unsigned long address)
{
	debugp("\n");
}

void tlb_start_vma(struct mmu_gather *tlb, struct vm_area_struct *vma)
{
	debugp("tlb %p vma %p\n", tlb, vma);
}
void tlb_end_vma(struct mmu_gather *tlb, struct vm_area_struct *vma)
{
	debugp("tlb %p vma %p\n", tlb, vma);
}

void tlb_flush(struct mmu_gather *tlb)
{
	debugp("\n");
	flush_tlb_mm(tlb->mm);
}

