/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) inline.c: version 25.1 created on 12/2/91 at 14:12:17	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)inline.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"lint:inline.c	1.1"	*/
	
#ident	"@(#)cxref/other:inline.c	25.1"

#include <ctype.h>
#include "comp/mfile1"

/*
/*
/* A special version of inline.c so lint cat recognize the new
/* version of asms
/*
/*
*/

extern char inlargs[];
extern int ninlargs;


squirrelmacro(name) 	/* called from grammar to put it away */
char *name;
{
	int c;


	while ((c = getchar()) != '}')
	{
		if (c == EOF)
		{
			cerror("EOF in asm pseudo-function definition");
			/*NOTREACHED*/
		}
		if (c == '\n')
		{
			++lineno;
			if ((c = getchar()) == '%')
			{
			}
			else
				ungetc(c, stdin);
		}

	}
	/* make parser see "{}" to belive it's a function */
	ungetc('}', stdin);	
	/* clear argument list and count */
	inlargs[0] = '\0';
	ninlargs = 0;
}
