/*
 * arch/xhyp/include/asm/pgtable.h
 *
 * Copyright (C) 1995-2003 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef ASM_PGTABLE_H
#define ASM_PGTABLE_H

#define __HAVE_ARCH_ENTER_LAZY_MMU_MODE
#define __HAVE_ARCH_START_CONTEXT_SWITCH
#define HAVE_ARCH_UNMAPPED_AREA
#define HAVE_ARCH_UNMAPPED_AREA_TOPDOWN

// TBD: #define __HAVE_PHYS_MEM_ACCESS_PROT


#include <asm-generic/pgtable-nopmd.h>
#include <asm-generic/pgtable-nopud.h>
#include <asm/page.h>

#define PTRS_PER_PTE	0x100
#define PTRS_PER_PMD	1
#define PTRS_PER_PGD	0x1000


#define PAGE_KERNEL	__pgprot(0x0001)

#define _PAGE_CHG_MASK 	0

#define _PAGE_BASE	0
#define _PAGE_SRE	0
#define _PAGE_SWE	0
#define _PAGE_ACCESSED	0
#define _PAGE_DIRTY	0

#define kern_addr_valid(addr)	(1)

#define _KERNPG_TABLE \
	(_PAGE_BASE | _PAGE_SRE | _PAGE_SWE | _PAGE_ACCESSED | _PAGE_DIRTY)

#define VMALLOC_OFFSET	  (8*1024*1024)
#define VMALLOC_START	   (((unsigned long)high_memory + VMALLOC_OFFSET) & ~(VMALLOC_OFFSET-1))
#define VMALLOC_END	     0xff000000UL
#define LIBRARY_TEXT_START      0x0c000000

#define PGDIR_SHIFT	     20

#define PGDIR_SIZE	      (1UL << PGDIR_SHIFT)
#define PGDIR_MASK	      (~(PGDIR_SIZE-1))

#ifndef __ASSEMBLY__

extern pgd_t swapper_pg_dir[PTRS_PER_PGD];

#include <asm/mmu.h>
#include <asm/fixmap.h>

#include <xhyp/xhyp.h>

static inline void arch_start_context_switch(struct task_struct *prev)
{
	//debugp("\n");
	//if (prev)
		//debugp("prev: %p\n", prev);
}

static inline void arch_enter_lazy_mmu_mode(void)
{
	debugp("\n");
}

static inline void arch_leave_lazy_mmu_mode(void)
{
	debugp("\n");
}

static inline void arch_flush_lazy_mmu_mode(void)
{
	debugp("\n");
}


static inline void update_mmu_cache(struct vm_area_struct *vma,
	unsigned long address, pte_t *pte)
{
	debugp("\n");
}


#define pgd_index(address)      ((address >> PGDIR_SHIFT) & (PTRS_PER_PGD-1))

#define __pgd_offset(address)   pgd_index(address)

#define pgd_offset(mm, address) ((mm)->pgd + pgd_index(address))

#define pgd_offset_k(address) pgd_offset(&init_mm, address)

#define __pmd_offset(address) \
	(((address) >> PMD_SHIFT) & (PTRS_PER_PMD-1))


/*
 * Bit maps of the SECTIONS
 * 0000 0000 0000 0rwx _ dsa0 000D DDDB C010
 * BC: Buffered Cached
 * DDDD: domain
 * a: Accessed bit
 * s: Special bit
 * d: Dirty bit
 * x: Exec bit
 * w: write bit
 * r: read bit
 */
#define SEC_VALID_BIT		0x00000001
#define SEC_TYPE		0x00000002
#define SEC_CACHE_BIT		0x00000008
#define SEC_BUFFER_BIT		0x00000010
#define SEC_DOMAIN_MASK		0x000001E0
#define SEC_ACCESS_BIT		0x00002000
#define SEC_SPECIAL_BIT		0x00004000
#define SEC_DIRTY_BIT		0x00008000
#define SEC_EXEC_BIT		0x00010000
#define SEC_WRITE_BIT		0x00020000
#define SEC_READ_BIT		0x00040000

#define SEC_RW			SEC_WRITE_BIT|SEC_READ_BIT
#define SEC_KERNEL		SEC_VALID_BIT|SEC_EXEC_BIT|SEC_RW|SEC_TYPE
/*
 * Bit maps of the PMD
 * 0000 0000 0000 0000 _ 0000 000D DDD0 0001
 */
#define PMD_VALID_BIT		0x00000001
#define PMD_DOMAIN_MASK		0x000001e0

#define pmd_none(pmd)		(!pmd_val(pmd))
#define pmd_bad(pmd)		(!(pmd_val(pmd)&PMD_VALID_BIT))
#define pmd_present(pmd)	(pmd_val(pmd))
#define pmd_clear(xp)		do { pmd_val(*(xp)) = 0; } while (0)


