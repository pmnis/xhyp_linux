#
# linux/arch/arm/lib/Makefile
#
# Copyright (C) 1995-2000 Russell King
#


lib-y		:= backtrace.o changebit.o csumipv6.o  \
		   clearbit.o lib1funcs.o \
		   delay.o delay-loop.o findbit.o  \
		   memzero.o setbit.o              \
		   testchangebit.o testclearbit.o testsetbit.o        \
		   ashldi3.o ashrdi3.o lshrdi3.o muldi3.o             \
		   ucmpdi2.o div64.o                      \
		   io-readsb.o io-writesb.o io-readsl.o io-writesl.o  \
		   call_with_stack.o copy_user.o

#memcpy_user.o

# obj-y	+= clear_user.o copy_page.o getuser.o putuser.o
obj-y	+= copy_page.o getuser.o putuser.o

# the code in uaccess.S is not preemption safe and
# probably faster on ARMv3 only
# mmu-y	+= copy_from_user.o copy_to_user.o
#lib-y	+= copy_from_user.o copy_to_user.o

# using lib_ here won't override already available weak symbols
# obj-$(CONFIG_UACCESS_WITH_MEMCPY) += uaccess_with_memcpy.o
# obj-y += uaccess_with_memcpy.o

lib-$(CONFIG_MMU) += $(mmu-y)

# ifeq ($(CONFIG_CPU_32v3),y)
#   lib-y	+= io-readsw-armv3.o io-writesw-armv3.o
# else
  lib-y	+= io-readsw-armv4.o io-writesw-armv4.o
# endif

lib-$(CONFIG_ARCH_RPC)		+= ecard.o io-acorn.o floppydma.o
lib-$(CONFIG_ARCH_SHARK)	+= io-shark.o

#$(obj)/csumpartialcopy.o:	$(obj)/csumpartialcopygeneric.S
#$(obj)/csumpartialcopyuser.o:	$(obj)/csumpartialcopygeneric.S

