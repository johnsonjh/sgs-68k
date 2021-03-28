#ident	"@(#)_addhour.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <time.h>
#include "_time.h"

extern void _addhour(struct tm *tmp)
{
	tmp->tm_hour++;
	if (tmp->tm_hour > 23)
	{tmp->tm_hour=0;
	 tmp->tm_mday++;
#ifdef _ANSI 
 	 if (tmp->tm_mday > _month_size[tmp->tm_mon])
#else
 	 if (tmp->tm_mday > month_size[tmp->tm_mon])
#endif /* _ANSI */
     {tmp->tm_mday=1;
 	  tmp->tm_mon++;
	  if (tmp->tm_mon > 11)
	  {tmp->tm_mon=0;
	   tmp->tm_year++;
	  }
	 }
	}
}
