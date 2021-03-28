#ifndef SYS_FS_S5DIR_H
#define SYS_FS_S5DIR_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) s5dir.h: version 24.1 created on 10/28/91 at 18:32:44	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)s5dir.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/fs/s5dir.h	10.1"	*/

#ifndef	DIRSIZ
#define	DIRSIZ	14
#endif
struct	direct
{
	unsigned short	d_ino;
	char		d_name[DIRSIZ];
};

#endif /* SYS_FS_S5DIR_H */
