#ifndef TCP_H
#define TCP_H

/*
 * @(#) Copyright 1986.  The Wollongong Group, Inc.  All Rights Reserved.
 */
/* log history */
/*
 *	TWU	05/20/91	To support TCP no delay options
 */

#ident "@(#)tcp.h  (TWG)      1.7     89/08/17 "

/*
 * Definitions of the TCP timers.  These timers are counted
 * down PR_SLOWHZ times a second.
 */
#define	TCPT_NTIMERS	4

#define	TCPT_REXMT	0		/* retransmit */
#define	TCPT_PERSIST	1		/* retransmit persistance */
#define	TCPT_KEEP	2		/* keep alive */
#define	TCPT_2MSL	3		/* 2*msl quiet time timer */

#define TCP_ALPHA	7
#define TCP_BETA	16

/*
 * The TCPT_REXMT timer is used to force retransmissions.
 * The TCP has the TCPT_REXMT timer set whenever segments
 * have been sent for which ACKs are expected but not yet
 * received.  If an ACK is received which advances tp->snd_una,
 * then the retransmit timer is cleared (if there are no more
 * outstanding segments) or reset to the base value (if there
 * are more ACKs expected).  Whenever the retransmit timer goes off,
 * we retransmit all unacknowledged segments, and do an exponential
 * backoff on the retransmit timer.
 *
 * The TCPT_PERSIST timer is used to keep window size information
 * flowing even if the window goes shut.  If all previous transmissions
 * have been acknowledged (so that there are no retransmissions in progress),
 * and the window is shut, then we start the TCPT_PERSIST timer, and at
 * intervals send a single byte into the peers window to force him to update
 * our window information.  We do this at most as often as TCPT_PERSMIN
 * time intervals, but no more frequently than the current estimate of
 * round-trip packet time.  The TCPT_PERSIST timer is cleared whenever
 * we receive a window update from the peer.
 *
 * The TCPT_KEEP timer is used to keep connections alive.  If an
 * connection is idle (no segments received) for TCPTV_KEEP amount of time,
 * but not yet established, then we drop the connection.  If the connection
 * is established, then we force the peer to send us a segment by sending:
 *	<SEQ=SND.UNA-1><ACK=RCV.NXT><CTL=ACK>
 * This segment is (deliberately) outside the window, and should elicit
 * an ack segment in response from the peer.  If, despite the TCPT_KEEP
 * initiated segments we cannot elicit a response from a peer in TCPT_MAXIDLE
 * amount of time, then we drop the connection.
 */

/*
 * Time constants.
 */
#define PR_SLOWHZ	2		/* ticks/second */
#define PR_FASTHZ	(2*PR_SLOWHZ)	/* Actual ticks/second of tcp_timeout
					 * routine and should divide HZ evenly
				         */
#define	TCPTV_MSL	(30*PR_SLOWHZ)	/* max seg lifetime */
#define TCPTV_SRTTBASE	0		/* base roundtrip time;
					 * if 0, no idea yet */
#define TCPTV_SRTTDFLT  (3 * PR_SLOWHZ)	/* assumed RTT if no info */

#define	TCPTV_PERSMIN	(  5*PR_SLOWHZ)		/* retransmit persistance */
#define	TCPTV_PERSMAX	( 60*PR_SLOWHZ)		/* maximum persist interval */

#define	TCPTV_KEEP_INIT	( 75*PR_SLOWHZ)		/* initial connect keep alive */
#define	TCPTV_KEEP_IDLE	(120*60*PR_SLOWHZ)	/* dflt time before probing */
#define	TCPTV_KEEPINTVL	( 75*PR_SLOWHZ)		/* default probe interval */
#define	TCPTV_KEEPCNT	8			/* max probes before drop */

#define	TCPTV_MAXIDLE	(8*TCPTV_KEEPINTVL)	/* maximum allowable idle
						   time before drop conn */
#define	TCP_LINGERTIME	120			/* linger at most 2 minutes */

#define	TCP_MAXRXTSHIFT	12			/* maximum retransmits */

#define	TCPTV_MAXKEEP	10			/* max times of missing keep-
						   alive before drop conn */

#define	TCPTV_MIN	(  1*PR_SLOWHZ)		/* minimum allowable value */
#define	TCPTV_REXMTMAX	( 64*PR_SLOWHZ)		/* maximum allowable value */

