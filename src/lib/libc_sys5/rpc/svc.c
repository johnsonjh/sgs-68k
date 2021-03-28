#ident	"@(#)svc.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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
#ident	"@(#)svc.c	3.4 LAI System V NFS Release 3.2/V3	source"
/*
 * svc.c, Server-side remote procedure call interface.
 *
 * There are two sets of procedures here.  The xprt routines are
 * for handling transport handles.  The svc routines handle the
 * list of service routines.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */


#include <stdio.h>
#include <sys/fs/nfs/time.h>

#ifdef EXOS
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#else
#include <sys/types.h>
#include <CMC/types.h>
#include <CMC/socket.h>
#include <CMC/in.h>
#include <errno.h>
#include <CMC/errno.h>
#endif
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <rpc/auth.h>
#include <rpc/clnt.h>
#include <rpc/rpc_msg.h>
#include <rpc/svc.h>
#include <rpc/svc_auth.h>
#include <rpc/pmap_clnt.h>
#include <fcntl.h>
#define NOFILE 32

static SVCXPRT *xports[NOFILE];
int svc_fds;
extern errno;

#define NULL_SVC ((struct svc_callout *)0)
#define	RQCRED_SIZE	400

	/* no printing */
/*#define DEBUG 		3	 */
#define	DEBUG0		0
	/* print bad news */
#define	DEBUG1		1
	/* print inportant good news */
#define	DEBUG2		2
	/* commentary */
#define	DEBUG3		3
	/* gossip */
/*#define	DEBUG4		4 */

/*
 * The services list
 * Each entry represents a set of procedures (an rpc program).
 * The dispatch routine takes request structs and runs the
 * apropriate procedure.
 */
static struct svc_callout {
	struct svc_callout *sc_next;
	ulong		    sc_prog;
	ulong		    sc_vers;
	void		    (*sc_dispatch)();
} *svc_head;

static struct svc_callout *svc_find();

/* ***************  SVCXPRT related stuff **************** */

/*
 * Activate a transport handle.
 */
void
xprt_register(xprt)
	SVCXPRT *xprt;
{
	register int sock = xprt->xp_sock;

	if (sock < NOFILE) {
		xports[sock] = xprt;
		svc_fds |= (1 << sock);
	}
}

/*
 * De-activate a transport handle. 
 */
void
xprt_unregister(xprt) 
	SVCXPRT *xprt;
{ 
	register int sock = xprt->xp_sock;

	if ((sock < NOFILE) && (xports[sock] == xprt)) {
		xports[sock] = (SVCXPRT *)0;
		svc_fds &= ~(1 << sock);
	}
} 


/* ********************** CALLOUT list related stuff ************* */

/*
 * Add a service program to the callout list.
 * The dispatch routine will be called when a rpc request for this
 * program number comes in.
 */
bool_t
svc_register(xprt, prog, vers, dispatch, protocol)
	SVCXPRT *xprt;
	ulong prog;
	ulong vers;
	void (*dispatch)();
	int protocol;
{
	struct svc_callout *prev;
	register struct svc_callout *s;

#if	DEBUG > DEBUG4
	fprintf (stderr,
		"svc_register (xprt 0x%x, prog %d, vers %d, dispatch 0x%x, protocol %d\n",
			xprt, prog, vers, dispatch, protocol);
	dump_SVCXPRT (stderr, "svc_register", xprt);
#endif

	if ((s = svc_find(prog, vers, &prev)) != NULL_SVC) {
		if (s->sc_dispatch == dispatch)
			goto pmap_it;  /* he is registering another xptr */
		return (FALSE);
	}
	s = (struct svc_callout *)mem_alloc(sizeof(struct svc_callout));
	if (s == (struct svc_callout *)0) {
		return (FALSE);
	}
	s->sc_prog = prog;
	s->sc_vers = vers;
	s->sc_dispatch = dispatch;
	s->sc_next = svc_head;
	svc_head = s;
pmap_it:
	/* now register the information with the local binder service */
	if (protocol) {
#if	DEBUG > DEBUG2
		fprintf (stderr,
			"svc_register: call pmap_set");
#endif
		return (pmap_set(prog, vers, protocol, xprt->xp_port));
	}
	return (TRUE);
}

