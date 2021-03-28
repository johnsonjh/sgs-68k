#ifndef SYS_STDIO_H
#define SYS_STDIO_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) stdio.h: version 24.1 created on 10/28/91 at 18:38:34	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)stdio.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/stdio.h	10.2"		*/

#ifndef DYNAMIC
#define DYNAMIC
#endif

#ifndef _NFILE
#define _NFILE	64

#define BUFSIZ	1024

/* buffer size for multi-character output to unbuffered files */
#define _SBFSIZ 8

#ifdef DYNAMIC

typedef struct _FILE {
	/* number of chars remaining in the buffer from _ptr to the end */
	int	_cnt;
	/* pointer into the associated buffer */
	unsigned char 	*_ptr;
	/* pointer to the associated buffer */
	unsigned char	*_base;
	/* read, write, no buffer, etc. flags */
	unsigned int	_flag;
	/* file descriptor returned from open() */
	int	_file;
	/* pointer to the end of the buffer */
	char	*_bufendp;
	/* pointer to next file structure on list */
	struct _FILE *_next;
	/* pointer to previous file structure on list */
	struct _FILE *_prev;
} FILE;

#else /* DYNAMIC */

typedef struct {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	char	_flag;
	char	_file;
} FILE;

#endif /* DYNAMIC */

/*
 * _IOLBF means that a file's output will be buffered line by line
 * In addition to being flags, _IONBF, _IOLBF and _IOFBF are possible
 * values for "type" in setvbuf.
 */
#define _IOFBF		000000 /* Fully buffered */
#define _IOREAD		000001 /* Read only */
#define _IOWRT		000002 /* Write only */
#define _IONBF		000004 /* Non buffered */
#define _IOMYBUF	000010 /* Set if we assign our own buffer. */
#define _IOEOF		000020 /* Set if EOF reached. */
#define _IOERR		000040 /* Set if I/O error ocurred. */
#define _IOLBF		000100 /* Line buffered (for tty's). */
#define _IORW		000200 /* Update, read and write. */
#ifdef DYNAMIC
#define _IOSTRING	000400 /* set if we are doing sprintf or sscanf. */
#define _IOIOB		001000 /* set if structure came from _iob array. */
#define _IOFREE		002000 /* set when we close a structure.	 */
#endif /* DYNAMIC */

#ifndef NULL
#define NULL		0
#endif
#ifndef EOF
#define EOF		(-1)
#endif

#define stdin		(&_iob[0])
#define stdout		(&_iob[1])
#define stderr		(&_iob[2])

#ifdef DYNAMIC

#define	_bufend(p)	((p)->_bufendp)	/* returns pointer to end of buffer */

#else /* DYNAMIC */

#define _bufend(p)	(_bufendtab[(p)->_file])

#endif /* DYNAMIC */

#define _bufsiz(p)	(_bufend(p) - (p)->_base) /* returns size of buffer */

#ifndef lint
#define getc(p)		(--(p)->_cnt < 0 ? _filbuf(p) : (int) *(p)->_ptr++)
#define putc(x, p)	(--(p)->_cnt < 0 ? \
			_flsbuf((unsigned char) (x), (p)) : \
			(int) (*(p)->_ptr++ = (unsigned char) (x)))
#define getchar()	getc(stdin)
#define putchar(x)	putc((x), stdout)
#define clearerr(p)	((void) ((p)->_flag &= ~(_IOERR | _IOEOF)))
#define feof(p)		((p)->_flag & _IOEOF)
#define ferror(p)	((p)->_flag & _IOERR)
#define fileno(p)	(p)->_file
#endif

extern FILE	_iob[_NFILE];
extern FILE	*fopen(), *fdopen(), *freopen(), *popen(), *tmpfile();
extern long	ftell();
extern void	rewind(), setbuf();
extern char	*ctermid(), *cuserid(), *fgets(), *gets(), *tempnam(), *tmpnam();
extern int	fclose(), fflush(), fread(), fwrite(), fseek(), fgetc(),
		getw(), pclose(), printf(), fprintf(), sprintf(),
		vprintf(), vfprintf(), vsprintf(), fputc(), putw(),
		puts(), fputs(), scanf(), fscanf(), sscanf(),
		setvbuf(), system(), ungetc();
#ifndef DYNAMIC

extern unsigned char *_bufendtab[];

#endif /* DYNAMIC */

#define L_ctermid	9
#define L_cuserid	9
#define P_tmpdir	"/usr/tmp/"
#define L_tmpnam	(sizeof(P_tmpdir) + 15)
#endif

#endif /* SYS_STDIO_H */
