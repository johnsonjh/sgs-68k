/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) rolelib.c: version 25.1 created on 12/2/91 at 20:14:49	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)rolelib.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*							*/
/*
 *	rolelib    some general purpose routines intended to help
 *		   out role processing. This set of routines rely
 *		   on the default getrolent library. 
 *	
 */
#include <sys/param.h>
#include <sys/types.h>
#include <sys/mount.h>

#include <sys/mls.h>
#include <sys/priv.h>
#include <sys/security.h>
#include <auth.h>

/*
 *	get_role   get the role from the current process.
 *		   This routine returns the role number else -1  
 */
get_role( role )
uint *role;
{
ushort tmp;

	if (kernpriv( GETROLE, &tmp, (char *) 0 ))
		return -1;
	*role = tmp;
	return 0;
}

/*
 *	set_role  set the role of the current process, forcing the 
 *		  associated privs and labels to be reset
 *
 */
set_role( role )
int *role;
{
ushort tmp;

	tmp = *role;
	return kernpriv( SETROLE, &tmp, (char *) 0 );
}

/*
 *	cvt_role_sym   convert a rolenumber into a symbolic rolename
 *		       return 0 is okay else -1 for an error
 */
cvt_role_sym( role, buf )
int role;
char *buf;
{
roldef_t *rp, *getrolent();

	/* search the role database */
	while ( rp = getrolent() ) {
		if ( rp->r_rolenumber == role ) {
			strcpy( buf, rp->r_name );
			return 0;
		}
	}
	return -1;
}

/*
 *	cvt_sym_role   convert the role symbol to a role name
 */
cvt_sym_role( sym )
char *sym;
{
roldef_t *rp, *getrolent();

	/* search the database for a name match */
	while ( rp = getrolent() ) {
		if (!strcmp( sym, rp->r_name )) 
			return rp->r_rolenumber;
	}
	return -1;
}



			
