#ifndef SYS_PROC_H
#define SYS_PROC_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) proc.h: version 24.1 created on 10/28/91 at 18:36:43	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)proc.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_PARAM_H
#include "sys/param.h"
#endif

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#ifndef SYS_REGION_H
#include "sys/region.h"
#endif

#ifndef SYS_SYNCH_H
#include "sys/synch.h"
#endif

#ifndef SYS_USER_H
#include "sys/user.h"
#endif

#ifndef SYS_TIME_H
#include "sys/time.h"
#endif

#ifdef	SECON

#ifndef SYS_PRIV_H
#include "sys/priv.h"
#endif

#ifndef SYS_MLS_H
#include "sys/mls.h"
#endif

#endif	/* SECON */

/* auth is a tag around structure that belongs to the proc table. It
   is essentially an parallel extention to the proc that carries extra
   information. It is parallel because we want to keep the proc table
   the right size forever
*/
typedef struct auth {
	ushort		a_groups[GROUPS];	/* suplementary groups */
	short		p_save_pgrp;		/* save nm of pgrp	*/
	ktimerval_t	p_rtimer;		/* setitimer() BSD 	*/
	int		p_rtimer_id;		/* for setitimer() BSD  */
	int		p_rtimer_ticks;		/* for setitimer() BSD  */
#ifdef SECON
	ushort		a_role;			/* user role number */
	ushort		a_auid;			/* user audit id */
	uint 		a_amask[2];		/* user audit mask */
	priv_t		a_priv;			/* user privileges */
	uint		a_pused;		/* privs used for acct */
	slabel_t 	a_minlabel;		/* users min label */
	slabel_t	a_maxlabel;		/* user max label */
#else
	/*
	 *	Make sure that size of 'pad4' always corresponds to
	 *	size of 'priv_t' structure.  And, size of 'pad6' and
	 * 	'pad7' must correspond to size of 'slabel_t' structure.
	 */
	ushort		a_pad1;			/* user role number */
	ushort		a_pad2;			/* user audit id */
	uint 		a_pad3[2];		/* user audit mask */
	uchar		a_pad4[12];		/* user privileges */
	uint		a_pad5;			/* privs used for acct */
	uchar		a_pad6[18];		/* users min label */
	uchar		a_pad7[18];		/* users max label */
#endif	/* SECON */
#ifdef	PERF
	int		pa_slice_clamp;		/* every proc is different */
	uint		pa_slice_cnt;		/* full usage count	   */
	uint		pa_slice_ovfl;		/* overflow of above count */
	int		pa_pri_clamp;		/* every proc is different */
	uint		pa_pri_cnt;		/* full usage count	   */
	uint		pa_pri_ovfl;		/* overflow of above count */
	int		pa_usrpri_clamp;	/* every proc is different */
	uint		pa_usrpri_cnt;		/* full usage count	   */
	uint		pa_usrpri_ovfl;		/* overflow of above count */
	int		pa_cpu_clamp;		/* every proc is different */
	uint		pa_cpu_cnt;		/* full usage count	   */
	uint		pa_cpu_ovfl;		/* overflow of above count */
#else	/* PERF */
	int		pa_pad1;		/* every proc is different */
	uint		pa_pad2;		/* full usage count	   */
	uint		pa_pad3;		/* overflow of above count */
	int		pa_pad4;		/* every proc is different */
	uint		pa_pad5;		/* full usage count	   */
	uint		pa_pad6;		/* overflow of above count */
	int		pa_pad7;		/* every proc is different */
	uint		pa_pad8;		/* full usage count	   */
	uint		pa_pad9;		/* overflow of above count */
	int		pa_pad10;		/* every proc is different */
	uint		pa_pad11;		/* full usage count	   */
	uint		pa_pad12;		/* overflow of above count */
#endif	/* PERF */
	uint		pa_perf_flags;		/* performance flags	     */
	uchar		pa_usrpri;  /* user priority based on p_cpu & p_nice */
	uchar		pa_slptime; /* time since last block (sleeping)      */
	uchar		pa_pri_slp; /* pri before going to sleep	     */
	uchar		pa_pri_adj; /* part of dynamic pri recalculation     */
	int		pa_lticks;  /* per process copy of ticks remaining   */
	int		pa_quantum; /* per process quantum slice size	     */
	uint		pa_nswtch;  /* per process count of swtch's	     */
	uint		pa_ovfswtch;  /* per process overflow of swtch's     */
	} auth_t;

