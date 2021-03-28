/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ns_getpass.c: version 25.1 created on 12/2/91 at 19:42:22	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ns_getpass.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libns:ns_getpass.c	1.2"	*/
	
#ident	"@(#)libns:ns_getpass.c	25.1"

#include <stdio.h>
#include "nserve.h"

char *
ns_getpass(name)
char *name;
{
	struct nssend send;
	struct nssend *rtn;
	struct nssend *ns_getblock();

	/*
	 *	Initialize the information structure to send to the
	 *	name server.
	 */

	send.ns_code = NS_GETPASS;
	send.ns_name = name;
	send.ns_type = 0;
	send.ns_flag = 0;
	send.ns_desc = NULL;
	send.ns_path = NULL;
	send.ns_addr = NULL;
	send.ns_mach = NULL;

	if ((rtn = ns_getblock(&send)) == (struct nssend *)NULL)
		return((char *)NULL);

	if (rtn->ns_desc == NULL)
		rtn->ns_desc = "";

	return(rtn->ns_desc);
}
