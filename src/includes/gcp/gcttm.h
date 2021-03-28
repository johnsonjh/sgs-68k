#ifndef SYS_GCP_GCTTM_H
#define SYS_GCP_GCTTM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gcttm.h: version 2.1 created on 5/22/89 at 19:06:38	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gcttm.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys/gcp:gcttm.h	2.1"

struct gcttm {
	unsigned char		t_eye_catcher[4];/* eye catcher		*/
	struct gcmux *		t_gcmux;	/* control tcb		*/
	struct tcb *		t_control_tcb;	/* control tcb		*/
	struct tcb *		t_input_tcb;	/* input tcb		*/
	struct tcb *		t_output_tcb;	/* output tcb		*/
	struct cirbuf		t_canq;		/* canonical queue	*/
	struct cir_buf		t_outq;		/* output queue		*/
	struct cirbuf		t_rbuf;		/* receiver buffer	*/
	struct cirbuf		t_tbuf;		/* transmitter buffer	*/
	struct cirbuf		t_rawq;		/* raw input queue	*/
	struct cirbuf		t_icmd;		/* input commands	*/
	struct sema		t_lock;		/* lock semaphore	*/
	unsigned char *		t_canwb;	/* canon work buffer	*/
	unsigned short		t_iflag;	/* input modes		*/
	unsigned short		t_oflag;	/* output modes		*/
	unsigned short		t_cflag;	/* control modes	*/
	unsigned short		t_lflag;	/* line modes		*/
	unsigned short		t_state;	/* internal state	*/
	unsigned short		t_lock_state;	/* locked state		*/
	unsigned short		t_gcp_state;	/* gcp state		*/
	short			t_pgrp;		/* process group name	*/
	unsigned char		t_cc[8];	/* settable ctrl chars	*/
	char			t_delct;	/* delimiter count	*/
	unsigned char		t_line;		/* line discipline	*/
	unsigned char		t_board;	/* board number		*/
	unsigned char		t_device;	/* device number	*/
	unsigned char		t_vc;		/* virtual circuit	*/
	unsigned char		t_open_flag;	/* open flag		*/
	unsigned char		t_close_flag;	/* close flag		*/
	struct termio		t_ioctl_set;	/* ioctl set		*/
	unsigned char		t_ioctl_cmd;	/* ioctl command	*/
	unsigned char		t_ioctl_arg;	/* ioctl argument	*/
	char			t_col;		/* current column	*/
	char			t_xmt_pkts;	/* xmt pkt count	*/
	char			t_max_credit;	/* maximum credit	*/
	char			t_send_credit;	/* credit remaining	*/
	char			t_rec_credit;	/* credit to be freed	*/
	char			t_rec_pkts;	/* pkts to be freed	*/
	unsigned char		t_vc_port;	/* mux port		*/
	unsigned char		t_vc_class;	/* mux class		*/
	unsigned char		t_vc_card;	/* mux card		*/
	struct gctio		t_gctio;	/* extened gcp tty ioctl*/
	struct pto		t_pto_dcd;	/* dsr to dcd delay	*/
	struct pto		t_pto_active;	/* inactive delay	*/
};

#define GCTTM_CFLAG_MASK (CBAUD|CSIZE|CSTOPB|PARENB|PARODD)

/*
 *	locked state variables
 */

#define GCTTM_LS_IRDY	0x0001		/* input ready			*/
#define GCTTM_LS_ORDY	0x0002		/* output ready			*/
#define GCTTM_LS_CRDY	0x0004		/* control ready		*/

#define GCTTM_LS_SISLP	0x0010		/* slave input sleeping		*/
#define GCTTM_LS_SOSLP	0x0020		/* slave output sleeping	*/

#define GCTTM_LS_MISLP	0x0100		/* master input sleeping	*/
#define GCTTM_LS_MOSLP	0x0200		/* master output sleeping	*/
#define GCTTM_LS_MCSLP	0x0400		/* master control sleeping	*/

/*
 *	gcp state variables
 */

#define GCTTM_STATE_RTO 	0x0001	/* raw io time out		*/
#define GCTTM_STATE_DTR 	0x0002	/* dtr asserted			*/
#define GCTTM_STATE_WVC 	0x0004	/* waiting for a vc		*/
#define GCTTM_STATE_ORG 	0x0008	/* origiante port		*/

/*
 *	slave to master commands
 */

#define GCTTM_STOM_STATE	(0x0100|GC_TTM_COMMANDS)
#define GCTTM_STOM_READ		(0x0200|GC_TTM_COMMANDS)
#define GCTTM_STOM_INTR		(0x0300|GC_TTM_COMMANDS)
#define GCTTM_STOM_QUIT		(0x0400|GC_TTM_COMMANDS)
#define GCTTM_STOM_SWITCH	(0x0500|GC_TTM_COMMANDS)
#define GCTTM_STOM_FLUSH	(0x0600|GC_TTM_COMMANDS)
#define GCTTM_STOM_IOCTL	(0x0700|GC_TTM_COMMANDS)
#define GCTTM_STOM_TXLOW	(0x0800|GC_TTM_COMMANDS)
#define GCTTM_STOM_HUP		(0x0900|GC_TTM_COMMANDS)
#define GCTTM_STOM_TRACE	(0x0a00|GC_TTM_COMMANDS)

