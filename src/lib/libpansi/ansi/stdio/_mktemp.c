#ident	"@(#)_mktemp.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/****************************************************************
 *	Routine expects a string of length at least 6, with
 *	six trailing 'X's.  These will be overlaid with a
 *	letter and the last (5) digits of the proccess ID.
 *	If every letter (a thru z) thus inserted leads to
 *	an existing file name, your string is shortened to
 *	length zero upon return (first character set to '\0').
 ***************************************************************/

#include <string.h>
#include <syscall.h>
#ifdef _ANSI
#include "unistd.h"
#endif /* _ANSI */

#define _XCNT  6

#ifdef _ANSI
char *_mktemp(char *as)
#else
char *mktemp(char *as)
#endif /* _ANSI */
{
	register char *s=as;
	register unsigned int pid;
	register unsigned int xcnt=0; /* keeps track of number of X's seen */

	pid = getpid();
	s += strlen(as);	/* point at the terminal null */
	while(*--s == 'X' && ++xcnt <= _XCNT) {
		*s = (pid%10) + '0';
		pid /= 10;
	}
	if(*++s) {		/* maybe there were no 'X's */
		*s = 'a';
#ifdef _ANSI
		while(access(as, F_OK) == 0) {
#else
		while(access(as, 0) == 0) {
#endif /* _ANSI */
			if(++*s > 'z') {
				*as = '\0';
				break;
			}
		}
	} else
#ifdef _ANSI
		if(access(as, F_OK) == 0)
#else
		if(access(as, 0) == 0)
#endif /* _ANSI */
			*as = '\0';
	return(as);
}
