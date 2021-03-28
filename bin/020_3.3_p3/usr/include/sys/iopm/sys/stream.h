/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) stream.h: version 24.1 created on 10/28/91 at 18:00:12	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)stream.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef IOPM_STREAM_H
#define IOPM_STREAM_H

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#ifndef SYS_SYNCH_H
#include "sys/synch.h"
#endif

/*
 * data queue
 */
struct	iqueue {
	struct qinit	*q_qinfo;	/* procs and limits for queue */
	struct msgb	*q_first;	/* first data block */
	struct msgb	*q_last;	/* last data block */
	struct iqueue	*q_next;	/* Q of next stream */
	struct iqueue	*q_link;	/* to next Q for scheduling */
	caddr_t          q_ptr;		/* to private data structure */
	ushort           q_count;	/* number of blocks on Q */
	ushort           q_flag;	/* queue state */
	short            q_minpsz;	/* min msg size accepted by this mod */
	short            q_maxpsz;	/* max msg size accepted by this mod */
	ushort           q_hiwat;	/* queue high water mark */
	ushort           q_lowat;	/* queue low water mark */
	struct sem_lock  q_lock;	/* not used. FIX CMW remove @ maj rel */
};

typedef struct iqueue iqueue_t;

#if !defined IOPM_OS
#define queue iqueue
#define queue_t iqueue_t
#endif

#ifndef SYS_STREAM_H
/*
 * Queue flags
 */
#define	QENAB	01			/* Queue is already enabled to run */
#define	QWANTR	02			/* Someone wants to read Q */
#define	QWANTW	04			/* Someone wants to write Q */
#define	QFULL	010			/* Q is considered full */
#define	QREADR	020			/* This is the reader (first) Q */
#define	QUSE	040			/* This queue in use (allocation) */
#define	QNOENB	0100			/* Don't enable Q via putq */
#define QREMOTE	0x800		/*iop*/	/* queue continues on another board */
#endif


#ifndef SYS_STREAM_H
/*
 * module information structure
 */
struct module_info {
	ushort	mi_idnum;		/* module id number */
	char 	*mi_idname;		/* module name */
	short   mi_minpsz;		/* min packet size accepted */
	short   mi_maxpsz;		/* max packet size accepted */
	ushort	mi_hiwat;		/* hi-water mark */
	ushort 	mi_lowat;		/* lo-water mark */
};


/*
 * queue information structure
 */
struct	qinit {
	int	(*qi_putp)();		/* put procedure */
	int	(*qi_srvp)();		/* service procedure */
	int	(*qi_qopen)();		/* called on startup */
	int	(*qi_qclose)();		/* called on finish */
	int	(*qi_qadmin)();		/* for 3bnet only */
	struct module_info *qi_minfo;	/* module information structure */
	struct module_stat *qi_mstat;	/* module statistics structure */
};


/*
 * Streamtab (used in cdevsw and fmodsw to point to module or driver)
 */

struct streamtab {
	struct qinit *st_rdinit;
	struct qinit *st_wrinit;
	struct qinit *st_muxrinit;
	struct qinit *st_muxwinit;
};
#endif

/* 
 * structure for storing triples of mux'ed streams 
 */
struct ilinkblk {
	iqueue_t *l_qtop;	/* lowest level write queue of upper stream */
	iqueue_t *l_qbot;	/* highest level write queue of lower stream */
	int      l_index;	/* file[] index for lower stream. */
};

#if !defined IOPM_OS
#define linkblk  ilinkblk
#endif

#ifndef SYS_STREAM_H
/*
 * Structure of list of processes to be sent SIGSEL signal
 * on request, or for processes sleeping on select().  The valid 
 * SIGSEL events are defined in stropts.h, and the valid select()
 * events are defined in select.h.
 */
struct strevent {
	union {
		struct {
			struct proc 	*procp;
			long 		events;
			} e;
		struct {
			int (*func)();
			long arg;
			} b;
	} x;
	struct strevent *se_next;
};

#define se_procp x.e.procp
#define se_events x.e.events
#define se_func x.b.func
#define se_arg x.b.arg

#define SE_SLEEP	0	/* ok to sleep in allocation */
#define SE_NOSLP	1	/* don't sleep in allocation */
#endif


