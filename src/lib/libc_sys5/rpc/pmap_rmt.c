#ident	"@(#)pmap_rmt.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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
#ident	"@(#)pmap_rmt.c	3.7 LAI System V NFS Release 3.2/V3	source"
#ifndef lint
#ident	"@(#)pmap_rmt.c 1.7 85/10/17 Copyr 1984 Sun Micro"
#endif

/*
 * pmap_rmt.c
 * Client interface to pmap rpc service.
 * remote call and broadcast service
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */

#include <stdio.h>

#ifdef EXOS
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#else
#include <sys/types.h>
#include <CMC/types.h>
#include <CMC/socket.h>
#include <CMC/in.h>
/*#include <fcntl.h> */
#include <errno.h>
#include <CMC/errno.h>
#endif

#include <sys/fs/nfs/time.h>
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <rpc/auth.h>
#include <rpc/clnt.h>
#include <rpc/rpc_msg.h>
#include <rpc/pmap_prot.h> 
#include <rpc/pmap_clnt.h>
#include <sys/fs/nfs/exmisc.h>

/*
 * The following are needed to support getbroadcastnets() using Lachman's
 * Streams TCP/IP product.
 */
#include <sys/fs/nfs/inet.h>
/* #include <net/if.h>
   #include <sys/ioctl.h> */

#define MAX_BROADCAST_SIZE 1400

	/* essential printing */
/*#define DEBUG0	1
#define	DEBUG		2 	  */
	/* print bad news */
#define	DEBUG1		1
	/* print important good news */
#define	DEBUG2		2
	/* commentary */
#define	DEBUG3		3
	/* gossip */
/*#define	DEBUG4		4 */
/*#define DEBUG5		1 */	

extern int errno;
static struct timeval timeout = { 3, 0 };

/*
 * Structures and XDR routines for parameters to and replys from
 * the pmapper remote-call-service.
 */

struct rmtcallargs {
	ulong prog, vers, proc, arglen;
	caddr_t args_ptr;
	xdrproc_t xdr_args;
};
static bool_t xdr_rmtcall_args();

struct rmtcallres {
	ulong *port_ptr;
	ulong resultslen;
	caddr_t results_ptr;
	xdrproc_t xdr_results;
};
static bool_t xdr_rmtcallres();

/*
 * pmapper remote-call-service interface.
 * This routine is used to call the pmapper remote call service
 * which will look up a service program in the port maps, and then
 * remotely call that routine with the given parameters.  This allows
 * programs to do a lookup and call in one step.
*/
enum clnt_stat
pmap_rmtcall(addr, prog, vers, proc, xdrargs, argsp, xdrres, resp, tout, port_ptr)
	struct sockaddr_in *addr;
	ulong prog, vers, proc;
	xdrproc_t xdrargs, xdrres;
	caddr_t argsp, resp;
	struct timeval tout;
	ulong *port_ptr;
{
	int socket = -1;
	register CLIENT *client;
	struct rmtcallargs a;
	struct rmtcallres r;
	enum clnt_stat stat;

	addr->sin_port = htons(PMAPPORT);
	client = clntudp_create(addr, PMAPPROG, PMAPVERS, timeout, &socket);
	if (client != (CLIENT *)NULL) {
		a.prog = prog;
		a.vers = vers;
		a.proc = proc;
		a.args_ptr = argsp;
		a.xdr_args = xdrargs;
		r.port_ptr = port_ptr;
		r.results_ptr = resp;
		r.xdr_results = xdrres;
		stat = CLNT_CALL(client, PMAPPROC_CALLIT, xdr_rmtcall_args, &a,
		    xdr_rmtcallres, &r, tout);
		CLNT_DESTROY(client);
	} else {
		stat = RPC_FAILED;
	}
	(void)close(socket);
	addr->sin_port = 0;
	return (stat);
}

/*
 * XDR remote call arguments
 * written for XDR_ENCODE direction only
 */
