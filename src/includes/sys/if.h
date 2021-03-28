/*
 * @(#) Copyright 1989.  The Wollongong Group, Inc.  All Rights Reserved.
 */

#ifndef SYS_IF_H
#define SYS_IF_H

#ident "@(#)if.h (TWG)  1.4     89/06/29 "

/*
 * For Compat with older versions of WIN/TCP
 */

#ifndef SYS_SOCKET_H
#include "sys/socket.h"
#endif

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)if.h	7.1 (Berkeley) 6/4/86
 */

/*
 * Structures defining a network interface, providing a packet
 * transport mechanism (ala level 0 of the PUP protocols).
 *
 * Each interface accepts output datagrams of a specified maximum
 * length, and provides higher level routines with input datagrams
 * received from its medium.
 *
 * Output occurs when the routine if_output is called, with three parameters:
 *	(*ifp->if_output)(ifp, m, dst)
 * Here m is the mbuf chain to be sent and dst is the destination address.
 * The output routine encapsulates the supplied datagram if necessary,
 * and then transmits it on its medium.
 *
 * On input, each interface unwraps the data received by it, and either
 * places it on the input queue of a internetwork datagram routine
 * and posts the associated software interrupt, or passes the datagram to a raw
 * packet input routine.
 *
 * Routines exist for locating interfaces by their addresses
 * or for locating a interface on a certain network, as well as more general
 * routing and gateway routines maintaining information used to locate
 * interfaces.  These routines live in the files if.c and route.c
 */

/*
 * Structure defining a queue for a network interface.
 *
 * (Would like to call this struct ``if'', but C isn't PL/1.)
 */
struct ifnet {
#define IFNAMESIZ	16
	char	if_name[IFNAMESIZ];	/* name, e.g. ``en'' or ``lo'' */
	short	if_unit;		/* sub-unit for lower level driver */
	short	if_mtu;			/* maximum transmission unit */
	short	if_flags;		/* up/down, broadcast, etc. */
	short	if_timer;		/* time 'til if_watchdog called */
	int	if_metric;		/* routing metric (external only) */
	struct	ifaddr *if_addrlist;	/* linked list of addresses per if */
/* procedure handles */
	int	(*if_init)();		/* init routine */
	int	(*if_output)();		/* output routine */
	int	(*if_ioctl)();		/* ioctl routine */
	int	(*if_reset)();		/* bus reset routine */
	int	(*if_watchdog)();	/* timer routine */
/* generic interface statistics */
	int	if_ipackets;		/* packets received on interface */
	int	if_ierrors;		/* input errors on interface */
	int	if_opackets;		/* packets sent on interface */
	int	if_oerrors;		/* output errors on interface */
	int	if_collisions;		/* collisions on csma interfaces */
/* link to next interface structure */
	struct	ifnet *if_next;
/* system V streams private info */
	unsigned short	ib_sap;		/* ethernet type or source sap # */
	unsigned char	ib_mstate;	/* mux state to bottom */
	unsigned char	ib_bottype;	/* subnet type: see lihdr.h */
	unsigned long	ib_speed;	/* in bits per second */
#ifdef KERNEL
	queue_t		*ib_wrq;	/* bottom write queue */
	unsigned long	ib_minpsz;	/* min packet size going down */
	queue_t		*ib_qtop;	/* control chan write q	*/
	unsigned long	ib_cookie;	/* cookie from I_LINK/UNLINK */
#else
	long	ib_filler[4];
#endif
	mblk_t	*ib_iocblk;		/* pending (I_UNLINK) ioctl */
};

/*
 * flags used for ip bottom queue ib_flag 
 */
