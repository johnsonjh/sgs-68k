/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:screen/unctrl.h	1.3"	*/
	

/*
 * unctrl.h
 *
 */

#ifndef UNCTRL_H
#ident	"@(#)unctrl.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define	UNCTRL_H

extern char	*_unctrl[];

#if	!defined(NOMACROS) && !defined(lint)

#define	unctrl(ch)	(_unctrl[(unsigned) ch])

#endif	/* NOMACROS && lint */

#endif	/* UNCTRL_H */
