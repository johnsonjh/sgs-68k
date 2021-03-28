/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) llib-lmall.c: version 25.1 created on 12/2/91 at 19:40:56	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)llib-lmall.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libmalloc:llib-lmall.c	1.3"	*/
	
#ident	"@(#)libmalloc:llib-lmall.c	25.1"

#include "malloc.h"
/*	Lint Library for Malloc(3x)	*/
/*	MALLOC(3X)	*/
/*	malloc, calloc, realloc and free are checked
/*	by the c library lint file
*/
int mallopt (cmd, value) int cmd, value; { return cmd+value; }
struct mallinfo mallinfo () { struct mallinfo s; return (s); }
