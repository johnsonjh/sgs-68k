/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) setup.c: version 25.1 created on 12/2/91 at 17:10:57	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)setup.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/
/*	ATT: #ident	"sccs:lib/comobj/setup.c	6.2"	*/

#ident	"@(#)sccs:setup.c	25.1"

# include	"../../hdr/defines.h"


setup(pkt,serial)
register struct packet *pkt;
int serial;
{
	register int n;
	register struct apply *rap;
	int	first_app   =   1;

	pkt->p_apply[serial].a_inline = 1;
	for (n = maxser(pkt); n; n--) {
		rap = &pkt->p_apply[n];
		if (rap->a_inline) {
			if (n != 1 && pkt->p_idel[n].i_pred == 0)
				fmterr(pkt);
			pkt->p_apply[pkt->p_idel[n].i_pred].a_inline = 1;
			if (pkt->p_idel[n].i_datetime > pkt->p_cutoff)
				condset(rap,NOAPPLY,CUTOFF);
			else {
				if (first_app)
					pkt->p_gotsid = pkt->p_idel[n].i_sid;
				first_app = 0;
				condset(rap,APPLY,SX_EMPTY);
			}
		}
		else
			condset(rap,NOAPPLY,SX_EMPTY);
		if (rap->a_code == APPLY)
			ixgsetup(pkt->p_apply,&(pkt->p_idel[n].i_ixg));
	}
}


ixgsetup(ap,ixgp)
struct apply *ap;
struct ixg *ixgp;
{
	int n;
	int code, reason;
	register int *ip;
	register struct ixg *cur, *prev;

	for (cur = ixgp; cur = (prev = cur)->i_next; ) {
		switch (cur->i_type) {

		case INCLUDE:
			code = APPLY;
			reason = INCL;
			break;
		case EXCLUDE:
			code = NOAPPLY;
			reason = EXCL;
			break;
		case IGNORE:
			code = SX_EMPTY;
			reason = IGNR;
			break;
		}
		ip = cur->i_ser;
		for (n = cur->i_cnt; n; n--)
			condset(&ap[*ip++],code,reason);
	}
}


condset(ap,code,reason)
register struct apply *ap;
int code, reason;
{
	if (code == SX_EMPTY)
		ap->a_reason |= reason;
	else if (ap->a_code == SX_EMPTY) {
		ap->a_code = code;
		ap->a_reason |= reason;
	}
}
