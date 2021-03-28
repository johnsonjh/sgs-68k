#ident	"@(#)inet_lnaof.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
 *	Copyright 1986 Lachman Associates Inc. and The Instruction Set Ltd.
 */
#ident	"@(#)inet_lnaof.c	2.2 LAI KNFS for ARETE source"
#ident	"@(#)inet_lnaof.c	2.6 System V NFS source"
/*	inet_lnaof.c	4.3	82/11/14	*/


#include <stdio.h>
#include <sys/types.h>
#include <sys/fs/nfs/inet_insup.h>

#ifdef EXOS
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <CMC/socket.h>
#include <CMC/in.h>
typedef unsigned long u_long;
#endif
/*
 * Return the local network address portion of an
 * internet address; handles class a/b/c network
 * number formats.
 */
inet_lnaof(in)
	struct in_addr in;
{
#ifdef EXOS
	return(ntohl(IN_LNAOF(in)));
#else
	register u_long i = ntohl(in.s_addr);

	if (IN_CLASSA(i))
		return ((i)&IN_CLASSA_HOST);
	else if (IN_CLASSB(i))
		return ((i)&IN_CLASSB_HOST);
	else
		return ((i)&IN_CLASSC_HOST);
#endif
}
