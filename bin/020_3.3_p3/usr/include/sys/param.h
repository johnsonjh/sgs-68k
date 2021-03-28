#ifndef SYS_PARAM_H
#define SYS_PARAM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) param.h: version 24.1 created on 10/28/91 at 18:36:17	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)param.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef SYS_FS_S5PARAM_H
#include "sys/fs/s5param.h"
#endif

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

/*
 * fundamental variables
 * don't change too often
 */

#ifndef USIZE

#if defined(M68020) || defined(M68040)
#define USIZE	1
#endif	/* M68020 | M68040 */

#endif /* USIZE */

#define	MAXPID	30000		/* max process id */
#define	MAXUID	60000		/* max user id */
#define	MAXLINK	1000		/* max links */

#define	SSIZE	1		/* initial stack size (*4096 bytes) */
#define	SINCR	1		/* increment of stack (*4096 bytes) */

#define	CANBSIZ	256		/* max size of typewriter line	*/

#define	HZ	50		/* 50 ticks/second of the clock */
#define CLKTICK	(1000000/HZ)	/* microseconds in a clock tick */
#define	SPM_HZ	2000		/* 2000 ticks/second of the fast spm clock */
#define IOPM_HZ 78125		/* time stamping clock on IOPM, 12.8 usec */

#define	NCARGS	5120		/* # characters in exec arglist */
				/*   must be multiple of NBPW.  */

/*	The NOFILE following define is here for temporary compatibility
**	and should be removed in the next release.  It gives a
**	value for the maximum number of open files per process.
**	However, this value is no longer a constant.  
**
**	The maximum number of open files per process is now quite
**	complicated:  The default maximum is SFNOLIM (soft file number
**	limit), which is set in the system file.  This may not exceed
**	SFNOLIM_MAX (set to NOFILE here for stdio concerns).  This default
**	limit may be raised through the system call setrlimit() to the
**	system file parameter HFNOLIM (hard file number limit), unless the
**	user is root, in which case it may be set to any value.
*/

#define	NOFILE	64
#define SFNOLIM_MAX	NOFILE	/* maximum defalt value for open file limit */
#define NOFILES_IN_U	32	/* number of file pointer in uarea */


/*	The following defines apply to the kernel virtual
**	address space.
*/
#define	SYSSEGSZ (NPGPT * 2)	/* The size of the kernel segment	*/
				/* sysseg in pages.  The starting	*/
				/* address comes from the vuifile.	*/
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
#define	PPIPE	26	/* (PZERO + 1)	/* 26 - pipe facility sleep pri	*/
/* #define	PSEMZ	(PZERO + 2)	/* 27 - sleep pri waiting for zero */
/* #define	PMSG	(PZERO + 2)	/* 27 - msg facility sleep pri	*/
/* #define	PSEMN	(PZERO + 3)	/* 28 - sleep pri waiting for > value */
/* #define	STIPRI	(PZERO + 3)	/* 28 - stream io sleep pri	*/
/* #define	STOPRI	(PZERO + 3)	/* 28 - stream io sleep pri	*/
#define	PWAIT	30
#define	PSLEP	39
#define	PUSER	60
#define	PIDLE	127

/*
 * fundamental constants of the implementation--
 * cannot be changed easily
 */

#define	NBPW	sizeof(int)	/* number of bytes in an integer */
#define	NCPS	NPGPT		/* Number of clicks per segment */
#define	CPSSHIFT	LOG_NPGPT	/* LOG2(NCPS) if exact */
#define	NBPC	4096		/* Number of bytes per click */
#define	BPCSHIFT	12	/* LOG2(NBPC) if exact */
#define	NULL	0
#define	CMASK	0		/* default mask for file creation */
#define	CDLIMIT	(1L<<11)	/* max write address, now configurable */
#define	NODEV	(dev_t)(-1)
#define	NBPSCTR		1024	/* Bytes per disk sector.	*/
#define SCTRSHFT	10	/* Shift for BPSECT.		*/

