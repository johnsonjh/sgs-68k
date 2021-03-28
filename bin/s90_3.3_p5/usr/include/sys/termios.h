#ifndef SYS_TERMIOS_H
#define SYS_TERMIOS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) termios.h: version 24.1 created on 10/28/91 at 18:39:27	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)termios.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/termios.h	10.3"		*/

#ifndef NCCS
#define NCCS    10
#endif

/* control characters */
#define	VINTR	0
#define	VQUIT	1
#define	VERASE	2
#define	VKILL	3
#define	VEOF	4
#define	VEOL	5
#define	VEOL2	6
#define	VMIN	4
#define	VTIME	5
#define	VSWTCH	7
#define VSUSP   7
#define VSTART  8
#define VSTOP   9
#define VDISABLE 037

#define	CNUL	0
#define	CDEL	0377
/* default control chars */
#define	CESC	'\\'
#define	CINTR	0177	/* DEL */
#define	CQUIT	034	/* FS, cntl | */
#define	CERASE	'#'
#define	CKILL	'@'
#define	CEOF	04	/* cntl d */
#define	CSTART	021	/* cntl q */
#define	CSTOP	023	/* cntl s */
#define	CSWTCH	032	/* cntl z */
#define CSUSP   032     /* cntl z */
#define	CNSWTCH	0


/* input modes */
#define	IGNBRK	0000001		/*Ignore break condition*/
#define	BRKINT	0000002		/*Signal interrupt on break*/
#define	IGNPAR	0000004		/*Ignore characters with parity errors*/
#define	PARMRK	0000010		/*Mark parity errors*/
#define	INPCK	0000020		/*Enable input parity check*/
#define	ISTRIP	0000040		/*Strip character*/
#define	INLCR	0000100		/*Map NL to CR on input*/
#define	IGNCR	0000200		/*Ignore CR*/
#define	ICRNL	0000400		/*Map CR to NL on input*/
#define	IUCLC	0001000
#define	IXON	0002000		/*Enable start/stop output control*/
#define	IXANY	0004000
#define	IXOFF	0010000		/*Enable start/stop input control*/
#define	ISWITCH	0020000

/* output modes */
#define	OPOST	0000001
#define	OLCUC	0000002
#define	ONLCR	0000004
#define	OCRNL	0000010
#define	ONOCR	0000020
#define	ONLRET	0000040
#define	OFILL	0000100
#define	OFDEL	0000200
#define	NLDLY	0000400
#define	NL0	0
#define	NL1	0000400
#define	CRDLY	0003000
#define	CR0	0
#define	CR1	0001000
#define	CR2	0002000
#define	CR3	0003000
#define	TABDLY	0014000
#define	TAB0	0
#define	TAB1	0004000
#define	TAB2	0010000
#define	TAB3	0014000
#define	BSDLY	0020000
#define	BS0	0
#define	BS1	0020000
#define	VTDLY	0040000
#define	VT0	0
#define	VT1	0040000
#define	FFDLY	0100000
#define	FF0	0
#define	FF1	0100000

/* control modes */
#define	CBAUD	0000017
#define	B0	0
#define	B50	0000001
#define	B75	0000002
#define	B110	0000003
#define	B134	0000004
#define	B150	0000005
#define	B200	0000006
#define	B300	0000007
#define	B600	0000010
#define	B1200	0000011
#define	B1800	0000012
#define	B2400	0000013
#define	B4800	0000014
#define	B9600	0000015
#define	B19200	0000016
#define EXTA	0000016
#define	B38400	0000017
#define EXTB	0000017
#define	CSIZE	0000060
#define	CS5	0
#define	CS6	0000020
#define	CS7	0000040
#define	CS8	0000060
#define	CSTOPB	0000100
#define	CREAD	0000200
#define	PARENB	0000400
#define	PARODD	0001000
#define	HUPCL	0002000
#define	CLOCAL	0004000
#define RCV1EN	0010000
#define	XMT1EN	0020000
#define	LOBLK	0040000

/* line discipline 0 modes */
#define	ISIG	0000001
#define	ICANON	0000002
#define	XCASE	0000004
#define	ECHO	0000010
#define	ECHOE	0000020
#define	ECHOK	0000040
#define	ECHONL	0000100
#define	NOFLSH	0000200
#define	IHON	0000400		/* use CTS to flow control output */
#define	IHOFF	0001000		/* use RTS to flow control input */
#define TOSTOP  0002000
#define IEXTEN  0004000

