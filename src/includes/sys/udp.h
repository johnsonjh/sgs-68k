#ifndef UDP_H
#define UDP_H

/*
 * @(#) Copyright 1986.  The Wollongong Group, Inc.  All Rights Reserved.
 */

#ident "@(#)udp.h  (TWG)      1.4     89/08/02 "

/*
 * Udp protocol header.
 * Per RFC 768, September, 1981.
 */
#ifdef XENIX
/* #pragma pack() */
#endif 
struct udphdr {
	ushort	uh_sport;		/* source port */
	ushort	uh_dport;		/* destination port */
	short	uh_ulen;		/* udp length */
	ushort	uh_sum;			/* udp checksum */
};

/*
 * udp+ip header, after ip options removed
 */
struct	udpiphdr {
	struct 	ipovly ui_i;		/* overlaid ip structure */
	struct	udphdr ui_u;		/* udp header */
};

#ifdef XENIX
/* #pragma pack() */
#endif

#define	ui_next		ui_i.ih_next
#define	ui_prev		ui_i.ih_prev
#define	ui_x1		ui_i.ih_x1
#define	ui_pr		ui_i.ih_pr
#define	ui_len		ui_i.ih_len
#define	ui_src		ui_i.ih_src.s_addr
#define	ui_dst		ui_i.ih_dst.s_addr
#define	ui_sport	ui_u.uh_sport
#define	ui_dport	ui_u.uh_dport
#define	ui_ulen		ui_u.uh_ulen
#define	ui_sum		ui_u.uh_sum

/*
 * udp per channel structure for the upper queue
 */
struct udpd {
	queue_t *ud_rdq;		/* read queue */
	int	ud_dev;			/* minor dev # */
	struct	tsap ud_la;		/* local address */
	struct	tsap ud_fa;		/* Foreign Address	*/
	struct	udpd *ud_next;		/* The next active block down */
	mblk_t	*ud_ipopt;		/* outgoing ip options */
	mblk_t	*ud_ipoptin;		/* incoming ip options */
#define	ud_src	ud_la.ta_addr
#define	ud_laddr	ud_la.ta_addr
#define	ud_sport ud_la.ta_port
#define	ud_fport ud_fa.ta_port
#define	ud_faddr ud_fa.ta_addr
	ulong	ud_lipaddr;		/* local ip addr for chksum */
	short	ud_topid;		/* who is on top */
	unchar	ud_prot;		/* protocol # on top for demux */
	unchar	ud_tstate;		/* TLI state */
	ushort	ud_flags;		/* Flags			*/
	unchar	ud_tos;			/* Type of Service */
	unchar	ud_ttl;			/* Time to Live */
	long	ud_iocid;		/* ioctl id */
	mblk_t  *ud_iocblk;		/* waiting ioctl block */
};

/*
 * ud_flags - ( 1<<0 thru 1<<8 are socket options - see socket.h)
 */
#define UD_CONNECT	(1<<12)
#define UD_CKSUM_IN	(1<<13)
#define UD_CKSUM_OUT	(1<<14)
#define UD_SUPERUSER	(1<<15)

/*
 * udp per channel structure for the bottom queue (mux)
 */
struct udpb {
	queue_t	*ub_wrq;		/* write queue */
	int	ub_cookie;		/* cookie from LINK/UNLINK */
	char	ub_mstate;		/* mux state */
	short	ub_botid;		/* who is down below */
	mblk_t	*ub_iocblk;		/* temp. holding the M_IOCTL */
	queue_t *ub_qtop;		/* control chan write q */
};

/*
 * ioctl format to the udp module
 * should be accessible to user
 */
struct udpioctl {
	int cookie;			/* from mux */
};

/*
 * from a pointer to udpd, find the dev #
 * could have been UDPCHAN(tp) ((int)tp - (int)udpd) / sizeof(struct udpd))
 */
#define UDPCHAN(tp)	tp->ud_dev
#define UDPCTLCHAN	0		/* udp control channel	*/
#define UDP_ADDRLEN	16		/* udp Address size (BINDLEN+8) */
#define UDP_BINDADDRLEN	UDP_ADDRLEN	/* udp Address size	*/
#define UDPPORT_RESERVED	1024
#define	UDP_TTL		30
#define	UDP_TOS		0		/* needs correcting */

/* udp_input error code */
#define	UDPE_HDR	0x01	/* udp header error	*/
#define	UDPE_SUM	0x02	/* checksum		*/
#define	UDPE_OFFSET	0x03	/* bad offset		*/
#define	UDPE_OPTLEN	0x04	/* option lenght	*/
#define	UDPE_ALLOCB	0x05	/* allocb failed	*/
#define	UDPE_LOOKUP	0x06	/* can't match		*/
#define	UDPE_IDLE	0x07	/* not in TS_IDLE	*/
#define	UDPE_FULL	0x08	/* queue is full	*/

struct	udpstat {
	int	udps_hdrops;
	int	udps_badsum;
#ifdef SHORT_IDENT
	int	Udps_badlen;
#define udps_badlen Udps_badlen	/* Compiler bug */
#else
	int	udps_badlen;
#endif /* SHORT_IDENT */
	int	udps_qfull;
	int	udps_busy;
/*
** Compliance with RFC 1066
*/
	int	udpInDatagrams;
	int	udpNoPorts;
	int	udpInErrors;
	int	udpOutDatagrams;
};
#ifdef INKERNEL
extern struct udpstat udpstat;
#endif /* INKERNEL */

#endif /* UDP_H */
