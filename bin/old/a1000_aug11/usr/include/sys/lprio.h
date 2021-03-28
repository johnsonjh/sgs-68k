#ifndef SYS_LPRIO_H
#define SYS_LPRIO_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) lprio.h: version 2.1 created on 5/22/89 at 19:08:24	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)lprio.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* @(#)lp/head/sys:lprio.h	1.3 */
/*
 * Line Printer Type Devices I/O Control
 */

struct lprio {
	short	ind;
	short	col;		/* col after which no more data is printed */
	short	line;		/* line after which a FF is output. 0 = no FF */
	short	flag;
	int	ackdly;		/* gc8 only */
};

/* ioctl commands */
#define	LPR	('l'<<8)
#define	LPRGET	(LPR|01)
#define	LPRSET	(LPR|02)
#define	LPRGETV	(LPR|05)
#define	LPRSETV	(LPR|06)

/* flags */

#define CAP	0x0001
#define NOCR	0x0002
#define WF_ERR	0x0004
#define LPRAW	0x0008		/* implies (overrides) no CAP and NOCR */

#endif /* SYS_LPRIO_H */
