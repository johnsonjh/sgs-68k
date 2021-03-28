#ifndef SYS_RFSYS_H
#define SYS_RFSYS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) rfsys.h: version 2.1 created on 5/22/89 at 19:10:08	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)rfsys.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/rfsys.h	10.4.1.1"	*/

#ident	"@(#)uts/head/sys:rfsys.h	2.1"

#ifdef INKERNEL
extern	char	Domain[];
#endif

/*  opcodes for rfsys system call */

#define RF_FUMOUNT	1	/* forced unmount */
#define RF_SENDUMSG	2	/* send buffer to remote user-level */
#define RF_GETUMSG	3	/* wait for buffer from remote user-level */
#define RF_LASTUMSG	4	/* wakeup from GETUMSG */
#define RF_SETDNAME	5	/* set domain name */
#define RF_GETDNAME	6	/* get domain name */
#define RF_SETIDMAP	7
#define RF_FWFD		8
#define RF_VFLAG	9
#define RF_DISCONN	10	/* return value for link down */
#define RF_VERSION	11
#define RF_RUNSTATE	12	/* see if RFS is running */


/* defines for VFLAG option	*/
#define V_CLEAR 0
#define V_SET	1
#define V_GET	2

/* defines for the VERSION option	*/
#define VER_CHECK	1
#define VER_GET		2

#endif /* SYS_RFSYS_H */
