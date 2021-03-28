#ident	"@(#)setlocale.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <locale.h>
#include <string.h>
#include "_locale.h"

extern char *setlocale(int cat,const char *loc)
{
	char part[LC_NAMELEN];

	if (loc == NULL)
	{   /* query */
		static char ans[LC_MAX * LC_NAMELEN + 1];

		if (cat != LC_ALL)
			strcpy(ans, _cur_locale[cat]);
		else
		{   /* Generate composite locale description. */
			register char *p;
			register char *q;
			register int i;
			register int flag = 0;

			p = ans;
			for (i = LC_CTYPE; i < LC_MAX; i++)
			{
				*p++ = '/';
				q = _cur_locale[i];
				strcpy(p,q);
				p += strlen(q);
				if (!flag && i > LC_CTYPE)
					flag = strcmp(q ,_cur_locale[i-1]);
			}
			if (!flag)
				strcpy(ans,q);
		}
		return ans;
	}
	/* This is the else, now actual setting of parameters */
	if (cat == LC_ALL)
	{   /* Handle LC_ALL setting separately */
		register char *p;
		register int i;

		if (*(p = loc) != '/')	/* simple locale */
		{
			loc = strncpy(part, p, LC_NAMELEN - 1);
			part[LC_NAMELEN - 1] = '\0';
		}
		cat = LC_CTYPE;
		do	/* for each category other than LC_ALL */
		{
			if (p[0] == '/')	/* piece of composite locale */
			{
				i = (int) strcspn(++p, "/");
				strncpy(part, p, i);
				part[i] = '\0';
				p += i;
			}
			if (setlocale(cat++, part) == NULL) return (char *) NULL;
		} while (cat <= LC_TIME);
		return (setlocale(LC_ALL, (char *) NULL));
	}
	/* This is an else again, cat now differs from LC_ALL
	* Set single category's locale.  By default,
	* just note the new name and handle it later.
	* For LC_CTYPE and LC_NUMERIC, fill in their
	* tables now.
	*/
	if (loc[0] == '\0')
		loc = _nativeloc(cat);
	else
	{
		loc = strncpy(part, loc, LC_NAMELEN - 1);
		part[LC_NAMELEN - 1] = '\0';
	};
	if (strcmp(loc, _cur_locale[cat])) { 
		switch (cat) {
		case LC_COLLATE: if (_setcollate(loc) == -1) return (char *) NULL;
		case LC_NUMERIC: if(_setnumeric(loc) == -1) return (char *) NULL;
#ifdef __STRICT_STDC__
		case LC_CTYPE: if(_setctype(loc) == -1) return (char *) NULL;
#else
		case LC_CTYPE: if(setchrclass(loc) == -1) return (char *) NULL;
#endif /* __STRICT_STDC__ */
		case LC_MONETARY: if(_setmonetary(loc) == -1) return (char *) NULL;
		case LC_TIME: if(_settime(loc) == -1) return (char *) NULL;
    	}
	};
	return(strcpy(_cur_locale[cat], loc));
}
