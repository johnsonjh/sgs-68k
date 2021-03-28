/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#define PARAM_H

/*	ATT: #ident	"kern-port:sys/param.h	10.10"		*/

#ident	"%Z%uts/head:%M%	%I%"

/*
 * fundamental variables
 * don't change too often
 */
#include "sys/fs/s5param.h"

#define	MAXPID	30000		/* max process id */
#define	MAXUID	60000		/* max user id */
#define	MAXLINK	1000		/* max links */

#define	SSIZE	1		/* initial stack size (*4096 bytes) */
#define	SINCR	1		/* increment of stack (*4096 bytes) */

#define	CANBSIZ	256		/* max size of typewriter line	*/
#define	HZ	50		/* 50 ticks/second of the clock */

#define	NCARGS	5120		/* # characters in exec arglist */
				/*   must be multiple of NBPW.  */

/*	The following define is here for temporary compatibility
**	and should be removed in the next release.  It gives a
**	value for the maximum number of open files per process.
**	However, this value is no longer a constant.  It is a
**	configurable parameter, NOFILES, specified in the kernel
**	master file and available in v.v_nofiles.  Programs which
**	include this header file and use the following value may
**	not operate correctly if the system has been configured
**	to a different value.
*/

#define	NOFILE	64

/*	The following represent the minimum and maximum values to
**	which the paramater NOFILES in the kernel master file may
**	be set.
*/

#define	NOFILES_MIN	 20
#define	NOFILES_MAX	100

/*
 * priorities
 * should not be altered too much
 */

#define	PMASK	0177
#define	PCATCH	0400
#define	PSWP	0
#define	PINOD	10
#define	PRIBIO	20
#define	PZERO	25
#define PMEM	0
#define	NZERO	20
#define	PPIPE	26
#define	PWAIT	30
#define	PSLEP	39
#define	PUSER	60
#define	PIDLE	127

/*
 * fundamental constants of the implementation--
 * cannot be changed easily
 */

#define	NBPW	sizeof(int)	/* number of bytes in an integer */
#define	NCPS	64		/* Number of clicks per segment */
#define CPSSHIFT 6
#define	NBPC	4096		/* Number of bytes per click */
#define	BPCSHIFT	12	/* LOG2(NBPC) if exact */
#define	NULL	0
#define	CMASK	0		/* default mask for file creation */
#define	CDLIMIT	(1L<<11)	/* default max write address */
#define	NODEV	(dev_t)(-1)
#define	NBPSCTR		1024	/* Bytes per disk sector.	*/
#define SCTRSHFT	10	/* Shift for BPSECT.		*/

#define	UMODE	PS_S		/* usermode bits */
#define	USERMODE(ps)	(!(ps & UMODE))
#define SYSMODE(ps)	(ps & UMODE)
#define	BASEPRI(ps)	(ps & PS_IPL)

#define	lobyte(X)	(((unsigned char *)&X)[1])
#define	hibyte(X)	(((unsigned char *)&X)[0])
#define	loword(X)	(((ushort *)&X)[1])
#define	hiword(X)	(((ushort *)&X)[0])


#define	MAXSUSE	255
#define	PNOSTOP	01000

/* REMOTE -- whether machine is primary, secondary, or regular */
#define SYSNAME 9		/* # chars in system name */
#define PREMOTE 39

#define	MAXMEM	0x1000		/* max core in clicks */
/*  MAXBLK should always be evenly divisible by 4 */
#define	MAXBLK	120		/* max blocks possible for phys IO */
#define MAXTBLK	130		/* max blks for tape phys io */
#define MAXDBLK	240		/* max blks for disk phys io */

#define	CLKTICK	20000		/* microseconds in a clock tick */

