#ifndef SYS_UADMIN_H
#define SYS_UADMIN_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) uadmin.h: version 24.1 created on 10/28/91 at 18:40:36	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)uadmin.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/uadmin.h	10.2"		*/

/* uadmin commands */

#define	A_REBOOT	1		/* stop the system immediately	*/
#define	A_SHUTDOWN	2		/* clean-up, sync, and umount /	*/
#define	A_REMOUNT	4		/* remount root (after fsck?)	*/

/* uadmin functions */

#define	AD_HALT		0		/* return to PROMs		*/
#define	AD_BOOT		1		/* reboot the system (sets MD_REBOOT) */
#define	AD_IBOOT	2		/* return to PROMs		*/

/* machine dependent parameters determine what happens in the PROMs */

#define MD_RESET	101		/* reset, use existing autoboot	*/
#define MD_REBOOT	102		/* set autoboot flag and reboot	*/
#define MD_PROM		103		/* clear autoboot flag & reset	*/
#define MD_POWEROFF	104		/* turn off the system		*/

/* added by uadmin to the machine dependent parameters */

#define MD_CLEAN	0x80		/* was a clean shutdown		*/

/* misc. */

#define MD_GET_ACTION(x) ((x) & ~MD_CLEAN)

#endif /* SYS_UADMIN_H */
