/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) getrolent.c: version 25.1 created on 12/2/91 at 20:14:06	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)getrolent.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*
 *	getrole.c   routines to parse the role database
 *	(C)Copyright 1990 by Jeff Tofano
 *
 */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <sys/priv.h>
#include <sys/mls.h>
#include <sys/stat.h>
#include <sys/sakioctl.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <param.h>
#include <limits.h>
#include <auth.h>

static int rlfd;

/*
 *	setrlent     
 *
 */

setrlent()
{
	if (!rlfd) 
		if (( rlfd = open( ROLES, O_RDWR | O_CREAT,0600 )) < 0 )
			return -1;
	
	lseek( rlfd, 0L, 0 );
	return rlfd;
}

/*
 *	endrlent
 *
 */

endrlent()
{
int rval;

	if (rlfd) {
		rval = close(rlfd);
		rlfd = 0;
	}
	return rval;
}
	

roldef_t *getrolent()
{
static roldef_t ra;
struct flock lck;

	if (!rlfd)
		if ( setrlent() < 0 )
			return NULL;

	/* initialize the file lock structure */
	lck.l_type = F_RDLCK;
	lck.l_whence = 0;
	lck.l_start  = lseek( rlfd, 0, 1 );
	lck.l_len = sizeof(roldef_t);

	/* if lock failed return a null */
	if ( fcntl( rlfd, F_SETLKW, &lck ) < 0 ) {
		lck.l_type = F_UNLCK;
		fcntl( rlfd, F_SETLK, &lck );
		return NULL;
	}	
	if ( read( rlfd, &ra, sizeof( roldef_t)) != sizeof(roldef_t))
		return NULL;

	lck.l_type = F_UNLCK;
	fcntl( rlfd, F_SETLKW, &lck );
	return &ra;
}


/*
 *	getrolnam  search the database for the named item 
 *
 */

roldef_t *getrolnam(name)
char *name;
{
roldef_t *ra, *getrolent();

	if (!rlfd)
		if ( setrlent() < 0 )
			return NULL;

	/* read each entry until we find the proper one */
	while ( ra = getrolent() ) {
		if ( !strncmp( ra->r_name, name, NAME_MAX ))
			return ra;
	}
	endrlent();
	return NULL;
}

/*
 *	putrlent  write the role record in the current position 
 *                in the role database.
 *
 */

putrlent( fd, ra )
int fd;
roldef_t *ra;
{
struct flock lck;
int rval;

	lck.l_whence = 0;
	lck.l_start = lseek( fd, 0, 1 );
	lck.l_type = F_WRLCK;
	lck.l_len = sizeof( roldef_t );

	if ( fcntl( fd, F_SETLKW, &lck ) < 0 ) {
		lck.l_type = F_UNLCK;
		fcntl( fd, F_SETLK, &lck );
		return -1;
	}	
	rval = write( fd, ra, sizeof(roldef_t));
	lck.l_type = F_UNLCK;
	fcntl( fd, F_SETLK, &lck );
	return rval;
}



/*
 *	updaterole  write the role record in it proper position 
 *                in the role database.
 *
 */

updaterole( ra )
roldef_t *ra;
{
struct flock lck;
int rval;

	if (!rlfd)
		if ( setrlent() < 0 )
			return -1;

	lck.l_whence = 0;
	lck.l_start = lseek( rlfd, ra->r_rolenumber*sizeof(roldef_t), 0 );
	lck.l_type = F_WRLCK;
	lck.l_len = sizeof( roldef_t );

	if ( fcntl( rlfd, F_SETLKW, &lck ) < 0 ) {
		lck.l_type = F_UNLCK;
		fcntl( rlfd, F_SETLK, &lck );
		return -1;
	}	
	rval = write( rlfd, ra, sizeof(roldef_t));
	lck.l_type = F_UNLCK;
	fcntl( rlfd, F_SETLK, &lck );
	return rval;
}



