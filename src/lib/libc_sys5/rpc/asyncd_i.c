#ident	"@(#)asyncd_i.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#ident	"@(#)asyncd_i.c	1.1 System V NFS source"
#include <stdio.h>
#include <fcntl.h>

/* NFS
char nfsd[] = "/dev/nfsd";
*/
char nfsd[] = "/dev/rnfsd";

async_daemon()
{
	int nfsdev, retval, serrno;
	extern int errno;

        if ((nfsdev = open(nfsd, O_RDWR)) < 0)
		return(-1);
        retval = ioctl(nfsdev, 3, 0);
	serrno = errno;
	(void) close(nfsdev);
	errno = serrno;
	return(retval);
}
