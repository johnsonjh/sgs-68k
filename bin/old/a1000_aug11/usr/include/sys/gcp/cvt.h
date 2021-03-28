#ifndef SYS_GCP_CVT_H
#define SYS_GCP_CVT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) cvt.h: version 2.1 created on 5/22/89 at 19:06:10	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)cvt.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/gcp:cvt.h	2.1"

/*
 *	C O M M U N I C A T I O N   V E C T O R    T A B L E
 */

struct cvt {
	struct tcb *tcb;		/* current tcb */
	struct tcb *tcb_head;		/* head of tcb chain */
	struct teb *teb_head;		/* head of teb chain */
	struct teb *teb_free;		/* head of free teb chain */
	struct tcb *tcb_oproc;		/* icb output processors tcb */
	struct gctty *tty;		/* current tty */
	int icb_base;			/* icb base address */
	unsigned short slice;		/* current time slice */
};

extern struct cvt cvt;

#endif /* SYS_GCP_CVT_H */
