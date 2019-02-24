/*
 * hyp.h
 *
 * General hypervisor definitions
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


#define _HYP_syscall		0
#define _HYP_console		1
#define _HYP_yield		2
#define _HYP_getpage		3
#define _HYP_freepage		4
#define _HYP_irq_request	5
#define _HYP_irq_enable		6
#define _HYP_irq_disable	7
#define _HYP_irq_return		8
#define _HYP_exit		9
#define _HYP_cpuctrl		10
#define _HYP_syscall_request	11
#define _HYP_syscall_return	12
#define _HYP_pgfault_request	13
#define _HYP_undef_request	14
#define _HYP_enable_mmu		15
#define _HYP_setmode		16
#define _HYP_set_domain		17
#define _HYP_disable_cache	18
#define _HYP_reset		19
#define _HYP_idle		20
#define _HYP_flush_cache		21
#define _HYP_flush_icache		22
#define _HYP_flush_user_cache_range	23
#define _HYP_coherent_user_range	24
#define _HYP_flush_kern_dcache_area	25
#define _HYP_dma_inv_range		26
#define _HYP_dma_clean_range		27
#define _HYP_dma_flush_range		28
#define _HYP_dcache_clean_area		29
#define _HYP_switch_mm		30
#define _HYP_set_pte		31
#define _HYP_tlb		32
#define _HYP_enable_cache	33
#define _HYP_probe		34
#define _HYP_task_switch	35
#define _HYP_copy_page		36
#define _HYP_clear_page		37
#define _HYP_new_pgd		38
#define _HYP_set_pmd		39
#define _HYP_set_pte_ext	40
#define _HYP_get_pmdval		41
#define _HYP_get_timer		42
#define _HYP_io_open                    43
#define _HYP_io_read                    44
#define _HYP_io_write                   45
#define _HYP_io_ioctl                   46
#define _HYP_io_close                   47
#define _HYP_event_send                 48
#define _HYP_preempt_disable            49
#define _HYP_preempt_enable             50
#define _HYP_hyp                        51
#define _HYP_abt_return                 52
#define _HYP_return_to_user		53

#define _HYP_CALLS                      54



