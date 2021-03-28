/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ns_info.c: version 25.1 created on 12/2/91 at 19:42:25	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ns_info.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libns:ns_info.c	1.9"	*/
	
#ident	"@(#)libns:ns_info.c	25.1"

#include  <stdio.h>
#include  <nserve.h>
#include  <sys/tiuser.h>
#include  <nsaddr.h>
#include  "stdns.h"
#include  "nsdb.h"

extern int ns_errno;

ns_info(name)
char	*name;
{

	static	char	*flg[] = {
		"read/write",
		"read-only"
	};
	char	dname[SZ_DELEMENT];
	struct	nssend 	send, *rtn;


	if (name[strlen(name)-1] == SEPARATOR) {
		sprintf(dname,"%s%c",name,WILDCARD);
		name = dname;
		send.ns_code = NS_QUERY;
	}
	else if (*name == WILDCARD)
		send.ns_code = NS_QUERY;
	else
		send.ns_code = NS_BYMACHINE;

	send.ns_type = 0;
	send.ns_flag = 0;
	send.ns_name = name;
	send.ns_path = NULL;
	send.ns_desc = NULL;
	send.ns_mach = NULL;

	/*
	 *	Setup communication path to the name server.
	 */
	
	if (ns_setup() == FAILURE)
		return(FAILURE);
	
	if (ns_send(&send) == FAILURE) {
		ns_close();
		return(FAILURE);
	}

	if ((rtn = ns_rcv()) == NULL) {
		ns_close();
		return(FAILURE);
	}
	
	do {
		if (rtn->ns_code == FAILURE) {
			if (ns_errno == R_NONAME)
				break;
			ns_close();
			return(FAILURE);
		}

		fprintf(stdout,"%-14.14s  %-10s  %-24.24s  %s\n",
				rtn->ns_name,flg[rtn->ns_flag],
				*rtn->ns_mach,
				(rtn->ns_desc) ? rtn->ns_desc : " "); 

		if (!(rtn->ns_code & MORE_DATA))
			break;

	} while ((rtn = ns_rcv()) != NULL);

	ns_close();
	return(SUCCESS);
}
