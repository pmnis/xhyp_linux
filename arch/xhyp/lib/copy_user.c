#include <linux/uaccess.h>
#include <linux/sched.h>
#include <xhyp/xhyp.h>
/*
 * TBD
 * voir: arch/l4/lib/memcpy_user.c
 * et idem pour les pages
 */


unsigned long __copy_from_user(void *to, const void __user *from, unsigned long n)
{
	//debugp("to %p from %p count %d\n", to, from, n);
	//if (access_ok(VERIFY_READ, from, n))
		memcpy(to, from, n);
	return 0;
}

unsigned long __copy_to_user(void __user *to, const void *from, unsigned long n)
{
	//debugp("to %p from %p count %d\n", to, from, n);
	//if (access_ok(VERIFY_WRITE, to, n))
		memcpy(to, from, n);
	return 0;
}

unsigned long __copy_to_user_std(void __user *to, const void *from, unsigned long n)
{
	return __copy_to_user(to, from, n);
}

unsigned long __clear_user(void __user *addr, unsigned long n)
{
	//debugp("addr %p count %d\n", addr, n);
	//debugp("process: %s\n", current->comm);
	//if (access_ok(VERIFY_WRITE, addr, n))
		memset(addr, 0, n);
	return 0;
}

unsigned long __clear_user_std(void __user *addr, unsigned long n)
{
	return __clear_user(addr, n);
}


