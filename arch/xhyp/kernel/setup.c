#include <linux/export.h>
#include <linux/sched.h>
#include <linux/personality.h>
#include <linux/binfmts.h>
#include <linux/elf.h>
#include <linux/seq_file.h>
#include <linux/initrd.h>
#include <linux/memblock.h>

#include <asm/sections.h>

#include <xhyp/shared_page.h>
#include <xhyp/hypercalls.h>
#include <xhyp/xhyp.h>

unsigned long atags_pointer	__initdata;
unsigned long soc_id		__initdata;
extern unsigned long __kernel_stack;

/* TBD */
char boot_cmdline[COMMAND_LINE_SIZE];

char elf_platform[ELF_PLATFORM_SIZE];
EXPORT_SYMBOL(elf_platform);


void __init xhyp_parse_atags  (unsigned long atags_pointer)
{
	/* add memory conforming to atags	*/
	//memblock_add(XHYP_GUEST_START, XHYP_GUEST_SIZE - 1);
	memblock_add(0, XHYP_SIZE + XHYP_GUEST_SIZE - 1);
	/* Add command line	*/
	strncpy(boot_cmdline, "console=hvc root=/dev/ram0 lpj=1757184 rdinit=/bin/busybox", COMMAND_LINE_SIZE);
	strncpy(boot_command_line, boot_cmdline, COMMAND_LINE_SIZE);
}

void show_pages(void)
{
	struct page *p = &mem_map[0];
	int i = 0;

	for (; i < (XHYP_MAX_MEM >> PAGE_SHIFT);i++, p++) {
		early_printk("%08lx ", (i << PAGE_SHIFT));
		early_printk("%08lx %08lx ", p->flags, p->mapping);
		early_printk("%08lx %08lx\n", p->index, p->counters);
	}
}

extern void show_bdata(void);

void __init setup_arch(char **cmdline_p)
{
	//debugp("IRQ should be off\n");
	//debugp("Machine ID %08lx\n", soc_id);
	//debugp("ATAGS      %08lx\n", atags_pointer);
	//debugp("stack      %08lx\n", __kernel_stack);

	/* process 1's initial memory region is the kernel code/data */
	init_mm.start_code	= (unsigned long) _text;
	init_mm.end_code	= (unsigned long) _etext;
	init_mm.end_data	= (unsigned long) _edata;
	init_mm.brk		= (unsigned long) _end;

	xhyp_parse_atags(atags_pointer);

	/* populate cmd_line too for later use, preserving boot_command_line */
	*cmdline_p = boot_cmdline;
	/* Is there any parameter to parse early ? */
	parse_early_param();

	/* Reserve memblocks	*/
	setup_memblock();

	/* setup bootmem allocator */
	setup_bootmem();

	/* paging_init() sets up the MMU and marks all pages as reserved */
	paging_init();
	/* setup percpu data	*/

	setup_per_cpu_areas();

	strncpy(*cmdline_p, boot_cmdline, COMMAND_LINE_SIZE);

	printk("Virtual ARM architecture initialized\n");

	return;
}

/*
int elf_check_arch(const struct elf32_hdr *x)
{
	debugp("==========\n");
	//while(1);
	return 1;
}
EXPORT_SYMBOL(elf_check_arch);

void elf_set_personality(const struct elf32_hdr *x)
{
	debugp("==========\n");
	//while(1);
}

*/

extern unsigned long __pv_phys_offset;
EXPORT_SYMBOL(__pv_phys_offset);


/*
 * Called by start_kernel
 */
__init void init_IRQ(void)
{
	//debugp("\n");
	xhyp_init_irq();
}

/*
 * Called by start_kernel
 */
__init void time_init(void)
{
	xhyp_timer_init();
#ifdef CONFIG_HAVE_SCHED_CLOCK
	sched_clock_postinit();
#endif
}

/*
unsigned long lpj_fine;
void calibrate_delay(void)
{
	debugp("==========\n");
	while(1);
}
*/

unsigned int elf_hwcap __read_mostly;
EXPORT_SYMBOL(elf_hwcap);

void show_regs(struct pt_regs *regs)
{
	debugp("==========\n");
	while(1);
}
EXPORT_SYMBOL(show_regs);



static int c_show(struct seq_file *m, void *v)
{
	seq_printf(m, "Processor\t: Virtual ARM @%d MHz\n", 100);
	seq_printf(m, "-- dcache disabled\n");
	seq_printf(m, "-- icache disabled\n");
	seq_printf(m, "-- additional features:\n");
	seq_printf(m, "-- PIC\n");
	seq_printf(m, "-- timer\n");

	return 0;
}

static void *c_start(struct seq_file *m, loff_t *pos)
{
	return *pos < 1 ? (void *)1 : NULL;
}

static void *c_next(struct seq_file *m, void *v, loff_t *pos)
{
	++*pos;
	return NULL;
}

static void c_stop(struct seq_file *m, void *v)
{
}

const struct seq_operations cpuinfo_op = {
	.start	= c_start,
	.next	= c_next,
	.stop	= c_stop,
	.show	= c_show
};


void pm_power_off(void)
{
	debugp("==========\n");
	while(1);
}

void machine_halt(void)
{
	debugp("==========\n");
	while(1);
}

void machine_power_off(void)
{
	debugp("==========\n");
	while(1);
}

void machine_restart(void)
{
	debugp("==========\n");
	while(1);
}

