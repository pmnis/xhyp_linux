/*
 * irq.c
 *
 * Interruption management
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

#include <linux/irq.h>
#include <asm/processor.h>
#include <xhyp/shared_page.h>
#include <xhyp/xhyp.h>

int irqdebug = 0;

void arch_local_irq_restore(unsigned long flags)
{
	unsigned long was_masked;

	if (flags == ARCH_IRQ_DISABLED) {
		xhyp_sp->v_cpsr |= IRQ_MASK;
		return;
	}

	was_masked = xhyp_sp->v_cpsr & IRQ_MASK;
	xhyp_sp->v_cpsr &= ~IRQ_MASK;

	if (was_masked) {
		if (irqdebug)
			debugp("ENABLING IRQs %08lx\n", IRQ_MASK);
		xhyp_irq_enable(-1);
	}
}

unsigned long arch_local_save_flags(void)
{
        if (xhyp_sp->v_cpsr & IRQ_MASK)
		return ARCH_IRQ_DISABLED;
	return ARCH_IRQ_ENABLED;
}

