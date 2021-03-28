/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) zmknod.c: version 25.1 created on 12/2/91 at 19:36:11	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)zmknod.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:zmknod.c	1.2"	*/
	
#ident	"@(#)libgen:zmknod.c	25.1"


/*	mknod(2) with error checking
*/

#include	<stdio.h>
#include	"errmsg.h"

int
zmknod( severity, path, mode, dev )
int	severity;
char	*path;
int	mode;
int	dev;
{

	int	err_ind;

	if( (err_ind = mknod(path, mode, dev )) == -1 )
	    _errmsg("UXzmknod1", severity,
		  "Cannot create file \"%s\" of mode %d on device %d.",
		   path, mode, dev);

	return err_ind;
}
