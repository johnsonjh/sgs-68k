#ident	"@(#)ecvt.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/ecvt.c	2.11"	*/
	

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
/*
 *	ecvt converts to decimal
 *	the number of digits is specified by ndigit
 *	decpt is set to the position of the decimal point
 *	sign is set to 0 for positive, 1 for negative
 *
 */
/* sw0
 * Fri May 30 10:45:49 PDT 1986
 * hanna
 * In short, printf("%e %e\n",1e-37,1e-35);
 * actually prints :.0e-38 :.0e-36
 * The problem is that we leave the 68881 programmed to round to extended
 * precision.
 * In the loop that:
 *	for ( ; value * sp->p10 < 10.0; *decpt -= sp->n)
 *		value *= sp->p10;
 * value is continually multiplied to a smidgen under 0.1 0x3bf999999999999
 * on the next loop to check if value * sp->p10 (=100) is less than 10.0, 
 * value got rounded up to 0x4024000000000000 and the test failed.
 * The test done with rounding set to double precision works.
 * I think this is the solution.
 *
 * sw1
 * Wed Jun 18 08:05:10 PDT 1986
 * hanna
 * Return huge, not maxdouble, on error.
 */
#include "shlib.h"
#include <values.h>
#include <nan.h>
#define	NMAX	((DSIGNIF * 3 + 19)/10) /* restrict max precision */
#define	NDIG	80
/* #define CHNGinf(X) (X=(X>0.0 ? MAXDOUBLE : -MAXDOUBLE)) #defined in nan.h */

extern char *cvt();

char *
ecvt(value, ndigit, decpt, sign)
double	value;
int	ndigit, *decpt, *sign;
{
	return (cvt(value, ndigit, decpt, sign, 0));
}

char *
fcvt(value, ndigit, decpt, sign)
double	value;
int	ndigit, *decpt, *sign;
{
	return (cvt(value, ndigit, decpt, sign, 1));
}

static char buf[NDIG];

static char *
cvt(value, ndigit, decpt, sign, f_flag)
double value;
int	ndigit, *sign, f_flag;
register int	*decpt;
{
	register char *p = &buf[0], *p_last = &buf[ndigit];
	unsigned int fpcr;
	if (IsNANorINF(value))
	  {
	    if (!IsINF(value)) 
	  	KILLFPE();	
	    else
		CHNGinf(value);
	  }
	fpcr=rd881_control();	/* sw0 */

/*	KILLNaN(value);  raise exception on Not-a-Number (3b only) */
	/* detect infinities and turn them into large #'s */
	/* hanna: comment out for now, see if handled by doprnt/doscan... 
	 if (INF(value))  CHNGinf(value);  sw1 */

	wr881_control((fpcr&0xff3f)|0x80);		/* sw0 */

	if (*sign = (value < 0.0))
		value = -value;
	buf[0] = '\0';
	*decpt = 0;
	if (value != 0.0) { /* rescale to range [1.0, 10.0) */
		/* in binary for speed and to minimize error build-up */
		/* even for the IEEE standard with its high exponents,
		   it's probably better for speed to just loop on them */
		static struct s { double p10; int n; } s[] = {
			1e32,	32,
			1e16,	16,
			1e8,	8,
			1e4,	4,
			1e2,	2,
			1e1,	1,
		};
		register struct s *sp = s;

		++*decpt;
		if (value >= 2.0 * MAXPOWTWO) /* can't be precisely integral */
			do {
				for ( ; value >= sp->p10; *decpt += sp->n)
					value /= sp->p10;
			} while (sp++->n > 1);
		else if (value >= 10.0) { /* convert integer part separately */
			register double pow10 = 10.0, powtemp;

			while ((powtemp = 10.0 * pow10) <= value)
				pow10 = powtemp;
			for ( ; ; pow10 /= 10.0) {
				register int digit = value/pow10;
				*p++ = digit + '0';
				value -= digit * pow10;
				++*decpt;
				if (pow10 <= 10.0)
					break;
			}
		
		} else if (value < 1.0){
			register double rvalue = value;
			do {
				for ( ; rvalue * sp->p10 < 10.0; *decpt -= sp->n)
					rvalue *= sp->p10;
			} while (sp++->n > 1);
		value = rvalue;
		}
	}
	if (f_flag)
		p_last += *decpt;
	if (p_last >= buf) {
		if (p_last > &buf[NDIG - 2])
			p_last = &buf[NDIG - 2];
		for ( ; ; ++p) {
			if (value == 0 || p >= &buf[NMAX])
				*p = '0';
			else {
				register int intx; /* intx in [0, 9] */
				*p = (intx = (int)value) + '0';
				value = 10.0 * (value - (double)intx);
			}
			if (p >= p_last) {
				p = p_last;
				break;
			}
		}
		if (*p >= '5') /* check rounding in last place + 1 */
			do {
				if (p == buf) { /* rollover from 99999... */
					buf[0] = '1'; /* later digits are 0 */
					++*decpt;
					if (f_flag)
						++p_last;
					break;
				}
				*p = '0';
			} while (++*--p > '9'); /* propagate carries left */
		*p_last = '\0';
	}
	wr881_control(fpcr);			/* sw0 */
	return (buf);
}
