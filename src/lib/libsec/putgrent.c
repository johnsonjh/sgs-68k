/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) putgrent.c: version 25.1 created on 12/2/91 at 20:14:46	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)putgrent.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include <sys/param.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <grp.h>

#define ferror(p)       ((p)->_flag & _IOERR)

static char line[BUFSIZ+1] ;

int
putgrent(p, f)
register struct group *p ;
register FILE *f ;
{
	(void) fprintf( f,"%s:%s:%d:", p->gr_name, p->gr_passwd, p->gr_gid);

	while ( *p->gr_mem != NULL ){
		(void) fprintf( f,"%s", *p->gr_mem );
		*p->gr_mem++;
		if ( *p->gr_mem != NULL )
			(void) fprintf( f,"," );
	} /* while */

	(void) fprintf ( f, "\n");

	return(ferror(f)) ;
}
