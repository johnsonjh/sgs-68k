#ifndef SYS_EREC_H
#define SYS_EREC_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) erec.h: version 24.1 created on 10/28/91 at 18:32:14	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)erec.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#ifndef SYS_UTSNAME_H
#include "sys/utsname.h"
#endif

/*
 * Every error record has a header as follows.
 */

struct errhdr {
	short	e_type;		/* record type */
	short	e_len;		/* bytes in record (with header) */
	time_t	e_time;		/* time of day */
};

/*
 * Error record types
 */

#define E_GOTS  	010		/* start for error logging */
#define E_STOP		012		/* Stop */
#define E_TCHG		013		/* Time change */
#define E_CCHG		014		/* Configuration change */
#define E_BLK		020		/* Block device error */
#define E_DISK  	021		/* disk error */
#define E_MIRROR	022		/* mirrored slice error */
#define E_STRAY		030		/* Stray interrupt */
#define E_PRTY		031		/* Memory ECC */

/*
 * Error logging startup record. One of these is
 * sent to the logging daemon when logging is
 * first activated.
 */


struct estart {
	struct utsname e_name;	/* system names */
};

/*
 * Error logging termination record that is sent to the daemon
 * when it stops error logging.
 */

#define eend errhdr

/*
 * A time change record is sent to the daemon whenever
 * the system's time of day is changed.
 */

struct etimchg {
	time_t	e_ntime;	/* new time */
};

/*
 * A configuration change message is sent to
 * the error logging daemon whenever a block device driver
 * is attached or detached (MERT only).
 */

struct econfchg {
	char	e_trudev;	/* "true" major device number */
	char	e_cflag;	/* driver attached or detached */
};

#define E_ATCH	1
#define E_DTCH	0


/*
 * Template for the error record that is logged by block devices.
 */
/* got rid of e_stats temporarily */

struct eblock {
	dev_t	e_dev;		/* "true" major + minor dev number */
	physadr	e_regloc;	/* controller address */
	short	e_bacty;	/* other block I/O activity */
/*	struct iostat e_stats;	/* unit I/O statistics */
	short	e_bflags;	/* read/write, error, etc */
	short	e_cyloff;	/* logical dev start cyl */
	daddr_t	e_bnum;		/* logical block number */
	ushort e_bytes;		/* number of bytes to transfer */
	paddr_t	e_memadd;	/* buffer memory address */
	ushort e_rtry;		/* number of retries */
	short	e_nreg;		/* number of device registers */
#ifdef vax
	struct	mba_regs e_mba;	/* mass bus adaptor registers */
#endif
};

/*
 * Flags (selected subset of flags in buffer header)
 */

#define E_WRITE	0
#define E_READ	1
#define E_NOIO	02
#define E_PHYS	04
#define E_MAP	010
#define E_ERROR	020

/*
 * flags indicating type of stray interrupt
*/
/*   the following are reserved to represent auto vectored interrupts
     whose source could not be determined
	#define E_AUTO1 4
	#define E_AUTO2 5
	#define E_AUTO3 6
	#define E_AUTO4 7
	#define E_AUTO5 8
	#define E_AUTO6 9
*/
#define E_RAND 10
#define E_SPUR 11

/*
 * Template for the stray interrupt record that is logged
 * every time an unexpected interrupt occurs.
 */

struct estray {
	physadr	e_saddr;	/* stray loc or device addr */
	char	e_flag;		/* type of stray interrupt */
};

/*
 * Memory error record that is logged whenever one
 * of those things occurs 
 */

struct ememory {
	int	e_syndrome;
	int	e_memcad;
	char	e_pmb_priority;
	char	e_rmw;
	char	e_uncorrectable;
	char	e_bytesel;
	char	e_code_access;
	char	e_user_access;
};

/*
 * error logging structure for disk errors
 * 
 */

struct edisk  {
	int	e_phys_drive_no;
	int	e_head_no;
	int	e_track_no;
	int	e_sector_no;
	int	e_log_drive_no;
	int	e_log_blk_no;
	int	e_err_code;
};

/*
 * error logging structure and defines  for mirrored slice errors
 * 
 */

#define READ_PRI_ERROR		0x0001	/* read error on primary	*/
#define READ_SEC_ERROR		0x0002	/* read error on secondary	*/
#define WRITE_PRI_ERROR		0x0004	/* write error on primary	*/
#define WRITE_SEC_ERROR		0x0008	/* write error on secondary	*/
#define DOUBLE_READ_ERROR	(READ_PRI_ERROR | READ_SEC_ERROR)
#define DOUBLE_WRITE_ERROR	(WRITE_PRI_ERROR | WRITE_SEC_ERROR)
#define BLOCK_IO_ERROR		0x0010
#define RAW_IO_ERROR		0x0020
#define MIRROR_CREATE		0x1000	/* mirror created		*/
#define MIRROR_INVALID		0x2000	/* mirror invalidated by error	*/
#define MIRROR_DELETE		0x4000	/* mirror deleted		*/
#define MIRROR_COPY		0x8000	/* mirror copied		*/

/* mirror copy direction hidden in em_log_blk_no if MIRROR_COPY */
#define MIRROR_P2S		1	/* copy primary to secondary	*/
#define MIRROR_S2P		0	/* copy secondary to primary	*/

struct emirror  {
	dev_t	em_mirror;		/* mirror minor device		*/
	dev_t	em_pri;			/* primary device		*/
	dev_t	em_sec;			/* secondary device		*/
	ushort	em_err_code;		/* error code			*/
	uint	em_blkno;		/* logical block number of err	*/
	uint	em_flags;		/* mirror_t flags		*/
};


#endif /* SYS_EREC_H */
