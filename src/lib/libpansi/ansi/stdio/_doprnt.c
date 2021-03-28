#ident	"@(#)_doprnt.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <syscall.h>
#include "_locale.h"
#include "nan.h"
#include "values.h"

/* Maximum (positive) exponent */
#if u3b || M32
#define _EXPMAX 310
#else
#define _EXPMAX 40
#endif

/* Maximum number of digits in any integer representation */
#define _MAXDIGS 11

/* Maximum total number of digits in E format */
#if u3b || M32 || never
#define _MAXECVT 17
#else			
#define _MAXECVT 18	/*  hanna: let's try 18 */
#endif

/* Maximum number of digits after decimal point in F format */
#define _MAXFCVT 60

/* Maximum significant figures in a floating-point number */
#define _MAXFSIG _MAXECVT

/* Maximum number of characters in an exponent */
#if u3b || M32 || never
#define _MAXESIZ 5
#else
#define _MAXESIZ 4	/* hanna:if we have 18 mantissa, must we have 4 here? */
#endif

/* Convert a number between 0 and 9 to the corresponding digit */
#define _todigit(x) ((x)+'0')

/* Max and Min macros */
#define _max(a,b) ((a) > (b)? (a): (b))
#define _min(a,b) ((a) < (b)? (a): (b))

#define _PUT(p, n)     { register unsigned char *newbufptr; \
	              if ((newbufptr = bufptr + n) > bufferend) { \
		              if (_dowrite(p, n, iop, &bufptr) <= 0) goto get_out; \
                      } else { \
		              memcpy((void *) bufptr,(void *) p, n); \
			      bufptr = newbufptr; \
                      } \
		    } 
#define _PAD(s,n)    { register int nn; \
		    for (nn = n; nn > 20; nn -= 20) \
			       if (_dowrite(s, 20, iop, &bufptr) <= 0) goto get_out; \
                    _PUT(s, nn); \
                   }

#define _SNLEN     5    	/* Length of string used when printing a NaN */

/* bit positions for flags used in doprnt */

#define _LENGTH 	  1	/* l */
#define _DLENGTH  1024 /* L ,MK*/
#define _FPLUS     2	/* + */
#define _FMINUS	  4	/* - */
#define _FBLANK	  8	/* blank */
#define _FSHARP	 16	/* # */
#define _PADZERO  32	/* padding zeroes requested via '0' */
#define _DOTSEEN  64	/* dot appeared in format specification */
#define _SUFFIX 	128	/* a suffix is to appear in the output */
#define _RZERO	256	/* there will be trailing zeros in output */
#define _LZERO	512	/* there will be leading zeroes in output */

static char _blanks[] = "                    ";
static char _zeroes[] = "00000000000000000000";
static char _uc_digs[] = "0123456789ABCDEF";
static char _lc_digs[] = "0123456789abcdef";
static char  _lc_nan[] = "nan0x";
static char  _uc_nan[] = "NAN0X";
static char  _lc_inf[] = "inf";
static char  _uc_inf[] = "INF";

