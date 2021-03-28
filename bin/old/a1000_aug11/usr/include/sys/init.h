#ifndef SYS_INIT_H
#define SYS_INIT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) init.h: version 2.1 created on 5/22/89 at 19:07:45	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)init.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* init.h	6.3 */

#ident	"@(#)uts/head/sys:init.h	2.1"

extern int cinit(),binit(),iinit(),inoinit();
extern int fsinit();
extern int finit(), flckinit();
extern int strinit();

/*	Array containing the addresses of the various initializing	*/
/*	routines executed by "main" at boot time.			*/

int (*init_tbl[])() = {
	cinit,
	inoinit,
	fsinit,
	finit,
	iinit,
	flckinit,
	strinit,
	0
};

#endif /* SYS_INIT_H */