/*
 * Remove a service program from the callout list.
 */
void
svc_unregister(prog, vers)
	ulong prog;
	ulong vers;
{
	struct svc_callout *prev;
	register struct svc_callout *s;

	if ((s = svc_find(prog, vers, &prev)) == NULL_SVC)
		return;
	if (prev == NULL_SVC) {
		svc_head = s->sc_next;
	} else {
		prev->sc_next = s->sc_next;
	}
	s->sc_next = NULL_SVC;
	mem_free((char *) s, (uint) sizeof(struct svc_callout));
	/* now unregister the information with the local binder service */
	(void)pmap_unset(prog, vers);
}

/*
 * Search the callout list for a program number, return the callout
 * struct.
 */
static struct svc_callout *
svc_find(prog, vers, prev)
	ulong prog;
	ulong vers;
	struct svc_callout **prev;
{
	register struct svc_callout *s, *p;

	p = NULL_SVC;
	for (s = svc_head; s != NULL_SVC; s = s->sc_next) {
		if ((s->sc_prog == prog) && (s->sc_vers == vers))
			goto done;
		p = s;
	}
done:
	*prev = p;
	return (s);
}

/* ******************* REPLY GENERATION ROUTINES  ************ */

/*
 * Send a reply to an rpc request
 */
bool_t
svc_sendreply(xprt, xdr_results, xdr_location)
	register SVCXPRT *xprt;
	xdrproc_t xdr_results;
	caddr_t xdr_location;
{
	struct rpc_msg rply; 

	rply.rm_direction = REPLY;  
	rply.rm_reply.rp_stat = MSG_ACCEPTED; 
	rply.acpted_rply.ar_verf = xprt->xp_verf; 
	rply.acpted_rply.ar_stat = SUCCESS;
	rply.acpted_rply.ar_results.where = xdr_location;
	rply.acpted_rply.ar_results.proc = xdr_results;
	return (SVC_REPLY(xprt, &rply)); 
}

/*
 * No procedure error reply
 */
void
svcerr_noproc(xprt)
	register SVCXPRT *xprt;
{
	struct rpc_msg rply;

	rply.rm_direction = REPLY;
	rply.rm_reply.rp_stat = MSG_ACCEPTED;
	rply.acpted_rply.ar_verf = xprt->xp_verf;
	rply.acpted_rply.ar_stat = PROC_UNAVAIL;
	SVC_REPLY(xprt, &rply);
}

/*
 * Can't decode args error reply
 */
void
svcerr_decode(xprt)
	register SVCXPRT *xprt;
{
	struct rpc_msg rply; 

	rply.rm_direction = REPLY; 
	rply.rm_reply.rp_stat = MSG_ACCEPTED; 
	rply.acpted_rply.ar_verf = xprt->xp_verf;
	rply.acpted_rply.ar_stat = GARBAGE_ARGS;
	SVC_REPLY(xprt, &rply); 
}

/*
 * Some system error
 */
void
svcerr_systemerr(xprt)
	register SVCXPRT *xprt;
{
	struct rpc_msg rply; 

	rply.rm_direction = REPLY; 
	rply.rm_reply.rp_stat = MSG_ACCEPTED; 
	rply.acpted_rply.ar_verf = xprt->xp_verf;
	rply.acpted_rply.ar_stat = SYSTEM_ERR;
	SVC_REPLY(xprt, &rply); 
}

/*
 * Authentication error reply
 */
void
svcerr_auth(xprt, why)
	SVCXPRT *xprt;
	enum auth_stat why;
{
	struct rpc_msg rply;

	rply.rm_direction = REPLY;
	rply.rm_reply.rp_stat = MSG_DENIED;
	rply.rjcted_rply.rj_stat = AUTH_ERROR;
	rply.rjcted_rply.rj_why = why;
	SVC_REPLY(xprt, &rply);
}

/*
 * Auth too weak error reply
 */
