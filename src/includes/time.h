/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#if !defined __STRICT_STDC__
#include "sys/types.h"
#endif

#ifndef _TIME_H
#ident	"@(#)time.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define _TIME_H
#pragma SYS

/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#define CLOCKS_PER_SEC	50
#define NULL			0		/* Also defined in locale, stddef, stdio, */
								/* stdlib and string					  */


/*****************************************************************************
                                  T Y P E S
 ****************************************************************************/

typedef struct tm 
{	int	tm_sec;
	int	tm_min;
	int	tm_hour;
	int	tm_mday;
	int	tm_mon;
	int	tm_year;
	int	tm_wday;
	int	tm_yday;
	int	tm_isdst;
};

#if  !defined _STDDEF_H && !defined _STDIO_H && !defined _STDLIB_H && !defined _STRING_H && !defined SYS_TYPES_H
typedef unsigned int size_t;	/* Also defined in stddef, stdio, stdlib */
								/* and string							 */
#endif

#if !defined SYS_TYPES_H
typedef unsigned int clock_t;
typedef long int time_t;
#endif


/*****************************************************************************
                   E X T E R N A L    D E C L A R A T I O N S 
 ****************************************************************************/

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

char *asctime _PARMS((const struct tm *));
char *ctime _PARMS((const time_t *));
struct tm *gmtime _PARMS((const time_t *));
struct tm *localtime _PARMS((const time_t *));

#if defined _SYSV
long time();
long clock();
#else
time_t time _PARMS((time_t *));
clock_t clock _PARMS((void));
double difftime _PARMS((time_t,time_t));
time_t mktime _PARMS((struct tm *));
size_t strftime _PARMS((char *,size_t,const char *,const struct tm *));
#endif /* _SYSV */

#if !defined __STRICT_STDC__

extern long timezone, altzone;
extern int daylight;
extern char *tzname[2];

#include "limits.h"
#include "sys/times.h"

long int times _PARMS((struct tms *));

#endif /* ! __STRICT_STDC__ */

#pragma noSYS
#endif	/* _TIME_H */
