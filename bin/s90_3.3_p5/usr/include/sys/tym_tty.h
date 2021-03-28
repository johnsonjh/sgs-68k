#ifndef	SYS_TYM_TTY_H
#define SYS_TYM_TTY_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tym_tty.h: version 24.1 created on 10/28/91 at 18:40:29	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tym_tty.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *	tym_tty.h - header file for the streams tty module on the IOPM board
 *
 *	gil hunt - Nov 14, 1988
 *	gil hunt - Nov 27, 1989 - added private stats
 */
/*--------------------------------------------------------------------*/
struct tym_rstat		/* private stats for read side */
{
	int	rawch;		/* input character count */
	int	canch;		/* input character count processed by canon */
};
typedef struct tym_rstat tym_rstat_t;

struct tym_wstat		/* private stats for write side */
{
	int	outch;		/* output char count, received from upstream */
	int	postch;		/* output char count, send downstream */
};
typedef struct tym_wstat tym_wstat_t;
/*--------------------------------------------------------------------*/
struct tym_tty
{
	unchar	myname[4];
	queue_t	*rq;		/* read  queue for this device */
	queue_t	*wq;		/* write queue for this device */
	mblk_t	*bp_canon;	/* addr of msg containing canon buf */
	mblk_t	*bp_tab;	/* addr of msg containing character table */
	mblk_t	*bp_tp;		/* addr of msg containing this tty structure */
	mblk_t	*bp_stat;	/* addr of msg containing stats structure */
	mblk_t	*echobuf;	/* addr of msg containing data to be echoed */
	mblk_t	*raw_message;	/* addr of msg containing non-ICANON data */ 
	mblk_t	*tcflsh_mp;	/* addr of TCFLSH ioctl to be acked */
	struct module_stat *rq_stat; /* addr of general read side stats */
	struct module_stat *wq_stat; /* addr of general write side stats */
	unchar	*canon_base;	/* start address of canon buffer */
	unchar *unused;
	unchar	*canptr;	/* addr to put next byte in canon buffer */
	unchar	*char_tab;	/* address of character table */
	int	m_read_request;	/* no. of bytes requested by M_READ msg */
	int	raw_id;		/* current id of RAW timer */
	ushort	raw_count;	/* no. bytes accumulated in raw mode */
	ushort	col;		/* current column position in opost mode */
	ushort	state;		/* current state */
	ushort	timer_enable;	/* incicates which timers are running */
	short	so_flags;	/* copy of M_SETOPT flags sent to stream head */
	short	so_readopt;	/* copy of M_SETOPT option sent to head */
	unchar	canon_xcase;	/* non-zero if both ICANON and XCASE set*/
	unchar	oflag_type;	/* non-zero if OPOST and at least one other
				   flag set in oflag */
	dev_t	t_device;	/* major/minor device number */
	dev_t	min_dev;	/* minor_device */
	struct	termio termio;	/* termio flags (see usr/include/sys/termio.h)*/
	tym_rstat_t pv_rstat;	/* private stats for read side */
	tym_wstat_t pv_wstat;	/* private stats for write side */
};
typedef struct tym_tty tym_tty_t;
/*--------------------------------------------------------------------*/
/*	state flags */
#define TTBLOCK		1		/* M_STOP msg send downstream */
#define LCLESC		2		/* last char received was '\' */
#define	M_READ_RECVD	4		/* M_READ msg received from upstream */ 
#define	TTSTOP		8		/* output stopped cause x-off received*/
#define	END_OF_LINE	0x10		/* end of line found in icanon mode */
/*--------------------------------------------------------------------*/
/*	timer_enable flags */
#define RAW_TIMER	2		/* RAW timer running */
/*--------------------------------------------------------------------*/
/*	types of characters that can be received from downstream
 *
 *	the following values reside within the input character table
 *	for each device, and are referenced by indexing into this table
 *	using the value of the received character as the index
 */
#define TYM_NEWLINE_1	1	/* new-line && (iflag & INLCR)	*/
#define TYM_NEWLINE_2	2	/* new-line && !(iflag & INLCR)	*/
#define TYM_RETURN_1	3	/* carriage return && (iflag & IGNCR) */
#define TYM_RETURN_2	4	/* carriage return && (iflag & ICRNL) */
#define TYM_A_Z		5	/* upper case && (iflag & IUCLC) */
#define TYM_a_z		6	/* lower case	ICANON & XCASE	*/
#define TYM_EOF		7	/* ^d (0x04)	(default)	*/
#define TYM_EOL		8
#define TYM_EOL2	9
#define TYM_INTR	10	/* 0x7f		lflag & ISIG	*/
#define TYM_QUIT	11	/* ^| (0x1c)	lflag & ISIG	*/
#ifdef POSIX
#define TYM_SUSP	12	/* ^z (0x1a)	lflag * ISIG for POSIX */
#else
#define TYM_SWTCH	12	/* ^z (0x1a)	lflag & ISIG	*/
#endif	/* POSIX */
#define TYM_ERASE	13	/* ^h (0x08)	(default)	*/
#define TYM_KILL	14	/* ^u (0x17)	(default)	*/
#define TYM_0x21	15	/* !  (0x21)	ICANON & XCASE	*/
#define TYM_0x27	16	/* '  (0x27)	ICANON & XCASE	*/
#define TYM_0x28	17	/* (  (0x28)	ICANON & XCASE	*/
#define TYM_0x29	18	/* )  (0x29)	ICANON & XCASE	*/
#define TYM_0x5c	19	/* \  (0x5c)	ICANON & XCASE	*/
#define TYM_0x5e	20	/* ^  (0x5e)	ICANON & XCASE	*/
#define TYM_CSTOP	21	/*    (0x11)	IXON   & X-OFF	*/
#define TYM_CSTART	22	/*    (0x13)	IXON   & X-ON	*/
/*--------------------------------------------------------------------*/
/*	types of characters that can be sent downstream
 *
 *	the following values reside within the output character table
 *	and are referenced by indexing into this table
 *	using the value of the received character as the index
 */
#define TYM_ONOP	1	/* Non-printing chars execpt 0x08 thru 0x0d */
#define TYM_Ox08	2	/* Backspace */
#define TYM_Ox09	3	/* tab */
#define TYM_Ox0a	4	/* line feed */
#define TYM_Ox0b	5	/* vertical tab */
#define TYM_Ox0c	6	/* form feed */
#define TYM_Ox0d	7	/* carriage return */
#define TYM_OA_Z	8	/* precede upper case letters with '\' */
#define TYM_Oa_z	9	/* Map lower to upper case if OLCUC set*/
#define TYM_Ox5c	10	/* map '\' to "\\" */
#define TYM_Ox60	11	/* map '`' to "\'" */
#define TYM_Ox7b	12	/* map '{' to "\(" */
#define TYM_Ox7c	13	/* map '|' to "\!" */
#define TYM_Ox7d	14	/* map '}' to "\)" */
#define TYM_Ox7e	15	/* map '~' to "\^" */
/*--------------------------------------------------------------------*/
#endif	/* SYS_TYM_TTY_H */