void
svcerr_weakauth(xprt)
	SVCXPRT *xprt;
{

	svcerr_auth(xprt, AUTH_TOOWEAK);
}

/*
 * Program unavailable error reply
 */
void 
svcerr_noprog(xprt)
	register SVCXPRT *xprt;
{
	struct rpc_msg rply;  

	rply.rm_direction = REPLY;   
	rply.rm_reply.rp_stat = MSG_ACCEPTED;  
	rply.acpted_rply.ar_verf = xprt->xp_verf;  
	rply.acpted_rply.ar_stat = PROG_UNAVAIL;
	SVC_REPLY(xprt, &rply);
}

/*
 * Program version mismatch error reply
 */
void  
svcerr_progvers(xprt, low_vers, high_vers)
	register SVCXPRT *xprt; 
	ulong low_vers;
	ulong high_vers;
{
	struct rpc_msg rply;

	rply.rm_direction = REPLY;
	rply.rm_reply.rp_stat = MSG_ACCEPTED;
	rply.acpted_rply.ar_verf = xprt->xp_verf;
	rply.acpted_rply.ar_stat = PROG_MISMATCH;
	rply.acpted_rply.ar_vers.low = low_vers;
	rply.acpted_rply.ar_vers.high = high_vers;
	SVC_REPLY(xprt, &rply);
}

/* ******************* SERVER INPUT STUFF ******************* */

/*
 * Get server side input from some transport.
 *
 * Statement of authentication parameters management:
 * This function owns and manages all authentication parameters, specifically
 * the "raw" parameters (msg.rm_call.cb_cred and msg.rm_call.cb_verf) and
 * the "cooked" credentials (rqst->rq_clntcred).  However, this function
 * does not know the structure of the cooked credentials, so it make the
 * following two assumptions:
 *   a) the structure is contiguous (no pointers), and
 *   b) the structure size does not exceed RQCRED_SIZE bytes. 
 * In all events, all three parameters are freed upon exit from this routine.
 * The storage is trivially management on the call stack in user land, but
 * is mallocated in kernel land.
 */
