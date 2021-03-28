/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) nlsenv.h: version 25.1 created on 12/2/91 at 19:41:20	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)nlsenv.h	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	AT&T: #ident	"listen:nlsenv.h	1.2"		*/

#ident	"@(#)listen:nlsenv.h	23.1"


/*
 * these are names of environment variables that the listener
 * adds to the servers environment before the exec(2).
 *
 * the variables should be accessed via library routines.
 *
 * see nlsgetcall(3X) and nlsprovider(3X).
 */

#define	NLSADDR		"NLSADDR"
#define NLSOPT		"NLSOPT"
#define NLSUDATA	"NLSUDATA"
#define NLSPROVIDER	"NLSPROVIDER"

/*
 * the following variables can be accessed "normally"
 */

#define	HOME		"HOME"
#define PATH		"PATH"


