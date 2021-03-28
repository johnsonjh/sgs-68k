#ifndef SYS_PSL_H
#define SYS_PSL_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) psl.h: version 2.1 created on 5/22/89 at 19:09:41	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)psl.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	psl.h	3.13	*/
/*   Arete/5.0   6/30/84   /usr/src/head/sys/psl.h   */

#ident	"@(#)uts/head/sys:psl.h	2.1"

/*
 *  	processor status
 */

#define	PS_C	0x0001		/* carry bit */
#define	PS_V	0x0002		/* overflow bit */
#define	PS_Z	0x0004		/* zero bit */
#define	PS_N	0x0008		/* negative bit */
#define	PS_X	0x0010		/* extend bit */

#define PS_IPL	0x0700		/* interrupt priority level */

#define	PS_S	0x2000		/* supervisor bit */

#define	PS_T	0x8000		/* trace enable bit */

#endif /* SYS_PSL_H */
