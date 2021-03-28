#ident	"@(#)proto.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/* function prototypes */
#include "mfile1.h"
#include "proto.h"
#include <sys/types.h>
#include <time.h>

#define PLIST_SZ 1000				/* max # of prototyped functions */
#define NUM_PTYPES (PLIST_SZ * 2)	/* pre-allocated prototype entries */
#define PSTCK_SZ 20					/* for nested prototypes */
#define PROTO_NOID	-1				/* prototype with no (comment) id */
#define PCALL 2
#define OK 1
#define BAD 0
#define MIN(x,y) ( (x) < (y) ? (x) : (y) )
#define AGGREGATE(x) 	( BTYPE(x)==STRTY || 			\
							BTYPE(x)==UNIONTY || BTYPE(x)==ENUMTY )
#define NEST_CALL 10					/* max # nested function calls */
			/* special conversion note: for now, we convert float->double */
			/* even if prototype specifies float.  We can experiment with */
			/* the (dangerous) possibility of passing prototyped floats */
			/* unpromoted by un-defining ARGCONV. */
#define ARGCONV(ty) 	(( ty == FLOAT ) ? DOUBLE : ty )

#ifdef NODBG
#define DBG(x)
#else
#define DBG(x) if ( pdebug ) x
#endif

typedef struct s_protoctrl {		/* prototype list control variables used */
									/* while constructing a prototype list: */
	t_prototype *chead;				/* head of current prototype list */
	t_prototype *ctail;				/* tail of current prototype list */	
	int cindx;						/* index in proto_lists */
	int cflag;						/* special flags */
} t_protoctrl;

t_protolist proto_lists[PLIST_SZ];	/* headers of prototype lists */
static t_prototype proto_nodes[NUM_PTYPES];	/* pool of prototype entries */

static t_protoctrl pctrl;					/* defines current prototype list */
static t_protoctrl proto_stack[PSTCK_SZ];	/* for nested ptype declarations */
static int pstk_indx;						/* stack pointer for proto_stack */
int pdebug;
int proto_argno;					/* current arg under type inspection */
int ftn_arg;						/* for float->double farg promotion */
static int nestcall[NEST_CALL];		/* remember stab idx of ftn call id */
static int nestcall_idx;


/* Prototypes for static functions */
static struct s_prototype * ProtoAlloc _PARMS((void));
static struct s_prototype * ProtoChain _PARMS((void));
static int  ProtoHeadInit _PARMS((void));
static void  ProtoDump _PARMS((struct s_protolist *, int ));
static int  ProtoArgCnt _PARMS((union ndu *));
static int  ProtoCmp _PARMS((int , int , int ));
static int  ProtoDimChk _PARMS((struct s_prototype *, struct s_prototype *));
static void  ProtoPrint _PARMS((struct symtab *, FILE *, int ));
static char * ProtoStructID _PARMS((struct symtab *));
static union ndu * ProtoId _PARMS((union ndu *));
static char * lscnames _PARMS((int ));

extern struct symtab *scopestack[]; 
int scope_proto_index;
int promoted_floats = 0; /* roger: if true, means some protoype parameter has been prmoted from float to double */

				/*********** MAINTENANCE ROUTINES ***************/
				/* ProtoAlloc:	Get free prototype entry        */
				/* ProtoChain:	Insert new prototype definition */
				/************************************************/

/* allocate a prototype node */
static t_prototype *
ProtoAlloc()
{
	static t_prototype *nextnode = proto_nodes;	/* next free node on list */

	if (nextnode >= &proto_nodes[NUM_PTYPES])
		cerror( "Too many prototypes" );	/* should just start malloc'ing */
	
	return( nextnode++ );
}

/* Allocate and insert a prototype entry into current function prototype list */
static t_prototype *
ProtoChain()
{
	t_prototype *ptype;

	ptype = ProtoAlloc();				/* get another one */

	if (pctrl.ctail == PNULL) {		/* starting a new list? */
		pctrl.chead = pctrl.ctail =
			ptype->pprev = ptype; 	/* remember head of list */
	} else {
		pctrl.ctail->pnext = ptype;
		ptype->pprev = pctrl.ctail;
		pctrl.ctail = ptype;		/* keep track of current location in list */
	}
	return(ptype);
}


			/***************** NESTED PROTOTYPES **********************/
			/* Note that grammar calls ProtoPush when it recognizes   */
			/* ftn-type argument, but before it knows if that ftn arg */
			/* has an (optional) parameter-type list.  This creates   */
			/* wasted calls to ProtoPush, but allows us to always     */
			/* call ProtoPop without worry.							  */
			/**********************************************************/

