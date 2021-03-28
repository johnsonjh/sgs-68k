#ifndef SYS_GCP_GCMUXT_H
#define SYS_GCP_GCMUXT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gcmuxt.h: version 2.1 created on 5/22/89 at 19:06:32	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gcmuxt.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys/gcp:gcmuxt.h	2.1"

struct gcmuxt {
	long		mt_time;
	unsigned char	mt_addr;
	unsigned char	mt_command;
	unsigned short	mt_count;
	unsigned char	mt_data[512];
};

#define GCMUXT_PRI		28

#define GCMUXT_IOCTL		('m'<<8)
#define GCMUXT_IOCTL_GET	(GCMUXT_IOCTL|1)
#define GCMUXT_IOCTL_SET	(GCMUXT_IOCTL|2)

#endif /* SYS_GCP_GCMUXT_H */
