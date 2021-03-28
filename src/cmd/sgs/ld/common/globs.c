#ident	"@(#)globs.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

	

#include "system.h"

#include <stdio.h>
#include "structs.h"

#if TRVEC
#include "tv.h"
#include "ldtv.h"
#endif

#include "paths.h"
#include "params.h"

/*
 * This include file is the complement of "extrns.h" and
 * should be changed whenever extrns.h is changed.
 */



int lineno;		/* current line number in ifile being parsed */
char *curfilnm;		/* ptr to current input file name	     */

long	Zflag = 0;	/* time stamp for fileheader */
char	aflag = 1,	/* file is to be "absolute" (default)	*/
	cflag,		/* build a shared library file		*/
	dflag,		/* turn on debug output			*/
	iflag,		/* ** b16ld/x86ld *** separate I and D	*/
	mflag,		/* generate a memory map		*/
	rflag,		/* mark the object file as relocatable	*/
	sflag,		/* strip relocation/line/symbol info	*/
	tvflag,		/* * b16ld/x86ld/n3bld * tv linkage	*/
	xflag,		/* do not preserve local symbols	*/
#if COMMON
	tflag,		/* do type checking, if on		*/
			/* bk: always warn against multiple definitions, */
			/* unless user requests lowest warning level */
	Mflag = 0,	/* msg for multiply defnd external defs */
#endif
	showflag,		/* bk: show names of linked library objects */
	/* sw2 */
	zflag=1,	/* do not use memory around zero, if on */
#if ILDOPT
	ildflag,	/* do not output .i_l_dxx sections	*/
#endif
	Hflag,		/* "Hide" global symbols by converting them
			   into variables of type "static"	*/
	Nflag,		/* squash text and data together, turn off -n */
	Sflag,		/* "silent" running mode: issue messages
			   only for fatal errors		*/
	Vflag;		/* echo version information about ld    */
int	hflag,		/* size reserved for optional header	*/
	pflag,		/* size reserved for patch list		*/
	/* sw2 */
	Bflag,		/* generate "padding" output sections	*/
	Pflag = 0 ;	/* run time paging - replaces kflag	*/

int 	Qflag = 1;		/* default to S90 a.out section directives */
	
unsigned short VSflag;	/* put version stamp in optional hdr */

short usrdeftv;		/* .tv input or SECTION			*/
short globfill;		/* default fill value */
short errlev;		/* error level generated during the ld run */

short bond_t_d;		/* set by dfltsec to indicate to pboslist that text
				and data must be properly bonded for paging */

#if FLEXNAMES		/* this implies PORTAR */
long stroffset = 4L;	/* offset of string in string table (4 is for length) */
long tnumsyms;			/* total number of symbols in output file */
#endif
#if PORTAR
long ar_size;			/* size of archive .o */
#endif


/*
 * Lists built during PASS 1, from user input
 */

struct listown symlist;		/* head of the symbol list	*/
struct listown memlist;		/* head of the MEMLIST		*/
struct listown reglist;		/* head of the REGLIST		*/
struct listown bldoutsc;	/* head of the BLDOUTSEC List	*/
struct listown explist;		/* head of the expression List	*/
struct listown commlist;	/* head of the COMMON list	*/


#if TRVEC
TVASSIGN *tvslot1;		/* ptr to head of tv slot list */
#endif


/*
 * Lists built during PASS 2, from information obtained in PASS 1
 */

struct listown bondlist;	/* head of the BONDLIST		*/
struct listown ownlist;		/* head of the OWNLIST		*/
struct listown atownlst;	/* head of the ATTOWN List	*/
struct listown infilist;	/* head of the INFILE List	*/
struct listown outsclst;	/* head of the OUTSEC List	*/
struct listown avlist;		/* head of available space list */
struct listown dsectlst;	/* head of DSECT list		*/

char outfilnm[128] = A_OUT;	/* name of output object file */

char *strbase,			/* global save area for input ifile  */
     *strnext,			/* names, input *.o and archive file */
     *strlimit;			/* names			     */
int strleng;

#if FLEXNAMES
char *epsymbol;
#else
char epsymbol[8] = "";		/* a.out header entry point symbol */
#endif

int numoutsc;			/* number of output sections */
long cur_dot = -1L;		/* current value of "dot" symbol
				 *	-1L indicates no value.
				 */

ACTITEM *curexp;		/* current expression being evaluated */

long maxlcsms;			/* max no. of local symbols for an input file */

long symtborg;			/* file address of symbol table in output file*/

#if PERF_DATA
long	allocspc;		/* amount of space allocated by myalloc */
#endif

#if DMERT
int	dmertplib;		/* DMERT public library			*/
#endif

#if IAPX
ADDRESS memorg;			/* memorg will be assigned when magic no. is known*/
ADDRESS textorg;		/* ditto */
#else
ADDRESS memorg = MEMORG;	/* beginning of configured memory */
#if M32
#ifdef ILLEGAL_0
ADDRESS textorg = SEGVIOORG;	/* beginning of segment in which text
						starts */
#else
ADDRESS textorg = 0;		/* beginning of segment in which text
						starts */
#endif
#else
ADDRESS textorg = MEMORG;
#endif
#endif

char	stat_name[9]	=	{ '\0','\0','\0','\0','\0','\0','\0','\0','\0'};
long	max_section;		/* largest input section */

long	noutsyms,
	maxrelocsms,
	numldsyms,
	numldaux;
long	unresolved;
