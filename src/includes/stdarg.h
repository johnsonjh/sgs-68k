/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#ifndef _STDARG_H
#ident	"@(#)stdarg.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define _STDARG_H
#pragma SYS


/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

/* Note: we must take default promotion of narrow types into consideration: */
/* __argtype (extended C Language) operator takes care of this. */

#define va_start(listptr,parmN) (void) ( listptr = 						\
				(va_list) ((va_list) &parmN + sizeof(__argtype(parmN)) ) )

#define va_arg(listptr,typename) 							\
		( (typename) ( (__argtype(typename) *)				\
			(listptr += sizeof(__argtype(typename))))[-1] )

#define va_end(listptr)


/*****************************************************************************
                                  T Y P E S
 ****************************************************************************/

#if !defined _STDIO_H 
typedef char *va_list; /* Also defined in stdio.h	*/
#endif


#pragma noSYS
#endif	/* _STDARG_H */
