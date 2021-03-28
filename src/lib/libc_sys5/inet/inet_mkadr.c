#ident	"@(#)inet_mkadr.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
 *	Copyright 1986 Lachman Associates Inc. and The Instruction Set Ltd.
 */
#ident	"@(#)inet_mkadr.c	2.2 LAI KNFS for ARETE source"
#ident	"@(#)inet_mkadr.c	2.6 System V NFS source"
/*	inet_makeaddr.c	4.3	82/11/14	*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/fs/nfs/inet_insup.h>

#ifdef EXOS
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <CMC/socket.h>
#include <CMC/in.h>
#endif
/*#define DEBUG	1 */

/*
 * Formulate an Internet address from network + host.  Used in
 * building addresses stored in the ifnet structure.
 */
struct in_addr
inet_makeaddr(net, host)
	int net, host;
{
	ulong addr;

#ifdef DEBUG
fprintf(stderr, "inet_makeaddr: net=%8x host=%8x IN_CLASSA_NSHIFT=%d\n", net, host, IN_CLASSA_NSHIFT);
#endif
	if (net < 128)
		addr = (net << IN_CLASSA_NSHIFT) | host;
	else if (net < 65536)
		addr = (net << IN_CLASSB_NSHIFT) | host;
	else
		addr = (net << IN_CLASSC_NSHIFT) | host;
	addr = htonl(addr);
#ifdef DEBUG
fprintf(stderr, "inet_makeaddr: net=%8x host=%8x addr=%8x\n", net, host, addr);
#endif
	return (*(struct in_addr *)&addr);
}
