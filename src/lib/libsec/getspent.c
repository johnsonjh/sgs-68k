/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) getspent.c: version 25.1 created on 12/2/91 at 20:14:12	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)getspent.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include <sys/param.h>
#include <stdio.h>
#include <shadow.h>
#include <string.h>
#include <sys/types.h>

#define ferror(p)       ((p)->_flag & _IOERR)

static FILE *spf = NULL ;
static char line[BUFSIZ+1] ;
static struct spwd spwd ;

void
setspent()
{
	if(spf == NULL) {
		spf = fopen(SHADOW, "r") ;
	}
	else
		rewind(spf) ;
}

void
endspent()
{
	if(spf != NULL) {
		(void) fclose(spf) ;
		spf = NULL ;
	}
}

static char *
spskip(p)
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

/* 	The getspent function will return a NULL for an end of 
	file indication or a bad entry
*/
struct spwd *
getspent()
{
	extern struct spwd *fgetspent() ;

	if(spf == NULL) {
		if((spf = fopen(SHADOW, "r")) == NULL)
			return (NULL) ;
	}
	return (fgetspent(spf)) ;
}

struct spwd *
fgetspent(f)
FILE *f ;
{
	register char *p ;
	char *end ;
	long	x, strtol() ;
	void *memchr() ;

	p = fgets(line, BUFSIZ, f) ;
	if(p == NULL)
		return (NULL) ;
	spwd.sp_namp = p ;
	p = spskip(p) ;
	spwd.sp_pwdp = p ;
	p = spskip(p) ;

	x = strtol(p, &end, 10) ;
	if (end != (char *) memchr(p, ':', strlen(p)))
		/* check for numeric value */
		return (NULL) ;
	if (end == p)	
		spwd.sp_lstchg = -1 ;
	else		
		spwd.sp_lstchg = x ;
	p = spskip(p) ;
	x = strtol(p, &end, 10) ;	
	if (end != (char *) memchr(p, ':', strlen(p)))
		/* check for numeric value */
		return (NULL) ;
	if (end == p)	
		spwd.sp_min = -1 ;
	else		
		spwd.sp_min = x ;
	p = spskip(p) ;
	x = strtol(p, &end, 10) ;	
	if (end != (char *) memchr(p, '\n', strlen(p)))
		/* check for numeric value */
		return (NULL) ;
	if (end == p)	
		spwd.sp_max = -1 ;
	else		
		spwd.sp_max = x ;

	return(&spwd) ;
}

struct spwd *
getspnam(name)
char	*name ;
{
	register struct spwd *p ;

	setspent() ;
	while ( (p = getspent()) && strcmp(name, p->sp_namp) )
		;
	endspent() ;
	return (p) ;
}

int
putspent(p, f)
register struct spwd *p ;
register FILE *f ;
{
	(void) fprintf ( f, "%s:%s:", p->sp_namp, p->sp_pwdp ) ;
	if ( p->sp_lstchg >= 0 )
	   (void) fprintf ( f, "%d:", p->sp_lstchg ) ;
	else
	   (void) fprintf ( f, ":" ) ;
	if ( p->sp_min >= 0 )
	   (void) fprintf ( f, "%d:", p->sp_min ) ;
	else
	   (void) fprintf ( f, ":" ) ;
	if ( p->sp_max >= 0 )
	   (void) fprintf ( f, "%d\n", p->sp_max ) ;
	else
	   (void) fprintf ( f, "\n" ) ;

	return(ferror(f)) ;
}

