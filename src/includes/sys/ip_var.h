/*
 * @(#) Copyright 1989.  The Wollongong Group, Inc.  All Rights Reserved.
 */

#ifndef SYS_IP_VAR_H
#define SYS_IP_VAR_H

#ident "@(#)ip_var.h (TWG)  1.5     89/08/02 "

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
 *	@(#)ip_var.h	7.4 (Berkeley) 1/7/88
 */

/*
 * Ip reassembly queue structure.  Each fragment
 * being reassembled is attached to one of these structures.
 * They are timed out after ipq_ttl drops to 0, and may also
 * be reclaimed if memory becomes tight.
 */
struct ipq {
	struct	ipq *next,*prev;	/* to other reass headers */
	uchar	ipq_ttl;		/* time for reass q to live */
	uchar	ipq_p;			/* protocol of this fragment */
	ushort	ipq_id;			/* sequence id for reassembly */
	struct	ipasfrag *ipq_next,*ipq_prev;
					/* to ip headers of fragments */
	struct	in_addr ipq_src,ipq_dst;
	mblk_t	*ipq_dtom;
};

/*
 * Ip header, when holding a fragment.
 *
 * Note: can't use elements outside of IPHLEN.
 *   ipf_next and ipf_prev must be at same offset as ipq_next above.
 */
struct	ipasfrag {
#if BYTE_ORDER == LITTLE_ENDIAN 
	uchar	ipf_hl:4,
		ipf_v:4;
#endif
#if BYTE_ORDER == BIG_ENDIAN 
	uchar	ipf_v:4,
		ipf_hl:4;
#endif
	uchar	ipf_mff;		/* copied from (ip_off&IP_MF) */
	short	ip_len;
	ushort	ip_id;
	short	ip_off;
	mblk_t	*ipf_dtom;
	struct	ipasfrag *ipf_next;	/* next fragment */
	struct	ipasfrag *ipf_prev;	/* previous fragment */
	struct	in_addr ipf_src, ipf_dst;
};

/*
 * Structure stored in mbuf in inpcb.ip_options
 * and passed to ip_output when ip options are in use.
 * The actual length of the options (including ipopt_dst)
 * is in m_len.
 */
#define MAX_IPOPTLEN	40

struct ipoption {
	struct	in_addr ipopt_dst;	/* first-hop dst if source routed */
	char	ipopt_list[MAX_IPOPTLEN];	/* options proper */
};

struct	ipstat {
	long	ips_total;		/* total packets received */
	long	ips_badsum;		/* checksum bad */
	long	ips_tooshort;		/* packet too short */
	long	ips_toosmall;		/* not enough data */
	long	ips_badhlen;		/* ip header length < data size */
	long	ips_badlen;		/* ip length < ip header length */
	long	ips_fragments;		/* fragments received */
	long	ips_fragdropped;	/* frags dropped (dups, out of space) */
	long	ips_fragtimeout;	/* fragments timed out */
	long	ips_forward;		/* packets forwarded */
	long	ips_cantforward;	/* packets rcvd for unreachable dest */
	long	ips_redirectsent;	/* packets forwarded on same net */
/*
** Variables added to reflect RFC 1066 for Network Management
*/
	long	ipInReceives;		/* total number of input Dgrams */
	long	ipInHdrErrors;		/* Dgrams chucked for bad Header */
	long	ipInAddrErrors;		/* Dgrams chucked for bad IP Add */
	long 	ipForwDatagrams;	/* Dgrams forwarded */
	long 	ipInUnknownProtos;	/* Dgrams chucked for bad protocol */
	long	ipInDiscards;		/* Dgrams chucked for no buff space */
	long	ipInDelivers;		/* Dgrams delivered up the stack */
	long	ipOutRequests;		/* Dgrams delivered to net */
	long	ipOutDiscards;		/* Dgrams chucked for no buff space */
	long	ipOutNoRoutes;		/* Dgrams chuched for no routes */
	long	ipReasmReqds;		/* Frags rcvd requiring reassembly */
	long	ipReasmOKs;		/* Dgrams reassembled */
	long	ipReasmFails;		/* Reassembly Failures */
        long	ipFragOKs;		/* Dgrams successfully fragmented */
	long	ipFragFails;		/* Dgrams chucked for "Don't Frag" */
	long	ipFragCreates;		/* Frags generated at this host */
};

/*
 * External Interface to IP Managers (for DATAKIT)
 */
struct	ipmgr {
	long	ipm_code;
#define		IPM_INCALL	661	/* Incoming Call (I_SENDFD preceeds) */
#define		IPM_HANGUP	662	/* Take Interface Down */
#define		IPM_CMDTYPE	663	/* Not implemented - for future use */
	long	ipm_inaddr;
	long	ipm_error;
	long	ipm_subcmd;		/* Not implemented - for future use */
	char	ipm_ifname[IFNAMESIZ];
#define		IPMGRPSIZ	48
	char	ipm_fromgroup[IPMGRPSIZ];
	char	ipm_iodata[IPMGRPSIZ];	/* Space for I/O data */
	long	ipm_timeout;
};

/*
 * The following are Wollongong IP driver data structures
 * Copyright (c) 1988
 */

/*
 * ip upper queues private data structure
 */
struct ipd {
	queue_t	*id_rdq;	/* read queue */
	short	id_topid;	/* who is on top */
	char	id_mstate;	/* mux state to top */
	uchar	id_prot;	/* tcp/udp/... protocol # */
	short	id_flags;
};

extern struct ifnet ipb[];
extern struct ipd ipd[];
extern ipd_cnt, ipb_cnt;
extern struct ifnet *ipbnext;

#define	IPCHAN(idp)	((int)idp - (int)ipd) / sizeof(struct ipd)
#define	IPBCHAN(ifp)	((int)ifp - (int)ipb) / sizeof(struct ifnet)
#define IPCTLCHAN	0		/* ip control channel		*/

/*
 * Commands from TCP/UDP/RAW in M_CTL
 */
#define IPIF_UNLINKED	0		/* Module is unlinked    	*/
#define IPIF_LINKED	1		/* Module is linked    		*/
#define IPIF_LINKING	2		/* Modules are linking		*/
#define IPIF_UNLINKING	3		/* Module is unlinking 		*/
#define IPIF_GETMYADDR	4		/* Get the internet addr	*/
/*#define IPIF_DATANXT	5		/* Data Follows			*/
#define IPIF_CLOSED	6		/* The Above Layer is closed	*/
#define IPIF_CTLERROR	7		/* This is a control Error 	*/
#define IPIF_GETHISADDR	8		/* Get his internet addr	*/
#define IPIF_OPTIONS	9		/* Set or get options		*/
#define IPIF_GETROUTE	10		/* Get cached route to dst	*/
#define IPIF_NGETMYADDR	11		/* New get the internet addr	*/
#define IPIF_NGETHISADDR 12

/*
 * The interface to IP from TCP/UDP/RAW.
 */
union iptun {
#ifdef IPIF_DATANXT
	/*
	 * IPIF_DATANXT.
	 */
	struct ipindata {
		ulong	ipin_src;		/* Source addr.		*/
		ulong	ipin_dst;		/* Dest. addr		*/
		struct	ifnet *ipin_rt;		/* XXX passed		*/
	} ipindata;
#endif /* IPIF_DATANXT */
	/*
	 * Linking Stuff
	 */
	long	ipln_id;			/* Module Id.		*/
	/*
	 * IPIF_GETMYADDR
	 */
	struct ipun_addr {
		ulong	ipad_addr;		/* Address		*/
		long	ipad_mtu;		/* MTU size, interface	*/
		long	ipad_mss;		/* MSS size, interface	*/
		long	ipad_flags;		/* flags for interface	*/
		long	ipad_thruput;		/* thruput bits/second	*/
		long	ipad_tos;		/* Type-of-Service	*/
		long	ipad_id;	/* sanity check id */
		char    *ipad_priv;	/* private data structure pointer */
	} ipun_addr;
	/*
	 * ICMP messages sent upstream
	 */
	struct ipcp_msg {
		long	ipcp_error;		/* ICMP Error codes     */
		ulong	ipcp_dst;		/* Dest. addr		*/
	} ipcp_msg;
	/*
	 * Route cache query
	 */
	struct ipcp_rt {
		long	iprt_dst;		/* Destination to cache */
	} ipcp_routec;
};

typedef short ipfcmd;
struct ip_if {
	ipfcmd ipf_type;	/* The command	*/
	union iptun ipf_un;	/* All the rest.*/
};

#ifdef IPIF_DATANXT
#define ipf_src	ipf_un.ipindata.ipin_src
#define ipf_dst ipf_un.ipindata.ipin_dst
#define ipf_rt	ipf_un.ipindata.ipin_rt
#endif /* IPIF_DATANXT	*/
#define ipf_id  ipf_un.ipln_id
#define ipf_addr ipf_un.ipun_addr.ipad_addr
#define ipf_mtu ipf_un.ipun_addr.ipad_mtu
#define ipf_mss ipf_un.ipun_addr.ipad_mss
#define ipf_tos ipf_un.ipun_addr.ipad_tos
#define ipf_srt	ipf_un.ipun_addr.ipad_rt
#define ipf_iocid ipf_un.ipun_addr.ipad_id
#define ipf_priv ipf_un.ipun_addr.ipad_priv
#define ipf_errorcode ipf_un.ipcp_msg.ipcp_error
#define ipf_errordst ipf_un.ipcp_msg.ipcp_dst

/*
 * The following defines and IP_args structure are for
 * sending "arguments" down to IP from the upper level protocol.
 * The argument list should be ended with IPENDLIST as the argument type.
 * The block after the arguments block will contain data.
 */
#define IPENDLIST	0
#define IPFLAGS		1
#define IPOPTS		3
#define IPROUTEC	7
struct IP_args {
	ushort IP_type;		/* argument type */
	ushort IP_length;	/* argument length */
};

#ifdef KERNEL
/* flags passed to ip_output as last parameter (see sys/socket.h!!!) */
#define	IP_FORWARDING		0x01	/* most of ip header exists */
#define	IP_ROUTETOIF		0x10	/* bypass routing tables */
#define	IP_ALLOWBROADCAST	0x20	/* can send broadcast packets */

struct	ipstat	ipstat;
extern	ushort	ip_id;			/* ip packet ctr, for ids */

mblk_t	*ip_srcroute();
#endif

#endif /* SYS_IP_VAR_H */