#define	p_realtimer		p_auth->p_rtimer
#define	p_realtimer_id		p_auth->p_rtimer_id
#define	p_realtimer_ticks	p_auth->p_rtimer_ticks
#ifdef	PERF
#define	p_slice_clamp		p_auth->pa_slice_clamp
#define	p_slice_cnt		p_auth->pa_slice_cnt
#define	p_slice_ovfl		p_auth->pa_slice_ovfl
#define	p_pri_clamp		p_auth->pa_pri_clamp
#define	p_pri_cnt		p_auth->pa_pri_cnt
#define	p_pri_ovfl		p_auth->pa_pri_ovfl
#define	p_usrpri_clamp		p_auth->pa_usrpri_clamp
#define	p_usrpri_cnt		p_auth->pa_usrpri_cnt
#define	p_usrpri_ovfl		p_auth->pa_usrpri_ovfl
#define	p_cpu_clamp		p_auth->pa_cpu_clamp
#define	p_cpu_cnt		p_auth->pa_cpu_cnt
#define	p_cpu_ovfl		p_auth->pa_cpu_ovfl
#endif	/* PERF */
#define	p_perf_flags		p_auth->pa_perf_flags
#define p_usrpri		p_auth->pa_usrpri
#define p_slptime		p_auth->pa_slptime
#define	p_pri_slp		p_auth->pa_pri_slp
#define	p_pri_adj		p_auth->pa_pri_adj
#define p_lticks		p_auth->pa_lticks
#define p_quantum		p_auth->pa_quantum
#define	p_nswtch		p_auth->pa_nswtch
#define	p_ovfswtch		p_auth->pa_ovfswtch

/*
 * One structure allocated per active process. It contains all data needed
 * about the process while the process may be swapped out.
 * Other per process data (user.h) is swapped with the process.
 */


typedef struct	proc {
	struct proc *f_link;	/* linked list of processes		*/

	/*
	 * The following fields are guaranteed to be at the same offsets in the
	 * proc structure.  Device drivers should not need any other fields.
	 */

	short	p_pid;		/* unique process id			*/
	short	p_pgrp;		/* name of process group leader		*/
	user_t	*p_userp;	/* kernel virtual address of u area	*/

	/* non-guaranteed fields */

	rde_t	p_urde;		/* user's root descriptor entry		*/

	uint	p_flag;		/* process flags			*/
	uchar	p_pri;		/* priority, smaller is better		*/
	char	pad3[3];
	char	p_time;		/* resident time for scheduling		*/
	char	pad5[3];
	char	p_stat;		/* process state			*/
	char	p_nice;		/* nice for cpu usage			*/
	uchar	p_cpu;		/* cpu usage for scheduling		*/
	uchar	p_cursig;	/* current signal			*/

	int	p_clktim;	/* time to alarm clock signal		*/
	caddr_t	p_wchan;	/* wait addr for sleeping processes	*/
	preg_t	*p_region;	/* process regions			*/
	ushort	p_whystop;	/* reason for process stop		*/
	ushort	p_whatstop;	/* more detailed reason for stop	*/

	uint	p_sig;		/* signals pending to this process	*/
	uint	p_hold;		/* hold signal bit mask			*/
	uint	p_chold;	/* defer signal bit mask		*/
	int	p_ppid;		/* process id of parent			*/

	struct proc *p_parent;	/* ptr to parent process		*/
	struct proc *p_child;	/* ptr to first child process		*/
	struct proc *p_sibling;	/* ptr to next sibling			*/
	struct proc *p_mlink;	/* processes asleep on memwant or swapwant */
				/* also used by sleep mechanism		*/

	uchar	p_running;	/* who is running proc, else NO_PM_ID	*/
	uchar	p_swapout;	/* if true, swapout wants to swap this process*/
	uchar	p_upkern_cnt;	/* reference count of uni-processor kernel */
	uchar	padY[1];

	uint	p_dests;	/* bit mask of PMs where p can run */
	uint	p_last_ran_on;	/* bit of last PM that p ran on */
	uint	p_cpu_dests;	/* bit mask of PMs with cpu mask needed p */
	uint	p_fpu_dests;	/* bit mask of PMs with fpu mask needed p */

	ushort	p_uid;		/* real user id				*/
	ushort	p_suid;		/* set (effective) user id		*/
	ushort	p_sgid;		/* effective group id			*/
	short	p_xstat;	/* exit status for wait			*/

	pde_t	p_ubptbl[USIZE];/* U block page table			*/
	uint	p_size;		/* size of swappable image in pages	*/
	time_t  p_utime;	/* user time, this proc			*/
	time_t  p_stime;	/* system time, this proc		*/

	struct  rcvd  *p_minwd; /* server msg arrived on queue		*/
	struct  proc  *p_rlink; /* linked list for server		*/
	short	p_epid;		/* effective pid (for RFS)		*/
	sysid_t p_sysid;	/* equal to sysid of requesting system	*/
	ushort	p_mpgneed;	/* number of pages needed in memwant	*/
				/* POSIX fields....agreeable?		*/
	uint	p_posix_flags;  /* posix signal flags like SA_NOCLDSTOP */
	uint	p_posix_sig;	/* signal set by posix sigaction	*/
	ushort	p_saved_set_uid;/* saved set user id; for posix		*/
	ushort	p_saved_set_gid;/* saved set group id; for posix	*/
	ushort	p_session_id;	/* session id added for posix		*/
	uint	p_my_tty;	/* pointer to stdata (stream) struct	*/
	auth_t	*p_auth;	/* user authentication structure 	*/
} proc_t;

#define p_link f_link
extern struct proc proc[];	/* the proc table itself */

#define PTRACED(p)	((p)->p_flag&STRC)
#define uauth		(u.u_procp->p_auth)

