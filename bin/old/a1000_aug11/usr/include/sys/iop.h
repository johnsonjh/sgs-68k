#ifndef SYS_IOP_H
#define SYS_IOP_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iop.h: version 2.1 created on 5/22/89 at 19:08:00	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iop.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys:iop.h	2.1"

#define MAXSLOTS 19
#define MAXIOPS 12
#define MAXASPS 12
#define MAXDTCS 12

/*
 *	board types
 */

#define CPU 0
#define MASTERCPU 0
#define SLAVECPU  1
#define DMC 2
#define DTC 3
#define ASP 4
#define DTC68K 5
#define MEMBOARD 6

/*
 *	board vectors
 */

#define VECDTC 0
#define VECASP 1
#define VECDMC 2
#define VECCPU 3

struct iop {
	struct iop *nextiop;
	struct iop *nextvec;
	int (* service)();		/* interrupt service routine */
	int (* proc)();
	union rtb *rtb;
	unsigned short offset;
	char slot;
	char type;
	char board;
};

#define IOPC	('I'<<8)
#define IOPGET	(IOPC|1)
#define IOPSET	(IOPC|2)
#define IOPPROC	(IOPC|3)

#define IOP_BASE(n) (0x600000 + (n << 16))

#endif /* SYS_IOP_H */
