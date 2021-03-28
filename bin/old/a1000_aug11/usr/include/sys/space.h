#ifndef SYS_SPACE_H
#define SYS_SPACE_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) space.h: version 2.1 created on 5/22/89 at 19:10:47	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)space.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident	"@(#)uts/head/sys:space.h	1.2"

/*
 *	@(#)space.h	2.1
 */
#include	"sys/acct.h"
#include	"sys/tty.h"
#include	"sys/termio.h"
#include	"sys/buf.h"
#include	"sys/file.h"
#include	"sys/inode.h"
#include	"sys/region.h"
#include	"sys/proc.h"
#include	"sys/map.h"
#include	"sys/callo.h"
#include	"sys/mount.h"
#include	"sys/elog.h"
#include	"sys/err.h"
#include	"sys/iobuf.h"
#include	"sys/utsname.h"
#include	"sys/opt.h"
#include	"sys/var.h"
#include	"sys/init.h"
#include	"sys/fcntl.h"
#include	"sys/fstyp.h"
#include	"sys/flock.h"
#include	"sys/tuneable.h"
#include	"sys/nami.h"
#include 	"sys/state.h"
#include 	"sys/open.h"
#include	"sys/fs/s5inode.h"

struct	buf	bfreelist;		/* head of the free list of buffers */
struct	pfree	pfreelist;		/* Head of physio header pool */
struct	buf	pbuf[NPBUF];		/* Physical io header pool */
struct  buf 	bufhdrs[NBUF];		/* buffer headers */
struct	hbuf	hbuf[NHBUF];		/* buffer hash table */
int		mbufmax = MBMAX;	/* max number of mapped buffers */ 
int		bdnosync = BDNOSYNC;	/* no sync flag for block devices */
int		bdflushmax = BDFLUSHMAX;
int		slice_size = MAXSLICE;

struct	file	file[NFILE];		/* file table */

struct	inode	inode[NINODE];		/* inode table */
struct	s5inode	s5inode[NS5INODE];	/* s5inode table */
struct	locklist locklist[NFLOCKS];	/* lock table */
struct flckinfo	flckinfo = {		/* lockf accounting */
	FLCKREC, 0, 0, 0 
};
struct filock	flox[FLCKREC];		/* lockf table */

struct	proc	proc[NPROC];		/* process table */
struct  region	region[NREGION];	/* region descriptor table */

struct	callo	callout[NCALL];
struct	map	sptmap[SPTMAP];
int		v_sptsize = SPTSIZE;

struct	mount	mount[NMOUNT];

struct	srmnt	srmount[NSRMOUNT];
int		nsrmount = {NSRMOUNT};

struct {
	long	shlbs;
	long	shlblnks;
	long	shlbovf;
}
shlbinfo = {
		SHLBMAX,
		0,
		0
};


tune_t	tune = {
		GPGSLO,
		GPGSHI,
		GPGSMSK,
		VHANDR,
		VHANDL,
		MAXSC,
		MAXFC,
		MAXUMEM,
		BDFLUSHR,
		MINARMEM,
		MINASMEM
};


		
struct	err	err = {
	NESLOT
};

cpuint() { panic("cpuint"); }
dmcint() { panic("dmcint"); }

struct	acct	acctbuf;
struct	inode	*acctp;

unsigned short	conmax = 1;		/* conmax = CON_0_DEV; */
char		conline = L_CON;
char		sxtline = L_SXT;
char		gctty_line = L_GCTTY;
struct	cblock	cfree[NCLIST];

unsigned short	condev = C_CON;
unsigned short	gctty_dev = C_GCTTY;

#ifdef	GCTTY_0
#	include	"sys/gc.h"
#else
#	ifdef	GCLP_0
#		include	"sys/gc.h"
#	else
#		ifdef	GCBSC_0
#			include	"sys/gc.h"
#		else
#			ifdef	GCSNA_0
#				include	"sys/gc.h"
#			else
				gc_init(){return(0);}
#			endif
#		endif
#	endif
#endif

#ifdef	GCTTY_0
#	include	"gcp/sema.h"
#	include	"gcp/pto.h"
#	include	"sys/gctio.h"
#	include	"sys/gctty.h"

