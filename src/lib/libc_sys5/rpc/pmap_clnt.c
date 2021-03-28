#ident	"@(#)pmap_clnt.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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
#ident	"@(#)pmap_clnt.c	3.6 LAI System V NFS Release 3.2/V3	source"
#ifndef lint
#ident	"@(#)pmap_clnt.c 2.2 86/08/01 Copyr 1984 Sun Micro"
#endif

/*
 * pmap_clnt.c
 * Client interface to pmap rpc service.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */

#include <stdio.h>

#ifdef EXOS
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <sys/types.h>
#include <CMC/socket.h>
#include <CMC/in.h>
#endif

#include <sys/fs/nfs/time.h>
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <rpc/auth.h>
#include <rpc/clnt.h>
#include <rpc/rpc_msg.h>
#include <rpc/pmap_prot.h> 
#include <rpc/pmap_clnt.h>
/* #include <sys/ioctl.h>
   #include <net/if.h>  */

#define NAMELEN 255

static struct timeval timeout = { 5, 0 };
static struct timeval tottimeout = { 60, 0 };
static struct sockaddr_in myaddress;

/*#define DEBUG		5   */
	/* no printing */
#define	DEBUG0		0
	/* print bad news */
#define	DEBUG1		1
	/* print inportant good news */
#define	DEBUG2		2
	/* commentary */
#define	DEBUG3		3
	/* gossip */
#define	DEBUG4		4
#define	V5		1

void clnt_perror();


/*
 * Set a mapping between program,version and port.
 * Calls the pmap service remotely to do the mapping.
 */
bool_t
pmap_set(program, version, protocol, port)
	ulong program;
	ulong version;
	ulong protocol;
	ushort port;
{
	struct sockaddr_in myaddress;
	int socket = -1;
	register CLIENT *client;
	struct pmap parms;
	bool_t rslt;

#if	DEBUG > DEBUG4
	fprintf (stderr,
		 "pmap_set (program 0x%x, version %d, protocol %d, port %d)\n",
			program, version, protocol, port);
#endif

	get_myaddress(&myaddress);

#if	DEBUG5
	dump_in_addr (stderr, "", &myaddress);
#endif

#if	DEBUG > DEBUG4
	fprintf (stderr,"pmap_set: myaddress=%x\n",myaddress.sin_addr.s_addr);
#endif
	client = clntudp_bufcreate(&myaddress, PMAPPROG, PMAPVERS,
	    timeout, &socket, PMAPMSGSZ, PMAPMSGSZ);
	if (client == (CLIENT *)NULL)
{
#if	DEBUG > DEBUG4
	fprintf (stderr,"pmap_set: client=NULL return FALSE\n");
#endif
		return (FALSE);
}
	parms.pm_prog = program;
	parms.pm_vers = version;
	parms.pm_prot = protocol;
	parms.pm_port = port;

#if	DEBUG > DEBUG4
	fprintf (stderr,
		"pmap_set calling CLNT_CALL PMAPPROC_SET\n");
#endif

	if (CLNT_CALL(client, PMAPPROC_SET, xdr_pmap, &parms, xdr_bool, &rslt,
	    tottimeout) != RPC_SUCCESS) {

#if	DEBUG > DEBUG2
	fprintf (stderr,
		"pmap_set bad CLNT_CALL\n");
#endif

#if	DEBUG > DEBUG4
	fprintf (stderr,"pmap_set: Cannot register service\n");
#endif
		clnt_perror(client, "Cannot register service");
		return (FALSE);
	}

#if	DEBUG > DEBUG4
	fprintf (stderr,
		"pmap_set good CLNT_CALL\n");
	fflush (stderr);
#endif

	CLNT_DESTROY(client);

#if	DEBUG > DEBUG4
	fprintf (stderr,
		"pmap_set good CLNT_DESTROY\n");
	fflush (stderr);
#endif

	/*
	 *	  I can hardly believe this.
	 *	(void)close(socket);
	 */

	/* I can. -mre */
	(void)close(socket);


#if	DEBUG > DEBUG4
	fprintf (stderr,
		"pmap_set returning %d\n", rslt);
	fflush (stderr);
#endif

	return (rslt);
}

