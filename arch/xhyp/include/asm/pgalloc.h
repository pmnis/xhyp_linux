#ifndef __ASM_XHYP_PGALLOC_H
#define __ASM_XHYP_PGALLOC_H

#include <asm/page.h>
#include <linux/threads.h>
#include <linux/mm.h>
#include <linux/memblock.h>
#include <linux/bootmem.h>

#include <xhyp/xhyp.h>

extern int mem_init_done;

#define pmd_populate_kernel(mm, pmd, pte) \
	set_pmd(pmd, __pmd(_KERNPG_TABLE + __pa(pte)))

extern void pmd_populate(struct mm_struct *mm, pmd_t *pmd, struct page *page);
extern pgd_t *pgd_alloc(struct mm_struct *mm);
extern void pgd_free(struct mm_struct *mm, pgd_t *pgd);
extern pte_t *pte_alloc_one_kernel(struct mm_struct *mm, unsigned long address);
extern struct page *pte_alloc_one(struct mm_struct *mm, unsigned long address);
extern void pte_free_kernel(struct mm_struct *mm, pte_t *pte);
extern void pte_free(struct mm_struct *mm, struct page *pte);


#define __pte_free_tlb(tlb, pte, addr) tlb_remove_page((tlb), (pte))
#define pmd_pgtable(pmd) pmd_page(pmd)

#define check_pgt_cache()          do { } while (0)

#endif

