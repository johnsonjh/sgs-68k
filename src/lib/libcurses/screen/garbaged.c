/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) garbaged.c: version 25.1 created on 12/2/91 at 19:19:20	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)garbaged.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)curses:screen/garbaged.c	1.1"
/*
    This routine indicates to curses that a screen line is garbaged and
    should be thrown away before having anything written over the top of it.
    It could be used for programs such as editors which want a command to
    redraw just a single line. Such a command could be used in cases where
    there is a noisy line and redrawing the entire screen would be subject
    to even more noise. Just redrawing the single line gives some semblance
    of hope that it would show up unblemished.

    To force the redrawing, we just fill the line up with garbage (-1's).
*/

#include <curses.ext>

garbagedlines (win, begline, numlines)
WINDOW *win;
int begline;
register int numlines;
{
	register struct line **cb = SP->cur_body;
	register struct line **sb = SP->std_body;
	register chtype *ch;
	register lastcolumn = columns;
	register int curcolumn;
	register int curline = win->_begy + begline + 1 + win->_yoffset;
	register int lastline = lines;
	extern struct line *_line_alloc();

	for ( ; numlines-- && (curline <= lastline); curline++) {
		if (!sb[curline]) {
			sb[curline] = _line_alloc ();
			sb[curline]->length = 0;
		}
		if (!cb[curline] || cb[curline] == sb[curline])
			cb[curline] = _line_alloc();
		cb[curline]->hash = -1;
		cb[curline]->length = cb[curline]->bodylen = columns;
		for (ch = &(cb[curline]->body[0]), curcolumn = 0; 
		     curcolumn < lastcolumn; curcolumn++)
		    *ch++ = (chtype) -1;
	}
}