/* Save control variables of current prototype list */
void 
ProtoPush()
{
	if (pstk_indx >= PSTCK_SZ)				/* boundary condition */
		cerror( "Unreasonably complicated function declaration" );

	proto_stack[pstk_indx++] = pctrl;

	pctrl.chead = pctrl.ctail = PNULL;		/* zap */
	pctrl.cflag = 0;
	pctrl.cindx = 0;
}

/* Restore control variables of previous prototype list */
int
ProtoPop()
{
	int oldplist;

	if (pstk_indx < 1)					/* boundary condition */
		cerror( "No prototypes on stack" );

	oldplist = pctrl.cindx;

	pctrl = proto_stack[--pstk_indx];

	return( oldplist );
}


				/***************************************************/
				/* Allocate and initialize a list header structure */
				/***************************************************/

static int
ProtoHeadInit()
{
	t_protolist *plist;
	t_prototype *ptype;
	static int next_plist = 1;		/* index to proto_lists: 0 used to */
									/* distinguish non-prototyped functions */
	
	if ( next_plist >= PLIST_SZ )
		cerror( "Too many function prototypes" );

	plist = &proto_lists[next_plist];	/* next available list */
	pctrl.cindx = next_plist;

	/* transfer info from temporary ctrl structure to permanent list header */
	plist->lhead = pctrl.chead;			/* head of list */
	plist->lflag = pctrl.cflag;

	pctrl.cflag = 0;					/* reset: don't reset cindx! */
	pctrl.chead = PNULL;

										/* count parms */
	for ( plist->lcnt = 0, ptype = plist->lhead; ptype; ptype = ptype->pnext )
		++plist->lcnt;

	return( next_plist++ );
}


				/******* STEPS IN DEFINING A PROTOTYPE LIST *******/
				/* ProtoType:	Define individual parameter type  */
				/* ProtoEnd:	Conclude a parm-type-list         */
				/* ProtoLink:	Link parm-type-list to function id*/
				/* ProtoDeclare:Remove dummy arg id's from symtab */
				/* ProtoDefine: Validate prototype for ftn defn   */
				/**************************************************/


/* Record type of prototyped parameter */
/* Special case: the parameter node could be for a typedef'd function
 * type; if so, the node's "proto" field will be non-zero (thanks
 * to tymerge).
 */
void
ProtoType(pn,optid)
NODE *pn;			/* parameter node in parse tree */
int optid;			/* equals -1 (PROTO_NOID) if no ftn arg identifier */
{
	t_prototype *ptype;
	int oldplist;			/* index to head of nested prototype list */

	if (pn == (NODE *) 0)
		cerror( "Null node in ProtoType" );

	if (is_ftn(DECREF(pn->in.type))) { /* "int foo(int (*x)(args))" */
		switch ( pn->in.proto ) {
			case -1:		/* yylex flag for typedef'd ftn w/out prototype */
				oldplist = 0;
				break;
			case 0:			/* normal case: just got done parsing nested plist*/
				oldplist = ProtoPop();	/* we need head of outer parm list */
				break;
			default:		/* typedef'd function w/ prototype */
				if ( pn->in.proto > 0 )			/* paranoia safety check */
					oldplist = pn->in.proto;
				else 
					cerror( "ProtoType, invalid node" );
				break;
		}
	} else {
		oldplist = 0;
	}

										/*  catch: */
										/* "int foo( void x, int y )" */
										/* "int foo( int y, void x )" */
	if ( pctrl.cflag & PVOID ) {
		uerror( "contradictory use of void type" );
	} else {
		if ( pn->fn.type == VOID ) {
			if ( pctrl.chead != PNULL ) {
				uerror( "contradictory use of void type" );
			} else {
				pctrl.cflag |= PVOID;	/* no sense creating a new entry */
			}
		} else {						/* standard prototype */
			ptype = ProtoChain();		/* get a new entry & link it into list*/

			fixtype(pn, PARAM);
										/* save prototype info for this arg */
			ptype->ptype = pn->fn.type;
			ptype->pdim = pn->fn.cdim;
			ptype->psiz = pn->fn.csiz;
			ptype->psym = optid;
			ptype->pqual = typequal;	/* needed by dclargs, not prototyping */
			if ( curclass == REGISTER )	/* needed by dclargs, not prototyping */
				ptype->pclass = REGISTER;
			else
				ptype->pclass = PARAM;
			typequal = 0;
			ptype->pnest = oldplist;
		}
	}
}

