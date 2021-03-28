#ident	"@(#)exit.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1989 ARIX Corp.	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ARIX Corp.	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/* atexit and exit functions as required by ANSI C STANDARD */

#include <stdlib.h>
#include <syscall.h>
#include <_stdlib.h>

#define _MAX_IND	32
#define _MAX_INDp1	( _MAX_IND + 1 )

/* first pointer initialised to NULL for exit */
static void (*func_array[ _MAX_INDp1 ]) (void) = { NULL };

static short int index	= 0;

int atexit ( void (*func) (void))
{
	if ( index < _MAX_IND ) {	/* enough place for another function */
		func_array [ ++index ] = func;
		return ( 0 );			/* return success */
	}

	return ( 1 ); 				/* not enough place, return failure */
}

void exit ( int status )
{
	register void (*p)();

	if ( index ) {				/* 1 or more function to call */
		while ( (p = func_array[index--]) != NULL )
			(void) (*p)();
	}

	 ( void ) _cleanup();

	 ( void ) _exit ( status );
}
