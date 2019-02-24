#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies. Remember to do have actions
# for "archclean" and "archdep" for cleaning up and making dependencies for
# this architecture
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1994 by Linus Torvalds
# Modifications for the OpenRISC architecture:
# Copyright (C) 2003 Matjaz Breskvar <phoenix@bsemi.com>
# Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
#
# Based on:
# arch/arm/Makefile

KBUILD_DEFCONFIG := xhyp_defconfig

LDFLAGS         :=
OBJCOPYFLAGS    := -O binary -R .note -R .comment -S
LDFLAGS_vmlinux :=-p --no-undefined -X
GZFLAGS         :=-9
LIBGCC 		:= $(shell $(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)

KBUILD_CFLAGS   +=$(call cc-option,-marm,)
KBUILD_CFLAGS   += $(call cc-option,-fno-dwarf2-cfi-asm)

ifeq ($(CONFIG_FRAME_POINTER),y)
KBUILD_CFLAGS   +=-fno-omit-frame-pointer -mapcs -mno-sched-prolog
endif

ifeq ($(CONFIG_CC_STACKPROTECTOR),y)
KBUILD_CFLAGS   +=-fstack-protector
endif

KBUILD_CPPFLAGS += -mlittle-endian
AS              += -EL
LD              += -EL

comma = ,

arch-$(CONFIG_CPU_32v5)         :=-D__LINUX_ARM_ARCH__=5 $(call cc-option,-march=armv5te,-march=armv4t)

#tune-$(CONFIG_CPU_ARM926T)      :=-mtune=arm9tdmi
tune-y      :=-mtune=arm9tdmi

CFLAGS_ABI      :=-mabi=aapcs-linux -mno-thumb-interwork

KBUILD_CFLAGS   +=$(CFLAGS_ABI) $(CFLAGS_THUMB2) $(arch-y) $(tune-y) $(call cc-option,-mshort-load-bytes,$(call cc-option,-malignment-traps,)) -msoft-float -Uarm
KBUILD_AFLAGS   +=$(CFLAGS_ABI) $(AFLAGS_THUMB2) $(arch-y) $(tune-y) -msoft-float

LINUXINCLUDE	+= -I$(srctree)/arch/$(hdr-arch)/xhyp/include

CHECKFLAGS      += -D__arm__

TEXT_OFFSET := 0x02100000

export	TEXT_OFFSET

head-y 		:= arch/xhyp/kernel/head.o


core-y		+= arch/xhyp/kernel/
core-y		+= arch/xhyp/mm/
core-y		+= arch/xhyp/xhyp/

libs-y		:= arch/xhyp/lib/ $(libs-y)

core-$(BUILTIN_DTB) += arch/xhyp/boot/

all: vmlinux
