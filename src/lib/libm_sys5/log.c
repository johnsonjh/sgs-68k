#ident	"@(#)log.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	ATT:#ident	"libm:log.c	1.14"	*/
	
/*LINTLIBRARY*/
/*
 *	log returns the natural logarithm of its double-precision argument.
 *	log10 returns the base-10 logarithm of its double-precision argument.
 *	Returns EDOM error and value -HUGE if argument <= 0.
 */
/* 	kd0	log was processing the wrong variable on error
 */

#include <math.h>
#include <errno.h>
#include <sys/fpu.h>

extern double log_error();

double
log(x)
register double x;
{
	double y;
	register unsigned int exc;

	exc = logn881(x, &y) & (EXC_OPER|EXC_DZ|FPCC_NAN|FPCC_I);
				/* mask all but the important status
				   bits */

	if (!(exc))
		return(y);	/* no exception, just return result */
	else
	/*	return (log_error(y, "log", 3, exc)); */
		return (log_error(x, "log", 3, exc));	/* kd0 */
}

double
log10(x)
register double x;
{
	double y;
	register unsigned int exc;

	exc = log881(x, &y) & (EXC_OPER|EXC_DZ|FPCC_NAN);
				/* mask all but the important status
				   bits */

	if (!(exc))
		return(y);	/* no exception, just return result */
	else
	/*	return (log_error(y, "log10", 5, exc));	*/
		return (log_error(x, "log10", 5, exc));	/* kd0 */
}

static double
log_error(x, f_name, name_len, exc_type)
double x;
char *f_name;
unsigned int name_len;
unsigned int exc_type;
{
	register char *err_type;
	unsigned int mess_len;
	struct exception exc;

	exc.name = f_name;
	exc.retval = -HUGE;
	exc.arg1 = x;
	if (exc_type & EXC_DZ) { /* divide by 0, returns -inf. result */
		exc.type = SING;
		err_type = ": SING error\n";
		mess_len = 13;
	}
	else if (exc_type & FPCC_I) {		/* result was an infinity */
		exc.type = OVERFLOW;
		exc.retval = HUGE;
		if (!matherr(&exc)) 
			errno = ERANGE;
		return (exc.retval);
	}
	else {
					/* only other expected exceptions are
				           domain error (out of range or
					   a NaN) */
		exc.type = DOMAIN;
		err_type = ": DOMAIN error\n";
		mess_len = 15;
	}
	if (!matherr(&exc)) {
		(void) write(2, f_name, name_len);
		(void) write(2, err_type, mess_len);
		errno = EDOM;
	}
	return (exc.retval);
}
