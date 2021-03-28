#ifndef __MATH_H
#ident	"@(#)_math.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/* return value for an ANSI C function if argument erronneous. This value is
	implementation defined */
#define _EDOM_RET_VAL 0.0

#define _NOEXC		0	/* no exception to report */
#define _DOMAIN		1
#define	_SING		2
#define	_OVERFLOW	3
#define	_UNDERFLOW	4
#define	_TLOSS		5
#define	_PLOSS		6

extern double _matherr ( register unsigned int status );
extern unsigned int _acos881( double, double * );
extern unsigned int _asin881( double, double * );
extern unsigned int _atan881( double, double * );
extern unsigned int _cos881(  double, double * );
extern unsigned int _cosh881( double, double * );
extern unsigned int _etox881( double, double * );
extern unsigned int _log881(  double, double * );
extern unsigned int _logn881( double, double * );
extern unsigned int _sin881(  double, double * );
extern unsigned int _sinh881( double, double * );
extern unsigned int _sqrt881( double, double * );
extern unsigned int _tan881(  double, double * );
extern unsigned int _tanh881( double, double * );


#if defined __STRICT_STDC__ 

/* some useful constants */

#define M_E		2.7182818284590452354
#define M_LOG2E		1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2		0.69314718055994530942
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962
#define M_1_PI		0.31830988618379067154
#define M_2_PI		0.63661977236758134308
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.41421356237309504880
#define M_SQRT1_2	0.70710678118654752440

#if u3b || u3b5 || m68 || m68k || M68020
#define MAXFLOAT	((float)3.40282346638528860*1.0e+38)	/* sw0 */
#endif
#if pdp11 || vax
#define MAXFLOAT	((float)1.701411733192644299e+38)
#endif
#if gcos
#define MAXFLOAT	((float)1.7014118219281863150e+38)
#endif
#if !defined MAXDOUBLE
#define MAXDOUBLE	1.79769313486231470e+308		/* ORG */
#endif

#define HUGE	MAXFLOAT

#define _ABS(x)	((x) < 0 ? -(x) : (x))
#define _REDUCE(TYPE, X, XN, C1, C2)	{ \
	double x1 = (double)(TYPE)X, x2 = X - x1; \
	X = x1 - (XN) * (C1); X += x2; X -= (XN) * (C2); }
#define _POLY1(x, c)	((c)[0] * (x) + (c)[1])
#define _POLY2(x, c)	(_POLY1((x), (c)) * (x) + (c)[2])
#define _POLY3(x, c)	(_POLY2((x), (c)) * (x) + (c)[3])
#define _POLY4(x, c)	(_POLY3((x), (c)) * (x) + (c)[4])
#define _POLY5(x, c)	(_POLY4((x), (c)) * (x) + (c)[5])
#define _POLY6(x, c)	(_POLY5((x), (c)) * (x) + (c)[6])
#define _POLY7(x, c)	(_POLY6((x), (c)) * (x) + (c)[7])
#define _POLY8(x, c)	(_POLY7((x), (c)) * (x) + (c)[8])
#define _POLY9(x, c)	(_POLY8((x), (c)) * (x) + (c)[9])


#define NOEXC		0	/* no exception to report */
#define DOMAIN		1
#define	SING		2
#define	OVERFLOW	3
#define	UNDERFLOW	4
#define	TLOSS		5
#define	PLOSS		6

#endif /*  __STRICT_STDC__ */


#define __MATH_H

#endif /* __MATH_H */