#	include	"sys/gctty_map.h"
	struct	gctty_map	gctty_map[GCTTY_0];

	unsigned short	gctty_max = GCTTY_0_DEV;
	struct	gctty	*gctty_tty[GCTTY_0_DEV];
	short	gctty_pgrp[GCTTY_0_DEV];
#ifdef	GCATTY_0
#	include	"sys/gcatty.h"
	unsigned short	gcatty_max = GCATTY_0_DEV;
	struct	gcatty	gcatty_tty[GCATTY_0_DEV];
#else
	gcaintr(){}
#endif
#else
	int	*	gctty_tty = (int *)NULL;
	int		gctty_pgrp = (int *)NULL;
	gctty_initialize(){}
	gctty_interrupt(){}
	gctty_reset(){}
#endif

#ifdef	GCLP_0
#	include	"sys/lprio.h"
#	include	"sys/gclp.h"
	unsigned short	gclp_max = GCLP_0_DEV;
	struct	gclp	*gclp_lp[GCLP_0_DEV];
#else
	gclp_initialize(){}
	gclp_interrupt(){}
	gclp_reset(){}
#endif

#ifdef	GCBSC_0
#	include	"sys/gcbsc.h"
	unsigned short	gcbsc_max = GCBSC_0_DEV;

#	ifdef	GCBSCT_0
		unsigned short	gcbsct_max = GCBSCT_0_DEV;
#	endif

#	ifdef	GCBSCM_0
		unsigned short	gcbscm_max = GCBSCM_0_DEV;
#	endif

#else
	gcbsc_initialize(){}
	gcbsc_interrupt(){}
	gcbsc_reset(){}
#endif

#ifdef	GCSNA_0
#	include	"sys/gcsna.h"
	unsigned short	gcsna_max = GCSNA_0_DEV;

#	ifdef	GCSNAT_0
		unsigned short	gcsnat_max = GCSNAT_0_DEV;
#	endif


#else
	gcsna_initialize(){}
	gcsna_interrupt(){}
	gcsna_reset(){}
#endif

#ifdef	GCSDLC_0
		unsigned short	gcsdlc_max = GCSDLC_0_DEV;

#	ifdef	GCSDLCT_0
		unsigned short	gcsdlct_max = GCSDLCT_0_DEV;
#	endif
#else
	gcsdlc_interrupt(){}
	gcsdlc_initialize(){}
	gcsdlc_reset(){}
#endif


#ifdef	GCTTM_0
	unsigned short	gcttm_dev = C_GCTTM;
	unsigned short	gcttm_max = GCTTM_0_DEV;
	struct	gctty	*gcttm_ttm[GCTTM_0_DEV];
	struct	cir_buf	*gcmuxt_muxt[GCMUXT_0_DEV];
	short	gcttm_pgrp[GCTTM_0_DEV];
#else
	int	*	gcttm_ttm = (int *)NULL;
	gcttm_initialize(){}
	gcttm_interrupt(){}
	gcttm_reset(){}
#endif





#ifdef SXT_0
#	include	"sys/sxt.h"
	unsigned short	sxtmax = SXT_0 * SXT_0_PHYS;
	struct Link	sxtlink[SXT_0 * SXT_0_PHYS] = {0};
#endif

/*
*	Allocate driver specific code, including drive info tables
*       queue flags, and queue heads
*
*/

#ifdef SDSK1_0
#	define SDSK_CNT	4
	unsigned short	scsimax = SDSK0_0_PHYS;
	unsigned short	scsi_cnt = SDSK0_0_PHYS * SDSK_CNT;
	struct	psd_info sdrv_info[SDSK_CNT * SDSK0_0_PHYS];
	struct	iobuf	sdktab[SDSK_CNT];
	unsigned char	sdk_queue_flag[SDSK0_0_PHYS];
	opentyp_t	sdk_opentyp[SDSK_CNT * SDSK0_0_DEV];
#endif

