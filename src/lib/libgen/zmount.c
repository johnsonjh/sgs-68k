/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zmount.c: version 25.1 created on 12/2/91 at 19:36:14	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zmount.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zmount.c	1.2"	*/
	
#ident	"@(#)libgen:zmount.c	25.1"


/*	mount(2) with error checking
*/

#include	"errmsg.h"

int
zmount( severity, spec, dir, rwflag )
int	 severity;
char	*spec;
char	*dir;
int	 rwflag;
{

	int	err_ind;

	if( (err_ind = mount(spec, dir, rwflag )) == -1 )
	    _errmsg ( "UXzmount1", severity,
		  "Cannot mount file system on block \"%s\" to directory \"%s\" , rwflag=%d.",
		   spec, dir, rwflag);

	return err_ind;
}
