/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zptrace.c: version 25.1 created on 12/2/91 at 19:36:30	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zptrace.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zptrace.c	1.2"	*/
	
#ident	"@(#)libgen:zptrace.c	25.1"


/*	ptrace(2) with error checking
*/

#include	"errmsg.h"

int
zptrace( severity, request, pid, addr, data )
int	 severity;
int	 request;
int	 pid;
int	 addr;
int	 data;
{

	int	err_ind;

	if( (err_ind = ptrace(request, pid, addr, data )) == -1 )
	    _errmsg ( "UXzptrace1", severity,
		  "Cannot process trace: request=%d process id=%d addr=%d data=%d.",
		   request, pid, addr, data);

	return err_ind;
}
