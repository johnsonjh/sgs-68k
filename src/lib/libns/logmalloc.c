/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) logmalloc.c: version 25.1 created on 12/2/91 at 19:41:56	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)logmalloc.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libns:logmalloc.c	1.3"	*/
	
#ident	"@(#)libns:logmalloc.c	25.1"

#include <stdio.h>
#include "nslog.h"
#undef free
#undef malloc
#undef calloc
#undef realloc
char	*malloc();
char	*realloc();
char	*calloc();

void
xfree(p)
char	*p;
{
	LOG2(L_MALLOC,"free(0x%x)\n",p);
	fflush(Logfd);
	free(p);
	return;
}
char	*
xmalloc(size)
unsigned size;
{
	char	*ret;
	LOG2(L_MALLOC,"malloc(%d)",size);
	fflush(Logfd);
	ret = malloc(size);
	LOG2(L_MALLOC,"returns 0x%x\n",ret);
	return(ret);
}
char	*
xrealloc(p,size)
char	*p;
unsigned size;
{
	char	*ret;
	LOG3(L_MALLOC,"realloc(0x%x,%d) ",p,size);
	fflush(Logfd);
	ret = realloc(p,size);
	LOG2(L_MALLOC,"returns 0x%x\n",ret);
	return(ret);
}
char	*
xcalloc(n,size)
unsigned n,size;
{
	char	*ret;
	LOG3(L_MALLOC,"calloc(%d,%d) ",n,size);
	fflush(Logfd);
	ret = calloc(n,size);
	LOG2(L_MALLOC,"returns 0x%x\n",ret);
	return(ret);
}
