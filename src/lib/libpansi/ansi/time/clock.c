#ident	"@(#)clock.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <time.h>
#include "_time.h"

#define _TIMES(B)	(B.tms_utime+B.tms_stime+B.tms_cutime+B.tms_cstime)

static long first = 0L;

clock_t clock(void)
{
	struct tms buffer;

	if (times(&buffer) != -1L && first == 0L) {
		first = _TIMES(buffer); return (clock_t) -1; 
	};
	return ((_TIMES(buffer) - first) * 1000000L);
}
