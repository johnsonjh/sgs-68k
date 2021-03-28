#ident	"@(#)print.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:print/print.h	1.8"	*/
	

/* Maximum number of digits in any integer representation */
#define MAXDIGS 11

/* Maximum total number of digits in E format */
#if u3b || M32 || never
#define MAXECVT 17
#else			
#define MAXECVT 18	/*  hanna: let's try 18 */
#endif

/* Maximum number of digits after decimal point in F format */
#define MAXFCVT 60

/* Maximum significant figures in a floating-point number */
#define MAXFSIG MAXECVT

/* Maximum number of characters in an exponent */
#if u3b || M32 || never
#define MAXESIZ 5
#else
#define MAXESIZ 4	/* hanna:if we have 18 mantissa, must we have 4 here? */
#endif

/* Maximum (positive) exponent */
#define MAXEXP 308
/* hanna: we do have a large exponent, but not that large
#if u3b || M32 |
#define MAXEXP 310
#else
#define MAXEXP 40
#endif
*/

/* Data type for flags */
typedef char bool;

/* Convert a digit character to the corresponding number */
#define tonumber(x) ((x)-'0')

/* Convert a number between 0 and 9 to the corresponding digit */
#define todigit(x) ((x)+'0')

/* Max and Min macros */
#define max(a,b) ((a) > (b)? (a): (b))
#define min(a,b) ((a) < (b)? (a): (b))
