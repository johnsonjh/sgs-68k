#ifndef	SYS_JIOCTL_H
#define	SYS_JIOCTL_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) jioctl.h: version 24.1 created on 10/28/91 at 18:34:43	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)jioctl.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
**	Unix to Jerq I/O control codes
*/

#define	JTYPE		('j'<<8)
#define	JBOOT		(JTYPE|1)
#define	JTERM		(JTYPE|2)
#define	JMPX		(JTYPE|3)
#define	JTIMO		(JTYPE|4)	/* Timeouts in seconds */
#define	JWINSIZE	(JTYPE|5)
#define	JTIMOM		(JTYPE|6)	/* Timeouts in millisecs */
#define	JZOMBOOT	(JTYPE|7)
#define JAGENT		(JTYPE|9)	/* control for both directions */
#define JTRUN		(JTYPE|10)	/* send runlayer command to layers*/

struct jwinsize {
	char	bytesx, bytesy;	/* Window size in characters */
	short	bitsx, bitsy;	/* Window size in bits */
};

/**	Channel 0 control message format **/

struct jerqmesg {
	char	cmd;		/* A control code above */
	char	chan;		/* Channel it refers to */
};

/*
**	Character-driven state machine information for Jerq to Unix communication.
*/

#define	C_SENDCHAR	1	/* Send character to layer process */
#define	C_NEW		2	/* Create new layer process group */
#define	C_UNBLK		3	/* Unblock layer process */
#define	C_DELETE	4	/* Delete layer process group */
#define	C_EXIT		5	/* Exit */
#define	C_DEFUNCT	6	/* Send terminate signal to proc. group */
#define	C_SENDNCHARS	7	/* Send several characters to layer proc. */
#define	C_RESHAPE	8	/* Layer has been reshaped */
#define C_RUN           9       /* Run command in layer */

/*
**	Usual format is: [command][data]
*/

/*
 *	This defines things to do with the host agent on the Blit.
 */

struct bagent {
	int size;	/* size of src string going in and dest string out */
	char * src;	/* address of the source byte string */
	char * dest;	/* address of the destination byte string */
};
#endif	/* SYS_JIOCTL_H */
