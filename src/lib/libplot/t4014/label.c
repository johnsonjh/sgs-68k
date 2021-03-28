/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) label.c: version 25.1 created on 12/2/91 at 20:11:33	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)label.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT/t4014:#ident	"libplot:t4014/label.c	1.2"	*/
	
#ident	"@(#)libplot/t4014:label.c	25.1"

#define N 0104
#define E 0101
#define NE 0105
#define S 0110
#define W 0102
#define SW 0112
/*	arrange by incremental plotting that an initial
 *	character such as +, X, *, etc will fall
 *	right on the point, and undo it so that further
 *	labels will fall properly in place
 */
char lbl_mv[] = {
	036,040,S,S,S,S,S,S,SW,SW,SW,SW,SW,SW,SW,SW,SW,SW,037,0
};
char lbl_umv[] = {
	036,040,N,N,N,N,N,N,NE,NE,NE,NE,NE,NE,NE,NE,NE,NE,037,0
};
label(s)
char *s;
{
	register i,c;
	for(i=0; c=lbl_mv[i]; i++)
		putch(c);
	for(i=0; c=s[i]; i++)
		putch(c);
	for(i=0; c=lbl_umv[i]; i++)
		putch(c);
}