#ifndef SYS_STREAM_H
/*
 *  Data block descriptor
 */
struct datab {
	struct datab	*db_freep;
	unsigned char	*db_base;
	unsigned char	*db_lim;
	unsigned char	db_ref;
	unsigned char	db_type;
	unsigned char	db_class;
};


/*
 * Message block descriptor
 */
struct	msgb {
	struct	msgb	*b_next;
	struct  msgb	*b_prev;
	struct	msgb	*b_cont;
	unsigned char	*b_rptr;
	unsigned char	*b_wptr;
	struct datab 	*b_datap;
};

typedef struct msgb mblk_t;
typedef struct datab dblk_t;


/*
 * Data block allocation information.  Defines cutoffs for allocation 
 * priorities; bufcall lists. 
 */

struct dbalcst {
	int dba_cnt;
	int dba_lo;
	int dba_med;
	struct strevent *dba_lop;
	struct strevent *dba_medp;
	struct strevent *dba_hip;
};
#endif



#ifndef SYS_STREAM_H
/************************************************************************/
/* 			Streams message types				*/
/************************************************************************/


/*
 * Data and protocol messages (regular priority)
 */
#define	M_DATA		00		/* regular data */
#define M_PROTO		01		/* protocol control */

/*
 * Control messages (regular priority)
 */
#define	M_BREAK		010		/* line break */
#define M_PASSFP	011		/* pass file pointer */
#define	M_SIG		013		/* generate process signal */
#define	M_DELAY		014		/* real-time xmit delay (1 param) */
#define M_CTL		015		/* device-specific control message */
#define	M_IOCTL		016		/* ioctl; set/get params */
#define M_SETOPTS	020		/* set various stream head options */
#define M_RSE		021		/* reserved for RSE use only */


/*
 * Control messages (high priority; go to head of queue)
 */
#define	M_IOCACK	0201		/* acknowledge ioctl */
#define	M_IOCNAK	0202		/* negative ioctl acknowledge */
#define M_PCPROTO	0203		/* priority proto message */
#define	M_PCSIG		0204		/* generate process signal */
#define	M_READ		0205		/* generate read notification */
#define	M_FLUSH		0206		/* flush your queues */
#define	M_STOP		0207		/* stop transmission immediately */
#define	M_START		0210		/* restart transmission after stop */
#define	M_HANGUP	0211		/* line disconnect */
#define M_ERROR		0212		/* fatal error used to set u.u_error */
#define M_COPYIN	0213		/* request to copin data */
#define M_COPYOUT	0214		/* request to copyout data */
#define M_IOCDATA	0215		/* response to M_COPYIN and M_COPYOUT */
#define M_PCRSE		0216		/* reserved for RSE use only */
/**iop*/
#define M_KILL		0xa0		/* kernel to IOPM kill sleeping task */
#define M_OPEN		0xa1		/* open control block for IOPM */
#define M_CLOSE		0xa2		/* close control block for IOPM */
#define M_READREQ	0xa3		/* IOPM to kernel req for read mblk */
/*iop**/


/*
 * Queue message class definitions.  
 */
#define QNORM    0			/* normal messages */
#define QPCTL 0200			/* priority cntrl messages */
#endif



#ifndef SYS_STREAM_H
/*
 *  IOCTL structure - this structure is the format of the M_IOCTL message type.
 */
struct iocblk {
	int 	ioc_cmd;		/* ioctl command type */
	ushort	ioc_uid;		/* effective uid of user */
	ushort	ioc_gid;		/* effective gid of user */
	uint	ioc_id;			/* ioctl id */
	uint	ioc_count;		/* count of bytes in data field */
	int	ioc_error;		/* error code */
	int	ioc_rval;		/* return value  */
};

/*
 * structure for the M_COPYIN and M_COPYOUT message types.
 */

struct copyreq {
	int	cq_cmd;			/* ioctl command (from ioc_cmd) */
	ushort	cq_uid;			/* effective uid of user */
	ushort	cq_gid;			/* effective gid of user */
	uint	cq_id;			/* ioctl id (from ioc_id) */
	caddr_t	cq_addr;		/* address to copy data to/from */
	uint	cq_size;		/* number of bytes to copy */
	int	cq_flag;		/* see below */
	mblk_t *cq_private;		/* privtate state information */
};

