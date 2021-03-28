/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#ifndef _ASSERT_H
#ident	"@(#)assert.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define _ASSERT_H
#pragma SYS

/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#if defined NDEBUG 

#define assert(EX) ((void) 0)

#else	/* NDEBUG */

#if __STDC__ > 0
#define assert(EX) ((void) ((EX) || _assert(#EX, __FILE__, __LINE__)))
#else
#define assert(EX) ((void) ((EX) || _assert("EX", __FILE__, __LINE__)))
#endif	/* __STDC__ */

#endif	/* NDEBUG */

/*****************************************************************************
                   E X T E R N A L    D E C L A R A T I O N S 
 ****************************************************************************/

#if !defined NDEBUG

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void _assert _PARMS((char *,char *,int));/*For use by assert macro only*/

#endif	/* NDEBUG */

#pragma noSYS
#endif	/* _ASSERT_H */

