/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) stest.c: version 25.1 created on 12/2/91 at 18:43:25	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)stest.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libI77:stest.c	1.4"	*/
	
#ident	"@(#)libI77:stest.c	25.1"

#include "fio.h"
#define out(a,b) {n=b;if(n==0) fprintf(stderr,"%s:%d\n",a,errno);else \
		fprintf(stderr,"%s:%d %d\n",a,errno,n);}
extern errno;
olist xopen;
cllist xclose;
alist xaux;
cilist xio;
long one = 1;
char xx[] = "1234";
char yy[] = "terrible junk";
main()
{
	int n;
	setolist(&xopen,9,"junk","o","u",0,NULL,0);
	out("open",f_open(&xopen));
	setalist(&xaux,9,0);
	out("rew",f_rew(&xaux));
	setcilist(&xio,9,NULL,0,0,0);
	out("start",s_wsue(&xio));
	out("io",do_uio(&one,xx,(long)sizeof(xx)-1));
	out("endw",e_wsue());
	out("enf",f_end(&xaux));
	out("bck",f_back(&xaux));
	out("bck",f_back(&xaux));
	out("startr",s_rsue(&xio));
	out("rio",do_uio(&one,yy,(long)sizeof(xx)-1));
	out("endr",e_rsue());
	fprintf(stdout,"yy: %s\n",yy);
}
