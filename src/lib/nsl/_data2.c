/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) _data2.c: version 25.1 created on 12/2/91 at 20:16:23	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)_data2.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libnsl:nsl/_data2.c	1.1.1.1"	*/

#ident	"@(#)lib/nsl:_data2.c	25.1"

#include "sys/types.h"
#include "sys/timod.h"
#include "sys/tiuser.h"
#include "stdio.h"
#include "_import.h"


/*
 * openfiles is number of open files returned by ulimit().
 * It is initialized when t_open() is called for the first
 * time.
 */

long openfiles = 0;

/*
 * State transition table for TLI user level
 */

char tiusr_statetbl[T_NOEVENTS][T_NOSTATES] = {

/* 0    1    2    3    4    5    6    7   8 */
{  1, nvs, nvs, nvs, nvs, nvs, nvs, nvs,  8},
{nvs,   2, nvs, nvs, nvs, nvs, nvs, nvs,  8},
{nvs, nvs,   2, nvs, nvs, nvs, nvs, nvs,  8},
{nvs, nvs,   1, nvs, nvs, nvs, nvs, nvs,  8},
{nvs,   0, nvs, nvs, nvs, nvs, nvs, nvs,  8},
{nvs, nvs,   2, nvs, nvs, nvs, nvs, nvs,  8},
{nvs, nvs,   2, nvs, nvs, nvs, nvs, nvs,  8},
{nvs, nvs,   2, nvs, nvs, nvs, nvs, nvs,  8},
{nvs, nvs,   5, nvs, nvs, nvs, nvs, nvs,  8},
{nvs, nvs,   3, nvs, nvs, nvs, nvs, nvs,  8},
{nvs, nvs, nvs,   5, nvs, nvs, nvs, nvs,  8},
{nvs, nvs,   4, nvs,   4, nvs, nvs, nvs,  8},
{nvs, nvs, nvs, nvs,   5, nvs, nvs, nvs,  8},
{nvs, nvs, nvs, nvs,   2, nvs, nvs, nvs,  8},
{nvs, nvs, nvs, nvs,   4, nvs, nvs, nvs,  8},
{nvs, nvs, nvs, nvs, nvs,   5, nvs,   7,  8},
{nvs, nvs, nvs, nvs, nvs,   5,   6, nvs,  8},
{nvs, nvs, nvs,   2,   2,   2,   2,   2,  8},
{nvs, nvs, nvs, nvs,   4, nvs, nvs, nvs,  8},
{nvs, nvs, nvs,   2, nvs,   2,   2,   2,  8},
{nvs, nvs, nvs, nvs,   2, nvs, nvs, nvs,  8},
{nvs, nvs, nvs, nvs,   4, nvs, nvs, nvs,  8},
{nvs, nvs, nvs, nvs, nvs,   6, nvs,   2,  8},
{nvs, nvs, nvs, nvs, nvs,   7,   2, nvs,  8},
{nvs, nvs,   5, nvs, nvs, nvs, nvs, nvs,  8},
};
