/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Err.c: version 25.1 created on 12/2/91 at 19:31:30	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Err.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"liberr:Err.c	1.1"	*/
	
#ident	"@(#)libgen:Err.c	25.1"


/*	Set default values in Err global structure.
*/

#include	"errmsg.h"

static	char	deftofix[] = "Refer to help error database or manual.";
static	char	*defsevmsg[] = {	/* default severity messages */
		"INFORM: ",
		"WARNING: ",
		"ERROR: ",
		"HALT: ",
		0
	};

struct Err	Err = {
					/* verbosity flags */
		/* vbell */	ENO,
		/* vprefix */	EYES,
		/* vsource */	EYES,
		/* vsevmsg */	EYES,
		/* vsyserr */	EDEF,
		/* vfix */	EYES,
		/* vtag */	EYES,
		/* vtext */	EYES,
					/* message content */
		/* prefix */	0,
		/* envsource */	0,
		/* source */	0,
		/* severity */	0,
		/* sevmsg */	defsevmsg,
		/* tofix */	deftofix,
		/* tagnum */	0,
		/* tagstr */	0,
		/* exit */	1,
};

