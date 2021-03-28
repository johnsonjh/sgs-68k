#ifndef SYS_VD_H
#define SYS_VD_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) vd.h: version 2.1 created on 5/22/89 at 19:12:45	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)vd.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef SYS_BUF_H
#include "sys/buf.h"
#endif  /* SYS_BUF_H */

#define MAX_STRIPE_COMPONENTS	16	/* max num of components of each
					 * of each striped slice.
					 */
typedef struct stripe {
	dev_t	s_bdev;			/* block device number */
	dev_t	s_cdev;			/* character device number */
	uint	s_track_size;		/* number of sectors in track */
} stripe_t;

typedef struct vd {
	ushort		v_configured;		/* configured by ioctl yet? */
	ushort		v_open;			/* number of times opened */
	ushort		v_num_stripe_components;
	ushort		v_cyl_size;
	uint		v_size;			/* number of sectors */
	stripe_t	v_sd[MAX_STRIPE_COMPONENTS];
} vd_t;

typedef struct mirror {
	uchar	m_configured;
	uchar	m_valid;
	ushort	m_open;
	uint	m_cyl_size;
	uint	m_lpercent;
	uint	m_size;		/* number of blocks in this slice */
	dev_t	m_b_left;
	dev_t	m_c_left;
	dev_t	m_b_right;
	dev_t	m_c_right;
	dev_t	m_b_valid_dev;	/* dev when only one side is valid */
	dev_t	m_c_valid_dev;	/* dev when only one side is valid */
} mirror_t;

extern vd_t	virt_slices[];
extern buf_t	*getvbuf();
extern mirror_t	mr_slices[];
extern buf_t	*getmrbuf();
extern buf_t	mrfreelist;
extern buf_t	mrbufhdrs[];
extern uint	num_mr_bufs;
extern buf_t	vdfreelist;
extern buf_t	vdbufhdrs[];
extern uint	num_vd_bufs;
extern uint	max_mr_slices;
extern uint	max_virt_slices;

extern unsigned int	vdfreelist_sem;
extern unsigned int	mrfreelist_sem;
extern unsigned int	mr_iodone_sem;

#endif /* SYS_VD_H */
