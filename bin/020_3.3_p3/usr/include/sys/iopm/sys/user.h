/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) user.h: version 24.1 created on 10/28/91 at 18:00:34	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)user.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef IOPM_USER_H
#define IOPM_USER_H

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#ifdef	IOPM_OS				/* private variables for IOPM_OS */
#ifndef SYS_BUF_H
#include "sys/buf.h"
#endif
#endif

struct i_user
{
	struct i_proc  *u_procp;
	ushort         u_uid;
	ushort         u_gid;
	ushort         u_ruid;
	ushort         u_rgid;
	int            u_error;
#ifdef	IOPM_OS
	int            u_rval;		/* no rval1 to avoid define in user */
#else
	union {
		struct {
			int  r_val1;
		} r_reg;
	} u_r;
#endif
	short          *u_ttyp;
#ifdef	IOPM_OS				/* private variables for IOPM_OS */
	struct buf     *u_kbp;
#else
	void           *u_kbp;
#endif
};

extern	struct i_user	iu;

#ifndef	IOPM_OS				/* trick drivers */
#define u_rval1  u_r.r_reg.r_val1
#define	user	i_user
extern  struct user  *get_u();
#define	u	(*get_u())
#endif

#endif /* IOPM_USER_H */
