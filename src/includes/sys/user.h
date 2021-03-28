#ifndef	SYS_USER_H
#define SYS_USER_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) user.h: version 25.2 created on 12/13/91 at 21:42:58	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)user.h	25.2	12/13/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#if defined(M68020) || defined(M68040)
#ifndef SYS_FPU_H
#include "sys/fpu.h"
#endif
#endif	/* m68k */

#ifndef SYS_FS_S5DIR_H
#include "sys/fs/s5dir.h"
#endif

#ifndef SYS_PARAM_H
#include "sys/param.h"
#endif

#if defined(INKERNEL) && !defined(SYS_INODE_H)
#include "sys/inode.h"
#endif


#ifndef SYS_SIGNAL_H
#include "sys/signal.h"
#endif

#ifndef SYS_IMMU_H
#include "sys/immu.h"
#endif

#ifndef SYS_STATE_H
#include "sys/state.h"
#endif

#ifndef SYS_TIME_H
#include "sys/time.h"
#endif

#include "sys/ofile.h"
#include "sys/resource.h"

#ifndef USIZE

#ifdef	M68020
#define USIZE	1
#endif	/* M68020 */

#endif /* USIZE */

#define	PSARGSZ	80	/* Space in u-block for exec arguments.	*/
			/* Used by ps command.			*/

#define	PSCOMSIZ	DIRSIZ	/* For the time being set PSCOMSIZ */
				/* to DIRSIZ until we can get rid of */
				/* struct direct u_dent */

/* JTOF - allows files not marked with POSIX_SOURCE to compile okay */
#ifndef _POSIX_SOURCE
#define	GROUPS	16
#else
#define	GROUPS	NGROUPS_MAX
#endif

/* JTH sgs3.4p1 changes */

typedef struct
{
    uint rlim_cur;
    uint rlim_max;
} rlimit_t;

/* End of changes */

/*
 * The user structure.
 * One allocated per process.
 * Contains all per process data that doesn't need to be referenced
 * while the process is swapped.
 *
 */
