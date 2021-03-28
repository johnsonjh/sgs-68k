#ident	"@(#)yp_match.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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
#ident	"@(#)yp_match.c	3.2 LAI System V NFS Release 3.2/V3	source"
#ifndef lint
#ident	"@(#)yp_match.c 1.4 85/11/19 Copyr 1985 Sun Micro"
#endif

#define NULL 0
#include <sys/fs/nfs/time.h>
#include <rpc/rpc.h>
#include <rpcsvc/yp_prot.h>
#include <rpcsvc/ypv1_prot.h>
#include <rpcsvc/ypclnt.h>

extern struct timeval _ypserv_timeout;
extern int _yp_dobind();
extern unsigned int _ypsleeptime;
extern char *malloc();
static int v2domatch(), v1domatch();
/*#define DEBUG 1  */
/*
 * Requests the yp server associated with a given domain to attempt to match
 * the passed key datum in the named map, and to return the associated value
 * datum. This part does parameter checking, and implements the "infinite"
 * (until success) sleep loop.
 */
int
yp_match (domain, map, key, keylen, val, vallen)
	char *domain;
	char *map;
	char *key;
	int  keylen;
	char **val;		/* returns value array */
	int  *vallen;		/* returns bytes in val */
{
	int domlen;
	int maplen;
	int reason;
	struct dom_binding *pdomb;
	int (*dofun)();

	if ( (map == NULL) || (domain == NULL) ) {
		return(YPERR_BADARGS);
	}
	
	domlen = strlen(domain);
	maplen = strlen(map);
	
	if ( (domlen == 0) || (domlen > YPMAXDOMAIN) ||
	    (maplen == 0) || (maplen > YPMAXMAP) ||
	    (key == NULL) || (keylen == 0) ) {
		return(YPERR_BADARGS);
	}

	for (;;) {
		
#ifdef DEBUG	
	printf("yp_match: _yp_dobind(%s)\n",domain);
#endif
		if (reason = _yp_dobind(domain, &pdomb) ) {
			return(reason);
		}
#ifdef DEBUG	
	printf("yp_match: ret from _yp_dobind\n");
#endif
		dofun = (pdomb->dom_vers == YPVERS) ? v2domatch : v1domatch;
#ifdef DEBUG	
	printf("yp_match: ret from v2domatch or v1domatch\n");
#endif

		reason = (*dofun)(domain, map, key, keylen, pdomb,
		    _ypserv_timeout, val, vallen);

#ifdef DEBUG	
	printf("yp_match: ret from (*dofun)\n");
#endif
		if (reason == YPERR_RPC) {
			yp_unbind(domain);
#ifdef DEBUG	
	printf("yp_match: ret from yp_unbind\n");
#endif
			(void) sleep(_ypsleeptime);
#ifdef DEBUG	
	printf("yp_match: ret from sleep()\n");
#endif
		} else {
			break;
		}
	}
	
#ifdef DEBUG	
	printf("yp_match: ret reason=%d\n",reason);
#endif
	return(reason);

}

/*
 * This talks v2 protocol to ypserv
 */
static int
v2domatch (domain, map, key, keylen, pdomb, timeout, val, vallen)
	char *domain;
	char *map;
	char *key;
	int  keylen;
	struct dom_binding *pdomb;
	struct timeval timeout;
	char **val;		/* return: value array */
	int  *vallen;		/* return: bytes in val */
{
	struct ypreq_key req;
	struct ypresp_val resp;
	unsigned int retval = 0;

	req.domain = domain;
	req.map = map;
	req.keydat.dptr = key;
	req.keydat.dsize = keylen;
	
	resp.valdat.dptr = NULL;
	resp.valdat.dsize = 0;

	/*
	 * Do the match request.  If the rpc call failed, return with status
	 * from this point.
	 */
	
	if(clnt_call(pdomb->dom_client,
	    YPPROC_MATCH, xdr_ypreq_key, &req, xdr_ypresp_val, &resp,
	    timeout) != RPC_SUCCESS) {
		return(YPERR_RPC);
	}

	/* See if the request succeeded */
	
	if (resp.status != YP_TRUE) {
		retval = ypprot_err((unsigned) resp.status);
	}

	/* Get some memory which the user can get rid of as he likes */

	if (!retval && ((*val = malloc((unsigned)
	    resp.valdat.dsize + 2)) == NULL)) {
		retval = YPERR_RESRC;
	}

	/* Copy the returned value byte string into the new memory */

	if (!retval) {
		*vallen = resp.valdat.dsize;
		memcpy(*val, resp.valdat.dptr, resp.valdat.dsize);
		(*val)[resp.valdat.dsize] = '\n';
		(*val)[resp.valdat.dsize + 1] = '\0';
	}

	CLNT_FREERES(pdomb->dom_client, xdr_ypresp_val, &resp);
	return(retval);

}

/*
 * This talks v1 protocol to ypserv
 */
static int
v1domatch (domain, map, key, keylen, pdomb, timeout, val, vallen)
	char *domain;
	char *map;
	char *key;
	int  keylen;
	struct dom_binding *pdomb;
	struct timeval timeout;
	char **val;		/* return: value array */
	int  *vallen;		/* return: bytes in val */
{
	struct yprequest req;
	struct ypresponse resp;
	unsigned int retval = 0;

	req.yp_reqtype = YPMATCH_REQTYPE;
	req.ypmatch_req_domain = domain;
	req.ypmatch_req_map = map;
	req.ypmatch_req_keyptr = key;
	req.ypmatch_req_keysize = keylen;
	
	resp.ypmatch_resp_valptr = NULL;
	resp.ypmatch_resp_valsize = 0;

	/*
	 * Do the match request.  If the rpc call failed, return with status
	 * from this point.
	 */
	
	if(clnt_call(pdomb->dom_client,
	    YPOLDPROC_MATCH, _xdr_yprequest, &req, _xdr_ypresponse, &resp,
	    timeout) != RPC_SUCCESS) {
		return(YPERR_RPC);
	}

	/* See if the request succeeded */
	
	if (resp.ypmatch_resp_status != YP_TRUE) {
		retval = ypprot_err((unsigned) resp.ypmatch_resp_status);
	}

	/* Get some memory which the user can get rid of as he likes */

	if (!retval && ((*val = malloc((unsigned)
	    resp.ypmatch_resp_valsize + 2)) == NULL)) {
		retval = YPERR_RESRC;
	}

	/* Copy the returned value byte string into the new memory */

	if (!retval) {
		*vallen = resp.ypmatch_resp_valsize;
		memcpy(*val, resp.ypmatch_resp_valptr,
		       resp.ypmatch_resp_valsize);
		(*val)[resp.ypmatch_resp_valsize] = '\n';
		(*val)[resp.ypmatch_resp_valsize + 1] = '\0';
	}

	CLNT_FREERES(pdomb->dom_client, _xdr_ypresponse, &resp);
	return(retval);

}
