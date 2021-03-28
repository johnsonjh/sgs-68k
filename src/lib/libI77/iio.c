/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iio.c: version 25.1 created on 12/2/91 at 18:42:40	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iio.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libI77:iio.c	1.5"	*/
	
#ident	"@(#)libI77:iio.c	25.1"

#include "fio.h"
#include "fmt.h"
extern char *icptr;
char *icend;
extern icilist *svic;
extern int rd_ed(),rd_ned(),w_ed(),w_ned(),y_ierr();
extern int z_wnew();
int icnum;
extern int hiwater;
z_getc()
{
	if(icptr >= icend) err(svic->iciend,(EOF),"endfile");
	if(recpos++ < svic->icirlen)
		return(*icptr++);
	else	err(svic->icierr,110,"recend");
}
z_putc(c)
{
	if(icptr >= icend) err(svic->icierr,110,"inwrite");
	if(recpos++ < svic->icirlen)
		*icptr++ = c;
	else	err(svic->icierr,110,"recend");
	return(0);
}
z_rnew()
{
	icptr = svic->iciunit + (++icnum)*svic->icirlen;
	recpos = 0;
	cursor = 0;
	hiwater = 0;
	return(1);
}
s_rsfi(a) icilist *a;
{	int n;
	if(n=c_si(a)) return(n);
	reading=1;
	doed=rd_ed;
	doned=rd_ned;
	getn=z_getc;
	dorevert = y_ierr;
	donewrec = doend = z_rnew;
	return(0);
}
s_wsfi(a) icilist *a;
{	int n;
	if(n=c_si(a)) return(n);
	reading=0;
	doed=w_ed;
	doned=w_ned;
	putn=z_putc;
	dorevert = y_ierr;
	donewrec = doend = z_wnew;
	return(0);
}
c_si(a) icilist *a;
{
	fmtbuf=a->icifmt;
	if(pars_f(fmtbuf)<0)
		err(a->icierr,100,"startint");
	fmt_bg();
	sequential=formatted=1;
	external=0;
	cblank=cplus=scale=0;
	svic=a;
	icnum=recpos=0;
	cursor = 0;
	hiwater = 0;
	icptr=svic->iciunit;
	icend=icptr+svic->icirlen*svic->icirnum;
	return(0);
}
z_wnew()
{
	while(recpos++ < svic->icirlen)
		*icptr++ = ' ';
	recpos = 0;
	cursor = 0;
	hiwater = 0;
	icnum++;
	return(1);
}
e_rsfi()
{	int n;
	n = en_fio();
	fmtbuf = NULL;
	return(n);
}
e_wsfi()
{
	int n;
	n = en_fio();
	fmtbuf = NULL;
	if(icnum >= svic->icirnum)
		return(n);
	while(recpos++ < svic->icirlen)
		*icptr++ = ' ';
	return(n);
}
y_ierr()
{
	err(elist->cierr, 110, "iio");
}
