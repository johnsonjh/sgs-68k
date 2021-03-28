/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#ifndef _ERRNO_H 
#ident	"@(#)errno.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define _ERRNO_H
#pragma SYS


/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#define EDOM      33	/* Math arg out of domain of func	*/
#define ERANGE    34	/* Math result not representable	*/

#if !__STRICT_STDC__

#include "sys/errno.h"

#endif	/* ! __STRICT_STDC__ */


/*****************************************************************************
                   E X T E R N A L    D E C L A R A T I O N S 
 ****************************************************************************/

#if __STDC__ > 0
extern volatile int errno;
#else
extern int errno;
#endif

#pragma noSYS
#endif /* _ERRNO_H */
