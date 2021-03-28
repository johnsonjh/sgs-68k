/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#ifndef _SETJMP_H
#ident	"@(#)setjmp.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define _SETJMP_H
#pragma SYS

/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#if defined m68 || defined m68k || defined M68020
#define _JBLEN	13
#elif defined vax || defined u3b5
#define _JBLEN	10
#elif defined pdp11
#define _JBLEN	3
#elif defined u370
#define _JBLEN	4
#elif defined u3b
#define _JBLEN	11
#else
#define _JBLEN  13
#endif

/* ANSI/POSIX require macros implementation for sigjmp and sigsetjmp */
#define setjmp(env)				_setjmp(env)

#if ! __STRICT_STDC__ && ! _SYSV
#define sigsetjmp(env,mask)		_sigsetjmp(env,mask)
#endif	/*  ! __STRICT_STDC__  && ! _SYSV */

/*****************************************************************************
                                  T Y P E S
 ****************************************************************************/

typedef int jmp_buf[_JBLEN];

#if ! __STRICT_STDC__  && ! _SYSV

/* Added for POSIX 1003.1 comp. */
typedef	int	sigjmp_buf[_JBLEN + 2];

#endif	/*  ! __STRICT_STDC__  && ! _SYSV */


/*****************************************************************************
                   E X T E R N A L    D E C L A R A T I O N S 
 ****************************************************************************/

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int 	_setjmp _PARMS((jmp_buf));
void longjmp _PARMS((jmp_buf,int));

#if ! __STRICT_STDC__ && ! _SYSV

extern	int		_sigsetjmp ();
extern	void	siglongjmp ();

#endif	/*  ! __STRICT_STDC__  && ! _SYSV */


#pragma noSYS
#endif	/* _SETJMP_H */
