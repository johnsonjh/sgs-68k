#ident	"@(#)_ct_numb.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

extern char *_ct_numb(char *cp,int n)
{
	cp++; 
	if(n >= 10) *cp++ = (n/10)%10 + '0'; else *cp++ =' '; /* Pad with blanks */
	*cp++ = n%10 + '0';
	return(cp);
}
