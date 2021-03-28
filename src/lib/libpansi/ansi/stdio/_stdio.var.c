#ident	"@(#)_stdio.var.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"

#ifndef DYNAMIC
extern unsigned char *_bufendtab[FOPEN_MAX+1] 
			  = { NULL, NULL, _smbuf[2]+_SBFSIZ, };
char _xtraflags[FOPEN_MAX+1]; 

extern FILE _iob[FOPEN_MAX] = {
#if defined vax || defined u3b || defined M32
	{ 0, NULL, NULL, _IOREAD, 0},
	{ 0, NULL, NULL, _IOWRT, 1},
	{ 0, _smbuf[2], _smbuf[2], _IOWRT+_IONBF, 2},
#endif
#ifdef pdp11
	{ NULL, 0, NULL, _IOREAD, 0},
	{ NULL, 0, NULL, _IOWRT, 1},
	{ _smbuf[2], 0, _smbuf[2], _IOWRT+_IONBF, 2},
#endif
#ifdef u370
	{ NULL, 0, NULL, _IOREAD, 0},
	{ NULL, 0, NULL, _IOWRT, 1},
	{ NULL, 0, NULL, _IOWRT+_IONBF, 2},
#endif
};

extern FILE *_lastbuf = { &_iob[FOPEN_MAX] };
#endif /* DYNAMIC */
extern unsigned char _sibuf[BUFSIZ+8] = { 0 };
extern unsigned char _sobuf[BUFSIZ+8] = { 0 };
#ifndef DYNAMIC
extern unsigned char _smbuf[FOPEN_MAX+1][_SBFSIZ] = 
{ "","","","","","","","","","","","","","","","","","","","","","","","","",
  "","","","","","","","","","","","","","","","","","","","","","","","","",
  "","","","","","","","","","","","","","",""};
#else /* DYNAMIC */
extern unsigned char _smbuf[_SBFSIZ] = { 0 };
#endif /* DYNAMIC */
extern int _chcount=NULL;
extern int _flag_eof=NULL;

extern void _cleanup()
{
	register FILE *iop;

#ifndef DYNAMIC
	for(iop = _iob; iop < _lastbuf; iop++)
		(void) fclose(iop);
#else /* DYNAMIC */
	while (_firstbuf != NULL)
		(void) fclose(_firstbuf);
#endif /* DYNAMIC */
}
