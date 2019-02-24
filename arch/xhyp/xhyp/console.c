/*
 * XHYP hypervisor console driver
 *
 * (C) Copyright 2012 MNIS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/console.h>

#include <xhyp/xhyp.h>

#include "../../../drivers/tty/hvc/hvc_console.h"

struct hvc_struct *hvc_xhyp_dev = NULL;

#define XHYP_CONSOLE_BUFSZ 16
static char xhyp_console_buffer[XHYP_CONSOLE_BUFSZ] = "toto\n";
static int xhyp_console_buffer_len = 5;
static int hvc_xhyp_get_chars(uint32_t vtermno, char *buf, int cnt)
{
	debugp("buf at %08lx count %d\n", buf, cnt);
	memcpy(buf, xhyp_console_buffer, xhyp_console_buffer_len);
	xhyp_console_buffer_len = 0;
	return 0;
}


static int hvc_xhyp_put_chars(uint32_t vtermno, const char *buf, int cnt)
{
	xhyp_console(buf, cnt);
	mb();
	return cnt;
}

static int notifier_add_xhyp(struct hvc_struct *hp, int data)
{
	debugp("\n");
	hp->irq_requested = 0;
	return 0;
}

static const struct hv_ops hvc_xhyp_get_put_ops = {
	.notifier_add = notifier_add_xhyp,
	.get_chars = hvc_xhyp_get_chars,
	.put_chars = hvc_xhyp_put_chars,
};

#define XHYP_COOKIE	0x58485950

static int __init hvc_xhyp_console_init(void)
{
	int retval;
//debugp("\n");
	//retval = hvc_instantiate(XHYP_COOKIE, 0, &hvc_xhyp_get_put_ops);
	retval = hvc_instantiate(0, 0, &hvc_xhyp_get_put_ops);
//debugp("retval: %d\n", retval);
	add_preferred_console("hvc", 0, NULL);
	return 0;
}

static int __init hvc_xhyp_init(void)
{
	struct hvc_struct *hp;
        register unsigned long sp asm ("sp");
//debugp("\n");
	hp = hvc_alloc(0, 0, &hvc_xhyp_get_put_ops, XHYP_CONSOLE_BUFSZ);
	if (IS_ERR(hp)) {
		debugp("ERROR %d\n", PTR_ERR(hp));
		while(1);
		return PTR_ERR(hp);
	}
	hvc_xhyp_dev = hp;
	return 0;
}

static void __exit hvc_xhyp_exit(void)
{
	if (hvc_xhyp_dev)
		hvc_remove(hvc_xhyp_dev);
}

module_init(hvc_xhyp_init);
module_exit(hvc_xhyp_exit);

console_initcall(hvc_xhyp_console_init);