/*
 * Force a time value to be in a certain range.
 */
#define	TCPT_RANGESET(tv, value, tvmin, tvmax) { \
	(tv) = (value); \
	if ((tv) < (tvmin)) \
		(tv) = (tvmin); \
	if ((tv) > (tvmax)) \
		(tv) = (tvmax); \
}

extern int tcp_keepidle;		/* time before keepalive probes begin */
extern int tcp_keepintvl;		/* time between keepalive probes */
extern int tcp_maxidle;			/* time to drop after starting probes */
extern unchar tcp_ttl;			/* time to live for TCP segs */
extern unchar tcp_tos;			/* type-of-service */
extern int tcp_backoff[];

typedef	unsigned long	tcp_seq;

/*
 * TCP header.
 * Per RFC 793, September, 1981.
 */

#ifdef XENIX
/* #pragma pack(1) */
#endif

struct tcphdr {
	ushort	th_sport;		/* source port */
	ushort	th_dport;		/* destination port */
	tcp_seq	th_seq;			/* sequence number */
	tcp_seq	th_ack;			/* acknowledgement number */
#if ( vax || ns32000 || i386)
	unchar	th_x2:4,		/* (unused) */
		th_off:4;		/* data offset */
#else
	unchar	th_off:4,		/* Data Offset	*/
		th_x2:4;		/* Unused	*/
#endif /* vax */
	unchar	th_flags;
#define	TH_FIN	0x01
#define	TH_SYN	0x02
#define	TH_RST	0x04
#define	TH_PUSH	0x08
#define	TH_ACK	0x10
#define	TH_URG	0x20
	ushort	th_win;			/* window */
	ushort	th_sum;			/* checksum */
	ushort	th_urp;			/* urgent pointer */
};

#define	TCPHLEN		sizeof(struct tcphdr)

#define	TCPOPT_EOL	0
#define	TCPOPT_NOP	1
#define	TCPOPT_MAXSEG	2

/*
 * Tcp+ip header, after ip options removed.
 */
struct tcpiphdr {
	struct 	ipovly ti_i;		/* overlaid ip structure */
	struct	tcphdr ti_t;		/* tcp header */
};

#ifdef XENIX
/* #pragma pack() */
#endif

#define	ti_next		ti_i.ih_next
#define	ti_prev		ti_i.ih_prev
#define	ti_x1		ti_i.ih_x1
#define	ti_pr		ti_i.ih_pr
#define	ti_len		ti_i.ih_len
#define	ti_src		ti_i.ih_src.s_addr
#define	ti_dst		ti_i.ih_dst.s_addr
#define	ti_sport	ti_t.th_sport
#define	ti_dport	ti_t.th_dport
#define	ti_seq		ti_t.th_seq
#define	ti_ack		ti_t.th_ack
#define	ti_x2		ti_t.th_x2
#define	ti_off		ti_t.th_off
#define	ti_flags	ti_t.th_flags
#define	ti_win		ti_t.th_win
#define	ti_sum		ti_t.th_sum
#define	ti_urp		ti_t.th_urp

#define	TCPIPHLEN	sizeof(struct tcpiphdr)

/* from tcp_seq.h */

/*
 * TCP sequence numbers are 32 bit integers operated
 * on with modular arithmetic.  These macros can be
 * used to compare such integers.
 */
#define	SEQ_LT(a,b)	((int)((a)-(b)) < 0)
#define	SEQ_LEQ(a,b)	((int)((a)-(b)) <= 0)
#define	SEQ_GT(a,b)	((int)((a)-(b)) > 0)
#define	SEQ_GEQ(a,b)	((int)((a)-(b)) >= 0)

/*
 * Macros to initialize tcp sequence numbers for
 * send and receive from initial send and receive
 * sequence numbers.
 */
#define	tcp_rcvseqinit(tp) \
	(tp)->rcv_adv = (tp)->rcv_nxt = (tp)->irs + 1

#define	tcp_sendseqinit(tp) \
	(tp)->snd_una = (tp)->snd_nxt = (tp)->snd_max = (tp)->snd_up = \
	    (tp)->iss

#define	TCP_ISSINCR	(125*1024)	/* increment for tcp_iss each second */

extern tcp_seq	tcp_iss;		/* tcp initial send seq # */

/*
 * tcp per channel structure for the upper queue
 */
