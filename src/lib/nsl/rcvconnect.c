/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) rcvconnect.c: version 25.1 created on 12/2/91 at 20:17:17	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)rcvconnect.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"libnsl:nsl/t_rcvconnect.c	1.2.1.1"*/

#ident	"@(#)lib/nsl:rcvconnect.c	25.1"

#include "sys/types.h"
#include "sys/timod.h"
#include "sys/tiuser.h"
#include "sys/param.h"
#include "_import.h"

extern int t_errno;
extern rcv_conn_con();
extern struct _ti_user *_t_checkfd();


t_rcvconnect(fd, call)
int fd;
struct t_call *call;
{
	register struct _ti_user *tiptr;
	int retval;

	if ((tiptr = _t_checkfd(fd)) == NULL)
		return(-1);

	if (((retval = _rcv_conn_con(fd, call)) == 0) || (t_errno == TBUFOVFLW))
		tiptr->ti_state = TLI_NEXTSTATE(T_RCVCONNECT, tiptr->ti_state);
	return(retval);
}
