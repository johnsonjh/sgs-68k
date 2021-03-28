/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#if !defined __STRICT_STDC__
#include "sys/types.h"
#endif


#ifndef _STDIO_H
#ident	"@(#)stdio.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define _STDIO_H

#if !lint
#pragma SYS
#endif

#ifndef _NODYNAMIC
#define DYNAMIC
#else
#undef DYNAMIC
#endif
/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#define NULL			0	/* Also defined in locale, stddef, stdlib,  */
							/* string and time							*/
#define FILENAME_MAX	14
#define FOPEN_MAX	64
#define L_tmpnam		(sizeof("/usr/tmp/") + 15)	/* refers to P_tmpdir */
#define SEEK_SET		0
#define SEEK_CUR		1
#define SEEK_END		2
#define TMP_MAX			17576 		/* |{'a'..'z'}|^3 */
#define EOF				-1
#define BUFSIZ			(size_t) 1024

/*
 * _IOLBF means that a file's output will be buffered line by line
 * In addition to being flags, _IONBF, _IOLBF and _IOFBF are possible
 * values for "type" in setvbuf.
 */

#define _IOFBF		0000
#define _IONBF		0004
#define _IOLBF		0100

#define stdin		(&_iob[0])
#define stdout		(&_iob[1])
#define stderr		(&_iob[2])

#if !defined __STRICT_STDC__

#define _IOREAD		000001
#define _IOWRT		000002
#define _IOMYBUF	000010
#define _IOEOF		000020
#define _IOERR		000040
#define _IORW		000200
#ifdef DYNAMIC
#define _IOSTRING	000400
#define _IOIOB		001000
#define _IOFREE		002000
#endif /* DYNAMIC */

#ifndef DYNAMIC
		/* additional flags, used in _xtraflags[iop->_file] */
#define _XFLAGS(iop)		_xtraflags[(iop)->_file]
#define _IOXPIPE 001
#define _IOXTEXT 002
#else /* DYNAMIC */
#define _IOXPIPE 004000
#define _IOXTEXT 010000
#endif /* DYNAMIC */

#define _NFILE	FOPEN_MAX
#define _SBFSIZ 8

#ifndef DYNAMIC
#define _bufend(p)	_bufendtab[(p)->_file]
#else /* DYNAMIC */
#define _bufend(p)	((p)->_bufendp)
#endif /* DYNAMIC */
#define _bufsiz(p)	(_bufend(p) - (p)->_base)
#define L_ctermid	9
#define L_cuserid	9
#define P_tmpdir	"/usr/tmp/"

#endif  /* ! __STRICT_STDC__ */

#if defined _SYSV
#define getc(p)		(--(p)->_cnt < 0 ? _filbuf(p) : (int) *(p)->_ptr++)
#define putc(x, p)	(--(p)->_cnt < 0 ? \
			_flsbuf((unsigned char) (x), (p)) : \
			(int) (*(p)->_ptr++ = (unsigned char) (x)))
#define getchar()	getc(stdin)
#define putchar(x)	putc((x), stdout)
#define feof(p)		((p)->_flag & _IOEOF)
#define ferror(p)	((p)->_flag & _IOERR)
#define fileno(p)	(p)->_file
#define clearerr(p)	((void) ((p)->_flag &= ~(_IOERR | _IOEOF)))
#endif 	/* _SYSV */


/*****************************************************************************
                                  T Y P E S
 ****************************************************************************/

#if !defined _STDARG_H && !defined _VARARGS_H
typedef char *va_list; /* Also defined in stdarg.h	*/
#endif

#if  !defined _STDDEF_H && !defined _STDLIB_H && !defined _STRING_H && !defined _TIME_H && !defined SYS_TYPES_H
typedef unsigned int size_t;	/* Also defined in stddef, stdlib, string */
								/* and time                               */
#endif							
#ifndef DYNAMIC

typedef struct {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	char	_flag;
	char	_file;
} FILE;
 
#else /* DYNAMIC */

typedef struct _FILE {
	int 	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	unsigned int	_flag;
	int	_file;
	char *_bufendp;
	struct _FILE *_next;
	struct _FILE *_prev;
} FILE;

#endif /* DYNAMIC */

/* Our filesystem cannot handle files that are larger than 4GB.
 * Therefore, this type does not have to be capable of handling larger ones
 */
typedef long int fpos_t; 


/*****************************************************************************
                   E X T E R N A L    D E C L A R A T I O N S 
 ****************************************************************************/

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif


extern FILE _iob[FOPEN_MAX];

FILE *fopen _PARMS((const char *,const char *));
FILE *freopen _PARMS((const char *,const char *,FILE *));
FILE *tmpfile _PARMS((void));
long int ftell _PARMS((FILE *));
void rewind _PARMS((FILE *));
void setbuf _PARMS((FILE *,char *));
char *gets _PARMS((char *));
char *fgets _PARMS((char *,int,FILE *));
char *tmpnam _PARMS((char *));
int fclose _PARMS((FILE *));
int fflush _PARMS((FILE *));
int fseek _PARMS((FILE *,long int,int));
int fgetc _PARMS((FILE *));
int printf _PARMS((const char *,...));
int fprintf _PARMS((FILE *,const char *,...));
int sprintf _PARMS((char *,const char *,...));
int vprintf _PARMS((const char *,va_list));
int vfprintf _PARMS((FILE *,const char *,va_list));
int vsprintf _PARMS((char *,const char *,va_list));
int fputc _PARMS((int,FILE *));
int puts _PARMS((char *));
int fputs _PARMS((char *,FILE *));
int scanf _PARMS((const char *,...));
int fscanf _PARMS((FILE *,const char *,...));
int sscanf _PARMS((const char *,const char *,...));
int setvbuf _PARMS((FILE *,char *,int,size_t));
int remove _PARMS((const char *));
int rename _PARMS((const char *,const char*));
int fsetpos _PARMS((FILE *,const fpos_t *));
int fgetpos _PARMS((FILE *,fpos_t *));
int ungetc _PARMS((int,FILE *));
void perror _PARMS((const char *));

#if defined _SYSV
int fread();
int fwrite();
#else
size_t fread  _PARMS((void *,size_t,size_t,FILE *));
size_t fwrite  _PARMS((const void *,size_t,size_t,FILE *));
void clearerr _PARMS((FILE *));
int putc _PARMS((int,FILE *));
int getc _PARMS((FILE *));
int getchar _PARMS((void));
int putchar _PARMS((int));
int feof _PARMS((FILE *));
int ferror _PARMS((FILE *));
#endif /* _SYSV */

#if!defined __STRICT_STDC__

char *ctermid _PARMS((char *));
char *cuserid _PARMS((char *));
char *getpass _PARMS((char *));
FILE *fdopen _PARMS((int,char *));
int getw _PARMS((FILE *));
int pclose _PARMS((FILE *));
FILE *popen _PARMS((char *,char *));
int putw _PARMS((int,FILE *));
int system _PARMS((char *));
char *ecvt _PARMS((double,int,int *,int *));
char *fcvt _PARMS((double,int,int *,int *));

#ifndef DYNAMIC
extern unsigned char *_bufendtab[];
extern char _xtraflags[];
#endif /* DYNAMIC */

#endif	/* ! __STRICT_STDC__ */

#if !lint
#pragma noSYS
#endif

#endif	/* _STDIO_H */
