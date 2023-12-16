#ifndef _MATH_H_
#define _MATH_H_

#include <float.h>

#if (FLT_EVAL_METHOD == 0)
typedef float float_t;
typedef double double_t;
#elif (FLT_EVAL_METHOD == 1)
typedef double float_t;
typedef double double_t;
#elif (FLT_EVAL_METHOD == 2)
typedef long double float_t;
typedef long double double_t;
#endif

/*
   Return values for fpclassify.
   These are based on Intel x87 fpu condition codes
   in the high byte of status word and differ from
   the return values for MS IEEE 754 extension _fpclass()
*/
#define FP_NAN		0x0100
#define FP_NORMAL	0x0400
#define FP_INFINITE	(FP_NAN | FP_NORMAL)
#define FP_ZERO		0x4000
#define FP_SUBNORMAL	(FP_NORMAL | FP_ZERO)
/* 0x0200 is signbit mask */

#endif
