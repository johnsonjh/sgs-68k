/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) _data.c: version 25.1 created on 12/2/91 at 20:16:20	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)_data.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"libnsl:nsl/_data.c	1.2.1.1"	*/

#ident	"@(#)lib/nsl:_data.c	25.1"

#include "sys/types.h"
#include "sys/timod.h"
#include "stdio.h"
#include "_import.h"


/*
 * ti_user structures get allocated
 * the first time the user calls t_open or t_sync
 */
struct _ti_user *_ti_user = 0;

/*
 * This must be here to preserve compatibility
 */
struct _oldti_user _old_ti = { 0, 0, NULL,0,NULL,NULL,NULL,0,0,0,0,0 };
