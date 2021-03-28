/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/



#ifndef _LIMITS_H 
#ident	"@(#)limits.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define _LIMITS_H
#pragma SYS

#ifndef SYS_PARAM_H
#include <sys/param.h>
#endif

/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#define SCHAR_MAX	127		/* max value of a "signed char"	*/
#define SCHAR_MIN	-128		/* min value of a "signed char"	*/
#define	CHAR_BIT	8		/* # of bits in a "char" */
#define	CHAR_MAX	SCHAR_MAX	/* max value of a "char" */
#define	CHAR_MIN	SCHAR_MIN	/* min value of a "char" */
#define	INT_MAX		2147483647	/* max value of an "int" */
#define	INT_MIN		-2147483648	/* min value of an "int" */
#define	LONG_MAX	2147483647	/* max value of a "long" */
#define	LONG_MIN	-2147483648	/* min value of a "long" */
#define	SHRT_MAX	32767		/* max value of a "short" */
#define	SHRT_MIN	-32768		/* min value of a "short" */
#define	MB_LEN_MAX	1		/* max # of bytes in multi-byte char */
#define UCHAR_MAX	255		/* max value of a "unsigned char" */
#define UINT_MAX	4294967295	/* max value of a "unsigned int" */
#define ULONG_MAX	UINT_MAX	/* max value of a "unsigned long" */
#define USHRT_MAX	65535		/* max value of a "unsigned short" */

#if !defined __STRICT_STDC__ 

#define HUGE_VAL 	1.79769313486231470e+308	/* from math.h */
										/* DBL_ & FLT_ macros from float.h */
#define DBL_DIG			16					
#define DBL_MAX			1.79769313486231470e+308
#define DBL_MIN			2.22507385850720282e-308
#define FLT_DIG			7
#define FLT_MAX			3.40282346638528860e+38
#define FLT_MIN			1.40129846432481707e-45

#define	ARG_MAX		NCARGS		/* max length of arguments to exec */
#define	CHILD_MAX	25		/* max # of processes per user id */
#define	CLK_TCK		50		/* # of clock ticks per second */
#define	FCHR_MAX	2097152		/* max size of a file in bytes */
#define	LINK_MAX	MAXLINK		/* max # of links to a single file */
#define	NAME_MAX	14		/* max # of characters in a file name */
#define	OPEN_MAX	64		/* max files a process can have open */
#define	PASS_MAX	8		/* max # of characters in a password */
#define	PATH_MAX	255		/* max # of characters in a path name */
#define	PID_MAX		MAXPID		/* max value for a process ID */
#define	PIPE_BUF	10240		/* max bytes atomic in write to pipe */
#define	PIPE_MAX	10240		/* max bytes written to pipe in write */
#define	STD_BLK		1024		/* # bytes in a physical I/O block */
#define	SYS_NMLN	9		/* chars in uname-returned strings */
#define	UID_MAX		MAXUID		/* max value for a user or group ID */
#define	USI_MAX		UINT_MAX	/* max decimal value of an "unsigned" */
#define	WORD_BIT	32		/* # of bits in a "word" or "int" */

/* fix for sur I0002316  X/OPEN issues*/
#define LOCK_MAX	30
#define LONG_BIT	32
#define MAX_CHAR	256
#define PROC_MAX	150
#define SYS_OPEN	175
#define SYSPID_MAX	3
#define TMP_MAX		17576

/* Run time invariant values */

#define MAX_INPUT	255
#define NGROUPS_MAX	 16
#define MAX_CANON	255


/* The following are POSIX defined minimum values */

#define _POSIX_ARG_MAX	4096		/* The length of the arguments for
					 * "exec()" in bytes, including
					 * environment data
					 */
#define _POSIX_CHILD_MAX  6		/* The number of simultaneous processes
					 * per user ID
					 */
#define _POSIX_LINK_MAX   8		/* The value of the files link count */
#define _POSIX_MAX_CANON  255		/* The number of bytes in a terminal
					 * canonical input queue
					 */
#define _POSIX_MAX_INPUT  255		/* The number of bytes for which space 
					 * is guaranteed to be available in a
					 * terminal input queue
					 */
#define _POSIX_NAME_MAX  14		/* The number of bytes in a filename */
#define _POSIX_NGROUPS_MAX  0		/* The number of simultaneous sup-
					 * plementary group IDs per process */
#define _POSIX_OPEN_MAX	  16		/* The number of files that one process
					 * can have open at one time
					 */
#define _POSIX_PATH_MAX   255		/* The number of bytes in a pathname */
#define _POSIX_PIPE_BUF   512		/* The number of bytes that is guaran-
					 * teed to be written atomically when 
					 * writing to a pipe
					 */


#endif	/* ! __STRICT_STDC__ */


#pragma noSYS
#endif	/* _LIMITS_H */
