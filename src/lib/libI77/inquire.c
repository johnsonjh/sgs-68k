/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) inquire.c: version 25.1 created on 12/2/91 at 18:42:43	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)inquire.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libI77:inquire.c	1.6.1.1"	*/
	
#ident	"@(#)libI77:inquire.c	25.1"


#include "fio.h"
f_inqu(a) inlist *a;
{	flag byfile;
	int i;
	unit *p;
	char buf[256];
	long x;
	if(a->infile!=NULL)
	{	byfile=1;
		g_char(a->infile,a->infilen,buf);
		x=inode(buf);
		for(i=0,p=NULL;i<MXUNIT;i++)
			if(units[i].uinode==x && units[i].ufd!=NULL)
				p = &units[i];
	}
	else
	{
		byfile=0;
		if(a->inunit<MXUNIT && a->inunit>=0)
		{
			p= &units[a->inunit];
		}
		else
		{
			p=NULL;
		}
	}
	if(a->inex!=NULL)
		if(byfile && x>0 || !byfile && p!=NULL)
			*a->inex=1;
		else *a->inex=0;
	if(a->inopen!=NULL)
		if(byfile) *a->inopen=(p!=NULL);
		else *a->inopen=(p!=NULL && p->ufd!=NULL);
	if(a->innum!=NULL) *a->innum= p-units;
	if(a->innamed!=NULL)
		if(byfile || p!=NULL && p->ufnm!=NULL)
			*a->innamed=1;
		else	*a->innamed=0;
	if(a->inname!=NULL)
		if(byfile)
			b_char(buf,a->inname,a->innamlen);
		else if(p!=NULL && p->ufnm!=NULL)
			b_char(p->ufnm,a->inname,a->innamlen);
	if(a->inacc!=NULL && p!=NULL && p->ufd!=NULL)
		if(p->url)
			b_char("DIRECT",a->inacc,a->inacclen);
		else	b_char("SEQUENTIAL",a->inacc,a->inacclen);
	if(a->inseq!=NULL)
		if(p!=NULL && p->url)		/* cdm 04-09-84 */
			b_char("NO",a->inseq,a->inseqlen);
		else	b_char("YES",a->inseq,a->inseqlen);
	if(a->indir!=NULL)
		if(p!=NULL && p->url)
			b_char("YES",a->indir,a->indirlen);
		else	b_char("NO",a->indir,a->indirlen);
	if(a->infmt!=NULL)
		if(p!=NULL && p->ufmt==0)
			b_char("UNFORMATTED",a->infmt,a->infmtlen);
		else	b_char("FORMATTED",a->infmt,a->infmtlen);
	if(a->inform!=NULL)
		if(p!=NULL && p->ufmt==0)
		b_char("NO",a->inform,a->informlen);
		else b_char("YES",a->inform,a->informlen);
	if(a->inunf)
		if(p!=NULL && p->ufmt==0)
			b_char("YES",a->inunf,a->inunflen);
		else if (p!=NULL) b_char("NO",a->inunf,a->inunflen);
		else b_char("UNKNOWN",a->inunf,a->inunflen);
	if(a->inrecl!=NULL && p!=NULL)
		*a->inrecl=p->url;
	if(a->innrec!=NULL && p!=NULL && p->url>0)
		*a->innrec=ftell(p->ufd)/p->url+1;
	if(a->inblank && p!=NULL && p->ufmt)
		if(p->ublnk)
			b_char("ZERO",a->inblank,a->inblanklen);
		else	b_char("NULL",a->inblank,a->inblanklen);
	return(0);
}
