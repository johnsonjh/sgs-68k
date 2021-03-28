/*
 * @(#) Copyright 1989.  The Wollongong Group, Inc.  All Rights Reserved.
 */

#ident "@(#)route.h (TWG)  1.11     89/07/30 "

#ifndef SYS_ROUTE_H
#define SYS_ROUTE_H

/*
 * Copyright (c) 1980, 1986 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 *
 *	@(#)route.h	7.3 (Berkeley) 12/30/87
 */

/*
 * Kernel resident routing tables.
 * 
 * The routing tables are initialized when interface addresses
 * are set by making entries for all directly connected interfaces.
 */

/*
 * We distinguish between routes to hosts and routes to networks,
 * preferring the former if available.  For each route we infer
 * the interface to use from the gateway address supplied when
 * the route was entered.  Routes that forward packets through
 * gateways are marked so that the output routines know to address the
 * gateway rather than the ultimate destination.
 */
struct rtentry {
	ulong	rt_hash;		/* to speed lookups */
	struct	in_addr rt_dst;		/* key */
	struct	in_addr rt_gateway;	/* value */
	uchar	rt_flags;		/* up/down?, host/net */
	uchar	rt_tos;			/* Type-of-Service field */
	uchar	rt_protocol;		/* route from Src/Protocol X */
	uchar	rt_priority;		/* priority of route */
	ulong	rt_use;			/* raw # packets forwarded */
	struct	ifnet *rt_ifp;		/* the answer: interface to use */
	struct	rtentry *rt_next;	/* linked-list */
	ulong	rt_net;			/* in_netof() of rt_dst */
	ulong	rt_time;		/* last modified time stamp */
};

/* The route types added for SNMP RFC1066 support */
#define	RTR_OTHER	1		/* OTHER sources (internal) */
#define	RTR_LOCAL	2		/* manually added routes */
#define	RTR_NETMGMT	3		/* SNMP added routes */
#define	RTR_ICMP	4		/* ICMP redirects */
#define	RTR_EGP		5		/* EGP routes (see gated) */
#define	RTR_HELLO	7		/* HELLO routes (see gated) */
#define	RTR_RIP		8		/* RIP routes (see gated) */

#define	RTF_UP		0x1		/* route useable */
#define	RTF_GATEWAY	0x2		/* destination is a gateway */
#define	RTF_HOST	0x4		/* host entry (net otherwise) */
#define	RTF_DYNAMIC	0x10		/* created dynamically (by redirect) */
#define	RTF_MODIFIED	0x20		/* modified dynamically (by redirect) */
#define	RTF_INUSE	0x80		/* route table entry in use */

/*
 * Routing statistics.
 */
struct	rtstat {
	short	rts_badredirect;	/* bogus redirect calls */
	short	rts_dynamic;		/* routes created by redirects */
	short	rts_newgateway;		/* routes modified by redirects */
	short	rts_unreach;		/* lookups which failed */
	short	rts_wildcard;		/* lookups satisfied by a wildcard */
};

#define	RTHASHSIZ	32

#ifdef KERNEL
#if	(RTHASHSIZ & (RTHASHSIZ - 1)) == 0
#define RTHASHMOD(h)	((h) & (RTHASHSIZ - 1))
#else
#define RTHASHMOD(h)	((h) % RTHASHSIZ)
#endif
struct	rtstat	rtstat;
#endif

/*
 * Route lookup structure
 */
struct route_dst {
	struct in_addr	rtd_dst;
	struct ifnet	*rtd_ifp;
	unsigned	rtd_flags;
};

/*
 * To accomodate the differences between 
 * streams and 4.3 kernel route structures.
 */
struct route_map {
	ulong		rtk_hash;
	struct sockaddr	rtk_dst;
	struct sockaddr	rtk_router;
	short		rtk_flags;
	uchar		rtk_tos;
	uchar		rtk_protocol;
	struct route_map *rtk_next;
};

#endif /* SYS_ROUTE_H */
