#ident	"@(#)protos.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../common/addvalue.c"
 *	(generated Jul 13 1990, 13:24)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  addvalueno _PARMS((union ndu *, int ));
extern void  freelcse _PARMS((struct Tlcse *));
extern void  delaltlist _PARMS((void));
extern void  lcse _PARMS((struct Tbasicblock *));
extern int  constcmp _PARMS((union flt_dbl *, union flt_dbl *));
extern unsigned  consthash _PARMS((struct hashtab *, union flt_dbl *));
extern unsigned  lcsehash _PARMS((struct hashtab *, struct Tlcsekey *));
extern int  lcsecmp _PARMS((struct Tlcsekey *, struct Tlcsekey *));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../common/adjframe.c"
 *	(generated Jul 13 1990, 13:24)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  adjframe _PARMS((void));
extern int  find_maxoffset _PARMS((struct Tlevel *, int ));
extern int  prmoves _PARMS((struct Tlevel *, int ));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../common/bb.c"
 *	(generated Jul 13 1990, 13:24)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  recognizebb _PARMS((union ndu *));
extern struct Tbasicblock * createbb _PARMS((void));
extern void  addnodetobb _PARMS((union ndu *));
extern int  lvalue _PARMS((union ndu *));
extern void  parentlink _PARMS((union ndu *, union ndu *, int ));
extern void  linearize _PARMS((union ndu *));
extern void  initfunc _PARMS((void));
extern void  freebb _PARMS((struct Tbasicblock *));
extern void  freebblink _PARMS((struct Tbblink *));
extern void  linkbbs _PARMS((struct Tbasicblock *, struct Tbasicblock *));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../common/bitset.c"
 *	(generated Jul 13 1990, 13:24)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern SET * CREATESET _PARMS((int ));
extern int  bitrangeerr _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../common/gather.c"
 *	(generated Jul 13 1990, 13:24)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern char  gatherfunc _PARMS((void));
extern union ndu * talloc _PARMS((void));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../common/hash.c"
 *	(generated Jul 13 1990, 13:25)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  hcreat _PARMS((struct hashtab *, int ));
extern struct entry * hsearch _PARMS((struct hashtab *, int (*)(), int (*)(), struct entry , ACTION ));
extern unsigned  hashd _PARMS((struct hashtab *, char *));
extern void  cleanupht _PARMS((struct hashtab *, void (*)()));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../common/hloptim.c"
 *	(generated Jul 13 1990, 13:25)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  optimize _PARMS((void));
extern void  dfncompute _PARMS((void));
extern void  dfns _PARMS((struct Tbasicblock *));
extern void  detectloops _PARMS((void));
extern SET * findnaturalloop _PARMS((struct Tbasicblock *, struct Tbasicblock *));
extern void  insertloop _PARMS((struct Tbasicblock *, SET *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/localopt.c"
 *	(generated Jul 13 1990, 13:25)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  conput _PARMS((union ndu *));
extern int  insput _PARMS((union ndu *));
extern int  upput _PARMS((union ndu *));
extern int  adrput _PARMS((union ndu *));
extern int  acon _PARMS((union ndu *));
extern int  tprint _PARMS((unsigned ));
extern int  ispow2 _PARMS((long ));
extern int  deflab _PARMS((int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../common/main.c"
 *	(generated Jul 13 1990, 13:25)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  main _PARMS((int , char **));
extern int  p2init _PARMS((int , char **));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../common/misc.c"
 *	(generated Jul 13 1990, 13:25)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  argsize _PARMS((union ndu *));
extern int  tinit _PARMS((void));
extern int  tfree _PARMS((union ndu *));
extern int  mkdope _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../../comp2/common/reader.c"
 *	(generated Jul 13 1990, 13:25)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern union ndu * eread _PARMS((void));
extern long  rdin _PARMS((int ));
extern int  prgoal _PARMS((int ));
extern int  e2print _PARMS((union ndu *));
extern int  e22print _PARMS((union ndu *, char *));
extern int  e222print _PARMS((int , union ndu *, char *));
extern int  t2print _PARMS((unsigned ));
extern int  cprt _PARMS((int , char *));
extern int  is_volatile _PARMS((unsigned ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../common/regasg.c"
 *	(generated Jul 13 1990, 13:26)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  asgregvars _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../common/symtab.c"
 *	(generated Jul 13 1990, 13:26)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  scopedump _PARMS((void));
extern void  indent _PARMS((int ));
extern void  traversescope _PARMS((struct Tlevel *, int ));
extern void  dumpsymtab _PARMS((int , int ));
extern unsigned  chgtype _PARMS((unsigned ));
extern int  acceptable _PARMS((unsigned ));
extern int  getsyment _PARMS((void));
extern int  getvalueno _PARMS((void));
extern void  addsymbol _PARMS((int , int ));
extern void  rdsymtab _PARMS((char *));
extern void  addscope _PARMS((struct Tlevel *, struct Tlevel *));
extern void  enterscope _PARMS((union ndu *));
extern void  exitscope _PARMS((union ndu *));
extern int  gettemp _PARMS((int ));
extern void  classify _PARMS((union ndu *));
extern int  lookup _PARMS((union ndu *));
extern struct Tsorted * createsv _PARMS((void));
extern int  cmpweights _PARMS((struct Tsorted *, struct Tsorted *));
extern void  flushstvn _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/storage.c"
 *	(generated Jul 13 1990, 13:26)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  addtemp _PARMS((struct Tlevel *, int ));
extern int  typesize _PARMS((int ));
extern int  getreg _PARMS((struct Tlevel *, unsigned , int *, struct syment *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/coptim/mach/../common/write.c"
 *	(generated Jul 13 1990, 13:26)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  writefunc _PARMS((void));
extern void  writebb _PARMS((struct Tbasicblock *));
extern void  prtree _PARMS((union ndu *));
extern void  writestats _PARMS((void));
extern void  prflowgraph _PARMS((void));
