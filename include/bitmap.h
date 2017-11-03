/*
 * bitmap.h
 *
 *  Created on: Oct 11, 2017
 *      Author: tong60514
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#ifndef BITS_PER_LONG

#ifdef CONFIG_X86_32
# define BITS_PER_LONG 32
#else
# define BITS_PER_LONG 64
#endif

#endif


#ifndef _BITOPS_LONG_SHIFT

#if BITS_PER_LONG == 32
# define _BITOPS_LONG_SHIFT 5
#elif BITS_PER_LONG == 64
# define _BITOPS_LONG_SHIFT 6
#else
# error "Unexpected BITS_PER_LONG"
#endif

#endif



#ifndef BITOP_ADDR
#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 1)
#define BITOP_ADDR(x) "=m" (*(volatile long *) (x))
#else
#define BITOP_ADDR(x) "+m" (*(volatile long *) (x))
#endif
#endif

#ifndef ADDR
#define ADDR	BITOP_ADDR(addr)
#endif


#ifndef DECLARE_BITMAP
#define DECLARE_BITMAP(name,bits) \
    unsigned long name[BITS_TO_LONGS(bits)]
#endif



#ifndef BITS_PER_BYTE
#define BITS_PER_BYTE 8
#endif

#ifndef DIV_ROUND_UP
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#endif

#ifndef BITS_TO_LONGS
#define BITS_TO_LONGS(nr) DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
#endif

#ifndef test_bit
#define test_bit(nr, addr)			\
	(__builtin_constant_p((nr))                 \
	 ? constant_test_bit((nr), (addr))	        \
	 : variable_test_bit((nr), (addr)))
#endif

#define LOCK_PREFIX \
		".section .smp_locks,\"a\"\n"\
		".align 4\n"\
		".long 661f\n" /* address */\
		".previous\n"\
		"661:\n\tlock;"

#define IS_IMMEDIATE(nr)		(__builtin_constant_p(nr))

#define CONST_MASK_ADDR(nr, addr)	BITOP_ADDR((void *)(addr) + ((nr)>>3))
#define CONST_MASK(nr)			(1 << ((nr) & 7))

#define SET_BIT(addr,nr) __set_bit(nr,addr);
#define CLEAR_BIT(addr,nr) __clear_bit(nr,addr);
static __always_inline void __clear_all(volatile unsigned long *addr)
{
	static const long zero = 0;
	asm volatile("mov $0,%0 "
			: ADDR
			: "Ir"(zero)
			:
			  );
}
static __always_inline void __clear_bit(long nr, volatile unsigned long *addr)
{
	asm volatile("btr %1,%0" : ADDR : "Ir" (nr));
}

static __always_inline void __set_bit(long nr, volatile unsigned long *addr)
{
	asm volatile("bts %1,%0" : ADDR : "Ir" (nr));
}



static __always_inline void
atomic_set_bit(long nr, volatile unsigned long *addr)
{
	asm volatile(LOCK_PREFIX "bts %1,%0"
		: ADDR : "Ir" (nr) : "memory");
}


static __always_inline void
atomic_clear_bit(long nr, volatile unsigned long *addr)
{
	asm volatile(LOCK_PREFIX "btr %1,%0"
		: ADDR
		: "Ir" (nr));
}



static inline int __test_and_change_bit(long nr, volatile unsigned long *addr)
{
	int oldbit;

	asm volatile("btc %2,%1\n\t"
		     "sbb %0,%0"
		     : "=r" (oldbit), ADDR
		     : "Ir" (nr) : "memory");

	return oldbit;
}
static inline int __test_and_clear_bit(long nr, volatile unsigned long *addr)
{
	int oldbit;

	asm volatile("btr %2,%1\n\t"
		     "sbb %0,%0"
		     : "=r" (oldbit), ADDR
		     : "Ir" (nr));
	return oldbit;
}
static inline unsigned long __ffs(unsigned long word)
{
	asm("rep; bsf %1,%0"
		: "=r" (word)
		: "rm" (word));
	return word;
}

static inline int variable_test_bit(long nr, volatile const unsigned long *addr)
{
	int oldbit;

	asm volatile(
			"bt %2,%1\n\t" // 1010 test 2 cf : 0 -> sbb  oldbit 0
		    "sbb %0,%0"
		    : "=r" (oldbit)
		    : "m" (*(unsigned long *)addr), "Ir" (nr));
	return oldbit;
}

static __always_inline int constant_test_bit(long nr, const volatile unsigned long *addr)
{
	return ((1UL << (nr & (BITS_PER_LONG-1))) &
		(addr[nr >> _BITOPS_LONG_SHIFT])) != 0;
}





#endif /* BITMAP_H_ */