/* Round out a parameter type list */
void
ProtoEnd(varargs)
int varargs;						/* boolean: do we have "..." ? */
{

					/* methinks this check is superfluous */
	if (pctrl.ctail) {				/* non-empty parameter type list? */

		if (varargs) 				/* variable # of arguments */
			pctrl.cflag |= PVARARGS;/* indicate "types explicitly unknown" */

		pctrl.ctail = PNULL;		/* next prototype will start a new list */
	}
}


/* Link parameter type list to its function identifier */
/* Note that optname<=0 if no id */
void
ProtoLink(idnode,ptrflag)
NODE *idnode;		/* head of function identifier's declaration subtree */
int ptrflag;		/* for function returning pointer to function */
{
	struct symtab *sptr;
	t_protolist *plist;
	NODE *wrk;
	int optname;
	unsigned short lindx;

	wrk = ProtoId(idnode);
	optname = wrk->tn.rval;		/* get NAME node's rval */

	if ( optname == PROTO_NOID )		/* sorry, confused with cast parse */
		return;

	if ( !ptrflag ) {
		if (( pctrl.chead == PNULL ) &&		/* empty parameter list */
			( (pctrl.cflag & PVOID) == 0 ))	/*ain't no prototype for this guy*/
			cerror( "ProtoLink: no list" ); 
	}

	sptr = &stab[optname];				/* ready to link with ftn id */

	if ( ptrflag && HASPROTO(sptr->sproto) )	/* int (*foo(int))(int) */
		lindx = sptr->sproto;
	else 
		lindx = ProtoHeadInit();		/* get new list header */
	plist = &proto_lists[ lindx ];

	if ( !ptrflag ) {					/* update function symbol's type info */
		if ( HASPROTO(sptr->sproto) ) {		/* already prototyped */
			sptr->sproto = ProtoCmp( sptr->sproto, lindx,0); /*"merge"*/
			plist = &proto_lists[sptr->sproto];
			/* plist->lhead = PNULL;			/ free extra list */
		} else {
			sptr->sproto = lindx;		/* link ftn symbol with proto list */
		}
		plist->lsymidx = optname;		/* link proto list to ftn symbol */

	} else {						/* also handles: void (*signal())(void) */	
									/* -> empty list, just points to plist */
									/* of pointed-to function */
		int save;		/* 1/9/90 */

		sptr->sproto = lindx;
		plist->lsymidx = optname;
		ProtoPop();					/* retrieve plist of returned ftn */
								/* define nameless plist and save "ptr" to it */
		save = plist->lptr2ftn;
		plist->lptr2ftn = ProtoHeadInit();
		if ( save )
			plist->lptr2ftn = ProtoCmp( save, plist->lptr2ftn,0); /*"merge"*/
	}

	DBG(ProtoDump( plist, 0 ));

	scope_proto_index = sptr->sproto;

	return;
}


#ifndef NODBG
/* For debugging: print function prototype list */
static void
ProtoDump(plist,nested)
t_protolist *plist;
int nested;
{
	t_prototype *ptype;

	fprintf( stderr,"ptlist(%s): ", 
				(plist->lsymidx > 0) ? stab[plist->lsymidx].sname : "??" );
	
	for (  ptype = plist->lhead; ptype != PNULL; ptype = ptype->pnext ) {

		putc(' ',stderr);
		tprint(stderr,ptype->ptype);
		fprintf(stderr," (%d,%d),",ptype->pdim,ptype->psiz);

		if (ptype->pnest) {						/* nested ftn prototype */
			fprintf( stderr," ->[" );
			ProtoDump( &proto_lists[ptype->pnest], 1 );
			fprintf( stderr,"]<- " );
		}
	}

	if ( plist->lflag & PVOID )
		fprintf( stderr, " VOID" );
	if ( plist->lflag & PVARARGS )
		fprintf( stderr, " VARARGS" );

	if ( plist->lptr2ftn ) {				/* ftn points to another */
		fprintf( stderr," ->*" );
		ProtoDump( &proto_lists[plist->lptr2ftn], 1 );	/* show latter's plist*/
		fprintf( stderr,"*<- " );
	}

	if ( !nested )
		putc('\n',stderr);
}
#endif


