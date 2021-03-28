/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) getoken.c: version 25.1 created on 12/2/91 at 19:41:43	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)getoken.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libns:getoken.c	1.3"	*/
	
#ident	"@(#)libns:getoken.c	25.1"


#include "nserve.h"
#include "sys/cirmgr.h"

int
getoken(s)
struct token *s;
{
	char *sp;

	s->t_id = 0;
	sp = (char *)&s->t_uname[0];
	return(netname(sp));
}
