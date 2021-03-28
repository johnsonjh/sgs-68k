#ifndef	SYS_TYPES_H
#define SYS_TYPES_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) types.h: version 24.1 created on 10/28/91 at 18:40:32	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)types.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#if defined(M68020) || defined(M68040)
#define	LABEL_T_LEN	13
#endif /* M68020 */

typedef	struct { int r[1]; } *	physadr;
typedef	long		daddr_t;	/* <disk address> type */
typedef	char *		caddr_t;	/* ?<core address> type */
typedef	unsigned char	unchar;
typedef unsigned char	uchar;
typedef	unsigned short	ushort;
typedef	unsigned int	uint;
typedef	unsigned long	ulong;
typedef	ushort		ino_t;		/* <inode> type */
typedef	short		cnt_t;		/* ?<count> type */
#if !defined _TIME_H
typedef	long		time_t;		/* <time> type */
#endif
#if defined(M68020) || defined(M68040)
typedef	int		label_t[LABEL_T_LEN];
#endif /* M68020 */
typedef	unsigned short	dev_t;		/* <old device number> type */
typedef	long		off_t;		/* ?<offset> type */
typedef	long		paddr_t;	/* <physical address> type */
typedef	int		key_t;		/* IPC key type */
typedef	unsigned char	use_t;		/* use count for swap.  */
typedef	short		sysid_t;
typedef	short		index_t;
typedef	short		lock_t;		/* lock work for busy wait */
#if  !defined _STDDEF_H && !defined _STDLIB_H && !defined _STRING_H && !defined _TIME_H && !defined _STDIO_H
typedef	unsigned int	size_t;		/* len param for string funcs */
#endif

#ifdef	_POSIX_SOURCE 
#if !defined _TIME_H
typedef uint clock_t;
#endif
typedef	short		pid_t;		/* Process ID */
typedef	ushort		gid_t;		/* Group ID */
typedef	ushort		uid_t;		/* User ID */
typedef	unsigned short	mode_t;		/* File permissions */
typedef	short		nlink_t;	/* Link count to file */
typedef uint sigset_t;
#endif


#endif	/* SYS_TYPES_H */
