/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tcsearch.c: version 25.1 created on 12/2/91 at 19:26:20	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tcsearch.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/tcsearch.c	1.3"	*/
	
#ident	"@(#)lib/libcurses:tcsearch.c	25.1"

_tcsearch(cap, offsets, names, size, n)
char	*cap;
short	offsets[];
char	*names[];
int	size, n;
{
    register	int	l = 0, u = size - 1;
    int		m, cmp;

    while (l <= u)
    {
	m = (l + u) / 2;
	cmp = ((n == 0) ? strcmp(cap, names[offsets[m]]) :
			  strncmp(cap, names[offsets[m]], n));

	if (cmp < 0)
	    u = m - 1;
	else
	    if (cmp > 0)
		l = m + 1;
	    else
		return (offsets[m]);
    }
    return (-1);
}