/*
 *	master to slave commands
 */

#define GCTTM_MTOS_OPEN		(0x0100|GC_TTM_COMMANDS)
#define GCTTM_MTOS_CLOSE	(0x0200|GC_TTM_COMMANDS)
#define GCTTM_MTOS_TIMEO	(0x0300|GC_TTM_COMMANDS)
#define GCTTM_MTOS_READ		(0x0400|GC_TTM_COMMANDS)
#define GCTTM_MTOS_UNBLOCK	(0x0500|GC_TTM_COMMANDS)
#define GCTTM_MTOS_IOCTL	(0x0600|GC_TTM_COMMANDS)
#define GCTTM_MTOS_WRITE	(0x0700|GC_TTM_COMMANDS)
#define GCTTM_STOS_TIMEO	(0x0a00|GC_TTM_COMMANDS)

/*
 *	virtual registers
 */

#define GCTTM_VR_BASE		GCTTY_VR_END
#define GCTTM_VR_TTY_BASE	GCTTM_VR_BASE
/*****************************************************
#define GCTTM_VR_TTY_BASE	2

#define GCTTM_VR_MTOS		GCTTM_VR_BASE + 0
#define GCTTM_VR_STOM		GCTTM_VR_BASE + 1
******************************************************/

#define GCTTM_VR_TTY00		GCTTM_VR_TTY_BASE + 00
#define GCTTM_VR_TTY01		GCTTM_VR_TTY_BASE + 01
#define GCTTM_VR_TTY02		GCTTM_VR_TTY_BASE + 02
#define GCTTM_VR_TTY03		GCTTM_VR_TTY_BASE + 03
#define GCTTM_VR_TTY04		GCTTM_VR_TTY_BASE + 04
#define GCTTM_VR_TTY05		GCTTM_VR_TTY_BASE + 05
#define GCTTM_VR_TTY06		GCTTM_VR_TTY_BASE + 06
#define GCTTM_VR_TTY07		GCTTM_VR_TTY_BASE + 07
#define GCTTM_VR_TTY08		GCTTM_VR_TTY_BASE + 08
#define GCTTM_VR_TTY09		GCTTM_VR_TTY_BASE + 09
#define GCTTM_VR_TTY10		GCTTM_VR_TTY_BASE + 10
#define GCTTM_VR_TTY11		GCTTM_VR_TTY_BASE + 11
#define GCTTM_VR_TTY12		GCTTM_VR_TTY_BASE + 12
#define GCTTM_VR_TTY13		GCTTM_VR_TTY_BASE + 13
#define GCTTM_VR_TTY14		GCTTM_VR_TTY_BASE + 14
#define GCTTM_VR_TTY15		GCTTM_VR_TTY_BASE + 15
#define GCTTM_VR_TTY16		GCTTM_VR_TTY_BASE + 16
#define GCTTM_VR_TTY17		GCTTM_VR_TTY_BASE + 17
#define GCTTM_VR_TTY18		GCTTM_VR_TTY_BASE + 18
#define GCTTM_VR_TTY19		GCTTM_VR_TTY_BASE + 19
#define GCTTM_VR_TTY20		GCTTM_VR_TTY_BASE + 20
#define GCTTM_VR_TTY21		GCTTM_VR_TTY_BASE + 21
#define GCTTM_VR_TTY22		GCTTM_VR_TTY_BASE + 22
#define GCTTM_VR_TTY23		GCTTM_VR_TTY_BASE + 23
#define GCTTM_VR_TTY24		GCTTM_VR_TTY_BASE + 24
#define GCTTM_VR_TTY25		GCTTM_VR_TTY_BASE + 25
#define GCTTM_VR_TTY26		GCTTM_VR_TTY_BASE + 26
#define GCTTM_VR_TTY27		GCTTM_VR_TTY_BASE + 27
#define GCTTM_VR_TTY28		GCTTM_VR_TTY_BASE + 28
#define GCTTM_VR_TTY29		GCTTM_VR_TTY_BASE + 29
#define GCTTM_VR_TTY30		GCTTM_VR_TTY_BASE + 30
#define GCTTM_VR_TTY31		GCTTM_VR_TTY_BASE + 31

#define GCTTM_VR_TRACE0		GCTTM_VR_TTY31 + 1
#define GCTTM_VR_TRACE1		GCTTM_VR_TRACE0 + 1

#define GCTTM_VR_END		GCTTM_VR_TRACE1 + 1

#endif /* SYS_GCP_GCTTM_H */
