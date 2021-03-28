#ifndef SYS_GCP_GCMUX_H
#define SYS_GCP_GCMUX_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gcmux.h: version 2.1 created on 5/22/89 at 19:06:29	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gcmux.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys/gcp:gcmux.h	2.1"

/*
 * GCP Mux Link
 */

struct gcmux {
	struct scc * 	m_scc;			/* scc base address	*/
	unsigned char	m_tstate;		/* transmitter state	*/
	unsigned char	m_ns;			/* next send		*/
	unsigned char	m_nr;			/* next receive		*/
	unsigned char	m_exst;			/* external status	*/
	char		m_cmd_rcredit;		/* command r credit	*/
	char		m_cmd_scredit;		/* command s credit	*/
	unsigned char	m_xmt_count;		/* xmt count		*/
	unsigned char	m_ackcount;		/* ack count		*/
	struct blu *	m_rblu;			/* receiver blu		*/
	struct blu *	m_rbuf;			/* receiver buffer	*/
	struct blu *	m_rblu_spare;		/* spare receiver blu	*/
	struct blu *	m_tblu;			/* transmit blu		*/
	struct blu *	m_tbuf;			/* transmit buffer	*/
	struct blu *	m_tbuf_tail;		/* transmit buffer	*/
	struct blu *	m_tblu_wak;		/* blu's waiting ack	*/
	struct pkt *	m_line_pkt;		/* line mgr input queue	*/
	struct blu *	m_agg_blu;		/* agg mgr blu queue	*/
	struct pkt *	m_agg_cmd_pkt;		/* agg mgr cmd queue	*/
	struct pkt *	m_agg_cmd_pkt_tail;	/* agg mgr cmd tail	*/
	struct pkt *	m_agg_pkt;		/* agg mgr pkt queue	*/
	struct pkt *	m_agg_pkt_tail;		/* agg mgr pkt queue	*/
	struct pkt *	m_mux_pkt;		/* mux mgr input queue	*/
	struct tcb *	m_mux_tcb;		/* mux mgr tcb		*/
	struct tcb *	m_agg_tcb;		/* agg mgr tcb		*/
	struct tcb *	m_agg_agg_tcb;		/* agg agg tcb		*/
	struct tcb *	m_line_tcb;		/* line mgr tcb		*/
	struct tcb *	m_line_xmt_tcb;		/* line xmt tcb		*/
	struct gcttm *	m_gcttm[32];		/* mux tty pointer	*/
	struct pto *	m_pto_timer;		/* protocol timeout q	*/
	struct pto	m_lmgr_downid;		/* line mgr down id	*/
	struct pto	m_lmgr_rstid;		/* line mgr reset id	*/
	struct pto	m_lmgr_ackid;		/* line mgr ack id	*/
	struct pto	m_lmgr_dlyid;		/* line mgr delay id	*/
	struct pto	m_lmgr_xmtid;		/* line mgr xmit id	*/
	struct pto	m_amgr_timer;		/* agg mgr timer	*/
	struct pto	m_mmgr_timer;		/* mux mgr timer	*/
	struct pto	m_smgr_timer;		/* sess mgr timer	*/
	unsigned char	m_mmgr_pto;		/* mux mgr pto ticks	*/
	unsigned char	m_mmgr_flag;		/* mux mgr flags	*/
	unsigned char	m_lmgr_flag;		/* line mgr flags	*/
	unsigned char	m_amgr_flag;		/* agg mgr flags	*/
	unsigned char	m_mmgr_state;		/* mux mgr state	*/
	unsigned char	m_mmgr_timerid;		/* mux mgr pto ticks	*/
	unsigned char	m_mmgr_carddown;	/* mux mgr card down	*/
	unsigned short	m_mmgr_waitcmd;		/* mux mgr pto ticks	*/
	unsigned short	m_rmt_random;		/* remotes random num	*/
	unsigned short	m_rmt_numvc;		/* remotes random num	*/
	unsigned short	m_lmgr_delay;		/* line mgr delay	*/
	short		m_tbuf_count;		/* no. xmit blu's	*/
	short		m_agg_pkt_count;	/* no. xmit pkt's	*/
	short		m_agg_cmd_pkt_count;	/* no. xmit cmd's	*/
	unsigned char	m_link_addr;		/* sdlc link addr	*/
	unsigned char	m_trace_lock;		/* trace lock		*/
	unsigned char	m_trace_level;		/* trace level		*/
	struct cir_buf 	*m_trace;		/* trace buffer		*/
	char		m_nochnnls;		/* no. of channels	*/
	char		m_maxcredit;		/* max credit		*/
	char		m_slavereqs;		/* slave reqs		*/
};

#endif /* SYS_GCP_GCMUX_H */
