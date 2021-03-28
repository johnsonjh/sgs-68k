/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sysmacros.h: version 24.1 created on 10/28/91 at 18:00:23	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sysmacros.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef IOPM_SYSMACROS_H
#define IOPM_SYSMACROS_H

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

/*
 * Some macros for units conversion
 */

/* bytes to clicks */
#define	btoc(x)	(((unsigned)(x)+(NBPC-1))>>BPCSHIFT)
#define	btoct(x)	((unsigned)(x)>>BPCSHIFT)

/*
 * Arix 9/88 JPC
 * 
 * The major, minor, and makedev macros were changed to allow extended minor
 * devices.  That is, easy extension of the number of minor devices per driver
 * to more than 256 by using more than one major device number for a driver.
 * The full device number is converted to a minor device number by subtracting
 * a value stored in the MINOR array, indexed by the major number.
 *
 * Problem:  If a minor device number can be > 255, then how does the minor
 * macro decide between a full device number with a small major component, and
 * a previously minored number?
 *
 * Solution:  Set the high bit (bit 15) of all full device numbers before
 * using minor.  This means that the largest major device number is 127,
 * and that there can be at most 32K devices.
 *
 * NOTE:  The only device driver functions that are given a full device
 * number are the open and close routines of streams drivers and the strategy
 * functions for block drivers (except when called for raw I/O).  The operating
 * system will already have set the "unminored" bit for the device number
 * passed to these functions.  All other driver functions will only get a minor
 * device number.
 *
 * As always, a device number should only be manipulated using these macros.
 * Any other use is implementation-dependent and will probably fail in future
 * releases of the operating system.
 */

#ifdef IOPM_OS
/* major part of a device number */
extern uchar  MAJOR[];
#define major(x)	(int)MAJOR[(dev_t)(x) >> 8 & 0x7f]

#define UNMINOR_BIT	0x8000		/* a dev_t with this bit is unminored */
#define UNMINOR_MASK	0x7fff		/* removes the unminored bit	*/

#define notminored(x)	((x) | UNMINOR_BIT)  /* add unminored bit to full dev */
#define denotminored(x)	((x) & UNMINOR_MASK) /*unzombie? dezombie? antizombie?*/

/* minor part of a device number */
#define minor(x)	(int)(((x) & UNMINOR_BIT) ? \
			  (dev_t)(x) - MINOR[major(x)] : (dev_t)(x))

extern unsigned short	MINOR[];	/* the offsets for extended minors */
#else

#define major(dev)  dmajor(dev)
#define minor(dev)  dminor(dev)

#endif

#endif /* IOPM_SYSMACROS_H */
