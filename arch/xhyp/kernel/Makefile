#
# Makefile for the linux kernel.
#

CPPFLAGS_vmlinux.lds := -DTEXT_OFFSET=$(TEXT_OFFSET)
AFLAGS_head.o        := -DTEXT_OFFSET=$(TEXT_OFFSET)

extra-y	:= head.o vmlinux.lds

obj-y	:= setup.o idle.o ksym_xhyp.o process.o \
	   traps.o time.o irq.o entry.o signal.o sys_xhyp.o \
	   sys_call_table.o

obj-y	+= ptrace.o

obj-$(CONFIG_MODULES)		+= module.o armksym.o
obj-$(CONFIG_OF)		+= prom.o

clean:
