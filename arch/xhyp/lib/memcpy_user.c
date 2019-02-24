#include <asm/page.h>
#include <asm/generic/memory.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/mm_types.h>



static inline int __copy_to_user_page(void *to, const void *from,
				      unsigned long n)
{
	unsigned long page, offset;

	if ((page = parse_ptabs_write((unsigned long)to, &offset)) != -EFAULT) {
		memcpy((void *)(page + offset), from, n);
		return 0;
	}
	return -EFAULT;
}

unsigned long __must_check
copy_to_user(void __user *to, const void *from, unsigned long n)
{
	unsigned long copy_size = (unsigned long)to & ~PAGE_MASK;

	/* kernel access */
	if (segment_eq(get_fs(), KERNEL_DS)) {
		memcpy(to, from, n);
		return 0;
	}

	if (copy_size) {
		copy_size = min(PAGE_SIZE - copy_size, n);
		if (__copy_to_user_page(to, from, copy_size) == -EFAULT)
			return n;
		n -= copy_size;
	}

	while (n) {
		from +=copy_size;
		to += copy_size;
		copy_size = min(PAGE_SIZE, n);
		if (__copy_to_user_page(to, from, copy_size) == -EFAULT)
			return n;
		n -= copy_size;
	}
	return 0;
}
EXPORT_SYMBOL(copy_to_user);

static inline int __copy_from_user_page(void *to, const void *from,
					unsigned long n)
{
	unsigned long page, offset;

	if ((page = parse_ptabs_read((unsigned long)from, &offset)) != -EFAULT) {
		memcpy(to, (void *)(page + offset), n);
		return 0;
	}
	log_efault(__func__, from, to, n);
	return -EFAULT;
}

unsigned long
copy_from_user(void *to, const void __user *from, unsigned long n)
{
	unsigned long copy_size = (unsigned long)from & ~PAGE_MASK;

	if (segment_eq(get_fs(), KERNEL_DS)) {
		memcpy(to, from, n);
		return 0;
	}

	if (copy_size) {
		copy_size = min(PAGE_SIZE - copy_size, n);
		if (__copy_from_user_page(to, from, copy_size) == -EFAULT) {
			memset(to, 0, n);
			return n;
		}
		n -= copy_size;
	}
	while (n) {
		from +=copy_size;
		to += copy_size;
		copy_size = min(PAGE_SIZE, n);
		if (__copy_from_user_page(to, from, copy_size) == -EFAULT) {
			memset(to, 0, n);
			return n;
		}
		n -= copy_size;
	}
	return 0;
}
EXPORT_SYMBOL(copy_from_user);

static inline int __clear_user_page(void * address, unsigned long n)
{
	unsigned long page, offset;

	page = parse_ptabs_write((unsigned long)address, &offset);
	if (page != -EFAULT) {
		memset((void *)(page + offset), 0, n);
		return 0;
	}
	log_efault(__func__, address, 0, n);
	return -EFAULT;
}
unsigned long clear_user(void *address, unsigned long n)
{
	unsigned long clear_size = (unsigned long)address & ~PAGE_MASK;

	if (segment_eq(get_fs(), KERNEL_DS)) {
		memset(address, 0, n);
		return 0;
	}

	if (clear_size) {
		clear_size = min(PAGE_SIZE - clear_size, n);
		if (__clear_user_page(address, clear_size) == -EFAULT)
			return n;
		n -= clear_size;
	}
	while (n) {
		address += clear_size;
		clear_size = min(PAGE_SIZE, n);
		if (__clear_user_page(address, clear_size) == -EFAULT)
			return n;
		n -= clear_size;
	}
	return 0;
}
EXPORT_SYMBOL(clear_user);

unsigned long
__clear_user(void __user *to, unsigned long n)
{
	return clear_user(to, n);
}
EXPORT_SYMBOL(__clear_user);

/*
 * Copy a null terminated string from userspace.
 */

#define __do_strncpy_from_user_page(dst, src, count)                    \
	do {								\
		char *_src = src;					\
		while (count) {						\
			*dst = *_src;					\
			if (!*_src)					\
				break;					\
			dst++;						\
			_src++;						\
			count--;					\
		}							\
	} while (0)
#endif

static inline int __strncpy_from_user_page(char * to, const char * from,
					   unsigned long n)
{
	unsigned long page, offset;

	page = parse_ptabs_read((unsigned long)from, &offset);
	if (page != -EFAULT) {
		/* after finishing the copy operation count is either
		 * - zero: max number of bytes copied or
		 * - non zero: end of string reached, n containing the
		 *             number of remaining bytes
		 */
		__do_strncpy_from_user_page(to, (char *)(page + offset), n);
		return n;
	}
	log_efault(__func__, from, to, n);
	return -EFAULT;
}

/* strncpy returns the number of bytes copied. We calculate the number
 * simply by substracting the number of bytes remaining from the
 * maximal length. The number of bytes remaining is (n + res) with n
 * beeing the number of bytes to copy from the next pages and res the
 * number of remaining bytes after reaching the '\0' */

long strncpy_from_user(char *dst, const char *src, long count)
{
	unsigned long copy_size = (unsigned long)src & ~PAGE_MASK;
	long res;
	unsigned long n = count;

	if (segment_eq(get_fs(), KERNEL_DS)) {
		/* strncpy the data but deliver back the bytes copied */
		long c = 0;
		while (c++ < count && (*dst++ = *src++) != '\0')
			/* nothing */;
		return c;
	}

	if (copy_size) {
		copy_size = min(PAGE_SIZE - copy_size, n);
		res = __strncpy_from_user_page(dst, src, copy_size);
		n -= copy_size;
		if (res == -EFAULT) {
			return -EFAULT;
		}
		else if (res)
			return count - (n + res);
	}
	while (n) {
		src += copy_size;
		dst += copy_size;
		copy_size = min(PAGE_SIZE, n);
		n -= copy_size;
		res = __strncpy_from_user_page(dst, src, copy_size);
		if (res == -EFAULT) {
			return -EFAULT;
		}
		else if (res)
			return count - (n + res);
	}
	return count;
}
EXPORT_SYMBOL(strncpy_from_user);

static inline int __strnlen_from_user_page(const char *from,
					   unsigned long n, unsigned long *len)
{
	unsigned long page, offset;

	page = parse_ptabs_read((unsigned long)from, &offset);
	if (page != -EFAULT) {
		int end;
		{
			unsigned long i, p = page + offset;
			end = 0;
			for (i = 0; i < n; i++) {
				if (*(unsigned char *)(p + i) == 0) {
					end = 1;
					i++; /* Include the zero */
					break;
				}
			}
			*len += i;
		}
		return end;
	}
	log_efault(__func__, from, 0, n);
	return -EFAULT;
}

/* strnlen returns the number of bytes in a string. We calculate the number
 * simply by substracting the number of bytes remaining from the
 * maximal length. The number of bytes remaining is (n + res) with n
 * being the number of bytes to copy from the next pages and res the
 * number of remaining bytes after reaching the '\0' */
long strnlen_user(const char *src, long n)
{
	unsigned long search_size = PAGE_SIZE - ((unsigned long)src & ~PAGE_MASK);
	int res;
	unsigned long len=0;

	if (segment_eq(get_fs(), KERNEL_DS)) {
		len = strnlen(src, n);
		return len + 1;
	}

	if (!search_size)
		search_size = PAGE_SIZE;
	if (search_size > n)
		search_size = n;

	while (n > 0) {
		res = __strnlen_from_user_page(src, search_size, &len);
		if (res == -EFAULT)
			return 0; /* EFAULT */
		else if (res)
			return len;

		src += search_size;
		n   -= search_size;
		search_size = PAGE_SIZE;
	}

	return 0; /* EFAULT */
}
EXPORT_SYMBOL(strnlen_user);

