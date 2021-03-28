/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	3.0 SID #	1.1	*/

#ifndef MNTTAB_H
#ident	"@(#)mnttab.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define MNTTAB_H

#define	NMOUNT	32

/* Format of the /etc/mnttab file which is set by the mount(1m)
 * command
 */
struct mnttab {
	char	mt_dev[32],
		mt_filsys[32];
	short	mt_ro_flg;
	time_t	mt_time;
	char	mt_fstyp[16],
		mt_mntopts[64];
};

#endif /* MNTTAB_H */