#define pmd_page(pmd)	   (pfn_to_page(pmd_val(pmd) >> PAGE_SHIFT))
#define pmd_page_kernel(pmd)    ((unsigned long) __va(pmd_val(pmd) & PAGE_MASK))
#define set_pmd(pmdptr, pmdval) (*(pmdptr) = pmdval)



#define __pte_offset(address)		   \
	(((address) >> PAGE_SHIFT) & (PTRS_PER_PTE - 1))
#define pte_offset_kernel(dir, address)	 \
	((pte_t *) pmd_page_kernel(*(dir)) +  __pte_offset(address))
#define pte_offset_map(dir, address)	    \
	((pte_t *)page_address(pmd_page(*(dir))) + __pte_offset(address))
#define pte_offset_map_nested(dir, address)     \
	pte_offset_map(dir, address)

/*
#define __pte_map(pmd)	  pmd_page_vaddr(*(pmd))
#define __pte_unmap(pte)	do { } while (0)
#define pte_unmap(pte)			__pte_unmap(pte)
*/

#define pte_unmap(pte)			do { } while (0)

#define pte_pfn(x)	      ((unsigned long)(((x).pte)) >> PAGE_SHIFT)
#define pfn_pte(pfn, prot)  __pte((((pfn) << PAGE_SHIFT)) | pgprot_val(prot))


#define pte_offset_map(dir, address)	    \
	((pte_t *)page_address(pmd_page(*(dir))) + __pte_offset(address))
#define pte_offset_map_nested(dir, address)     \
	pte_offset_map(dir, address)

/*
static inline pte_t __mk_pte(void *page, pgprot_t pgprot)
{
	pte_t pte;
	pte_val(pte) = __pa(page) | pgprot_val(pgprot);
	return pte;
}
*/
extern pte_t __mk_pte(void *page, pgprot_t pgprot);

#define mk_pte(page, pgprot) __mk_pte(page_address(page), (pgprot))

#define mk_pte_phys(physpage, pgprot) \
({								      \
	pte_t __pte;						    \
									\
	pte_val(__pte) = (physpage) + pgprot_val(pgprot);	       \
	__pte;							  \
})


static inline unsigned long __pte_page(pte_t pte)
{
	/* the PTE contains a physical address */
	return (unsigned long)__va(pte_val(pte) & PAGE_MASK);
}

#define pte_pagenr(pte)	 ((__pte_page(pte) - PAGE_OFFSET) >> PAGE_SHIFT)

/* permanent address of a page */

#define __page_address(page) (PAGE_OFFSET + (((page) - mem_map) << PAGE_SHIFT))
#define pte_page(pte)	   (mem_map+pte_pagenr(pte))

static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
	pte_val(pte) = (pte_val(pte) & _PAGE_CHG_MASK) | pgprot_val(newprot);
	return pte;
}


/*
 * ZERO_PAGE is a global shared page that is always zero: used
 * for zero-mapped memory areas etc..
 */
extern void *empty_zero_page;
#define ZERO_PAGE(vaddr)	(virt_to_page(empty_zero_page))

/*
 * Bit maps of the PTE
 * 0000 0000 0000 0000 _ 0000 RWED sASF BCVV
 * V: Valid bits: 00 invalid 01 SMALL PAGE
 * BC: Buffered Cached
 * F: File bit
 * S: SWAP bit
 * A: Accessed bit
 * s: Special bit
 * D: Dirty bit
 * E: Exec bit
 * W : write bit
 * R : read bit
 */
#define PTE_VALID_BIT		0x00000001
#define PTE_FILE_BIT		0x00000002
#define PTE_SWAP_BIT		0x00000004
#define PTE_CACHE_BIT		0x00000010
#define PTE_BUFFER_BIT		0x00000020
#define PTE_ACCESS_BIT		0x00000040
#define PTE_SPECIAL_BIT		0x00000080
#define PTE_DIRTY_BIT		0x00000100
#define PTE_EXEC_BIT		0x00000200
#define PTE_WRITE_BIT		0x00000400
#define PTE_READ_BIT		0x00000800

#define pte_present(pte)	(pte_val(pte) & PTE_VALID_BIT)
#define pte_none(pte)		(!pte_val(pte))
#define pte_file(pte)		(pte_val(pte) & PTE_FILE_BIT)
#define pte_swap(pte)		(pte_val(pte) & PTE_SWAP_BIT)
#define pte_write(pte)		(pte_val(pte) & PTE_WRITE_BIT)
#define pte_dirty(pte)		(pte_val(pte) & PTE_DIRTY_BIT)
#define pte_young(pte)		(!(pte_val(pte) & PTE_ACCESS_BIT))
#define pte_exec(pte)		(pte_val(pte) & PTE_EXEC_BIT)
#define pte_special(pte)	(pte_val(pte) & PTE_SPECIAL_BIT)

