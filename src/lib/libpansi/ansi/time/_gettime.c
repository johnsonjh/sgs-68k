#ident	"@(#)_gettime.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <time.h>
#include "_time.h"

extern char *_gettime(char *p,time_t *timez,int f)
{
	register time_t t = 0;
	int d, sign = 0;

	d = 0;
	if (f)
		if ( (sign = (*p == '-')) || (*p == '+'))
			p++;
	if ( (p = _getdigit(p,&d)) != 0) 
	{
		t = d * 60 * 60;
		if (*p == ':')
		{
			if( (p = _getdigit(p+1,&d)) != 0)
			{
				t += d * 60;
				if (*p == ':')
				{
					if( (p = _getdigit(p+1,&d)) != 0)
						t += d;
				}
			}
		}
	}
	if(sign) *timez = -t; else *timez = t;
	return(p);
}
