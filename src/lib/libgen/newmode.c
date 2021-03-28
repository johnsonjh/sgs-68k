/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) newmode.c: version 25.1 created on 12/2/91 at 19:33:15	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)newmode.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/




#include <stdio.h>
#include <sys/types.h>
#define	SYMLINK	1

#define	USER	05700	/* user's bits */
#define	GROUP	02070	/* group's bits */
#define	OTHER	00007	/* other's bits */
#define	ALL	07777	/* all */

#define	READ	00444	/* read permit */
#define	WRITE	00222	/* write permit */
#define	EXEC	00111	/* exec permit */
#define	SETID	06000	/* set[ug]id */
#define	LOCK	02000	/* lock permit */
#define	STICKY	01000	/* sticky bit */

/* For the time being, used here AND in the chmod utility */
static void _errormsg();

int
newmode(nm, file, ms)
unsigned int nm;
char *file;
char *ms;
{
	/* m contains USER|GROUP|OTHER information
	   o contains +|-|= information
	   b contains rwx(slt) information  */
	register int o, m, b;
	register int lcheck, scheck, xcheck, goon;
	int om = nm;	/* save mode for file mode incompatibility return */

	if (*ms >= '0' && *ms <= '9')
		return(abs(ms));
	do {
		for (m=0; ;ms++) {
			switch (*ms) {
			case 'u':
				m |= USER;
				continue;
			case 'g':
				m |= GROUP;
				continue;
			case 'o':
				m |= OTHER;
				continue;
			case 'a':
				m |= ALL;
				continue;
			default:
				if (m == 0)
					m = ALL;
				
			}
			break;
		}
	
		while (o = what(ms)) {
/*
	this section processes permissions
*/
			*ms++;
			b = 0;
			goon = 0;
			lcheck = scheck = xcheck = 0;
			switch (*ms) {
			case 'u':
				b = (nm & USER) >> 6;
				goto dup;
			case 'g':
				b = (nm & GROUP) >> 3;
				goto dup;
			case 'o':
				b = (nm & OTHER);
		    dup:
				b &= (READ|WRITE|EXEC);
				b |= (b << 3) | (b << 6);
				ms++;
				goon = 1;
			}
			while (goon == 0) switch (*ms++) {
			case 'r':
				b |= READ;
				continue;
			case 'w':
				b |= WRITE;
				continue;
			case 'x':
				b |= EXEC;
				xcheck = 1;
				continue;
			case 'l':
				b |= LOCK;
				m |= LOCK;
				lcheck = 1;
				continue;
			case 's':
				b |= SETID;
				scheck = 1;
				continue;
			case 't':
				b |= STICKY;
				continue;
			default:
				ms--;
				goon = 1;
			}

			b &= m;

			switch (o) {
			case '+':

				/* is group execution requested? */
				if ( xcheck == 1 && (b & GROUP & EXEC) == (GROUP & EXEC)) {

					/* not locking, too! */
					if ( lcheck == 1 ) {
						_errormsg(1, 3, "Group execution and locking not permitted together\n", (char *)0);
					}

					/* not if the file is already lockable */
					if ( (nm & GROUP & (LOCK | EXEC)) == LOCK ) {
						_errormsg(2, 0, "Group execution not permitted on %s, a lockable file\n", file);
						return(om);
					}
				}

				/* is setgid on execution requested? */
				if ( scheck == 1 && (b & GROUP & SETID) == (GROUP & SETID) ) {

					/* not locking, too! */
					if ( lcheck == 1 && (b & GROUP & EXEC) == (GROUP & EXEC) ) {
						_errormsg(1, 4, "Set-group-ID and locking not permitted together\n", (char *)0);
					}

					/* not if the file is already lockable */
					if ( (nm & GROUP & (LOCK | EXEC)) == LOCK ) {
						_errormsg(2, 0, "Set-group-ID not permitted on %s, a lockable file\n", file);
						return(om);
					}
				}

				/* is setid on execution requested? */
				if ( scheck == 1 ) {

					/* the corresponding execution must be requested or already set */
					if ( ((nm | b) & m & EXEC & (USER | GROUP)) != (m & EXEC & (USER | GROUP)) ) {
						_errormsg(2, 0, "Execute permission required for set-ID on execution for %s\n", file);
						return(om);
					}
				}

				/* is locking requested? */
				if ( lcheck == 1 ) {

					/* not if the file has group execution set */
					/* NOTE: this also covers files with setgid */
					if ( (nm & GROUP & EXEC) == (GROUP & EXEC) ) {
						_errormsg(2, 0, "Locking not permitted on %s, a group executable file\n", file);
						return(om);
					}
				}

				/* create new mode */
				nm |= b;
				break;
			case '-':

				/* don't turn off locking, unless it's on */
				if ( lcheck == 1 && scheck == 0 
				     && (nm & GROUP & (LOCK | EXEC)) != LOCK )
					b &= ~LOCK;

				/* don't turn off setgid, unless it's on */
				if ( scheck == 1 && lcheck == 0
				     && (nm & GROUP & (LOCK | EXEC)) == LOCK )
					b &= ~(GROUP & SETID);

				/* if execution is being turned off and the corresponding
				   setid is not, turn setid off, too & warn the user */
				if ( xcheck == 1 && scheck == 0
				     && ((m & GROUP) == GROUP || (m & USER) == USER)
				     && (nm & m & (SETID | EXEC)) == (m & (SETID | EXEC)) ) {
					_errormsg(2, 0, "Corresponding set-ID also disabled on %s since set-ID requires execute permission\n", file);
					if ( (b & USER & SETID) != (USER & SETID)
					     && (nm & USER & (SETID | EXEC)) == (m & USER & (SETID | EXEC)) )
						b |= USER & SETID;
					if ( (b & GROUP & SETID) != (GROUP & SETID)
					     && (nm & GROUP & (SETID | EXEC)) == (m & GROUP & (SETID | EXEC)) )
						b |= GROUP & SETID;
				}

				/* create new mode */
				nm &= ~b;
				break;
			case '=':

				/* is locking requested? */
				if ( lcheck == 1 ) {

					/* not group execution, too! */
					if ( (b & GROUP & EXEC) == (GROUP & EXEC) ) {
						_errormsg(1, 3, "Group execution and locking not permitted together\n", (char *)0);
					}

					/* if the file has group execution set, turn it off! */
					if ( (m & GROUP) != GROUP ) {
						nm &= ~(GROUP & EXEC);
					}
				}

				/* is setid on execution requested?
				   the corresponding execution must be requested, too! */
				if ( scheck == 1 && (b & EXEC & (USER | GROUP)) != (m & EXEC & (USER | GROUP)) ) {
					_errormsg(1, 2, "Execute permission required for set-ID on execution\n", (char *)0);
				}

				/* create new mode */
				nm &= ~m;
				nm |= b;
				break;
			}
		}
	} while (*ms++ == ',');
	if (*--ms) {
		_errormsg(1, 5, "Invalid mode\n", (char *)0);
	}
	return(nm);
}

static int
abs(ms)
char *ms;
{
	register c, i;

	for ( i = 0; (c = *ms) >= '0' && c <= '7'; ms++)
		i = (i << 3) + (c - '0');
	if (*ms) {
		_errormsg(1, 6, "invalid mode\n", (char *)0);
	}
	return(i);
}

static int
what(ms)
char *ms;
{
	switch (*ms) {
	case '+':
	case '-':
	case '=':
		return *ms;
	}
	return(0);
}

static void
_errormsg(severity, code, format, file)
int severity, code;
char *format;
char *file;
{
	static char *msg[] = {
	"",
	"ERROR",
	"WARNING",
	""
	};

	(void) fprintf(stderr, "%s: ", msg[severity]);
	(void) fprintf(stderr, format, file);
	if (code != 0)
		exit(code);
	return;
}