#ifdef DISK_0
	unsigned short	dkmax = DISK_0_PHYS;
	unsigned short	dk_cnt = DISK_0_PHYS * DISK_0; 
	struct	psd_info dk_info[DISK_0 * DISK_0_PHYS];
	unsigned int 	dk_queue_flag[DISK_0];
	struct	iobuf	dktab[DISK_0 * DISK_0_PHYS];
	opentyp_t	dk_opentyp[DISK_0 * DISK_0_PHYS];
#endif


/* Now define the disk status structure for all the possible disks
*  that will exist in the system. Also declar variables pdrvcnt (physical
*  drive count) and cntrlcnt (controller count) for reference. dkstat_size
*  will always contain the total count of elements in dkstat. Note
*  that dkstat is alway allocated to largest possibility!
*/

#ifdef SDSK0_0
	unsigned short	physcnt = SDSK0_0_PHYS;
	struct	iotime	dkstat[SDSK_CNT * SDSK0_0_PHYS];
#else
	unsigned short  physcnt = DISK_0_PHYS;
	struct	iotime	dkstat[DISK_0 * DISK_0_PHYS];
#endif

#ifdef MIRROR_0
#	ifndef SYS_BUF_H
#	include "sys/buf.h"
#	endif

#	ifndef SYS_VD_H
#	include "sys/vd.h"
#	endif

buf_t		mrfreelist;
buf_t		mrbufhdrs[NMRBUF];
uint		num_mr_bufs = NMRBUF;
mirror_t	mr_slices[MAX_MR_SLICES];
uint		max_mr_slices = MAX_MR_SLICES;
uint		mr_lock[MAX_MR_SLICES];
unsigned int	mrfreelist_sem, mr_iodone_sem;
#endif /* MIRROR_0 */

#ifdef VD_0
#	ifndef SYS_BUF_H
#	include "sys/buf.h"
#	endif

#	ifndef SYS_VD_H
#	include "sys/vd.h"
#	endif

vd_t		virt_slices[MAX_VIRT_DISKS];
uint		max_virt_slices = MAX_VIRT_DISKS;
buf_t		vdfreelist, vdbufhdrs[NVBUF];
uint		vd_lock[MAX_VIRT_DISKS];
uint		num_vd_bufs = NVBUF;
uint		vdfreelist_sem;
#endif /* VD_0 */

#ifdef TAPE_0
	unsigned short	mtmax = TAPE_0_DEV;
	struct	iobuf	mttab[TAPE_0 * TAPE_0_PHYS];
	int		mt_cnt = TAPE_0 * TAPE_0_PHYS;
	struct	iotime	mtstat[TAPE_0 * TAPE_0_PHYS];
#endif

#ifdef NINETRK_0
	unsigned short	ntmax = NINETRK_0_DEV;
	struct	iobuf	nttab[NINETRK_0 * NINETRK_0_PHYS];
	int		nt_cnt = NINETRK_0 * NINETRK_0_PHYS;
	struct	iotime	ntstat[NINETRK_0 * NINETRK_0_PHYS];
#endif

#ifdef TRACE_0
#	include	"sys/trace.h"
	struct	trace	trace[TRACE_0 * TRACE_0_PHYS];
#endif

#ifdef MESG
#	include	"sys/ipc.h"
#	include	"sys/msg.h"
	struct	map	msgmap[MSGMAP];
	struct	msqid_ds msgque[MSGMNI];
	struct	msg	msgh[MSGTQL];
	char	msglock[MSGMNI];
	unsigned long     msg_lock_wakeup[MSGMNI];
	struct	msginfo	msginfo = {
		MSGMAP, MSGMAX, MSGMNB, MSGMNI, MSGSSZ, MSGTQL, MSGSEG
	};
	char	msgbuf[MSGSEG * MSGSSZ]; 	/* kernel message buffers */
	char	*msg;
#endif

#ifdef SEMA
#	ifndef SYS_IPC_H
#		include	"sys/ipc.h"
#	endif
#	include	"sys/sem.h"
	struct	semid_ds sema[SEMMNI];
	struct	sem	sem[SEMMNS];
	struct	map	semmap[SEMMAP];
	struct	sem_undo *sem_undo[NPROC];
