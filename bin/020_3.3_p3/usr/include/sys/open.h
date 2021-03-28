#ifndef SYS_OPEN_H
#define SYS_OPEN_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) open.h: version 24.1 created on 10/28/91 at 18:36:07	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)open.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/open.h	10.2"		*/

/* Some drivers need to be able to keep accurate records of open/close
 * calls to determine whether a device is still in use.  To allow this
 * open/close calls have been typed and the type is passed as a third
 * argument in open/close calls, as in:
 *	(*cdevsw[major(dev)].d_open)(minor(dev), flag, OTYP_CHR);
 * or
 *	(*cdevsw[major(dev)].d_close)(minor(dev), flag, OTYP_CHR);
 * Five types of open/close calls have been defined:
 * OTYP_BLK:	open/close of a block special file
 * OTYP_MNT:	open/close for mounting/unmounting a file system
 * OTYP_CHR:	open/close of a character special file
 * OTYP_SWP:	open/close of a swapping device.
 * OTYP_LYR:	open/close calls from a driver to another driver,
 *		without a file being open for the dev of the lower driver.
 * New from ARIX!
 * OTYP_MIR:	open/close of a mirrored device.
 *
 * The first four types of open/close calls obey the protocol rule
 * that many more opens may occur for a given minor(dev) for that type of open,
 * but a close call happens only on the last close of that dev.
 * This protocol allows a flag to be used (set by opens, cleared by closes)
 * to keep track of the state for a given minor device value.
 *
 * Calls of the fifth type (OTYP_LYR) must obey the protocol rule
 * that open and close call calls are always paired.  This protocol
 * permits several drivers to be layers above the same device driver.
 * A counter can be used for this protocol.
 *
 * The value OTYPCNT is defined for the purpose of declaring arrays
 * in drivers and for performing range checks (0 <= otyp < OTYPCNT)
 * on values passed.
 */

#define OTYPCNT		6
#define OTYP_BLK	0
#define OTYP_MNT	1
#define OTYP_CHR	2
#define OTYP_SWP	3
#define OTYP_LYR	4
#define OTYP_MIR	5

/*
 * Arix	8/19/88
 * The mount and umount code has been changed to use the minor define too.
 * Now the only driver routines that receive a full dev_t are strategy
 * routines and stream open routines.
 */

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#define OTYP_FLG(x)	(1 << (x))

#define OFLG_BLK	OTYP_FLG(OTYP_BLK)	/* open type flags	*/
#define OFLG_MNT	OTYP_FLG(OTYP_MNT)
#define OFLG_CHR	OTYP_FLG(OTYP_CHR)
#define OFLG_SWP	OTYP_FLG(OTYP_SWP)
#define OFLG_MIR	OTYP_FLG(OTYP_MIR)

#define ot_flag		u.o_flag
#define ot_lyrcnt	u.o_lyrcnt

typedef union opentyp {
	struct ot_u {
		ushort	o_lyrcnt;	/* layer open count		*/
		ushort	o_flag;		/* many-opens-to-one-close flags */
	} u;
	ulong	ot_all_opens;	/* check both types of opens at once	*/
} opentyp_t;

#endif /* SYS_OPEN_H */
