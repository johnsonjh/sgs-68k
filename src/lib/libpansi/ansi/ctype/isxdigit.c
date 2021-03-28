#ident	"@(#)isxdigit.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <ctype.h>

extern int isxdigit(int c)
{

  return(isdigit(c) ||
 		 c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' ||
		 c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F');
}
