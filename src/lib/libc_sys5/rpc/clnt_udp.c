#ident	"@(#)clnt_udp.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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
#ident	"@(#)clnt_udp.c	3.5 LAI System V NFS Release 3.2/V3	source"
/*
 * clnt_udp.c, Implements a UDP/IP based, client side RPC.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */

#include <stdio.h>

#ifdef EXOS
#include <sys/soioctl.h>
#include <exos/misc.h>
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

#include <rpc/types.h>
#include <sys/fs/nfs/time.h> 
/*#include <sys/ioctl.h>*/
#include <rpc/netdb.h>
#include <rpc/xdr.h>
#include <rpc/auth.h>
#include <rpc/clnt.h>
#include <rpc/rpc_msg.h>
#include <rpc/pmap_clnt.h> 

#ifdef EXOS
#include <sys/fs/nfs/exmisc.h>
#endif

	/* essential printing */
/*#define DEBUG		5     */
/*#define	DEBUG0		0	*/
	/* print bad news */
#define	DEBUG1		1
	/* print important good news */
#define	DEBUG2		2
	/* commentary */
/*#define	DEBUG3		3  */
	/* gossip */
/*#define	DEBUG4		4  */
/*#define DEBUG5		5  */ 



extern int errno;
long lrand48();

/*
 * UDP bases client side rpc operations
 */
static enum clnt_stat	clntudp_call();
static void		clntudp_abort();
static void		clntudp_geterr();
static bool_t		clntudp_freeres();
static void		clntudp_destroy();

static struct clnt_ops udp_ops = {
	clntudp_call,
	clntudp_abort,
	clntudp_geterr,
	clntudp_freeres,
	clntudp_destroy
};

/* 
 * Private data kept per client handle
 */
struct cu_data {
	int		   cu_sock;
	struct sockaddr_in cu_raddr;
	int		   cu_rlen;
	struct timeval	   cu_wait;
	struct rpc_err	   cu_error;
	XDR		   cu_outxdrs;
	uint		   cu_xdrpos;
	uint		   cu_sendsz;
	char		   *cu_outbuf;
	uint		   cu_recvsz;
	char		   cu_inbuf[1];
};

#ifdef S3000
#define getsockname gtsoname
#endif

delay()
{
	int jj;
	for (jj=0; jj<10000; jj++); 
}

/*
 * Create a UDP based client handle.
 * If *sockp<0, *sockp is set to a newly created UDP socket.
 * If raddr->sin_port is 0 a binder on the remote machine
 * is consulted for the correct port number.
 * NB: It is the clients responsibility to close *sockp.
 * NB: The rpch->cl_auth is initialized to null authentication.
 *     Caller may wish to set this something more useful.
 *
 * wait is the amount of time used between retransmitting a call if
 * no response has been heard;  retransmition occurs until the actual
 * rpc call times out.
 *
 * sendsz and recvsz are the maximum allowable packet sizes that can be
 * sent and received.
 */
