/*
 * xhyp/xhyp.h
 *
 * Xhypervisor definitions
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

#ifndef _XHYP_XHYP_H
#define _XHP_XHYP_H
#include <asm/siginfo.h>

#include <xhyp/hypercalls.h>
#include <xhyp/shared_page.h>

extern void xhyp_die(const char *str, struct pt_regs *regs, int err, int trap);
extern void xhyp_notify_die(const char *str, struct pt_regs *regs,
                struct siginfo *info, unsigned long err, unsigned long trap);

extern void do_DataAbort(unsigned long addr, unsigned int fsr);

extern void do_PrefetchAbort(unsigned long addr, unsigned int ifsr, struct pt_regs *regs);
extern void __init xhyp_timer_init(void);
extern void __init xhyp_init_irq(void);

extern unsigned long irq_stack[];

extern void setup_bootmem(void);
extern void setup_memblock(void);
extern void paging_init(void);

#define XHYP_START		0x00000000
#define XHYP_SIZE		0x02000000
#define	XHYP_SP_START		(XHYP_START + XHYP_SIZE)
#define	XHYP_SP_SIZE		0x00100000
#define XHYP_GUEST_START	(XHYP_START + XHYP_SIZE)
#define XHYP_GUEST_SIZE		0x02000000
#define XHYP_MAX_MEM		XHYP_GUEST_START + XHYP_GUEST_SIZE

#endif
