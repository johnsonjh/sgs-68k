/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) getsgent.c: version 25.1 created on 12/2/91 at 20:14:09	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)getsgent.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include <sys/param.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <auth.h>

#define ferror(p)       ((p)->_flag & _IOERR)

static FILE *sgf = NULL ;
static char line[BUFSIZ+1] ;
static struct sgroup sgroup ;

void
setsgent()
{
	if(sgf == NULL) {
		sgf = fopen(SGRPFLE, "r") ;
	}
	else
		rewind(sgf) ;
}

void
endsgent()
{
	if(sgf != NULL) {
		(void) fclose(sgf) ;
		sgf = NULL ;
	}
}

static char *
sgskip(p)
register char *p ;
{
	while(*p && *p != ':' && *p != '\n')
		++p ;
	if(*p == '\n')
		*p = '\0' ;
	else if(*p)
		*p++ = '\0' ;
	return(p) ;
}

/* 	The getsgent function will return a NULL for an end of 
	file indication or a bad entry
*/

struct sgroup *
getsgent()
{
	extern struct sgroup *fgetsgent() ;

	if(sgf == NULL) {
		if((sgf = fopen(SGRPFLE, "r")) == NULL)
			return (NULL) ;
	}
	return (fgetsgent(sgf)) ;
}

struct sgroup *
fgetsgent(f)
FILE *f ;
{
	register char *p ;
	char *end, *tmp;
	long x, strtol() ;

	p = fgets(line, BUFSIZ, f) ;
	if(p == NULL)
		return (NULL) ;

	sgroup.sg_name = p ;
	p = sgskip(p) ;

	sgroup.sg_pwd = p ;
	p = sgskip(p) ;

	x = strtol(p, &end, 10 );
	if ( (x<0) || (x>7))
		return (NULL);
	sgroup.sg_mode = x;

	return(&sgroup) ;
}

struct sgroup *
getsgnam(name)
char	*name ;
{
	register struct sgroup *p ;

	setsgent() ;
	while ( (p = getsgent()) && strcmp(name, p->sg_name) )
		;
	endsgent() ;
	return (p) ;
}

int
putsgent(p, f)
register struct sgroup *p ;
register FILE *f ;
{
char buf[40];

	(void) fprintf ( f, "%s:%s:%l\n", p->sg_name, p->sg_pwd , p->sg_mode) ;

	return(ferror(f)) ;
}
