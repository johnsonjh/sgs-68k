#ident	"@(#)_nativeloc.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <locale.h>
#include <stdlib.h>
#include <string.h>

extern char *_nativeloc(int cat)
{   /* return value for category with "" locale */
	static char lang[];
	static char *_loc_envs[LC_MAX][4] =
	{	/* env. vars for "" */
		{"LC_CTYPE"		,lang ,"CHRCLASS" ,0},
		{"LC_NUMERIC"	,lang ,0},
		{"LC_TIME"		,lang ,"LANGUAGE" ,0},
		{"LC_COLLATE"	,lang ,0},
		{"LC_MESSAGES"	,lang ,0},
    };
	static char ans[LC_NAMELEN];
	register char *s;
	register char **p;

	for (p = _loc_envs[cat]; *p != 0; p++)
		if (!((s = getenv(*p)) != 0 && (s[0] != '\0')))
		{
		  return "C";
        }
		else
		{
		  strncpy(ans, s, LC_NAMELEN - 1);
		  ans[LC_NAMELEN - 1] ='\0';
		  return ans;
        }
}
