/*
 *  arch/arm/include/asm/memory.h
 *
 *  Copyright (C) 2000-2002 Russell King
 *  modification for nommu, Hyok S. Choi, 2004
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Note: this file should not be included by non-asm/.h files
 */
#ifndef __ASM_ARM_MEMORY_H
#define __ASM_ARM_MEMORY_H

#ifdef __KERNEL__

#include <linux/compiler.h>
#include <linux/const.h>
#include <linux/types.h>
#include <linux/sizes.h>

/*
 * Allow for constants defined here to be used from assembly code
 * by prepending the UL suffix only with actual C code compilation.
 */
#define UL(x) _AC(x, UL)

#ifdef CONFIG_MMU

/*
 * PAGE_OFFSET - the virtual address of the start of the kernel image
 * TASK_SIZE - the maximum size of a user space task.
 * TASK_UNMAPPED_BASE - the lower boundary of the mmap VM area
 */
/* TBD */
#define PHYS_OFFSET 		0
#define PAGE_OFFSET		0xC0000000
#define TASK_SIZE		PAGE_OFFSET - 0x01000000
#define TASK_UNMAPPED_BASE	PAGE_OFFSET / 3

/*
 * The maximum size of a 26-bit user space task.
 */
#define TASK_SIZE_26		UL(0x04000000)

/*
 * The module space lives between the addresses given by TASK_SIZE
 * and PAGE_OFFSET - it must be within 32MB of the kernel text.
 */
#ifndef CONFIG_THUMB2_KERNEL
#define MODULES_VADDR		(PAGE_OFFSET - 16*1024*1024)
#else
/* smaller range for Thumb-2 symbols relocation (2^24)*/
#define MODULES_VADDR		(PAGE_OFFSET - 8*1024*1024)
#endif

#if TASK_SIZE > MODULES_VADDR
#error Top of user space clashes with start of module space
#endif

/*
 * The highmem pkmap virtual space shares the end of the module area.
 */
#ifdef CONFIG_HIGHMEM
#define MODULES_END		(PAGE_OFFSET - PMD_SIZE)
#else
#define MODULES_END		(PAGE_OFFSET)
#endif

/*
 * The XIP kernel gets mapped at the bottom of the module vm area.
 * Since we use sections to map it, this macro replaces the physical address
 * with its virtual address while keeping offset from the base section.
 */
#define XIP_VIRT_ADDR(physaddr)  (MODULES_VADDR + ((physaddr) & 0x000fffff))

/*
 * Allow 16MB-aligned ioremap pages
 */
#define IOREMAP_MAX_ORDER	24

#define CONSISTENT_END		(0xffe00000UL)

#else /* CONFIG_MMU */
#error "We do not support no MMU"

#endif /* !CONFIG_MMU */

/*
 * We fix the TCM memories max 32 KiB ITCM resp DTCM at these
 * locations
 */
#ifdef CONFIG_HAVE_TCM
#define ITCM_OFFSET	UL(0xfffe0000)
#define DTCM_OFFSET	UL(0xfffe8000)
#endif

/*
 * Convert a physical address to a Page Frame Number and back
 */
#define	__phys_to_pfn(paddr)	((unsigned long)((paddr) >> PAGE_SHIFT))
#define	__pfn_to_phys(pfn)	((phys_addr_t)(pfn) << PAGE_SHIFT)

/*
 * Convert a page to/from a physical address
 *
 * TBD
#define page_to_phys(page)	(__pfn_to_phys(page_to_pfn(page)))
#define phys_to_page(phys)	(pfn_to_page(__phys_to_pfn(phys)))
 */

#ifndef __ASSEMBLY__

/*
 * Physical vs virtual RAM address space conversion.  These are
 * private definitions which should NOT be used outside memory.h
 * files.  Use virt_to_phys/phys_to_virt/__pa/__va instead.
 */
#ifndef __virt_to_phys
#define __virt_to_phys(x)	((x) - PAGE_OFFSET + PHYS_OFFSET)
#define __phys_to_virt(x)	((x) - PHYS_OFFSET + PAGE_OFFSET)
#endif
#endif /* __ASSEMBLY__ */


#ifndef __ASSEMBLY__

/*
 * PFNs are used to describe any physical page; this means
 * PFN 0 == physical address 0.
 *
 * This is the PFN of the first RAM page in the kernel
 * direct-mapped view.  We assume this is the first page
 * of RAM in the mem_map as well.
 */
#define PHYS_PFN_OFFSET	(PHYS_OFFSET >> PAGE_SHIFT)

/*
 * These are *only* valid on the kernel direct mapped RAM memory.
 * Note: Drivers should NOT use these.  They are the wrong
 * translation for translating DMA addresses.  Use the driver
 * DMA support - see dma-mapping.h.
 */
/*
static inline phys_addr_t virt_to_phys(const volatile void *x)
{
	return __virt_to_phys((unsigned long)(x));
}

static inline void *phys_to_virt(phys_addr_t x)
{
	return (void *)(__phys_to_virt((unsigned long)(x)));
}
*/

/*
 * Drivers should NOT use these either.
 */
#define __pa(x)			__virt_to_phys((unsigned long)(x))
#define __va(x)			((void *)__phys_to_virt((unsigned long)(x)))
#define pfn_to_kaddr(pfn)	__va((pfn) << PAGE_SHIFT)

/*
 * Virtual <-> DMA view memory address translations
 * Again, these are *only* valid on the kernel direct mapped RAM
 * memory.  Use of these is *deprecated* (and that doesn't mean
 * use the __ prefixed forms instead.)  See dma-mapping.h.
 */
#ifndef __virt_to_bus
#define __virt_to_bus	__virt_to_phys
#define __bus_to_virt	__phys_to_virt
#define __pfn_to_bus(x)	__pfn_to_phys(x)
#define __bus_to_pfn(x)	__phys_to_pfn(x)
#endif

/*
static inline __deprecated unsigned long virt_to_bus(void *x)
{
	return __virt_to_bus((unsigned long)x);
}

static inline __deprecated void *bus_to_virt(unsigned long x)
{
	return (void *)__bus_to_virt(x);
}
*/
/*
 * Conversion between a struct page and a physical address.
 *
 * Note: when converting an unknown physical address to a
 * struct page, the resulting pointer must be validated
 * using VALID_PAGE().  It must return an invalid struct page
 * for any physical address not corresponding to a system
 * RAM address.
 *
 *  page_to_pfn(page)	convert a struct page * to a PFN number
 *  pfn_to_page(pfn)	convert a _valid_ PFN number to struct page *
 *
 *  virt_to_page(k)	convert a _valid_ virtual address to struct page *
 *  virt_addr_valid(k)	indicates whether a virtual address is valid
 */
#define ARCH_PFN_OFFSET		PHYS_PFN_OFFSET

/*
 * TBD
 * #define virt_to_page(kaddr)	pfn_to_page(__pa(kaddr) >> PAGE_SHIFT)
 * #define virt_addr_valid(kaddr)	((unsigned long)(kaddr) >= PAGE_OFFSET && (unsigned long)(kaddr) < (unsigned long)high_memory)
*/

/*
 * Optional coherency support.  Currently used only by selected
 * Intel XSC3-based systems.
 */
#ifndef arch_is_coherent
#define arch_is_coherent()		0
#endif

#endif

#include <asm-generic/memory_model.h>

#endif
#endif