struct tcpd {
	queue_t *td_rdq;		/* read queue */
	mblk_t	*td_tcpcb;		/* to protocol cntl block */
	struct	tcpd	*td_next;	/* The next device in active list */
	int	td_wcount;		/* # of bytes waiting to go down */
	int	td_rcount;		/* # of bytes waiting to go up */
	int	td_iocid;		/* Ioctl id's passed down.	*/
	mblk_t	*td_ipopt;		/* outgoing ip options */
	mblk_t	*td_ipoptin;		/* incoming ip options */
	short	td_topid;		/* who is on top */
	ushort 	td_sockflags;		/* socket option flags */
	unchar	td_flags;		/* see below */
	unchar	td_dev;			/* minor dev # */
	unchar	td_prot;		/* protocol # on top for demux */
	unchar	td_tstate;		/* TLI state */
	unchar	td_tos;			/* Type of service */
	unchar	td_ttl;			/* Time to live */
};

/*
 * td_sockflags are the socket option flags (1<<0 thru 1<<8)
 * as defined in socket.h
 *
 * td_flags are as follows:
 */
#define	TD_ERROR	(1<<0)		/* fatal error occurred		*/
#define TD_SUPERUSER	(1<<1)		/* Super User opened it		*/
#define TD_RDWR		(1<<2)		/* Don't send M_PROTO for data	*/
#define TD_NOLINGER	(1<<3)		/* Connections linger		*/
#define TD_LINGER	~TD_NOLINGER
#define TD_CLOSING      (1<<4)          /* The user has issued a close  */
#define TD_NEWPORT      (1<<5)          /* This means he needs  new port*/


/*
 * tcp per channel structure for the bottom queue (mux)
 */
struct tcpb {
	queue_t	*tb_wrq;		/* write queue */
	int	tb_cookie;		/* cookie from LINK/UNLINK */
	char	tb_mstate;		/* mux state */
	short	tb_botid;		/* who is down below */
	mblk_t	*tb_iocblk;		/* temp. holding the M_IOCTL */
	queue_t *tb_qtop;		/* control chan write q */
};

#ifdef notdef
/*
 * ioctl format to the tcp module
 * should be accessible to user
 */
struct tcpioctl {
	int cookie;			/* from mux */
};
#endif /* notdef */

/*
 * from a pointer to tcpd, find the dev #
 * could have been TCPCHAN(tp) ((int)tp - (int)tcpd) / sizeof(struct tcpd))
 */
#define TCPCHAN(tdp)	((tdp) ? (tdp)->td_dev : 0)
#define TCPCTLCHAN	0	/* tcp control channel */

/*
 * they should be somewhere else
 */
#define MUXSID	-2		/* sid for sadlog */
#define	UNREACH		255	/* unreachable td_tstate */
#define	SAMETLIST	254	/* same tli state */

extern struct qhead tcpcb;
extern struct tcpd tcpd[];
extern struct tcpb tcpb;


/*
 * Tcp control block, one per active tcp (after binding)
 * pointer back to tcpd (tcpd is alloc when the stream is opened)
 */
