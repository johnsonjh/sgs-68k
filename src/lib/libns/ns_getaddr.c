/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ns_getaddr.c: version 25.1 created on 12/2/91 at 19:42:19	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ns_getaddr.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libns:ns_getaddr.c	1.4"	*/
	
#ident	"@(#)libns:ns_getaddr.c	25.1"

#include <stdio.h>
#include <sys/tiuser.h>
#include <nsaddr.h>
#include "nserve.h"

struct address *
ns_getaddr(resource, ro_flag, dname)
char	 *resource;
int	  ro_flag;
char	 *dname;
{
	struct nssend send;
	struct nssend *rtn;
	struct nssend *ns_getblock();

	/*
	 *	Initialize the information structure to send to the
	 *	name server.
	 */

	send.ns_code = NS_QUERY;
	send.ns_flag = ro_flag;
	send.ns_name = resource;
	send.ns_type = 0;
	send.ns_desc = NULL;
	send.ns_path = NULL;
	send.ns_addr = NULL;
	send.ns_mach = NULL;

	/*
	 *	start up communication with name server.
	 */

	if ((rtn = ns_getblock(&send)) == (struct nssend *)NULL)
		return((struct address *)NULL);

	if (dname)
		strncpy(dname,rtn->ns_mach[0],MAXDNAME);
	return(rtn->ns_addr);
}
