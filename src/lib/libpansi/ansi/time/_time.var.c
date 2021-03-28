#ident	"@(#)_time.var.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <time.h>
#include <_time.h>

#ifdef _ANSI 
extern time_t _timezone=0;
extern time_t _altzone=0;
extern int _daylight=0;
#pragma strwrite,show
extern char *_tzname[2] = { "UTC", "   " };
#pragma nostrwrite,show
extern time_t _start_dst=0;    /* Start date of alternate time zone */
extern time_t _end_dst=0;      /* End date of alternate time zone */
extern short _month_size[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
#else
extern time_t timezone=0;
extern time_t altzone=0;
extern int daylight=0;
#pragma strwrite,show
extern char *tzname[2] = { "UTC", "   " };
#pragma nostrwrite,show
extern time_t start_dst=0;    /* Start date of alternate time zone */
extern time_t end_dst=0;      /* End date of alternate time zone */
extern short month_size[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
#endif /* _ANSI */

extern char * _time[] = {
	"Jan","Feb","Mar","Apr","May","Jun","Jul", "Aug", "Sep","Oct", "Nov", "Dec",
	"January", "February", "March","April",
	"May","June", "July", "August", "September",
	"October", "November", "December",
	"Sun","Mon", "Tue", "Wed","Thu", "Fri","Sat",
	"Sunday","Monday","Tuesday","Wednesday", "Thursday","Friday","Saturday",
	"%H:%M:%S","%a %b %e, %Y", "%a %b %e %T %Z %Y",
	"AM", "PM", NULL
};
