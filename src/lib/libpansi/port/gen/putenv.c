#ident	"@(#)putenv.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/putenv.c	1.4"	*/
	

/*	LINTLIBRARY	*/
/*	putenv - change environment variables

	input - char *change = a pointer to a string of the form
			       "name=value"

	output - 0, if successful
		 1, otherwise
*/
#define NULL 0
extern char **_environ;		/* pointer to enviroment */
static reall = 0;		/* flag to reallocate space, if putenv is called
				   more than once */

int
putenv(change)
char *change;
{
	char **newenv;		    /* points to new environment */
	register int which;	    /* index of variable to replace */
	char *realloc(), *malloc(); /* memory alloc routines */

	if ((which = find(change)) < 0)  {
		/* if a new variable */
		/* which is negative of table size, so invert and
		   count new element */
		which = (-which) + 1;
		if (reall)  {
			/* we have expanded _environ before */
			newenv = (char **)realloc(_environ,
				  which*sizeof(char *));
			if (newenv == NULL)  return -1;
			/* now that we have space, change _environ */
			_environ = newenv;
		} else {
			/* _environ points to the original space */
			reall++;
			newenv = (char **)malloc(which*sizeof(char *));
			if (newenv == NULL)  return -1;
			(void)memcpy((char *)newenv, (char *)_environ,
 				(int)(which*sizeof(char *)));
			_environ = newenv;
		}
		_environ[which-2] = change;
		_environ[which-1] = NULL;
	}  else  {
		/* we are replacing an old variable */
		_environ[which] = change;
	}
	return 0;
}

/*	find - find where s2 is in _environ
 *
 *	input - str = string of form name=value
 *
 *	output - index of name in _environ that matches "name"
 *		 -size of table, if none exists
*/
static
find(str)
register char *str;
{
	register int ct = 0;	/* index into _environ */

	while(_environ[ct] != NULL)   {
		if (match(_environ[ct], str)  != 0)
			return ct;
		ct++;
	}
	return -(++ct);
}
/*
 *	s1 is either name, or name=value
 *	s2 is name=value
 *	if names match, return value of 1,
 *	else return 0
 */

static
match(s1, s2)
register char *s1, *s2;
{
	while(*s1 == *s2++)  {
		if (*s1 == '=')
			return 1;
		s1++;
	}
	return 0;
}
