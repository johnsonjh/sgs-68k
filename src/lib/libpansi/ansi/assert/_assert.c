#ident	"@(#)_assert.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int _assert(char *assertion,char *filename,int line_num)
{
	static char linestr[] 	= ", line NNNNN\n";
	register char *p 		= &linestr[7];
	register int div, digit;
	
	fwrite((void *) "Assertion failed: ",18,1,stdout);
	fwrite((void *) assertion,strlen(assertion),(size_t) 1,stdout);
	fwrite((void *) ", file ",7,1,stdout);
	fwrite((void *) filename,strlen(filename),1,stdout);
	for (div = 10000; div != 0; line_num %= div, div /= 10)
		if ((digit = line_num/div) != 0 || p != &linestr[7] || div == 1)
			*p++ = digit + '0';
	*p++ = '\n';
	*p = '\0';
	fwrite((void *) linestr,strlen(linestr),1,stdout);
	abort();
}
