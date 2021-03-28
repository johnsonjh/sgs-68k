/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) _dummy.c: version 25.1 created on 12/2/91 at 20:16:26	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)_dummy.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libnsl:nsl/_dummy.c	1.2"	*/
	
#ident	"@(#)nsl:_dummy.c	25.1"


#include "sys/errno.h"
#include "sys/tiuser.h"
#include "_import.h"

extern int t_errno;
extern int errno;

_dummy()
{
	t_errno = TSYSERR;
	errno = ENXIO;
	return(-1);
}