int _doprnt(char *format,va_list args,FILE *iop)
{
                
	/* bufptr is used inside of doprnt instead of iop->_ptr; */
	/* bufferend is a copy of _bufend(iop), if it exists.  For */
	/* dummy file descriptors (iop->_file = FOPEN_MAX), bufferend */
	/* may be meaningless.				*/

	unsigned char *bufptr, *bufferend;

	/* This variable counts output characters. */
	int	count = 0;

	/* Starting and ending points for value to be printed */
	register char	*bp;
	char *p;

	/* Field width and precision */
	int	width, prec;

	/* Format code */
	register int	fcode;

	/* Number of padding zeroes required on the left and right */
	int	lzero, rzero;

	/* Flags - bit positions defined by _LENGTH, _FPLUS, _FMINUS, _FBLANK, */
	/* and _FSHARP are set if corresponding character is in format */
	/* Bit position defined by _PADZERO means extra space in the field */
	/* should be padded with leading zeroes rather than with blanks */
	register long	flagword;

	/* Values are developed in this buffer */
	char	buf[_max(_MAXDIGS, 1+_max(_MAXFCVT+_EXPMAX, _MAXECVT))];

	/* Pointer to sign, "0x", "0X", or empty */
	char	*prefix;

	/* Exponent or empty */
	char	*suffix;

	/* Buffer to create exponent */
	char	expbuf[_MAXESIZ + 1];

	/* Length of prefix and of suffix */
	int	prefixlength, suffixlength;

	/* Combined length of leading zeroes, trailing zeroes, and suffix */
	int 	otherlength;

	/* The value being converted, if integer */
	long int val;

	/* The value being converted, if real */
	double	dval;

	/* Output values from _fcvt and _ecvt */
	int	decpt, sign;

	/* Pointer to a translate table for digits of whatever radix */
	char	*tab;

	/* Work variables */
	int	k, lradix, mradix;

	/* Variables used to flag an infinities and nans, resp. */
	/* Nan_flg is used with two purposes: to flag a NaN and */
	/* as the length of the string ``NAN0X'' (``nan0x'') */
	 int	 Inf = 0, NaN_flg = 0 ;

	/* Pointer to string "NAN0X" or "nan0x" */
	 char	 *SNAN ;

     /* Flag for negative infinity */
	 int neg_inf = 0;

	 int h_specified = 0;

	/* initialize buffer pointer and buffer end pointer */
	{ register int fno = iop->_file;
	bufptr = iop->_ptr;
#ifndef DYNAMIC
	bufferend = (fno == FOPEN_MAX) ? 
#else /* DYNAMIC */
	bufferend = (iop->_flag & _IOSTRING) ?
#endif /* DYNAMIC */
			(unsigned char *)((long) bufptr | (-1L & ~HIBITL))
#ifndef DYNAMIC
				 : _bufendtab[fno];
#else /* DYNAMIC */
				 : _bufend(iop);
#endif /* DYNAMIC */
	}

	/*
	 *	The main loop -- this loop goes through one iteration
	 *	for each string of ordinary characters or format specification.
	 */
	for ( ; ; ) {
		register int n;

		if ((fcode = *format) != '\0' && fcode != '%') {
			bp = format;
			do {
				format++;
			} while ((fcode = *format) != '\0' && fcode != '%');
		
			count += (n = format - bp); /* n = no. of non-% chars */
			_PUT(bp, n);
		}
		if (fcode == '\0') {  /* end of format; return */
			register int nn = bufptr - iop->_ptr;
			iop->_cnt -= nn;
			iop->_ptr = bufptr;
			if (bufptr + iop->_cnt > bufferend &&
			 /* in case of interrupt */
#ifndef DYNAMIC
					iop->_file != FOPEN_MAX)
#else /* DYNAMIC */
					(!(iop->_flag & _IOSTRING)))
#endif /* DYNAMIC */
				_BUFSYNC(iop);	/* during last several lines */
			if (iop->_flag & (_IONBF | _IOLBF) &&
				    (iop->_flag & _IONBF ||
				     memchr((void *)(bufptr-count),'\n',(size_t)count)!=NULL))
				 _xflsbuf(iop);
get_out:
			return(ferror(iop) ? EOF : count);
		}

		/*
		 *	% has been found.
		 *	The following switch is used to parse the format
		 *	specification and to perform the operation specified
		 *	by the format letter.  The program repeatedly goes
		 *	back to this switch until the format letter is
		 *	encountered.
		 */
		width = prefixlength = otherlength = flagword = suffixlength = 0;
		format++;

	charswitch:

		switch (fcode = *format++) {

		case '+':
			flagword |= _FPLUS;
			goto charswitch;
		case '-':
			flagword |= _FMINUS;
			goto charswitch;
		case ' ':
			flagword |= _FBLANK;
			goto charswitch;
		case '#':
			flagword |= _FSHARP;
			goto charswitch;
		case '0':
			if (!(flagword & (_DOTSEEN | _FMINUS)))
				flagword |= _PADZERO;
			goto charswitch;

		/* Scan the field width and precision */
		case '.':
			flagword |= _DOTSEEN;
			prec = 0;
			goto charswitch;

		case '*':
			if (!(flagword & _DOTSEEN)) {
				width = va_arg(args, int);
				if (width < 0) {
					width = -width;
					flagword ^= _FMINUS;
				}
			} else {
				prec = va_arg(args, int);

				/* see ANSI 4.9.6.1  paragraph 25 */
				if (prec < 0)
					flagword &= ~_DOTSEEN;
			}
			goto charswitch;

		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		      { register num = fcode - '0';
			while (isdigit(fcode = *format)) {
				num = num * 10 + fcode - '0';
				format++;
			}
			if (flagword & _DOTSEEN)
				prec = num;
			else
				width = num;
			goto charswitch;
		      }

		/* Scan the length modifier */
		case 'l':
			flagword |= _LENGTH;
			goto charswitch;
		case 'L':
			flagword |= _DLENGTH;
			/* No break */
		case 'h':
			h_specified = 1;
			goto charswitch; 

		/*
		 *	The character addressed by format must be
		 *	the format letter -- there is nothing
		 *	left for it to be.
		 *
		 *	The status of the +, -, #, and blank
		 *	flags are reflected in the variable
		 *	"flagword".  "width" and "prec" contain
		 *	numbers corresponding to the digit
		 *	strings before and after the decimal
		 *	point, respectively. If there was no
		 *	decimal point, then flagword & _DOTSEEN
		 *	is false and the value of prec is meaningless.
		 *
		 *	The following switch cases set things up
		 *	for printing.  What ultimately gets
		 *	printed will be padding blanks, a
		 *	prefix, left padding zeroes, a value,
		 *	right padding zeroes, a suffix, and
		 *	more padding blanks.  Padding blanks
		 *	will not appear simultaneously on both
		 *	the left and the right.  Each case in
		 *	this switch will compute the value, and
		 *	leave in several variables the informa-
		 *	tion necessary to construct what is to
		 *	be printed.  
		 *
		 *	The prefix is a sign, a blank, "0x",
		 *	"0X", or null, and is addressed by
		 *	"prefix".
		 *
		 *	The suffix is either null or an
		 *	exponent, and is addressed by "suffix".
		 *	If there is a suffix, the flagword bit
		 *	_SUFFIX will be set.
		 *
		 *	The value to be printed starts at "bp"
		 *	and continues up to and not including
		 *	"p".
		 *
		 *	"lzero" and "rzero" will contain the
		 *	number of padding zeroes required on
		 *	the left and right, respectively.
		 *	The flagword bits _LZERO and _RZERO tell
		 *	whether padding zeros are required.
		 *
		 *	The number of padding blanks, and
		 *	whether they go on the left or the
		 *	right, will be computed on exit from
		 *	the switch.
		 */



		
		/*
		 *	decimal fixed point representations
		 *
		 *	HIBITL is 100...000
		 *	binary, and is equal to	the maximum
		 *	negative number.
		 *	We assume a 2's complement machine
		 */

		case 'i':
		case 'd':
			if ( flagword & _DOTSEEN )
				flagword &= ~_PADZERO; 	/* ANSI 4.9.6.1, see section on flag 0*/

			/* Fetch the argument to be printed */
			if (flagword & _LENGTH)
				val = va_arg(args, long);
			else if (!(flagword & _DLENGTH) && h_specified)
				val = va_arg(args, short);
			else
				val = va_arg(args, int);

			/* Set buffer pointer to last digit */
	                p = bp = buf + _MAXDIGS;

			/* If signed conversion, make sign */
			if (val < 0) {
				prefix = "-";
				prefixlength = 1;
				/*
				 * Negate, checking in
				 * advance for possible
				 * overflow.
				 */
				if (val != HIBITL)
					val = -val;
				else     /* number is -HIBITL; convert last */
					 /* digit now and get positive number */
					*--bp = _lowdigit(&val);
			} else if (flagword & _FPLUS) {
				prefix = "+";
				prefixlength = 1;
			} else if (flagword & _FBLANK) {
				prefix = " ";
				prefixlength = 1;
			}

		decimal:
			{ register long qval = val;
				if (qval <= 9) {
					if (qval != 0 || !(flagword & _DOTSEEN))
						*--bp = qval + '0';
				} else {
					do {
						n = qval;
						qval /= 10;
						*--bp = n - qval * 10 + '0';
					} while (qval > 9);
					*--bp = qval + '0';
				}
			}

			/* Calculate minimum padding zero requirement */
			if (flagword & _DOTSEEN) {
				register leadzeroes = prec - (p - bp);
				if (leadzeroes > 0) {
					otherlength = lzero = leadzeroes;
					flagword |= _LZERO;
				}
			}

			break;

		case 'u':
			if ( flagword & _DOTSEEN )
				flagword &= ~_PADZERO; 	/* ANSI 4.9.6.1, see section on flag 0*/
			/* Fetch the argument to be printed */
			if (flagword & _LENGTH)
				val = va_arg(args, long);
			else if (!(flagword & _DLENGTH) && h_specified)
				val = va_arg(args, unsigned short);
			else
				val = va_arg(args, unsigned int);

			p = bp = buf + _MAXDIGS;

			if (val & HIBITL)
				*--bp = _lowdigit(&val);

			goto decimal;

		/*
		 *	non-decimal fixed point representations
		 *	for radix equal to a power of two
		 *
		 *	"mradix" is one less than the radix for the conversion.
		 *	"lradix" is one less than the base 2 log
		 *	of the radix for the conversion. Conversion is unsigned.
		 *	HIBITL is 100...000
		 *	binary, and is equal to	the maximum
		 *	negative number.
		 *	We assume a 2's complement machine
		 */

		case 'o':
			if ( flagword & _DOTSEEN )
				flagword &= ~_PADZERO; 	/* ANSI 4.9.6.1, see section on flag 0*/
			mradix = 7;
			lradix = 2;
			goto fixed;

		case 'X':
		case 'x':
			if ( flagword & _DOTSEEN )
				flagword &= ~_PADZERO; 	/* ANSI 4.9.6.1, see section on flag 0*/
		case 'p':
			mradix = 15;
			lradix = 3;

		fixed:
			/* Fetch the argument to be printed */
			if (flagword & _LENGTH)
				val = va_arg(args, long);
			else if (!(flagword & _DLENGTH) && h_specified)
				val = va_arg(args, unsigned short);
			else
				val = va_arg(args, unsigned int);

			/* Set translate table for digits */
			  tab = (fcode == 'X') ? _uc_digs : _lc_digs;

			/* Entry point when printing a double which is a NaN */
		put_pc:
			/* Develop the digits of the value */
			p = bp = buf + _MAXDIGS;
			{ register long qval = val;
				if (qval == 0) {
					if (!(flagword & _DOTSEEN)) {
						otherlength = lzero = 1;
						flagword |= _LZERO;
					}
				} else
					do {
						*--bp = tab[qval & mradix];
						qval = ((qval >> 1) & ~HIBITL)
								 >> lradix;
					} while (qval != 0);
			}

			/* Calculate minimum padding zero requirement */
			if (flagword & _DOTSEEN) {
				register leadzeroes = prec - (p - bp);
				if (leadzeroes > 0) {
					otherlength = lzero = leadzeroes;
					flagword |= _LZERO;
				}
			}

			/* Handle the # flag */
			if (flagword & _FSHARP && val != 0)
				switch (fcode) {
				case 'o':
					if (!(flagword & _LZERO)) {
						otherlength = lzero = 1;
						flagword |= _LZERO;
					}
					break;
				case 'x':
					prefix = "0x";
					prefixlength = 2;
					break;
				case 'X':
					prefix = "0X";
					prefixlength = 2;
					break;
				}

			break;

		case 'E':
		case 'e':
			/*
			 * E-format.  The general strategy
			 * here is fairly easy: we take
			 * what _ecvt gives us and re-format it.
			 */

			/* Establish default precision */
			if (!(flagword & _DOTSEEN))
				prec = 6;

			/* Fetch the value */
	                dval = va_arg(args, double);

                        /* Check for NaNs and Infinities */
			if (IsNANorINF(dval))  {
			   if (IsINF(dval)) {
			      if (IsNegNAN(dval)) 
				neg_inf = 1;
			      Inf = 1;
			      bp = (fcode == 'E')? _uc_inf: _lc_inf;
			      p = bp + 3;
			      break;
                           }
			   else {
				if (IsNegNAN(dval)) {
					prefix = "-";
					prefixlength = 1;
				}
			        val  = GETNaNPC(dval); 
				NaN_flg = _SNLEN;
				mradix = 15;
				lradix = 3;
				if (fcode == 'E') {
					SNAN = _uc_nan;
					tab =  _uc_digs;
				}
				else {
					SNAN =  _lc_nan;
					tab =  _lc_digs;
				}
				goto put_pc;
                           }
			}
			/* Develop the mantissa */
#ifdef _ANSI 
			bp = _ecvt(dval, _min(prec + 1, _MAXECVT), &decpt, &sign);
#else
			bp = ecvt(dval, _min(prec + 1, _MAXECVT), &decpt, &sign);
#endif /* _ANSI */

			/* Determine the prefix */
		e_merge:
			if (sign) {
				prefix = "-";
				prefixlength = 1;
			} else if (flagword & _FPLUS) {
				prefix = "+";
				prefixlength = 1;
			} else if (flagword & _FBLANK) {
				prefix = " ";
				prefixlength = 1;
			}

			/* Place the first digit in the buffer*/
			p = &buf[0];
			*p++ = (*bp != '\0') ? *bp++ : '0';

			/* Put in a decimal point if needed */
			if (prec != 0 || (flagword & _FSHARP))
				*p++ = '.';

			/* Create the rest of the mantissa */
			{ register rz = prec;
				for ( ; rz > 0 && *bp != '\0'; --rz)
					*p++ = *bp++;
				if (rz > 0) {
					otherlength = rzero = rz;
					flagword |= _RZERO;
				}
			}

			bp = &buf[0];

			/* Create the exponent */
			*(suffix = &expbuf[_MAXESIZ]) = '\0';
			if (dval != 0) {
				register int nn = decpt - 1;
				if (nn < 0)
				    nn = -nn;
				for ( ; nn > 9; nn /= 10)
					*--suffix = _todigit(nn % 10);
				*--suffix = _todigit(nn);
			}

			/* Prepend leading zeroes to the exponent */
			while (suffix > &expbuf[_MAXESIZ - 2])
				*--suffix = '0';

			/* Put in the exponent sign */
			*--suffix = (decpt > 0 || dval == 0) ? '+' : '-';

			/* Put in the e */
			*--suffix = isupper(fcode) ? 'E'  : 'e';

			/* compute size of suffix */
			otherlength += (suffixlength = &expbuf[_MAXESIZ]
								 - suffix);
			flagword |= _SUFFIX;

			break;

		case 'f':
			/*
			 * F-format floating point.  This is a
			 * good deal less simple than E-format.
			 * The overall strategy will be to call
			 * _fcvt, reformat its result into buf,
			 * and calculate how many trailing
			 * zeroes will be required.  There will
			 * never be any leading zeroes needed.
			 */

			/* Establish default precision */
			if (!(flagword & _DOTSEEN))
				prec = 6;

			/* Fetch the value */
			dval = va_arg(args, double);

                        /* Check for NaNs and Infinities  */
			if (IsNANorINF(dval)) {
			   if (IsINF(dval)) {
			      if (IsNegNAN(dval))
				neg_inf = 1;
                              Inf = 1;
			      bp = _lc_inf;
			      p = bp + 3;
			      break;
                           }
                           else {
				if (IsNegNAN(dval)) {
					prefix = "-";
					prefixlength = 1;
				}
			        val  = GETNaNPC(dval);
				NaN_flg = _SNLEN;
				mradix = 15;
				lradix = 3;
				tab =  _lc_digs;
				SNAN = _lc_nan;
				goto put_pc;
                           }
                        } 
			/* Do the conversion */
#ifdef _ANSI
			bp = _fcvt(dval, _min(prec, _MAXFCVT), &decpt, &sign);
#else
			bp = fcvt(dval, _min(prec, _MAXFCVT), &decpt, &sign);
#endif /* _ANSI */

			/* Determine the prefix */
		f_merge:
			if (sign && decpt > -prec && *bp != '0') {
				prefix = "-";
				prefixlength = 1;
			} else if (flagword & _FPLUS) {
				prefix = "+";
				prefixlength = 1;
			} else if (flagword & _FBLANK) {
				prefix = " ";
				prefixlength = 1;
			}

			/* Initialize buffer pointer */
			p = &buf[0];

			{ register int nn = decpt;

				/* Emit the digits before the decimal point */
				k = 0;
				do {
					*p++ = (nn <= 0 || *bp == '\0' 
						|| k >= _MAXFSIG) ?
				    		'0' : (k++, *bp++);
				} while (--nn > 0);

				/* Decide whether we need a decimal point */
				if ((flagword & _FSHARP) || prec > 0)
					*p++ = '.';

				/* Digits (if any) after the decimal point */
				nn = _min(prec, _MAXFCVT);
				if (prec > nn) {
					flagword |= _RZERO;
					otherlength = rzero = prec - nn;
				}
				while (--nn >= 0)
					*p++ = (++decpt <= 0 || *bp == '\0' ||
				   	    k >= _MAXFSIG) ? '0' : (k++, *bp++);
			}

			bp = &buf[0];

			break;

		case 'G':
		case 'g':
			/*
			 * g-format.  We play around a bit
			 * and then jump into e or f, as needed.
			 */
		
			/* Establish default precision */
			if (!(flagword & _DOTSEEN))
				prec = 6;
			else if (prec == 0)
				prec = 1;

			/* Fetch the value */
			dval = va_arg(args, double);

			/* Check for NaN and Infinities  */
			if (IsNANorINF(dval)) {
			   if (IsINF(dval)) {
			      if (IsNegNAN(dval)) 
				neg_inf = 1;
			   bp = (fcode == 'G') ? _uc_inf : _lc_inf;
			   p = bp + 3;
                           Inf = 1;
			   break;
                           }
                           else {
				if (IsNegNAN(dval)) {
					prefix = "-";
					prefixlength = 1;
				}
			        val  = GETNaNPC(dval);
				NaN_flg = _SNLEN;
				mradix = 15;
				lradix = 3;
				if ( fcode == 'G') {
					SNAN = _uc_nan;
					tab = _uc_digs;
				}
				else {
					SNAN = _lc_nan;
					tab =  _lc_digs;
				}
				goto put_pc;
                           }
                        }

			/* Do the conversion */
#ifdef _ANSI 
			bp = _ecvt(dval, _min(prec, _MAXECVT), &decpt, &sign);
#else
			bp = ecvt(dval, _min(prec, _MAXECVT), &decpt, &sign);
#endif /* _ANSI */
			if (dval == 0)
				decpt = 1;

			{ register int kk = prec;
				if (!(flagword & _FSHARP)) {
					n = (int) strlen(bp);
					if (n < kk)
						kk = n;
					while (kk >= 1 && bp[kk-1] == '0')
						--kk;
				}
				
				if (decpt < -3 || decpt > prec) {
					prec = kk - 1;
					goto e_merge;
				}
				prec = kk - decpt;
				goto f_merge;
			}

		case '%':
			buf[0] = fcode;
			goto c_merge;

		case 'c':
			buf[0] = va_arg(args, int);
		c_merge:
			p = (bp = &buf[0]) + 1;
			break;

		case 's':
			bp = va_arg(args, char *);
			if (!(flagword & _DOTSEEN))
				p = bp + strlen(bp);
			else { /* a strnlen function would  be useful here! */
				register char *qp = bp;
				while (*qp++ != '\0' && --prec >= 0)
					;
				p = qp - 1;
			}
			break;

		case 'n': 
			if (flagword & _LENGTH) 
				*va_arg(args,long *) = (long) count;
			else 
				if (h_specified) 
					*va_arg(args,short *) = (short) count; 
				else 
					*va_arg(args,int *) = count;
			continue;

		default: /* this is technically an error; what we do is to */
			/* back up the format pointer to the offending char */
			/* and continue with the format scan */
			format--;
			continue;

		}
       
		if (Inf) {
		   if (neg_inf) {
			prefix = "-";
			prefixlength = 1;
			neg_inf = 0;
                   }
		   else if (flagword & _FPLUS) {
			prefix = "+";
			prefixlength = 1;
			}
		   else if (flagword & _FBLANK) {
			prefix = " ";
			prefixlength = 1;
		   }
		   Inf = 0;
		}
		 
		/* Calculate number of padding blanks */
		k = (n = p - bp) + prefixlength + otherlength + NaN_flg;
		if (width <= k)
			count += k;
		else {
			count += width;

			/* Set up for padding zeroes if requested */
			/* Otherwise emit padding blanks unless output is */
			/* to be left-justified.  */

			if (flagword & _PADZERO)
			{
				if (!(flagword & _LZERO)) {
					flagword |= _LZERO;
					lzero = width - k;
				}
				else lzero += width - k;
				k = width; /* cancel padding blanks */
			} else
				/* Blanks on left if required */
				if (!(flagword & _FMINUS))
					_PAD(_blanks, width - k);
		}

		/* Prefix, if any */
		if (prefixlength != 0)
			_PUT(prefix, prefixlength);

		/* If value is NaN, put string NaN*/
		if (NaN_flg) {
			_PUT(SNAN,_SNLEN);
			NaN_flg = 0;
                }

		/* Zeroes on the left */
		if (flagword & _LZERO)
			_PAD(_zeroes, lzero);
		
		/* The value itself */
		if (n > 0)
			_PUT(bp, n);

		if (flagword & (_RZERO | _SUFFIX | _FMINUS)) {
			/* Zeroes on the right */
			if (flagword & _RZERO)
				_PAD(_zeroes, rzero);

			/* The suffix */
			if (flagword & _SUFFIX)
				_PUT(suffix, suffixlength);

			/* Blanks on the right if required */
			if (flagword & _FMINUS && width > k)
				_PAD(_blanks, width - k);
		}
	}
}
