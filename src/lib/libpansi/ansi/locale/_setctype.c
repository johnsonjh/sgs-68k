#ident	"@(#)_setctype.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <locale.h>
#include <stdio.h>
#include <string.h>
#include "_locale.h"
#include "_ctype.h"

#ifdef __STRICT_STDC__
extern int _setctype(char *loc)
#else
extern int setchrclass(char *loc)
#endif /* __STRICT_STDC__ */
{   /* Pre: loc=LOC
       Returns: -1 if side-effect succeeded, >=0 otherwise
	   Side-effect: _ctype is completely reread from 
				    "/lib/locale/"LOC"/LC_CTYPE"
    */

	unsigned char my_ctype[_SZ_TOTAL];
	FILE *fd;
	register int ret;

	if (((fd = fopen(_fullocale(loc,"LC_CTYPE"),"r")) == NULL) ||
	    (fread((void *) my_ctype,1,_SZ_TOTAL,fd) != _SZ_TOTAL)
	   )
		ret= -1;
	else
	{
		memcpy((void *) _ctype,(void *) my_ctype, _SZ_TOTAL);
		ret= 0;
	}
	fclose(fd);
	return ret;
}
