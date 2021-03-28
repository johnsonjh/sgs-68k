#ifndef SYS_SLICB_H
#define SYS_SLICB_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) slicb.h: version 2.1 created on 5/22/89 at 19:10:43	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)slicb.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#)slicb.h	2.1	*/
/*   Arete/5.0   6/30/84   /usr/src/head/sys/slicb.h   */

#ident	"@(#)uts/head/sys:slicb.h	2.1"

#define NUM_CMDS 4
/* command 1 is used to cause slaves to reschedule the current process */
/* command 2 is used to cause slaves to look at runq */
/* command 3 is used to cause slaves to reprogram their mmu registers */
extern char cmd_length[NUM_CMDS];

#endif /* SYS_SLICB_H */