#	define	SEMUSZ	(sizeof(struct sem_undo)+sizeof(struct undo)*SEMUME)
	int		semu[ ( ( SEMUSZ * SEMMNU ) + NBPW - 1 ) / NBPW ];
	union {
		short		semvals[SEMMSL];
		struct	semid_ds ds;
		struct	sembuf	semops[SEMOPM];
	} semtmp;

	struct	seminfo	seminfo = {
		SEMMAP, SEMMNI, SEMMNS, SEMMNU, SEMMSL,
		SEMOPM, SEMUME, SEMUSZ, SEMVMX, SEMAEM
	};
#endif

#ifdef SHMEM
#	ifndef SYS_IPC_H
#		include	"sys/ipc.h"
#	endif
#	include	"sys/shm.h"
	struct	shmid_ds *shm_shmem[NPROC*SHMSEG];
	struct	shmid_ds shmem[SHMMNI];	
	struct	shmpt_ds shm_ppindex[NPROC*SHMSEG];
	struct	map 	shm_map[SHMMNI*2] = {mapdata(SHMMNI*2)};
	pde_t		 shm_mmu[SHMALL*2];	/* bigger for fragmentation */
	char	shm_lock[SHMMNI];
	struct	shminfo	shminfo = {
		SHMMAX, SHMMIN, SHMMNI, SHMSEG, SHMBRK, SHMALL
	};
#endif

#ifndef	MAC_0
#define MAC_0_DEV	0
	unsigned int	macmax = 0;
	int	*	mac_slot = (int *)NULL;
	int	*	mac_ptr = (int *)NULL;
	int mac_busy;
struct mac_slt {			/* Device to MAC array */
	int enth;			/* Which instance of a MAC */
	int intrpt;			/* Interrupt flag */
	struct icb *icb;			/* Icb ptr to a MAC */
	struct mac *mac;			/* Mac ptr to a MAC */
} mac_slt[1];

	macint(){return(0);}
	macinit(){return(0);}
#else
#	include	"sys/mac.h"
	unsigned int	macmax = MAC_0_DEV;
	struct	icb	*mac_slot[MAC_0_DEV]=0;
	struct	mac	*mac_ptr[MAC_0_DEV]=0;
	int mac_busy[MAC_0_DEV];		/* Local device busy array */

struct mac_slt {			/* Device to MAC array */
	int enth;			/* Which instance of a MAC */
	int intrpt;			/* Interrupt flag */
	struct icb *icb;			/* Icb ptr to a MAC */
	struct mac *mac;			/* Mac ptr to a MAC */
} mac_slt[MAC_0_DEV];

#endif

#ifdef PRF_0
#	include	"sys/profiler.h"
	unsigned short	prfmax = PRF_0_DEV;
	struct	prfargs	prf_ptr[PRF_0_DEV];
#else
	prfintr(){}
#endif

/* non aligned tape io */
char 	non_aligned_buf[NON_ALIGNED_SIZE];
int 	non_aligned_size = NON_ALIGNED_SIZE;
short 	non_aligned_kern_map[(NON_ALIGNED_SIZE/4096)+2];
short 	non_aligned_user_map[(NON_ALIGNED_SIZE/4096)+2];

/* The following stuff is for streams.
 */

queue_t queue[NQUEUE];			/* table of queues */
struct stdata streams[NSTREAM];		/* table of streams */
mblk_t mblock[		 		/* table of msg blk desc */
	NBLK4096+NBLK2048+NBLK1024+NBLK512+NBLK256+NBLK128+NBLK64+NBLK16+NBLK4
        + ((NBLK4096+NBLK2048+NBLK1024+NBLK512+NBLK256+NBLK128
	   +NBLK64+NBLK16+NBLK4)/4) ];

dblk_t dblock[	 	/* table of data blk desc */
	NBLK4096+NBLK2048+NBLK1024+NBLK512+NBLK256+NBLK128+NBLK64+NBLK16+NBLK4];

int	strbase[NBLK4096*1024 + NBLK2048*512 + NBLK1024*256 + NBLK512*128
		+ NBLK256*64 + NBLK128*32 + NBLK64*16 + NBLK16*4 + NBLK4];

struct linkblk linkblk[NMUXLINK];	/* multiplexor link table */

