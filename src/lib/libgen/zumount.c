/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zumount.c: version 25.1 created on 12/2/91 at 19:36:52	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zumount.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zumount.c	1.2"	*/
	
#ident	"@(#)libgen:zumount.c	25.1"


/*	umount(2) with error checking
*/

#include	<stdio.h>
#include	"errmsg.h"

int
zumount( severity, spec )
int	severity;
char	*spec;
{

	int	err_ind;

	if( (err_ind = umount( spec )) == -1 )
	    _errmsg("UXzumount1", severity,
		  "Cannot unmount file system, on device \"%s\".",
		   spec);

	return err_ind;
}
