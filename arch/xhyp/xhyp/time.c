/*
 *  linux/arch/xhyp/time.c
 *
 *  Copyright (C) 2012-2013 MNIS
 *
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <linux/interrupt.h>
#include <linux/memblock.h>

#include <asm/irq.h>
#include <asm/hw_irq.h>
#include <asm/cpu.h>
#include <xhyp/xhyp.h>

#include <xhyp/hypercalls.h>
#include <xhyp/shared_page.h>
#include <xhyp/xhyp.h>

static cycle_t xhyp_timer_read(struct clocksource *cs)
{
	//debugp("jiffies: %ld\n", xhyp_sp->jiffies);
	return xhyp_sp->jiffies;
}

#define NSEC_PER_XHYP_JIFFY	1000L*1000L
#define XHYP_JIFFY_SHIFT	0

struct clocksource xhyp_clocksource = {
	.name	= "xhyp_timer",
	.rating	= 200,
	.read	= xhyp_timer_read,
	.mask	= CLOCKSOURCE_MASK(32),
	.flags	= CLOCK_SOURCE_IS_CONTINUOUS,
	.mult	= NSEC_PER_XHYP_JIFFY << XHYP_JIFFY_SHIFT,
	.shift	= XHYP_JIFFY_SHIFT,
};

static int xhyp_timer_set_next_event(unsigned long cycles, struct clock_event_device *evt)
{
	register unsigned long sp asm ("sp");
	debugp("SP %08lx\n", sp);
while(1);
	return 0;
}

static void xhyp_timer_set_mode(enum clock_event_mode mode, struct clock_event_device *evt)
{
	//debugp("\n");
	evt->mode = mode;
}

struct clock_event_device xhyp_clockevent = {
	.name		= "xhyp_timer",
	.features	= CLOCK_EVT_FEAT_PERIODIC,
	.rating		= 200,
	.set_next_event	= xhyp_timer_set_next_event,
	.set_mode	= xhyp_timer_set_mode,
	.max_delta_ns	= 2*1000*1000,
	.min_delta_ns	= 1000*1000,
};

static irqreturn_t xhyp_timer_handler(int irq, void *dev_id)
{
	struct clock_event_device *evt = dev_id;
//debugp("evt: %p\n", evt);
//debugp("evt->handler: %p\n", evt->event_handler);
	if (evt->event_handler)
		evt->event_handler(evt);
	else
		debugp("No handler\n");
	return IRQ_HANDLED;
}

struct irqaction xhyp_timer_irq = {
	.name		= "xhyp_timer",
	//.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.flags		= IRQF_DISABLED | IRQF_TIMER,
	.handler	= xhyp_timer_handler,
	.dev_id		= &xhyp_clockevent,
};

void __init xhyp_timer_init(void)
{
	int retval = 0;
	register unsigned long sp asm ("sp");
	//debugp("\n");
//PMR .... TBD
	/* Register our clock source	*/
	if (clocksource_register_hz(&xhyp_clocksource, 100)) {
		debugp("ERROR\n");
		while(1);
	}
	//debugp("SP %08lx\n", sp);
	//debugp("requesting IRQ %d\n", 2);
/*
	if (setup_irq(2, &xhyp_timer_irq)) {
		printk(KERN_ERR "xhyp: setup_irq failed for %s\n",
					xhyp_clockevent.name);
		debugp("ERROR\n");
		while(1);
	}
*/
	/* Reset jiffies	*/
	//jiffies = 0L;
	/* Allocate the interrupt handler	*/
	retval = request_irq(2, xhyp_timer_handler,
			IRQF_DISABLED | IRQF_TIMER,
			"xhyp_timer", &xhyp_clockevent);
	if (retval) {
		debugp("ERROR: %d\n", retval);
		while(1);
	}
	/* Register the clock event handler	*/
	clockevents_register_device(&xhyp_clockevent);


	xhyp_irq_enable(0x10);
	xhyp_sp->v_irq_enabled |= 0x10;
	return;
}