static bool_t
xdr_rmtcall_args(xdrs, cap)
	register XDR *xdrs;
	register struct rmtcallargs *cap;
{
	uint lenposition, argposition, position;

	if (xdr_u_long(xdrs, &(cap->prog)) &&
	    xdr_u_long(xdrs, &(cap->vers)) &&
	    xdr_u_long(xdrs, &(cap->proc))) {
		lenposition = XDR_GETPOS(xdrs);
		if (! xdr_u_long(xdrs, &(cap->arglen)))
		    return (FALSE);
		argposition = XDR_GETPOS(xdrs);
		if (! (*(cap->xdr_args))(xdrs, cap->args_ptr))
		    return (FALSE);
		position = XDR_GETPOS(xdrs);
		cap->arglen = (ulong)position - (ulong)argposition;
		XDR_SETPOS(xdrs, lenposition);
		if (! xdr_u_long(xdrs, &(cap->arglen)))
		    return (FALSE);
		XDR_SETPOS(xdrs, position);
		return (TRUE);
	}
	return (FALSE);
}

/*
 * XDR remote call results
 * written for XDR_DECODE direction only
 */
static bool_t
xdr_rmtcallres(xdrs, crp)
	register XDR *xdrs;
	register struct rmtcallres *crp;
{

	if (xdr_reference(xdrs, &crp->port_ptr, sizeof (ulong), xdr_u_long) &&
		xdr_u_long(xdrs, &crp->resultslen))
		return ((*(crp->xdr_results))(xdrs, crp->results_ptr));
	return (FALSE);
}

/*
 * The following is kludged-up support for simple rpc broadcasts.
 * Someday a large, complicated system will replace these trivial 
 * routines which only support udp/ip .
 */

static int
getbroadcastnets(addrs, sock, buf)
	struct in_addr *addrs;
	int sock;  /* any valid socket will do */
	char *buf;  /* why allocxate more when we can use existing... */
{
#ifdef EXOS
	char hname[255];
	char *hnamep = hname;

	(void) gethostname(hname, sizeof(hname));
	addrs[0] = inet_makeaddr(inet_netof(rhost(&hnamep)), INADDR_ANY);
	return(1);
#else
#ifdef S3000
	char hname[255];
	char *hnamep = hname;
	struct	sockaddr_in my_sockaddr;

	(void) gethostname(hname, sizeof(hname));
	get_myaddress(&my_sockaddr);
	addrs[0] = inet_makeaddr(inet_netof(rhost(&hnamep)), INADDR_ANY); 
#ifdef DEBUG5
	printf("getbroadcastnet: addrs[0].s_addr=%x\n",addrs[0].s_addr);
#endif
	addrs[0].s_addr=addrs[0].s_addr|0x00ffffff;	
	addrs[1] = my_sockaddr.sin_addr;  

#ifdef DEBUG0
	printf(" getbroadcastnets: hname=%s addrs=%x\n",hname, addrs->s_addr); 
#endif
	return(2);
#else
	/*
	 * This is the 4.[23] BSD method, which the LAI/CT xprt will
	 * someday be completely compatible with.
	 */
	struct ifconf ifc;
        struct ifreq ifreq, *ifr;
	struct sockaddr_in *sin;
        int n, i;

	ifc.ifc_len = MAX_BROADCAST_SIZE;
        ifc.ifc_buf = buf;
        	if (ioctl(sock, SIOCGIFCONF, (char *)&ifc) < 0) {
                perror("broadcast: ioctl (get interface configuration)");
                return (0);
        }
        ifr = ifc.ifc_req;
        for (i = 0, n = ifc.ifc_len/sizeof (struct ifreq); n > 0; n--, ifr++) {
                ifreq = *ifr;
                	if (ioctl(sock, SIOCGIFFLAGS, (char *)&ifreq) < 0) {
                        perror("broadcast: ioctl (get interface flags)");
                        continue;
                }
                if ((ifreq.ifr_flags & IFF_BROADCAST) &&
		    (ifreq.ifr_flags & IFF_UP) &&
		    ifr->ifr_addr.sa_family == AF_INET) {
                        sin = (struct sockaddr_in *)&ifr->ifr_addr;
			addrs[i++] = inet_makeaddr(inet_netof
			    (sin->sin_addr.s_addr), INADDR_ANY);
                }
        }
	return (i);
#endif
#endif
}

