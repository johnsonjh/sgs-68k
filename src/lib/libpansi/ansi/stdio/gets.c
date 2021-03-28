#ident	"@(#)gets.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <string.h>				/* for memcpy declaration */

extern char *gets(char *ptr)
{
	char *p, *ptr0 = ptr;
	register int n;

	for ( ; ; ) {
		if (stdin->_cnt <= 0) { /* empty buffer */
			if (_filbuf(stdin) == EOF) {
				if (ptr0 == ptr)
					return (NULL);
				break; /* no more data */
			}
			stdin->_ptr--;
			stdin->_cnt++;
		}
		n = stdin->_cnt;
#ifdef _ANSI 
		if ((p = _memccpy(ptr, stdin->_ptr,(int) '\n', n)) != NULL)
#else
		if ((p = (char *) memccpy(ptr, (char *) stdin->_ptr,(int) '\n', n)) != NULL)
#endif /* _ANSI */
			n = p - ptr;
		ptr += n;
		stdin->_cnt -= n;
		stdin->_ptr += n;
		_BUFSYNC(stdin);
		if (p != NULL) { /* found '\n' in buffer */
			ptr--; /* step back over '\n' */
			break;
		}
	}
	*ptr = '\0';
	return (ptr0);
}
