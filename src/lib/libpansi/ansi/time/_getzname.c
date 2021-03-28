#ident	"@(#)_getzname.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <ctype.h>
#include <_time.h>

extern char *_getzname(char *p,char *tz)
{
  int n = _MAXTZNAME;

  if (!isalpha(*p)) return(0);
  do { *tz++ = *p ; } while (--n > 0 && isalpha(*++p) );
  while(isalpha(*p)) p++;
  while(--n >= 0) *tz++ = ' ';		/* Pad with blanks */
  *tz='\0';
  return(p);	
}