struct tcpcb {
	struct	tcpd *t_tdp;		/* back pointer to tcpd */
	struct	tsap t_la;		/* local address */
#define	t_laddr	t_la.ta_addr
#define	t_lport	t_la.ta_port
	struct	tsap t_fa;		/* foreign address */
#define	t_faddr	t_fa.ta_addr
#define	t_fport	t_fa.ta_port
	struct	qhead	t_conq;		/* conn queue */
	long	t_cqlen;		/* # of indications waiting in conq */
	long	t_maxcon;		/* max # of indications allowed*/
	long	t_conid;		/* conn id when floating */
	mblk_t	*t_template;		/* skeletal packet for transmit */
	struct	ipb	*t_ipb;		/* Pointer that is saved not used */
	struct	qhead	t_rsq;		/* reassembly queue */
#define	t_nseg	t_rsq.b_next
#define	t_pseg	t_rsq.b_prev
	short	t_state;		/* state of this connection */
	ushort	t_flags;
#define	TF_ACKNOW	0x01		/* ack peer immediately */
#define	TF_DELACK	0x02		/* ack, but try to delay it */
#define	TF_NODELAY	0x04		/* don't delay packets to coalesce */
#define	TF_NOOPT	0x08		/* don't use tcp options */
#define	TF_SENTFIN	0x10		/* have sent FIN */
	short	t_timer[TCPT_NTIMERS];	/* tcp timers */
	mblk_t	*t_tcpopt;		/* tcp options */
	ushort	t_maxseg;		/* maximum segment size */
	short	t_rxtshift;		/* log(2) of rexmt exp. backoff */
	short	t_rxtcur;		/* current retransmit value */
	short	t_dupacks;		/* consecutive dup acks recd */
	char	t_force;		/* 1 if forcing out a byte */
/*
 * The following fields are used as in the protocol specification.
 * See RFC783, Dec. 1981, page 21.
 */
/* send sequence variables */
	tcp_seq	snd_una;		/* send unacknowledged */
	tcp_seq	snd_nxt;		/* send next */
	tcp_seq	snd_up;			/* send urgent pointer */
	tcp_seq	snd_wl1;		/* window update seg seq number */
	tcp_seq	snd_wl2;		/* window update seg ack number */
	tcp_seq	iss;			/* initial send sequence number */
	ushort	snd_wnd;		/* send window */
/* receive sequence variables */
	ushort	rcv_wnd;		/* receive window */
	tcp_seq	rcv_nxt;		/* receive next */
	tcp_seq	rcv_up;			/* receive urgent pointer */
	tcp_seq	irs;			/* initial rcv seq number */
	tcp_seq	rcv_adv;		/* Advertised window		*/
/* retransmit variables */
	tcp_seq	snd_max;		/* highest sequence number sent */
/* congestion control (for slow start, source quench, retransmit after loss) */
	ushort	snd_cwnd;		/* congestion-controlled window */
	ushort	snd_ssthresh;		/* snd_cwnd size threshhold for
					 * for slow start exponential to
					 * linear switch */
	ushort	t_rxt;			/* retransmit times 		*/
	ushort	t_rtt;			/* round trip timer		*/
	tcp_seq t_rtseq;		/* Sequence number being timed  */
	short	t_srtt;			/* Smoothed ROund trip timer	*/
	short	t_rttvar;		/* variance in round-trip time */
	ushort	t_keep;			/* # of unack keep-alive mesg	*/
/* transmit timing stuff */
	ushort	t_idle;			/* inactivity time */
	ushort  t_srtv;			/* Smoothed Round Trip Variance */
	ushort	max_rcvd;		/* most peer has sent into window */
	ushort	max_sndwnd;		/* largest window peer has offered */

};

/*
 *  struct used by netstat to fetch tcp data
 */
struct tcpglob {
	mblk_t		tcb;		/* tcb entry */
	struct tcpcb	tcpcb;		/* associated tcpcb entry */
	struct tcpd	tcpd;		/* associated tcpd  entry */
};

#define	TILOG(x)	{errcode = x;}
/*
 * tcp error code in tcp_input.c
 */
#define	TCPE_HDR	1	/* tcp header error */
#define	TCPE_SUM	2	/* checksum */
#define	TCPE_OFFSET	3	/* bad offset */
#define	TCPE_OPTLEN	4	/* tcp option length */
#define	TCPE_ALLOCB	5	/* allocb fails */
#define	TCPE_LOOKUP	6	/* can't match a tcpcb */
#define	TCPE_BASE	7	/* db_base != b_rptr */
#define	TCPE_SSACK	10	/* SYN_SENT, got bad ack */
#define	TCPE_SSRST	11	/* SYN_SENT, got rst */
#define	TCPE_SSSYN	12	/* SYN_SENT, no syn */
#define	TCPE_SSADJ	13	/* SYN_SENT, adjmsg */
#define	TCPE_RWIN0	21	/* rcv_wnd=0, rcv_nxt != ti_seq */
#define	TCPE_OUTSEQ	22	/* rcv seg not in seq */
#define	TCPE_RSTRST	25	/* rcv rst, send rst */
#define	TCPE_RSTDROP	26	/* rcv rst, don't send rst, just drop */
#define	TCPE_BADSYN	27	/* rcv unexpected syn, close user, rst peer */
#define	TCPE_NOACK	31	/* TH_ACK is not set, drop seg */
#define	TCPE_SRBADACK	32	/* SYN_RCVD, bad ack */
#define	TCPE_BADACK	33	/* other than SYN_RCVD, bad ack */
#define	TCPE_LASTACK	35	/* get FIN|ACK in LAST_ACK */
#define	TCPE_CLOSING	36	/* get FIN|ACK in CLOSING */
#define	TCPE_TWAIT	37	/* get FIN|ACK in TIME_WAIT */
#define	TCPE_NQRST	41	/* new-req: has rst */
#define	TCPE_NQACK	42	/* new-req: has ack */
#define	TCPE_NQSYN	43	/* new-req: has no syn */
#define	TCPE_PASSFIN	44	/* want to send non-existing stuff */

