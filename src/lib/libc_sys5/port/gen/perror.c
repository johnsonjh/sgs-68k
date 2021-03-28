#ident	"@(#)perror.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/perror.c	1.7"	*/
	

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
/*
 * Print the error indicated
 * in the cerror cell.
 */

/*  Changes added in order to print out an "Unknown error" message, along
 *  with the errno number.
 */

extern int errno, sys_nerr, strlen(), write();
extern char *sys_errlist[];

void
perror(s)
char	*s;
{
	register char *c;
	register int n;

	if (errno >= 0 && errno < sys_nerr)
		c = sys_errlist[errno];
	else {
		int		nflag = 0;
		static char	unkn[] = "Unknown error # 12345";
#define UNKN_DIGITS	5
		c = &unkn[sizeof(unkn) - 2];
		n = errno;
		if (n < 0) {
			n = -n;
			++nflag;
		}
		do {
			*c-- = (n % 10) + '0';
		} while ((n /= 10) > 0);
		if (nflag)
			*c-- = '-';
		do
			*c = ' ';
		while (c-- > &unkn[sizeof(unkn) - UNKN_DIGITS - 1]);
		c = unkn;
	}
	n = strlen(s);
	if(n) {
		(void) write(2, s, (unsigned)n);
		(void) write(2, ": ", 2);
	}
	(void) write(2, c, (unsigned)strlen(c));
	(void) write(2, "\n", 1);
}
