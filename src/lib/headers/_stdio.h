#ifndef __STDIO_H
#ident	"@(#)_stdio.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

#include <stdio.h>
#include "values.h"

#define _P_tmpdir       "/usr/tmp/"
#define _L_ctermid      9
#define _L_cuserid      9
#define _SBFSIZ         8
#define _fileno(p)  (p)->_file
#define _bufsiz(p)	(_bufend(p) - (p)->_base)

extern void _bufsync(FILE *);
extern int _wrtchk(FILE *);


		/* additional flags, used in _xtraflags[iop->_file] */
#ifndef DYNAMIC
#define _XFLAGS(iop)		_xtraflags[(iop)->_file]
#define _IOXPIPE 001
#define _IOXTEXT 002
#else /* DYNAMIC */
#define _IOXPIPE 004000
#define _IOXTEXT 010000
#endif /* DYNAMIC */

/* barryk: don't ungetc onto non-writable "streams"  */
/* (e.g. string literal as 1st arg to sscanf) */
#ifndef DYNAMIC
#define RDONLY(iop)		(_XFLAGS(iop) & _IOXTEXT)
/* barryk: prevent seeks on non-seekable devices */
#define SEEKCHK(iop)	( (_XFLAGS(iop) & _IOXPIPE) == 0 )
#else /* DYNAMIC */
#define RDONLY(iop)		(iop->_flag & _IOXTEXT)
#define SEEKCHK(iop)	( (iop->_flag & _IOXPIPE) == 0 )
#endif /* DYNAMIC */

/* The following macros improve performance of the stdio by reducing the
	number of calls to _bufsync and _wrtchk.  _BUFSYNC has the same
	effect as _bufsync, and _WRTCHK has the same effect as _wrtchk,
	but often these functions have no effect, and in those cases the
	macros avoid the expense of calling the functions.  
*/

#define _BUFSYNC(iop)	if ( (_bufend(iop) - iop->_ptr <   \
				( iop->_cnt < 0 ? 0 : iop->_cnt )) && SEEKCHK(iop) )  \
					_bufsync(iop)
#define _WRTCHK(iop)	((((iop->_flag & (_IOWRT | _IOEOF)) != _IOWRT) \
				|| (iop->_flag & (_IORW | _IOREAD) == _IORW) \
				|| (iop->_base == NULL)  \
				|| (iop->_ptr == iop->_base && iop->_cnt == 0 \
					&& !(iop->_flag & (_IONBF | _IOLBF)))) \
			? _wrtchk(iop) : 0 )

#define _NCHARS	(1 << BITSPERBYTE)
#define _locgetc()	(_chcount+=1,getc(iop))
#define _locungetc(x)	(_chcount-=1,ungetc(x,iop))

#ifndef DYNAMIC
extern unsigned char _smbuf[FOPEN_MAX+1][_SBFSIZ];
#else
extern unsigned char _smbuf[_SBFSIZ];
#endif /* DYNAMIC */
#if !u370
/* some slop is allowed at the end of the buffers in case an upset in
 * the synchronization of _cnt and _ptr (caused by an interrupt or other
 * signal) is not immediately detected.
 */
extern unsigned char _sibuf[BUFSIZ+8], _sobuf[BUFSIZ+8];
/*
 * Ptrs to start of preallocated buffers for stdin, stdout.
 */
#endif
#ifndef DYNAMIC
extern unsigned char *_bufendtab[FOPEN_MAX+1];
		/* barryk: we ran out of space in FILE._flags */
extern char _xtraflags[FOPEN_MAX+1];
extern FILE *_lastbuf;
#else /* DYNAMIC */
extern FILE *_firstbuf;
extern FILE *_freeiob;
extern FILE *_freemalloc;
#endif /* DYNAMIC */

extern int _chcount;
extern int _flag_eof;

void _bufsync(FILE *);
void _findbuf(FILE *);
#ifdef DYNAMIC
void _sortfree(FILE *);
#endif /* DYNAMIC */
int _dowrite(char *,int,FILE *,unsigned char **);
int _doprnt(char *,va_list,FILE *);
int _doscan(FILE *,const unsigned char *,va_list);
int _filbuf(FILE *);
int _flsbuf(unsigned char,FILE *);
int _lowdigit(long int *);
int _number(int,int,int,int,FILE *,va_list *);
int _string(int,int,int,char *,FILE *,va_list *);
int _ungetc(int,FILE *);
int _wrtchk(FILE *);
int _xflsbuf(FILE *);
FILE *_endopen(const char *,const char *,FILE *);
#ifndef DYNAMIC
FILE *_findiop(void);
#else /* DYNAMIC */
FILE *_findiop(FILE *);
#endif /* DYNAMIC */
unsigned char *_setup(unsigned char *,char *);
char *_memccpy(void *,const void *,int,int);
char *_mktemp(char *);
char *_ecvt(double,int,int *,int *);
char *_fcvt(double,int,int *,int *);
char *_cvt(double,int,int *,int *,int);
unsigned int _rd881_control(void);
void _wr881_control(unsigned int);

#define __STDIO_H

#endif /* __STDIO_H */
