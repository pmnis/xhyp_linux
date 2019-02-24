/*
 * XHYP arch/xhyp/kernel/time.c
 *
 * Linux architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * Modifications for the XHYP architecture:
 * Copyright (C) 2012-2013 Pierre Morel <pmorel@mnis.fr>
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#include <linux/timex.h>
#include <xhyp/xhyp.h>
#include <xhyp/shared_page.h>

/*
 * The current timer is setup by XHYP as 1000 HZ
 * we need to return ns...
 */

int read_current_timer(unsigned long *timer_val)
{
	//*timer_val = xhyp_get_timer();

	*timer_val = xhyp_sp->jiffies;
	debugp("timer_val: %08lx\n", *timer_val);
	return 0;
}

