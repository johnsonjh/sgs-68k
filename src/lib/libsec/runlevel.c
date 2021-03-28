/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) runlevel.c: version 25.1 created on 12/2/91 at 20:14:52	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)runlevel.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*							*/
/*
 *	runlevel   return the current run level 
 *
 */

#include <sys/types.h>
#include <errno.h>
#include <utmp.h>

/*
 *   	runlevel   return the current runlevel or -1 
 *
 */

int runlevel()
{
struct utmp *ut, tmp, *getutid();

	tmp.ut_type = RUN_LVL;
	if (!(ut=getutid( &tmp))) 
		return -1;
	if ( ut->ut_exit.e_termination == 'S' )
		return 0;
	else
	     	 return (ut->ut_exit.e_termination-'0');
}
		
