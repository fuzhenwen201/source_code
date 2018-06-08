/*************************************************************************
	> File Name: bithack.h
	> Author: xyz
	> Mail: xiao13149920@foxmail.com 
	> Created Time: Fri 10 Feb 2017 03:17:43 PM CST
 ************************************************************************/
#ifndef SOLUTIONGATEWAY_BITHACK_H_
#define SOLUTIONGATEWAY_BITHACK_H_
/* test if x is even */
#define B_EVEN(x)        (((x)&1)==0)

/* test if x is odd */
#define B_ODD(x)         (!B_EVEN((x)))

/* test if n-th bit in x is set */
#define B_IS_SET(x, n)   (((x) & (1<<(n)))?1:0)

/* set n-th bit in x */
#define B_SET(x, n)      ((x) |= (1<<(n)))

/* unset n-th bit in x */
#define B_UNSET(x, n)    ((x) &= ~(1<<(n)))

/* toggle n-th bit in x */
#define B_TOGGLE(x, n)   ((x) ^= (1<<(n)))

/* turn off right-most 1-bit in x */
#define B_TURNOFF_1(x)   ((x) &= ((x)-1))

/* isolate right-most 1-bit in x */
#define B_ISOLATE_1(x)   ((x) &= (-(x)))

/* right-propagate right-most 1-bit in x */
#define B_PROPAGATE_1(x) ((x) |= ((x)-1))

/* isolate right-most 0-bit in x */
#define B_ISOLATE_0(x)   ((x) = ~(x) & ((x)+1))

/* turn on right-most 0-bit in x */
#define B_TURNON_0(x)    ((x) |= ((x)+1))

#endif