int	nmuxlink ={NMUXLINK};		/* number of multiplexor links */
int	nstrpush  ={NSTRPUSH};		/* maxmimum number of pushes allowed */
struct strevent strevent[NSTREVENT];	/* table of stream event cells */
int	nstrevent  ={NSTREVENT};	/* initial number stream event cells */
int	maxsepgcnt  ={MAXSEPGCNT};	/* page limit for event cell alloc */
int	strmsgsz  ={STRMSGSZ};		/* maximum stream message size */
int	strctlsz  ={STRCTLSZ};		/* max size of ctl part of message */
char	strlofrac  ={STRLOFRAC};
char	strmedfrac  ={STRMEDFRAC};
int	nmblock ={			/* number of msg blk desc */
	 NBLK4096+NBLK2048+NBLK1024+NBLK512+NBLK256+NBLK128+NBLK64+NBLK16+NBLK4
	 + ((NBLK4096+NBLK2048+NBLK1024+NBLK512+NBLK256
	    +NBLK128+NBLK64+NBLK16+NBLK4)/4)};

struct	var	v = {
	NBUF,
	NCALL,
	NINODE,
	(char *)(&inode[NINODE]),
	NFILE,
	(char *)(&file[NFILE]),
	NMOUNT,
	(char *)(&mount[NMOUNT]),
	NPROC,
	(char *)(&proc[1]),
	NREGION,
	NCLIST,
	MAXUP,
	NHBUF,
	NHBUF-1,
	NPBUF,
	SPTMAP,
	VHNDFRAC,
	0,
	NAUTOUP,
	NOFILES,

	NQUEUE,
	NSTREAM,
	NBLK4096,
	NBLK2048,
	NBLK1024,
	NBLK512,
	NBLK256,
	NBLK128,
	NBLK64,
	NBLK16,
	NBLK4,
	CDLIMIT_CONF,
	NS5INODE,
	MAC_0_DEV,
	NFLOCKS
};


short   ipid[NINODE];

/* 
 * file system related
 * will be generated dynamically later
 */

/* 0*/	extern int		(s5init)();
/* 1*/	extern int		(s5iput)();
/* 2*/	extern struct inode	*(s5iread)();
/* 4*/	extern int		(s5iupdat)();
/* 5*/	extern int		(s5readi)();
/* 6*/	extern int		(s5writei)();
/* 7*/	extern int		(s5itrunc)();
/* 8*/	extern int		(s5statf)();
/* 9*/	extern int		(s5namei)();
/*10*/	extern int		(s5mount)();
/*11*/	extern int		(s5umount)();
/*12*/	extern struct inode	*(s5getinode)();
/*13*/	extern int		(s5openi)();	/* open inode */
/*14*/	extern int		(s5closei)();	/* close inode */
/*15*/	extern int		(s5update)();	/* update */
/*16*/	extern int		(s5statfs)();	/* statfs and ustat */
/*17*/	extern int		(s5access)();
/*18*/	extern int		(s5getdents)();
/*19*/	extern int		(s5allocmap)();	/* Let the fs decide if */
						/* if can build a map so */
						/* this fs can be used for */
						/* paging */
/*20*/	extern int		(s5freemap)();	/* free block list */
/*21*/	extern int		(s5readmap)();	/* read a page from the fs */
						/* using the block list */
/*22*/	extern int		(s5setattr)();	/* set attributes */
/*23*/	extern int		(s5notify)();	/* notify fs of action */
/*24*/	extern int		(s5fcntl)();	/* fcntl */
/*25*/	extern int		(s5fsinfo)();	/* additional info */
/*26*/	extern int		(s5ioctl)();	/* ioctl */

#if RFS
/* declarations/definitions for RFS
 */

extern int		(duinit)();
extern int		(duiput)();
extern struct inode	*(duiread)();
extern int		(duiupdat)();
extern int		(dureadi)();
extern int		(duwritei)();
extern int		(duitrunc)();
extern int		(dustatf)();
extern int		(dumount)();
extern int		(duopeni)();
extern int		(duclosei)();
extern int		(duupdate)();
extern int		(duaccess)();
extern int		(dugetdents)();
extern int		(dufreemap)();
extern int		(dunotify)();
extern int		(dufcntl)();
extern int		(duioctl)();
#endif	/* RFS */
#if NFS

