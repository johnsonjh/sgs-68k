#ifdef  NO_SCCS_ID
#ident "@(#)tty_pty.h  (TWG)      1.3     89/09/06 "
#endif  /*NO_SCCS_ID*/

#ifdef	notdef
/*
 * These defines are already in <sys/inetioctl.h>.
 * 	pam daniel
 */
/*
 * Berkeley PTY ioctl defines
 */
#define	TIOCREMOTE	((('t')<<8)|105)	/* remote input editing */
#define	TIOCPKT		((('t')<<8)|112)	/* pty: set/clear packet mode */
#define		TIOCPKT_DATA		0x00	/* data packet */
#define		TIOCPKT_FLUSHREAD	0x01	/* flush packet */
#define		TIOCPKT_FLUSHWRITE	0x02	/* flush packet */
#define		TIOCPKT_STOP		0x04	/* stop output */
#define		TIOCPKT_START		0x08	/* start output */
#define		TIOCPKT_NOSTOP		0x10	/* no more ^S, ^Q */
#define		TIOCPKT_DOSTOP		0x20	/* now do ^S ^Q */
#endif	/* notdef */

#ifdef KERNEL
/*
 * @(#) Copyright 1986.  The Wollongong Group, Inc.  All Rights Reserved.
 */
struct pt_ioctl {
	short pt_flags;
	short pt_send;
	struct proc *pt_selr;
	struct proc *pt_selw;
};
/* Flags */
#define PF_PTSOPEN	1
#define PF_PTCOPEN	02
#define PF_PKT		04
#define PF_NBIO		010
#define PF_WTIMER	020
#define PF_REMOTE	040
#define PF_STOPPED	0100
#define PF_NOSTOP	0200
#define PF_WCOLL	0400
#define PF_RCOLL	01000
#endif

