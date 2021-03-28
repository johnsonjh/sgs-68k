#ifndef SYS_GCLP_H
#define SYS_GCLP_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gclp.h: version 2.1 created on 5/22/89 at 19:06:00	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gclp.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident	"@(#)head/sys:gclp.h	1.9"

/*
 * GCP Line Printer Type Devices I/O Control
 */

/*
 09-22-88 - pad gclp struct to 4-byte multiple for sparc - gil hunt 
 */

#ifndef	SYS_LPRIO_H
#include <sys/lprio.h>
#endif	/* SYS_LPRIO_H */

#ifndef	SYS_GC_H
#include <sys/gc.h>
#endif	/* SYS_GC_H */

struct gclp {
	struct lprio    lprio;
	struct cir_buf  l_outq;
	struct cirbuf   l_commands;
	struct cirbuf   l_lbuf[2];
	int             ccc;		/* software col count */
	int             mcc;		/* hardware col count */
	int             mlc;		/* hardware line count */
	unsigned short  l_state;
	char            open_flag;
	unsigned char	l_board;	/* board number			*/
	unsigned char	l_cent_stat;	/* cent port status		*/
	unsigned char   l_freelbuf;
	unsigned short  unused;		/* pad to 4-byte multiple for sparc */
};

/* l_state bits */
#define LP_BUSY		0x0001

#define LPVERSION  0x30000000

#define	FORM	014

#define GCLP_MTOS_OPEN		(0x0000|GC_LP_COMMANDS)
#define GCLP_MTOS_CLOSE		(0x0100|GC_LP_COMMANDS)
#define GCLP_MTOS_WRITE		(0x0200|GC_LP_COMMANDS)

#define GCLP_STOM_LOW_WATER	(0x0000|GC_LP_COMMANDS)

#endif /* SYS_GCLP_H */
