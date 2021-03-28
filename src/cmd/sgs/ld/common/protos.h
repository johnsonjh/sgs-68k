#ident	"@(#)protos.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/alloc.c"
 *	(generated Jul 13 1990, 13:28)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  alloc _PARMS((void));
extern int  bldmemlist _PARMS((void));
extern int  alc_bonds _PARMS((void));
extern int  alc_owners _PARMS((void));
extern int  alc_attowns _PARMS((void));
extern int  alc_the_rest _PARMS((void));
extern int  audit_groups _PARMS((void));
extern int  audit_regions _PARMS((void));
extern struct anode * findsanode _PARMS((struct outsect *));
extern int  alc_lists _PARMS((struct outsect **, struct anode **, int ));
extern int  can_alloc _PARMS((struct anode *, struct anode *, struct outsect *, struct anode **, struct anode **, unsigned long ));
extern struct anode * splitnode _PARMS((struct anode *, unsigned long ));
extern int  do_alloc _PARMS((struct anode *, struct anode *, struct outsect *));
extern struct anode * findnode _PARMS((unsigned long , int ));
extern int  cmp_anl _PARMS((struct anode **, struct anode **));
extern int  cmp_uos _PARMS((struct outsect **, struct outsect **));
extern long  alignment _PARMS((unsigned long , unsigned long ));
extern int  creatsym _PARMS((char *, long ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/expr0.c"
 *	(generated Jul 13 1990, 13:29)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern union exprnode * buildtree _PARMS((int , union exprnode *, union exprnode *));
extern union exprnode * symnode _PARMS((char *));
extern union exprnode * sectnode _PARMS((int , char *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/expr1.c"
 *	(generated Jul 13 1990, 13:29)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern unsigned long  eval _PARMS((union exprnode *));
extern int  pexlist _PARMS((void));
extern int  pexitem _PARMS((void));
extern int  clrexp _PARMS((union act_item *));
extern int  clrlimb _PARMS((union exprnode *));
extern unsigned long  findnext _PARMS((unsigned long ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/ld00.c"
 *	(generated Jul 13 1990, 13:30)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  filespec _PARMS((char *));
extern int  bldadscn _PARMS((char *, char *, union act_item *));
extern union act_item * bldadfil _PARMS((char *, union act_item *));
extern int  bldexp _PARMS((union exprnode *, struct listown *));
extern int  pflags _PARMS((char *, int ));
extern char * library _PARMS((char *));
extern char * savefn _PARMS((char *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/ld01.c"
 *	(generated Jul 13 1990, 13:30)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  pass2 _PARMS((void));
extern int  initpss2 _PARMS((void));
extern int  ldexit _PARMS((void));
extern int  undefsm _PARMS((char *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/ld1.c"
 *	(generated Jul 13 1990, 13:30)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  pmemreg _PARMS((void));
extern int  ploadfil _PARMS((char *, FILE *, int ));
extern int  loadobjf _PARMS((struct ldfile *, char *, int , struct syment *));
extern char * strtabread _PARMS((FILE *, struct infile *, int *));
extern int  commspec _PARMS((void));
extern int  pf77comm _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/ld2.c"
 *	(generated Jul 13 1990, 13:30)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  pboslist _PARMS((void));
extern struct outsect * dfn_scn_grp _PARMS((union act_item *, int ));
extern int  inscadd _PARMS((struct insect *, struct outsect *));
extern struct infile * fndinfil _PARMS((char *));
extern int  chgdot _PARMS((union exprnode *));
extern int  add_pad _PARMS((void));
extern int  updatsms _PARMS((void));
extern struct outsect * findoscn _PARMS((int ));
extern int  split_scns _PARMS((void));
extern int  psize _PARMS((void));
extern struct outsect * fndoutsec _PARMS((char *));
extern long  check_sect _PARMS((struct outsect *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/lists.c"
 *	(generated Jul 13 1990, 13:31)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  listadd _PARMS((int , void *, void *));
extern int  listins _PARMS((int , struct listown *, void *, void *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/output.c"
 *	(generated Jul 13 1990, 13:31)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  print_undefines _PARMS((void));
extern int  ldoutput _PARMS((void));
extern int  wrtinfl _PARMS((FILE *, FILE *, FILE *, FILE *, char *));
extern long  plocsyms _PARMS((FILE *, FILE *, FILE *, struct infile *));
extern int  wrtaout _PARMS((FILE *, struct filehdr *));
extern int  wrtplst _PARMS((FILE *));
extern int  setfile _PARMS((FILE *, long ));
extern int  strwrite _PARMS((FILE *, struct syment *, int ));
extern void  init_arrays _PARMS((struct insect *, int *, long *, long *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/syms.c"
 *	(generated Jul 13 1990, 13:31)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  hash _PARMS((char *));
extern struct gsym * findsym _PARMS((char *));
extern struct gaux * findaux _PARMS((struct gsym *, int ));
extern struct gsym * makesym _PARMS((struct syment *, struct infile *));
extern struct gaux * makeaux _PARMS((struct gsym *, union auxent *, int ));
extern struct gsym * getsym _PARMS((long ));
extern struct gaux * getaux _PARMS((long ));
extern long  psymtab _PARMS((FILE *, long , struct infile *, struct syment *, char *));
extern struct gaux * loopaux _PARMS((struct gsym *, struct gaux *, int ));
extern struct insect * sectnum _PARMS((int , struct infile *));
extern long  add_dot _PARMS((long , struct insect *));
extern int  sym_scope _PARMS((char *, enum scope ));
extern int  hideornots _PARMS((char *));
extern int  hideornot _PARMS((struct gsym *));
extern int  hide_sym _PARMS((struct syment *, enum scope ));
extern int  take_out_escapes _PARMS((char *));
extern char * my_realloc _PARMS((char *, int ));
extern int  sym_equate _PARMS((struct syment *, char *));
extern int  make_equate _PARMS((char *, char *));
extern int  delete_equate _PARMS((char *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/slotvec.c"
 *	(generated Jul 13 1990, 13:31)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  svinit _PARMS((long ));
extern int  svcreate _PARMS((long , long , long , long , int , int ));
extern struct slotvec * svread _PARMS((long ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/util.c"
 *	(generated Jul 13 1990, 13:32)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern union act_item * dfnscngrp _PARMS((int , union exprnode *, union exprnode *, unsigned long *));
extern int  chkpower2 _PARMS((unsigned long ));
extern void * zero _PARMS((void *, int ));
extern int  zerofile _PARMS((FILE *, unsigned *, int ));
extern char * sname _PARMS((char *));
extern char * myalloc _PARMS((int ));
extern char * mycalloc _PARMS((int ));
extern int  stoi _PARMS((char *));
extern union exprnode * cnstnode _PARMS((long ));
extern long  copy_section _PARMS((FILE *, struct infile *, struct insect *, struct outsect *, FILE *, char *, long ));
extern int  do_dotlib _PARMS((long *, long *, char *, unsigned long ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/gmatch.c"
 *	(generated Jul 13 1990, 13:32)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  store_re _PARMS((char *, enum scope ));
extern int  action_re _PARMS((char *, enum scope ));
extern int  hasmeta _PARMS((char *));
extern int  gmatch _PARMS((unsigned char *, unsigned char *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/lex.yy.c"
 *	(generated Jul 13 1990, 14:06)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  yylex _PARMS((void));
extern int  main _PARMS((int , char **));
extern long * savelng _PARMS((long ));
extern char * savestr _PARMS((char *));
extern int  moveit _PARMS((int ));
extern int  initpass1 _PARMS((void));
extern int  yylook _PARMS((void));
extern int  yyback _PARMS((int *, int ));
extern int  yyinput _PARMS((void));
extern int  yyoutput _PARMS((int ));
extern int  yyunput _PARMS((int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/maps.c"
 *	(generated Jul 13 1990, 13:33)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  ldmap _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/../../ld/common/ld.yac"
 *	(generated Jul 13 1990, 14:07)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  yyparse _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/special1.c"
 *	(generated Jul 13 1990, 13:33)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  specflags _PARMS((char *, char **));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/special2.c"
 *	(generated Jul 13 1990, 13:34)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  adjneed _PARMS((unsigned long *, struct outsect *, struct anode *));
extern void  undefine _PARMS((void));
extern void  dfltsec _PARMS((void));
extern void  procspecobj _PARMS((struct ldfile *, char *));
extern void  adjsize _PARMS((struct outsect *));
extern void  adjaout _PARMS((struct aouthdr *));
extern int  relocate _PARMS((FILE *, struct infile *, struct insect *, FILE *, FILE *, char *, long ));
extern long  swapb4 _PARMS((long , int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/specsyms.c"
 *	(generated Jul 13 1990, 13:34)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  set_spec_syms _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/ld/mach/stamptime.c"
 *	(generated Jul 13 1990, 13:34)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern long  stamptime _PARMS((char *));

extern char *nmspace_equate _PARMS((char *, char *));
