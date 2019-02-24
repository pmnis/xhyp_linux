/*
 * hyp.h
 *
 * General hypervisor macros for assembly
 *
 * Author: Pierre Morel <pmorel@mnis.fr>
 *
 * $LICENSE:
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#ifndef _HYP_MACROS_S
#define _HYP_MACROS_S
#include <xhyp/hyp.h>

	.macro	hyp_get_cp15	reg, a, b
		mov	r5, r0
		mov	r4, r1
                mov     r0, #\a
                mov     r1, #\b
                swi     #_HYP_cpuctrl
                mov     \reg, r0
		mov	r0, r5
		mov	r1, r4
	.endm

	.macro hyp_console	addr, count
		mov	r0, \addr
		mov	r1, \count
		swi	#_HYP_console
	.endm

	.macro hyp_disable_cache
		swi     #_HYP_disable_cache
	.endm

	.macro hyp_reset
		swi     #_HYP_reset
	.endm

	.macro hyp_idle
		mov	r0, #123
		swi     #_HYP_idle
	.endm

	.macro	hyp_flush_cache
		swi     #_HYP_flush_cache
	.endm

	.macro	hyp_flush_icache
		swi     #_HYP_flush_icache
	.endm

	.macro hyp_flush_user_cache_range
		swi	#_HYP_flush_user_cache_range
	.endm

	.macro hyp_coherent_user_range
		swi	#_HYP_coherent_user_range
	.endm

	.macro hyp_flush_kern_dcache_area
		swi	#_HYP_flush_kern_dcache_area
	.endm

	.macro hyp_dma_inv_range
		swi	#_HYP_dma_inv_range
	.endm

	.macro hyp__dma_clean_range
		swi	#_HYP_dma_clean_range
	.endm

	.macro hyp_dma_flush_range
		swi	#_HYP_dma_flush_range
	.endm

	.macro hyp_dcache_clean_area
		swi	#_HYP_dcache_clean_area
	.endm

	.macro hyp_switch_mm
		swi	#_HYP_switch_mm
	.endm

	.macro hyp_set_pte
		swi	#_HYP_set_pte
	.endm

	.macro hyp_enable_mmu
		swi	#_HYP_enable_mmu
	.endm

	.macro hyp_setmode
		swi	#_HYP_setmode
	.endm

	.macro hyp_set_domain
		swi	#_HYP_set_domain
	.endm

	.macro hyp_set_tls
	.endm

#endif