/* Having reduced a function DECLARATION (not a definition), */
/* remove placeholder symbols for prototype'd arguments from symtab. */
/* Return value just allows us to fit this into grammar more easily. */
NODE *
ProtoDeclare(fn)
NODE *fn;									/* function declaration node */
{
	t_prototype *ptype;
	unsigned short ftnidx;

	adjust_scope();	/* called to fix spr 5383: see explanation below */
	clearst(blevel+1);						/* clear symtab of any */
	checkst(blevel);						/* id's inside prototype-scope */

	ftnidx = stab[fn->tn.rval].sproto;		/* get indx into proto_lists */
	if (HASPROTO(ftnidx)) {					/* got a prototype? */

		ptype = proto_lists[ftnidx].lhead;

		for (  ;	ptype != PNULL;  ptype = ptype->pnext ) {

			if ( ptype->psym != PROTO_NOID ) {	/* got optional identifier? */
				struct symtab *sptr;

				sptr = &stab[ptype->psym];		/* got the symbol entry */
				if( sptr->sflags & SHIDES )		/* did it hide another sym? */
					unhide(sptr);
				sptr->stype = TNULL;			/* free up slot */
				DBG(fprintf(stderr, "REMOVING PROTO SYMBOL, idx = #%d\n", 
											ptype->psym));
			}
		}
		proto_lists[ftnidx].lflag |= PDECLARE;	/* this guy's not a definition*/
	}


	/* roger: Ansi 3.5.4.3. lines 19-22: and the type of each prototype parameter shall be compatible with the type that results from the application of the default argument promotions to the type of the corresponding identifier .  That means, in plain english, that we can not have floats in a prototype: they have to be doubles*/
	if ( promoted_floats )
		werror( W_DFLT,"type of prototype has to be compatible with the widened definition parameter. Use double instead of float." );
	promoted_floats = 0;
	return(fn);
}


/* Verify that ftn prototypes are valid for a function DEFINITION */
/* Note use of "lflag" (PDECLARE) in list header, a kludge to distinguish */
/* if last prototype for given function identifier was a declaration */
/* or given by the actual function definition. */
int 
ProtoDefine(fn)
NODE *fn;							/* fdeclarator */
{
	t_prototype *ptype;
	t_protolist *plist;
	unsigned short ftnidx;

	ftnidx = stab[fn->in.left->tn.rval].sproto;	/* get indx into proto_lists */
	if (!HASPROTO(ftnidx))			/* no prototype */
		return( OK );

	plist = &proto_lists[ftnidx];	/* we have a prototype */
	if ( plist->lflag & PDECLARE )	/* old-style ftn definition preceded */
		return( OK );				/* by prototype declaration */

	if (plist->lflag & PVOID) 		/* function takes 0 parameters */
		return( OK );

	ptype = plist->lhead;			/* point at start of ftn's prototypes */
	for (  ; ptype && ptype->ptype; ptype = ptype->pnext ) {
		if (ptype->psym == PROTO_NOID)	{
			uerror( "missing parameter identifier" );
			/** Hmm... **/
			return( BAD );
		}
	}

	return( OK );
}

					/**************************************/
					/** CHECK ACTUAL ARGS VS. PROTOTYPES **/
					/**************************************/