typedef bool_t (*resultproc_t)();

enum clnt_stat 
clnt_broadcast(prog, vers, proc, xargs, argsp, xresults, resultsp, eachresult)
	ulong		prog;		/* program number */
	ulong		vers;		/* version number */
	ulong		proc;		/* procedure number */
	xdrproc_t	xargs;		/* xdr routine for args */
	caddr_t		argsp;		/* pointer to args */
	xdrproc_t	xresults;	/* xdr routine for results */
	caddr_t		resultsp;	/* pointer to results */
	resultproc_t	eachresult;	/* call with each result obtained */
{
	enum clnt_stat stat;
	AUTH *unix_auth = authunix_create_default();
	XDR xdr_stream;
	register XDR *xdrs = &xdr_stream;
	int outlen, inlen, fromlen, nets;
	int	readfds;
	register int sock, mask, i;
	bool_t done = FALSE;
	register ulong xid;
	ulong port;
	struct in_addr addrs[20];
	struct sockaddr_in baddr, raddr; /* broadcast and response addresses */
	struct rmtcallargs a;
	struct rmtcallres r;
	struct rpc_msg msg;
	struct timeval t; 
	char outbuf[MAX_BROADCAST_SIZE], inbuf[MAX_BROADCAST_SIZE];
	int flags,s,l;

	/*
	 * initialization: create a socket, a broadcast address, and
	 * preserialize the arguments into a send buffer.
	 */

	baddr.sin_addr.s_addr = 0;
	baddr.sin_family = AF_INET;
	baddr.sin_port = 0;
#ifdef EXOS
	if ((sock = socket(SOCK_DGRAM, (struct sockproto *)0, &baddr,0)) < 0) {
		perror("Cannot create socket for broadcast rpc");
		stat = RPC_CANTSEND;
		goto done_broad;
	}
#else
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("Cannot create socket for broadcast rpc");
		stat = RPC_CANTSEND;
		goto done_broad;
	}
#ifdef DEBUG0
	printf("clnt_broadcast: sock=%d\n",sock);
#endif
	if (bind(sock, (caddr_t)&baddr, sizeof(struct sockaddr_in))!=0) {
		perror("clnt_broadcast: cannot bind");
		stat = RPC_CANTSEND;
		goto done_broad;
	}
#endif

	mask = (1 << sock);
	nets = getbroadcastnets(addrs, sock, inbuf);
#ifdef DEBUG0
	printf("clnt_broadcast: nets=%d baddr=%x\n",nets,baddr.sin_addr.s_addr);
#endif
	memset((char *)&baddr, '\0', sizeof(baddr));
	baddr.sin_family = AF_INET;
	baddr.sin_port = htons(PMAPPORT);
	baddr.sin_addr.s_addr = (ulong) htonl(INADDR_ANY);
	(void)gettimeofday(&t, (struct timezone *)0);
	msg.rm_xid = xid = getpid() ^ t.tv_sec ^ t.tv_usec;
	t.tv_usec = 0;
	msg.rm_direction = CALL;
	msg.rm_call.cb_rpcvers = RPC_MSG_VERSION;
	msg.rm_call.cb_prog = PMAPPROG;
	msg.rm_call.cb_vers = PMAPVERS;
	msg.rm_call.cb_proc = PMAPPROC_CALLIT;
	msg.rm_call.cb_cred = unix_auth->ah_cred;
	msg.rm_call.cb_verf = unix_auth->ah_verf;
	a.prog = prog;
	a.vers = vers;
	a.proc = proc;
	a.xdr_args = xargs;
	a.args_ptr = argsp;
	r.port_ptr = &port;
	r.xdr_results = xresults;
	r.results_ptr = resultsp;
	xdrmem_create(xdrs, outbuf, MAX_BROADCAST_SIZE, XDR_ENCODE);
	if ((! xdr_callmsg(xdrs, &msg)) || (! xdr_rmtcall_args(xdrs, &a))) {
		stat = RPC_CANTENCODEARGS;
		goto done_broad;
	}
	outlen = (int)xdr_getpos(xdrs);
	xdr_destroy(xdrs);
	/*
	 * Basic loop: broadcast a packet and wait a while for response(s).
	 * The response timeout grows larger per iteration.
	 */