#define	SSPEED	7	/* default speed: 300 baud */

#ifdef _POSIX_SOURCE
typedef unsigned int speed_t;
typedef unsigned int tcflag_t;
typedef unsigned char  cc_t;

/*
 * Ioctl control packet
 */
struct termios {
	tcflag_t	c_iflag;	/* input modes */
	tcflag_t	c_oflag;	/* output modes */
	tcflag_t	c_cflag;	/* control modes */
	tcflag_t	c_lflag;	/* line discipline modes */
	char		c_line;		/* line discipline */
	cc_t		c_cc[NCCS];	/* control chars */
};
#endif
#define	IOCTYPE	0xff00

#define	TIOC	('T'<<8)
#define	TCGETA	(TIOC|1)
#define	TCSETA	(TIOC|2)
#define	TCSETAW	(TIOC|3)
#define	TCSETAF	(TIOC|4)
#define	TCSBRK	(TIOC|5)
#define	TCXONC	(TIOC|6)
#define	TCFLSH	(TIOC|7)
#define TCSETPGRP (TIOC|8)
#define TCGETPGRP (TIOC|9)
#ifdef SAK
#define TCSAK_SET (TIOC|10)
#define TCSAK_GET (TIOC|11)
#endif /* SAK */
#define TCLDTERM (TIOC|12)
#define TCCON_ALERT (TIOC|13) /* ioctl toggle so ttydvr will send a hi-pri
                                * upstrem if a connect occurs (DSR and DCD
                                * asserted).
                                */

#define	TCDSET	(TIOC|32)

/* JTOF - added this window stuff for XWIN support */
#define	TIOCGWINSZ	(TIOC|104)
#define	TIOCSWINSZ	(TIOC|103)

#define	LDIOC	('D'<<8)
#define	LDOPEN	(LDIOC|0)
#define	LDCLOSE	(LDIOC|1)
#define	LDCHG	(LDIOC|2)
#define	LDGETT	(LDIOC|8)
#define	LDSETT	(LDIOC|9)

#define CIOC	('C'<<8)
#define	CDPENA	(CIOC|0)
#define	CDPDIS	(CIOC|1)

/*
 * Terminal types
 */
#define	TERM_NONE	0	/* tty */
#define	TERM_TEC	1	/* TEC Scope */
#define	TERM_V61	2	/* DEC VT61 */
#define	TERM_V10	3	/* DEC VT100 */
#define	TERM_TEX	4	/* Tektronix 4023 */
#define	TERM_D40	5	/* TTY Mod 40/1 */
#define	TERM_H45	6	/* Hewlitt-Packard 45 */
#define	TERM_D42	7	/* TTY Mod 40/2B */

/*
 * Terminal flags
 */
#define TM_NONE		0000	/* use default flags */
#define TM_SNL		0001	/* special newline flag */
#define TM_ANL		0002	/* auto newline on column 80 */
#define TM_LCF		0004	/* last col of last row special */
#define TM_CECHO	0010	/* echo terminal cursor control */
#define TM_CINVIS	0020	/* do not send esc seq to user */
#define TM_SET		0200	/* must be on to set/res flags */


#ifdef _POSIX_SOURCE
/* POSIX optional_actions flags.  Used in tcsetattr.  */
#define	TCSANOW		01000   /* change attribute NOW */
#define	TCSADRAIN	02000	/* drain o/p and set attr */
#define	TCSADFLUSH	04000	/* drain o/p, discard unread chars
				   and set attr */
#define TCSAFLUSH	04000	

/* Queue_selector flags used for tcflush */

#define TCIFLUSH	00000   /* flush data received but not read */
#define TCOFLUSH	00001   /* flush data written but not transmitted*/
#define TCIOFLUSH	00002   /* flush both data received but not read and
				data written but not transmitted */

/* Action flags used by tcflow */

#define TCOOFF		00000   /* suspend output */
#define TCOON		00001   /* restart suspended output */
#define TCIOFF		00002	/* transmit a stop character */
#define TCION		00003   /* restart suspended input */

#endif

#endif /* SYS_TERMIOS_H */