/* We've just parsed a function call. */
/* If there's a prototype declaration, typecheck arguments and */
/* perform implicit conversions. */
/* If not, perform default promotions. */
/* Always return OK for now. */
int 
ProtoChk(fn)
NODE *fn;						/* head of function call in parse tree */
{
	int argcnt;					/* number of actual arguments */
	int protocnt;				/* count of prototype'd parameters */
	int unknowns;				/* how many unknown argtypes? */
	NODE *arglist;				/* subtree of function call containing args */
	unsigned short ftnidx;		/* starting index of prototypes for this ftn */
	t_prototype *ptype;			/* point at saved prototypes for given funct */
	NODE *pnode, *chknode;		/* to build proto type checking tree nodes */
	t_protolist *plist;
	NODE *arg, *prev, **parent, *wrk;
	int nestarg;
	int ftnauto, i;

								/* ftn NAME is left child of CALL */
	wrk = ProtoId(fn->in.left);
	ftnidx = wrk->in.proto;

	protocnt = 0;
	arglist = fn->in.right;				/* arglist is right child of ftn node */
										/* at first, assume we know nothing */
	unknowns = argcnt = ProtoArgCnt( arglist );

	if ( !HASPROTO(ftnidx) ) {			/* sorry... no prototype */
		if ( ftnidx == 0 ) {			/* haven't complained yet */
			werror( W_LINT, "no prototype for function %s", 
						optype(wrk->tn.op) == LTYPE &&
							wrk->tn.rval!=NONAME && wrk->tn.op!=REG ? 
							stab[wrk->tn.rval].sname : "" );
			if ( wrk->tn.op == NAME )
				stab[wrk->tn.rval].sproto = WARNED;	/*prevent more warnings*/
		}
	} else {

		plist = &proto_lists[ftnidx];	/* we have a prototype */

										/* make sure # parms tally */
		if (plist->lflag & PVOID) {		/* function takes 0 parameters */
			if (argcnt != 0)
				uerror( "function prototyped as void" /*ignore arguments?*/ );
			return( OK );				/* this is debatable */
		}

		protocnt = plist->lcnt;			/* count of prototyped parameters */
		unknowns -= protocnt;			/* varargs */

		if ( argcnt < protocnt ) {
			uerror( "Less arguments than specified in prototype" );
			return( OK );				/* don't waste our time */
		} else {
			if ( (argcnt > protocnt) && !(plist->lflag & PVARARGS) ) {
				uerror( "More arguments than specified in prototype" );
				return( OK );
			}
		}
	}

									/* perform DEFAULT CONVERSIONS on */
									/* any un-prototyped parms. */
									/* ftn_arg takes care of float->double, */
									/* now that indtype won't by default */
	prev = (NODE *) 0;
	ftn_arg = 1;					/* tell indtype() this is special */
	for (  ;  unknowns;  --unknowns, arglist = arglist->in.left ) {
/*		if ( optype(arglist->in.op) == BITYPE ) { *  augh!!! **/
		if ( arglist->in.op == CM ) {
			arglist->in.right = pconvert( arglist->in.right, 0 );
		} else {
			if ( arglist == fn->in.right ) {		/* just one arg, period */
				fn->in.right = pconvert( arglist, 0 );
			} else {
				if ( prev == (NODE *) 0 )
					cerror( "ProtoChk tree traversal" );
				prev->in.left = pconvert( arglist, 0 );
			}
		}
		prev = arglist;
	}
	ftn_arg = 0;					/* back to float->float */

									/* now perform IMPLICIT CONVERSIONS */
									/* for all prototyped parms */
	if ( protocnt )
		for ( ptype = plist->lhead, i = 0; i < protocnt-1; i++ )
			ptype = ptype->pnext;		/* scan to end: arglist is RTOL */

	for (  	;  protocnt;  
			--protocnt, ptype=ptype->pprev, arglist=arglist->in.left ) {

		if ( arglist->in.op == CM ) {				/* arglist navigation */
			parent = &arglist->in.right;
			arg = arglist->in.right;
		} else {
			arg = arglist;
			if ( arglist == fn->in.right ) {		/* just one arg, period */
				parent = &fn->in.right;
			} else {
				parent = &prev->in.left;
			}
		}
		prev = arglist;

		nestarg = 0;
		if ( ISFTN(arg->tn.type) ) {
			if ( ptype->pnest )
				nestarg = arg->tn.rval;
		}
			

							/* Run through the full type-checking mechanism. */
							/* We can insert conversions if reasonable. */
		proto_argno = protocnt;	/* help make buildtree errors user-friendly */
		pnode = block( PROTOTYP, NIL, NIL,
					ARGCONV(ptype->ptype),ptype->pdim,ptype->psiz, 0);
		chknode = buildtree( PROTOCHK, pnode, arg );	/* chk & convert */
		*parent = chknode->in.right; 		/* in case CONV node inserted*/
		do {								/* free PROTOCHK/PROTOTYP nodes */
			chknode->tn.op = FREE;
		} while ( chknode = chknode->in.left );

		if ( nestarg )						/* nested ftn parm prototype */
			if ( nestarg = stab[ nestarg ].sproto )
				ProtoCmp( nestarg, ptype->pnest, PCALL );
	}

	return( OK );
}


static int
ProtoArgCnt( arglist )
NODE *arglist;
{
	int argcnt;

	if ( arglist == (NODE *) NULL )			/* UNARY CALL */
		return( 0 );

	for ( argcnt = 0;  arglist->in.op == CM; arglist = arglist->in.left )
		++argcnt;
	return( ++argcnt );
}


