#ifndef SYS_ADV_H
#define SYS_ADV_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) adv.h: version 24.1 created on 10/28/91 at 18:31:06	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)adv.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/adv.h	10.7"		*/

/*
 *	advertise structure.
 *	one entry per advertised object.
 */
#ifndef	SYS_MOUNT_H
#include <sys/mount.h>
#endif	/* SYS_MOUNT_H */

struct	advertise	{
	int	a_flags;		/* defines are in sys/nserve.h	*/
	int	a_count;		/* number of active rmounts	*/
	char	a_name [NMSZ];		/* name sent to name server	*/
	struct	rcvd	*a_queue;	/* receive queue for this name	*/
	char	*a_clist;		/* ref to authorization list	*/
} ;

#ifdef INKERNEL

extern	struct	advertise	advertise[];

#endif

#endif /* SYS_ADV_H */
