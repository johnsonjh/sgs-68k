#ifndef SYS_SAKIOCTL_H
#define SYS_SAKIOCTL_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sakioctl.h: version 24.1 created on 10/28/91 at 18:37:25	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sakioctl.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *	sakioctl.h - 'secure attention key' (SAK) ioctl struct
 */
struct	sakioctl
{
	unsigned char	flags;		/* SAK definition flags */
	unsigned char	sak_key1;	/* 1st SAK key if SAK_KEYS */
	unsigned char	sak_key2;	/* 2nd SAK key if SAK_KEYS */
	unsigned char	sak_key3;	/* 3rd SAK key if SAK_KEYS */
};

/*	SAK flags	*/

#define	SAK_BREAK	1	/* enable SAK on a break */
#define SAK_CARR	2	/* enable SAK on carrier drop (DCD/DSR) */
#define SAK_1KEY	4	/* enable SAK on one character */
#define SAK_2KEYS	8	/* enable SAK on two characters */
#define SAK_3KEYS	0x10	/* enable SAK on three characters */

#define SAK_KEYS	(SAK_1KEY | SAK_2KEYS | SAK_3KEYS)
#endif /* SYS_SAKIOCTL_H */
