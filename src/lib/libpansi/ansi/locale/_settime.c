#ident	"@(#)_settime.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <locale.h>
#include "_locale.h"
#include <ctype.h>
#include <stdio.h>
#include "_time.h"
#include <string.h>
#include <stdlib.h>
#include <syscall.h>

extern int _settime(char *new_time)
{
	register char *p;
	register int  j;
	char *my_time[LAST];
	static char *ostr = (char *)0;
	char *str;
	FILE *fd;
	struct stat buf;

	if ( (fd = fopen(_fullocale(new_time, "LC_TIME"),"r")) == NULL)
		return -1;

	if( (fstat(fd->_file, &buf)) != 0)
		{ fclose(fd); return -1; };

	if ( (str = (char *) malloc(buf.st_size + 2)) == NULL
		|| (read(fd->_file, str, buf.st_size)) != buf.st_size)
		{ free(str); fclose(fd); return -1; };

	/* Set last character of str to '\0' */
	p = &str[buf.st_size];
	p[0] = '\n';
	p[1] = '\0';

	/* p will "walk thru" str */
	p = str;  	

	j = -1;
	while (*p != '\0')
	{ 
		/* "Look for a newline, i.e. end of sub-string
		 * and  change it to a '\0'. If LAST pointers
		 * have been set in my_time, but the newline hasn't been seen
		 * yet, keep going thru the string leaving my_time alone.
		 */
		if (++j < LAST) 
			my_time[j] = p;
		p = strchr(p,'\n');
		*p++ = '\0';
	}
	if (j == LAST)
	{
		memcpy((void *) _time,(void *) my_time, sizeof(my_time)); 
		if (ostr != 0)	 /* free the previoulsy allocated local array*/
			free(ostr);
		ostr = str;
		return 1;
	};
	
	free(str);
	fclose(fd);
	return -1;
}
