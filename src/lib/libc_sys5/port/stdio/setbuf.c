#ident	"@(#)setbuf.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:stdio/setbuf.c	2.6"	*/
	

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
#include "shlib.h"
#include <stdio.h>

extern void free();
extern int isatty();
#ifndef DYNAMIC
extern unsigned char _smbuf[][_SBFSIZ];
#else /* DYNAMIC */
extern unsigned char _smbuf[_SBFSIZ];
#endif /* DYNAMIC */
#if !u370
/* extern unsigned char *_stdbuf[]; */
extern unsigned char _sibuf[], _sobuf[];
#else
extern char *malloc();
#endif

void
setbuf(iop, buf)
register FILE *iop;
char	*buf;
{
	register int fno = fileno(iop);  /* file number */

	if(iop->_base != NULL && iop->_flag & _IOMYBUF) {
		free((char*)iop->_base);
	}
	iop->_flag &= ~(_IOMYBUF | _IONBF | _IOLBF);
	if((iop->_base = (unsigned char*)buf) == NULL) {

		/* file unbuffered except in fastio */
		iop->_flag |= _IONBF;
#if u370
		if ((iop->_base = (unsigned char *) malloc(BUFSIZ+8)) != NULL){
			iop->_flag |= _IOMYBUF;
			_bufend(iop) = iop->_base + BUFSIZ;
#else
		/* for stdin, stdout, use the existing bufs */
		if (fno < 2) {
			iop->_base = (fno == 0 ? _sibuf : _sobuf) + BUFSIZ;
			_bufend(iop) = iop->_base;
#endif
		/* otherwise, use small buffers reserved for this */
		} else {
#ifndef DYNAMIC
			_bufend(iop) = (iop->_base = _smbuf[fno]) + _SBFSIZ;
#else /* DYNAMIC */
			iop->_base = (unsigned char *) malloc(_SBFSIZ);
			_bufend(iop) = iop->_base;
#endif /* DYNAMIC */
		}

	/* regular buffered I/O, standard buffer size */
	} else {
		_bufend(iop) = iop->_base + BUFSIZ;
		if (isatty(fno)) {
			iop->_flag |= _IOLBF;
		}
	}
	iop->_ptr = iop->_base;
	iop->_cnt = 0;
}
