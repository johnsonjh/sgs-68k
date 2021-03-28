/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ns_findp.c: version 25.1 created on 12/2/91 at 19:42:16	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ns_findp.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libns:ns_findp.c	1.2"	*/
	
#ident	"@(#)libns:ns_findp.c	25.1"

#include <stdio.h>
#include <sys/tiuser.h>
#include <nsaddr.h>
#include "nserve.h"

char *
ns_findp(dname)
char	 *dname;
{
	struct nssend send;
	struct nssend *rtn;
	struct nssend *ns_getblock();

	/*
	 *	Initialize the information structure to send to the
	 *	name server.
	 */

	send.ns_code = NS_FINDP;
	send.ns_flag = 0;
	send.ns_name = dname;
	send.ns_type = 0;
	send.ns_desc = NULL;
	send.ns_path = NULL;
	send.ns_addr = NULL;
	send.ns_mach = NULL;

	/*
	 *	start up communication with name server.
	 */

	if ((rtn = ns_getblock(&send)) == (struct nssend *)NULL)
		return((char *)NULL);

	return(rtn->ns_mach[0]);
}