#define	IFF_UP		(1<<0)		/* interface is up */
#define	IFF_BROADCAST	(1<<1)		/* broadcast address valid */
#define	IFF_DEBUG	(1<<2)		/* turn on debugging */
#define IFF_ROUTE	(1<<3)		/* routing entry installed */
#define	IFF_POINTOPOINT	(1<<4)		/* interface is point-to-point link */
#define	IFF_NOTRAILERS	(1<<5)		/* avoid use of trailers */
#define	IFF_RUNNING	(1<<6)		/* resources allocated */
#define	IFF_NOARP	(1<<7)		/* no address resolution protocol */
/* next two not supported now, but reserved: */
#define	IFF_PROMISC	(1<<8)		/* receive all packets */
#define	IFF_ALLMULTI	(1<<9)		/* receive all multicast packets */
/* additional flags used for streams implementation: */
#define IFF_ARP		(1<<10)		/* perform arp			*/
#define IFF_LOCAL	(1<<11)		/* for subnets. still needed ???*/
#define	IFF_LOOPBACK	(1<<12)		/* is a loopback net */
#define IFF_BINDNEED	(1<<13)		/* during binding		*/
#define IFF_IPTOX25	(1<<14)		/* for ACP IPX25 interface	*/
#define	IFF_IPTOVCS	(1<<15)		/* for Datakit VCS interface */
/* flags set internally only: */
#define	IFF_CANTCHANGE \
	(IFF_BROADCAST | IFF_POINTOPOINT | IFF_RUNNING | IFF_IPTOX25)

/*
 * The ifaddr structure contains information about one address
 * of an interface.  They are maintained by the different address families,
 * are allocated and attached when an address is set, and are linked
 * together so all addresses for an interface can be located.
 */

struct ifaddr {
	struct	sockaddr ifa_addr;	/* address of interface */
	union {
		struct	sockaddr ifu_broadaddr;
		struct	sockaddr ifu_dstaddr;
	} ifa_ifu;
#define	ifa_broadaddr	ifa_ifu.ifu_broadaddr	/* broadcast address */
#define	ifa_dstaddr	ifa_ifu.ifu_dstaddr	/* other end of p-to-p link */
	struct	ifnet *ifa_ifp;		/* back-pointer to the interface */
	struct	ifaddr *ifa_next;	/* next address for interface */
};

/*
 * Interface request structure used for socket
 * ioctl's.  All interface ioctl's must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */
struct	ifreq {
	int ifr_cookie;
#define	IFNAMSIZ	16
	char	ifr_name[IFNAMSIZ];		/* if name, e.g. "en0" */
	union {
		struct	sockaddr ifru_addr;
		struct sockproto ifru_saddr;
		struct	sockaddr ifru_dstaddr;
		struct	sockaddr ifru_broadaddr;
		ushort	ifru_flags;
		int	ifru_metric;
		caddr_t	ifru_data;
		ulong	ifru_ioctldata[2];
	} ifr_ifru;
#define	ifr_addr	ifr_ifru.ifru_addr	/* address */
#define	ifr_saddr	ifr_ifru.ifru_saddr	/* address */
#define	ifr_dstaddr	ifr_ifru.ifru_dstaddr	/* other end of p-to-p link */
#define	ifr_broadaddr	ifr_ifru.ifru_broadaddr	/* broadcast address */
#define	ifr_flags	ifr_ifru.ifru_flags	/* flags */
#define	ifr_metric	ifr_ifru.ifru_metric	/* metric */
#define	ifr_data	ifr_ifru.ifru_data	/* for use by interface */
#define ifr_ipackets	ifr_ifru.ifru_ioctldata[0]
#define ifr_opackets	ifr_ifru.ifru_ioctldata[1]
};

/*
 * Structure used in SIOCGIFCONF request.
 * Used to retrieve interface configuration
 * for machine (useful for programs which
 * must know all networks accessible).
 */
struct	ifconf {
	int	ifc_len;		/* size of associated buffer */
	union {
		caddr_t	ifcu_buf;
		struct	ifreq *ifcu_req;
	} ifc_ifcu;
#define	ifc_buf	ifc_ifcu.ifcu_buf	/* buffer address */
#define	ifc_req	ifc_ifcu.ifcu_req	/* array of structures returned */
};

#ifdef KERNEL
struct	ifaddr *ifa_ifwithaddr(), *ifa_ifwithnet();
struct	ifaddr *ifa_ifwithdstaddr();
#endif /* KERNEL */

#endif /* SYS_IF_H */