/*
 * tcp macros
 * t_wcc (write count) is exact
 */
#define t_rhiw(tdp)	((tdp) ? min ((tdp)->td_rdq->q_hiwat, tcp_maxrcvwin) : 0)
#define t_rwin(tdp)	((tdp) ? ((tdp)->td_rdq ? (t_rhiw(tdp) - (tdp)->td_rcount) : 0) : 0)
#define t_wcc(tdp)	((tdp) ? tdp->td_wcount : 0)

/*
 * TCP FSM state definitions.
 * Per RFC793, September, 1981.
 */

#define	TCP_NSTATES	11

#define	TV_CLOSED		0	/* closed */
#define	TV_LISTEN		1	/* listening for connection */
#define	TV_SYN_SENT		2	/* active, have sent syn */
#define	TV_SYN_RCVD		3	/* have send and received syn */
/* states < TV_ESTABLISHED are those where connections not established */
#define	TV_ESTABLISHED		4	/* established */
#define	TV_CLOSE_WAIT		5	/* rcvd fin, waiting for close */
/* states > TV_CLOSE_WAIT are those where user has closed */
#define	TV_FIN_W1		6	/* have closed, sent fin */
#define	TV_CLOSING		7	/* closed xchd FIN; await FIN ACK */
#define	TV_LAST_ACK		8	/* had fin and close; await FIN ACK */
/* states > TV_CLOSE_WAIT && < TV_FIN_W2 await ACK of FIN */
#define	TV_FIN_W2		9	/* have closed, fin is acked */
#define	TV_TIME_WAIT		10	/* in 2*msl quiet wait after close */

#define	TV_HAVERCVDSYN(s)	((s) >= TV_SYN_RCVD)
#define	TV_HAVERCVDFIN(s)	((s) >= TV_TIME_WAIT)
#define	TV_CANTRCVMORE(s)	((s)==TV_CLOSE_WAIT || (s)==TV_LAST_ACK || \
				 (s)==TV_CLOSING || (s)==TV_TIME_WAIT)

extern mblk_t *tv_popen(), *bp_dupall();
extern struct tcpcb *tn_connreq();

extern int tcp_run_serv, tcp_need_timeout;
#define TCP_LOCK_TIMEOUT() {int s; s = splstr(); tcp_run_serv = 1; splx(s);}
#define TCP_RUN_TIMEOUT() {int s = splstr(); \
		tcp_run_serv = 0; \
		if (tcp_need_timeout) { \
			(void) splx (s); tcptimeout(); \
		}else (void) splx (s); }
#define	WQREPLY(q, bp)	{q = RD(q); putnext(q, bp);}

#define	TCP_ADDRLEN		16 /* Long + short sizeof does not work */
#ifndef TCP_BINDADDRLEN
#define	TCP_BINDADDRLEN		TCP_ADDRLEN
#endif /* TCP_BINDADDRLEN */
#define	TCP_BINDACKLEN		sizeof(struct T_bind_ack) + TCP_ADDRLEN
#define	TCP_MAXCONIND		4	/* # of outstanding indications */

#define	TCP_CONNCONRESOFFSET	sizeof(struct T_conn_con)
#define	TCP_CONNCONRESLEN	TCP_ADDRLEN
#define	TCP_CONNCONLEN		TCP_CONNCONRESOFFSET + TCP_CONNCONRESLEN
#define	TCP_CONNINDSRCOFFSET	sizeof(struct T_conn_ind)
#define	TCP_CONNINDSRCLEN	TCP_ADDRLEN
#define	TCP_CONNINDLEN		TCP_CONNINDSRCOFFSET + TCP_CONNINDSRCLEN

#define TCPPORT_RESERVED	1024	/* Non root UID */

