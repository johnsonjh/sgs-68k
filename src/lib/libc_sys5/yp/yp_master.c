#ident	"@(#)yp_master.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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
#ident	"@(#)yp_master.c	3.2 LAI System V NFS Release 3.2/V3	source"
#ifndef lint
#ident	"@(#)yp_master.c 1.4 85/11/19 Copyr 1985 Sun Micro"
#endif

#define NULL 0
#include <sys/fs/nfs/time.h>
#include <rpc/rpc.h>
#include <rpcsvc/yp_prot.h>
#include <rpcsvc/ypv1_prot.h>
#include <rpcsvc/ypclnt.h>

int v1domaster();
int v2domaster();

extern struct timeval _ypserv_timeout;
extern int _yp_dobind();
extern unsigned int _ypsleeptime;
extern char *malloc();
/*#define DEBUG 1 */
/*
 * This checks parameters, and implements the outer "until binding success"
 * loop.
 */
int
yp_master (domain, map, master)
	char *domain;
	char *map;
	char **master;
{
	int domlen;
	int maplen;
	int reason;
	struct dom_binding *pdomb;
	int (*dofun)();

	if ( (map == NULL) || (domain == NULL) ) {
		return (YPERR_BADARGS);
	}
	
	domlen = strlen(domain);
	maplen = strlen(map);
	
	if ( (domlen == 0) || (domlen > YPMAXDOMAIN) ||
	    (maplen == 0) || (maplen > YPMAXMAP) ||
	    (master == NULL) ) {
		return (YPERR_BADARGS);
	}

	for (;;) {
		
#ifdef DEBUG
	printf("yp_master: _yp_dobind\n");
#endif
		if (reason = _yp_dobind(domain, &pdomb) ) {
			return (reason);
		}

#ifdef DEBUG
	printf("yp_master: ret from _yp_dobind v2domaster or v1domaster\n");
#endif
		dofun = (pdomb->dom_vers == YPVERS) ? v2domaster : v1domaster;

#ifdef DEBUG
	printf("yp_master: dofun\n");
#endif
		reason = (*dofun)(domain, map, pdomb, _ypserv_timeout,
		    master);

#ifdef DEBUG
	printf("yp_master: ret from dofun\n");
#endif
		if (reason == YPERR_RPC) {
#ifdef DEBUG
	printf("yp_master: yp_unbind(%s)\n",domain);
#endif
			yp_unbind(domain);
			(void) sleep(_ypsleeptime);
		} else {
			break;
		}
	}
	
	return (reason);
}

/*
 * This talks v2 to ypserv
 */
static int
v2domaster (domain, map, pdomb, timeout, master)
	char *domain;
	char *map;
	struct dom_binding *pdomb;
	struct timeval timeout;
	char **master;
{
	struct ypreq_nokey req;
	struct ypresp_master resp;
	unsigned int retval = 0;

	req.domain = domain;
	req.map = map;
	resp.master = NULL;

	/*
	 * Do the get_master request.  If the rpc call failed, return with
	 * status from this point.  
	 */
	
	if(clnt_call(pdomb->dom_client, YPPROC_MASTER, xdr_ypreq_nokey, &req,
	    xdr_ypresp_master, &resp, timeout) != RPC_SUCCESS) {
		return (YPERR_RPC);
	}

	/* See if the request succeeded */
	
	if (resp.status != YP_TRUE) {
		retval = ypprot_err((unsigned) resp.status);
	}

	/* Get some memory which the user can get rid of as he likes */

	if (!retval && ((*master = malloc(
	    (unsigned) strlen(resp.master) + 1)) == NULL)) {
		retval = YPERR_RESRC;

	}

	if (!retval) {
		strcpy(*master, resp.master);
	}
	
	CLNT_FREERES(pdomb->dom_client, xdr_ypresp_master, &resp);
	return (retval);
}

/*
 * This talks v1 to ypserv
 */
static int
v1domaster(domain, map, pdomb, timeout, master)
	char *domain;
	char *map;
	struct dom_binding *pdomb;
	struct timeval timeout;
	char **master;
{
	struct yprequest req;
	struct ypresponse resp;
	enum clnt_stat clnt_stat;
	unsigned int retval = 0;
	
	req.yp_reqtype = YPPOLL_REQTYPE;
	req.yppoll_req_domain = domain;
	req.yppoll_req_map = map;
	resp.yppoll_resp_domain = resp.yppoll_resp_map =
	    resp.yppoll_resp_owner = (char *) NULL;
	    
	/*
	 * Simulate a v2 "get master" request by doing a v1 "poll map"
	 * request, interpreting the response into current ypclnt.h
	 * return values, or returning the map master.
	 */
	if( (clnt_stat = (enum clnt_stat) clnt_call(pdomb->dom_client,
	    YPOLDPROC_POLL, _xdr_yprequest, &req, _xdr_ypresponse,
	    &resp, timeout) ) != RPC_SUCCESS) {
		return (YPERR_RPC);
	}

	if (resp.yp_resptype != YPPOLL_RESPTYPE) {
#ifdef DEBUG
	printf("yp_master: v1domaster:return YPERR_YPERR\n");
#endif
		return (YPERR_YPERR);
	}

	if (!strcmp(resp.yppoll_resp_domain, domain) ) {

		if (!strcmp(resp.yppoll_resp_map, map) ) {
			
			if (! strcmp(resp.yppoll_resp_owner, "") ) {
				retval = YPERR_BADDB;
			}
				
		} else {
			retval = YPERR_MAP;
		}
		
	} else {
		retval = YPERR_DOMAIN;
	}

	if (!retval && ((*master = malloc(
	    (unsigned) strlen(resp.yppoll_resp_owner) + 1)) == NULL)) {
		retval = YPERR_RESRC;
	}

	if (!retval) {
		strcpy(*master, resp.yppoll_resp_owner);
	}
	
	CLNT_FREERES(pdomb->dom_client, _xdr_ypresponse, &resp);
	return (retval);
}
