/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) callo.h: version 24.1 created on 10/28/91 at 17:59:46	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)callo.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef IOPM_CALLO_H
#define IOPM_CALLO_H

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

/*
 *	The callout structure is for a routine arranging
 *	to be called by the clock interrupt
 *	(clock.c) with a specified argument,
 *	in a specified amount of time.
 *	Used, for example, to time tab delays on typewriters.
 */

struct icallo
{
	struct icallo *c_next;		/* linked list of call outs */
	int           c_time;		/* incremental time */
	uint          c_id;		/* timeout id */
	caddr_t       c_arg;		/* argument to routine */
	void          (*c_func)();	/* routine */
};

#endif /* IOPM_CALLO_H */
