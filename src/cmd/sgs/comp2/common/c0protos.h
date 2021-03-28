#ident	"@(#)c0protos.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/trees.c"
 *	(generated Jul 13 1990, 13:06)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern unsigned  indtype _PARMS((unsigned ));
extern union ndu * xicolon _PARMS((union ndu *, union ndu *, union ndu *));
extern union ndu * buildtree _PARMS((int , union ndu *, union ndu *));
extern union ndu * strargs _PARMS((union ndu *, int *));
extern int  chkstr _PARMS((int , int , unsigned ));
extern union ndu * conval _PARMS((union ndu *));
extern int  chkpun _PARMS((union ndu *));
extern int  combo _PARMS((char *, union ndu *));
extern void  loseval_chk _PARMS((union ndu *));
extern int  icon_overflow _PARMS((union ndu *, int ));
extern union ndu * stref _PARMS((union ndu *));
extern int  notlval _PARMS((union ndu *));
extern int  not_intval _PARMS((union ndu *));
extern int  str_expr _PARMS((union ndu *));
extern union ndu * bcon _PARMS((int ));
extern union ndu * bpsize _PARMS((union ndu *));
extern long  psize _PARMS((union ndu *));
extern union ndu * unconvert _PARMS((union ndu *, union ndu *));
extern union ndu * convert _PARMS((union ndu *, int ));
extern int  econvert _PARMS((union ndu *));
extern union ndu * pconvert _PARMS((union ndu *, int ));
extern int  bigsize _PARMS((unsigned ));
extern int  cbigger _PARMS((union ndu *));
extern union ndu * oconvert _PARMS((union ndu *));
extern union ndu * ptmatch _PARMS((union ndu *));
extern union ndu * tymatch _PARMS((union ndu *));
extern union ndu * makety _PARMS((union ndu *, unsigned , int , int ));
extern union ndu * block _PARMS((int , union ndu *, union ndu *, unsigned , int , int , int ));
extern int  icons _PARMS((union ndu *));
extern int  opact _PARMS((union ndu *));
extern int  moditype _PARMS((unsigned ));
extern long  ccast _PARMS((long , unsigned ));
extern union ndu * doszof _PARMS((union ndu *));
extern union ndu * do_argty _PARMS((union ndu *));
extern int  eprint _PARMS((union ndu *));
extern int  e1print _PARMS((union ndu *, char *));
extern int  tprint _PARMS((FILE *, unsigned ));
extern int  locctr _PARMS((int ));
extern int  prtdcon _PARMS((union ndu *));
extern int  getlab _PARMS((void));
extern int  ecomp _PARMS((union ndu *));
extern int  ecode _PARMS((union ndu *));
extern int  prtree _PARMS((union ndu *));
extern int  p2tree _PARMS((union ndu *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/scan.c"
 *	(generated Jul 13 1990, 13:07)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  mainp1 _PARMS((int , char **));
extern int  eatline _PARMS((void));
extern int  spitline _PARMS((char *));
extern int  lxenter _PARMS((char *, int ));
extern int  lxmore _PARMS((int , int ));
extern int  lxinit _PARMS((void));
extern int  lxstr _PARMS((int ));
extern int  lxcom _PARMS((void));
extern int  yylex _PARMS((void));
extern int  lxres _PARMS((void));
extern int  lxtitle _PARMS((void));
extern int  asmout _PARMS((void));
extern char * savestr _PARMS((char *));
extern char * hash _PARMS((char *));
extern int  ppcontrol _PARMS((int ));
extern int  bg_file _PARMS((void));
extern int  mode_init _PARMS((int ));
extern int  do_pragma _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/cgram.c"
 *	(generated Jul 13 1990, 13:07)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern union ndu * mkty _PARMS((unsigned , int , int , int ));
extern union ndu * bdty _PARMS((int , union ndu *, int ));
extern int  dstash _PARMS((int ));
extern int  savebc _PARMS((void));
extern int  resetbc _PARMS((int ));
extern int  addcase _PARMS((union ndu *));
extern int  adddef _PARMS((void));
extern int  swstart _PARMS((void));
extern int  swend _PARMS((void));
extern int  yyparse _PARMS((void));


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
 *	"/mnt/barryk/wd/comp2/mach/../common/pftn.c"
 *	(generated Jul 13 1990, 13:08)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  defid _PARMS((union ndu *, int ));
extern int  asave _PARMS((int ));
extern int  psave _PARMS((int ));
extern int  ftnend _PARMS((void));
extern int  dclargs _PARMS((void));
extern int  regargs _PARMS((void));
extern union ndu * rstruct _PARMS((int , int ));
extern int  moedef _PARMS((int ));
extern int  bump_enum _PARMS((void));
extern int  bstruct _PARMS((int , int ));
extern union ndu * dclstruct _PARMS((int ));
extern int  yyerror _PARMS((char *));
extern int  yyaccpt _PARMS((void));
extern int  ftnarg _PARMS((int ));
extern int  salign _PARMS((unsigned , int ));
extern int  talign _PARMS((unsigned , int ));
extern long  tsize _PARMS((unsigned , int , int ));
extern int  inforce _PARMS((long ));
extern int  vfdalign _PARMS((int ));
extern int  beginit _PARMS((int ));
extern int  instk _PARMS((int , unsigned , unsigned , int , int , long ));
extern union ndu * getstr _PARMS((void));
extern int  ary_strbyte _PARMS((int ));
extern int  putbyte _PARMS((int ));
extern int  endinit _PARMS((void));
extern int  hiddengen _PARMS((int ));
extern int  doinit _PARMS((union ndu *));
extern int  gotscal _PARMS((void));
extern int  ilbrace _PARMS((void));
extern int  irbrace _PARMS((void));
extern int  upoff _PARMS((int , int , int *));
extern int  oalloc _PARMS((struct symtab *, int *, int ));
extern int  falloc _PARMS((struct symtab *, int , int , union ndu *));
extern int  argsize _PARMS((unsigned , int , int ));
extern int  nidcl _PARMS((union ndu *));
extern unsigned  types _PARMS((void));
extern int  type_quals _PARMS((unsigned , int , int ));
extern union ndu * tymerge _PARMS((union ndu *, union ndu *));
extern int  tyreduce _PARMS((union ndu *));
extern int  fixtype _PARMS((union ndu *, int ));
extern unsigned  ctype _PARMS((unsigned ));
extern int  uclass _PARMS((int ));
extern int  fixclass _PARMS((int , unsigned , int *));
extern struct symtab * mknonuniq _PARMS((int *));
extern int  lookup _PARMS((char *, int ));
extern struct symtab * relook _PARMS((struct symtab *));
extern int  clearst _PARMS((int ));
extern int  movestab _PARMS((struct symtab *, struct symtab *));
extern int  hide _PARMS((struct symtab *));
extern int  mkspace _PARMS((struct symtab *));
extern int  unhide _PARMS((struct symtab *));
extern int  inscope _PARMS((struct symtab *, int ));
extern int  type_category _PARMS((unsigned ));
extern int  qprint _PARMS((unsigned ));
extern int  is_const _PARMS((unsigned ));
extern int  is_ary _PARMS((unsigned ));
extern int  is_ftn _PARMS((unsigned ));
extern int  is_ptr _PARMS((unsigned ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/optim.c"
 *	(generated Jun 04 1990, 16:53)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern union ndu * aadjust _PARMS((union ndu *, int ));
extern int  adjust _PARMS((union ndu *, int ));
extern unsigned  simpstr _PARMS((int , int ));
extern int  tydown _PARMS((union ndu *));
extern union ndu * sconvert _PARMS((union ndu *));
extern union ndu * pvconvert _PARMS((union ndu *));
extern union ndu * fortarg _PARMS((union ndu *));
extern union ndu * doptim _PARMS((union ndu *));
extern union ndu * optim _PARMS((union ndu *));
extern int  ispow2 _PARMS((long ));
extern int  nncon _PARMS((union ndu *));
extern union ndu * offcon _PARMS((long , unsigned , int , int ));
extern void  unlong _PARMS((union ndu *));
extern int  bccode _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/common"
 *	(generated Jun 04 1990, 16:53)
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
 *	"/mnt/barryk/wd/comp2/mach/debug.c"
 *	(generated Jul 13 1990, 13:08)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  fixdef _PARMS((struct symtab *));
extern int  prdims _PARMS((struct symtab *));
extern int  prdef _PARMS((struct symtab *, int ));
extern int  strend _PARMS((int ));
extern struct symtab * strfind _PARMS((int ));
extern char * strname _PARMS((int ));
extern unsigned  chgtype _PARMS((unsigned ));
extern int  beg_file _PARMS((void));
extern int  fpe_catch _PARMS((void));
extern int  sdbline _PARMS((void));
extern int  aobeg _PARMS((void));
extern int  aocode _PARMS((struct symtab *));
extern int  aoend _PARMS((void));
extern int  bfdebug _PARMS((int *, int ));
extern int  efdebug _PARMS((void));
extern int  eobl2dbg _PARMS((void));
extern int  ejsdb _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/local.c"
 *	(generated Jun 04 1990, 16:53)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  fincode _PARMS((double , int ));
extern int  rallc _PARMS((unsigned ));
extern union ndu * clocal _PARMS((union ndu *));
extern int  myinit _PARMS((union ndu *, int ));
extern int  cisreg _PARMS((unsigned ));
extern int  incode _PARMS((union ndu *, int ));
extern int  vfdzero _PARMS((int ));
extern char * exname _PARMS((char *));
extern int  commdec _PARMS((int ));
extern int  branch _PARMS((int ));
extern int  deflab _PARMS((int ));
extern int  defalign _PARMS((int ));
extern int  eccode _PARMS((int ));
extern int  efcode _PARMS((void));
extern int  bfcode _PARMS((int *, int ));
extern int  defnam _PARMS((struct symtab *));
extern int  bycode _PARMS((int , int ));
extern int  zecode _PARMS((int ));
extern int  main _PARMS((int , char **));
extern int  genswitch _PARMS((struct sw *, int ));
extern int  e2print _PARMS((union ndu *));
extern int  e22print _PARMS((union ndu *, char *));
extern int  e222print _PARMS((int , union ndu *, char *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/inline1.c"
 *	(generated Jun 04 1990, 16:53)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  squirrelmacro _PARMS((char *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/proto.c"
 *	(generated Jul 13 1990, 13:09)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  ProtoPush _PARMS((void));
extern int  ProtoPop _PARMS((void));
extern void  ProtoType _PARMS((union ndu *, int ));
extern void  ProtoEnd _PARMS((int ));
extern void  ProtoLink _PARMS((union ndu *, int ));
extern union ndu * ProtoDeclare _PARMS((union ndu *));
extern int  ProtoDefine _PARMS((union ndu *));
extern int  ProtoChk _PARMS((union ndu *));
extern void  ProtoGen _PARMS((struct symtab *, int *, int , int ));
extern void  ProtoMix _PARMS((union ndu *));
extern void  ProtoMirage _PARMS((void));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/comp2/mach/../common/grey.c"
 *	(generated Jul 13 1990, 13:09)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern void  grey _PARMS((union ndu *));
