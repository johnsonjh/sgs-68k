#ident	"@(#)experror.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/perror.c	1.7"	*/

/*
 * experror.c -- a hacked version of perror that knows about EXOS err codes
 */
	

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
/*
 * Print the error indicated
 * in the cerror cell.
 */

/*
 * EXOS 3.3 error codes
 */
#ifdef S3000
#define NFSEBASE	10000
#define EXMINERR	35
#define EXMAXERR	65
#define MINEXERROR	(NFSEBASE+EXMINERR)
#define MAXEXERROR	(NFSEBASE+EXMAXERR)

char	*ex_errlist[] = {
"Operation would block",
"Operation now in progress",
"Operation already in progress",
"Socket operation on non-socket",
"Destination address required",
"Message too long",
"Protocol wrong type for socket",
"Protocol not available",
"Protocol not supported",
"Socket type not supported",
"Operation not supported on socket",
"Protocol family not supported",
"Address family not supported by protocol family",
"Address already in use",
"Can't assign requested address",
"Network is down",
"Network is unreachable",
"Network dropped connection on reset",
"Software caused connection abort",
"Connection reset by peer",
"No buffer space available",
"Socket is already connected",
"Socket is not connected",
"Can't send after socket shutdown",
"Too many references: can't splice",
"Connection timed out",
"Connection refused",
"Too many levels of symbolic links",
"File name too long",
"Host is down",
"No route to host"
};
/* end of EXOS 3.3 err codes */

extern int	errno, sys_nerr, strlen(), write();
extern void	perror();

void
experror(s)
char	*s;
{
	register char	*c;
	register int	n;

	if (errno >= 0 && errno < sys_nerr) {
		perror(s);
		return;
	}
	else if (errno >= MINEXERROR && errno <= MAXEXERROR) {
		c = ex_errlist[errno - MINEXERROR];
	}
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

/*	#define DEBUG		/* */

#ifdef DEBUG

main()
{
	register int	i;

	experror("Test");
	for (i = MINEXERROR; i <= MAXEXERROR; ++i) {
		errno = i;
		experror("Test");
	}
	for (i = -10000000; i < 10000000; i += 174567) {
		errno = i;
		experror("Test");
	}
	return (0);
}
#endif	/* DEBUG */
#endif
