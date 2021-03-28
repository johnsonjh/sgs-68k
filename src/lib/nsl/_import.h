/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) _import.h: version 25.1 created on 12/2/91 at 20:16:32	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)_import.h	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libnsl:nsl/_import.h	1.3"	*/
	
#ident	"@(#)lib/nsl:_import.h	25.1"

#ifdef SHLIB
#define free		(*__free)
#define calloc 		(*__calloc)
#define perror		(*__perror)
#define strlen		(*__strlen)
#define write		(*__write)
#define ioctl		(*__ioctl)
#define putmsg		(*__putmsg)
#define getmsg		(*__getmsg)
#define errno		(*__errno)
#define memcpy		(*__memcpy)
#define fcntl		(*__fcntl)
#define sigset		(*__sigset)
#define open		(*__open)
#define close		(*__close)
#define ulimit		(*__ulimit)
#endif