typedef	struct	user {
	/*
	 * The following fields are guaranteed to remain at the same offset
	 * in the user structure.  Drivers should not need any others.
	 */
	struct proc *u_procp;		/* pointer to proc structure	*/
	int	u_error;		/* return error code		*/
	ushort	u_uid;			/* effective user id		*/
	ushort	u_gid;			/* effective group id		*/
	ushort	u_ruid;			/* real user id			*/
	ushort	u_rgid;			/* real group id		*/

	caddr_t	u_base;			/* base address for IO		*/
	uint	u_count;		/* bytes remaining for IO	*/
	off_t	u_offset;		/* offset in file for IO	*/
	short  *u_ttyp;			/* pointer to pgrp in "tty" struct*/

	dev_t	u_ttyd;			/* controlling tty dev		*/
	short	u_fmode;		/* file mode for IO		*/

	union {				/* syscall return values	*/
		struct	{
			int	r_val1;
			int	r_val2;
		}r_reg;
		off_t	r_off;
		time_t	r_time;
	} u_r;

	/* non-guaranteed fields */

	int	*u_ap;			/* pointer to arglist		*/

	char	u_comm[PSCOMSIZ];	/* command name			*/
	short	u_cmask;		/* mask for file creation	*/

	struct state *u_astk;		/* address of users saved stack frame */
	int	u_caddrflt;		/* Ptr to function to handle user
					 * space memory faults encountered in
					 * the kernel.
					 */
	char	*u_comp;		/* pointer to current component	*/
	char	*u_nextcp;		/* pointer to beginning of next	*/

					/* following for Distributed UNIX */
 	struct rem_ids {		/* for exec'ing REMOTE text	*/
 		ushort	ux_uid;		/* uid of exec'd file		*/
 		ushort	ux_gid;		/* group of exec'd file		*/
 		ushort	ux_mode;	/* file mode (set uid, etc.)	*/
 	} u_exfile;
	ushort 		u_rflags;	/* flags for distribution	*/
	int 		u_syscall;	/* system call number		*/
	int		u_mntindx;	/* mount index from sysid	*/

	struct sndd	*u_gift;	/* gift from message		*/
	struct response	*u_copymsg;	/* copyout unfinished business	*/
	struct msgb	*u_copybp;	/* copyin premeditated send	*/
	char 		*u_msgend;	/* last byte of copymsg + 1	*/
					/* end of Distributed UNIX	*/
	
	void (*u_signal[MAXSIG])();	/* disposition of signals	*/

	label_t	u_rsav;			/* save area for stack exchange */
	label_t	u_qsav;			/* save area for quits and interrupts */
	char	u_psargs[PSARGSZ];	/* arguments from exec system call */
	int	u_sysabort;		/* for debuggers, signal causing
					 * core dump (ATT uses this for
					 * system call debugging)	*/
	daddr_t	u_limit;		/* maximum write address	*/
	daddr_t	u_limit_adj;		/* maximum write address	*/

	char	u_acflag;		/* accounting flag		*/
	char	u_segflg;		/* IO: 0=user D; 1=system; 2=user I */
	short	u_lock;			/* process/text locking flags */
	ushort	u_pbsize;		/* Bytes in block for IO	*/
	ushort	u_pboff;		/* offset in block for IO	*/
	dev_t	u_pbdev;		/* real device for IO		*/
	short	u_errcnt;		/* syscall error count		*/
	daddr_t	u_rablock;		/* read ahead block address	*/
	daddr_t	u_next_rablock;		/* read ahead block address	*/

	struct inode *u_cdir;		/* current directory		*/
	struct inode *u_rdir;		/* root directory		*/
	struct inode *u_pdir;		/* inode of parent directory of dirp */
	caddr_t	u_dirp;			/* pathname pointer		*/

	struct direct u_dent;		/* current directory entry	*/

	uint	:32;

	int	*u_ar0;			/* pointer to registers for debuggers */
	int	u_arg[6];		/* arguments to current system call */

	time_t	u_utime;		/* this process user time	*/
	time_t	u_stime;		/* this process system time	*/
	time_t	u_cutime;		/* sum of childs' utimes	*/
	time_t	u_cstime;		/* sum of childs' stimes	*/
	ktimerval_t	u_timer[3];	/* for setitimer() BSD syscall	*/

	struct {			/* profile arguments		*/
		short	*pr_base;	/* buffer base			*/
		unsigned pr_size;	/* buffer size			*/
		unsigned pr_off;	/* pc offset			*/
		unsigned pr_scale;	/* pc scaling			*/
	} u_prof;

	struct exdata {
		struct    inode  *ip;
		long      ux_tsize;	/* text size			*/
		long      ux_dsize;	/* data size			*/
		long      ux_bsize;	/* bss size			*/
		long      ux_lsize;  	/* lib size			*/
		long      ux_nshlibs; 	/* num of shared libs needed	*/
		short     ux_mag;   	/* magic number MUST be here	*/
		short	  ux_vers_pad;	/* make others long aligned */
		long      ux_toffset;	/* file offset to raw text	*/
		long      ux_doffset;	/* file offset to raw data	*/
		long      ux_loffset;	/* file offset to lib sctn	*/
		long      ux_txtorg;	/* start addr. of text in mem	*/
		long      ux_datorg;	/* start addr. of data in mem	*/
		long      ux_entloc;	/* entry location		*/
	} u_exdata;
	long   	u_execsz;		/* exec's sum of text, data, and bss */
	struct stdata *u_stp;		/* stream head of controlling tty */
	long	u_ioch;			/* RFS I/O			*/

	long	u_mem;			/* total number of valid pages	*/
	long	u_ior;			/* number of reads		*/
	long	u_iow;			/* number of writes		*/
	long	u_iosw;			/* number of swap operations	*/

	time_t	u_start;		/* starting time (seconds)	*/
	time_t	u_ticks;		/* starting time (clock ticks)	*/
	ulong	u_sub;			/* stack upper bound		*/
	caddr_t	u_datorg;		/* start of data page		*/

	int	u_ssize;		/* stack size (in pages)	*/
	int	u_tsize;		/* text size (in pages)		*/
	int	u_dsize;		/* data size (in pages)		*/
		
	fpu_info u_fpu;			/* floating point unit regs	*/
	uint		u_saved_blk;	/* bmap cache information */
	uint		u_saved_physblk;
#if	defined(INKERNEL)
	inode_t		*u_saved_ip;
#else
	caddr_t		u_saved_ip;
#endif
	uint		u_saved_ip_mod_cnt;

					/* posix fields folks...O.K?    */
	uint 	u_sigmask[MAXSIG];	/* mask to be effecive when the
					   signal handler is called;
					   only for sigaction		*/
	uint	u_oldmask;		/* especially for sigsuspend	*/

	uint		u_nofiles; 		/* number of fd's touched */
	ofile_chain_t	*u_ofile_chain; 	/* pointer to ofile chain */
	struct file	*u_fp[NOFILES_IN_U];	/* open file pointers */
	uchar		u_oflag[NOFILES_IN_U];  /* open file flags */
	rlimit_t	u_rlimit[RLIM_NLIMITS]; /* rlimit array */

} user_t;

#define USERSTACK_OFF	4	/* set stack to next lower int aligned adr */

#define USERSTACK_END	(caddr_t)((uint)userstack - USERSTACK_OFF)

extern user_t	u;
extern int	userstack[];

#define u_exuid u_exfile.ux_uid
#define u_exgid u_exfile.ux_gid
#define u_exmode u_exfile.ux_mode

#define	u_rval1	u_r.r_reg.r_val1
#define	u_rval2	u_r.r_reg.r_val2
#define	u_roff	u_r.r_off
#define	u_rtime	u_r.r_time

/* rcstat values for client caching */
#define	U_RCACHE	0x1


/* ioflag values: Read/Write, User/Kernel, Ins/Data */

#define	U_WUD	0
#define	U_RUD	1
#define	U_WKD	2
#define	U_RKD	3
#define	U_WUI	4
#define	U_RUI	5

#define	EXCLOSE	01

/* distribution: values for u_rflags */
#define FREMOTE	0x0002	/* file is remote  */

#define	U_RSYS		0x0004	/* system call has gone remote */
#define	U_DOTDOT	0x0200
#define U_RCOPY		0x0400	/* used by copyout for non-delay copy */

/*
 * U_AR0 is a fixed (bogus) address for debuggers to request register values
 */
#define	U_AR0		0x1000

#define	PAGESIZE	NBPP
 
/* for posix; to indicate the end of supplementary groups */
#define	NOGROUP		0xFFFF

#endif /* SYS_USER_H */
