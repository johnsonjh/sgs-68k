#ident	"@(#)clnt_simp.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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
#ident	"@(#)clnt_simp.c	3.4 LAI System V NFS Release 3.2/V3	source"
/* 
 * clnt_simple.c
 * Simplified front end to rpc.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */

#include <stdio.h>

#ifdef EXOS
#include <sys/socket.h>
#else
#include <sys/types.h>
#include <CMC/socket.h>
#endif

#include <sys/fs/nfs/time.h>
#include <rpc/rpc.h>
#include <rpc/netdb.h>
/*#define DEBUG   1 */

callrpc(host, prognum, versnum, procnum, inproc, in, outproc, out)
	char *host;
	xdrproc_t inproc, outproc;
	char *in, *out;
{
	struct sockaddr_in server_addr;
	enum clnt_stat clnt_stat;
	struct hostent *hp;
	struct timeval timeout, tottimeout;

	static CLIENT *client;
	static int sock = RPC_ANYSOCK;
	static int oldprognum, oldversnum, valid;
	static char oldhost[256];

	if (valid && oldprognum == prognum && oldversnum == versnum
		&& strcmp(oldhost, host) == 0) {
		/* reuse old client */		
	}
	else {
		(void) close(sock);
		sock = RPC_ANYSOCK;
		if (client) {
			clnt_destroy(client);
			client = NULL;
		}
#ifdef DEBUG
fprintf(stderr,"callrpc: gethostbyname(%s)\n",host);
#endif
		if ((hp = gethostbyname(host)) == NULL)
			return ((int) RPC_UNKNOWNHOST);
		timeout.tv_usec = 0;
		timeout.tv_sec = 5;
		memcpy((char *)&server_addr.sin_addr, hp->h_addr, hp->h_length);
#ifdef DEBUG
fprintf(stderr,"callrpc: gethostbyname ret server_addr=%x\n",server_addr.sin_addr.s_addr); 
#endif
		server_addr.sin_family = AF_INET;
		server_addr.sin_port =  0;
#ifdef DEBUG
fprintf(stderr,"callrpc: clntudp_create\n");
#endif
		if ((client = clntudp_create(&server_addr, prognum,
		    versnum, timeout, &sock)) == NULL)
{
#ifdef DEBUG
fprintf(stderr,"callrpc: clntudp_create NULL return %x\n",rpc_createerr.cf_stat);
#endif
			return ((int) rpc_createerr.cf_stat);
}
		valid = 1;
		oldprognum = prognum;
		oldversnum = versnum;
		(void) strcpy(oldhost, host);
	}
	tottimeout.tv_sec = 25;
	tottimeout.tv_usec = 0;
#ifdef DEBUG
fprintf(stderr,"callrpc: clnt_call(%x)\n",client);
#endif
	clnt_stat = clnt_call(client, procnum, inproc, in,
	    outproc, out, tottimeout);
#ifdef DEBUG
fprintf(stderr,"callrpc: clnt_call return clnt_stat=%x\n",clnt_stat);
#endif
	/* 
	 * if call failed, empty cache
	 */
	if (clnt_stat != RPC_SUCCESS)
		valid = 0;
	return ((int) clnt_stat);
}
