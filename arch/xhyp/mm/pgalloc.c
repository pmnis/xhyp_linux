/*
 * Copyright	(C) MNIS
 */

#include <asm/page.h>
#include <linux/threads.h>
#include <linux/mm.h>
#include <linux/memblock.h>
#include <linux/bootmem.h>
#include <linux/sched.h>

#include <xhyp/xhyp.h>

#define PMD_TYPE_TABLE	0x00000011
#define PTE_TYPE_TABLE	0x00000001
#define SEC_TYPE_TABLE	0x00000012
#define PMD_DOMAIN_USR	0x00000020
#define PMD_DOMAIN_KRNL	0x00000000
#define PMD_PROT_USER	(PMD_TYPE_TABLE|PMD_DOMAIN_USR)

/*
 * pgd_alloc
 * Allocate the first level page table.
 * In XHYP-ARM TBL1 is 16k wide so 4 pages are needed
 * The new PGD will be initialized with kernel entries.
 */
pgd_t *pgd_alloc(struct mm_struct *mm)
{
	pgd_t *pgd;
	pgd_t *init_pgd;
	unsigned long *p;

	//debugp("----------------current: %d - %s\n", current->pid, current->comm);
	/* Allocate 4 pages	*/
	pgd = (pgd_t *) __get_free_pages(GFP_KERNEL, 2);
	//debugp("pgd %p\n", pgd);

	/* Zeroes the new PGD	*/
	memset(pgd, 0, PAGE_SIZE * 4);

	/* Copy kernel entries	*/
	init_pgd = pgd_offset_k(0);
	//debugp("init_pgd at %p\n", init_pgd);
	memcpy(pgd, init_pgd, PAGE_SIZE * 4);

	p = (unsigned long *)pgd;
	//debugp("xhyp 0 entries: %08lx\n", *p);
	//debugp("xhyp 1 entries: %08lx\n", *(p+1));
	p = &pgd[0xc00];
	//debugp("kernel entries: %p %08lx\n", p, *p++);
	//debugp("kernel entries: %p %08lx\n", p, *p++);
	//debugp("kernel entries: %p %08lx\n", p, *p++);
	//debugp("kernel entries: %p %08lx\n", p, *p++);
	//debugp("kernel entries: %p %08lx\n", p, *p++);
	

	return pgd;
}

/*
 * pmd_populate
 * place an entry in the PMD
 * In XHYP-ARM a PMD is a PGD entry
 * it will contain the pointer to the TBL2 in the page page
 *
 */

void pmd_populate(struct mm_struct *mm, pmd_t *pmd,
				struct page *page)
{
	unsigned long pmdval;

	debugp("pgd %p pmd %p page %p\n", mm->pgd, pmd, page);
	pmdval = page_to_phys(page) | PMD_PROT_USER;

	//debugp("pmdval: %08lx\n", pmdval);
	*pmd = __pmd(pmdval);

	xhyp_set_pmd(mm->pgd, pmdval, pmd, pmdval);
//while(1);
}

#if 0
/* FIXME: This seems to be the preferred style, but we are using
 * current_pgd (from mm->pgd) to load kernel pages so we need it
 * initialized.  This needs to be looked into.
 */
extern inline pgd_t *pgd_alloc(struct mm_struct *mm)
{
	return (pgd_t *)get_zeroed_page(GFP_KERNEL);
}
#endif

void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	//debugp("pgd: %p\n", pgd);
	free_pages(pgd, 2);
}

pte_t __init_refok *pte_alloc_one_kernel(struct mm_struct *mm,
                                         unsigned long address)
{
	debugp("\n");
	while(1);
        return NULL;
}


struct page *pte_alloc_one(struct mm_struct *mm,
					 unsigned long address)
{
	struct page *page;

	page = alloc_pages(GFP_KERNEL|__GFP_REPEAT, 0);
	//debugp("page: %p for address %08lx\n", page, address);
	if (page) {
		clear_page(page_address(page));
		pgtable_page_ctor(page);
	}
	return page;
}

void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	//debugp("pte: %p\n", pte);
	free_page(pte);
}

void pte_free(struct mm_struct *mm, struct page *pte)
{
	//debugp("page: %p\n", pte);
	__free_page(pte);
}

void set_pte_at(struct mm_struct *mm, unsigned long addr,
                                pte_t *ptep, pte_t pteval)
{
	debugp("pgd: %08lx address %08lx ptep: %08lx val %08lx\n", mm->pgd, addr, ptep, pte_val(pteval));
//while(1);
	/* Ask xhyp to setup the real PTE	*/
	xhyp_set_pte(mm->pgd, addr, ptep, pte_val(pteval));
	/* Set the fake PTE for Linux		*/
	*ptep = pteval;
	//debugp("\n");
}

pte_t __mk_pte(void *page, pgprot_t pgprot)
{
        pte_t pte;
        pte_val(pte) = __pa(page) | pgprot_val(pgprot) | PTE_VALID_BIT;
//debugp("page %p pte %08lx\n", page, pte_val(pte));
        return pte;
}