/*
 * Remove the mapping between program,version and port.
 * Calls the pmap service remotely to do the un-mapping.
 */
bool_t
pmap_unset(program, version)
	ulong program;
	ulong version;
{
	struct sockaddr_in myaddress;
	int socket = -1;
	register CLIENT *client;
	struct pmap parms;
	bool_t rslt;

#if	DEBUG > DEBUG4
	fprintf (stderr, "pmap_unset: pmap_set(program 0x%x,version %d)\n",program,version);
#endif

	get_myaddress(&myaddress);
	client = clntudp_bufcreate(&myaddress, PMAPPROG, PMAPVERS,
	    timeout, &socket, PMAPMSGSZ, PMAPMSGSZ);
	if (client == (CLIENT *)NULL)
		return (FALSE);
	parms.pm_prog = program;
	parms.pm_vers = version;
	parms.pm_port = parms.pm_prot = 0;
#if	DEBUG > DEBUG4
	fprintf (stderr,"pmap_unset: CLNT_CALL(%x)\n",client );
#endif
	CLNT_CALL(client, PMAPPROC_UNSET, xdr_pmap, &parms, xdr_bool, &rslt,
	    tottimeout);
#if	DEBUG > DEBUG4
	fprintf (stderr,"pmap_unset: CLNT_DESTROY(%x)\n",client );
#endif
	CLNT_DESTROY(client);
	(void)close(socket);
#if	DEBUG > DEBUG4
	fprintf (stderr,"pmap_unset: return rslt\n" );
#endif
	return (rslt);
}

/* 
 * don't use gethostbyname, which would invoke yellow pages
 */
get_myaddress(addr)
	struct sockaddr_in *addr;
{
	int s;

#ifdef EXOS 
	char hname[255];
	char *hnamep = hname;

	(void) gethostname(hname, sizeof(hname));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = rhost(&hnamep);
	addr->sin_port = htons(PMAPPORT);
#else	
#ifdef S3000
	char hname[255];
	char *hnamep = hname;

	(void) gethostname(hname, sizeof(hname));
#if	DEBUG > DEBUG4
	fprintf(stderr,"get_myaddress: hname=%s\n",hname);
#endif
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = rhost(&hnamep);
#if	DEBUG > DEBUG4
	fprintf(stderr,"get_myaddress: addr=%x\n", addr->sin_addr.s_addr);
#endif
	addr->sin_port = htons(PMAPPORT);
#else
	/* This is the current method available to retrieve the nodes addressi
	   with LAI's streams tcp/ip
	*/		

	char buf[BUFSIZ];
	struct ifconf ifc;
	struct ifreq ifreq, *ifr;
	int len;
	/*
	 * This is the method 4.[23] BSD uses to
	 * retrieve a node's address. Now, LAI's tcp/ip
	 * does it this way.
	 */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	    perror("get_myaddress: socket");
	    exit(1);
	}
	ifc.ifc_len = sizeof (buf);
	ifc.ifc_buf = buf;
		if (ioctl(s, SIOCGIFCONF, (char *)&ifc) < 0) {
		perror("get_myaddress: ioctl (get interface configuration)");
		exit(1);
	}
	ifr = ifc.ifc_req;
	for (len = ifc.ifc_len; len; len -= sizeof ifreq) {
		ifreq = *ifr;
		if (ioctl(s, SIOCGIFFLAGS, (char *)&ifreq) < 0) {
			perror("get_myaddress: ioctl");
			exit(1);
		}
		if ((ifreq.ifr_flags & IFF_UP) &&
		    ifr->ifr_addr.sa_family == AF_INET) {
			*addr = *((struct sockaddr_in *)&ifr->ifr_addr);
			addr->sin_port = htons(PMAPPORT);
			break;
		}
		ifr++;
	}
	close(s);
#endif
#endif
}
