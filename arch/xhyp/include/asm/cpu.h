/*
 *  arch/xhyp/include/asm/cpu.h
 *
 *  Copyright (C) 2012-2013 MNIS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_XHYP_CPU_H
#define __ASM_XHYP_CPU_H

#include <linux/percpu.h>
#include <linux/cpu.h>

struct cpuinfo_xhyp {
	struct cpu      cpu;
#ifdef CONFIG_SMP
	unsigned int    loops_per_jiffy;
#endif
};

DECLARE_PER_CPU(struct cpuinfo_xhyp, cpu_data);

#endif

