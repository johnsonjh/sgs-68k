#ident	"@(#)defin.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)defin.h	7.1		*/
/*	Copyright 1985 Motorola Inc.			*/

/* Definition of most of the global variables. Those global variables
   not defined here are part of the common code shared between the
   other compiler phases.
*/


#define TVOID 020000
#define SYMTABSZ 10
#define CEFF (NRGS+1)
#define TRUE 1
#define FALSE 0

#include "macdefs.h"
#include "hash.h"
#include <manifest.h>
#include "bitset.h"
#include "symtab.h"
#include "protos.h"

int	aregstat;	/* counter for number of A registers assigned 
			** per function */
int	atendofblock=FALSE; /* flag for talloc to malloc when at the end of a
			    ** basic block and no nodes are available */
int	bbcnt;		/* number of basic blocks created */
int	bbflag;		/* basic block debug flag */
int	clseonly;	/* flag indicating only common local subexpressions 
			** can be removed for this function.  */
int	leaffunc;	/* leaf function flag (call no others) */
char	COPYRIGHT[] = "Copyright 1985 Motorola Inc.";
HASHTAB	constht;	/* hash table for constants */
BBPTR	currentbb = NULL;	/* pointer to the current basic block */
SCOPE*	curscope;	/* pointer to current SCOPE */
int	cursymlist;	/* index to the first symbol in the scope being built */
BBPTR*	dfn = NULL;	/* depth first number array */
int	dregstat;	/* counter for number of D registers assigned 
			** per function */
BBPTR	fgroot = NULL;	/* pointer to the root of the flow graph */
int	fpregstat;	/* counter for number of floating point registers
			** assigned per function */
char	funcname[257];	/* name of the current function */
HASHTAB	labelsht;	/* hash table for labels */
int	lcsectrl;	/* execution flag for turning off lcse */
int	lcseflag = TRUE;/* command line flag for controlling lcse */
int	lcsedbflg = 0;	/* if > 0 then lcses are written to stdout */
HASHTAB	lcseht;		/* hash table for local common subexpression */
int	conflag = 0;	/* inhibit constant promotion to register flag */
int	lcsestat;	/* statistics counter - number of LCSEs in function */
int	loopstat;	/* statistics counter - number of loops in function */
int	maxautooffset;	/* current maximum offset for automatic variables */
int	nodesin;	/* statistics counter - number of nodes read */
int	nodesout;	/* statistics counter - number of ctree nodes written */
int	odebug=0;	/* debug flag */
int	regctrl;	/* execution flag which controls register allocation */
int	regflag=TRUE;	/* command line flag: controls register allocation */
SCOPE*	scoperoot = NULL; /* pointer to top of SCOPE list */
int	scopestat;	/* counter for number of scopes encounter in 
			** a function */
SORTED*	sortedvars;	/* pointer to the array of sorted weights */
#ifndef NODBG
FILE*	statsfile;	/* parameter block for statistics file */
#endif
int	statsflag = 0;	/* If true (>0) then statistics will be written out */
int	symflag = 0;	/* debug flag for printing out symbol table */
SYMENT*	symtab = NULL;	/* pointer to the symbol table */
int	symtabndx;   	/* index to next free symbol table entry */
int	symtabsz;	/* current size of the symbol table */
int	tempstat;	/* statistics counter - # of temporaries */
int	valueno;	/* number of next value number */
#ifdef m68k
# ifdef DFLTDBLALIGN
int     align_val=32;   /* variable alignment value in bits, can be altered */
			/* by the DBL_ALIGN environement variable */
# else
int     align_val=16;	/* variable alignment value in bits, can be altered */
			/* by the DBL_ALIGN environement variable */
# endif
#endif
