#ifndef NO_SCCS_ID
#ident "@(#)in_var.h (TWG)  1.1     89/05/17 "
#endif /*NO_SCCS_ID*/

/*
 * @(#) Copyright 1989.  The Wollongong Group, Inc.  All Rights Reserved.
 */

#ifndef SYS_IN_VAR_H
#define SYS_IN_VAR_H

/*
 * Copyright (c) 1985, 1986 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 *
 *	@(#)in_var.h	7.2 (Berkeley) 12/7/87
 */

/*
 * Interface address, Internet version.  One of these structures
 * is allocated for each interface with an Internet address.
 * The ifaddr structure contains the protocol-independent part
 * of the structure and is assumed to be first.
 */
struct in_ifaddr {
	struct	ifaddr ia_ifa;		/* protocol-independent info */
#define	ia_addr	ia_ifa.ifa_addr
#define	ia_broadaddr	ia_ifa.ifa_broadaddr
#define	ia_dstaddr	ia_ifa.ifa_dstaddr
#define	ia_ifp		ia_ifa.ifa_ifp
	ulong	ia_net;			/* network number of interface */
	ulong	ia_netmask;		/* mask of net part */
	ulong	ia_subnet;		/* subnet number, including net */
	ulong	ia_subnetmask;		/* mask of net + subnet */
	struct	in_addr ia_netbroadcast; /* broadcast addr for (logical) net */
	int	ia_flags;
	struct	in_ifaddr *ia_next;	/* next in list of internet addresses */
};

/*
 * Given a pointer to an in_ifaddr (ifaddr),
 * return a pointer to the addr as a sockadd_in.
 */
#define	IA_SIN(ia) ((struct sockaddr_in *)(&((struct in_ifaddr *)ia)->ia_addr))

/*
 * ia_flags
 */
#define	IFA_ROUTE	0x01		/* routing entry installed */

#ifdef	KERNEL
struct	in_ifaddr *in_ifaddr;
struct	in_ifaddr *in_iaonnetof();
#endif

/*
 * Given a sockaddr value, return 
 * sin_addr or sin_addr.s_addr fields.
 */
#define satoinaddr(x)	(((struct sockaddr_in *)&(x))->sin_addr)
#define satosaddr(x)	(((struct sockaddr_in *)&(x))->sin_addr.s_addr)

#endif /* SYS_IN_VAR_H */
