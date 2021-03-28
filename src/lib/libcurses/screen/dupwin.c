/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) dupwin.c: version 25.1 created on 12/2/91 at 19:19:06	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)dupwin.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/dupwin.c	1.5"	*/
	
#ident	"@(#)lib/libcurses:dupwin.c	25.1"

#include	"curses_inc.h"

/*
 * Duplicate a window.
 *
 * SS:	calling makenew to allocate a new window is wastefull, since
 *	makenew initializes all the variables, and then we re-initialize
 *	the desired values to these variables.
 */

WINDOW	*
dupwin(win)
register	WINDOW	*win;
{
    register	WINDOW	*new;
    register	int	i, nlines = win->_maxy, ncolumns = win->_maxx,
			line_size = nlines * sizeof (short);
    register	chtype	**wincp, **newcp;

    /* allocate storage for new window and do block copy of old one into new */ 

    if ((new = (WINDOW *) malloc(sizeof (WINDOW))) == NULL)
	 goto out0;

    (void) memcpy(new, win, sizeof (WINDOW));

    /* allocate storage for "malloced" fields of the new window */

    if ((new->_firstch = (short *) malloc((unsigned) 2 * line_size)) == NULL)
         goto out1;
    else 
	win->_lastch = win->_firstch + nlines;

    if ((new->_y = (chtype **) malloc(nlines * sizeof (chtype *))) == NULL)
    {
	/*
	 * We put the free's here rather than after the image call, this
	 * is because _image free's all the rest of the malloc'ed areas.
	 */
	free((char *) new->_firstch);
out1:
	free((char *) new);
        goto out0;
    }

    if (_image(new) == ERR)
    {
out0:
	curs_errno = CURS_BAD_MALLOC;
#ifdef	DEBUG
	strcpy(curs_parm_err, "dupwin");
	curserr();
#endif	/* DEBUG */
	return ((WINDOW *) NULL);
    }

    /* copy information from "malloced" areas of the old window into new */

    wincp = win->_y;
    newcp = new->_y;
    ncolumns *= sizeof(chtype);
    for (i = 0; i < nlines; ++i, ++wincp, ++newcp)
	(void) memcpy((char *) *newcp, (char *) *wincp, ncolumns);

    (void) memcpy((char *) new->_firstch, (char *) win->_firstch, 2 * line_size);

    new->_flags |= _WINCHANGED;
    new->_ndescs = 0;
    /*
     * Just like we don't create this window as a subwin if the one
     * sent is, we don't create a padwin.  Besides, if the user
     * calls p*refresh a padwin will be created then.
     */
    new->_padwin = new->_parent = (WINDOW *) NULL;
    new->_pary = new->_parx = -1;

    return (new);
}
