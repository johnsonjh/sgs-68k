/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) resource.h: version 24.1 created on 10/28/91 at 18:37:18	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)resource.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_RESOURCE_H
#define SYS_RESOURCE_H

/*
 * Resource limits
 */

#ifdef not_supported
#define	RLIMIT_CPU	0	/* cpu time (milliseconds) */
#define	RLIMIT_FSIZE	1	/* file size */
#define	RLIMIT_DATA	2	/* data segment size */
#define	RLIMIT_STACK	3	/* stack segment size */
#define	RLIMIT_CORE	4	/* core file size */
#define	RLIMIT_VMEM	6	/* mapped memory */
#endif

#define RLIMIT_NOFILE	5		/* open file descriptors */

#define	RLIM_NLIMITS	7		/* number of resource limits */

typedef struct rlimit {
	uint	rlim_cur;		/* current configured limit */
	uint	rlim_max;		/* maximum configurable value */
} rlimit_t;

#ifdef INKERNEL
extern	rlimit_t	default_rlimits[];
#endif

#endif	/* SYS_RESOURCE_H */