CLIENT *
clntudp_bufcreate(raddr, program, version, wait, sockp, sendsz, recvsz)
	struct sockaddr_in *raddr;
	ulong program;
	ulong version;
	struct timeval wait;
	register int *sockp;
	uint sendsz;
	uint recvsz;
{
	CLIENT *cl;
	register struct cu_data *cu;
	struct timeval now;
	struct rpc_msg call_msg;
	struct sockaddr_in	tmpaddr;
	int len=sizeof(struct sockaddr_in);

#ifdef	DEBUG4
	(void) fprintf (stderr, "clntudp_bufcreate: len=%d\n",len);
#endif
#if	DEBUG > DEBUG4
	(void) fprintf (stderr,
		"clntudp_bufcreate(raddr 0x%x, program %d, version %d, wait %d, sockp 0x%x, sendsz %d, recvsz %d)\n",
	raddr->sin_addr.s_addr, program, version, wait.tv_sec, *sockp, sendsz, recvsz);
	(void) fprintf (stderr, "raddr --> sin_port %d\n", raddr -> sin_port);
#endif

	cl = (CLIENT *)mem_alloc(sizeof(CLIENT));
	if (cl == NULL) {
		(void) fprintf(stderr, "clntudp_create: out of memory\n");
		rpc_createerr.cf_stat = RPC_SYSTEMERROR;
		rpc_createerr.cf_error.re_errno = errno;
		goto fooy;
	}
	sendsz = ((sendsz + 3) / 4) * 4;
	recvsz = ((recvsz + 3) / 4) * 4;
	cu = (struct cu_data *)mem_alloc(sizeof(*cu) + sendsz + recvsz);
	if (cu == NULL) {
		(void) fprintf(stderr, "clntudp_create: out of memory\n");
		rpc_createerr.cf_stat = RPC_SYSTEMERROR;
		rpc_createerr.cf_error.re_errno = errno;
		goto fooy;
	}
	cu->cu_outbuf = &cu->cu_inbuf[recvsz];

	(void)gettimeofday(&now, (struct timezone *)0);
	if (raddr->sin_port == 0) {
		ushort port;
	if ((port = pmap_getport(raddr, program, version, IPPROTO_UDP)) == 0) {
#ifdef DEBUG > DEBUG4
	fprintf(stderr,"clntudp_create: pmap_getport returns 0\n");
#endif
#ifdef NOTDEF  /*S3000 */
	if ((port = pmap_getport(raddr, program, version, IPPROTO_UDP)) == 0) {
			goto fooy;
		}
#else
			goto fooy;
#endif
		}
		raddr->sin_port = htons(port);
	}
	cl->cl_ops = &udp_ops;
	cl->cl_private = (caddr_t)cu;
	cu->cu_raddr = *raddr;
	cu->cu_rlen = sizeof (cu->cu_raddr);
	cu->cu_wait = wait;
	cu->cu_sendsz = sendsz;
	cu->cu_recvsz = recvsz;
	call_msg.rm_xid = getpid() ^ (int)lrand48() ^ now.tv_sec ^ now.tv_usec;
	call_msg.rm_direction = CALL;
	call_msg.rm_call.cb_rpcvers = RPC_MSG_VERSION;
	call_msg.rm_call.cb_prog = program;
	call_msg.rm_call.cb_vers = version;
	xdrmem_create(&(cu->cu_outxdrs), cu->cu_outbuf,
	    sendsz, XDR_ENCODE);
	if (! xdr_callhdr(&(cu->cu_outxdrs), &call_msg)) {
		goto fooy;
	}
	cu->cu_xdrpos = XDR_GETPOS(&(cu->cu_outxdrs));

	tmpaddr.sin_family= AF_INET;
	tmpaddr.sin_port= 0; 
	tmpaddr.sin_addr.s_addr = 0; 
#ifdef EXOS 
	if (*sockp < 0) {
		int dontblock = 1;
		*sockp = socket(SOCK_DGRAM,(struct sockproto *)0, &tmpaddr, 0); 
		if (*sockp < 0) {
			rpc_createerr.cf_stat = RPC_SYSTEMERROR;
			rpc_createerr.cf_error.re_errno = errno;
			goto fooy;
		}
		(void)ioctl(*sockp, FIONBIO, &dontblock);	
	}
#else	
	if (*sockp < 0) {
		int dontblock = 1;

#if	DEBUG > DEBUG4
	(void) fprintf (stderr, "clntudp_bufcreate: socket\n");
#endif

		*sockp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (*sockp < 0) {
#if	DEBUG5 
	printf ("clntudp_bufcreate: socket ret %d\n",sockp);
#endif
			rpc_createerr.cf_stat = RPC_SYSTEMERROR;
			rpc_createerr.cf_error.re_errno = errno;
			goto fooy;
		}

#if	DEBUG5 
	printf ("clntudp_bufcreate: sock=%d\n",sockp);
#endif
	if (bind(*sockp, (caddr_t)&tmpaddr, sizeof(struct sockaddr_in))!=0){
#if	DEBUG > DEBUG4
	(void) fprintf (stderr, "clntudp_bufcreate: cannot bind\n");
#endif
			rpc_createerr.cf_stat = RPC_SYSTEMERROR;
			rpc_createerr.cf_error.re_errno = errno;
			goto fooy;
	}	
		/* the sockets rpc controls are non-blocking */
		/*	(void)ioctl(*sockp, FIONBIO, &dontblock);*/
#if	DEBUG > DEBUG4
	(void) fprintf (stderr, "clntudp_bufcreate: ret from bind\n");
#endif
	}
#endif	

	cu->cu_sock = *sockp;
	cl->cl_auth = authnone_create();
#if	DEBUG > DEBUG4
	(void) fprintf (stderr, "clntudp_bufcreate: ret cl=%x\n",cl);
#endif
	return (cl);
fooy:
	mem_free((caddr_t)cu, sizeof(*cu));
	mem_free((caddr_t)cl, sizeof(CLIENT));
#if	DEBUG > DEBUG4
	(void) fprintf (stderr, "clntudp_bufcreate: ret CLIENT NULL\n");
#endif
	return ((CLIENT *)NULL);
}

