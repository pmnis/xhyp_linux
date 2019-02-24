
#ifndef __ASM_XHYP_IO_H
#define __ASM_XHYP_IO_H

#define IO_SPACE_LIMIT		0

#define HAVE_ARCH_PIO_SIZE	1
#define PIO_RESERVED		0X0UL
#define PIO_OFFSET		0
#define PIO_MASK		0

#include <asm-generic/io.h>

extern void __iomem *__ioremap(phys_addr_t offset, unsigned long size,
				pgprot_t prot);

static inline void __iomem *ioremap(phys_addr_t offset, unsigned long size)
{
	return __ioremap(offset, size, PAGE_KERNEL);
}

static inline void __iomem *ioremap_nocache(phys_addr_t offset,
					     unsigned long size)
{
	return __ioremap(offset, size,
			 __pgprot(pgprot_val(PAGE_KERNEL)));
}

extern void iounmap(void *addr);
#endif
