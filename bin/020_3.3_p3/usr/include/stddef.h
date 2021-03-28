/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#if !defined __STRICT_STDC__
#include "sys/types.h"
#endif /* ! __STRICT_STDC__ */

#ifndef _STDDEF_H
#ident	"@(#)stddef.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define _STDDEF_H
#pragma SYS


/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#define NULL	0	/* Also defined in locale, stdio, stdlib, string and time */

#define offsetof(structname,fieldname) (size_t) &(((structname *)0)->fieldname)


/*****************************************************************************
                                  T Y P E S
 ****************************************************************************/

typedef int ptrdiff_t;

#if  !defined _STDIO_H && !defined _STDLIB_H && !defined _STRING_H && !defined _TIME_H && !defined SYS_TYPES_H
typedef unsigned int size_t;	/* Also defined in stdio, stdlib, string and */
								/* time 									 */
#endif

#if !defined _STDLIB_H
typedef int wchar_t;			/* Also defined in stdlib 				 	 */
#endif


#pragma noSYS
#endif	/* _STDDEF_H */