/* cq_flag values */

#define STRCANON	0x01		/* b_cont data block contains canonical
					   format specifier */
#define RECOPY		0x02		/* perform I_STR copyin again, this time
					   using canonical format specifier */


/*
 * structure for the M_IOCDATA message type.
 */

struct copyresp {
	int	cp_cmd;			/* ioctl command (from ioc_cmd) */
	ushort	cp_uid;			/* effective uid of user */
	ushort	cp_gid;			/* effective gid of user */
	uint	cp_id;			/* ioctl id (from ioc_id) */
	caddr_t	cp_rval;		/* status of request: 0 -> success
							      non-zero -> failure */
	uint	cp_pad1;		/* reserved */
	int	cp_pad2;		/* reserved */
	mblk_t *cp_private;		/* private state information */
};

/*
 * Options structure for M_SETOPTS message.  This is sent upstream
 * by driver to set stream head options.
 */
struct stroptions {
	short so_flags;		/* options to set */
	short so_readopt;	/* read option */
	ushort so_wroff;	/* write offset */
	short so_minpsz;	/* minimum read packet size */
	short so_maxpsz;	/* maximum read packet size */
	ushort so_hiwat;	/* read queue high water mark */
	ushort so_lowat;	/* read queue low water mark */
};

/* flags for stream options set message */

#define SO_ALL		  077	/* set all options */
#define SO_READOPT	   01	/* set read opttion */
#define SO_WROFF	   02	/* set write offset */
#define SO_MINPSZ	   04	/* set min packet size */
#define SO_MAXPSZ	  010	/* set max packet size */
#define SO_HIWAT	  020	/* set high water mark */
#define SO_LOWAT	  040	/* set low water mark */
#define SO_MREADON       0100	/* set read notification ON */
#define SO_MREADOFF      0200	/* set read notification OFF */
#define SO_NDELON	 0400	/* old TTY semantics for NDELAY rd and wr */
#define SO_NDELOFF      01000	/* old TTY semantics for NDELAY rd and wr */
#ifdef _POSIX_SOURCE 
#define	SO_TOSTOPON	02000	/* set TOSTOP option.  used n strwrite */
#define	SO_TOSTOPOFF	04000	/* set TOSTOP option.  used n strwrite */
#endif
#endif




#ifndef SYS_STREAM_H
/************************************************************************/
/*		Miscellaneous parameters and flags			*/
/************************************************************************/

/*
 * Default timeout in seconds for ioctls and close
 */
#define STRTIMOUT 15

/*
 * Stream head default high/low water marks 
 */
#define STRHIGH 5120
#define STRLOW	1024

/*
 * sleep priorities for stream io
 */
#define	STIPRI	PZERO+3
#define	STOPRI	PZERO+3

/*
 * Block allocation parameters
 */
#define NCLASS 9			/* number of block classes */
#define QBSIZE		65		/* min size for block allocation retries */
#define MAXBSIZE	4096		/* max block size */
#define MAXIOCBSZ	1024		/* max ioctl data block size */

/*
 * Values for stream flag in open to indicate module open, clone open;
 * return value for failure.
 */
#define MODOPEN 	0x1		/* open as a module */
#define CLONEOPEN	0x2		/* open for clone, pick own minor device */
#define OPENFAIL	-1		/* returned for open failure */

/*
 * Priority definitions for block allocation.
 */
#define BPRI_LO		1
#define BPRI_MED	2
#define BPRI_HI		3

#define pri_ok(pri) ((pri) == BPRI_LO || (pri) == BPRI_MED || (pri) == BPRI_HI)
/*
 * Value for packet size that denotes infinity
 */
#define INFPSZ		-1

/*
 * Flags for flushq()
 */
#define FLUSHALL	1	/* flush all messages */
#define FLUSHDATA	0	/* don't flush control messages */

/*
 * flag for transparent ioctls
 */
#define TRANSPARENT	(unsigned int)(-1)

/*
 * canonical structure definitions
 */

#define STRLINK		"lli"
#define STRIOCTL	"iiil"
#define STRPEEK		"iiliill"
#define STRFDINSERT	"iiliillii"
#define STRRECVFD	"lssc8"
#define STRNAME		"c0"
#define STRINT		"i"
#define STRTERMIO	"ssssc12"
#define STRTERMCB	"c6"
#define STRSGTTYB	"c4i"
#endif

