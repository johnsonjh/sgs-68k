#ident	"@(#)protos.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/pass0.c"
 *	(generated Jul 13 1990, 13:29)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  getargs _PARMS((int , char **));
extern int  main _PARMS((int , char **));
extern int  callm4 _PARMS((void));
extern int  callsys _PARMS((char *, char **, char *));
extern long  myatol _PARMS((char *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/parse.c"
 *	(generated Jul 13 1990, 13:29)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  yylex _PARMS((void));
extern int  hexdigit _PARMS((int ));
extern int  binstr _PARMS((char *));
extern struct exp * combine _PARMS((int , struct exp *, struct exp *));
extern int  fill _PARMS((long ));
extern int  ckalign _PARMS((int ));
extern int  mk_indirect _PARMS((int , instr *, struct exp *));
extern int  mkdisp_indirect _PARMS((int , instr *, struct exp *));
extern int  mkindex_indirect _PARMS((int , instr *, instr *, struct exp *));
extern int  mk_full_ind _PARMS((int , instr *, instr *, struct exp *));
extern int  mk_post_ind _PARMS((instr *, struct exp *));
extern int  power _PARMS((int , int ));
extern int  get_fp_prec _PARMS((int ));
extern int  get_fp_round _PARMS((int ));
extern int  get_value _PARMS((symbol *));
extern int  yyparse _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/M68020/gencode.c"
 *	(generated Jul 13 1990, 13:30)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  eff_add _PARMS((struct arg *, int , int ));
extern int  gen0op _PARMS((instr *));
extern int  gen1op _PARMS((instr *, int , struct arg *));
extern int  gen2op _PARMS((instr *, int , struct arg *, struct arg *));
extern int  gen3op _PARMS((instr *, int , struct arg *, struct arg *, struct arg *));
extern int  gen4op _PARMS((instr *, int , struct arg *, struct arg *, struct arg *, struct arg *));
extern int  gen6op _PARMS((instr *, int , struct arg *, struct arg *, struct arg *, struct arg *, struct arg *, struct arg *));
extern int  flags _PARMS((int ));
extern int  gen_eff_add _PARMS((void));
extern int  gen_inst_word _PARMS((void));
extern int  get_fp_value _PARMS((int ));
extern int  function_code _PARMS((struct arg *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/M68020/addr2.c"
 *	(generated Jul 13 1990, 13:30)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  genreloc _PARMS((symbol *, codebuf *));
extern int  brloc _PARMS((symbol *, codebuf *));
extern int  braopt _PARMS((symbol *, codebuf *));
extern int  bccopt _PARMS((symbol *, codebuf *));
extern int  bsropt _PARMS((symbol *, codebuf *));
extern int  absopt _PARMS((symbol *, codebuf *));
extern int  swbeg _PARMS((symbol *, codebuf *));
extern int  move _PARMS((symbol *, codebuf *));
extern int  iopt _PARMS((symbol *, codebuf *));
extern int  insti _PARMS((symbol *, codebuf *));
extern int  absbr _PARMS((symbol *, codebuf *));
extern int  iabsbr _PARMS((symbol *, codebuf *));
extern int  ndxreloc _PARMS((symbol *, codebuf *));
extern int  cpbccopt _PARMS((symbol *, codebuf *));
extern int  brbyt _PARMS((symbol *, codebuf *));
extern int  picky _PARMS((codebuf *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/pass1.c"
 *	(generated Jul 13 1990, 13:31)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  aspass1 _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/symbols.c"
 *	(generated Jul 13 1990, 13:31)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern symbol * newent _PARMS((char *));
extern upsymins * lookup _PARMS((char *, int , int ));
extern int  traverse _PARMS((int (*)()));
extern int  creasyms _PARMS((void));
extern int  addstr _PARMS((char *));
extern int  strtabinit _PARMS((void));
extern int  symkluge _PARMS((int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/code.c"
 *	(generated Jul 13 1990, 13:31)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  generate _PARMS((int , int , long , symbol *));
extern int  flushbuf _PARMS((void));
extern int  cgsect _PARMS((int, int ));
extern short  mksect _PARMS((symbol *, int ));
extern void  comment _PARMS((char *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/expand1.c"
 *	(generated Jul 13 1990, 13:31)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  update _PARMS((ssentry *, int ));
extern int  sdiclass _PARMS((ssentry *));
extern int  expand _PARMS((void));
extern int  punt _PARMS((void));
extern int  newlab _PARMS((symbol *));
extern int  deflab _PARMS((symbol *));
extern int  fixsyms _PARMS((void));
extern int  sdi _PARMS((symbol *, long , int ));
extern int  newsdi _PARMS((symbol *, long , int ));
extern int  shortsdi _PARMS((symbol *, long , int ));
extern int  resolve _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/errors.c"
 *	(generated Jul 13 1990, 13:31)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  onintr _PARMS((void));
extern int  aerror _PARMS((char *));
extern int  yyerror _PARMS((char *));
extern int  werror _PARMS((char *));
extern int  errmsg _PARMS((char *, char *));
extern int  delexit _PARMS((void));
extern int  deltemps _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/pass2.c"
 *	(generated Jul 13 1990, 13:31)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  aspass2 _PARMS((void));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/addr1.c"
 *	(generated Jul 13 1990, 13:31)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  setmagic _PARMS((symbol *, codebuf *));
extern int  setfile _PARMS((upsymins , codebuf *));
extern int  newstmt _PARMS((symbol *, codebuf *));
extern int  lineno _PARMS((symbol *, codebuf *));
extern int  linenum _PARMS((symbol *, codebuf *));
extern int  lineval _PARMS((symbol *, codebuf *));
extern int  define _PARMS((symbol *, codebuf *));
extern int  setval _PARMS((symbol *, codebuf *));
extern int  settyp _PARMS((symbol *, codebuf *));
extern int  settypq _PARMS((symbol *, codebuf *));
extern int  setscl _PARMS((symbol *, codebuf *));
extern int  settag _PARMS((symbol *, codebuf *));
extern int  setlno _PARMS((symbol *, codebuf *));
extern int  setsiz _PARMS((symbol *, codebuf *));
extern int  setdim1 _PARMS((symbol *, codebuf *));
extern int  setdim2 _PARMS((symbol *, codebuf *));
extern int  dfaxent _PARMS((long , int , int ));
extern int  xform _PARMS((void));
extern int  endef _PARMS((symbol *, codebuf *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/codeout.c"
 *	(generated Jul 13 1990, 13:31)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  codgen _PARMS((int , long ));
extern int  codout _PARMS((char *, long , int ));
extern int  getcode _PARMS((codebuf *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/getstab.c"
 *	(generated Jul 13 1990, 13:32)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  fix _PARMS((symbol *));
extern int  fixstab _PARMS((long , long , int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/obj.c"
 *	(generated Jul 13 1990, 13:32)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  headers _PARMS((void));
extern int  copysect _PARMS((char *));
extern int  reloc _PARMS((long ));
extern int  invent _PARMS((symbol *, long , int , int ));
extern int  outsyms _PARMS((symbol *));
extern int  symout _PARMS((void));
extern struct symentag * pop _PARMS((void));
extern int  setsym _PARMS((int ));
extern int  fixst _PARMS((FILE *));

/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/symlist.c"
 *	(generated Jul 13 1990, 13:32)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern char * alloc _PARMS((int ));
extern int  putindx _PARMS((symbol *, int , long ));
extern long  getindx _PARMS((char *, int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/../../as/common/unused.c"
 *	(generated Jul 13 1990, 13:32)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  unused _PARMS((char *, char *));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/dofpnum.c"
 *	(generated Jul 13 1990, 13:32)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  do_fp_num _PARMS((unsigned *));
extern int  decpack _PARMS((char *, char *, int , int , int , unsigned *));
extern int  decbin _PARMS((char *, char *, int , int , int , unsigned *));
extern int  fppack _PARMS((struct fpbin , int *));
extern struct fpbin  strbin _PARMS((char *, int , int ));
extern int  validout _PARMS((struct fpbin *, int ));
extern int  decinfinity _PARMS((short *));
extern int  decmaxnbr _PARMS((short *));
extern int  maxnbr _PARMS((struct fpbin *, int ));
extern int  infinity _PARMS((struct fpbin *, int ));
extern struct fpbin  fpmult _PARMS((struct fpbin , struct fpbin ));
extern struct fpbin  fpdiv _PARMS((struct fpbin , struct fpbin ));
extern int  fastleft _PARMS((int , short *, short *));
extern struct fpbin  getpwr _PARMS((int ));
extern struct fpbin  pwrten _PARMS((int , int ));
extern int  round _PARMS((struct fpbin *, int , int ));
extern int  mul10 _PARMS((short *, int , int ));
extern int  quicknorm _PARMS((struct fpbin *));
extern int  lshift _PARMS((short *, int ));
extern int  rshift _PARMS((short *, int ));
extern int  binadd _PARMS((short *, short *, short *, int ));
extern int  binadjst _PARMS((short *, int ));
extern int  negate _PARMS((short *, int ));
extern int  bzero _PARMS((short *, int ));
extern int  stzero _PARMS((char *));
extern int  stripzr _PARMS((char *));
extern int  leadzr _PARMS((char *));
extern int  bincr _PARMS((short *, unsigned ));
extern int  bindec _PARMS((short *, int ));
extern int  binassgn _PARMS((short *, short *, int ));
extern int  bifzero _PARMS((short *, int ));
extern int  bincmpr _PARMS((short *, short *, int ));
extern int  binscmpr _PARMS((short *, short *, int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/spitcode.c"
 *	(generated Jul 13 1990, 13:32)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern int  spitline _PARMS((long , long ));
extern int  spitrmd _PARMS((long ));
extern int  spitcode _PARMS((ulong , int , ulong , int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/radfix.c"
 *	(generated Jul 13 1990, 13:32)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern long  radfix _PARMS((char *, int ));


/*	Function Prototypes for 
 *	"/mnt/barryk/wd/as/mach/stamptime.c"
 *	(generated Jul 13 1990, 13:32)
 */

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

extern long  stamptime _PARMS((char *));