/* compare two prototypes for same function */
static int 
ProtoCmp(oldp,newp,nested)
unsigned short oldp;			/* index of old prototype */
unsigned short newp;			/* index of new prototype */
int nested;						/* boolean: is this a nested prototype? */
{
	t_prototype *ptype1, *ptype2;
	t_protolist *plist1, *plist2;
	int pcnt;

	if ( !HASPROTO(oldp) || !HASPROTO(newp) || oldp == newp ) {
		if ( !nested ) {
			cerror( "ProtoCmp args" );
		} else {
				/* Either: old guy has parm-list, new doesn't: make composite */
				/*     Or: they're both equal already */	
			DBG(fprintf( stderr, "PROTO NESTED COMPOSITE\n" ));
			return( oldp );	
		}
	}

	plist1 = &proto_lists[oldp];
	plist2 = &proto_lists[newp];
	ptype1 = plist1->lhead;
	ptype2 = plist2->lhead;

									/* first, make sure # parms tally */
	if (( plist1->lcnt != plist2->lcnt ) ||
		( (plist1->lflag & PVARARGS) != (plist2->lflag & PVARARGS) )) {
		uerror( "prototypes for %s specify unequal # of parameters",
				plist1->lsymidx ? stab[plist1->lsymidx].sname : "??" );
		pcnt = MIN( plist1->lcnt,plist2->lcnt );
	} else {
		pcnt = plist1->lcnt;
	}

									/* now check parameter-by-parameter */
	for (  ;  pcnt;  --pcnt, ptype1=ptype1->pnext, ptype2=ptype2->pnext ) {

/*		if ( nested != PCALL ) 
			ProtoDimChk( ptype1, ptype2 ); */

							/* corresponding parameters of function */
							/* prototypes must have compatible types */
		if (( ptype1->ptype != ptype2->ptype ) ||
			( dimtab[ptype1->psiz] != dimtab[ptype2->psiz] ) ||
			ProtoDimChk( ptype1, ptype2 ) )
			uerror( "incompatible function prototypes" );

		if (ptype1->pnest)  			/* nested ftn parm prototype */
			if ( nested != PCALL )		/* uh... */
				ptype2->pnest = ProtoCmp( ptype1->pnest, ptype2->pnest, 1 );
					/* We thus use 2nd parm-list if 1st lacks any; this may */
					/* conflict with ANSI 3.5.4.3 (69/15), but see 3.1.2.6. */
	}

	if ( plist1->lptr2ftn && plist2->lptr2ftn == 0 )
		plist2->lptr2ftn = plist1->lptr2ftn;	/* int (*foo(int))(char) */

	return( newp );					/* second plist overrides first, always */
}


/* Detail of ProtoCmp: Check if array dimensions are incompatible. */
/* Return TRUE if not incompatible.                                */
static int
ProtoDimChk( ptype1, ptype2 )
t_prototype *ptype1, *ptype2;
{
	int i;
	int dims;
	int dim1, dim2;
	TWORD ty;

	dims = 0;
	ty = ptype1->ptype;			/* by now, we know that both have same type */

	for( i=0; i<=(SZINT-BTSHIFT-1); i+=TSHIFT )
	{
		switch( (ty>>i)&TMASK )
		{
			case ARY:
				++dims;
				continue;
			default:
				break;
		}
	}

	dim1 = ptype1->pdim;
	dim2 = ptype2->pdim;

	for ( i = 0; i < dims; i++, dim1++, dim2++ ) 
	{
		if (dimtab[dim1] != dimtab[dim2]) {
			if (dimtab[dim1] == 0) {
				ptype1->pdim = ptype2->pdim; /* make composite array type */
				DBG(fprintf( stderr, "PROTO COMPOSITE\n" ));
			} else if (dimtab[dim2] == 0) {
				ptype2->pdim = ptype1->pdim; /* make composite array type */
				DBG(fprintf( stderr, "PROTO COMPOSITE\n" ));
			} else 
				return 1;
		}
	}
	return 0;
}


/* Extract prototype declaration from a function definition in source file */
void
ProtoGen( ftndef, argstk, argno, newstyle )
struct symtab *ftndef;
int argstk[];
int argno;
int newstyle;				/* is ftn definition newstyle or not? */
{
	static int firsttime = 1;		/* firsttime switch */
	int i;
	struct symtab *p;
	extern FILE *protofp;		/* see scan.c */
	FILE *out = protofp;		/* write on output file passed from driver */
	int varargs = 0;			/* special flag for ftn's taking varargs */
	extern int vaflag;			/* LINT VARARGS comment value from scanner */

	if ( firsttime ) {
		char datestamp[30];
		long mytime;
		struct tm *tminfo;

		firsttime = 0;
		mytime = time( (long *) 0 );
		tminfo = localtime( &mytime );
		ascftime( datestamp, "%b %d %Y, %H:%M", tminfo);
		fprintf( out, 
		"\n\n/*\tFunction Prototypes for \n *\t%s\n *\t(generated %s)\n */\n", 
								ftitle, datestamp  );
		fprintf( out, 
"\n#if __STDC__ > 0\n#define _PARMS(x) x\n#else\n#define _PARMS(x) ()\n#endif\n\n" );

	}	/* end of header */

	if ( ftndef->sclass == STATIC )		/* alert user if static function */
		werror( W_DFLT, "Prototype generated for static function %s", 
											ftndef->sname );
	fprintf( out, "%s ", lscnames(ftndef->sclass) );

	ProtoPrint( ftndef, out, 0 );
	fprintf( out, " %s _PARMS((", ftndef->sname );

	if ( (HASPROTO(ftndef->sproto) && 		/* new-style with ellipsis? */
			(proto_lists[ ftndef->sproto ].lflag & PVARARGS))
		|| vaflag > 0 ) {						/* LINT VARARGSn comment? */
		varargs = 1;
		if ( vaflag > 0 ) {
			if ( vaflag > argno )
				uerror( "VARARGS comment specifies too many args" );
			else
				argno = vaflag;
		}
	}

	if ( argno == 0 ) {
		fprintf( out, "void" );
	} else {
		for ( i=0; i<argno; i++ ) {
			p = &stab[ argstk[i] ];
			ProtoPrint( p, out, newstyle );
			if ( i < argno-1 ) {
				putc(',',out);
				putc(' ',out);
			}
		}
	}

	if ( varargs )
		fprintf( out, ", ... " );

	fprintf( out, "));\n" );
}

