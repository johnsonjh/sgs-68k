#ident	"@(#)isdigit.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

extern int isdigit(int c)
{

  return(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || 
		 c == '5' || c == '6' || c == '7' || c == '8' || c == '9');
}
