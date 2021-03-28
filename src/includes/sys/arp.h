#ifndef ARP_H
#define ARP_H

/*
 * @(#) Copyright 1986.  The Wollongong Group, Inc.  All Rights Reserved.
 */

#ident "@(#)arp.h  (TWG)      1.6     89/08/28 "

/*
 * Ethernet Address Resolution Protocol.
 *
 * See RFC 826 for protocol description.  Structure below is adapted
 * to resolving internet addresses.  Field names used correspond to 
 * RFC 826.
 */
#ifdef XENIX
/* #pragma pack(1) */
#endif
struct	ether_arp {
	ushort	arp_hrd;	/* format of hardware address */
#define ARPHRD_ETHER 	1	/* ethernet hardware address */
#define ARPHRD_AMX25 	2	/* amateur radio x.25 */
#define ARPHRD_PRONET 	4	/* Proteon ProNET token ring */
#define ARPHRD_IEEE 	6	/* IEEE 802 networks */
	ushort	arp_pro;	/* format of proto. address (ETHERPUP_IPTYPE) */
	unchar	arp_hln;	/* length of hardware address (6) */
	unchar	arp_pln;	/* length of protocol address (4) */
	ushort	arp_op;
#define	ARPOP_REQUEST	1	/* request to resolve address */
#define	ARPOP_REPLY	2	/* response to previous request */
#define	RARPOP_REQUEST	3	/* request ip from phys address */
#define	RARPOP_REPLY	4	/* response to previous request */
	unchar	arp_sha[6];	/* sender hardware address */
	unchar	arp_spa[4];	/* sender protocol address */
	unchar	arp_tha[6];	/* target hardware address */
	unchar	arp_tpa[4];	/* target protocol address */
};

#ifdef XENIX
/* #pragma pack() */
#endif

/*
 * Internet (32 bit) to Ethernet/Starlan (48 bit) address translation table
 * the table ends when the IP address is 0
 * should be hashed!!!
 */

struct arptab {
	struct	arptab *at_next;/* The next arptab	*/
	ulong	at_in;		/* internet address	*/
	time_t	at_time;	/* aging		*/
	ushort	at_type;	/* subnet type		*/
	unchar	at_mac[6];	/* ethernet address	*/
	short	at_flags;	/* See below.		*/
	struct	arpb *at_ab;	/* from bottom queue	*/
	mblk_t	*at_last;	/* message waiting	*/
};
#define ATF_COM		02	/* The entry is complete	*/
#define ATF_PERM	04	/* A permanent Entry		*/
#define ATF_PUBL	010	/* Reply arp for another host	*/

/*
 * ioctl structure to arpld
 */
typedef struct arptab	arpioctl;

/*
 * arp module information
 */
typedef struct arp {
	queue_t	*ar_rdq;		/* read queue		*/
	ulong	ar_inaddr;		/* my internet addr	*/
	ushort	ar_sap;			/* lli sap		*/
	uchar	ar_state;		/* lli state		*/
	uchar	ar_flags;		/* lli flags		*/
	struct arpb *ar_first;		/* ptr to first bq	*/
} arp_t;
#define ARP_NEEDINFO	1

typedef struct	arpb {
	queue_t *ab_wrq;
	ulong	ab_inaddr;		/* my internet addr	*/
	int 	ab_cookie;		/* used by I_LINK/ULINK */
	ulong 	ab_ppa;			/* DLS provider cookie  */
	uchar	ab_state;		/* bound state of queue */
	uchar	ab_flags;		/* ARPB bitwise flags   */
	ushort	ab_dltype;		/* dl_send subnet type	*/
	ushort	ab_sap;			/* for 802 1byte sap,	*/
					/* for ether, 2b type	*/
	short	ab_pro;			/* proto format		*/
	ushort	ab_maxpsz;		/* max packet size	*/
	ushort	ab_minpsz;		/* min packet size	*/
	short	ab_type;		/* subnet type: lihdr.h	*/
	char	ab_sha[MACSIZE];	/* mac address		*/
	arp_t	*ab_ar;			/* top queue pointer 	*/
	struct arpb *ab_next;		/* ptr to next bottom q */
	mblk_t *ab_iocblk;		/* temp. hold on to M_IOCTL */ 
} arpb_t;
#define ARPB_PPA	1
#define ARPB_IP		2
#define ARPB_ARP	4
#define ARPB_HANGUP	8
#define ARPB_STOP	16

/*
 * ioctl known to arpld
 */
#define	ARPIOC		('I'<<8)
#define	ARP_INSRC	(ARPIOC|30)	/* my internet address	*/
#define	ARP_RESOLVE	(ARPIOC|31)	/* resolve inet-ether mapping */
#define	ARP_DELETE	(ARPIOC|32)	/* delete inet-ether mapping */
#define	ARP_GET		(ARPIOC|33)	/* get inet-ether mapping */
#define	ARP_INSAP	(ARPIOC|34)	/* bottom queue SAP value */
#define	ARP_INPPA	(ARPIOC|35)	/* bottom queue PPA value */
#define	ARP_INTOP	(ARPIOC|36)	/* Top queue setup cmd */
#define	ARP_GETTABLEN	(ARPIOC|37)	/* get size of ARP table */
#define	ARP_GETARPTAB	(ARPIOC|38)	/* get entire table contents */
#define	ARP_GETBOTQUE	(ARPIOC|39)	/* get bottom queue data structure */

#define	ARPAGE		(3600*HZ)	/* 1 hour before invalidation	*/
/* -ac created for arptimer routine 8/12/87 */
#define ARPT_AGE	(60*1)		/* age entry every minute */
#define ARPT_KILLC	20		/* kill completed entry in 20 mins */
#define ARPT_KILLI	3		/* kill incompleted entry in 3 mins */

#define	ARP_TRUE	1
#define	ARP_FALSE	0

#define NARP_BUCKETS	19
/* #define ARP_HASH(a)	\
	(((((unsigned long)(a)) >> 16) ^ ((a) & 0xffff)) % NARP_BUCKETS)
*/
#define ARP_HASH(addr)	 ((addr) % NARP_BUCKETS)
#define ARPTAB_LOOK(at, addr, type)  { int s = splstr(); \
	for(at = arp_bhash[ARP_HASH(addr)] ; \
		at ; at = at->at_next)  \
		if (at->at_in == addr && at->at_type == type) \
			break; \
	splx(s); }
#define ARPTAB_ENLOOK(at, addr, type)  { int s = splstr(); \
	for(at = arptab; at < &arptab[arptab_cnt]; ++at)  \
		if (bcmp(at->at_mac,addr,MACSIZE) == 0 && at->at_type == type) \
			break; \
	splx(s); }

#endif /* ARP_H */
