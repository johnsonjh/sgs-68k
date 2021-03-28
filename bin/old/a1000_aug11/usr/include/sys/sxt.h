#ifndef SYS_SXT_H
#define SYS_SXT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sxt.h: version 2.1 created on 5/22/89 at 19:11:31	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sxt.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	<sxt.h	6.2>	*/

#ident	"@(#)uts/head/sys:sxt.h	2.1"

/*
 **	Multiplexed channels driver header
 */

#define	MAXPCHAN	8		/* Maximum channel number */
#define	CHANBITS	3		/* Bits for channel number */
#define	CHANMASK	07		/* 2**CHANBITS - 1 */

#define	MAXLINKS	32
#define	CHAN(dev)	(dev&CHANMASK)
#define	LINK(dev)	((dev>>CHANBITS)&(0xff>>CHANBITS))
#define	DEV(link,chan)	(((link&(0xff>>CHANBITS))<<CHANBITS)|(chan&CHANMASK))

#if	(MAXPCHAN*MAXLINKS) > 256
	ERROR -- product cannot be greater than minor(dev)
#endif

struct Channel {
	short		l_state;	/* internal state */
	short		l_pgrp;		/* process group */
	struct termio	cb;		/* ioctl control packet */
};

struct Link {
	char		state;		/* current state of link */
	char		chan;		/* the current top dog */
	char		old;		/* Old line discipline for line */
	short		oldpgrp;	/* Orig pgrp for line */
	short		ttyd;		/* Orig ttyd for line */
	char		open;		/* Channel open bits */
	char		xopen;		/* Exclusive open bits */
	char		iblocked;	/* channels blocked for input */
	char		oblocked;	/* channels blocked for output*/
	dev_t		dev;		/* major and minor device # */
	struct	gctty	*line;		/* Real tty for this link */
	struct	Channel	chans[MAXPCHAN];/* Array of channels for this link */
};

/*
 * Link state values
 */
#define	SXFREE		0		/* link is unused */
#define	SXOPEN		1		/* link is opened */
#define	SXACTV		2		/* link is active */
#define	SXREAD		3		/* link is reading */

/*
**	Ioctl args
*/

#define	SXTIOCLINK	('b'<<8)	/* initiate link to real tty */
#define	SXTIOCTRACE	(SXTIOCLINK|1)	/* set trace level */
#define	SXTIOCWAIT	(SXTIOCLINK|2)	/* wait until active again */
#define SXTIOCSWTCH	(SXTIOCLINK|3)	/* switch active channels */
#define SXTIOCBLK	(SXTIOCLINK|4)	/* block channel output */
#define SXTIOCUBLK	(SXTIOCLINK|5)	/* unblock channel output */
#define SXTIOCSTAT	(SXTIOCLINK|7)	/* get status on layers */

#endif /* SYS_SXT_H */
