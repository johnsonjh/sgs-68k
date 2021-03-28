#ifndef SYS_PROC_H
#define SYS_PROC_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) proc.h: version 2.1 created on 5/22/89 at 19:09:35	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)proc.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/* @(#)proc.h	6.1 */

#ident	"@(#)uts/head/sys:proc.h	2.1"

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif /* SYS_TYPES_H */

#ifndef SYS_REGION_H
#include "sys/region.h"
#endif /* SYS_REGION_H */

/*
 * One structure allocated per active process. It contains all data needed
 * about the process while the process may be swapped out.
 * Other per process data (user.h) is swapped with the process.
 */


typedef struct	proc {
	struct proc *f_link;	/* linked list of processes */

	char	p_stat;
	char	p_nice;		/* nice for cpu usage */
	ushort	p_sgid;		/* effective group id */

	char	p_pri;		/* priority, negative is high */
	char	pad3[3];

	uint	p_flag;

	char	p_time;		/* resident time for scheduling */
	char	pad5[3];

	time_t  p_utime;	/* user time, this proc */
	time_t  p_stime;	/* system time, this proc */

	char	p_cpu;		/* cpu usage for scheduling */
	char	p_fpp_save;
	ushort	p_whatstop;	/* more detailed reason for stop */

	ushort	p_uid;		/* real user id */
	ushort	p_suid;		/* set (effective) user id */

	short	p_pgrp;		/* name of process group leader */
	short	p_pid;		/* unique process id */
	int	p_ppid;		/* process id of parent */

	pde_t  	p_addr;		/* upage 1st page descriptor */
	/*
	**	This is really not required because we allocate
	**	contiguous pages for our ublock, BUT, the page
	**	allocation routine (ptconmemall) requires an
	**	entry for each page it allocates.
	**	These entries must be one after another (contiguous)
	**	in the proc structure because they are accessed
	**	indirectly via pointer manipulation.
	**	All commands and utilities that use proc structure
	**	must be recompiled every time proc structure chages size.
	*/
	pde_t  	p_addr2;	/* upage 2nd page descriptor */
	preg_t	*p_region;	/* process regions */

	struct text *p_textp;	/* pointer to text structure */
	struct proc *p_back; 	/* mfs_sleep/wakeup */
	struct proc *p_parent;	/* ptr to parent process */
	struct proc *p_child;	/* ptr to first child process */
	struct proc *p_sibling;	/* ptr to next sibling */
	struct proc *p_mlink;	/* processes asleep on memwant or swapwant */

	caddr_t	p_wchan;	/* wait addr for sleeping processes */
	ushort	p_mpgneed;	/* number of pages needed in memwant */
	short	p_epid;		/* effective pid (for RFS) */
	struct  rcvd  *p_minwd; /* server msg arrived on queue */
	struct  proc  *p_rlink; /* linked list for server */
	int     p_trlock;	/* */
	struct	inode *p_trace; /* inode ptr for /proc */
	int	p_sigmask;	/* */
	sysid_t p_sysid;	/* equal to sysid of requesting system */

	short	p_xstat;	/* exit status for wait */

	int	p_sig;		/* signals pending to this process */
	int	p_hold;		/* hold signal bit mask */
	int	p_chold;	/* defer signal bit mask */

	int	p_clktim;	/* time to alarm clock signal */
	uint	p_size;		/* size of swappable image in pages */

	short	p_smbeg;	/* first mmuindex for shared mem */
	short	p_smend;	/* last mmuindex for shared mem */

	char	p_sys;		/* sys bits */
	char	p_sl_resumable;	/* bit 0 == 1 then process can be resumed to
				   slave from sleep/wakeup */
	char	p_fpp_type;	/* version number of the fpp */
	char	p_cpu_type;	/* version number of the cpu */

	char	p_mfs_sleep;	/* set when proc waiting to enter long cr */
	unchar	p_cursig;	/* current signal */
	ushort	p_whystop;	/* reason for process stop */

	
	uint	p_signal_sem;   /* used for synchronizing psignal(), issig() */
	uint	p_start_time;	/* waiting for long cc (for timeouts) */
} proc_t;

#define p_link f_link
extern struct proc proc[];	/* the proc table itself */

#define PTRACED(p)	((p)->p_flag&(STRC|SPROCTR|SSEXEC|SPROPEN))

/* sys codes */
#define SLBUS	0x01		/* should be taken by master processor */
#define RUNNING 0x02		/* running on slave */
#define SLSYS 	0x10		/* can be taken by slave processor */

/* p_sys mode masks */
#define SYS_MODE_MASK 0x1	/* bit 0 == 0 if process is in system mode */
#define RUNNING_MASK  0x2	/* bit 1 == 1 if process is on slave */
#ifndef M68020_MASK
#define M68020_MASK   0x4	/* bit 2 == 1 if process requires 68020 cpu */
#endif  /* M68020_MASK */
#define FPP_MASK      0x8	/* bit 3 == 1 if process requires FPP */
#define SLOTNUM_MASK  0xe0	/* bits 5 - 7 contain slotnum of processor 
				   executing this process */


/* stat codes */
#define	SSLEEP	1		/* awaiting an event */
#define	SRUN	2		/* running */
#define	SZOMB	3		/* terminated, but not waited for */
#define	SSTOP	4		/* stopped by signal since being traced */
#define	SIDL	5		/* intermediate state in process creation */
#define SONPROC 6		/* process will be run on a processor */
#define	SXBRK	7		/* process being xswapped */


/* flag codes */
#define	SSYS	0x0001		/* system process */
#define	STRC	0x0002		/* process is being traced */
#define	SWTED	0x0004		/* stopped process taken by parent with wait */
#define	SNWAKE	0x0008		/* cannot be woken by a signal */
#define	SLOAD	0x0010		/* in core */
#define	SLOCK	0x0020		/* process cannot be swapped */
#define	SRSIG	0x0040		/* Set when signal goes remote	*/
#define SPOLL	0x0080		/* Process in stream poll	*/
#define SPRSTOP	0x0100		/* process is being stopped via /proc */
#define SPROCTR	0x0200		/* signal tracing via /proc */
#define SPROCIO	0x0400		/* doing I/O via /proc, so don't swap */
#define SSEXEC	0x0800		/* stop on exec */
#define SPROPEN	0x1000		/* process is open via /proc */
#define SRUNLCL 0x00004000	/* set process running on last /proc close */
#define SNOSTOP 0x00008000	/* proc asleep, stop not allowed */
#define SPTRX	0x00010000	/* process is exiting via ptrace(2) */
#define SASLEEP 0x00020000	/* process is stopped within a call to sleep */


/* flags for newproc */
#define NP_FAILOK	0x1	/* don't panic if cannot create process */
#define NP_NOLAST	0x2	/* don't use last process slot */
#define NP_SYSPROC	0x4	/* system (resident) process */

/* reasons for stopping */
#define REQUESTED	1
#define SIGNALLED	2
#define SYSENTRY	3
#define SYSEXIT		4


/* Macro to reduce unnecessary calls to issig() */

#define	ISSIG(p, why) \
  ((p)->p_cursig || (((p)->p_sig || ((p)->p_flag & SPRSTOP)) && issig(why)))

/* Reasons for calling issig() */

#define	FORREAL		0	/* Usual side-effects */
#define	JUSTLOOKING	1	/* Don't stop the process */

#endif /* SYS_PROC_H */
