/*
 *  linux/arch/xhyp/xhyp.c
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

#include <xhyp/hypercalls.h>
#include <xhyp/shared_page.h>
#include <xhyp/xhyp.h>

int xhyp_debug = 1;

void __init xhyp_map_io(void)
{
	debugp("\n");
	return;
}

void __init xhyp_init_early(void)
{
	debugp("\n");
	return;
}


void xhyp_set_pte_ext(pte_t *ptep, pte_t pteval, unsigned int ext)
{
	debugp("ptep %p pteval %08lx ext %08x\n", ptep, pte_val(pteval), ext);
while(1);
}


unsigned long irq_stack[1024];

void xhyp_show_it(void)
{
	early_printk("%s [%d]: in_irq  %08lx irq_disabled %d \n", __func__, __LINE__, in_irq(), irqs_disabled());
	early_printk("%s [%d]: CPSR	 : %08lx \n", __func__, __LINE__, xhyp_sp->v_cpsr);
	early_printk("%s [%d]: v_irq_enabled: %08lx\n", __func__, __LINE__, xhyp_sp->v_irq_enabled);
	early_printk("%s [%d]: v_irq_pending: %08lx\n", __func__, __LINE__, xhyp_sp->v_irq_pending);
	early_printk("%s [%d]: v_irq_acked  : %08lx\n", __func__, __LINE__, xhyp_sp->v_irq_acked);
	early_printk("%s [%d]: v_irq_mask   : %08lx\n", __func__, __LINE__, xhyp_sp->v_irq_mask);
}

static void xhyp_irq_handler(unsigned long mask)
{
	int irq = 0;

	irq_enter();
/* TBD: Interrupts must be handle in the thread stack or in a special stack */

	while (mask >>= 1) irq++;

	//debugp("in_irq  %08lx irq_disabled %d \n", in_irq(), irqs_disabled());
	//debugp("irq %d jiffies %lu\n", irq, jiffies);
	if (!(xhyp_sp->v_cpsr & PSR_I_BIT)) {
		xhyp_show_it();
		debugp("ERROR\n");
		while(1);
	}
	//debugp("before generic_handle_irq\n");
	generic_handle_irq(irq);

	//debugp("before irq_exit\n");
	irq_exit();
	//debugp("end\n");
	xhyp_irq_return(0);
}

static void xhyp_irq_ack(struct irq_data *d)
{
	//debugp("\n");
	xhyp_sp->v_irq_acked |= (1 << d->irq);
}

static void xhyp_irq_mask(struct irq_data *d)
{
	//debugp("\n");
	xhyp_sp->v_irq_mask |= (1 << d->irq);
}

static void xhyp_irq_unmask(struct irq_data *d)
{
	//debugp("\n");
	xhyp_sp->v_irq_mask &= ~(1 << d->irq);
}

static int xhyp_irq_set_type(struct irq_data *d, unsigned int flow_type)
{
	debugp("\n");
	return 0;
}

static struct irq_chip xhyp_irq_chip = {
	.name = "xhyp",
	.irq_ack = xhyp_irq_ack,
	.irq_mask = xhyp_irq_mask,
	.irq_unmask = xhyp_irq_unmask,
	.irq_set_type = xhyp_irq_set_type,
};

void __init xhyp_init_irq(void)
{
	int i;

//debugp("NR_IRQS_LEGACY: %d\n", NR_IRQS_LEGACY);
	xhyp_irq_request(xhyp_irq_handler, irq_stack+512);
	for (i = 0; i < NR_IRQS_LEGACY; i++) {
		irq_set_chip_and_handler(i, &xhyp_irq_chip, handle_level_irq);
		irq_set_status_flags(i, IRQ_LEVEL);
		//irq_modify_status(i, IRQ_NOREQUEST, ~IRQ_NOREQUEST);
		//irq_set_percpu_devid_flags(i);
	}
//debugp("\n");
	return;
}

static void __init xhyp_reserve(void)
{
	__memblock_dump_all();
	//debugp("at: %08x size %08x\n", 0x00000000, 0x00100000);
	memblock_reserve(0x00000000, 0x00100000); /* reserve xhyp */
	__memblock_dump_all();
	//debugp("at: %08x size %08x\n", 0x00100000, 0x00200000);
	memblock_reserve(0x00100000, 0x00200000); /* reserve shared page */
	//debugp("CURRENT %p\n", current);
	__memblock_dump_all();
	return;
}

struct shared_page *xhyp_sp = XHYP_SHARED_PAGE_ADDR;

static int  __init xhyp_init(void)
{
debugp("\n");
//while(1);
	//xhyp_reserve();
	return 0;
}
arch_initcall(xhyp_init);

static int __init xhyp_init_late(void)
{
debugp("\n");
//while(1);
	//xhyp_reserve();
	return 0;
}
late_initcall(xhyp_init_late);

DEFINE_PER_CPU(struct cpuinfo_xhyp, cpu_data);

static int __init xhyp_topology_init(void)
{
	int cpu;
debugp("\n");
//while(1);
/*
	for_each_possible_cpu(cpu) {
		struct cpuinfo_xhyp *cpuinfo = &per_cpu(cpu_data, cpu);
		cpuinfo->cpu.hotpluggable = 1;
		register_cpu(&cpuinfo->cpu, cpu);
	}
*/
	return 0;
}

subsys_initcall(xhyp_topology_init);

