#ifndef SYS_VD_H
#define SYS_VD_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) vd.h: version 24.1 created on 10/28/91 at 18:40:58	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)vd.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_BUF_H
#include "sys/buf.h"
#endif

#ifndef SYS_SYNCH_H
#include "sys/synch.h"
#endif

#ifndef SYS_OPEN_H
#include "sys/open.h"
#endif


#define MAX_STRIPE_COMPONENTS	16	/* max num of components of each
					 * of each striped slice.
					 */
/* v_configured flags */
#define VD_CONFIGURED		0x01	/* has been configured		*/
#define VD_OPENED		0x02	/* configured and opened	*/

typedef struct stripe {
	dev_t	s_bdev;			/* block device number */
	dev_t	s_cdev;			/* character device number */
	uint	s_track_size;		/* number of sectors in track */
} stripe_t;

typedef struct vd {
	uchar		v_configured;		/* configured by ioctl yet? */
	uchar		v_self_destruct;	/* cleared on last close */
	ushort		v_num_stripe_components;
	uint		v_cyl_size;		/* number of sectors taking
						 * one track per component
						 */
	uint		v_size;			/* number of sectors */
	stripe_t	v_sd[MAX_STRIPE_COMPONENTS];
	/* everything up to this point will be cleared by CLEAR_VIRT_DISK */
	opentyp_t	v_opentyp;		/* ways the stripe is open */
	suspend_lock_t	v_lock;			/* must be last element	*/
} vd_t;

typedef struct mirror {
	uint	m_flags;
	ulong	m_iocnt;	/* count of active I/O requests		*/
	long	m_size;		/* mirror size in blocks		*/
	uint	m_cyl_size;	/* effective cylinder size in blocks	*/
	uint	m_ppercent;	/* percent of reads that go to primary	*/
	dev_t	m_devs[2];	/* left and right devices		*/
	dev_t	m_valid_dev;	/* dev when only one side is valid	*/
	ushort	m_pad1;
	ulong	m_mon[2];	/* copy write monitor counters		*/
	long	m_cp_blk;	/* block that is being copied		*/
	buf_t	*m_conf_head;	/* list of bufs that conflicted with copy */
	struct mirror *m_nxupd;	/* next mirror to be updated in mirrortab */
	opentyp_t m_opentyp;	/* ways the mirror is open		*/
	spin_lock_t m_lock;	/* locks must be last elements in struct */
	suspend_lock_t m_sem;
} mirror_t;

/* psuedo structure members */
#define m_left	m_devs[0]
#define m_right	m_devs[1]
#define m_pri	m_devs[0]
#define m_sec	m_devs[1]

/* defines for mr_tab */
#define MVALID_BIT	0x8000
#define MIRROR_IDX	0x7fff
#define MR_DEV_CLOSED	0xFFFF
#define mr_shine(dev)	(!(mr_tab[(dev) & UNMINOR_MASK] & MVALID_BIT))
#define mr_dev_closed(dev) (mr_tab[(dev) & UNMINOR_MASK] == MR_DEV_CLOSED)
#define mr_addr(dev) (&mr_data[(mr_tab[(dev) & UNMINOR_MASK] & MIRROR_IDX) -1])

/* mirror_t m_flag defines */
#define MR_MON_IDX	0x0001	/* index of current monitor		*/
#define MR_OPEN		0x0002	/* mirror is "open"			*/
#define MR_VALID	0x0004	/* mirror is valid -- both sides match	*/
#define MR_IN_COPY	0x0008	/* mirror is being copied from m_valid_dev */
#define MR_UPDATE	0x0010	/* scheduled to have mirrortab entry updated */
#define MR_INUSE	0x0020	/* mirror is in use			*/
#define MR_PRI_ERROR	0x0040	/* I/O error on the primary		*/
#define MR_SEC_ERROR	0x0080	/* I/O error on the secondary		*/
/* These defines are found both in m_flags and used by the CAUSE_MIRROR_ERROR
 * ioctl.  They will cause the next read/write to the correct device to fail. */
#define MR_DO_RP_ERR	0x0100	/* next read from primary will fail	*/
#define MR_DO_RS_ERR	0x0200	/* next read from secondary will fail	*/
#define MR_DO_WP_ERR	0x0400	/* next write to primary will fail	*/
#define MR_DO_WS_ERR	0x0800	/* next write to secondary will fail	*/
#define MR_DO_READ_ERR	(MR_DO_RP_ERR | MR_DO_RS_ERR)
#define MR_DO_WRITE_ERR	(MR_DO_WP_ERR | MR_DO_WS_ERR)
#define MR_DO_ALL_ERRS	(MR_DO_READ_ERR | MR_DO_WRITE_ERR)
#define MR_DELAYED_ERR	0x1000	/* error happened on source during copy	*/
#define MR_ABORT_COPY	0x2000	/* abort a mirror copy operation	*/

#define MR_ACTIVE	0x80000000 /* m_iocnt and m_mon active flag	*/

/* b_driver_flag values for use in kern_bp */
#define MKF_MON_FLAG	0x80000000 /* incremented monitor		*/

/* MR_COPY_BLOCK ioctl arguments */
#define MR_COPY_P_TO_S	1	/* copy primary to secondary		*/
#define MR_COPY_S_TO_P	0

/* mirror state defines */
#define mr_open(m)	((m)->m_flags & MR_OPEN)
#define mr_valid(m) 	((m)->m_flags & MR_VALID)
#define mr_copying(m) 	((m)->m_flags & MR_IN_COPY)

				/* calculate read side on a 50% mirror */
#define mr_side(m, blk)	((m)->m_devs[((blk) / (m)->m_cyl_size) & 1])

#define MIRROR_COPY_DELAY	(30 * HZ)	/* delay before starting copy */

/* struct for the READ_MIRROR_DISK ioctl */
typedef struct mr_read {
	uint	rd_pri;		/* read from primary if non-zero	*/
	caddr_t	buf;		/* read buffer				*/
	uint	count;		/* byte count				*/
	off_t	offset;		/* mirror offset (in bytes)		*/
} mr_read_t;

/* struct for real [bc]devsw entries */
typedef struct mr_devsw {
	int	(*m_open)();
	int	(*m_close)();
	int	(*m_read)();
	int	(*m_write)();
	int	(*m_ioctl)();
	int	(*m_strategy)();
	int	(*m_print)();
} mr_devsw_t;

extern ushort		mr_tab[];
extern uint		max_mr_devs;
extern mirror_t		mr_data[];
extern uint		max_mr_data;

#endif /* SYS_VD_H */
