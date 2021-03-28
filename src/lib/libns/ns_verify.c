/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ns_verify.c: version 25.1 created on 12/2/91 at 19:42:32	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ns_verify.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libns:ns_verify.c	1.4"	*/
	
#ident	"@(#)libns:ns_verify.c	25.1"

#include <stdio.h>
#include <sys/tiuser.h>
#include <nsaddr.h>
#include <nserve.h>
#include "stdns.h"
#include "nsdb.h"

char *
ns_verify(name, passwd)
char *name, *passwd;
{
	struct nssend send;
	struct nssend *rtn;
	struct nssend *ns_getblock();

	/*
	 *	Initialize the information structure to send to the
	 *	name server.
	 */

	send.ns_code = NS_VERIFY;
	send.ns_type = 0;
	send.ns_flag = 0;
	send.ns_name = name;
	send.ns_desc = passwd;
	send.ns_mach = NULL;
	send.ns_addr = NULL;
	send.ns_path = NULL;

	while ((rtn = ns_getblock(&send)) == (struct nssend *)NULL &&
		ns_errno == R_INREC)
			sleep(1);

	if (rtn == (struct nssend *) NULL)
		return((char *)NULL);

	return(rtn->ns_desc);
}
