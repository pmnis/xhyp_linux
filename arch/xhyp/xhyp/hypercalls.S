/* arch/arm/mach-xhyp/hypercalls.S
 *
 * Copyright (C) 2012 MNIS.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/linkage.h>
#include <asm/assembler.h>
#include <xhyp/hyp.h>

ENTRY(xhyp_console)
        swi	#_HYP_console
        mov     pc, lr

ENTRY(xhyp_proc_fin)
        mov     pc, lr

ENTRY(xhyp_proc_init)
        mov     pc, lr

ENTRY(xhyp_yield)
        swi	#_HYP_yield
        mov     pc, lr

ENTRY(xhyp_get_timer)
        swi	#_HYP_get_timer
        mov     pc, lr

ENTRY(xhyp_syscall)
	swi	#_HYP_syscall

/*
ENTRY(xhyp_getpage)
        swi	#_HYP_getpage
        mov     pc, lr

ENTRY(xhyp_freepage)
        swi	#_HYP_freepage
        mov     pc, lr
*/

ENTRY(xhyp_irq_request)
	swi	#_HYP_irq_request
	mov	pc, lr

ENTRY(xhyp_irq_enable)
	swi	#_HYP_irq_enable
	mov	pc, lr

ENTRY(xhyp_irq_disable)
	swi	#_HYP_irq_disable
	mov	pc, lr

ENTRY(xhyp_irq_return)
	swi	#_HYP_irq_return

ENTRY(xhyp_return_to_user)
	swi	#_HYP_return_to_user
	nop
	nop
	ldmia	sp, {r0-r13}
	add	sp, sp, #60
	mov	pc, sp

ENTRY(xhyp_exit)
	swi	#_HYP_exit
	mov	pc, lr

ENTRY(xhyp_cpuctrl)
	swi     #_HYP_cpuctrl
	mov	pc, lr

ENTRY(xhyp_cpuctrl_ext)
	swi     #_HYP_cpuctrl
	mov	pc, lr

ENTRY(xhyp_syscall_request)
	swi	#_HYP_syscall_request
	mov	pc, lr

ENTRY(xhyp_syscall_return)
	swi	#_HYP_syscall_return
	mov	pc, lr

ENTRY(xhyp_pgfault_request)
	swi     #_HYP_pgfault_request
	mov	pc, lr

ENTRY(xhyp_abrt_ret)
	swi     #_HYP_abt_return
	mov	pc, lr

ENTRY(xhyp_undef_request)
	swi	#_HYP_undef_request

ENTRY(xhyp_setmode)
	swi     #_HYP_setmode
	mov	pc, lr

ENTRY(xhyp_reset)
	swi     #_HYP_reset
	mov	pc, lr

ENTRY(xhyp_idle)
	swi     #_HYP_idle
	mov	pc, lr

ENTRY(xhyp_set_domain)
	swi     #_HYP_set_domain
	mov	pc, lr

ENTRY(xhyp_probe)
	swi     #_HYP_probe
	mov	pc, lr

/*
 * CACHE ENTRIES
*/

ENTRY(xhyp_disable_cache)
	swi     #_HYP_disable_cache
	mov	pc, lr

ENTRY(xhyp_flush_user_cache_all)
	swi     #_HYP_flush_cache
	mov	pc, lr

ENTRY(xhyp_flush_cache)
	swi     #_HYP_flush_cache
	mov	pc, lr

ENTRY(xhyp_flush_kern_cache_all)
	swi     #_HYP_flush_cache
	mov	pc, lr

ENTRY(xhyp_flush_icache)
	swi     #_HYP_flush_icache
	mov	pc, lr

ENTRY(xhyp_flush_user_cache_range)
	swi     #_HYP_flush_user_cache_range
	mov	pc, lr

ENTRY(xhyp_flush_kern_dcache_area)
	swi     #_HYP_flush_kern_dcache_area
	mov	pc, lr

ENTRY(xhyp_dcache_clean_area)
	swi     #_HYP_dcache_clean_area
	mov	pc, lr

/*
 * DMA ENTRIES
*/

ENTRY(xhyp_coherent_kern_range)
	mov	r0, #3
	swi     #_HYP_idle
	mov	pc, lr

ENTRY(xhyp_coherent_user_range)
	swi     #_HYP_coherent_user_range
	mov	pc, lr

ENTRY(xhyp_dma_inv_range)
	swi     #_HYP_dma_inv_range
	mov	pc, lr

ENTRY(xhyp_dma_clean_range)
	swi     #_HYP_dma_clean_range
	mov	pc, lr

ENTRY(xhyp_dma_flush_range)
	swi     #_HYP_dma_flush_range
	mov	pc, lr

ENTRY(xhyp_dma_map_area)
	mov	r0, #4
	swi     #_HYP_idle
	mov	pc, lr

ENTRY(xhyp_dma_unmap_area)
	mov	r0, #5
	swi     #_HYP_idle
	mov	pc, lr

/*
 * PAGE TABLE ENTRIES
*/

ENTRY(xhyp_enable_mmu)
	swi     #_HYP_enable_mmu
	mov	pc, lr

ENTRY(xhyp_set_pte)
	swi     #_HYP_set_pte
	mov	pc, lr


ENTRY(xhyp_set_pmd)
	swi     #_HYP_set_pmd
	mov	pc, lr

ENTRY(xhyp_get_pmdval)
	swi     #_HYP_get_pmdval
	mov	pc, lr

ENTRY(xhyp_switch_mm)
	swi     #_HYP_switch_mm
	mov	pc, lr

/*
 * TLB ENTRIES
*/

ENTRY(xhyp_tlb)
	swi     #_HYP_tlb
	mov	pc, lr

ENTRY(_xhyp_set_pte_ext)
	swi	#_HYP_set_pte_ext
	mov	pc, lr

ENTRY(xhyp_flush_kern_tlb_range)
	swi	#_HYP_tlb
	mov	pc, lr

ENTRY(xhyp_flush_user_tlb_range)
	swi	#_HYP_tlb
	mov	pc, lr

/*
 * USER ENTRIES
*/

ENTRY(_xhyp_copy_user_page)
	swi	#_HYP_copy_page
	mov	pc, lr

ENTRY(_xhyp_clear_user_page)
	swi	#_HYP_clear_page
	mov	pc, lr

