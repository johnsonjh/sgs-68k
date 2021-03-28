/*	ustat.h	1.1	*/
/*	3.0 SID #	1.1	*/


#ifndef USTAT_H
#ident	"@(#)ustat.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define USTAT_H

struct  ustat {
	daddr_t	f_tfree;	/* total free */
	ino_t	f_tinode;	/* total inodes free */
	char	f_fname[6];	/* filsys name */
	char	f_fpack[6];	/* filsys pack name */
};

#endif /* USTAT_H */
