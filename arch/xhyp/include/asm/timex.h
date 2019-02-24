/*
 * OpenRISC Linux
 *
 * Linux architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * OpenRISC implementation:
 * Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __ASM_OPENRISC_TIMEX_H
#define __ASM_OPENRISC_TIMEX_H

#define get_cycles get_cycles
#define ARCH_HAS_READ_CURRENT_TIMER

extern int read_current_timer(unsigned long *);

#include <asm-generic/timex.h>
#include <linux/timex.h>

/* TBD */
static inline cycles_t get_cycles(void)
{
	cycles_t c;
	return read_current_timer(&c) ? 0 : c;
}

/* This isn't really used any more */
//#define CLOCK_TICK_RATE 1000


#endif
