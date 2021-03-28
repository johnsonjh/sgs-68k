/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) nsgetblock.c: version 25.1 created on 12/2/91 at 19:42:42	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)nsgetblock.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libns:ns_getblock.c	1.3"	*/
	
#ident	"@(#)libns:nsgetblock.c	25.1"

#include <stdio.h>
#include <sys/tiuser.h>
#include <nsaddr.h>
#include "nserve.h"

struct nssend *
ns_getblock(send)
struct nssend *send;
{
	struct nssend *rcv;

	/*
	 *	Setup the communication path to the name server.
	 */
	
	if (ns_setup() == FAILURE)
		return((struct nssend *)NULL);
	
	if (ns_send(send) == FAILURE) {
		ns_close();
		return((struct nssend *)NULL);
	}

	/*
	 *	Get a return structure and check the return code
	 *	from the name server.
	 */
	
	if ((rcv = ns_rcv()) == (struct nssend *)NULL) {
		ns_close();
		return((struct nssend *)NULL);
	}

	if (rcv->ns_code != SUCCESS) {
		ns_close();
		return((struct nssend *)NULL);
	}

	ns_close();

	return(rcv);
}
