#ifndef SYS_BUFSTAT_H
#define SYS_BUFSTAT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) bufstat.h: version 24.1 created on 10/28/91 at 18:31:22	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)bufstat.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

struct use_stat {
	uint  use;		/* in use now */
	uint  total;		/* total used since boot */
	uint  max;		/* max used at one time */
	uint  fail;		/* failures to allocate this resource */
};

#define BUMPUSE(X)	{X.use++; X.total++;\
			 X.max = (X.use > X.max ? X.use : X.max);}

struct bufstat {
	struct use_stat  bp;	/* buffer header w/o buffers */
	struct use_stat  buf;	/* buffers */
	struct use_stat  eblk;	/* geteblk requests */
};

#endif /* SYS_BUFSTAT_H */