CLIENT *
clntudp_create(raddr, program, version, wait, sockp)
	struct sockaddr_in *raddr;
	ulong program;
	ulong version;
	struct timeval wait;
	register int *sockp;
{

	return(clntudp_bufcreate(raddr, program, version, wait, sockp,
	    UDPMSGSIZE, UDPMSGSIZE));
}

static enum clnt_stat 
clntudp_call(cl, proc, xargs, argsp, xresults, resultsp, timeout)
	register CLIENT	*cl;		/* client handle */
	ulong		proc;		/* procedure number */
	xdrproc_t	xargs;		/* xdr routine for args */
	caddr_t		argsp;		/* pointer to args */
	xdrproc_t	xresults;	/* xdr routine for results */
	caddr_t		resultsp;	/* pointer to results */
	struct timeval	timeout;	/* seconds to wait before giving up */
{
	register struct cu_data *cu = (struct cu_data *)cl->cl_private;
	register XDR *xdrs;
	register int outlen;
	register int inlen;
	int readfds; 
	int fromlen;
	register int mask;
	struct sockaddr_in from;
	struct rpc_msg reply_msg;
	XDR reply_xdrs;
	struct timeval time_waited;
	bool_t ok;
	int i,sock,flags,found;
	int len,jj;	long *temp;
	struct sockaddr_in me;