extern int		(nfsinit)();
extern int		(nfsiput)();
extern struct inode	*(nfsiread)();
extern int		(nfsiupdat)();
extern int		(nfsreadi)();
extern int		(nfswritei)();
extern int		(nfsitrunc)();
extern int		(nfsstatf)();
extern int		(nfsnamei)();
extern int		(nfsmount)();
extern int		(nfsumount)();
extern int		(nfsopeni)();
extern int		(nfsclosei)();
extern int		(nfsupdate)();
extern int		(nfsstatfs)();
extern int		(nfsaccess)();
extern int		(nfsgetdents)();
extern int		(nfsallocmap)();
extern int		(nfsfreemap)();
extern int		(nfsreadmap)();
extern int		(nfssetattr)();
extern int		(nfsnotify)();
extern int		(nfsfcntl)();
extern int		(nfsioctl)();
#endif	/* NFS */

extern int		fsstray();	/* unused one */
extern int *		fsistray();	/* unused one */
extern struct inode *	fsinstray();	/* unused one */

struct sp_sp { long a,b; } sp_sp[NSTREAM];
int	spcnt = {NSTREAM};

/* file system info table
 */
struct fsinfo fsinfo[] = {			/* fstyp never == 0 */
	{0, (struct mount *)0, (char *)0, 0},
	{0, (struct mount *)0, "S51K", NO_SEEK},
#if RFS
	{FS_NOICACHE, (struct mount *)0, "S5DU", NO_SEEK},
#endif
#if	NFS
	{FS_NOICACHE, (struct mount *)0, "NFS", NO_IGET},
#endif
};

/* stream log device	*/
#include "sys/log.h"

struct ilog {
	struct inode	*il_addr;
	struct inode	*il_forw;
	struct inode	*il_back;
	struct inode	*il_avforw;
	struct inode	*il_avback;
	int		*il_fsptr;
	long		il_number;
	struct rcvd	*il_rcvd;
	short		il_fstyp;
	dev_t		il_dev;
	cnt_t		il_count;
	short		il_op;
	int		*il_caller;
};

int ilogsize = ILOGSIZE;
struct ilog ilogs[ILOGSIZE];

struct log log_log[NUM_LOGS];	/* sad device state table */
int log_cnt = NUM_LOGS;		/* number of configured minor devices */

/* JTOF LOG_BSZ must always be smaller than NMBLK128 */
int log_bsz = LOG_BSZ;		/* size of internal buffer of log messages */

/*
 * file system switch table
 */
struct fstypsw fstypsw[] = {
		{
			fsstray,
			fsstray,
			fsinstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsinstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
		},
		{	s5init,
			s5iput,
			s5iread,
			fsstray,
			s5iupdat,
			s5readi,
			s5writei,
			s5itrunc,
			s5statf,
			s5namei,
			s5mount,
			s5umount,
			s5getinode,
			s5openi,
			s5closei,
			s5update,
			s5statfs,
			s5access,
			s5getdents,
			s5allocmap,
			s5freemap,
			s5readmap,
			s5setattr,
			s5notify,
			s5fcntl,
			fsstray,
			s5ioctl,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
		},
#if RFS
		{	duinit,
			duiput,
			duiread,
			fsstray,
			duiupdat,
			dureadi,
			duwritei,
			duitrunc,
			dustatf,
			fsstray,
			dumount,
			fsstray,
			fsinstray,
			duopeni,
			duclosei,
			duupdate,
			fsstray,
			duaccess,
			dugetdents,
			fsstray,
			dufreemap,
			fsstray,
			fsstray,
			dunotify,
			dufcntl,
			fsstray,
			duioctl,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
		},
#endif	/* RFS */
#if	NFS
		{	nfsinit,
			nfsiput,
			nfsiread,
			fsstray,
			nfsiupdat,
			nfsreadi,
			nfswritei,
			nfsitrunc,
			nfsstatf,
			nfsnamei,
			nfsmount,
			nfsumount,
			fsinstray,
			nfsopeni,
			nfsclosei,
			nfsupdate,
			nfsstatfs,
			nfsaccess,
			nfsgetdents,
			nfsallocmap,
			nfsfreemap,
			nfsreadmap,
			nfssetattr,
			nfsnotify,
			nfsfcntl,
			fsstray,
			nfsioctl,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
			fsstray,
		}
#endif	/* NFS */
};