/* unravel and print a symbol's declaration */
static void
ProtoPrint( sym ,out, newstyle )
struct symtab *sym;
FILE *out;
int newstyle;
{
	TWORD ty, tq; 
	int dim;
	struct {				/* beats recursion, and more than enough */
		TWORD type, typeq;	/* for 13 levels of derived type */
	} stack[20];
	int sx;
	int i;

	 /* output a nice description of the type of t */
	static char * tnames[] = 
	{
			"undef",
			"farg",
			"char",
			"short",
			"int",
			"long",
			"float",
			"double",
			"strty",				/* won't print */
			"unionty",				/* won't print */
			"enumty",				/* won't print */
			"moety",				/* won't print */
			"unsigned char",
			"unsigned short",
			"unsigned int",
			"unsigned long",
			"void", "?"
	};

	ty = sym->stype;
	tq = sym->stypeq;
	if ( !newstyle ) {
		ftn_arg++;
		MODTYPE(ty,indtype(ty));		/* widen basic type of argument */
		ftn_arg--;
	}
	dim = sym->dimoff;

	sx = 0;
	memset(stack,0,sizeof(stack));

	DBG(fprintf(stderr,"(%o/%o)",ty,tq));

	for ( i = 0; i <= (SZINT-BTSHIFT-1); i+=TSHIFT ) {

		stack[sx].typeq = (tq >> i) & TMASK;

		switch ( (ty >> i) & TMASK ) {
			case PTR:
				if (( i == 0 ) &&
					( ((ty>>(i+TSHIFT)) & TMASK ) == ARY )) {
					--dim;
					stack[sx++].type = ARY;
				} else {
					stack[sx++].type = PTR;
				}
				break;
			case ARY:
				stack[sx++].type = ARY;
				break;
			case FTN:
				stack[sx++].type = FTN;
				break;
			default:
			case 0:							/* no more derived types */
				break;
		}
	}

										/* print any basic type qualifiers */
	if ( sym->stypeq & CONST )		
		fprintf( out, "const " );
	if ( sym->stypeq & VOLATILE )
		fprintf( out, "volatile " );
												/* print basic type first */
	if ( AGGREGATE(ty) ) {
		char *name;
		int tag = dimtab[ sym->sizoff+3 ];		/* symidx of structure tag */

		if ( tag == -1 ) {						/* unnamed template */
			name = ProtoStructID( sym );		/* try to get typedef name */
			if ( name == NULL ) {
				werror( W_DFLT, 
					"Cannot generate prototype for nameless templates" );
				fprintf( out, "??? " );
				return;							/* forget this arg */
			} else
				fprintf( out, "%s ", name );
		} else {  								/* tagged aggregate */
			char *key;
			switch ( BTYPE(ty) ) {
				default:
				case STRTY:
					key = "struct";
					break;
				case UNIONTY:
					key = "union";
					break;
				case ENUMTY:
					key = "enum";
					break;
			}
			fprintf( out, "%s %s ", key, stab[ tag ].sname );
		}
	} else
		fprintf( out, "%s ",tnames[ty&BTMASK] );

	while ( --sx >= 0 ) {					/* then derived types */
		tq = stack[sx].typeq;
		switch ( stack[sx].type ) {
			default:
				goto fatal;
			case PTR:
				fprintf( out, "*" );
				break;
			case ARY:
				fprintf( out, "[%.0d]", dimtab[dim++] );
				break;
			case FTN:
				if ( --sx >= 0 ) {
					if ( stack[sx].type != PTR ) {
						++sx;
					} else {
						fprintf( out, "(*)()" );
					}
				}
				break;
		}
		if ( tq ) fprintf( out, "%s", tq == CONST ? "const " : "volatile " );
	}
	return;
			
fatal:
	cerror ( "ProtoGen stack" );
}

