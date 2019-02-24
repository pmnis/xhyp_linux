/*
 * XHYP mm/init.c
 *
 * Linux architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

#include <linux/export.h>
#include <linux/sched.h>
#include <linux/personality.h>
#include <linux/binfmts.h>
#include <linux/elf.h>
#include <linux/seq_file.h>
#include <linux/initrd.h>
#include <linux/memblock.h>
#include <linux/bootmem.h>
#include <linux/swap.h>

#include <asm/sections.h>

#include <xhyp/shared_page.h>
#include <xhyp/hypercalls.h>
#include <xhyp/xhyp.h>

void *empty_zero_page;
EXPORT_SYMBOL(empty_zero_page);
//extern void show_pcpu(void);

/*
extern struct list_head bdata_list;
void __init show_bdata(void)
{
	bootmem_data_t *bdata;
debugp("\n");
	list_for_each_entry(bdata, &bdata_list, list) {
		debugp("min %08lx low %08lx\n", bdata->node_min_pfn, bdata->node_low_pfn);
		debugp("last_end_off %08lx hint_idx %08lx\n", bdata->last_end_off, bdata->hint_idx);
	}
}
*/

static inline int free_area(unsigned long pfn, unsigned long end, char *s)
{
	unsigned int pages = 0, size = (end - pfn) << (PAGE_SHIFT - 10);

	for (; pfn < end; pfn++) {
		struct page *page = pfn_to_page(pfn);
		ClearPageReserved(page);
		init_page_count(page);
		__free_page(page);
		pages++;
	}

	if (size && s)
		printk(KERN_INFO "Freeing %s memory: %dK\n", s, size);

	return pages;
}

void __init mem_init(void)
{
	int codesize, reservedpages, datasize, initsize;
	int pfn;

//debugp("\n");
//show_pcpu();
	/* Setup high_memory	*/
	//debugp("Setup high_memory\n");
	max_mapnr = num_physpages = max_low_pfn;
//debugp("num_physpages: %08lx\n", num_physpages);
//debugp("max_low_pfn  : %08lx\n", max_low_pfn);
//debugp("max_mapnr    : %08lx\n", max_mapnr);
//debugp("mem_map      : %08lx\n", mem_map);
//debugp("mem_map[max] : %08lx\n", mem_map + max_mapnr);
	high_memory = (void *)__va(max_low_pfn * PAGE_SIZE);

	/* Clear zero page	*/
	empty_zero_page = XHYP_EMPTY_ZERO_PAGE;
	//debugp("Clear zero page: %p\n", empty_zero_page);
	memset(empty_zero_page, 0, PAGE_SIZE);
//debugp("\n");
//show_pcpu();


	/* Free all boot memory */
//show_bdata();
	printk("Free all boot memory\n");
	totalram_pages = free_all_bootmem();

	//totalram_pages += free_area(0x03000, 0x04fff, "free memory");
//debugp("\n");
//show_bdata();
//show_pcpu();
//show_bdata();
//debugp("mem_mep: %08lx\n", mem_map);

	//debugp("Count reserved pages\n");
	reservedpages = 0;
	for (pfn = 0; pfn < max_low_pfn; pfn++) {
		if (PageReserved(mem_map + pfn))
			reservedpages++;
	}
	//debugp("reserved pages: %d\n", reservedpages);
	/* Print informations	*/
	codesize = (unsigned long)&_etext - (unsigned long)&_stext;
	datasize = (unsigned long)&_edata - (unsigned long)&_etext;
	initsize = (unsigned long)&__init_end - (unsigned long)&__init_begin;

	//debugp("codesize = %08x\n", codesize);
	//debugp("datasize = %08x\n", datasize);
	//debugp("initsize = %08x\n", initsize);
	printk(KERN_INFO
		"Memory: %luk/%luk available (%dk kernel code, "
		"%dk reserved, %dk data, %dk init)\n",
		(unsigned long)nr_free_pages() << (PAGE_SHIFT - 10),
		max_mapnr << (PAGE_SHIFT - 10), codesize >> 10,
		reservedpages << (PAGE_SHIFT - 10), datasize >> 10,
		initsize >> 10
		);

	printk("mem_init_done ...........................................\n");
//debugp("\n");
//show_pcpu();
	//debugp("MEM_INIT_DONE................\n");
}

void free_initrd_mem(unsigned long start, unsigned long size)
{
	//debugp("start %08lx size %08lx\n", start, size);
	debugp("==========\n");
	while(1);
}
void free_initmem(void)
{
	totalram_pages += free_area(__phys_to_pfn(__pa(__init_begin)),
				    __phys_to_pfn(__pa(__init_end)), "init");
}
