/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) uparm.h: version 25.1 created on 12/2/91 at 19:28:11	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)uparm.h	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/uparm.h	1.2"	*/
	
#ident	"@(#)lib/libcurses:uparm.h	25.1"

/*
 * Local configuration of various files.  Used if you can't put these
 * things in the standard places or aren't the super user, so you
 * don't have to modify the source files.  Thus, you can install updates
 * without having to re-localize your sources.
 *
 * This file used to go in /usr/include/local/uparm.h.  Every version of
 * UNIX has undone this, so now it wants to be installed in each source
 * directory that needs it.  This means you now include "uparm.h" instead
 * of "local/uparm.h".
 */

/* Path to library files */
#define libpath(file) "/usr/lib/file"

/* Path to local library files */
#define loclibpath(file) "/usr/local/lib/file"

/* Path to binaries */
#define binpath(file) "/usr/ucb/file"

/* Path to things under /usr (e.g. /usr/preserve) */
#define usrpath(file) "/usr/file"

/* Location of termcap file */
#define E_TERMCAP	"/etc/termcap"

/* Location of terminfo source file */
#define E_TERMINFO	"./terminfo.src"

/* Location of terminfo binary directory tree */
#define _TERMPATH(file)	"/usr/lib/terminfo/file"

/* Location of the C shell */
#define B_CSH		"/bin/csh"
