/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sid_ab.c: version 25.1 created on 12/2/91 at 17:11:00	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sid_ab.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/
/*	ATT: #ident	"sccs:lib/comobj/sid_ab.c	6.2"	*/

#ident	"@(#)sccs:sid_ab.c	25.1"

# include	"../../hdr/defines.h"


char *
sid_ab(p,sp)
register char *p;
register struct sid *sp;
{
	extern	char	*satoi();
	if (*(p = satoi(p,&sp->s_rel)) == '.')
		p++;
	if (*(p = satoi(p,&sp->s_lev)) == '.')
		p++;
	if (*(p = satoi(p,&sp->s_br)) == '.')
		p++;
	p = satoi(p,&sp->s_seq);
	return(p);
}
