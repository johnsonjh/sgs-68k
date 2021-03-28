#ifndef SYS_FS_S5FBLK_H
#define SYS_FS_S5FBLK_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) s5fblk.h: version 24.1 created on 10/28/91 at 18:32:47	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)s5fblk.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/fs/s5fblk.h	1.1"	*/

struct	fblk
{
	int	df_nfree;
	daddr_t	df_free[NICFREE];
};

#endif /* SYS_FS_S5FBLK_H */