/* value needed for p_running when no processor is running a process */
#define	NO_PM_ID	0xff


/* stat codes */
#define	SSLEEP	1		/* awaiting an event */
#define	SRUN	2		/* running */
#define	SZOMB	3		/* terminated, but not waited for */
#define	SSTOP	4		/* stopped by signal since being traced */
#define	SIDL	5		/* intermediate state in process creation */
#define SONPROC 6		/* process will be run on a processor */
#define	SXBRK	7		/* process being xswapped */
#define	SPARKED	8		/* process hasn't given up it's stack yet */
#define	SUPKERN	9		/* next upkern process to run */


/* flag codes */
#define	SSYS	0x0001		/* system process */
#define	STRC	0x0002		/* process is being traced */
#define	SWTED	0x0004		/* stopped process taken by parent with wait */
#define	SNWAKE	0x0008		/* cannot be woken by a signal */
#define	SLOAD	0x0010		/* in core */
#define	SLOCK	0x0020		/* process cannot be swapped */
#define	SRSIG	0x0040		/* Set when signal goes remote	*/
#define SPOLL	0x0080		/* Process in stream poll	*/
#define SOMASK  0x0100		/* posix; flag used by sigpending and psig */
#define SNOSTOP 0x00008000	/* proc asleep, stop not allowed */
#define SPTRX	0x00010000	/* process is exiting via ptrace(2) */
#define SSIGTRC 0x00020000	/* signal during coproc inst.  trace til end */
				/* of coproc inst then handle signal	*/
#define SBUSTRC 0x00040000	/* took signal in u_buserr: force trace trap */


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

#define	ISSIG(p,why)	((p)->p_cursig || (((p)->p_sig) && issig(why)))

/* Reasons for calling issig() */

#define	FORREAL		0	/* Usual side-effects */
#define	JUSTLOOKING	1	/* Don't stop the process */

/* defines for p_special */
#define PS_CPU_TYPE	0x01
#define PS_FPU_TYPE	0x02
#define PS_SWAPOUT	0x04

#define UPKERN_MAX	0xff	/* maximum value of p_upkern_cnt	*/

/* coprocessor mid instruction exception stack frame type */
#define	COP_MIDINSTR	9

#define NUM_PRIORITIES	128

/*
 *	Defines for p_posix_flags.
 */
/* define SA_NOCLDSTOP	 0x0001	/* in sys/signal.h			*/
/* define SA_CLDSTOP	 0x0002	/* in sys/signal.h			*/
/* define TRACED	 0x0004	/* in sys/wait.h			*/
#define WAS_EXECED	 0x0010 /* have been execed since last fork	*/
#define	MY_CON_TTY	 0x0100	/* the structure in p_my_tty is con_tty	*/
#define	MY_STREAM_TTY	 0x0400	/* the structure in p_my_tty is stream	*/
#define JOB_CONTROL_PROC 0x0800	/* this is a job control process  which
				means that the session of which it is a
				member was started by a POSIX job control
				process */
#define	JOB_SESSION_PROC 0x1000	/* part of session started by setsid()	*/
#define	SVID_COMPAT_PROC 0x2000	/* proc environ is pure SVID compatible	*/
#define F_POSIX_BINARY	 0x8000	/* proc exec'd from posix binary	*/

#define	JOBS_FLAGS_MASK		(JOB_CONTROL_PROC | JOB_SESSION_PROC)
#define	POSIX_FLAGS_MASK	(JOBS_FLAGS_MASK | F_POSIX_BINARY | \
				  SVID_COMPAT_PROC)

#define	IS_POSIX()		(u.u_procp->p_posix_flags & F_POSIX_BINARY)
#define	IS_POSIX_PROC(pp)	((pp)->p_posix_flags & F_POSIX_BINARY)

#define	IS_JOBC()		(u.u_procp->p_posix_flags & JOB_CONTROL_PROC)
#define	IS_JOBC_PROC(pp)	((pp)->p_posix_flags & JOB_CONTROL_PROC)

#define	IS_SVID()		(u.u_procp->p_posix_flags & SVID_COMPAT_PROC)
#define	IS_SVID_PROC(pp)	((pp)->p_posix_flags & SVID_COMPAT_PROC)

#define	IS_SVID_ENV(pp)		((pp)->p_posix_flags & SVID_COMPAT_PROC)
#define	IS_POSIX_ENV(pp)	(!(IS_SVID_ENV(pp)))

#include "sys/pm_iomap.h"

typedef struct {
	proc_t	*rq_head;
	proc_t	*rq_tail;
} RQHASH;

typedef struct {
	uint	rq_cnt;		/* number of procs on rq_hash */
	int	rq_best_pri;	/* index of highest priority active rq_hash */
	uint	rq_map[(NUM_PRIORITIES/UintBits) + 1];	/* pad required */
	RQHASH	rq_hash[NUM_PRIORITIES];
} RUNQ;

#ifdef	INKERNEL
extern	RUNQ	runqs, upkern_runqs;
#endif

#endif /* SYS_PROC_H */