static inline pte_t pte_mkwrite(pte_t pte) 
	{ pte_val(pte) |= PTE_WRITE_BIT; return pte;}
static inline pte_t pte_mkold(pte_t pte) 
	{ pte_val(pte) &= ~PTE_ACCESS_BIT; return pte;}
static inline pte_t pte_mkyoung(pte_t pte) 
	{ pte_val(pte) |= PTE_ACCESS_BIT; return pte;}
static inline pte_t pte_wrprotect(pte_t pte) 
	{ pte_val(pte) &= ~PTE_WRITE_BIT; return pte;} 
static inline pte_t pte_rdprotect(pte_t pte) 
	{ pte_val(pte) &= ~PTE_READ_BIT; return pte;} 
static inline pte_t pte_mkspecial(pte_t pte) 
	{ pte_val(pte) |= PTE_DIRTY_BIT; return pte;}
static inline pte_t pte_mkdirty(pte_t pte) 
	{ pte_val(pte) |= PTE_DIRTY_BIT; return pte;}
static inline pte_t pte_mkclean(pte_t pte) 
	{ pte_val(pte) &= ~PTE_DIRTY_BIT; return pte;}



#define PTE_FILE_MAX_BITS       29

#define pte_to_pgoff(x)		(pte_val(x) >> 3)
#define pgoff_to_pte(x)		__pte(((x) << 3) | PTE_FILE_BIT)


#define __SWP_TYPE_SHIFT	3
#define __SWP_TYPE_BITS	 5
#define __SWP_TYPE_MASK	 ((1 << __SWP_TYPE_BITS) - 1)
#define __SWP_OFFSET_SHIFT      (__SWP_TYPE_BITS + __SWP_TYPE_SHIFT)

#define __swp_type(x)	   (((x).val >> __SWP_TYPE_SHIFT) & __SWP_TYPE_MASK)
#define __swp_offset(x)	 ((x).val >> __SWP_OFFSET_SHIFT)
#define __swp_entry(type,offset) ((swp_entry_t) { ((type) << __SWP_TYPE_SHIFT) | ((offset) << __SWP_OFFSET_SHIFT) })

#define __pte_to_swp_entry(pte) ((swp_entry_t) { pte_val(pte) })
#define __swp_entry_to_pte(swp) ((pte_t) { (swp).val })

extern void set_pte_ext(pte_t *, pte_t, unsigned long);

#define pte_clear(mm,addr,ptep) set_pte_ext(ptep, __pte(0), 0)

extern void set_pte_at(struct mm_struct *mm, unsigned long addr,
				pte_t *ptep, pte_t pteval);

#define pagtable_cache_init()	    do { } while (0)

#include <asm-generic/pgtable.h>

#define pgtable_cache_init()	    do { } while (0)

extern void __pte_error(const char *file, int line, pte_t);
extern void __pgd_error(const char *file, int line, pgd_t);

#define pgd_ERROR(pgd)		__pgd_error(__FILE__, __LINE__, pgd)
#define pte_ERROR(pte)		__pte_error(__FILE__, __LINE__, pte)

#endif

#define __P000	__pgprot(0)
#define __P001	__pgprot(PTE_READ_BIT)
#define __P010	__pgprot(PTE_WRITE_BIT)
#define __P011	__pgprot(PTE_READ_BIT|PTE_WRITE_BIT)
#define __P100	__pgprot(PTE_EXEC_BIT)
#define __P101	__pgprot(PTE_READ_BIT|PTE_EXEC_BIT)
#define __P110	__pgprot(PTE_WRITE_BIT|PTE_EXEC_BIT)
#define __P111	__pgprot(PTE_READ_BIT|PTE_WRITE_BIT|PTE_EXEC_BIT)

#define __S000	__pgprot(0)
#define __S001	__pgprot(PTE_READ_BIT)
#define __S010	__pgprot(PTE_WRITE_BIT)
#define __S011	__pgprot(PTE_READ_BIT|PTE_WRITE_BIT)
#define __S100	__pgprot(PTE_EXEC_BIT)
#define __S101	__pgprot(PTE_READ_BIT|PTE_EXEC_BIT)
#define __S110	__pgprot(PTE_WRITE_BIT|PTE_EXEC_BIT)
#define __S111	__pgprot(PTE_READ_BIT|PTE_WRITE_BIT|PTE_EXEC_BIT)

#define USER_PTRS_PER_PGD       (TASK_SIZE/PGDIR_SIZE)
#define FIRST_USER_ADDRESS      0


#endif

