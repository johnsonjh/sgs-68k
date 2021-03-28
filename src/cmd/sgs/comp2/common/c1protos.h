#ident	"@(#)c1protos.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/reader.c"
 *	(generated Jul 13 1990, 13:09)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern union ndu * force _PARMS((union ndu *));
extern int  p2init _PARMS((int , char **));
extern union ndu * dlabel _PARMS((union ndu *, int ));
extern union ndu * genbr _PARMS((int , int , union ndu * ));
extern int  tcond _PARMS((union ndu *));
extern int  fcond _PARMS((union ndu *));
extern union ndu * rcomma _PARMS((union ndu *));
extern union ndu * seq _PARMS((union ndu *, union ndu *));
extern union ndu * gtb _PARMS((union ndu *, int ));
extern union ndu * condit _PARMS((union ndu *, int , int , int ));
extern int  mainp2 _PARMS((int , char **));
extern union ndu * eread _PARMS((void));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/match.c"
 *	(generated Jun 04 1990, 16:54)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  ttype _PARMS((unsigned ));
extern struct optab * match _PARMS((union ndu *, struct optab *));
extern int  ustrip _PARMS((int ));
extern int  rtspine _PARMS((struct shape **, union ndu *));
extern int  restrip _PARMS((struct shape **));
extern int  noresult _PARMS((struct shape *));
extern int  smspine _PARMS((struct shape *, union ndu *, int ));
extern int  mspine _PARMS((struct shape **, union ndu *, int , struct shape **));
extern int  tempok _PARMS((union ndu *));
extern int  rmside _PARMS((union ndu *));
extern int  expand _PARMS((union ndu *, int , char *, struct optab *));
extern union ndu * getlr _PARMS((union ndu *, int ));
extern union ndu * getadr _PARMS((union ndu *, char **));
extern int  spshape _PARMS((int , union ndu *));
extern int  user_need _PARMS((union ndu *, struct optab *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/allo.c"
 *	(generated Jun 04 1990, 16:54)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  allo0 _PARMS((void));
extern int  allo _PARMS((union ndu *, struct optab *));
extern int  markbusy _PARMS((int , unsigned ));
extern int  freetemp _PARMS((int ));
extern int  freepair _PARMS((union ndu *, int ));
extern int  freereg _PARMS((union ndu *, int ));
extern int  shared _PARMS((union ndu *));
extern int  usable _PARMS((union ndu *, int , int ));
extern int  shareit _PARMS((union ndu *, int , int ));
extern int  sameregtype _PARMS((unsigned , unsigned ));
extern int  ushare _PARMS((union ndu *, int , unsigned ));
extern int  regrcl _PARMS((union ndu *));
extern int  rfree _PARMS((int , unsigned ));
extern int  rbusy _PARMS((int , unsigned ));
extern int  rwprint _PARMS((int ));
extern int  reclaim _PARMS((union ndu *, int , int ));
extern union ndu * tcopy _PARMS((union ndu *));
extern int  allchk _PARMS((void));
extern int  argsize _PARMS((union ndu *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/common"
 *	(generated Jun 04 1990, 16:55)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  where _PARMS((int ));
extern int  tinit _PARMS((void));
extern union ndu * talloc _PARMS((void));
extern int  tcheck _PARMS((void));
extern int  tfree _PARMS((union ndu *));
extern int  mkdope _PARMS((void));
extern char * tstr _PARMS((char *));
extern union ndu * cptree _PARMS((union ndu *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/cgen.c"
 *	(generated Jul 13 1990, 13:10)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  rewcom _PARMS((union ndu *, int ));
extern int  rewlhs _PARMS((union ndu *));
extern int  rewsto _PARMS((union ndu *));
extern int  iseff _PARMS((union ndu *));
extern int  volatile_chk _PARMS((union ndu *));
extern void  protect _PARMS((int ));
extern union ndu * lhsto _PARMS((union ndu *));
extern union ndu * ind2type _PARMS((union ndu *, union ndu *));
extern union ndu * reweop _PARMS((union ndu *));
extern int  rewass _PARMS((union ndu *));
extern int  outshp _PARMS((struct shape **));
extern int  tabpr _PARMS((void));
extern int  codgen _PARMS((union ndu *));
extern int  insprt _PARMS((void));
extern int  cfix _PARMS((union ndu *, int ));
extern int  preff _PARMS((int ));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/cost.c"
 *	(generated Jun 04 1990, 16:55)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  commute _PARMS((union ndu *));
extern int  bcost _PARMS((union ndu *, struct optab *));
extern int  lhsok _PARMS((union ndu *));
extern int  shpr _PARMS((struct shape *));
extern int  pstrat _PARMS((int ));
extern int  insout _PARMS((union ndu *, int ));
extern int  bprt _PARMS((union ndu *, struct optab *, int ));
extern int  findsub _PARMS((union ndu *, struct shape *));
extern int  setregtype _PARMS((unsigned ));
extern int  rscost _PARMS((int *, union ndu *));
extern int  subregs _PARMS((int *, unsigned ));
extern int  sumreg _PARMS((int , int *));
extern int  costs _PARMS((union ndu *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/xdefs.c"
 *	(generated Jun 04 1990, 16:52)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern char * scnames _PARMS((int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/M68020/local2.c"
 *	(generated Jun 04 1990, 16:55)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  lineid _PARMS((int , char *));
extern int  eobl2 _PARMS((void));
extern int  special _PARMS((int , union ndu *));
extern int  popargs _PARMS((int ));
extern int  next_address _PARMS((union ndu *, int ));
extern int  zzzcode _PARMS((union ndu *, char **, struct optab *));
extern int  star _PARMS((union ndu *, int , struct optab *));
extern int  stas _PARMS((union ndu *, int , int ));
extern int  conput _PARMS((union ndu *));
extern int  intr_881instr_put _PARMS((union ndu *));
extern int  insput _PARMS((union ndu *));
extern int  upput _PARMS((union ndu *));
extern int  adrput _PARMS((union ndu *));
extern int  acon _PARMS((union ndu *));
extern int  genscc _PARMS((int ));
extern int  genfpscc _PARMS((int ));
extern int  cbgen _PARMS((int , int , int ));
extern int  fp_cbgen _PARMS((int , int , int ));
extern int  dbccgen _PARMS((int , int , int ));
extern int  tempreg _PARMS((int *, int , unsigned ));
extern int  main _PARMS((int , char **));
extern int  getlab _PARMS((void));
extern int  tprint _PARMS((unsigned ));
extern int  ispow2 _PARMS((long ));
extern int  deflab _PARMS((int ));
extern int  doprolog _PARMS((union ndu *));
extern int  doepilog _PARMS((union ndu *));
extern int  doswitch _PARMS((union ndu *));
extern int  indiradr _PARMS((union ndu *));
extern int  bldea _PARMS((union ndu *, EADESC *, int ));
extern union ndu * blddisp _PARMS((EADESC *, union ndu *, int ));
extern int  recrd_node _PARMS((union ndu *, EADESC *, int ));
extern int  oprndadr _PARMS((EADESC *));
extern int  putinx _PARMS((EADESC *));
extern int  putregnm _PARMS((int ));
extern int  putdisp _PARMS((union ndu *, int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/inline2.c"
 *	(generated Jul 13 1990, 13:11)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  genicall _PARMS((union ndu *, int ));



/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/iconmul.c"
 *	(generated Jul 13 1990, 13:11)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  iconmult _PARMS((ulong ));
extern int  iconmulg _PARMS((ulong , int , union ndu *, union ndu *));
