/*
 * xhyp/mm/mm.c 
 *
 * memory handling code
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

#include <linux/bootmem.h>
#include <linux/memblock.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/sections.h>

#include <xhyp/xhyp.h>
#include <xhyp/pages.h>

/*
 * setup_memblock is in charge to reserve the memory
 * Linux can NOT use for allocation.
 */

void __init setup_memblock(void)
{
	phys_addr_t memory_start, memory_end, memory_size;
	struct memblock_region *region;

	//debugp("PAGE_OFFSET %08lx TEXT_OFFSET %08lx\n", PAGE_OFFSET, PHYS_OFFSET);
	//memblock_debug = 1;
	for_each_memblock(memory, region) {
		memory_start = region->base;
		memory_end = region->base + region->size;
		//debugp("Memory: 0x%x-0x%x\n", memory_start, memory_end);
		printk(KERN_INFO "%s: Memory: 0x%x-0x%x\n", __func__,
		       memory_start, memory_end);
	}
	/* Reserve XHYP space	*/
	memblock_reserve(XHYP_START, XHYP_SIZE);
	/* Reserve Shared page	*/
	memblock_reserve(XHYP_SP_START, XHYP_SP_SIZE);
	/* Reserve Kernel space	*/
	memory_start = __pa(_stext) & PAGE_MASK;
	memory_size = ((_end - _stext) & PAGE_MASK) + PAGE_SIZE;
	memblock_reserve(memory_start, memory_size);
	/* Allow memblock to resize	*/
	memblock_allow_resize();
	/* Show what we got	*/
	//memblock_dump_all();
}

/*
 * setup_bootmem setup the bootmem allocator
 */

void __init setup_bootmem(void)
{
	unsigned long memory_start = 0L;
	unsigned long memory_end = 0L;
	unsigned int boot_pages;
	struct memblock_region *region;
	phys_addr_t bitmap;
	unsigned long ram_start_pfn, start_pfn, end_pfn;
	unsigned long bootmap_size;

	//debugp("\n");
	/* Find start and end of memory	*/
        for_each_memblock(memory, region) {
                memory_start = region->base;
                memory_end = region->base + region->size;
		//debugp("memory_start: %08lx, memory_end: %08lx\n", memory_start, memory_end);
        }

	/* first PFN after the kernel, last PFN to end of RAM	*/
	ram_start_pfn = PFN_UP(memory_start);
	start_pfn = PFN_UP(__pa(&_end));
	end_pfn = PFN_DOWN(memory_end);

	//debugp("start_pfn: %08lx end_pfn: %08lx\n", start_pfn, end_pfn);

	/* Initialize bootmem allocator	*/
	bootmap_size = init_bootmem(start_pfn, end_pfn);

	//debugp("%08lx - %08lx\n", PFN_PHYS(start_pfn),(end_pfn - start_pfn) << PAGE_SHIFT);
	/* Free blocks back into bootmem allocator	*/
	free_bootmem(PFN_PHYS(start_pfn),
			(end_pfn - start_pfn) << PAGE_SHIFT);

	//debugp("%08lx - %08lx\n", PFN_PHYS(start_pfn), bootmap_size);
	/* Reserve the bootmap	*/
	reserve_bootmem(PFN_PHYS(start_pfn), bootmap_size,
			BOOTMEM_DEFAULT);

	//debugp("\n");
	/* Reserve reserved memblocks	*/
	for_each_memblock(reserved, region) {
	//debugp("Reserved - 0x%08x-0x%08x\n", (u32) region->base, (u32) region->size);
		printk(KERN_INFO "Reserved - 0x%08x-0x%08x\n",
			(u32) region->base, (u32) region->size);
		reserve_bootmem(region->base, region->size, BOOTMEM_DEFAULT);
        }
	/* Declare memory belongs to node 0	*/
	////debugp("%08lx - %08lx\n", PFN_PHYS(start_pfn), PFN_PHYS(end_pfn - start_pfn));
        //memblock_set_node(PFN_PHYS(start_pfn), PFN_PHYS(end_pfn - start_pfn), 0);
	//debugp("\n");
}

/*
 * paging_init
 * 	- initialize the init_mm PGD
 * 	- initialize the zone max pfn
 * 	- initialize the mem_map
 *
 */

/* TBD : doit etre calculee	*/
phys_addr_t lowmem_limit __initdata = 0x04000000;
//extern void show_bdata(void);

void __init paging_init(void)
{
	struct memblock_region *region;
	unsigned long max_zone_pfns[MAX_NR_ZONES];
	unsigned long max_low_pfn, highend_pfn = 0;
	unsigned long real, end, virt;
	pgd_t	*pgd;
	int i;

	/* Initialize init PGD to 0	*/
	memset(max_zone_pfns, 0, sizeof(max_zone_pfns));
	for (i = 0; i < PTRS_PER_PGD; i++)
		init_mm.pgd[i] = __pgd(0);
	/* Initialize high_memory to lowmem_limit	*/
	high_memory = __va(lowmem_limit - 1) + 1;
//debugp("node_mem_map = %08lx\n", NODE_DATA(0)->node_mem_map);
	memblock_set_current_limit(lowmem_limit);
	//memblock_dump_all();
//debugp("\n");
//debugp("node_mem_map = %08lx\n", NODE_DATA(0)->node_mem_map);
//debugp("init_mm.pgd: %p\n", init_mm.pgd);
	/* Initialize init PGD with each memory region */
	for_each_memblock(memory, region) {
		real = region->base;
		end  = region->base + region->size;
		virt = __va(real);
		pgd = pgd_offset_k(virt);
		//debugp("real %08lx virt %08lx size %08lx\n", real, virt, end - real);
		//debugp("end %08lx lowmem_limit     %08lx\n", end, lowmem_limit);
	/* We loop on sections and break on lowmem_limit */
		while (real < end && real < lowmem_limit) {
			virt = (unsigned long)__va(real) | SEC_KERNEL;
			*pgd = __pgd(virt);
			//debugp("real %08lx virt %08lx pgd %p *pgd %08lx\n", real, virt, pgd, *pgd);
			pgd++;
			real += SECTION_SIZE;
		}
	/* Reset highend_pfn (used as address)	*/
		if (end > highend_pfn)
			highend_pfn = end;
	}
	highend_pfn >>= PAGE_SHIFT;	/* In fact it is a PFN ... */
	max_low_pfn = lowmem_limit >> PAGE_SHIFT;
//debugp("mem_map: %08lx\n", mem_map);

#ifdef CONFIG_HIGHMEM
	/* We probably never get highmem in XHYP */
	BUG();
	/* But we should do the following	*/
	kmap_init();
        max_zone_pfns[ZONE_HIGHMEM] = highend_pfn;
#endif
//debugp("\n");
//debugp("node_mem_map = %08lx\n", NODE_DATA(0)->node_mem_map);
        max_zone_pfns[ZONE_NORMAL] = max_low_pfn;
//debugp("max_zone_pfns[ZONE_NORMAL] : %08lx\n", max_zone_pfns[ZONE_NORMAL]);
	free_area_init(max_zone_pfns);
//debugp("node_mem_map = %08lx\n", NODE_DATA(0)->node_mem_map);

//show_bdata();
//debugp("\n");
	mem_map = NODE_DATA(0)->node_mem_map;
	//debugp("node_mem_map = %08lx\n", NODE_DATA(0)->node_mem_map);
//debugp("mem_map: %08lx\n", mem_map);
}