short	nfstyp = { (sizeof fstypsw)/sizeof(fstypsw[0]) };

/*
 * stream module switch table
 */

extern struct streamtab timinfo;
extern struct streamtab trwinfo;
extern struct streamtab pteminfo;
extern struct streamtab ldterminfo;
#ifdef	GIL_LDTERM
extern struct streamtab	tym_info;
extern struct streamtab	tyd_info;
extern struct streamtab	msq_info;
extern struct streamtab	raw_info;
#endif	/* GIL_LDTERM */

struct fmodsw fmodsw[] = {
	"timod", &timinfo,
	"tirdwr", &trwinfo,
	"ptem", &pteminfo,
	"ldterm", &ldterminfo,
#ifdef	GIL_LDTERM
	"tym_",   &tym_info,
	"msq_",   &msq_info,
	"tyd_",   &tyd_info,
	"raw_",   &raw_info,
#endif	/* GIL_LDTERM */
};

#ifdef	GIL_LDTERM
int	fmodcnt = {8};
#else	/* GIL_LDTERM */
int	fmodcnt = {4};
#endif	/* GIL_LDTERM */

/* dummy config defs for stream modules */
struct tim_tim { long a,b,c; } tim_tim[NSTREAM];
int	tim_cnt = {NSTREAM};

struct trw_trw { long a,b,c; } trw_trw[NSTREAM];
int	trw_cnt = {NSTREAM};

int	nservers  ={ 0 };	/* must be always declared	*/
int	idleserver  ={ 0 };	/* used as stubs when RFS	*/
int	msglistcnt  ={ 0 };	/* is not present		*/

/*
 * miscellaneous RFS declarations
 */
#if RFS
#include "sys/message.h"
#include "sys/adv.h"
#include "sys/nserve.h"
#include "sys/sema.h"
#include "sys/cirmgr.h"
#include "sys/comm.h"

struct advertise advertise[NADVERTISE];
int	nadvertise ={ NADVERTISE };
struct rcvd	rcvd[NRCVD];
int	nrcvd ={ NRCVD };
struct sndd	sndd [NSNDD];
int	nsndd ={ NSNDD };
int	maxgdp = { MAXGDP };
struct gdp	gdp [MAXGDP];
int	minserve ={ MINSERVE };
int	maxserve ={ MAXSERVE };
int	nrduser ={ NRDUSER };
struct rd_user	rd_user[NRDUSER];
char	rfheap[RFHEAP];
int	rfsize ={ RFHEAP };
int	rfs_vhigh = { RFS_VHIGH };
int	rfs_vlow = { RFS_VLOW };
struct	response srv_rsps[MAXSERVE];		/* server response buffers */

/*
 * RFS cache parms
 * NREMOTE and NLOCAL default to one third of NBUF when set to zero.
 * Non-zero value overrides the default.
 */
#define RCACHETIME 10
#define NREMOTE	 0
#define NLOCAL   0

int	nremote  = { NREMOTE };
int	nlocal  ={ NLOCAL };
struct buf rhbuf[NHBUF];
int	nrhbuf  ={ NHBUF };
int	rhmask  ={ NHBUF-1 };
int	rc_time  ={ RCACHETIME };
int		rcache_enable;
unsigned long	rfs_vcode;
unsigned long	rbuf_ct;
unsigned long	lbuf_ct;
unsigned long	nlbuf;
unsigned long	nrbuf;
unsigned long	rcache_time;
unsigned long	maxbufage;
int		rcacheinit;

#endif	/* RFS */
#if NFS
uint nfs_rnum = NFS_RNUM;

#endif /* NFS */

int	slave_died = 0;
int	slave_cpu = 0;
int	slave_vaddr = 0;
int	slave_fault = 0;
struct	state slave_dump;

