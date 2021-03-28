/*	Copyright (c) 1989 ARIX CORP */
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX CORP. */
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ARIX: #ident	"head:utime.h	1.5"			*/
/*	3.0 SID #	1.2	*/

/* This header file was created for POSIX 1003.1 compliance */

#ifdef _POSIX_SOURCE

#ifndef	UTIME_H
#ident	"@(#)utime.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define	UTIME_H

struct	utimbuf {
	time_t	actime;
	time_t	modtime;
};

#endif	/* UTIME_H */

#endif /* _POSIX_SOURCE */
