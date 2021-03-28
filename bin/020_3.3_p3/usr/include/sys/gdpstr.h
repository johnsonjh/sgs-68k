#ifndef SYS_GDPSTR_H
#define SYS_GDPSTR_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gdpstr.h: version 24.1 created on 10/28/91 at 18:33:15	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gdpstr.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/gdpstr.h	10.5"		*/

#ifndef	SYS_SEMA_H
#include <sys/sema.h>
#endif	/* SYS_SEMA_H */

#ifndef	SYS_COMM_H
#include <sys/comm.h>
#endif	/* SYS_COMM_H */

/*  
 *	commands for virtual circuits set up 
 */

#define PM_DATA		0
#define	PM_LISTEN	1
#define	PM_CONNECT	2
#define	PM_ACCEPT	3
#define	PM_DISCONNECT	4

/*  
 *	reason for disconnect
 */

#define	VC_REJECT	1	/* reject a conection request */
#define	VC_CLOSE	2	/* closed by the other end */
#define	VC_CONNFAIL	3	/* fail to contact remote side */
#define	VC_LINKDOWN	4	/* link down */
#define VC_NORMAL	6	/* normal disconnect link not needed */

/*  
 *	messages passed from the upper modules  
 *	to the protocol module 
 */

/*  connection request */

struct	pm_connect {
	naddr_t	dstnadr;	/* destination network address */
	ushort	dstmdev;	/* destination minor device number */
	ushort	nretry;		/* number of retries for set up */
	ushort	interval;	/* time interval between retries */
};


/*  accept the connection request */

struct	pm_accept {
	naddr_t	dstnadr;	/* destination network address */
	ushort	dstmdev;	/* destination minor device number */
	ushort	respmdev;	/* responding minor device number */
};


/*  reject the connection request or 
    disconnect a virtual circuit */

struct	pm_disconnect {
	naddr_t	dstnadr;	/* destination network address */
	ushort	dstmdev;	/* destination minor device number */
	short	reason;		/* reason for disconnect */
};


/*  
 *	messages returned from the protocol module
 *	to the upper modules  
 */

struct	resp_msg {
	short	cmd;		/* command */
	short	reason;		/* reason for disconnect */
	naddr_t	srcnadr;	/* source network address */
	ushort	srcmdev;	/* source minor device number */
};

#endif /* SYS_GDPSTR_H */
