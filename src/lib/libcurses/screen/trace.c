/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) trace.c: version 25.1 created on 12/2/91 at 19:27:38	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)trace.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/trace.c	1.8"	*/
	
#ident	"@(#)lib/libcurses:trace.c	25.1"

#include "curses_inc.h"

traceon()
{
#ifdef DEBUG
    if (outf == NULL)
    {
	outf = fopen("trace", "a");
	if (outf == NULL)
	{
	    perror("trace");
	    exit(-1);
	}
	fprintf(outf, "trace turned on\n");
    }
#endif /* DEBUG */
    return (OK);
}

traceoff()
{
#ifdef DEBUG
    if (outf != NULL)
    {
	fprintf(outf, "trace turned off\n");
	fclose(outf);
	outf = NULL;
    }
#endif /* DEBUG */
    return (OK);
}

#ifdef DEBUG
#include <ctype.h>

char *
_asciify(str)
register char *str;
{
    static	char	string[1024];
    register	char	*p1 = string;
    register	char	*p2;
    register	char	c;

    while (c = *str++)
    {
	p2 = unctrl(c);
	while (*p1 = *p2++)
	    p1++;
    }
    return string;
}
#endif /* DEBUG */
