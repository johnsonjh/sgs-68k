#ifndef SYS_M68020_MPSL_H
#define SYS_M68020_MPSL_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Mpsl.h: version 24.1 created on 10/28/91 at 18:30:16	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Mpsl.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

typedef	ushort	psw_t;

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

#endif /* SYS_M68020_MPSL_H */
