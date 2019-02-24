#include <linux/linkage.h>
#include <linux/init.h>

#include <asm/assembler.h>
#include <asm/domain.h>
#include <asm/ptrace.h>
#include <asm/asm-offsets.h>
#include <asm/memory.h>
#include <asm/thread_info.h>
#include <asm/pgtable.h>

#include <xhyp/hyp.h>

#define KERNEL_RAM_VADDR        (PAGE_OFFSET + TEXT_OFFSET)
#define PG_DIR_SIZE     0x4000
#define STACK_SIZE	0x2000

	__HEAD

/*
	r1: machine no
	r2: atags
*/
ENTRY(stext)
	mov	fp, #0
	ldr	sp, __kernel_stack
	add	sp, #STACK_SIZE
	ldr	r0, __soc_id
	str	r1, [r0]
	ldr	r0, __atags_pointer
	str	r2, [r0]
	mov	r10, r0
	mov	r11, r1
	ldr	r0, =chain
	mov	r1, #15
	swi	#_HYP_console
nop
nop
	b start_kernel

chain:	.asciz	"Starting Linux\n"

/* Definitions needed later in setup_arch	*/
	.globl	__kernel_stack
__kernel_stack:
	.long	init_thread_union
__atags_pointer:
	.long	atags_pointer
__soc_id:
	.long	soc_id

        .globl  swapper_pg_dir
        .equ    swapper_pg_dir, KERNEL_RAM_VADDR - PG_DIR_SIZE


