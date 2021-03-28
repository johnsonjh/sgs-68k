/*
 * @(#) Copyright 1989.  The Wollongong Group, Inc.  All Rights Reserved.
 */

#ident "@(#)ip.h (TWG)  1.2     89/07/21 "

#ifndef SYS_IP_H
#define SYS_IP_H

/*
 * For user compat with old version of WIN/TCP
 */
#ifndef SYS_INET_H
#include "sys/inet.h"
#endif
#ifndef SYS_IF_H
#include "sys/if.h"
#endif
#ifndef SYS_IN_H
#include "sys/in.h"
#endif

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 *
 *	@(#)ip.h	7.6.1.1 (Berkeley) 3/15/88
 */

#ifndef BYTE_ORDER
/*
 * Definitions for byte order,
 * according to byte significance from low address to high.
 */
#define	LITTLE_ENDIAN	1234	/* least-significant byte first (vax) */
#define	BIG_ENDIAN	4321	/* most-significant byte first (IBM, net) */
#define	PDP_ENDIAN	3412	/* LSB first in word, MSW first in long (pdp) */

#if defined(vax) || defined(i386)
#define	BYTE_ORDER	LITTLE_ENDIAN
#else
#define	BYTE_ORDER	BIG_ENDIAN	/* mc68000, tahoe, most others */
#endif
#endif

/*
 * Definitions for internet protocol version 4.
 * Per RFC 791, September 1981.
 */
#define	IPVERSION	4

/*
 * Structure of an internet header, naked of options.
 *
 * We declare ip_len and ip_off to be short, rather than u_short
 * pragmatically since otherwise unsigned comparisons can result
 * against negative integers quite easily, and fail in subtle ways.
 */
struct ip {
#if BYTE_ORDER == LITTLE_ENDIAN 
	uchar	ip_hl:4,		/* header length */
		ip_v:4;			/* version */
#endif
#if BYTE_ORDER == BIG_ENDIAN 
	uchar	ip_v:4,			/* version */
		ip_hl:4;		/* header length */
#endif
	uchar	ip_tos;			/* type of service */
	short	ip_len;			/* total length */
	ushort	ip_id;			/* identification */
	short	ip_off;			/* fragment offset field */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
	uchar	ip_ttl;			/* time to live */
	uchar	ip_p;			/* protocol */
	ushort	ip_sum;			/* checksum */
	struct	in_addr ip_src,ip_dst;	/* source and dest address */
};

/*
 * Overlay for ip header used by other protocols (tcp, udp).
 */
struct ipovly {
	caddr_t	ih_next, ih_prev;	/* for protocol sequence q's */
	uchar	ih_x1;			/* see transport flags */
	uchar	ih_pr;			/* protocol */
	short	ih_len;			/* protocol length */
	struct	in_addr ih_src;		/* source internet address */
	struct	in_addr ih_dst;		/* destination internet address */
};
#define IPO_BROADCAST	0x01		/* ih_x1 - broadcast flag */

#define	IPHLEN		sizeof(struct ip)
#define	IPMAXHLEN	64		/* max IP header size */
#define	IP_MAXPACKET	65535		/* maximum packet size */

/*
 * Definitions for options.
 */
#define	IPOPT_COPIED(o)		((o)&0x80)
#define	IPOPT_CLASS(o)		((o)&0x60)
#define	IPOPT_NUMBER(o)		((o)&0x1f)

#define	IPOPT_CONTROL		0x00
#define	IPOPT_RESERVED1		0x20
#define	IPOPT_DEBMEAS		0x40
#define	IPOPT_RESERVED2		0x60

#define	IPOPT_EOL		0		/* end of option list */
#define	IPOPT_NOP		1		/* no operation */

#define	IPOPT_RR		7		/* record packet route */
#define	IPOPT_TS		68		/* timestamp */
#define	IPOPT_SECURITY		130		/* provide s,c,h,tcc */
#define	IPOPT_LSRR		131		/* loose source route */
#define	IPOPT_EXTSECURITY	133		/* extended security */
#define	IPOPT_SATID		136		/* satnet id */
#define	IPOPT_SSRR		137		/* strict source route */

/*
 * Offsets to fields in options other than EOL and NOP.
 */
#define	IPOPT_OPTVAL		0		/* option ID */
#define	IPOPT_OLEN		1		/* option length */
#define IPOPT_OFFSET		2		/* offset within option */
#define	IPOPT_MINOFF		4		/* min value of above */

/*
 * Time stamp option structure.
 */
struct	ip_timestamp {
	uchar	ipt_code;		/* IPOPT_TS */
	uchar	ipt_len;		/* size of structure (variable) */
	uchar	ipt_ptr;		/* index of current entry */
#if BYTE_ORDER == LITTLE_ENDIAN 
	uchar	ipt_flg:4,		/* flags, see below */
		ipt_oflw:4;		/* overflow counter */
#endif
#if BYTE_ORDER == BIG_ENDIAN 
	uchar	ipt_oflw:4,		/* overflow counter */
		ipt_flg:4;		/* flags, see below */
#endif
	union ipt_timestamp {
		n_long	ipt_time[1];
		struct	ipt_ta {
			struct in_addr ipt_addr;
			n_long ipt_time;
		} ipt_ta[1];
	} ipt_timestamp;
};

/* hack for i386 compiler bug */
#define IPT_FLG(ipt)	((ipt)->ipt_flag & 0x0f)
#define IPT_OVFLW(ipt)	(((ipt)->ipt_flag >>4) & 0x0f)

/* flag bits for ipt_flg */
#define	IPOPT_TS_TSONLY		0		/* timestamps only */
#define	IPOPT_TS_TSANDADDR	1		/* timestamps and addresses */
#define	IPOPT_TS_PRESPEC	2		/* specified modules only */

/* bits for security (not byte swapped) */
#ifdef notdef
#define	IPOPT_SECUR_UNCLASS	0x0000
#define	IPOPT_SECUR_CONFID	0xf135
#define	IPOPT_SECUR_EFTO	0x789a
#define	IPOPT_SECUR_MMMM	0xbc4d
#define	IPOPT_SECUR_RESTR	0xaf13
#define	IPOPT_SECUR_SECRET	0xd788
#define	IPOPT_SECUR_TOPSECRET	0x6bc5
#endif

/*
 * U.S. classification levels supported by the new variable length IP Security
 * Option.  Dated 18 MAR 1985.
 */
#define	IPOPT_SECUR_TOPSECRET	0xde
#define	IPOPT_SECUR_SECRET	0xad
#define	IPOPT_SECUR_CONFID	0x7a
#define	IPOPT_SECUR_UNCLASS	0x55
#define IPOPT_SECUR_LENGTH	4

/*
 * The current list of Accrediting Authorities allowed.
 */
#define IPOPT_SECUR_MASK	0xf0
#define IPOPT_SECUR_DCA		0x80	/* Defense Communications Agency */
#define IPOPT_SECUR_JCS		0x40	/* Joint Chiefs of Staff */
#define IPOPT_SECUR_DIA		0x20	/* Defense Intelligence Agency */
#define IPOPT_SECUR_NSA		0x10	/* National Security Agency */
#define IPOPT_SECUR_MORE	0x1	/* More Accrediting Authorities */

/*
 * Structure for the variable length IP Security Option.
 * As of 18 MAR 1985, there is only one byte for the authority field.
 * If this changes, change -- IPOPT_SECUR_LENGTH define.
 */
struct ipoption_security {
	uchar	ipsec_type;		/* 130 */
	uchar	ipsec_length;		/* IPOPT_SECUR_LENGTH */
	uchar	ipsec_class;		/* IPOPT_SECUR_TOPSECRET ... */
	uchar	ipsec_authority[1];	/* IPOPT_SECUR_DCA ... */
};

/* structure for Stream identifier */
struct	ip_satid{
	uchar	ipsat_code;		/* IPOPT_SATID		 */
	uchar	ipsat_len;		/* size of structure (4) */
	ushort	ipsat_type;		/* stream identifier	 */
};

/*
 * Internet implementation parameters.
 */
#define	MAXTTL		255		/* maximum time to live (seconds) */
#define	IPFRAGTTL	60		/* time to live for frags, slowhz */
#define	IPTTLDEC	1		/* subtracted when forwarding */

#define	IP_MSS		576		/* default maximum segment size */

/*
 * IP/ICMP Error Messages.
 */
#define	PRC_IFDOWN		0	/* interface transition */
#define	PRC_ROUTEDEAD		1	/* select new route if possible */
#define	PRC_QUENCH		4	/* some said to slow down */
#define	PRC_MSGSIZE		5	/* message size forced drop */
#define	PRC_HOSTDEAD		6	/* normally from IMP */
#define	PRC_HOSTUNREACH		7	/* ditto */
#define	PRC_UNREACH_NET		8	/* no route to network */
#define	PRC_UNREACH_HOST	9	/* no route to host */
#define	PRC_UNREACH_PROTOCOL	10	/* dst says bad protocol */
#define	PRC_UNREACH_PORT	11	/* bad port # */
#define	PRC_UNREACH_NEEDFRAG	12	/* IP_DF caused drop */
#define	PRC_UNREACH_SRCFAIL	13	/* source route failed */
#define	PRC_REDIRECT_NET	14	/* net routing redirect */
#define	PRC_REDIRECT_HOST	15	/* host routing redirect */
#define	PRC_REDIRECT_TOSNET	16	/* redirect for type of service & net */
#define	PRC_REDIRECT_TOSHOST	17	/* redirect for tos & host */
#define	PRC_TIMXCEED_INTRANS	18	/* packet lifetime expired in transit */
#define	PRC_TIMXCEED_REASS	19	/* lifetime expired on reass q */
#define	PRC_PARAMPROB		20	/* header incorrect */

#define	PRC_NCMDS		21

#ifdef PRCREQUESTS
char	*prcrequests[] = {
	"IFDOWN", "ROUTEDEAD", "#2", "#3",
	"QUENCH", "MSGSIZE", "HOSTDEAD", "HOSTUNREACH",
	"NET-UNREACH", "HOST-UNREACH", "PROTO-UNREACH", "PORT-UNREACH",
	"FRAG-UNREACH", "SRCFAIL-UNREACH", "NET-REDIRECT", "HOST-REDIRECT",
	"TOSNET-REDIRECT", "TOSHOST-REDIRECT", "TX-INTRANS", "TX-REASS",
	"PARAMPROB"
};
#endif

#endif /* SYS_IP_H */
