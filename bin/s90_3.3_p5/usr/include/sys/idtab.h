#ifndef SYS_IDTAB_H
#define SYS_IDTAB_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) idtab.h: version 24.1 created on 10/28/91 at 18:33:26	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)idtab.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/idtab.h	10.4.1.1"		*/

/*
 *
 *    defines for uid/gid translation.
 *
 */
#define MAXSNAME	20
#define OTHERID		MAXUID+1
#define NO_ACCESS	MAXUID+2
#define CFREE		0
#define CINUSE		1
#define CINTER		2
#define GLOBAL_CH	'.'	/* name of the "global" table	*/
#define UID_DEV		0	/* minor device number for uid device	*/
#define	GID_DEV		1	/* minor device number for gid device	*/
#define UID_MAP		UID_DEV
#define GID_MAP		GID_DEV

struct idtab	{
	unsigned short	i_rem;
	unsigned short	i_loc;
};
#define i_defval i_rem
#define i_tblsiz i_loc

struct idhead {
	unsigned short	i_default;
	unsigned short	i_size;
	unsigned short	i_cend;
	unsigned short	i_next;
	unsigned long	i_tries;
	unsigned long	i_hits;
};
#define HEADSIZE	(sizeof(struct idhead)/sizeof(struct idtab))
#ifdef INKERNEL
extern ushort	glid();
extern char	rfheap[];
extern int	rfsize;

#define	gluid(a,b)	glid(UID_DEV,a,b)
#define glgid(a,b)	glid(GID_DEV,a,b)
#endif

#endif /* SYS_IDTAB_H */
