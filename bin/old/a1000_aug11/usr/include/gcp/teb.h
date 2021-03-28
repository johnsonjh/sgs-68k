#ifndef SYS_GCP_TEB_H
#define SYS_GCP_TEB_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) teb.h: version 2.1 created on 5/22/89 at 19:07:10	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)teb.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)iok/head:teb.h	I%"
/*
 *	T I M E R    E V E N T    B L O C K S
 */

#define NTEB	100

struct teb {
	struct teb *	teb_forward;	/* forward chain pointer	*/
	struct teb *	teb_backward;	/* backward chain pointer	*/
	struct tcb *	teb_tcb;	/* task control block		*/
	int	  (*	teb_service)();	/* service routine address	*/
	int 		teb_ticks;	/* number of ticks		*/
	int 		teb_arg;	/* arg				*/
};

#endif /* SYS_GCP_TEB_H */
