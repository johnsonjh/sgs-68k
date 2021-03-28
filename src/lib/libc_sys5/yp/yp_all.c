#ident	"@(#)yp_all.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
 *	System V NFS - Release 3.2/V3
 *
 *	Copyright 1986, 1987, 1988 Lachman Associates, Incorporated (LAI)
 *
 *	All Rights Reserved.
 *
 *	The copyright above and this notice must be preserved in all
 *	copies of this source code.  The copyright above does not
 *	evidence any actual or intended publication of this source
 *	code.
 *
 *	This is unpublished proprietary trade secret source code of
 *	Lachman Associates.  This source code may not be copied,
 *	disclosed, distributed, demonstrated or licensed except as
 *	expressly authorized by Lachman Associates.
 */
#ident	"@(#)yp_all.c	3.2 LAI System V NFS Release 3.2/V3	source"
#ifndef lint
#ident	"@(#)yp_all.c 1.3 85/11/19 Copyr 1985 Sun Micro"
#endif

#define NULL 0
#include <sys/fs/nfs/time.h>
#include <rpc/rpc.h>
#include <rpcsvc/yp_prot.h>
#include <rpcsvc/ypv1_prot.h>
#include <rpcsvc/ypclnt.h>


static struct timeval tcp_timout = {
	120,				/* 120 seconds */
	0
	};
extern int _yp_dobind();
extern unsigned int _ypsleeptime;
extern char *malloc();

/*
 * This does the "glommed enumeration" stuff.  callback->foreach is the name
 * of a function which gets called per decoded key-value pair:
 * 
 * (*callback->foreach)(status, key, keylen, val, vallen, callback->data);
 *
 * If the server we get back from _yp_dobind speaks the old protocol, this
 * returns YPERR_VERS, and does not attempt to emulate the new functionality
 * by using the old protocol.
 */
int
yp_all (domain, map, callback)
	char *domain;
	char *map;
	struct ypall_callback *callback;
{
#ifndef UDPONLY
	int domlen;
	int maplen;
	struct ypreq_nokey req;
	int reason;
	struct dom_binding *pdomb;
	enum clnt_stat s;

	if ( (map == NULL) || (domain == NULL) ) {
		return(YPERR_BADARGS);
	}
	
	domlen = strlen(domain);
	maplen = strlen(map);
	
	if ( (domlen == 0) || (domlen > YPMAXDOMAIN) ||
	    (maplen == 0) || (maplen > YPMAXMAP) ||
	    (callback == (struct ypall_callback *) NULL) ) {
		return(YPERR_BADARGS);
	}

	if (reason = _yp_dobind(domain, &pdomb) ) {
		return(reason);
	}

	if (pdomb->dom_vers == YPOLDVERS) {
		return (YPERR_VERS);
	}
		
	clnt_destroy(pdomb->dom_client);
	(void) close(pdomb->dom_socket);
	pdomb->dom_socket = RPC_ANYSOCK;
	pdomb->dom_server_port = pdomb->dom_server_addr.sin_port = 0;
	
	if ((pdomb->dom_client = clnttcp_create(&(pdomb->dom_server_addr),
	    YPPROG, YPVERS, &(pdomb->dom_socket), 0, 0)) ==
	    (CLIENT *) NULL) {
		    clnt_pcreateerror("yp_all - TCP channel create failure");
		    return(YPERR_RPC);
	}

	req.domain = domain;
	req.map = map;
	
	s = clnt_call(pdomb->dom_client, YPPROC_ALL, xdr_ypreq_nokey, &req,
	    xdr_ypall, callback, tcp_timout);

	if (s != RPC_SUCCESS) {
		clnt_perror(pdomb->dom_client,
		    "yp_all - RPC clnt_call (TCP) failure");
	}

	yp_unbind(domain);
	
	if (s == RPC_SUCCESS) {
		return(0);
	} else {
		return(YPERR_RPC);
	}
#else
/*
 * We really should provide a yp_first, yp_next feature, like ypcat
 * does when yp_all() fails. But for now, return error.
 */
	return(YPERR_RPC);
#endif
}