#ifndef SYS_STREAM_H
/************************************************************************/
/*	Defintions of Streams macros and function interfaces.		*/
/************************************************************************/

/*
 * determine block allocation cutoff for given class and priority.
 */
#define bcmax(class, pri) ( (pri) == BPRI_LO ? dballoc[class].dba_lo : \
			   ((pri) == BPRI_HI ? nmblock : dballoc[class].dba_med))

#if !defined IOPM_OS
/*
 *  Queue scheduling macros
 */
#define setqsched()     qrunflag = 1	/* set up queue scheduler */
#define qready()	qrunflag	/* test if queues are ready to run */
#endif

/*
 * canenable - check if queue can be enabled by putq().
 */
#define canenable(q)	!((q)->q_flag & QNOENB)

/*
 * put a message of the next queue of the given queue
 */
#define putnext(q, mp)	((*q->q_next->q_qinfo->qi_putp)(q->q_next, mp))

/*
 * Test if data block type is one of the data messages (i.e. not a control
 * message).
 */
#define datamsg(type) (type == M_DATA || type == M_PROTO || type == M_PCPROTO)

/*
 * extract queue class of message block
 */
#define queclass(bp) (bp->b_datap->db_type & QPCTL)

/*
 * Align address on next lower word boundary
 */
#define straln(a)	(caddr_t)((long)(a) & ~(sizeof(int)-1))

#define	strbcpy(s, d, c)	bcopy(s, d, c)
#endif

#if defined IOPM
/*
 * declarations of common routines
 */
extern mblk_t *rmvb();
extern mblk_t *dupmsg();
extern mblk_t *copymsg();
extern mblk_t *allocb();
extern mblk_t *unlinkb();
extern mblk_t *dupb();
extern mblk_t *copyb();
extern mblk_t *getq();
extern int    putq();
extern iqueue_t *backq();
extern iqueue_t *allocq();
extern int    qenable();
extern mblk_t *unlinkb();
extern mblk_t *unlinkmsg();
extern int    pullupmsg();
extern int    adjmsg();
extern struct iqueue *getendq();
extern struct linkblk *findlinks();
extern struct file *getf();
extern struct strevent *sealloc();
extern int   sefree();
extern int noenable();
extern int enabelok();
#endif

#if defined IOPM_OS
/*
 * shared or externally configured data structures
 */
extern struct stdata streams[];		/* table of streams */
extern iqueue_t	queue[];		/* table of queues */
extern mblk_t 	mblock[]; 		/* table of msg blk desc */
extern dblk_t 	dblock[];	 	/* table of data blk desc */
extern ushort	rbsize[];		/* map of class to real block size */
extern struct linkblk linkblk[];	/* multiplexor link table */
extern struct strevent strevent[];	/* table of stream event cells */
extern struct strstat strst;		/* Streams statistics structure */
extern iqueue_t	*qhead;			/* head of runnable queue list */
extern iqueue_t	*qtail;			/* tail of runnable queue list */
extern int strmsgsz;			/* maximum stream message size */
extern int strctlsz;			/* maximum size of ctl part of msg */
extern int nmblock;			/* number of msg blk desc */
extern int nmuxlink;			/* number of multiplexor links */
extern int nstrpush;			/* maxmimum number of pushes allowed */
extern int nstrevent;			/* initial num of stream event cells */
extern int maxsepgcnt;			/* page limit for event cell alloc */
extern int pollwait;			/* poll sleeps on &pollwait */
extern char qrunflag;			/* set if there are queues to run */
extern struct sem_lock  qrunlock;	/* lock for qhead & qtail */
extern mblk_t           *mbfreelist;	/* message block freelist */
extern dblk_t           *dbfreelist[NCLASS];	/* data block freelists */
extern struct dbalcst   dballoc[NCLASS];/* data block allocation parameters */
extern char             strbcflag;	/* bufcall functions ready to go */
extern struct sem_lock  allocb_lock;	/* for allocb */
#endif

extern iqueue_t *RD();
extern iqueue_t *WR();
extern iqueue_t *OTHERQ();

#endif /* IOPM_STREAM_H */
