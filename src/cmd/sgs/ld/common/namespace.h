#ident	"@(#)namespace.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/* Access to External Identifier Namespaces */

 
/*******************************************************************************
Library functions often depend on other library symbols in
ways that are not made evident to the user.  A well-defined access method
publicizes a complete list of the external symbols contained in the library, 
and, with those exceptions, promises the user that all other identifiers are 
available.

The ANSI C Standard lays down clear guidance in this area (section 4.1.2.1
of the Standard, and with greater clarity in the corresponding section of 
the Rationale).  The Standard defines the C Language namespace (which
includes the library) as the set whose elements are: 

	1. the C Language keywords 
	2. the C Library's external identifiers
	3. the macros and typedef's included in the 15 ANSI headers
	4. all file-scope identifiers that begin with an underscore
	5. all remaining identifiers (i.e. macros) that begin with an
	   underscore followed by an upper-case letter or another underscore

In order to satisfy the requirements of ANSI C, as well as to provid a
generic method for resolving similar namespace issues, the Arix linker 
provides a namespace option (-n).  The implementation reflects certain
judgements about the context in which namespace issues arise.  Consider
a situation where linking with a given libary has introduced symbols
which collide with names already defined in the user's program.

	Application	|	C Language Implementation
	------------|--------------------------------
	fread()	   <->	fread()	-> read()
	read()	   <->

Assume the application makes a call to the C Library's fread, but
elsewhere the same program contains a definition of a function read.
Because of fread's dependency on read, two problems may result:

	1) the linker will ignore the library version of read.  Thus,
	   fread will actually call the user's read.
	2) if fread and read are in the same library module (this isn't true
	   for this example, but applies to many other cases), the linker
	   will issue a multiple definition error message and the link will fail.

Experienced Unix programmer's learn to expect exactly these kinds of
problems.  With increasingly complicated libraries, however, the
programmer may need to control this pollution.  Using the linker namespace
feature, the programmer creates of list of names in a given library which 
collide with the application, and specifies an (arbitrary) prefix 
(_C_ in this example) which is prepended to those names to keep them out
of the application namespace:

	Application	|	C Language Implementation
	------------|--------------------------------
	fread()	   <->	fread()	-> _C_read()
	read()		|

*******************************************************************************/

/*
 * For now, when the programmer invokes the compiler in strictly conforming
 * ANSI mode (-M2), the cc(1) driver automatically invokes the linker with
 * the proper option to partition the external identifiers associated with
 * ARIX's expanded C Library into the implementation's namespace, i.e.
 * all external names present in the ARIX C and math libraries which are not
 * sanctioned by ANSI are prepended with an underbar-leading prefix.
 */

#include <limits.h>
#define PREFIX_MAX 10
#define MAX_NSPACE	10			/* maximum number of namespaces */

/* namespace control structure */
typedef struct s_namespace {
	char lib[PATH_MAX];		/* full pathname of library */
	char pfx[PREFIX_MAX];	/* unique prefix */
	int nalloc;				/* number of currently allocated name entries */
	int nused;				/* number of currently used name entries */
	char **names;			/* array of pointers to names */
} t_namespace;
