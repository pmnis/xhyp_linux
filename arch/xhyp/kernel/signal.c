#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/personality.h>
#include <linux/freezer.h>
#include <linux/uaccess.h>
#include <linux/tracehook.h>

#include <asm/elf.h>
#include <asm/cacheflush.h>
#include <asm/ucontext.h>
#include <asm/unistd.h>


asmlinkage int sys_rt_sigreturn(struct pt_regs *regs)
{
	return 0;
}

