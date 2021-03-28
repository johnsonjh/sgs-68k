/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tcb.h: version 24.1 created on 10/28/91 at 18:00:30	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tcb.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#if !defined IOPM_TCB_H
#define IOPM_TCB_H

#if !defined SYS_TYPES_H
#include "sys/types.h"
#endif

#if !defined SYS_IMMU_H
#include "sys/immu.h"
#endif

#define STKSZ NBPP	/* stack size is tied to 1 page in machdep */

struct tcb
{
	struct tcb     *tcb_link;
	caddr_t        tcb_wchan;
	int            tcb_pri;
	int            tcb_sig;
	int            tcb_rsav[13];
	ushort         tcb_flags;
	short          p_pid;
	short          p_pgrp;
	short          p_session_id;
	ushort         u_uid;
	ushort         u_gid;
	ushort         u_ruid;
	ushort         u_rgid;
	int            u_error;
	int            u_rval;
	short          *u_ttyp;
	struct buf     *u_kbp;
};

/* positions in tcb_rsav */
#define PC  6
#define SP  12

/* flags */
#define T_FREE     0
#define T_CURRENT  1
#define T_RUNQ     2
#define T_SLEEPQ   3

#endif /* IOPM_TCB_H */
