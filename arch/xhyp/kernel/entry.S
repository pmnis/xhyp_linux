#include <linux/linkage.h>
#include <linux/init.h>

#include <asm/assembler.h>
#include <asm/domain.h>
#include <asm/ptrace.h>
#include <asm/asm-offsets.h>
#include <asm/memory.h>
#include <asm/thread_info.h>
#include <asm/pgtable.h>

#include <xhyp/hyp-macros.S>

hang:
	b hang

/*
** Write back the arguments:
** r4: argument for the thread
** r5: Thread function
** r6: Thread Exit
*/
ENTRY(kernel_thread_helper)
	mov	r0, r4
	mov	lr, r6
	mov	pc, r5

ENTRY(ret_to_user)
	hyp_idle
	b hang

ENTRY(sys_sigaltstack)
	mov	sp, r0
	mov	pc, lr

ENTRY(sys_or1k_atomic)
	hyp_idle
	b hang
/*
ENTRY(__aeabi_ldiv0)
	mov	r0, #0
	mov	pc, lr
*/

/*
 * Register switch for XHYP for ARM
 * r0 = previous thread_info, r1 = next thread_info
 * previous and next are guaranteed not to be the same.
 */
ENTRY(__switch_to2)
	add	ip, r0, #TI_CPU_SAVE	/* IP = registers address	*/
	stmia	ip!, {r4 - sl, fp, sp, lr}	/* save registers	*/
/* TBD: MODIFIER LA PARTIE UTILISATEUR .... USER KERNEL HELPER		*/
	mov	r3, r0
	ldr	r0, [r1, #TI_TP_VALUE]		/* retrieve TLS		*/
	hyp_set_tls				/* set tls		*/
/* TBD: ajouter la macro.... hyp_set_tls				*/
	ldr	r0, [r1, #TI_CPU_DOMAIN]	/* get domain		*/
	hyp_set_domain				/* set domain		*/
	mov	r0, r3
	add	r4, r1, #TI_CPU_SAVE	/* point to next registers	*/
	ldmia	r4, {r4 - sl, fp, sp, pc}	/* switch ...		*/
/* NEVER REACHED	*/

/*
 * Going back to user restoring registers
 * r0: thread
 */

ENTRY(__back_to_user)
	mov	sp, r0
	ldr	r1, [sp, #S_PSR]
	msr	CPSR_f, r1
	ldr	lr, [sp, #S_PC]!
	sub	sp, sp, #8
	ldmdb	sp, {r0 - r12}
	mov	r0, r0
	add	sp, sp, #S_FRAME_SIZE - S_PC
	mov	pc, lr


