#ident	"@(#)getfh_i.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#ident	"@(#)getfh_i.c	3.1 LAI System V NFS Release 3.0/V3 source"
#include <stdio.h>
#include <fcntl.h>

/* NFS
char nfsd[] = "/dev/nfsd";
*/
char nfsd[] = "/dev/rnfsd";

nfs_getfh(fd, fhp)
	int	fd;
	char	*fhp;
{
	int nfsdev, retval, serrno;
	extern int errno;
	struct {
		int fdes;
		char *fhp;
	} args;

	args.fdes = fd;
	args.fhp = fhp;
        if ((nfsdev = open(nfsd, O_RDWR)) < 0)
		return(-1);
        retval = ioctl(nfsdev, 2, &args);
	serrno = errno;
	(void) close(nfsdev);
	errno = serrno;
	return(retval);
}
