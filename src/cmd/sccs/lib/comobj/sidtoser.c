/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sidtoser.c: version 25.1 created on 12/2/91 at 17:11:07	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sidtoser.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/
/*	ATT: #ident	"sccs:lib/comobj/sidtoser.c	6.2"	*/

#ident	"@(#)sccs:sidtoser.c	25.1"

# include	"../../hdr/defines.h"


sidtoser(sp,pkt)
register struct sid *sp;
struct packet *pkt;
{
	register int n;
	register struct idel *rdp;

	for (n = maxser(pkt); n; n--) {
		rdp = &pkt->p_idel[n];
		if (rdp->i_sid.s_rel == sp->s_rel &&
			rdp->i_sid.s_lev == sp->s_lev &&
			rdp->i_sid.s_br == sp->s_br &&
			rdp->i_sid.s_seq == sp->s_seq)
				break;
	}
	return(n);
}
