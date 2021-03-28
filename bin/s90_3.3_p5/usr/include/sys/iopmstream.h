/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iopmstream.h: version 24.1 created on 10/28/91 at 18:34:29	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iopmstream.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_IOPMSTREAM_H
#define SYS_IOPMSTREAM_H

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#ifndef SYS_SYNCH_H
#include "sys/synch.h"
#endif

/*
** struct of data passed to IOPM to continue the stream onto the IOPM
*/

struct iopmscb {			/* iopm stream control block */
	ushort         iopms_uid;	/* uid of proc doing open or close */
	ushort         iopms_gid;	/* gid of proc doing open or close */
	ushort         iopms_ruid;	/* real uid */
	ushort         iopms_rgid;	/* read gid */
	ulong          iopms_err;	/* error return to kernel */
	short          *iopms_ttyp;	/* tty group id pointer */
	short          iopms_pid;	/* process id */
	short          iopms_pgrp;	/* process group id */
	short          iopms_session_id;/* POSIX session id */
	dev_t          iopms_dev;	/* used by iopm to open stream to dev */
	ulong          iopms_oflag;	/* open (and close) flag */
	short          iopms_sflag;	/* clone open flag */
	uchar          iopms_savsig;
	char           iopms_mname[9];	/* 9 = FMNAMESZ+1 */
};

/*
** struct kept on the q_ptr of the kernel IOPM driver to coordinate the
** transfer of msg's to & from the IOPM.
*/
struct iopm_ctl {
	uint             ic_version;
	mblk_t           *ic_mblkp;
	struct queue     *ic_krq;
	struct iqueue    *ic_iwq;
	ushort           ic_kflags;
	ushort           ic_iflags;
	short            *ic_iopmttyp;
	short            ic_pgrp;
	dev_t            ic_iopmdev;
	uint             ic_iopmslot;
	mblk_t           *ic_hicopy_mp;
	mblk_t           *ic_locopy_mp;
	mblk_t           *ic_wflow_mp;
	mblk_t           *ic_rflow_mp;
	uint             ic_wproduced;
	uint             ic_wconsumed;
	uint             ic_wavail;
	uint             ic_rproduced;
	uint             ic_rconsumed;
	uint             ic_ravail;
	uchar            ic_wflow_state;
	uchar            ic_rflow_state;
	struct iopm_ctl  *ic_savctl;
	struct stdata    *ic_strhd;
};

/* version number of iopm_ctl, used as a sanity check */
#define CTL_VERSION  0x04091991

/* ic_kflags bits */
#define PM_DONE   0x1
#define CLOSING	  0x2
#define SIGDONE   0x4

/* ic_iflags bits */
#define QSLPCLS   0x400			/* close on this q sleeping */

struct copy_msg {
	ushort  cm_count;
	uchar   cm_use;
	uchar   cm_close;
};

/* flow states */
#define EMPTY     0
#define DRAINING  1
#define CLOGGED   2

#define DEFAULT_HIWAT  0x8000

#endif /* SYS_IOPMSTREAM_H */