#ifdef EXOS
	for (t.tv_sec = 4; t.tv_sec <= 14; t.tv_sec += 2) {
		for (i = 0; i < nets; i++) {
			baddr.sin_addr = addrs[i];
			if (sendto(sock, outbuf, outlen, 0,
				(struct sockaddr *)&baddr,
				sizeof (struct sockaddr)) != outlen) {
				perror("Cannot send broadcast packet");
				stat = RPC_CANTSEND;
				goto done_broad;
			}
		}
	recv_again:
		msg.acpted_rply.ar_verf = _null_auth;
		msg.acpted_rply.ar_results.where = (caddr_t)&r;
                msg.acpted_rply.ar_results.proc = xdr_rmtcallres;
		readfds = mask;

		switch (select(32, &readfds, (int *)NULL, (int *)NULL, &t)) {
		case 0:  /* timed out */
			stat = RPC_TIMEDOUT;
			continue;

		case -1:  /* some kind of error */
			if (errno == EINTR)
				goto recv_again;
			perror("Broadcast select problem");
			stat = RPC_CANTRECV;
			goto done_broad;

		}  /* end of select results switch */
		if ((readfds & mask) == 0)
			goto recv_again;

	try_again:
		fromlen = sizeof(struct sockaddr);
		inlen = recvfrom(sock, inbuf, MAX_BROADCAST_SIZE, 0,
			(struct sockaddr *)&raddr, &fromlen);
		if (inlen < 0) {
			if (errno == EINTR)
				goto try_again;
			perror("Cannot receive reply to broadcast");
			stat = RPC_CANTRECV;
			goto done_broad;
		}
		if (inlen < sizeof(ulong))
			goto recv_again;
		/*
		 * see if reply transaction id matches sent id.
		 * If so, decode the results.
		 */
		xdrmem_create(xdrs, inbuf, inlen, XDR_DECODE);
		if (xdr_replymsg(xdrs, &msg)) {
			if ((msg.rm_xid == xid) &&
				(msg.rm_reply.rp_stat == MSG_ACCEPTED) &&
				(msg.acpted_rply.ar_stat == SUCCESS)) {
				raddr.sin_port = htons((ushort)port);
				done = (*eachresult)(resultsp, &raddr);
			}
			/* otherwise, we just ignore the errors ... */
		} else {
#ifdef notdef
			/* some kind of deserialization problem ... */
			if (msg.rm_xid == xid)
				fprintf(stderr, "Broadcast deserialization problem");
			/* otherwise, just random garbage */
#endif
		}
		xdrs->x_op = XDR_FREE;
		msg.acpted_rply.ar_results.proc = xdr_void;
		(void)xdr_replymsg(xdrs, &msg);
		(void)(*xresults)(xdrs, resultsp);
		xdr_destroy(xdrs);
		if (done) {
			stat = RPC_SUCCESS;
			goto done_broad;
		} else {
			goto recv_again;
		}
	}
done_broad:
	(void)close(sock);
	AUTH_DESTROY(unix_auth);
	return (stat);

