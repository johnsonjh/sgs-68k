#ident	"@(#)protos.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/optim/mach/M68020/scan.c"
 *	(generated Jul 13 1990, 13:27)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  sline _PARMS((void));
extern short  sdisp _PARMS((void));
extern void  scan_020_mode _PARMS((long *, short *, short *));
extern void  fillrand _PARMS((struct rand *));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/optim/mach/M68020/mipsup.c"
 *	(generated Jul 13 1990, 13:27)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  ishlp _PARMS((struct node *));
extern char * newlab _PARMS((void));
extern char * getp _PARMS((struct node *));
extern void  setlab _PARMS((struct node *));
extern void  setbr _PARMS((struct node *, char *));
extern void  revbr _PARMS((struct node *));
extern void  setop _PARMS((struct node *, struct op *));
extern void  setrand _PARMS((struct node *, int , struct rand *));
extern void  prinst _PARMS((struct node *));
extern char * regchar _PARMS((long ));
extern long  uses _PARMS((struct node *));
extern long  sets _PARMS((struct node *));
extern int  instsize _PARMS((struct node *));
extern void  bshortsub _PARMS((struct node *));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/optim/mach/M68020/vtrace.c"
 *	(generated Jul 13 1990, 13:27)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  vtrace _PARMS((void));
extern void  nullstore _PARMS((struct node *, int ));
extern int  redund_mov _PARMS((struct node *, struct rand **, unsigned , int ));
extern int  peep _PARMS((struct node *, int ));
extern int  rmnode _PARMS((struct node *, int ));
extern long  getimmnum _PARMS((struct rand *));
extern void  prld _PARMS((void));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/optim/mach/M68020/optab.c"
 *	(generated Jul 13 1990, 13:27)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern struct op * findop _PARMS((char *, int ));
extern struct op * getOP _PARMS((int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/optim/mach/parse.c"
 *	(generated Jul 13 1990, 13:49)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  yyinit _PARMS((char *));
extern int  getline _PARMS((void));
extern void  yylex _PARMS((void));
extern void  dopass2opt _PARMS((int , int ));
extern int  doop _PARMS((char *, int ));
extern void  dolab _PARMS((char *, int ));
extern void  dorand _PARMS((struct rand *, int ));
extern int  rprint _PARMS((struct rand *, int ));
extern void  serr _PARMS((char *));
extern void  oerr _PARMS((char *));
extern int  doset _PARMS((void));
extern void  delsets _PARMS((struct node *));
extern void  outswitch _PARMS((struct node *));
extern int  switsize _PARMS((struct node *));
extern int  dstats _PARMS((void));
extern int  wrapup _PARMS((void));
extern int  digits _PARMS((char *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/optim/mach/rand.c"
 *	(generated Jul 13 1990, 13:28)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern unsigned long  rhash _PARMS((char *, int ));
extern void  clrhash _PARMS((void));
extern struct rand * saverand _PARMS((char *, int , int , long , int ));
extern struct rand * findrand _PARMS((char *, int ));
extern void  chgregrand _PARMS((char *, long , char *, long ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/optim/mach/motopt.c"
 *	(generated Jul 13 1990, 13:28)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  loopopt _PARMS((void));
extern int  foropt _PARMS((struct node *, int , struct node *, int ));
extern int  decodebcc _PARMS((int ));
extern int  cmpsize _PARMS((int , int *, int *));
extern int  setsreg _PARMS((long , struct node *));
extern int  asgto _PARMS((struct rand *, struct node *));
extern int  alters _PARMS((long , struct node *));
extern int  inline _PARMS((struct node *));
extern struct node * bldmov _PARMS((struct node *, int , struct rand *, struct rand *));
extern int  redmov _PARMS((void));
extern int  trysubmv _PARMS((struct node *, struct rand *, struct rand *, int , int ));
extern int  isused _PARMS((long , struct node *));
extern void  brarts _PARMS((void));
extern void  rmconlbl _PARMS((void));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/optim/mach/motopt2.c"
 *	(generated Jul 13 1990, 13:28)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  eaopt _PARMS((void));
extern int  collapse _PARMS((struct node *, struct rand *, long , int , int ));
extern int  usetemps _PARMS((void));
extern void  preprocfp _PARMS((void));
extern int  sccopt _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/optim/mach/rmframe.c"
 *	(generated Jul 13 1990, 13:28)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  newproc _PARMS((void));
extern void  writefunc _PARMS((int ));
extern void  compbias _PARMS((struct block *, int , int , int ));
extern void  issuefunc _PARMS((struct block *));
extern void  adjbias _PARMS((int , int ));
extern void  setfold _PARMS((void));
extern void  foldstore _PARMS((void));
extern void  stash _PARMS((char *));
extern void  fold _PARMS((struct proc *));
extern int  regsavesz _PARMS((void));
extern void  expanmovm _PARMS((long , int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/optim/mach/../common/optim.c"
 *	(generated Jul 13 1990, 13:29)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  main _PARMS((int , char **));
extern int  main2 _PARMS((int , char **));
extern void  init _PARMS((void));
extern int  optim _PARMS((void));
extern void  bldgr _PARMS((int ));
extern void  ldanal _PARMS((void));
extern struct block * lblhash _PARMS((char *, struct block *));
extern char * getspace _PARMS((unsigned ));
extern void  bspan _PARMS((int ));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/optim/mach/../common/inter.c"
 *	(generated Jul 13 1990, 13:29)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern struct node * Saveop _PARMS((int , char *, unsigned , int ));
extern void  addref _PARMS((char *, unsigned ));
extern void  filter _PARMS((void));
extern void  prtext _PARMS((void));
extern int  same _PARMS((struct node *, struct node *));
extern int  sameaddr _PARMS((struct node *, struct node *));
extern char * xalloc _PARMS((unsigned ));
extern void  xfree _PARMS((char *));
extern void  fatal _PARMS((char *, char *));
