#ident	"@(#)namespace.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/* namespace.c: routines for processing namespace files, storing
 * namespace information in namespace database, and accessing such information 
 * for use by symbol lookup functions */

#include <extrns.h>
#include <namespace.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

/* ANSI vs CUC stuff */
#if __STDC__ > 0
#include <stdlib.h>
#include <string.h>
#else
extern int strcmp();
extern char *bsearch();
extern char *strtok();
#endif
/***/

t_namespace Space[MAX_NSPACE];		/* array of namespaces */
int cur_ns;							/* current namespace */
int next_ns;						/* next available namespace slot */

#define NUM_ALLOC 5
#define NUM_REALLOC 5
#define TOKSEP " \t\n"

/* prototypes for static functions */
static void nmspace_addlib _PARMS((char *));
static void nmspace_addprefix _PARMS((char *));
static void nmspace_addname _PARMS((char *));
static void nmspace_syntax _PARMS((char *, int));


/* Search for given symbol name in table of equates.
 * If found, return a pointer to a static instance of the equate;
 * else return NULL
 */
char *
nmspace_equate( target, fname )
char *target;			/* search target */
char *fname;			/* current library/filename */
{
	char *ret = 0;
	char namebuf[BUFSIZ];
	int i;

			/* See if fname is listed in Spaces[].
			 * If so, call bsearch() to see if given symbol name
			 * has been equated.
			 */
	for ( i = 0; i < next_ns; i++ ) {

		if ( strcmp(fname, Space[i].lib) == 0 ) {

			ret = bsearch( target, Space[i].names[0], Space[i].nused, 
								sizeof(char *), strcmp );	

			if ( ret ) {	/* found */
							/* prepend prefix to symbol name */
				sprintf( namebuf, "%s%s", Space[i].pfx, ret );
				ret = savefn(namebuf);	/* get permanent storage */
			}

			break;			/* a given filename is listed only once */
		}
	}

	return ret;
}

/******************************************************************************
 * Namespace file processing.
 * Almost all errors are considered fatal (i.e. the namespace file 
 * must be clean).
 ******************************************************************************/
static char input_namefile[BUFSIZ];

void
nmspace_create(namefile)
char *namefile;
{
	/* We parse input file using a trivial state machine */
	enum {
		INIT,				/* initial state */
		GOT_LIBNAME,		/* have seen library name */ 
		GOT_PREFIX,			/* have seen prefix */ 
		GOT_NAME			/* have seen at least one symbol name */ 
	} state = INIT;
	FILE *nfp;
	char buf[BUFSIZ];
	int lineno;

	strcpy(input_namefile, namefile);		/* to facilitate error messages */

	if ( (nfp = fopen( namefile, "r" )) == NULL ) {
		lderror(2,0,NULL, "Can't open namespace file %s", namefile );
		/* NOTREACHED */
	}
		
	lineno = 0;
	while ( fgets(buf, BUFSIZ, nfp) != NULL ) {
		lineno++;

		switch( buf[0] ) {

		case '#':			/* comment line */
		case ' ':			/* data must start in column 1 */
		case '\t':
		case '\n':
		case '\f':
			continue;

		case '/':			/* UNIX pathname */	
			switch (state) {
			case INIT:
			case GOT_NAME:
				nmspace_addlib(strtok(buf,TOKSEP));
				state = GOT_LIBNAME;
				break;
			default:
				nmspace_syntax(namefile,lineno);
				break;
			}
			break;

		default:
			if ( !isalpha(buf[0]) && buf[0] != '_' )
				nmspace_syntax(namefile,lineno);
			switch ( state ) {
				case GOT_LIBNAME:	/* get prefix */
					nmspace_addprefix(strtok(buf,TOKSEP));
					state = GOT_PREFIX;
					break;
				case GOT_PREFIX:	/* collect entry in name list  */
				case GOT_NAME:	/* collect entry in name list  */
					nmspace_addname(strtok(buf,TOKSEP));
					state = GOT_NAME;
					break;
			}
			break;
		}
	}

	fclose(nfp);
}

static void
nmspace_addlib(lname)
char *lname;		/* tokenized library name */
{
	struct stat sbuf;
	int i;
	int reuse = 0;
	t_namespace *ns;


			/* First, check if namespace already exists for given library. */
			/* If so, new overrides old. */
	for( i = 0; i<next_ns; i++ ) {
		if ( strcmp( lname, Space[i].lib ) == 0 ) {
			lderror(1,0, NULL, "Namespace %s overrides previous", lname ); 
			Space[i].nused = 0;			/* reclaim existing space */
			cur_ns = i;					/* and reuse table slot */
			reuse = 1;
		}
	}

	if ( !reuse ) {

		if ( stat(lname, &sbuf) < 0 || S_ISREG(sbuf.st_mode) == 0 )
			lderror( 2,0, NULL, "Unable to stat %s", lname );

		cur_ns = next_ns++;				/* time to allocate new namespace */ 
	}

	ns = &Space[cur_ns];

	ns->names = (char **) myalloc(NUM_ALLOC * sizeof(char *));
	ns->nalloc = NUM_ALLOC;
	ns->nused = 0;

	strcpy(Space[cur_ns].lib, lname);
}

static void
nmspace_addprefix(pfx)
char *pfx;
{
	sprintf(Space[cur_ns].pfx, "%-.*s", PREFIX_MAX, pfx);

	if ( strlen(pfx) > PREFIX_MAX ) {
		lderror(1,0, input_namefile,
			"Namespace prefix %s truncated to %d characters", 
			pfx, PREFIX_MAX );
	}
}


/* Note on memory allocation:
 * As a first implementation, we simply allocate an array of NUM_ALLOC
 * character pointers, and realloc() the array if we run out of room.
 * The names themselves are stored using the linker's general-purpose
 * savefn() heap allocator for strings; the entries in the names[] array
 * point to the addresses of the names stored by savefn().
 */
static void
nmspace_addname(newname)
char *newname;
{
	t_namespace *ns;

	ns = &Space[cur_ns];

						/* check if we need more space for names */
	if ( ns->nused > ns->nalloc ) {
		ns->nalloc += NUM_REALLOC * sizeof(char *);
		ns->names = (char **) myrealloc(ns->names, ns->nalloc );
	}

						/* check if in proper collating sequence */
	if ( ns->nused > 0 ) {
		if ( strcmp( newname, ns->names[ ns->nused -1 ] ) <= 0 )
			lderror( 2,0, input_namefile, "Unsorted name entries" );
	}
	
						/* store name, and save pointer in ns->names[] */
	ns->names[ ns->nused++ ]  = savefn( newname );
}
 
static void
nmspace_syntax(namefile,lineno)
char *namefile;
int lineno;
{
	lderror(2,lineno,namefile, "namespace file syntax error" );
}
