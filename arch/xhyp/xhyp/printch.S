#include <linux/linkage.h>
#include <xhyp/hyp-macros.S>
#include <xhyp/hyp.h>

ENTRY(mprintch)
hang:
	hyp_console r0, #0
	mov     pc, lr

ENDPROC(mprintch)


