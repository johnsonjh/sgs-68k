/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) nssendpass.c: version 25.1 created on 12/2/91 at 19:43:09	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)nssendpass.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libns:ns_sendpass.c	1.4.2.1	*/
/*	ATT:#ident	"libns:ns_sendpass.c	1.4"	*/
	
#ident	"@(#)libns:nssendpass.c	25.1"

#include <stdio.h>
#include <sys/tiuser.h>
#include <nsaddr.h>
#include "nserve.h"

ns_sendpass(name, oldpass, newpass)
char	*name, *oldpass, *newpass;
{
	struct nssend send;
	struct nssend *ns_getblock();

	/*
	 *	Initialize the information structure to send to the
	 *	name server.
	 *	oldpass is not encrypted, newpass is.
	 */

	send.ns_code = NS_SENDPASS;
	send.ns_name = name;
	send.ns_type = 0;
	send.ns_flag = 0;
	send.ns_path = NULL;
	send.ns_addr = NULL;
	send.ns_mach = NULL;
	send.ns_desc = malloc(strlen(oldpass)+strlen(newpass)+3);
	strcpy(send.ns_desc, oldpass);
	strcat(send.ns_desc, ":");
	strcat(send.ns_desc, newpass);

	if (ns_getblock(&send) == (struct nssend *)NULL)
		return(FAILURE);

	return(SUCCESS);
}