#define	UMODE	PS_S		/* usermode bits */
#define	USERMODE(ps)	(!((ps) & UMODE))
#define SYSMODE(ps)	((ps) & UMODE)
#define	BASEPRI(ps)	((ps) & PS_IPL)

#define	lobyte(X)	(((unsigned char *)&X)[1])
#define	hibyte(X)	(((unsigned char *)&X)[0])
#define	loword(X)	(((ushort *)&X)[1])
#define	hiword(X)	(((ushort *)&X)[0])


#define	MAXSUSE	255
#define	PNOSTOP	01000

/* REMOTE -- whether machine is primary, secondary, or regular */
#define SYSNAME 9		/* # chars in system name */
#define PREMOTE 39

/* maximum size of a raw transfer, must be evenly divisable by 4k */
#define	MAXPHYSIO	(256*1024)

#ifdef _POSIX_SOURCE
#define NGROUPS_MAX  16

#define _SC_ARG_MAX		1
#define _SC_CHILD_MAX		2
#define _SC_CLK_TCK		3
#define _SC_NGROUPS_MAX		4
#define _SC_OPEN_MAX		5
#define _SC_PASS_MAX		6
#define _SC_PID_MAX		7
#define _SC_UID_MAX		8
#define _SC_EXIT_SIGHUP		9
#define _SC_KILL_PID_NEG1	10
#define _SC_KILL_SAVED		11
#define _SC_PGID_CLEAR		12
#define _SC_JOB_CONTROL		13
#define _SC_SAVED_IDS		14
#define _SC_VERSION		15
#define _SC_SECURITY		16

#define _PC_FCHR_MAX		1
#define _PC_LINK_MAX		2
#define _PC_MAX_CANNON		3
#define _PC_MAX_CANON		3	/*POSIX as opposed to FIPS */
#define _PC_MAX_INPUT		4
#define _PC_NAME_MAX		5
#define _PC_PATH_MAX		6
#define _PC_PIPE_BUF		7
#define _PC_CHOWN_RESTRICTED	8
#define _PC_CHOWN_SUP_GRP	9
#define _PC_DIR_DOTS		10
#define _PC_GROUP_PARENT	11
#define _PC_LINK_DIR		12
#define _PC_NO_TRUNC		13
#define _PC_UTIME_OWNER		14
#define _PC_V_DISABLE		15
#define _PC_VDISABLE		15	/*This is for POSIX, the above is FIPS*/


#define MAX_CANON		255
#define	LINK_MAX		MAXLINK	/* max # of links to a single file */
#define PIPE_BUF		10240
#define MAX_INPUT		255
#define NAME_MAX		14
#define PATH_MAX		255	
#define MAXSYMLINKS		20
#define _POSIX_UTIME_OWNER	1
#define _POSIX_GROUP_PARENT	1
#define _POSIX_DIR_DOTS		1
#define _POSIX_CHOWN_SUP_GRP	1
#define _POSIX_CHOWN_RESTRICTED 1	/* The use of chown is restricted to 
					   a process with the apporpriate
					   privileges, and to changing the 
					   group ID of a file only to the
					   effective group ID of the process or
					   to one of its supplementary group IDs
					*/
#define _POSIX_NO_TRUNC  1		/* Pathname components longer than
					   NAME_MAX generate an error
					*/
#define _POSIX_VDISABLE  0x1f		/* Terminal special characters defined
					   in <termios.h> can be disabled
					   using this character value, if it
					   is defined.
					*/
#endif


/*
 * It is unclear who needs these, but well keep them around for now.
 */
#define	MAXMEM	0x1000		/* max core in clicks */
/*  MAXBLK should always be evenly divisible by 4 */
#define	MAXBLK	120		/* max blocks possible for phys IO */
#define MAXTBLK	130		/* max blks for tape phys io */
#define MAXDBLK	240		/* max blks for disk phys io */


#endif /* SYS_PARAM_H */
