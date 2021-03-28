/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) proc.h: version 24.1 created on 10/28/91 at 18:00:03	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)proc.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef IOPM_PROC_H
#define IOPM_PROC_H

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

struct i_proc
{
	short         p_pid;
	short         p_pgrp;
	short         p_session_id;
};

#ifndef	IOPM_OS		/* trick drivers */
#define	proc	i_proc
#endif

#endif /* IOPM_PROC_H */
