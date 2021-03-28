#ident	"@(#)strftime.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <time.h>
#include <ctype.h>
#include <string.h>
#include "_time.h"
#include "_locale.h"
#include <stdlib.h>

#pragma strwrite, show
static char	*dflt_tz[] = {"UTC", "   "};
#pragma reset, show

size_t strftime(char *s,size_t maxsize,const char *format,const struct tm *tm)
{
	register char	*p,  c;
	register int	r;
	int			i, temp;
	size_t		size = 0;
	char		*ptr;
	char		nstr[5];
	size_t		n;
	char		tznm[_MAXTZNAME + 1];

	/* Set format string, if not already set */
	if (format == NULL) format = _time[DFL_FMT];

	/* Build date string by parsing format string */
	while ((c = *format++) != '\0') {
		if (c == '%') {
			switch (*format++) {
			case '%':	/* Percent sign */
				p = "%";
				break;
			case 'a':	/* Abbreviated weekday name */
				p = _time[aSun + tm->tm_wday];
				break;
			case 'A':	/* Weekday name */
				p = _time[Sun + tm->tm_wday];
				break;
			case 'b':	/* Abbreviated month name */
				p = _time[aJan + tm->tm_mon];
				break;
			case 'h':	/* X/Open conformance */
				p = _time[aJan + tm->tm_mon];
				break;
			case 'B':	/* Month name */
				p = _time[Jan + tm->tm_mon];
				break;
			case 'c':	/* Localized date & time format */
				p = _time[DFL_FMT];
				goto recur;
			case 'd':	/* Day number */
				p = _itoa(tm->tm_mday, nstr, 2);
				break;
			case 'D':
				p = "%m/%d/%y";
				goto recur;
			case 'e':
				p = _itoa(tm->tm_mday, nstr, 2);
				if (tm->tm_mday < 10)
					p[0] = ' ';
				break;
			case 'H':	/* Hour (24 hour version) */
				p = _itoa(tm->tm_hour, nstr, 2);
				break;
			case 'I':	/* Hour (12 hour version) */
				temp = tm->tm_hour % 12;
				p = _itoa(temp == 0 ? 12 : temp, nstr, 2);
				break;
			case 'j':	/* Julian date */
				p = _itoa(tm->tm_yday + 1, nstr, 3);
				break;
			case 'm':	/* Month number */
				p = _itoa(tm->tm_mon + 1, nstr, 2);
				break;
			case 'M':	/* Minute */
				p = _itoa(tm->tm_min, nstr, 2);
				break;
			case 'n':	/* Newline */
				p = "\n";
				break;
			case 'p':	/* AM or PM */
				if (tm->tm_hour >= 12)
					p = _time[PM];
				else
					p = _time[AM];
				break;
			case 'r':
				p = "%I:%M:%S %p";
				goto recur;
			case 'R':
				p = "%H:%M";
				goto recur;
			case 'S':	/* Seconds */
				p = _itoa(tm->tm_sec, nstr, 2);
				break;
			case 't':	/* Tab */
				p = "	";
				break;
			case 'T':
				p = "%H:%M:%S";
				goto recur;
			case 'U':	/* Weekday number, taking Sunday as
					 * the first day of the week */
				p = _itoa((tm->tm_yday - tm->tm_wday)/7 + 1, nstr, 2);
				break;
			case 'w':	/* Weekday number */
				p = _itoa(tm->tm_wday, nstr, 1);
				break;
			case 'W':	/* Week number of year, taking Monday as
					 * first day of week */
				if ((size += 2) >= maxsize)
					break;
				if (tm->tm_wday == 0)
					temp = tm->tm_yday - 6;
				else
					temp = tm->tm_yday - tm->tm_wday + 1;
				p = _itoa(temp/7 + 1, nstr, 2);
				break;
			case 'x':	/* Localized date format */
				p = _time[Local_date];
				goto recur;
			case 'X':	/* Localized time format */
				p = _time[Local_time];
				goto recur;
			case 'y':	/* Year in the form yy */
				p = _itoa(tm->tm_year, nstr, 2);
				break;
			case 'Y':	/* Year in the form ccyy */
				p = _itoa(1900 + tm->tm_year, nstr, 4);
				break;
			case 'Z':	/* Timezone */
				if ((ptr = getenv("TZ")) == 0 || *ptr == '\0'
					|| (ptr = _getzname(ptr, tznm)) == 0) {
					if (tm->tm_isdst) p = dflt_tz[1]; else p = dflt_tz[0];
					break;
				} else if (tm->tm_isdst) {
					while (*ptr != '\0' && !isalpha(*ptr)) ptr++;
					if ((ptr = _getzname(ptr, tznm)) == 0) {
						p = dflt_tz[1];
						break;
					}
				}
				p = tznm;
				break;
			default:
				nstr[0] = c;
				nstr[1] = *(format - 1);
				nstr[2] = '\0';
				p = nstr;
				break;
			recur:;
				if ((n = strftime(s, maxsize-size, p, tm)) == 0)
					return(0);
				s += n;
				size += n;
				continue;
			}
			n = strlen(p);
			if ((size += n) >= maxsize)
				return(0);
			strcpy(s, p);
			s += n;
		} else {
			if (++size >= maxsize)
				return(0);
	 		*s++ = c;
		}
	}
	*s = '\0';
	return(size);
}
