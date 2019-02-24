/*
 * shared_page.h
 *
 * Shared page definitions
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
#ifndef __SHARED_PAGE_H
#define __SHARED_PAGE_H

struct shadow {
	unsigned long   sp;
	unsigned long   lr;
	unsigned long   pc;
	unsigned long   spsr;
};

struct context	{
	unsigned long   regs[13];
	struct shadow   sregs;
	unsigned long   cpsr;
	unsigned long   far;    /* 72                           */
	unsigned long   dfsr;
	unsigned long   ifsr;
	unsigned long   domain;
};

struct shared_page {
	unsigned long	magic;
	unsigned long	cp15_c0;
	unsigned long	cp15_c1;
	unsigned long	v_pgd;
	struct context	ctx;
	struct shadow	sregs_usr;
	struct shadow	sregs_svc;
	struct shadow	sregs_irq;
	struct shadow	sregs_fiq;
	struct shadow	sregs_abt;
	struct shadow	sregs_und;
	struct shadow	sregs_pre;
	unsigned long 	jiffies;
	unsigned long 	irq;
	unsigned long 	last_state;
	unsigned long 	v_cpsr;
	unsigned long 	v_spsr;
	unsigned long 	v_irq_enabled;
	unsigned long 	v_irq_pending;
	unsigned long 	v_irq_acked;
	unsigned long 	v_irq_mask;
};

extern struct shared_page *xhyp_sp;
#define XHYP_SHARED_PAGE_ADDR	(struct shared_page *)0xc2000000
#define XHYP_EMPTY_ZERO_PAGE	0xc2010000;
#endif
