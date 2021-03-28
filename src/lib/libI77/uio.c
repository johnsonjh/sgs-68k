/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) uio.c: version 25.1 created on 12/2/91 at 18:43:31	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)uio.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libI77:uio.c	1.4"	*/
	
#ident	"@(#)libI77:uio.c	25.1"

/*	3.0 SID #	1.2	*/
#include "fio.h"
int reclen;
do_us(number,ptr,len) ftnint *number; ftnlen len; char *ptr;
{
	if(reading)
	{
		recpos += *number * len;
		if(recpos>reclen)
		{
			err(elist->ciend,(-1), "eof/uio");
		}
		(void) fread(ptr,(int)len,(int)(*number),cf);
		return(0);
	}
	else
	{
		reclen += *number * len;
		(void) fwrite(ptr,(int)len,(int)(*number),cf);
		return(0);
	}
}
do_uio(number,ptr,len) ftnint *number; ftnlen len; char *ptr;
{
	if(sequential)
		return(do_us(number,ptr,len));
	else	return(do_ud(number,ptr,len));
}
do_ud(number,ptr,len) ftnint *number; ftnlen len; char *ptr;
{
	recpos += *number * len;
	if(recpos > curunit->url && curunit->url!=1)
		err(elist->cierr,110,"eof/uio");
	if(reading)
	{
		if(fread(ptr,(int)len,(int)(*number),cf)
			!= *number)
			err(elist->cierr,27,"eof/uio")
		else return(0);
	}
	(void) fwrite(ptr,(int)len,(int)(*number),cf);
	return(0);
}
