#ifndef SYS_LPRIO_H
#define SYS_LPRIO_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) lprio.h: version 24.1 created on 10/28/91 at 18:35:21	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)lprio.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

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