void
svc_getreq(rdfds)
	int rdfds;
{
	register enum xprt_stat stat;
	struct rpc_msg msg;
	int prog_found;
	ulong low_vers;
	ulong high_vers;
	struct svc_req r;
	register int sock;
	register int readfds = rdfds & svc_fds;
	register SVCXPRT *xprt;
	char cred_area[2*MAX_AUTH_BYTES + RQCRED_SIZE];

#if	DEBUG > DEBUG4
	fprintf (stderr,
		"svc_getreq (rdfds %d)\n",
			rdfds);
#endif

	msg.rm_call.cb_cred.oa_base = cred_area;
	msg.rm_call.cb_verf.oa_base = &(cred_area[MAX_AUTH_BYTES]);
	r.rq_clntcred = &(cred_area[2*MAX_AUTH_BYTES]);

	for (sock = 0; readfds != 0; sock++, readfds >>= 1) {
	    if ((readfds & 1) != 0) {
		/* sock has input waiting */

#if	DEBUG > DEBUG4
	fprintf (stderr,
		"svc_getreq: input on sock %d\n",
			sock);
#endif

		xprt = xports[sock];
		/* now receive msgs from xprtprt (support batch calls) */
		do {

#if	DEBUG > DEBUG4
		fprintf (stderr,
			"svc_getreq: SVC_RECV (xprt 0x%x, &msg 0x%x)\n",
				xprt, msg);
#endif

			if (SVC_RECV(xprt, &msg)) {

				/* now find the exported program and call it */
				register struct svc_callout *s;
				enum auth_stat why;

#if	DEBUG < DEBUG4
				fprintf (stderr,
					"SVC_RECV true\n");
#endif


				r.rq_xprt = xprt;
				r.rq_prog = msg.rm_call.cb_prog;
				r.rq_vers = msg.rm_call.cb_vers;
				r.rq_proc = msg.rm_call.cb_proc;
				r.rq_cred = msg.rm_call.cb_cred;
#if DEBUG < DEBUG4
	fprintf(stderr,	"rprog=%x rvers=%x rproc=%x\n", r.rq_prog,r.rq_vers,
	r.rq_proc);  
#endif
				/* first authenticate the message */

#if	DEBUG < DEBUG4
				fprintf (stderr,
					"svc_getreq: calling _authenticate\n");
#endif

				if ((why= _authenticate(&r, &msg)) != AUTH_OK) {

#if	DEBUG > DEBUG3
				fprintf (stderr,
					"svc_getreq bad authentication\n");
#endif

					svcerr_auth(xprt, why);
					goto call_done;
				}
				/* now match message with a registered service*/

#if	DEBUG < DEBUG4
				fprintf (stderr,
					"svc_getreq: matching service\n");
#endif

				prog_found = FALSE;
				low_vers = 0 - 1;
				high_vers = 0;
				for (s = svc_head; s != NULL_SVC; s = s->sc_next) {
					if (s->sc_prog == r.rq_prog) {
						if (s->sc_vers == r.rq_vers) {
							(*s->sc_dispatch)(&r, xprt);
							goto call_done;
						}  /* found correct version */
						prog_found = TRUE;
						if (s->sc_vers < low_vers)
							low_vers = s->sc_vers;
						if (s->sc_vers > high_vers)
							high_vers = s->sc_vers;
					}   /* found correct program */
				}
				/*
				 * if we got here, the program or version
				 * is not served ...
				 */

#if	DEBUG < DEBUG4
				fprintf (stderr, "svc_getreq: no service\n");
	fprintf(stderr,	"rprog=%x rvers=%x rproc=%x\n", r.rq_prog,r.rq_vers,
	r.rq_proc);  
#endif

				if (prog_found)
					svcerr_progvers(xprt,
					low_vers, high_vers);
				else
					 svcerr_noprog(xprt);
				/* Fall through to ... */
			}
		call_done:

#if	DEBUG > DEBUG4
			fprintf (stderr,
				"svc_getreq: call done\n");
#endif

			if ((stat = SVC_STAT(xprt)) == XPRT_DIED){
				SVC_DESTROY(xprt);
				break;
			}
		} while (stat == XPRT_MOREREQS);
	    }
	}
}


/*
 * This is the rpc server side idle loop
 * Wait for input, call server program.
 */
void
svc_run()
{
	int readfds;
	int sock,flags;
	struct timeval timeout;

#if	DEBUG > DEBUG4
	fprintf (stderr,
		"svc_run ()\n");
#endif
#ifdef EXOS
	timeout.tv_sec=0;
	timeout.tv_usec=100000L;
#else
	timeout.tv_sec=0;
	timeout.tv_usec=0;
#endif

	while (TRUE) {
		readfds = svc_fds;
#ifdef EXOS
		switch (select(32, &readfds, (int *)0, 100000L)) 
		{
		case -1:
			if (errno == EINTR)
				continue;
			else {
				perror("svc.c: - Select failed");
				return;
			}
		case 0:
			continue;
		default:
			svc_getreq(readfds);
		}

#else
/*		switch (select(32, (fd_set *)&readfds, (fd_set *)0, (fd_set *)0,100000L))  
		switch (select(32, &readfds, 0, 0,&timeout)) */ 
		switch (select(32, &readfds, 0, 0, 0)) /* shen 2/6/89 */
		{ 
		case -1:
			if (errno == EINTR)
				continue;
			else {
				perror("svc.c: - Select failed");
				return;
			}
		case 0:
			continue;
		default:
			svc_getreq(readfds);
		}

#endif
	}
}

#ifdef EXOS
/*
 * this is a serious klude needed by the EXOS phony socket accept routine
 * to be able to fake out 4.2 BSD accept functionality.
 */
void
xprt_reregister(oldsock, newsock)
int oldsock;
int newsock;
{
	SVCXPRT *xprt;

	xprt = xports[oldsock];
	xprt_unregister(xports[oldsock]);
	xprt->xp_sock = newsock;
	xprt_register(xprt);
}
#endif

