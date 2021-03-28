/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#ifndef _SIGNAL_H
#ident	"@(#)signal.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define _SIGNAL_H
#pragma SYS

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#define SIG_ERR 	(void (*) _PARMS((int)))-1
#define SIG_DFL 	(void (*) _PARMS((int)))0
#define	SIG_IGN		(void (*) _PARMS((int)))1
#define	SIGINT	2	/* interrupt (rubout) 							*/
#define	SIGILL	4	/* illegal instruction (not reset when caught)	*/
#define SIGABRT 6	/* used by abort, replace SIGIOT in the  future */
#define	SIGFPE	8	/* floating point exception 					*/
#define	SIGSEGV	11	/* segmentation violation 						*/
#define	SIGTERM	15	/* software termination signal from kill 		*/

#if !defined __STRICT_STDC__

#include "sys/signal.h"

#endif	/* ! __STRICT_STDC__ */


/*****************************************************************************
                                  T Y P E S
 ****************************************************************************/

typedef int sig_atomic_t;


/*****************************************************************************
                   E X T E R N A L    D E C L A R A T I O N S 
 ****************************************************************************/

void (*signal _PARMS((int,void (*)(int)))) _PARMS((int));
int raise _PARMS((int));

#if !defined __STRICT_STDC__

void (*sigset _PARMS((int,void (*)(int)))) _PARMS((int));
int sighold _PARMS((int));
int sigignore _PARMS((int));
int sigrelse _PARMS((int));
int sigpause _PARMS((int));

#endif /* ! __STRICT_STDC__ */

#pragma noSYS
#endif /* _SIGNAL_H */
