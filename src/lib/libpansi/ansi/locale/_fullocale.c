#ident	"@(#)_fullocale.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <locale.h>
#include "_locale.h"
#include <string.h>

extern char *_fullocale(char *loc,char *file)
{   /* Pre: loc is name of subdir where locale can be found and
			file is name of file in which locale can be found
       Returns: _P_locale++loc++"/"++file
	   Side-effect: None
	*/
	static char ans[14 + 2 * LC_NAMELEN] = _P_locale;
	register char *p					 = ans + 12; /* [*p='\0'] */

	strcpy(p, loc);
	p += strlen(loc);
	p[0] = '/';
	strcpy(++p, file);
	return ans;
}
