#ifndef SYS_TTOLD_H
#define SYS_TTOLD_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ttold.h: version 2.1 created on 5/22/89 at 19:12:16	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ttold.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/ttold.h	10.2"		*/

#ident	"@(#)uts/head/sys:ttold.h	2.1"

struct	sgttyb {
	char	sg_ispeed;
	char	sg_ospeed;
	char	sg_erase;
	char	sg_kill;
	int	sg_flags;
};

/* modes */
#define	O_HUPCL	01
#define	O_XTABS	02
#define	O_LCASE	04
#define	O_ECHO	010
#define	O_CRMOD	020
#define	O_RAW	040
#define	O_ODDP	0100
#define	O_EVENP	0200
#define	O_NLDELAY	001400
#define	O_NL1	000400
#define	O_NL2	001000
#define	O_TBDELAY	002000
#define	O_NOAL	004000
#define	O_CRDELAY	030000
#define	O_CR1	010000
#define	O_CR2	020000
#define	O_VTDELAY	040000
#define	O_BSDELAY	0100000

#define	tIOC	('t'<<8)
#define	TIOCGETP	(tIOC|8)
#define	TIOCSETP	(tIOC|9)

#endif /* SYS_TTOLD_H */