/* A parameter is of an unnamed structure/union/enum type: */
/* Find a typedef name to use instead, if possible. */
/* Return pointer to typedef name if found, NULL otherwise. */
static char *
ProtoStructID( sym )
struct symtab *sym;			/* parameter's symbol entry */
{	
	int i;

	for ( i = 0; i<SYMTSZ; i++ ) {	/* linear search through symbol table */
		struct symtab *p;

		p = &stab[i];
		if ( p->sizoff == sym->sizoff && p!=sym ) {
			if ( p->sclass == TYPEDEF ) {
				if (BTYPE(p->stype) != STRTY && 		/* sanity checks */
					BTYPE(p->stype) != UNIONTY &&
					BTYPE(p->stype) != ENUMTY)
				cerror( "ProtoPrint: tag" );
				return p->sname;						/* FOUND IT!! */
			}
		}
	}
													/* no match */
	return NULL;
}


/* Enforce ANSI 3.5.4.3 SemP6S3 */
void
ProtoMix( fdcl )
NODE *fdcl;
{
	unsigned short ptx;
	t_protolist *plist;
	t_prototype *ptype;
	int pcnt;
	int bad;

	fdcl = ProtoId( fdcl );
	ptx = stab[ fdcl->tn.rval ].sproto;
	if ( !HASPROTO(ptx) )
		return;

	plist = &proto_lists[ptx];
								
	bad = plist->lflag & PVARARGS;
	if ( !bad ) {
		ptype = plist->lhead;  
		pcnt = plist->lcnt; 
		for (  ; pcnt;  --pcnt, ptype = ptype->pnext )
		{
			if ( indtype( ptype->ptype ) != ptype->ptype ) {
				bad = 1;
				break;
			}
		}
	}

	if ( bad )	/* don't see any reason why this should be more than a warning*/
		werror( W_STRICT, "incompatible function declarations" );
}


/* Search for function identifier in (possibly) complex declarator tree */
static NODE *
ProtoId(dtree)
NODE *dtree;
{
	if ( dtree == (NODE *) 0 )
		cerror( "ProtoId: unidentifiable function" );

	while ( optype(dtree->in.op) != LTYPE ) {
								/* don't be confused by fpf()() */
		if ( !indcl  && callop(dtree->in.op) )
			break;
		dtree = dtree->in.left;
	}

	switch ( dtree->in.op ) {	/* validity check */
		case NAME:				/* external name */
		case VAUTO:				/* automatic variable */
		case VPARAM:			/* function parameter */
		case REG:				/* cpu register */
		case ICON:				/* e.g. (*(int (*)())1)();	*/
		case CALL:
		case UNARY CALL:
		case STCALL:
		case UNARY STCALL:
			break;
		default:
			cerror( "ProtoId, invalid node" );
			break;
	}

	return( dtree );
}

/* We have just recognized a cast-expression:             *
 * Resolve subtle differences between the productions for *
 * cast-expression vs. parameter-declaration              */
void 
ProtoMirage()
{
	extern int saveclass, tqshift;	/* DECL_RESET should be in manifest.h! */

	if ( pstk_indx )
		(void) ProtoPop();

	if ( curclass )
		uerror( "storage class illegal in cast expression" );
	curclass = saveclass;
	saveclass = SNULL;
	typequal = 0;
	tqshift = 0;
	indcl = 0;
}

static char *lccnames[] = 
{
	 /* names of storage classes */
	0, 0,
	"extern",
	"static",
	0, "extern", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#ifdef IN_LINE
	0,
#endif
};

static char *
lscnames( c )
register c; 
{
	/* return the name for storage class c */
	if (( c&FIELD ) || (lccnames[c] == 0) )
		cerror( "Insane function class" );

	return( lccnames[c] );
}

/* roger: this little function is called to link to the
scopestack list all the identifiers that we have found in
function prototypes. Doing that we achieve that "clearst" will
clear the symbol table properly. */
adjust_scope()
{
	struct symtab *optional_name;
	t_prototype *temp;

	temp = proto_lists[scope_proto_index].lhead;
	while ( temp )
	{
		if (temp->psym > 0)
		{
			optional_name = &stab[temp->psym];
			optional_name->scopelink = scopestack[blevel+1];
			scopestack[blevel+1] = optional_name;
		}
		temp = temp->pnext;
	}
}
