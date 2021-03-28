#ident	"@(#)declar.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)declar.h	7.1		*/
/*	Copyright 1985 Motorola Inc.			*/


#define SYMTABSZ 10
#define TVOID 020000
#define CEFF (NRGS+1)
#define TRUE 1
#define FALSE 0

#include "macdefs.h"
#include "hash.h"
#include "manifest.h"
#include "symtab.h"
#include "bitset.h"
#include <string.h>
#if __STDC__ == 1
#include <stdlib.h>
#endif
#include "protos.h"

/* EXTERNAL VARIABLES (global and nonstatic) - see defin.h for definitions */

extern	int	atendofblock;
extern	int	aregstat;	
extern	int	bbcnt;		
extern	int	bbflag;		
extern	int	clseonly;	
extern	int leaffunc;				/* leaf function flag */
extern	HASHTAB	constht;	
extern	BBPTR	currentbb;	
extern	SCOPE*	curscope;	
extern	int	cursymlist;	
extern	BBPTR*	dfn;	
extern	int	dregstat;	
extern	BBPTR	fgroot;	
extern	int	fpregstat;
extern	char	ftitle[];
extern	char	funcname[257];	
extern	HASHTAB	labelsht;	
extern	NODE*	lastfree;
extern	NODE*	highnode;
extern	int	conflag;				/* no constant promotion flag */
extern	int	lcsectrl;
extern	int	lcsedbflg;
extern	int	lcseflag;	
extern	HASHTAB	lcseht;		
extern	int	lcsestat;	
extern	int	lineno;
extern	int	loopstat;	
extern	int	maxautooffset;
extern	NODE	node[];
extern	int	nodesin;	
extern	int	nodesout;	
extern	int	odebug;
extern	int	regctrl;
extern	int	regflag;
extern	SCOPE*	scoperoot;
extern	int	scopestat;	
extern	SORTED*	sortedvars;	
# ifndef NODBG
extern	FILE*	statsfile;
# endif
extern	int	statsflag;
extern	int	symflag;
extern	SYMENT*	symtab;	
extern	int	symtabndx;   	
extern	int	symtabsz;	
extern	int	tempstat;	
extern	int	valueno;	
extern  int	absmaxoffset;
# ifdef m68k
extern  int     align_val;
# endif

/* EXTERNAL FUNCTIONS (global and nonstatic) */

#if __STDC__ <= 0
extern char *calloc();
extern char *malloc();
extern char *realloc();
extern char *strcpy();
extern char *strtok();
#endif

extern	SET*	CREATESET();
extern	void	addnodetobb();
extern	int	addtemp();
extern	void	asgregvars();
extern	void	cerror();
extern	void	classify();
extern	void	cleanupht();
extern	void	clse();
extern	NODE*	condit();
extern	int	constcmp();
extern	BBPTR	createbb();
extern	SORTED*	createsv();
extern	void	detectloops();
extern	void	dfncompute();
extern	void	dfns();
# ifndef NODBG
extern	void	dumpsymtab();
# endif
extern	void	enterblock();
extern	void	enterscope();
extern	NODE*	eread();
extern	void	exitblock();
extern	void	exitscope();
extern	SET*	findnaturalloop();
extern	void	free();
extern	void	freebb();
extern	void	freebblink();
extern	void	freelcse();
extern	char	gatherfunc();
extern	int	getreg();
extern	int	gettemp();
extern	int	hcreate();
extern	void	hdestroy();
extern	ENTRY*	hsearch();
extern	void	initfunc();
extern	void	insertloop();
extern	void	lcse();
extern	int	lcsecmp();
extern	void	linearize();
extern	void	linkbbs();
extern	void	optimize();
extern	void	parentlink();
#ifndef NODBG
extern	void	prflowgraph();
#endif
extern	void	prtree();
extern	void	rdsymtab();
extern	void	recognizebb();
# ifndef NODBG
extern	HNODE*	removekey();
extern	void	scopedump();
# endif
extern	NODE*	talloc();
# ifndef NODBG
extern	void	traversescope();
# endif
extern	int	typesize();
extern	void	writebb();
extern	void	writefunc();
# ifndef NODBG
extern	void	writestats();
# endif
extern	unsigned int	consthash();
extern	unsigned int	hashd();
extern	unsigned int	hashd();
extern	unsigned int	lcsehash();