struct	tcpstat {
	int	tcps_rcvbadsum;
	int	tcps_rcvbadoff;
	int	tcps_rcvshort;
	int	tcps_badsegs;
	int	tcps_unack;
#define	tcps_badsum	tcps_rcvbadsum
#define	tcps_badoff	tcps_rcvbadoff
#define	tcps_hdrops	tcps_rcvshort
	u_long	tcps_connattempt;	/* connections initiated */
	u_long	tcps_accepts;		/* connections accepted */
	u_long	tcps_connects;		/* connections established */
	u_long	tcps_drops;		/* connections dropped */
	u_long	tcps_conndrops;		/* embryonic connections dropped */
	u_long	tcps_closed;		/* conn. closed (includes drops) */
	u_long	tcps_segstimed;		/* segs where we tried to get rtt */
	u_long	tcps_rttupdated;	/* times we succeeded */
	u_long	tcps_delack;		/* delayed acks sent */
	u_long	tcps_timeoutdrop;	/* conn. dropped in rxmt timeout */
	u_long	tcps_rexmttimeo;	/* retransmit timeouts */
	u_long	tcps_persisttimeo;	/* persist timeouts */
	u_long	tcps_keeptimeo;		/* keepalive timeouts */
	u_long	tcps_keepprobe;		/* keepalive probes sent */
	u_long	tcps_keepdrops;		/* connections dropped in keepalive */

	u_long	tcps_sndtotal;		/* total packets sent */
	u_long	tcps_sndpack;		/* data packets sent */
	u_long	tcps_sndbyte;		/* data bytes sent */
	u_long	tcps_sndrexmitpack;	/* data packets retransmitted */
	u_long	tcps_sndrexmitbyte;	/* data bytes retransmitted */
	u_long	tcps_sndacks;		/* ack-only packets sent */
	u_long	tcps_sndprobe;		/* window probes sent */
	u_long	tcps_sndurg;		/* packets sent with URG only */
	u_long	tcps_sndwinup;		/* window update-only packets sent */
	u_long	tcps_sndctrl;		/* control (SYN|FIN|RST) packets sent */

	u_long	tcps_rcvtotal;		/* total packets received */
	u_long	tcps_rcvpack;		/* packets received in sequence */
	u_long	tcps_rcvbyte;		/* bytes received in sequence */
	u_long	tcps_rcvduppack;	/* duplicate-only packets received */
	u_long	tcps_rcvdupbyte;	/* duplicate-only bytes received */
	u_long	tcps_rcvpartduppack;	/* packets with some duplicate data */
	u_long	tcps_rcvpartdupbyte;	/* dup. bytes in part-dup. packets */
	u_long	tcps_rcvoopack;		/* out-of-order packets received */
	u_long	tcps_rcvoobyte;		/* out-of-order bytes received */
	u_long	tcps_rcvpackafterwin;	/* packets with data after window */
	u_long	tcps_rcvbyteafterwin;	/* bytes rcvd after window */
	u_long	tcps_rcvafterclose;	/* packets rcvd after "close" */
	u_long	tcps_rcvwinprobe;	/* rcvd window probe packets */
	u_long	tcps_rcvdupack;		/* rcvd duplicate acks */
	u_long	tcps_rcvacktoomuch;	/* rcvd acks for unsent data */
	u_long	tcps_rcvackpack;	/* rcvd ack packets */
	u_long	tcps_rcvackbyte;	/* bytes acked by rcvd acks */
	u_long	tcps_rcvwinupd;		/* rcvd window update packets */
/*
** Counters added for compliance with RFC 1066
*/
	int	tcpActiveOpens;
	int	tcpPassiveOpens;
	int	tcpAttemptFails;
	int	tcpEstabResets;
	int	tcpInSegs;
	int	tcpOutSegs;
	int	tcpRetransSegs;
};
#ifdef	TCPSTATES
char *tcpstates[] = {
	"CLOSED",	"LISTEN",	"SYN_SENT",	"SYN_RCVD",
	"ESTABLISHED",	"CLOSE_WAIT",	"FIN_WAIT_1",	"CLOSING",
	"LAST_ACK",	"FIN_WAIT_2",	"TIME_WAIT",
};
#endif
/* TWU 05/20/91 to support TCP no delay option */
#define	TCP_NODELAY	0x01	/* force TCP doing no delay on transmit data */
extern struct tcpstat tcpstat;

#endif /* TCP_H */
