/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) debug.h: version 25.1 created on 12/2/91 at 17:40:41	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)debug.h	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident	"@(#)f77/f77optim/mach:debug.h	25.1"

extern int eflag;
#define TRACE(F)	if (eflag) printf("# " #F " \n")