#else	/* S90 */
	for (t.tv_sec = 4; t.tv_sec <= 14; t.tv_sec += 2) {
		for (i = 0; i < nets; i++) {
			baddr.sin_family = AF_INET;
			baddr.sin_port = htons(PMAPPORT);
			baddr.sin_addr=addrs[i]; 
/*		baddr.sin_addr.s_addr=baddr.sin_addr.s_addr|0xffffffff; */
#if	DEBUG > DEBUG1
	fprintf (stderr, "clnt_broadcast: sendto %x outlen=%d\n", baddr.sin_addr.s_addr,outlen);
#endif
			if (sendto(sock, outbuf, outlen, 0,
				&baddr, sizeof (struct sockaddr_in))!=outlen) {
#if DEBUG > DEBUG1
	printf("clnt_broadcast: Cannot send broadcast packet\n");
#endif
				perror("Cannot send broadcast packet");
				stat = RPC_CANTSEND;
				goto done_broad;
			}
		}
	recv_again:
		msg.acpted_rply.ar_verf = _null_auth;
		msg.acpted_rply.ar_results.where = (caddr_t)&r;
                msg.acpted_rply.ar_results.proc = xdr_rmtcallres;
		readfds = mask;

#ifdef DEBUG0
	printf("clnt_broadcast: select readfds=%d\n", readfds); 
#endif
		switch (select(32, &readfds, (int *)NULL, (int *)NULL, &t)) {
		case 0:  /* timed out */
			stat = RPC_TIMEDOUT;
#ifdef DEBUG0
	printf("clnt_broadcast: select case 0\n"); 
#endif
			continue;

		case -1:  /* some kind of error */
			if (errno == EINTR)
				goto recv_again;
			perror("Broadcast select problem");
#ifdef DEBUG0
	printf("clnt_broadcast: select case -1\n"); 
	printf("clnt_broadcast: Broadcast select problem\n"); 
#endif
			stat = RPC_CANTRECV;
			goto done_broad;

		}  /* end of select results switch */
		if ((readfds & mask) == 0)
			goto recv_again;

	try_again:
		fromlen = sizeof(struct sockaddr_in);
#if	DEBUG > DEBUG1
	fprintf (stderr, "clnt_broadcast:recvfrom %x fromlen=%d\n", raddr.sin_addr.s_addr,fromlen);
#endif
		inlen = recvfrom(sock, inbuf, MAX_BROADCAST_SIZE, 0,
			&raddr, &fromlen);
		if (inlen < 0) {
			if (errno == EINTR)
				goto try_again;
			perror("Cannot receive reply to broadcast");
			stat = RPC_CANTRECV;
			goto done_broad;
		}
		if (inlen < sizeof(ulong))
			goto recv_again;
		/*
		 * see if reply transaction id matches sent id.
		 * If so, decode the results.
		 */
		xdrmem_create(xdrs, inbuf, inlen, XDR_DECODE);
		if (xdr_replymsg(xdrs, &msg)) {
			if ((msg.rm_xid == xid) &&
				(msg.rm_reply.rp_stat == MSG_ACCEPTED) &&
				(msg.acpted_rply.ar_stat == SUCCESS)) {
				raddr.sin_port = htons((ushort)port);
				done = (*eachresult)(resultsp, &raddr);
			}
			/* otherwise, we just ignore the errors ... */
		} else {
#ifdef notdef
			/* some kind of deserialization problem ... */
			if (msg.rm_xid == xid)
				fprintf(stderr, "Broadcast deserialization problem");
			/* otherwise, just random garbage */
#endif
		}
		xdrs->x_op = XDR_FREE;
		msg.acpted_rply.ar_results.proc = xdr_void;
		(void)xdr_replymsg(xdrs, &msg);
		(void)(*xresults)(xdrs, resultsp);
		xdr_destroy(xdrs);
		if (done) {
			stat = RPC_SUCCESS;
			goto done_broad;
		} else {
			goto recv_again;
		}
	}
done_broad:
	(void)close(sock);
#ifdef DEBUG
	printf("clnt_broadcast: AUTH_DESTROY\n");
#endif
	AUTH_DESTROY(unix_auth);
	return (stat);

#endif
}
