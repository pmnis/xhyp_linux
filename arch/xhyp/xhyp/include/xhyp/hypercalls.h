/*
 * hypercalls.h
 *
 * definitions for hypervisor calls
 *
 * Author: Pierre Morel <pmorel@mnis.fr>
 *
 * $LICENSE:
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


extern void xhyp_console(volatile char *s, int n);
extern void xhyp_irq_request(void(*)(unsigned long), unsigned long *);
extern void xhyp_irq_enable(int);
extern void xhyp_irq_disable(int);
extern void xhyp_irq_return(unsigned long);
extern void xhyp_setmode(unsigned long);
extern unsigned long xhyp_cpuctrl(unsigned long);
extern void xhyp_pgfault_request(void(*)(unsigned long, unsigned int, void *), unsigned long);
extern void xhyp_idle(void);
extern void xhyp_set_pte(void *, unsigned long, unsigned long *, unsigned long);
extern void xhyp_tlb(unsigned long cmd, unsigned long entry);
extern void xhyp_set_domain(unsigned long domain);
extern unsigned long xhyp_probe(void *, void *, unsigned long);
extern void _xhyp_copy_user_page(void *, const void *, unsigned long);
extern void _xhyp_clear_user_page(void *, unsigned long);
extern unsigned long xhyp_get_pmdval(unsigned long);
extern unsigned long xhyp_set_pmd(unsigned long,unsigned long, unsigned long, unsigned long);
extern unsigned long _xhyp_set_pte_ext(unsigned long, unsigned long, unsigned long);
extern unsigned long xhyp_get_timer(void);
extern unsigned long xhyp_switch_mm(void *);
extern unsigned long xhyp_abrt_ret(void);
extern unsigned long xhyp_return_to_user(void);
/*
 * _hyp_flush_cache:
 *  - unsigned long: flags
 *  - unsigned long: MVA / ASID
 *  - unsigned long: size
 */
extern void _hyp_flush_cache(unsigned long, unsigned long, unsigned long);

#define HYP_FC_MVA	0x01
#define HYP_FC_ASID	0x02
#define HYP_FC_ALLD	0x04
#define HYP_FC_ALLI	0x08
#define HYP_FC_LVL1	0x10
#define HYP_FC_LVL2	0x20

extern unsigned long _hyp_cpuctrl_ext(unsigned long);
extern unsigned long _hyp_tlb(unsigned long, unsigned long);

extern int xhyp_debug;
//#define debugp(format, arg...) if (xhyp_debug) early_printk("%s-%s [%d] " format,  __FILE__, __func__, __LINE__, ##arg)
#define debugp(format, arg...) if (xhyp_debug) early_printk("%08lx %s-%s [%d] " format, xhyp_sp->v_cpsr, __FILE__, __func__, __LINE__, ##arg)

