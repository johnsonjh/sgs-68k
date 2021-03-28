/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#ifndef _MATH_H 
#ident	"@(#)math.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define _MATH_H
#pragma SYS

/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#define HUGE_VAL 	1.79769313486231470e+308

#if !defined __STRICT_STDC__

/* some useful constants */

#define M_E			2.7182818284590452354
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

#endif /* ! __STRICT_STDC__ */


/*****************************************************************************
                                  T Y P E S
 ****************************************************************************/

#if !defined __STRICT_STDC__

struct exception {
	int type;
	char *name;
	double arg1;
	double arg2;
	double retval;
};

#endif	/* ! __STRICT_STDC__ */

/*****************************************************************************
                   E X T E R N A L    D E C L A R A T I O N S 
 ****************************************************************************/

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

double acos _PARMS((double));
double asin _PARMS((double));
double atan _PARMS((double));
double atan2 _PARMS(( double, double ));
double ceil _PARMS((double));
double cos _PARMS((double));
double cosh _PARMS((double));
double exp _PARMS((double));
double fabs _PARMS((double));
double floor _PARMS((double));
double fmod _PARMS(( double, double ));
double frexp _PARMS((double,int *));
double ldexp _PARMS((double,int));
double log _PARMS((double));
double log10 _PARMS((double));
double modf _PARMS((double,double *));
double pow _PARMS((double,double));
double sin _PARMS((double));
double sinh _PARMS((double));
double sqrt _PARMS((double));
double tan _PARMS((double));
double tanh _PARMS((double));


#if !defined ___STRICT_STDC__ 

extern double atof _PARMS((const char *));

/* Bessel functions */
extern double j0 _PARMS((double ));
extern double j1 _PARMS((double ));
extern double jn _PARMS((int, double ));
extern double y0 _PARMS((double ));
extern double y1 _PARMS((double ));
extern double yn _PARMS((int, double ));

extern double erf _PARMS((double ));
extern double erfc _PARMS((double ));

extern double gamma _PARMS((double) );

extern double hypot _PARMS((double, double ));

extern unsigned int acos881 _PARMS((double, double * ));
extern unsigned int asin881 _PARMS((double, double * ));
extern unsigned int atan881 _PARMS((double, double * ));
extern unsigned int cos881 _PARMS((double, double * ));
extern unsigned int cosh881 _PARMS((double, double * ));
extern unsigned int etox881 _PARMS((double, double * ));
extern unsigned int log881 _PARMS((double, double * ));
extern unsigned int logn881 _PARMS((double, double * ));
extern unsigned int sin881 _PARMS((double, double * ));
extern unsigned int sinh881 _PARMS((double, double * ));
extern unsigned int sqrt881 _PARMS((double, double * ));
extern unsigned int tan881 _PARMS((double, double * ));
extern unsigned int tanh881 _PARMS((double, double * ));

extern int signgam;
#if __STDC__ > 0
extern volatile int errno;
#else
extern int errno;
#endif

#endif	/* ! __STRICT_STDC__ */

#pragma noSYS
#endif	/* _MATH_H */
