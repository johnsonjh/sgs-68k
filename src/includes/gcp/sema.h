#ifndef SYS_GCP_SEMA_H
#define SYS_GCP_SEMA_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sema.h: version 2.1 created on 5/22/89 at 19:06:50	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sema.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)head/gcp:sema.h		1.4"
/* 9-22-88	pad out sema to 4-byte multiple for sparc - gil hunt */

struct sema {
	struct tcb *	sema_tcb_wait;
	unsigned char 	sema_master_waiting;
	unsigned char 	sema_slave_waiting;
	unsigned char 	sema_id;
	unsigned char 	unused;  	/* pad out for sparc */
};

#define SEMA_FREE		0
#define SEMA_MASTER		1
#define SEMA_SLAVE		2
#define SEMA_ERROR		3

#endif /* SYS_GCP_SEMA_H */
