#ident	"@(#)paths.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.1	89/06/08 00:37:27 root
	date and time created 89/06/08 00:37:27 by root

*/
/*	From the Motorola/ATT 5.1/2 source: paths.h	2.1		*/
/*
 * Pathnames for UNIX V/68
 */

	


#ifndef LIBDIR
#define LIBDIR	"/lib"
#endif

#ifndef LLIBDIR1
#define LLIBDIR1 "/usr/lib"
#endif

#define NDELDIRS 2

/*
 * Directory containing executable ("bin") files
 */
#define BINDIR	"/bin"

/*
 * Directory for "temp"  files
 */
#define TMPDIR	"/tmp"

/*
 * Name of default output object file
 */
#define A_OUT	"a.out"

#define	M4	"/usr/bin/m4"		/* macro processor program */

/*
** Names of various pieces called by the cc command.  The names are
** formed by prepending either BINDIR (B) or LIBDIR (L) and the cc
** command prefix (e.g., "mc68" in "mc68cc").  See cc command for
** details.
*/

#define NAMEcpp		"cpp"		/* C preprocessor:  L		*/
#define NAMEccom	"ccom"		/* C compiler:  L		*/
#define NAMEoptim	"optim"		/* optimizer:  L		*/
#define NAMEas		"as"		/* assembler:  B		*/
#define NAMEld		"ld"		/* loader:  B			*/
#define NAMEcrt0	"crt0.o"	/* C run time startoff:  L	*/
#define NAMEmcrt0	"mcrt0.o"	/* profiling startoff:  L	*/
