/*	grp.h	1.1	*/


#ifndef	GRP_H
#ident	"@(#)grp.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define	GRP_H

#include <sys/types.h>

/*	3.0 SID #	1.1	*/
/* gid_t added for POSIX 1003.1 compliance */

struct	group {	/* see getgrent(3) */
	char	*gr_name;
	char	*gr_passwd;

#ifdef	_POSIX_SOURCE
	ushort	:16;	/* required for proper alignment */
	gid_t	gr_gid;
#else	/* _POSIX_SOURCE */
	int	gr_gid;
#endif  /*_POSIX_SOURCE */

	char	**gr_mem;
};

extern	struct	group *getgrent ();
extern	struct	group *getgrgid ();
extern	struct	group *getgrnam ();
extern	struct	group *fgetgrent ();

#endif	/* GRP_H */