	found=0;
	time_waited.tv_sec = 0;
	time_waited.tv_usec = 0;
call_again:
	xdrs = &(cu->cu_outxdrs);
	xdrs->x_op = XDR_ENCODE;
	XDR_SETPOS(xdrs, cu->cu_xdrpos);
	/*
	 * the transaction is the first thing in the out buffer
	 */
	(*(ushort *)(cu->cu_outbuf))++;
	if ((! XDR_PUTLONG(xdrs, (long *)&proc)) ||
	    (! AUTH_MARSHALL(cl->cl_auth, xdrs)) ||
	    (! (*xargs)(xdrs, argsp)))
		return (cu->cu_error.re_status = RPC_CANTENCODEARGS);
	outlen = (int)XDR_GETPOS(xdrs);

send_again:
#ifdef EXOS
	if (sendto(cu->cu_sock, cu->cu_outbuf, outlen, 0,
	    (struct sockaddr *)&(cu->cu_raddr), cu->cu_rlen)
	    != outlen) {
#else
#ifdef DEBUG3
	len=sizeof(struct sockaddr_in);
	getsockname(cu->cu_sock, &me, &len);
  printf("clntudp_call: I AM sock=%d addr=%x port=%x\n", cu->cu_sock,
		me.sin_addr.s_addr, me.sin_port); 

printf("clntudp_call: sendto(sock=%d outlen=%d outbuf=%x addr=%x port=%d rlen=%d)\n",
	cu->cu_sock, outlen,cu->cu_outbuf, cu->cu_raddr.sin_addr.s_addr,cu->cu_raddr.sin_port, cu->cu_rlen );
	printf("clntudp_call: ");
	temp=(long *) cu->cu_outbuf;
	for (jj=0; jj< (outlen+3)/4; jj++) fprintf(stderr,"%x ", temp[jj]); 
#endif

	if (sendto(cu->cu_sock, cu->cu_outbuf, outlen, 0,
	    &(cu->cu_raddr), cu->cu_rlen)
	    != outlen) {
#endif

#if	DEBUG3
	printf ("clntudp_call: sendto != outlen\n");
#endif
		cu->cu_error.re_errno = errno;
		return (cu->cu_error.re_status = RPC_CANTSEND);
	}
	/*
	 * Hack to provide rpc-based message passing
	 */
	if (timeout.tv_sec == 0 && timeout.tv_usec == 0) {
#ifdef DEBUG3
	printf("clntudp_call: tv_sec && tv_usec==0, ret RPC_TIMEDOUT\n");
#endif
		return (cu->cu_error.re_status = RPC_TIMEDOUT);
	}
	/*
	 * sub-optimal code appears here because we have
	 * some clock time to spare while the packets are in flight.
	 * (We assume that this is actually only executed once.)
	 */
	reply_msg.acpted_rply.ar_verf = _null_auth;
	reply_msg.acpted_rply.ar_results.where = resultsp;
	reply_msg.acpted_rply.ar_results.proc = xresults;
	mask = 1 << cu->cu_sock;

#ifdef EXOS
	for (;;) {
		readfds = mask;
		switch (select(32, &readfds, (int *)NULL, (int *)NULL,
		    &(cu->cu_wait))) {

		case 0:
			time_waited.tv_sec += cu->cu_wait.tv_sec;
			time_waited.tv_usec += cu->cu_wait.tv_usec;
			while (time_waited.tv_usec >= 1000000) {
				time_waited.tv_sec++;
				time_waited.tv_usec -= 1000000;
			}
			if ((time_waited.tv_sec < timeout.tv_sec) ||
				((time_waited.tv_sec == timeout.tv_sec) &&
				(time_waited.tv_usec < timeout.tv_usec)))
				goto send_again;	
#ifdef DEBUG5
	printf("clntudp_call: select case 0: ret RPC_TIMEDOUT\n");
#endif
			return (cu->cu_error.re_status = RPC_TIMEDOUT);

		/*
		 * buggy in other cases because time_waited is not being
		 * updated.
		 */
		case -1:
			if (errno == EINTR)
				continue;	
			cu->cu_error.re_errno = errno;
			return (cu->cu_error.re_status = RPC_CANTRECV); 
		}
		if ((readfds & mask) == 0)
			continue;	
		do {
			fromlen = sizeof(struct sockaddr_in ); /*shen 12/7/88*/
			inlen = recvfrom(cu->cu_sock, cu->cu_inbuf, 
				(int) cu->cu_recvsz, 0,
				(struct sockaddr *)&from, &fromlen);
		} while (inlen < 0 && errno == EINTR);
		if (inlen < 0) {
			if (errno == EWOULDBLOCK)
				continue;	
			cu->cu_error.re_errno = errno;
			return (cu->cu_error.re_status = RPC_CANTRECV);
		}
		if (inlen < sizeof(ulong))
			continue;	
		/* see if reply transaction id matches sent id */
		if (*((ulong *)(cu->cu_inbuf)) != *((ulong *)(cu->cu_outbuf)))
			continue;	
		/* we now assume we have the proper reply */
		break;
	}

#else

	for (;;) {
	  readfds = mask;
	  switch (select(32, &readfds, NULL, NULL,
		    &(cu->cu_wait))) {

		case 0:
			time_waited.tv_sec += cu->cu_wait.tv_sec;
			time_waited.tv_usec += cu->cu_wait.tv_usec;
			while (time_waited.tv_usec >= 1000000) {
				time_waited.tv_sec++;
				time_waited.tv_usec -= 1000000;
			}
			if ((time_waited.tv_sec < timeout.tv_sec) ||
				((time_waited.tv_sec == timeout.tv_sec) &&
				(time_waited.tv_usec < timeout.tv_usec)))
				goto send_again;	
			return (cu->cu_error.re_status = RPC_TIMEDOUT);

		/*
		 * buggy in other cases because time_waited is not being
		 * updated.
		 */
		case -1:
#ifdef DEBUG5
	printf("clntudp_call: select case -1: \n");
#endif
			if (errno == EINTR)
				continue;	
			cu->cu_error.re_errno = errno;
			return (cu->cu_error.re_status = RPC_CANTRECV);
		}

		if ((readfds & mask) == 0)
			continue;	
		do {
			fromlen = sizeof(struct sockaddr_in ); /*shen 12/7/88*/
		  	inlen = recvfrom(cu->cu_sock, cu->cu_inbuf, 
				(int) cu->cu_recvsz, 0, &from, &fromlen);
#ifdef DEBUG3
(void)fprintf(stderr,"clntudp_call: recvfrom(sock=%d len=%d fromaddr=%x fromlen=)\n", cu->cu_sock, cu->cu_recvsz, from.sin_addr.s_addr, fromlen);
/*delay(); */
#endif
		} while (inlen < 0 && errno == EINTR);
		if (inlen < 0) {
			if (errno == EWOULDBLOCK)
				continue;	
			cu->cu_error.re_errno = errno;
			return (cu->cu_error.re_status = RPC_CANTRECV);
		}
		if (inlen < sizeof(ulong))
			continue;	
		/* see if reply transaction id matches sent id */
		if (*((ulong *)(cu->cu_inbuf)) != *((ulong *)(cu->cu_outbuf)))
			continue;	
		/* we now assume we have the proper reply */
		break;
	}


#endif
	/*
	 * now decode and validate the response
	 */
#if	DEBUG3
	(void) fprintf (stderr,"clntudp_call: decode & validate response\n");
#endif
	xdrmem_create(&reply_xdrs, cu->cu_inbuf, (uint)inlen, XDR_DECODE);
	ok = xdr_replymsg(&reply_xdrs, &reply_msg);
	/* XDR_DESTROY(&reply_xdrs);  save a few cycles on noop destroy */
	if (ok) {
		_seterr_reply(&reply_msg, &(cu->cu_error));
		if (cu->cu_error.re_status == RPC_SUCCESS) {
			if (! AUTH_VALIDATE(cl->cl_auth,
				&reply_msg.acpted_rply.ar_verf)) {
				cu->cu_error.re_status = RPC_AUTHERROR;
				cu->cu_error.re_why = AUTH_INVALIDRESP;
			}
			if (reply_msg.acpted_rply.ar_verf.oa_base != NULL) {
				xdrs->x_op = XDR_FREE;
				(void)xdr_opaque_auth(xdrs,
				    &(reply_msg.acpted_rply.ar_verf));
			} 
		}  /* end successful completion */
		else {
			/* maybe our credentials need to be refreshed ... */
			if (AUTH_REFRESH(cl->cl_auth))
				goto call_again;
		}  /* end of unsuccessful completion */
	}  /* end of valid reply message */
	else {
		cu->cu_error.re_status = RPC_CANTDECODERES;
	}
#if	DEBUG3
	(void) fprintf (stderr,"clntudp_call: end return(%x)\n",cu->cu_error.re_status);
#endif
	return (cu->cu_error.re_status);
}

static void
clntudp_geterr(cl, errp)
	CLIENT *cl;
	struct rpc_err *errp;
{
	register struct cu_data *cu = (struct cu_data *)cl->cl_private;

	*errp = cu->cu_error;
}


static bool_t
clntudp_freeres(cl, xdr_res, res_ptr)
	CLIENT *cl;
	xdrproc_t xdr_res;
	caddr_t res_ptr;
{
	register struct cu_data *cu = (struct cu_data *)cl->cl_private;
	register XDR *xdrs = &(cu->cu_outxdrs);

	xdrs->x_op = XDR_FREE;
	return ((*xdr_res)(xdrs, res_ptr));
}

static void 
clntudp_abort(/*h*/)
	/*CLIENT *h;*/
{
}

static void
clntudp_destroy(cl)
	CLIENT *cl;
{
	register struct cu_data *cu = (struct cu_data *)cl->cl_private;

#if	DEBUG > DEBUG4
	(void) fprintf (stderr,"clntudp_destroy: \n");
#endif
	XDR_DESTROY(&(cu->cu_outxdrs));
	mem_free((caddr_t)cu, (sizeof(*cu) + cu->cu_sendsz + cu->cu_recvsz));
	mem_free((caddr_t)cl, sizeof(CLIENT));
#if	DEBUG > DEBUG4
	(void) fprintf (stderr,"clntudp_destroy: return\n");
#endif
}
