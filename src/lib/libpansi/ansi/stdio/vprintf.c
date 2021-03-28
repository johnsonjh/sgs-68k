#ident	"@(#)vprintf.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <stdarg.h>
#include <errno.h>

extern int vprintf(const char *format,va_list ap)
{
	register int count;

	if (!(stdout->_flag & _IOWRT)) { /* if no write flag */
		if (stdout->_flag & _IORW) { /* if ok, cause read-write */
			stdout->_flag |= _IOWRT;
		} else { /* else error */
			errno = EBADF;		/* barryk: for POSIX 1003.1 */
			return EOF;
		}
	}
	count = _doprnt(format, ap, stdout);
	return(ferror(stdout)? EOF: count);
}
