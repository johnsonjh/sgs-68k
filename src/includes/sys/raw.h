#ifndef RAW_H
#define RAW_H

/*
 * @(#) Copyright 1989.  The Wollongong Group, Inc.  All Rights Reserved.
 */

#ident "@(#)raw.h  (TWG)      1.3     89/08/03 "

/*
 * raw+ip header, after ip options removed
 */
struct	rawiphdr {
	struct 	ipovly ui_i;		/* overlaid ip structure */
};
#define	ui_next		ui_i.ih_next
#define	ui_prev		ui_i.ih_prev
#define	ui_x1		ui_i.ih_x1	/* maybe use this to hold address family
					 * before calling raw_input();  ip_input
					 * or icmp_input() will have to set it.
					 * See raw_input().
					 */
#define	ui_pr		ui_i.ih_pr
#define	ui_len		ui_i.ih_len
#define	ui_src		ui_i.ih_src.s_addr
#define	ui_dst		ui_i.ih_dst.s_addr

/*
 * raw per channel structure for the upper queue
 */
struct rawd {
	queue_t *rawd_rdq;		/* read queue */
	int	rawd_dev;		/* minor device number */
	struct	tsap rawd_la;		/* local address */
	struct	tsap rawd_fa;		/* foreign Address */
	struct	rawd *rawd_next;	/* the next active block down */
	mblk_t	*rawd_ipopt;		/* outgoing ip options */
	mblk_t	*rawd_ipoptin;		/* incoming ip options */
	struct	sockproto rawd_proto;	/* address family & protocol number */
#define	rawd_laddr	rawd_la.ta_addr
#define	rawd_sport	rawd_la.ta_port
#define	rawd_fport	rawd_fa.ta_port
#define	rawd_faddr	rawd_fa.ta_addr
	short	rawd_topid;		/* who is on top */
	unchar	rawd_prot;		/* protocol # on top for demux */
	unchar	rawd_tstate;		/* TLI state */
	ushort	rawd_flags;		/* flags */
	unchar	rawd_tos;		/* Type-of-Service */
	unchar	rawd_ttl;		/* Time-to-Live */
};

/*
 * rawd_flag - ( 1<<0 thru 1<<8 are socket options - see socket.h)
 */
#define RAW_LADDR	(1<<13)
#define RAW_FADDR	(1<<14)
#define RAW_SUPERUSER	(1<<15)

/*
 * raw per channel structure for the bottom queue (mux)
 */
struct rawb {
	queue_t	*rawb_wrq;		/* write queue */
	int	rawb_cookie;		/* cookie from LINK/UNLINK */
	char	rawb_mstate;		/* mux state */
	short	rawb_botid;		/* who is down below */
	mblk_t	*rawb_iocblk;		/* temp. holding the M_IOCTL */
	queue_t *rawb_qtop;		/* control chan write q */
};

/*
 * ioctl format to the raw module
 * should be accessible to user
 */
struct rawioctl {
	int cookie;			/* from mux */
};

/*
 * from a pointer to rawd, find the dev #
 * could have been RAWCHAN(tp) ((int)tp - (int)rawd) / sizeof(struct rawd))
 */
#define RAWCHAN(tp)	tp->rawd_dev
#define RAWCTLCHAN	0		/* raw control channel */
#define RAW_ADDRLEN	16		/* Address size */
#define RAW_BINDADDRLEN	RAW_ADDRLEN	/* raw Address size */
#define RAWPORT_RESERVED	1024
#define RAW_TTL		30
#define RAW_TOS		0		/* needs correcting */

/* raw_input error code */
#define	RAWE_HDR	0x01	/* raw header error	*/
#define	RAWE_IDLE	0x02	/* not in TS_IDLE	*/
#define	RAWE_ALLOCB1	0x03	/* allocb failed	*/
#define	RAWE_ALLOCB2	0x04	/* allocb failed	*/
#define	RAWE_DUPMSG	0x05	/* dupmsg failed	*/
#define	RAWE_FULL	0x06	/* queue is full	*/
#define	RAWE_LOOKUP	0x07	/* can't match		*/

struct	rawstat {
	int	raws_hdrops;
	int	raws_busy;
	int	raws_allocb1;
	int	raws_allocb2;
	int	raws_dupmsg;
	int	raws_qfull;
	int	raws_lookup;
};

#ifdef INKERNEL
extern struct rawstat rawstat;
#endif /* INKERNEL */

#endif /* RAW_H */
