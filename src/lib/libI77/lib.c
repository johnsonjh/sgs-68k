/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) lib.c: version 25.1 created on 12/2/91 at 18:42:47	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)lib.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libI77:lib.c	1.4"	*/
	
#ident	"@(#)libI77:lib.c	25.1"

#include "fio.h"
setcilist(x,u,fmt,rec,xerr,end) cilist *x;
char *fmt;
{
	x->ciunit=u;
	x->cifmt=fmt;
	x->cirec=rec;
	x->cierr=xerr;
	x->ciend=end;
}
setolist(x,xunit,fname,sta,fm,rl,blnk,oerr) olist *x;
	char *fname,*sta,*fm,*blnk;
{
	x->oerr=oerr;
	x->ounit=xunit;
	x->ofnm=fname;
	x->ofnmlen=strlen(fname);
	x->osta=sta;
	x->ofm=fm;
	x->orl=rl;
	x->oblnk=blnk;
}
stcllist(x,xunit,stat,cerr) cllist *x; char *stat;
{
	x->cerr=cerr;
	x->cunit=xunit;
	x->csta=stat;
}
setalist(x,xunit,aerr) alist *x;
{
	x->aunit=xunit;
	x->aerr=aerr;
}
