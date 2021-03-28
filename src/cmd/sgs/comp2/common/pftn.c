#ident	"@(#)pftn.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.6	89/09/11 17:24:50 barryk
	Changed wording of warning for hiding nested extern declarations

    1.5	89/09/11 16:16:44 barryk
	

    1.4	89/09/11 15:25:02 barryk
	block-nested extern declarations & goto label namespace

    1.3	89/08/25 11:41:15 barryk
	(bk#6) warning for incompatible REGISTER storage class declarations.

    1.2	89/06/26 15:46:49 hossein
	changes related to "cosnt" type qualifier for POSIX.

    1.1	89/06/08 00:38:32 root
	date and time created 89/06/08 00:38:32 by root

*/
/*	@(#)pftn.c	7.3		*/
static	char	SCCSID[] = "@(#) pftn.c:	@(#)pftn.c	7.3	";

# include "mfile1.h"
# include "proto.h"
# include <limits.h>		/* bk: for INT_MAX */
# include "modes.h"			/* bk: for pragma control info */

extern int compiler_mode;	/* Set to determine behavior */
unsigned int maxoffset;
extern int protoflg;		/* bk: should be in xdefs.c */
extern int argspace;			/* (same) */
extern int noilist;
extern TWORD struc_qual[];
extern int sulvl;
extern int tentative_flag;
int enum_chk;				/* for moedef */
int chk_struct;
extern int maybe_cast;			/* roger: flag for rstruct */
extern int promoted_floats;			/* roger: flag for ANSI warning 3.5.4.2*/

 /*bk: size bumped from 10 to satisfy ANSI 2.2.4.1 */
#define STR_NEST 21		/* max level of structure nesting */
struct instk 
{
	int in_sz;   /* size of array element */
	int in_x;    /* current index for structure member in initializations */
	int in_n;    /* number of initializations seen */
	int in_s;    /* sizoff */
	int in_d;    /* dimoff */
	TWORD in_t;    /* type */
	TWORD in_tq;   /* type qualifier */
	int in_id;   /* stab index */
	int in_fl;   /* flag which says if this level is controlled by {} */
	OFFSZ in_off;  /* offset of the beginning of this level */
}instack[STR_NEST],*pstk;

int brack[STR_NEST]; /* roger: flags which controll inconsistent bracketing */
int cur_level = 0;  /* roger: current level of bracketing */

struct symtab *relook();

#ifndef NODBG	/* ken#4 */
int structflg = 0, bracketinc = 0, typeflg = 0, hitstruct = 0, printsize = 0;
#endif

int hlflag = 0;	/* Motorola 4.1 */

int ddebug = 0;
#ifdef m68k
int struct_init = 0; /* Tells if we are in the middle of initializing
		     ** a structure.
		     */
#endif

struct symtab * mknonuniq();

defid( q, class )
register NODE *q; 
{
	register struct symtab *p;
 	extern struct symtab *scopestack[];
	int idp;
	register TWORD type;
	register TWORD stp;
	register scl;
	register dsym, ddef;
	register slev, temp;
	int labspace;
	int regabstract = 0;

	if( q == NIL ) return;  /* an error was detected */
	if( q < node || q >= &node[TREESZ] ) cerror( "defid call" );
	idp = q->tn.rval;
	if( idp < 0 ) cerror( "tyreduce" );
	p = &stab[idp];
# ifndef NODBG
	if( ddebug )
	{
		printf( "defid( %s (%d), ", p->sname, idp );
		qprint( q->in.typeq );
		tprint(stdout, q->in.type );
		printf( ", %s, (%d,%d) ), level %d\n", scnames(class),
		q->fn.cdim, q->fn.csiz, blevel );
	}
# endif

		/**bk#8**/
	/* Label symbols are separated from their non-label namesakes: */
	/* (Either add a separate symbol, or merge with a previous */
	/* label symbol if we find one.) */
	labspace = 0;
	if ((p->sclass != ULABEL) && (p->sclass != LABEL)) {
		if ((class == ULABEL) || (class == LABEL)) {

			int j;
			j = lookup( p->sname, SLAB );
			if (( stab[j].sclass == LABEL ) ||
			    ( stab[j].sclass == ULABEL )) {
			    p = &stab[idname = j];
			} else {
			    if ( p->sclass != SNULL && 
				    stab[j].stype == UNDEF ) {

					labspace = 1;
					stab[j].stype = TNULL;
		
					q->tn.rval = idp = mkspace( p );
					p = &stab[idp];
			    }
			}
		}
	}
	/* end of bk#8 */
	fixtype( q, class );
	type = q->in.type;
	class = fixclass( class, type, &regabstract );
	stp = p->stype;
	slev = p->slevel;
# ifndef NODBG
	if( ddebug )
	{
		printf( "	modified to " );
		if ( class == STNAME || class == UNAME)
			qprint( 0 );
		else
			qprint( typequal );
		tprint(stdout, type );
		printf( ", %s\n", scnames(class) );
		printf( "	previous def'n: " );
		tprint(stdout, stp );
		printf( ", %s, (%d,%d) ), level %d\n", scnames(p->sclass),
		p->dimoff, p->sizoff, slev );
	}
# endif
	if (labspace)
			goto enter;

	if( stp == FTN && p->sclass == SNULL )goto enter;
	/* name encountered as function, not yet defined */
	/* BUG here!  can't incompatibly declare func. in inner block */
	if( stp == UNDEF|| stp == FARG )
	{
		if( blevel==1 && stp!=FARG ) switch( class )
		{
		default:
			if(!(class&FIELD)) uerror(
			"declared argument %s is missing", p->sname );
		case MOS:
		case STNAME:
		case MOU:
		case UNAME:
		case MOE:
		case ENAME:
		case TYPEDEF:
			;
		}
		goto enter;
	}
		/* bk:  long and int are different, even if same size */
# define ANSI_CTYPE(x) 	( (x&(~BTMASK)) | 						\
						((BTYPE(x) == LONG) ? INT :				\
						(BTYPE(x)==ULONG) ? UNSIGNED : BTYPE(x)) )
	if( type != stp ) {
		if ( ISFTN( type ) && ( ANSI_CTYPE(type) == ANSI_CTYPE(stp) ) )
				werror( W_STRICT, "non-portable redeclaration of return type" );
		else 
			goto mismatch;
	}
	/* test (and possibly adjust) dimensions */
	dsym = p->dimoff;
	ddef = q->fn.cdim;
	for( temp=type; temp&TMASK; temp = DECREF(temp) )
	{
		if( ISARY(temp) )
		{
			if( dimtab[dsym] == 0 ) dimtab[dsym] = dimtab[ddef];
			else if( dimtab[ddef]!=0 && dimtab[dsym] != dimtab[ddef] )
			{
				goto mismatch;
			}
			++dsym;
			++ddef;
		}
	}
	/* check that redeclarations are to the same structure */
	if( (temp==STRTY||temp==UNIONTY||temp==ENUMTY) && p->sizoff != q->fn.csiz
	    && class!=STNAME && class!=UNAME && class!=ENAME )
	{
		goto mismatch;
	}
	scl = ( p->sclass );
# ifndef NODBG
	if( ddebug )
	{
		printf( "	previous class: %s\n", scnames(scl) );
	}
# endif
	if( class&FIELD )
	{
		/* redefinition */
		if( !falloc( p, class&FLDSIZ, 1, NIL ) ) 
		{
			/* successful allocation */
			psave( idp );
			return;
		}
		/* blew it: resume at end of switch... */
	}
	else switch( class )
	{
	case EXTERN:
		switch( scl )
		{
		case STATIC:
		case USTATIC:
			if( slev==0 ) {
/*				uerror( "Storage class conflict" );*/	/*bk*/
				return;
			}
			break;
		case EXTDEF:
		case EXTERN:
		case FORTRAN:
		case UFORTRAN:
#ifdef IN_LINE
		case INLINE:
#endif
			return;
		}
		break;
	case STATIC:
		if( scl==USTATIC || (scl==EXTERN && blevel==0) )
		{
			p->sclass = STATIC;
			if( ISFTN(type) ) curftn = idp;
			return;
		}
		break;
	case USTATIC:
		if( scl==STATIC || scl==USTATIC ) return;
		break;
	case LABEL:
		if( scl == ULABEL )
		{
			p->sclass = LABEL;
			deflab( p->offset );
			return;
		}
		break;
	case TYPEDEF:
		if( scl == class ) {	/* if benign, warn anyway (ANSI constraint) */
			if ( !mode_chk( MODE_BENIGN ) )
				werror( W_STRICT, "benign re-typedef" );
			return;
		}
		break;
	case UFORTRAN:
		if( scl == UFORTRAN || scl == FORTRAN ) return;
		break;
	case FORTRAN:
		if( scl == UFORTRAN )
		{
			p->sclass = FORTRAN;
			if( ISFTN(type) ) curftn = idp;
			return;
		}
		break;
#ifdef IN_LINE
	case INLINE:
		curftn = idp;
		break;
#endif
	case MOU:
	case MOS:
		if( scl == class ) 
		{
			if( oalloc( p, &strucoff, strucdef) ) break;
			if( class == MOU ) strucoff = 0;
			psave( idp );
			return;
		}
		break;
	case MOE:
		if( scl == class )
		{
/*			if( p->offset!= strucoff++ ) break;		*/ 	/* bk */
			if( p->offset!= strucoff ) break;
			bump_enum();
			psave( idp );
		}
		break;
	case EXTDEF:
		if( scl == EXTERN ) 
		{
			p->sclass = EXTDEF;
			if( ISFTN(type) ) curftn = idp;
			return;
		}
		break;
	case STNAME:
	case UNAME:
	case ENAME:
		if( scl != class ) break;
		if( dimtab[p->sizoff] == 0 ) {
			if ( !mode_chk( MODE_TAGBLOCK ) ||
				blevel <= slev )	/* ANSI 3.5.2.3 Semantics P2S3 */
				return;  /* previous entry just a mention */
		}
		break;
	case ULABEL:
		if( scl == LABEL || scl == ULABEL ) return;
	case PARAM:
	case AUTO:
	case REGISTER:
		;  /* mismatch.. */
	}
mismatch:
	/* allow nonunique structure/union member names */
	if( class==MOU || class==MOS || class & FIELD )
	{
		/* make a new entry */
		register * memp;
		p->sflags |= SNONUNIQ;  /* old entry is nonunique */
		/* determine if name has occurred in this structure/union */
		for( memp = &paramstk[paramno-1];
			/* while */ *memp>=0 && stab[*memp].sclass != STNAME
				&& stab[*memp].sclass != UNAME;
			/* iterate */ --memp)
		{
			if( stab[*memp].sflags & SNONUNIQ )
			{
				if ( p->sname != stab[*memp].sname )
					continue;
				uerror("redeclaration of: %s",
						p->sname);
				break;
			}
		}
		p = mknonuniq( &idp ); /* update p and idp to new entry */
		goto enter;
	}
			
	if ( ( class == MOE && argspace ) ||
		( (blevel > slev) && 
	    /* && class != EXTERN */ 	/* bk#7 */
		class != FORTRAN &&
#ifdef IN_LINE
	    class != INLINE &&
#endif
	    class != UFORTRAN ) )
	{
		if ((class == EXTERN) && (slev == 0)) {		/* bk#7 */
			if ( inscope(p, mode_chk(MODE_XBLOCK) ? blevel : slev) ) {
				goto redec;
			} else {
				werror ( W_LINT, "extern %s hides global of different type",
					p->sname);
			}
		}
		q->tn.rval = idp = hide( p );
		p = &stab[idp];
		goto enter;
	}

			/* bk: allow redeclaration of compatible structure types */
/*	if ( class == STNAME ) {		** needs work in dclstruct **
		chk_struct = p->sizoff;
		return;
	} */

redec:
	uerror( "redeclaration of %s", p->sname );
	if( class==EXTDEF && ISFTN(type) ) curftn = idp;
	return;
enter:  /* make a new entry */
# ifndef NODBG
	if( ddebug ) printf( "	new entry made\n" );
# endif
	if( type == UNDEF ) uerror("void type for %s",p->sname);
	p->stype = type;
	p->sclass = class;
	p->slevel = blevel;
	p->offset = NOOFFSET;
	p->suse = lineno;
	if (regabstract)		/* bk */
		p->sflags |= SREG;	/* can't allocate real reg, but do usage checks */

	if ( class == STNAME || class == UNAME ) 	/* bk: tag isn't qualified */
		p->stypeq = 0;
	else
		p->stypeq = typequal;

	if( class == STNAME || class == UNAME || class == ENAME ) 
	{
		p->sizoff = curdim;
		dstash( 0 );  /* size */
		dstash( -1 ); /* index to members of str or union */
		dstash( ALSTRUCT );  /* alignment */
		dstash( idp );  /* name index */
	}
	else 
	{
		switch( BTYPE(type) )
		{
		case STRTY:
		case UNIONTY:
		case ENUMTY:
			p->sizoff = q->fn.csiz;
			p->stypeq |= struc_qual[sulvl];			/* bk */
# ifndef NODBG
			if( ddebug )
			{
				printf( "	structure var typequal: " );
				qprint( p->stypeq );
				qprint( struc_qual[sulvl] );
				printf( "\n" );
			}
#endif
			break;
		default:
			p->sizoff = BTYPE(type);
		}
	}

	/* copy dimensions */
	p->dimoff = q->fn.cdim;
	/* allocate offsets */
	if( class&FIELD )
	{
		falloc( p, class&FLDSIZ, 0, NIL );  /* new entry */
		psave( idp );
	}
	else switch( class )
	{
	case AUTO:
		/* Motorola 4.1 */
		/* if high-level optimizer follows c0 we spread out addresses */
		/* to allow coptim to uniquely tie frame addresses to objects  */
		if (hlflag)
			autooff += COPTPAD * SZCHAR;  /* address spread for locals */
		oalloc( p, &autooff, 0 );
		break;
	case STATIC:
	case EXTDEF:
		p->offset = getlab();
#ifdef IN_LINE
	case INLINE:
#endif
		if( ISFTN(type) ) curftn = idp;
		break;
	case ULABEL:
	case LABEL:
		p->sflags |= SLAB;	 /*bk#8*/
		p->offset = getlab();
		p->slevel = 2;
		if( class == LABEL )
		{
			locctr( PROG );
			deflab( p->offset );
		}
		break;
	case EXTERN:
	case UFORTRAN:
	case FORTRAN:
		p->offset = getlab();
		p->slevel = 0;
		break;
	case MOU:
	case MOS:
		oalloc( p, &strucoff, strucdef );
		if( class == MOU ) strucoff = 0;
		psave( idp );
		break;
	case MOE:
		p->offset = bump_enum();
		psave( idp );
		break;
	case REGISTER:
		/* nextrvar is left set by cisreg when it returns 1 */
		p->offset = nextrvar;
/*		if( blevel == 1 ) p->sflags |= SSET;		*/	/* bk: not used! */
		break;
	}
	/* user-supplied routine to fix up new definitions */

# ifdef FIXDEF
	if ( tentative_flag )	/* bk: context where tentative def is possible? */
		if ( p->sclass != EXTERN )		/* well then: is it tentative? */
			tentative_flag = 0;
	FIXDEF(p);
# endif
 	if (blevel >= MAXNEST)
 	{
 		cerror("too many nesting levels");
 		/*NOTREACHED*/
 	}

					/*bk#7*/
		/* don't link block-nested extern into scopestack at external level */
	if ((p->sclass == EXTERN) && (blevel >= 2) && mode_chk( MODE_XBLOCK )) {
	    p->scopelink = scopestack[slev = blevel];
	    scopestack[slev] = p;
	} else {
	    p->scopelink = scopestack[slev = p->slevel];
	    scopestack[slev] = p;
	}
# ifndef NODBG
	if( ddebug )
		printf( "	dimoff, sizoff, offset: %d, %d, %d\n",
			p->dimoff, p->sizoff, p->offset );
	if (printsize) {	/* begin ken#4 */
	    if (((BTYPE(type)==STRTY || BTYPE(type)==UNIONTY)
		    && class == TYPEDEF)){
		typeflg &= ~(bracketinc<<1);
		bracketinc--;
	    } /* if */
	    if ((class == UNAME) || (class == STNAME)) {
		fprintf(stderr,"%.*sBEGIN STRUCT/UNION: ", bracketinc+blevel,
			"                                        ");
		tprint(stderr, type);
		fprintf(stderr," %s\n", p->sname);
		bracketinc++;
		structflg |= (bracketinc<<1);
		hitstruct = 1;
	    } /* if */

	    if (((BTYPE(type)==STRTY || BTYPE(type)==UNIONTY)
			&& class == TYPEDEF)){
		fprintf(stderr,"%.*sEND TYPEDEF: ", bracketinc+blevel,
			"                                        ");
		tprint(stderr, type);
		fprintf(stderr," %s:\tsize[ %#x ]\n\n", 
			p->sname, 
			tsize(q->in.type, q->fn.cdim, q->fn.csiz)/SZCHAR);
	    } /* if */

	    if ((class == MOU) || (class == MOS)) {
	    /* not quite right --- puts extra indent in */
		if (hitstruct || (typeflg & (bracketinc<<1))) {
		    hitstruct = 0;
		} else {
		    fprintf(stderr,"%.*sBEGIN TYPEDEF\n", bracketinc+blevel,
		    "                                        ");
		    bracketinc++;
		    typeflg |= (bracketinc<<1);

		} /* if */
		fprintf(stderr,"%.*s",
		    bracketinc+blevel,"                                        ");
		tprint(stderr, type);
		fprintf(stderr," %s:\t%#x", p->sname, p->offset/SZCHAR);
		if (is_ptr(type)) {
		    if ((p->offset/SZCHAR) % 4)
			fprintf(stderr,"<----- %% 4");
		} else {
		    switch(BTYPE(type)) {
			case CHAR: case UCHAR: break;
			case SHORT: case USHORT:
			    if ((p->offset/SZCHAR) % 2)
				fprintf(stderr,"<----- %% 2");
			    break;
			case INT: case UNSIGNED: 
			case LONG: case ULONG: case FLOAT:
			    if ((p->offset/SZCHAR) % 4)
				fprintf(stderr,"<----- %% 4");
			    break;
			case DOUBLE:
			    if ((p->offset/SZCHAR) % 8)
				fprintf(stderr,"<----- %% 8");
			    break;
			default:
			    
			    if ((p->offset/SZCHAR) %4)
				fprintf(stderr,"<------- default %% 4");
			    break;
		    } /* switch */
		} /* if is_ptr */
		fprintf(stderr,"\n");
	    } /* if */
	} /* end ken#4: if */	
# endif
}

asave( i )
{
	if( argno >= ARGSZ )
	{
		cerror( "too many arguments");
	}
	argstk[ argno++ ] = i;
}

psave( i )
{
	if( paramno >= PARAMSZ )
	{
		cerror( "parameter stack overflow");
	}
	paramstk[ paramno++ ] = i;
}

/* maximium size of outgoing arguments */
int maxarg;

ftnend()
{
	 /* end of function */
	if( retlab != NOLAB )
	{
		 /* inside a real function */
		efcode();
# ifdef SDB
		{
			register i;
			register struct symtab *p;
			register TWORD t;
			for( i=0; i<argno; ++i )
			{
				/* use the saved types */
				p = &stab[argstk[i]];
				t = p->stype;
				p->stype = argty[i];
				if( p->sclass == REGISTER )
				{
					p->sclass = PARAM;
					p->offset = argsoff[i];
				}
				/*ppstab(p);*/
				p->stype = t;
			}
		}
# endif
# ifndef TWOPASS
		p2bend();
# else
# ifdef MC68K
		printf("]0\t0\t\n");	/* end of block mark */
# else
		printf("]\n");	/* end of block mark */
# endif
# endif
	}
	if (ddebug) 
		checkst(0);
	retstat = 0;
	tcheck();
	curclass = SNULL;
	brklab = contlab = retlab = NOLAB;
	flostat = 0;
	strftn = 0;
#ifdef MC68K
	ftntype = TNULL;
#endif
	argno = 0;
	if( nerrors == 0 )
	{
		if( psavbc != & asavbc[0] ) cerror("bcsave error");
		if( paramno != 0 ) cerror("parameter reset error");
		if( swx != 0 ) cerror( "switch error");
	}
	psavbc = &asavbc[0];
	paramno = 0;
	autooff = AUTOINIT;
	maxarg = 0;
	reached = 1;
	swx = 0;
	swp = swtab;
	locctr(DATA);
}

dclargs()
{
	register i, j;
	register struct symtab *p;
	register NODE *q;
	register TWORD temp;
	extern TWORD simpstr();
	int	newstyle,		/* new-style ftn def */
		oldwproto; 		/* old-style ftn def, but comes with a prototype */
	t_prototype *pt;
	t_protolist *plist;
	int protoargs;

	argoff = ARGINIT;

						/* ask if old-style def but has a prototype */
	oldwproto = newstyle = 0;
	i = stab[curftn].sproto;
	if ( HASPROTO(i) ) {
		if ( (plist = &proto_lists[i])->lflag & PDECLARE ) {
			if ( (plist->lcnt != argno) && !(plist->lflag & PVARARGS) )
				uerror( "Incorrect number of arguments" );
			oldwproto = 1;
			protoargs = plist->lcnt;
		} else
			newstyle = 1;			/* new-style ftn-def needs symbol fixing */
		pt = plist->lhead;
	} 

# ifndef NODBG
	if( ddebug > 2) printf("dclargs()\n");
# endif
	for( i=0; i<argno; ++i )
	{
		if( (j = argstk[i]) < 0 ) continue;
		p = &stab[j];
# ifndef NODBG
		if( ddebug > 2 )
		{
			printf("\t%s (%d) ",p->sname, j);
			tprint(stdout,p->stype);
			printf("\n");
		}
# endif

		if ( newstyle || oldwproto ) 				/* prototype work */
		{
			if ( oldwproto ) {
				if( p->stype == FARG ) 
				{
					q = block(FREE,NIL,NIL,INT,0,INT, protoargs ? pt->pqual: 0);
					q->tn.rval = j;
					defid( q, PARAM );
				}
/* ANSI: "old-style fn parms are taken as widened before type-matching" */
/* bk:	really, if we choose for efficiency to not widen */
/* certain types, we may produce bad code if a "narrow" prototype */
/* does not match a "wide" ftn definition (e.g. "float" in prototype, */
/* followed by function call with no float->double widening, followed by  */
/* an old-style ftn-def declaring "float", which is widened to double by */
/* the time we get here). For now, we look stupid with this error in */
/* cases involving narrow types, so we make it a STRICT-level warning. */
				if ( protoargs <= 0 ) {
					uerror( "Incorrect number of arguments" );
				} else if ( indtype(p->stype) != pt->ptype ) { /* way off! */
					if ( indtype(p->stype) == indtype(pt->ptype) ) {
						werror( W_STRICT,
						"Parameter declaration (%s) conflicts with prototype",
						p->sname );
					} else {
						uerror(
						"Parameter declaration (%s) clashes with prototype",
						p->sname );
					}
				}
			} else {							/* newstyle */
				p->slevel--; /* was set 1 level higher for "prototype scope" */
				q = block(FREE,NIL,NIL,pt->ptype,pt->pdim,pt->psiz, pt->pqual );
				q->tn.rval = j;
				typequal = pt->pqual;
				defid( q, pt->pclass );
				typequal = 0;
			}

			if ( oldwproto ) {
				if ( --protoargs > 0 )
					pt = pt->pnext;
			} else 
				pt = pt->pnext;

		} else if( p->stype == FARG ) 
		{
			q = block(FREE,NIL,NIL,INT,0,INT, 0 );
			q->tn.rval = j;
			defid( q, PARAM );
		}

		if( p->sclass == REGISTER )
		{
			/* must set aside space, fill argsoff */
			int tmp = p->offset;
			p->offset = NOOFFSET;
			p->sclass = PARAM;
			oalloc( p, &argoff, 1 );
			argsoff[i] = p->offset;
			argty[i] = p->stype;
			p->sclass = REGISTER;
			p->offset = tmp;
		}
		else 
		{
			oalloc( p, &argoff, 1 );  /* always set aside space */
			argsoff[i] = p->offset;
			argty[i] = p->stype;
		}
	}
	autooff = AUTOINIT;
# ifdef CENDARG
	CENDARG();
# endif
#ifdef IN_LINE
	if (curclass != INLINE)  /* No code gen'ed for INLINE asm 'function'*/
#endif
	{               /* start INLINE conditional block */
	locctr(PROG);
	defalign(ALINT);
	++ftnno;
	p = &stab[curftn];

	if ( protoflg )
		ProtoGen( p, argstk, argno, newstyle );			/* generate prototype */

	if( p->slevel>1 && p->sclass == STATIC ) deflab( p->offset );
	else defnam( p );
	temp = p->stype;
	temp = DECREF( temp );
#ifdef MC68K
	ftntype = temp;
#endif

	/* optimization: functions returning short structs */
	/* strftn != 0 if function returns structure */
	strftn = (temp==STRTY) || (temp==UNIONTY);
	if( strftn && simpstr( p->dimoff, p->sizoff ) != STRTY )
		strftn = 0;

	bfcode( argstk, argno );
# ifndef NOREGARGS
	regargs();
# endif
# ifdef STATSRET
	/* code for the case where returning a structure uses a static */
	if( strftn ) 
	{
		  /* scalars ok */
		/* define something the proper size */
		i = tsize( temp, p->dimoff, p->sizoff);
# ifdef SRETNAME
		SRETNAME(i);
# else
		locctr( DATA );
		defalign( talign( temp, p->sizoff ) );
		deflab ( strftn = getlab() );
		zecode( (i+SZINT-1)/SZINT );  /* set aside integer zeros */
		locctr( PROG );
# endif
	}
# endif
	}       /* end INLINE conditional block */
}

# ifndef NOREGARGS
regargs()
{
	register i;
	register NODE *p, *q;
	register struct symtab *s;
	/* output the copy assignements for register arguments */
	for( i=0; i<argno; ++i )
	{
		s = &stab[argstk[i]];
		if( s->sclass == REGISTER )
		{
			int temp;
			idname = argstk[i];
			p = buildtree( NAME, NIL, NIL );
			temp = s->offset;
			s->offset = argsoff[i];
			s->sclass = PARAM;
			q = buildtree( NAME, NIL, NIL );
			p = buildtree( ASSIGN, p, q );
			ecomp( p );
			s->offset = temp;
			s->sclass = REGISTER;
		}
	}
}
# endif

extern int yychar; /*roger: we need to know if the struct
declaration is incomplete or not. We will use the current token
held by yyparse, which is stored in yychar. 61 is the t_val for
SM (semi colon) */

NODE *
rstruct( idn, soru )
register idn,soru;
{
	 /* reference to a struct or union, with no definition */
	register struct symtab *p;
	register NODE *q;
	p = &stab[idn];

	/* roger: check to see if 'p' is in the current scope,
	but make sure we are not confusing a type cast or a full
	type declaration with an incomplete declaration */
	if ( yychar==61 && p->slevel != blevel ) goto def;

	switch( p->stype )
	{
	case UNDEF:
def:
		q = block( FREE, NIL, NIL, 0, 0, 0, 0 );
		q->tn.rval = idn;
		q->in.type = (soru&INSTRUCT) ? STRTY : ( (soru&INUNION) ? UNIONTY : ENUMTY );
		defid( q, (soru&INSTRUCT) ? STNAME : ( (soru&INUNION) ? UNAME : ENAME ) );
		break;
	case STRTY:
		if( soru & INSTRUCT ) break;
		goto def;
	case UNIONTY:
		if( soru & INUNION ) break;
		goto def;
	case ENUMTY:
		if( !(soru&(INUNION|INSTRUCT)) ) break;
		goto def;
	}
	stwart = instruct;
	return( mkty( p->stype, 0, p->sizoff, typequal ) );
}

moedef( idn )
register idn;
{
	register NODE *q;

	q = block( FREE, NIL, NIL, MOETY, 0, 0, 0 );
	q->tn.rval = idn;
	if( idn>=0 ) defid( q, MOE );
}

/* bk: control increment of enumeration constants */
/* Since we use the value of strucoff before incrementing it for next time, */
/* we use enum_chk to remember an overflow condition. */
int 
bump_enum()
{
	int curval = strucoff++;

	if ( curval == 0 )
		enum_chk = 0;

	if ( enum_chk ) {
		werror( W_STRICT, "enumeration constant overflow" );
		enum_chk = 0;
	} else {
		if ( curval > 0 && strucoff < 0 )			/* check for overflow */
			enum_chk = 1;
	}

	return( curval );
}


bstruct( idn, soru )
register idn,soru;
{
	 /* begining of structure or union declaration */
	register NODE *q;

	psave( instruct );
	psave( curclass );
	psave( strucoff );
	strucoff = 0;
	instruct = soru;
	q = block( FREE, NIL, NIL, 0, 0, 0, 0 );
	q->tn.rval = idn;
	if( instruct==INSTRUCT )
	{
		curclass = MOS;
		q->in.type = STRTY;
		if( idn >= 0 ) defid( q, STNAME );
	}
	else if( instruct == INUNION ) 
	{
		curclass = MOU;
		q->in.type = UNIONTY;
		if( idn >= 0 ) defid( q, UNAME );
	}
	else 
	{
		 /* enum */
		curclass = MOE;
		q->in.type = ENUMTY;
		if( idn >= 0 ) defid( q, ENAME );
	}
	psave( idn = q->tn.rval );
	/* the "real" definition is where the members are seen */
	if( idn >= 0 ) stab[idn].suse = lineno;
	return( paramno-4 );
}

# ifndef ENUMSIZE
# define ENUMSIZE(h,l) INT
# endif

NODE *
dclstruct( oparam )
register oparam;
{
	register struct symtab *p;
	register i, al, sa, j, sz, szindex;
	register TWORD temp;
	register high, low;
	int memidx;
# ifndef NODBG		/* ken#4 */
	NODE * q;
	int hanval;
#endif

	/* paramstack contains:
	** paramstack[ oparam ] = previous instruct
	** paramstack[ oparam+1 ] = previous class
	** paramstk[ oparam+2 ] = previous strucoff
	** paramstk[ oparam+3 ] = structure name
	** 
	** paramstk[ oparam+4, ... ]  = member stab indices
	** 
	*/


	if( (i=paramstk[oparam+3]) < 0 )
	{
		szindex = curdim;
		dstash( 0 );  /* size */
		dstash( -1 );  /* index to member names */
#ifdef m68k
		if (strucdef)  /* inside structure defn. */
			dstash( SASTRUCT);
			/* Do special m68k structure alignment */
		else
#endif
		dstash( ALSTRUCT );  /* alignment */
		dstash( -lineno );  /* name of structure */
	}
	else 
	{
		szindex = stab[i].sizoff;
	}


# ifndef NODBG
	if( ddebug )
	{
		printf( "dclstruct( %s szindex = %d, qual=%x\n",
			(i>=0)? stab[i].sname : "??", szindex, struc_qual[sulvl] );
	}
	if (i>=0) hanval =i;		/* ken#4 */
	else hanval = -1;		/* ken#4 */
# endif
	temp = (instruct&INSTRUCT)?STRTY:((instruct&INUNION)?UNIONTY:ENUMTY);
	stwart = instruct = paramstk[ oparam ];
	curclass = paramstk[ oparam+1 ];
	if ( chk_struct )
		memidx = dimtab[szindex+1];
	else
		dimtab[ szindex+1 ] = curdim;
#ifdef m68k
	if ((temp==STRTY) || (temp==UNIONTY))	/* m68k structure alignment */
		al = SASTRUCT;
	else
#endif
		al = ALSTRUCT;

	high = low = 0;

	for( i = oparam+4;  i< paramno; ++i )
	{
		j=paramstk[i];
		if ( chk_struct ) {
			if ( j != dimtab[memidx++] ) {
				uerror( "redeclaration of %s", stab[paramstk[oparam+3]].sname );
				chk_struct = -1;
				break;
			}
			continue;
		}
		dstash( j );
		if( j<0 || j>= SYMTSZ ) cerror( "gummy structure member" );
		p = &stab[j];
		if( temp == ENUMTY )
		{
			if( p->offset < low ) low = p->offset;
			if( p->offset > high ) high = p->offset;
			p->sizoff = szindex;
			continue;
		}
#ifdef m68k
		if (strucdef) /* inside a structure */
			sa = salign( p->stype, p->sizoff );
			/* do special m68k structure alignment */
		else
#endif
			sa = talign( p->stype, p->sizoff );

		if( p->sclass & FIELD )
		{
			sz = p->sclass&FLDSIZ;
		}
		else 
		{
			sz = tsize( p->stype, p->dimoff, p->sizoff );
		}
		if( sz == 0 )
		{
			werror( W_DFLT, "structure member has size 0: %s", p->sname );
		}
		if( sz > strucoff ) strucoff = sz;  /* for use with unions */
		SETOFF( al, sa );
		/* al, the alignment, to the LCM of the members' alignments */
	}
	if ( chk_struct ) {
		if ( chk_struct != -1 )	/* if benign, warn anyway (ANSI constraint) */
			if ( !mode_chk( MODE_BENIGN ) )
				werror( W_STRICT, "benign structure template redeclaration" );
		chk_struct = 0;
		paramno = oparam;
		return( mkty( temp, 0, szindex, typequal ) );
	}
	dstash( -1 );  /* endmarker */
	SETOFF( strucoff, al );

	if( temp == ENUMTY )
	{
		register TWORD ty;

		ty = ENUMSIZE(high,low);
		strucoff = tsize( ty, 0, (int)ty );
		dimtab[ szindex+2 ] = al = talign( ty, (int)ty );
	}

	if( strucoff == 0 ) uerror( "zero sized structure" );
	dimtab[ szindex ] = strucoff;
	dimtab[ szindex+2 ] = al;
	dimtab[ szindex+3 ] = paramstk[ oparam+3 ];  /* name index */

#ifdef	FIXSTRUCT
	FIXSTRUCT( szindex, oparam );	/* local hook for debugging */
#endif

# ifndef NODBG
	if( ddebug>1 )
	{
		printf( "\tdimtab[%d,%d,%d,%d] = %d,%d,%d,%d\n",
		szindex,szindex+1,szindex+2,szindex+3,
		dimtab[szindex],dimtab[szindex+1],dimtab[szindex+2],
		dimtab[szindex+3] );
		for( i = dimtab[szindex+1]; dimtab[i] >= 0; ++i )
		{
			printf( "\tmember %s(%d)\n", stab[dimtab[i]].sname, dimtab[i] );
		}
	}
# endif

	strucoff = paramstk[ oparam+2 ];
	paramno = oparam;

#ifdef NEVER
	if ( chk_struct ) {			/* bk */
		if ( !equ_struct( chk_struct, szindex ) )
			uerror( "redeclaration of %s", stab[dimtab[chk_struct+3]].sname );
		chk_struct = 0;
	}
#endif

#ifdef NODBG
	return( mkty( temp, 0, szindex, typequal ) );
#else		/* begin ken#4 */
	q = mkty( temp, 0, szindex, typequal );
	if (printsize) {
	    i = hanval;
	    if(i>=0) {
		hitstruct = 0;
		structflg &= ~(bracketinc<<1);

		bracketinc--;
		fprintf(stderr,
		    "%.*sEND STRUCT/UNION: %s: size[ %#x ]\n\n", 
		    bracketinc+blevel,"                                        ",
			stab[i].sname,
			tsize(stab[i].stype, 
				stab[i].dimoff, stab[i].sizoff)/SZCHAR);
	    }
	} /* if */
	return(q);	/* end ken#4 */
#endif /* NODBG */
}

#ifdef NEVER
equ_struct( st1, st2 )
int st1, st2;
{
	return 0;
}
#endif

/* VARARGS */
yyerror( s ) char *s; 

{
	 /* error printing routine in parser */
	uerror( s );

}

yyaccpt()
{
	register int i;
	ftnend();

	/* roger: check for STATIC functions that have been
	 * declared (prototype) but never defined in the current
	 * file 
	 */
	for( i=0; i<SYMTSZ; i++)
	{
		if ( stab[i].sname != NULL &&
			 stab[i].sclass == USTATIC &&
			 stab[i].scopelink == NULL )
			 uerror("EOF encountered prior to static function %s definition",
					stab[i].sname);
	}
}

#ifdef IN_LINE
/*
/* inlargs[] collects the asm function argument names from the formal
/* parameter list.  The names will be written into the  asm expansion
/* storage file by squirrelmacro().  inlargs[] will also be used
/* to read the formal parameter names back in when an asm call is expanded.
*/
extern char inlargs[];
extern int ninlargs;
#endif

ftnarg( idn ) 
register idn;
{
	register struct symtab *p;
	p = &stab[idn];
	switch( p->stype )
	{

	case UNDEF:
		/* this parameter, entered at scan */
		break;
	case FARG:
		uerror("redeclaration of formal parameter, %s",p->sname);
		/* fall thru */
	case FTN:
		/* the name of this function matches parm */
		/* fall thru */
	default:
		idn = hide(p);
		p = &stab[idn];
		break;
	case TNULL:
		/* unused entry, fill it */
		;
	}
	p->stype = FARG;
	p->sclass = PARAM;
	asave( idn );

#ifdef IN_LINE
/* collect the asm function argument names, delimited by '#',
/* to be written into the expansion storage file by squirrelmacro()
*/
	if (curclass == INLINE)
	{
	    if ( strlen(inlargs) + strlen(p->sname) > BUFSIZ -2 )
		cerror( " out of room for asm argument storage ");
	    strcat( inlargs, p->sname);
	    strcat( inlargs, "#");
	    ninlargs++;
	}
#endif
	return( idn );			/* bk: in case of hide() */
}

#ifdef m68k
	/* Special routine to do alignment inside structures */
salign( ty, s)
register unsigned ty; 
register s; 
{
	/* compute the alignment of an object with type ty, sizeoff index s */

	register i;
# ifdef FUNNYFLDS
	if( s<0 && ty!=INT && ty!=CHAR && ty!=SHORT && ty!=UNSIGNED && ty!=UCHAR && ty!=USHORT 
#ifdef LONGFIELDS
	    && ty!=LONG && ty!=ULONG
#endif
	    )
	{
		return( fldal( ty ) );
	}
# endif

	for( i=0; i<=(SZINT-BTSHIFT-1); i+=TSHIFT )
	{
		switch( (ty>>i)&TMASK )
		{

		case FTN:
			cerror( "compiler takes alignment of function");
		case PTR:
			return( SAPOINT );
		case ARY:
			continue;
		case 0:
			break;
		}
	}

	switch( BTYPE(ty) )
	{

	case UNIONTY:
	case ENUMTY:
	case STRTY:
		{
		register retval = dimtab[s+2];
		if (retval > SASTRUCT)	/* no long align. in strucs */
			return(SASTRUCT);
		else    return(retval);
		}
	case CHAR:
	case UCHAR:
		return( SACHAR );
	case FLOAT:
		return( SAFLOAT );
	case DOUBLE:
		return( SADOUBLE );
	case LONG:
	case ULONG:
		return( SALONG );
	case SHORT:
	case USHORT:
		return( SASHORT );
	default:
		return( SAINT );
	}
}
#endif

talign( ty, s)
register unsigned ty; 
register s; 
{
	/* compute the alignment of an object with type ty, sizeoff index s */

	register i;

# ifdef FUNNYFLDS
	if( s<0 && ty!=INT && ty!=CHAR && ty!=SHORT && ty!=UNSIGNED && ty!=UCHAR && ty!=USHORT 
#ifdef LONGFIELDS
	    && ty!=LONG && ty!=ULONG
#endif
	    )
	{
		return( fldal( ty ) );
	}
# endif

	for( i=0; i<=(SZINT-BTSHIFT-1); i+=TSHIFT )
	{
		switch( (ty>>i)&TMASK )
		{

		case FTN:
			cerror( "compiler takes alignment of function");
		case PTR:
			return( ALPOINT );
		case ARY:
			continue;
		case 0:
			break;
		}
	}

	switch( BTYPE(ty) )
	{

	case UNIONTY:
	case ENUMTY:
	case STRTY:
		return( dimtab[ s+2 ] );
	case CHAR:
	case UCHAR:
		return( ALCHAR );
	case FLOAT:
		return( ALFLOAT );
	case DOUBLE:
		return( ALDOUBLE );
	case LONG:
	case ULONG:
		return( ALLONG );
	case SHORT:
	case USHORT:
		return( ALSHORT );
	default:
		return( ALINT );
	}
}

OFFSZ
tsize( ty, d, s )
register TWORD ty; 
{
	/* compute the size associated with type ty,
	** dimoff d, and sizoff s 
	*/
	/* BETTER NOT BE CALLED WHEN t, d, and s REFER TO A BIT FIELD... */

	register i;
	register OFFSZ mult;

	mult = 1;

	for( i=0; i<=(SZINT-BTSHIFT-1); i+=TSHIFT )
	{
		switch( (ty>>i)&TMASK )
		{

		case FTN:
			cerror( "compiler takes size of function");
		case PTR:
			return( SZPOINT * mult );
		case ARY:
			mult *= dimtab[ d++ ];
			continue;
		case 0:
			break;

		}
	}

	if( dimtab[s]==0 ) 
	{
		if ( s == VOID )					/* bk */
			if ( ISPTR(ty) || ISFTN(ty) )
				return( 0 );
		uerror( "unknown size");
		return( SZINT );
	}
	return( dimtab[ s ] * mult );
}

inforce( n )
register OFFSZ n; 
{
	  /* force inoff to have the value n */
	/* inoff is updated to have the value n */
	register OFFSZ wb;
	register rest;
	/* rest is used to do a lot of conversion to ints... */

	if( inoff == n ) return;
	if( inoff > n ) 
	{
		cerror( "initialization alignment error");
	}

	wb = inoff;
 	SETOFF( wb, SZINT );

	/* wb now has the next higher word boundary */
	if( wb >= n )
	{
		 /* in the same word */
		rest = n - inoff;
		vfdzero( rest );
		return;
	}

	/* otherwise, extend inoff to be word aligned */

	rest = wb - inoff;
	vfdzero( rest );

	/* now, skip full words until near to n */

	rest = (n-inoff)/SZINT;
	zecode( rest );

	/* now, the remainder of the last word */

	rest = n-inoff;
	vfdzero( rest );
	if( inoff != n ) cerror( "inoff error");

}

vfdalign( n )
{
	 /* make inoff have the offset the next alignment of n */
	register OFFSZ m;

	m = inoff;
	SETOFF( m, n );
	inforce( m );
}


int idebug = 0;

int ibseen = 0;  /* the number of } constructions which have been filled */

int iclass;  /* storage class of thing being initialized */

int ihidden;  /* bk: we build hidden statics to handle auto aggregates init */

int ilocctr = 0;  /* location counter for current initialization */
 
int icomplain = 0;	/* bk: complain about incompletely initialized aggregates */
int icomplain2 = 0;     /* roger: complain about inconsistent initialized aggregates */

beginit(curid)
{
	/* beginning of initilization; set location ctr and set type */
	register struct symtab *p;

# ifndef NODBG
	if( idebug >= 3 ) printf( "beginit(), curid = %d\n", curid );
# endif

	p = &stab[curid];

#ifdef m68k
	struct_init = (BTYPE(p->stype)==STRTY); /* structure initialization */
#endif

	iclass = p->sclass;
	if ( iclass == EXTERN && blevel > 0 )	/* bk: ANSI 3.5.7 ConP4S1 */
	{
		uerror( "can't initialize block scope extern" );
		iclass = SNULL;
		return;
	}
	if ( iclass == AUTO && 			/*bk: we'll make hidden static aggregate */
		( ISARY(p->stype) ) && 
		( dimtab[p->dimoff] == 0 ) ) {		/* i.e. is size yet unknown? */
		ihidden = 1;
		iclass = STATIC;
	}

	if( curclass == EXTERN || curclass == FORTRAN ) iclass = EXTERN;
#ifdef IN_LINE
	if (curclass == INLINE) iclass = EXTERN;
#endif
	switch( iclass )
	{

	case UNAME:				/*bk: why is this never the case? */
/*	case EXTERN: */
		return;
	case AUTO:
	case REGISTER:
		break;
	case EXTERN:		/*bk*/
						/* initialized extern equals "external definition"*/
		iclass = p->sclass = EXTDEF;
	case EXTDEF:
	case STATIC:
 		if ( is_const(p->stypeq ) 	/* put const data in text section; */
			|| ihidden ) 			/* hidden static initializers as well */
 			ilocctr = TEXT2;
 		else
 			ilocctr = ISARY(p->stype)?ADATA:DATA;
		locctr( ilocctr );
		if (ISARY(p->stype))
			defalign( ALARRAY );
		 else defalign( talign( p->stype, p->sizoff ) );
		if( ihidden ) deflab( ihidden = getlab() );
		else if( p->slevel>1 && p->sclass == STATIC ) deflab( p->offset );
		else defnam( p );

	}

	inoff = 0;
	ibseen = 0;

	pstk = 0;

	instk( curid, p->stype, p->stypeq, p->dimoff, p->sizoff, inoff );
  	/* roger: if we are dealing with structures or arrays we MUST
         have a '{' at the beginning of the initialization so we do the 
         work here instead of calling ilbrace() */
         if ( ISARY(p->stype) || p->stype == STRTY || p->stype == ENUMTY )
         {
          instack[0].in_fl = 1;
          brack[0] = 1;
         }
}

instk( id, t, tq, d, s, off )
register OFFSZ off; 
register TWORD t, tq; 

{
	/* make a new entry on the parameter stack to initialize id */

	register struct symtab *p;

	for(;;)
	{
# ifndef NODBG
		if( idebug ) printf( "instk((%d, %o,%o,%d,%d, %d)\n", 
									id, t, tq, d, s, off );
# endif

		/* save information on the stack */

		if( !pstk ) pstk = instack;
		else {
			cur_level++;
			if ( ++pstk > &instack[STR_NEST-1] )			/* bk */
				cerror( "reduce structure nesting to <= %d levels", STR_NEST );
		}

		pstk->in_fl = 0;	/* { flag */
		pstk->in_id =  id ;
		pstk->in_t =  t ;
		pstk->in_tq =  tq ;
		pstk->in_d =  d ;
		pstk->in_s =  s ;
		pstk->in_n = 0;  /* number seen */
		if ( t == STRTY || t == UNIONTY ) {		/*bk*/
			pstk->in_x = dimtab[s+1];
		} else {
			pstk->in_x = 0;
		}
		pstk->in_off = off;  /* offset at beginning of this element */
		/* if t is an array, DECREF(t) can't be a field */
		/* INS_sz has size of array elements, and -size for fields */
		if( ISARY(t) )
		{
			pstk->in_sz = tsize( DECREF(t), d+1, s );
			if ( pstk->in_sz <= 0 ) {
				uerror( "cannot initialize incomplete array type" );
				pstk->in_sz = SZINT;
			}
		}
		else if( stab[id].sclass & FIELD )
		{
			pstk->in_sz = - ( stab[id].sclass & FLDSIZ );
		}
		else 
		{
			pstk->in_sz = 0;
		}

		/* should fail in -SYSV */
		if( ( compiler_mode == MODE_CUC ) && 
			(iclass==AUTO || iclass == REGISTER ) &&
			(ISARY(t) || t==STRTY) 
		)
			uerror( "no automatic aggregate initialization" );

		/* now, if this is not a scalar, put on another element */

		if( ISARY(t) )
		{
			t = DECREF(t);
			++d;
			continue;
		}
		else if( t == STRTY )
		{
			id = dimtab[pstk->in_x];
			p = &stab[id];
			if( p->sclass != MOS && !(p->sclass&FIELD) )
				cerror( "insane structure member list" );
			t = p->stype;
			d = p->dimoff;
			s = p->sizoff;
			off += p->offset;
			continue;
		}
		else if ( t == UNIONTY )			/*bk*/
		{
			id = dimtab[pstk->in_x];
			p = &stab[id];
			if( p->sclass != MOU )
				cerror( "insane union member list" );
			t = p->stype;
			d = p->dimoff;
			s = p->sizoff;
			if ( p->offset )
				cerror( "Non-zero union member offset" );
			off += p->offset;
			continue;
		}
		else return;
	}
}

NODE *
getstr()
{
	 /* decide if the string is external or an initializer,
	    and get the contents accordingly */
	register l, temp;
	register NODE *p;

	if( (iclass==EXTDEF||iclass==STATIC) &&
	    (pstk->in_t == CHAR || pstk->in_t == UCHAR) &&
	    pstk!=instack && ISARY( pstk[-1].in_t ) )
	{
		int modsav;
		/* treat "abc" as { 'a', 'b', 'c', 0 } */
		strflg = 1;
		modsav = modes[MODE_TOPDOWN].lxmval;
		modes[MODE_TOPDOWN].lxmval = 0;
		ilbrace();  /* simulate { */
		inforce( pstk->in_off );

		/* if the array is inflexible (not top level), pass in the size
		** Be prepared to throw away unwanted initializers 
		*/

		/* get the contents */
/* bk: even top lvl of array of known size is "inflexible": ANSI 3.5.7 SemP8S2*/
		if ( mode_chk( MODE_STRNULL ) )
			lxstr((pstk-1)!=instack?dimtab[(pstk-1)->in_d]:0);
		else
			lxstr(dimtab[(pstk-1)->in_d]);

		irbrace();  /* simulate } */
		modes[MODE_TOPDOWN].lxmval = modsav;
		return( NIL );
	}
	else 
	{
		int arraystr;		/* array, of known dim, initialized with string */

		 /* make a label, and get the contents and stash them away */
		if( iclass != SNULL )
		{
			 /* initializing */
			/* fill out previous word, to permit pointer */
#ifdef m68k
			if (struct_init)	/* within a structure init. */
				vfdalign( SAPOINT );
			else

#endif
			vfdalign( ALPOINT );
		}
		if ( iclass == AUTO && 					/*bk*/
			(pstk->in_t == CHAR || pstk->in_t == UCHAR) &&
			pstk!=instack && ISARY( pstk[-1].in_t ) ) {
			strflg = 2;
			arraystr = 1;
		} else {
			arraystr = 0;
			strflg = 0;
		}

		/* set up location counter */
		if ( mode_chk( MODE_STRWRITE ) ) {		/* writable string literal? */
			temp = locctr( STRWRT ); 
		} else {
			temp = locctr( blevel==0?ISTRNG:STRNG ); 
		}
		defalign( ALARRAY );
		deflab( l = getlab() );

		lxstr(0); /* get the contents */

		locctr( blevel==0?ilocctr:temp );
		/* put out length for string constant optimizations */
		printf("\tset\tL%%%dL,%d\n", l, dimtab[curdim]);
		if (!struct_init)
			defalign( ALARRAY );
		if ( arraystr )
/*			gotscal();  */
				;
		else
			p = buildtree( STRING, NIL, NIL );
		p->tn.rval = -l;
		if ( arraystr )
			return( NIL );
		else 
			return(p);
	}
}

/* bk */
ary_strbyte( v )
{
	register NODE *p;
	p = bcon(v);
	doinit( p );
	tfree( p );
}

putbyte( v )
{
	 /* simulate byte v appearing in a list of integer values */
	register NODE *p;
	p = bcon(v);
	incode( p, SZCHAR );
	tfree( p );
	gotscal();
}

endinit()
{
	register TWORD t;
	register d, s, n, d1, n1;
	struct instk *temp;
	int do_sdb = 0;

# ifndef NODBG
	if( idebug ) printf( "endinit(), inoff = %d\n", inoff );
# endif

#ifdef m68k
	struct_init = 0; /* initialization done */
#endif
	for (d=0 ; d<STR_NEST; d++ ) brack[d] = 0;
	cur_level = 0;

	switch( iclass )
	{

	case SNULL:			/* bk: results from user error caught in beginit */
	case EXTERN:
		goto reset;
	case AUTO:
	case REGISTER:
		temp = instack;
		t = temp->in_t;
		if( ISARY(t) )
		{
			d = temp->in_d;
			n = temp->in_n;  /* real number of initializers */
			d1 = dimtab[d];	/* should-be number of initializers */

			n1 = instack[1].in_n;		/* should be zero */

			if( d1!=0 && (d1!=n || n1) ) {
				if ( n >= d1 )
					uerror( "too many initializers");
				else {
					werror( W_STRICT, "too few initializers" );
					while( temp->in_n < d1 ) {
						doinit( bcon( 0 ) );		/* padd with zeroes */
					}
				}
			}
			if( n==0 ) werror( W_DFLT, "empty array declaration");
			dimtab[d] = n+(n1 != 0) ;
		} else if ( BTYPE(t) == STRTY )
		{
			while ( pstk > instack ) 
				doinit( bcon(0) );	/* zero-pad */
		}
		goto reset;
	}

	pstk = instack;

	t = pstk->in_t;
	d = pstk->in_d;
	s = pstk->in_s;
	n = pstk->in_n;

	if( ISARY(t) )
	{
		d1 = dimtab[d];

		/* fill out part of the last element, if needed */
		vfdalign( pstk->in_sz );

		n = inoff/pstk->in_sz;  /* real number of initializers */
		if( d1 >= n )
		{
			/* once again, t is an array, so no fields */
			inforce( tsize( t, d, s ) );
			n = d1;
		}
		if( d1!=0 && d1!=n ) uerror( "too many initializers");
		if( n==0 ) werror( W_DFLT, "empty array declaration");
		if ( dimtab[d] == 0 ) {		/* did we just now find out dimension? */
			dimtab[d] = n;
			do_sdb = 1;
		} else
			dimtab[d] = n;
	}

	else if( t == STRTY || t == UNIONTY )
	{
		/* clearly not fields either */
		inforce( tsize( t, d, s ) );
	}
	else if( n > 1 ) uerror( "bad scalar initialization");
	/* this will never be called with a field element... */
	else inforce( tsize(t,d,s) );

	paramno = 0;
	vfdalign( ALINIT );

	if ( ihidden && !noilist )
		hiddengen(do_sdb);
	else if ( do_sdb )
		prdef( &stab[pstk->in_id], 0 );
reset:
	ihidden = 0;
	inoff = 0;
	iclass = SNULL;
	icomplain = 0;
	icomplain2 = 0;
}

/*bk*/
hiddengen(do_sdb)
int do_sdb;
{
	NODE *dest, *src, *cpy; 
	struct symtab *autosym;
	int label;

	label = ihidden; 				/* get label number of hidden static */
	ihidden = 0;					/* must be turned off before prdef call */

#ifndef NODBG
	if ( idebug )
		fprintf( stderr, "endinit, hidden aggregate label %d\n", label );
#endif

	idname = instack[0].in_id;
	autosym = &stab[idname];
	autosym->offset = NOOFFSET;			/* so oalloc will work */
	oalloc( autosym, &autooff, 0 );

	if ( do_sdb )
		prdef( autosym, 0 );			/* now that the offset is correct */
	dest = buildtree( NAME, NIL, NIL );
	src = bdty( NAME, NIL, -label );
	src->fn.type = dest->fn.type;
	src->fn.cdim = dest->fn.cdim;
	src->fn.csiz = dest->fn.csiz;

	cpy = buildtree( HIDASG, dest, src );
	bccode();
	ecomp( cpy );
}

doinit( p )
register NODE *p; 
{

	/* take care of generating a value for the initializer p */
	/* inoff has the current offset (last bit written)
	** in the current word being generated 
	*/

	register sz, d, s;
	register TWORD t;
	TWORD tq;

	/* note: size of an individual initializer assumed to fit into an int */

	if( iclass <= 0 ) goto leave;		/* bk: was "iclass < 0" */
	 
#ifdef NEVER			/*bk*/
	if( iclass == EXTERN || iclass == UNAME )
	{
		uerror( "cannot initialize extern" );	/*bk: forbade union as well*/
		iclass = -1;
		goto leave;
	}
#endif

	if( p == NIL ) return;  /* throw away strings already made into lists */

	if( ibseen )
	{
		uerror( "} expected");
		goto leave;
	}

# ifndef NODBG
	if( idebug > 1 ) printf( "doinit(%o)\n", p );
# endif

	if( !pstk )
	{
		uerror("{ expected");
		goto leave;
	} else {
		if ( (pstk == instack) && 			/*bk */
			((pstk->in_t == STRTY) || 
			(pstk->in_t == UNIONTY) ||
			(ISARY(pstk->in_t))) ) {
			uerror( "too many initializers" );
			goto leave;
		}
	}

       if ( mode_chk( MODE_TOPDOWN ) &&
                pstk > instack &&
                (ISARY(t = pstk[-1].in_t) || t==STRTY || t==UNIONTY) ) {
 /* bk */
                if ( pstk[-1].in_fl == 0  && !icomplain) {
 /* before complaining, allow for completely unbracketed initializations */
                         register struct instk *temp;
                         for ( temp = instack; temp < pstk; temp++ ) {
                                 if ( temp->in_fl != 0 ) {
                   werror( W_DFLT, "Inconsistently-bracketed initialization" );
                                         icomplain = 1;
                                         break;
                                 }
                         }
                 }
       }


	if( !noilist && (iclass == AUTO || iclass == REGISTER) )
	{	/*bk */
		
		struct symtab *sym;

		bccode();
		idname = pstk->in_id;

		if ( ((sym = &stab[idname])->sclass == MOS ) ||
				(sym->sclass == MOU) || (sym->sclass & FIELD) )
		{
			NODE *off, *base;

			if ( ISARY(pstk[-1].in_t) )
			{
				/* Simulate an assignment statement. We calculate the address 
				 * of the lhs using a modified base+offset formula.
				 * The initialization bookkeeping is all relative to 0, so the
				 * problem is to "anchor" our calculation to the stack address
				 * of the automatic variable.  Since pstk->in_off contains
				 * the (positive) 0-relative offset of the current initializer,
				 * we use the stack offset of the symbol at instack[0] to
				 * anchor the formula:
				 *
				 *	lval = [stack offset of instack[0] aggregate] + pstk->in_off
				 *
				 */
				NODE *off, *base;
				int lval;
		
				lval = BITOOR(pstk->in_off);
				off = bcon( lval );

				idname = (pstk-1)->in_id;
				base = buildtree( NAME, NIL, NIL );			/* base */
				base->tn.type = (pstk-1)->in_t;	/* for multidimensional arrays*/
				base->tn.op = VAUTO;
				base->tn.rval = NONAME;
				base->tn.lval = BITOOR(stab[instack[0].in_id].offset);
				base->fn.csiz = 2;

				base = buildtree( LB, base, off );		/* p[n]... *(p+n) */
				p = buildtree( ASSIGN, base, p );

				ecomp( p );
				gotscal();
				
			} else {
				
				/* structure offsets:
				 * stref() will use the offset field of the member's symbol 
				 * table entry in its offset calculation.  We need to factor
				 * in the initialization offset (pstk->in_off) instead.
				 * We do this here by (impolitely) temporarily overriding
				 * the member symbol's offset value with it's initialization
				 * offset.  By this means we attain our standard formula:
				 *	lval = [stack offset of instack[0] aggregate] + pstk->in_off
				 */
				int temp, curmem;

				off = buildtree( NAME, NIL, NIL );
				curmem = pstk->in_id;
				temp = stab[curmem].offset;
				stab[curmem].offset = pstk->in_off;

				idname = (pstk-1)->in_id;
				base = buildtree( NAME, NIL, NIL );			/* base */
				base->in.type = (pstk-1)->in_t;
				base->tn.op = VAUTO;
				base->tn.rval = NONAME;
				base->tn.lval = BITOOR(stab[instack[0].in_id].offset);
/*				base->fn.csiz = 2;*/

				if ( !ISPTR(base->in.type ) )
					base = buildtree( UNARY AND, base, NIL );

				base = buildtree( STREF, base, off );		/* p->n... *(p+n) */
				stab[curmem].offset = temp;
				p = buildtree( ASSIGN, base, p );

				ecomp( p );
				gotscal();
			}

		} else if ( ISARY(pstk[-1].in_t) )
		{
			NODE *off, *base;
			int lval;
	
/*			if ( strflg != 2 )
				return; */
/*			lval = BITOOR(pstk->in_off) / pstk->in_s ;	/ offset */
/*			lval = pstk[-1].in_off + ( pstk[-1].in_n ); */
			lval = BITOOR(pstk->in_off);
			lval = lval / (BITOOR(dimtab[pstk->in_s]));
			if ( ISPTR( DECREF(pstk[-1].in_t) ) )
				lval /= 4;
			off = bcon( lval );

			idname = (pstk-1)->in_id;
			base = buildtree( NAME, NIL, NIL );			/* base */
			base->tn.type = (pstk-1)->in_t;	/* for multidimensional arrays*/

			base = buildtree( LB, base, off );		/* p[n]... *(p+n) */
			p = buildtree( ASSIGN, base, p );

			ecomp( p );
			gotscal();

		} else {
			
			p = buildtree( ASSIGN, buildtree( NAME, NIL, NIL ), p );
#ifdef M68881
			if( is_m68881 )
			{
				optim( p );
				prtdcon( p );
			}
#endif
			ecomp(p);
		}
		return;
	}

	if ( noilist ) 		/* bk: in case of "struct x y = expr;" */
	{	
		pstk = &instack[0];
		idname = pstk->in_id;
		p = buildtree( ASSIGN, buildtree( NAME, NIL,NIL ), p );
		bccode();
		ecomp( p );
		t = pstk->in_t;
		d = pstk->in_d;
		s = pstk->in_s;
		sz = tsize( t, d, s );
		inoff += sz;
		goto leave;
	}

	t = pstk->in_t;  /* type required */
	tq = pstk->in_tq;
	d = pstk->in_d;
	s = pstk->in_s;

	p = buildtree( ASSIGN, block( NAME, NIL,NIL, t, d, s, tq ), p );
	p->in.left->in.op = FREE;
	p->in.left = p->in.right;
	p->in.right = NIL;
	p->in.left = optim( p->in.left );

	if( p->in.left->in.op == UNARY AND )
	{
		p->in.left->in.op = FREE;
		p->in.left = p->in.left->in.left;
	}

	if( p->in.left->in.op == CONV)
	{
	register NODE *l, *q;
	NODE *doptim();

			q = p->in.left;
			l = q->in.left;
			if( ISPTR( l->tn.type ) )
			{
					/* Must force conversion here, optim will not do it */
					l->tn.type = q->tn.type;
					l->fn.csiz = q->fn.csiz;
					l->fn.cdim = q->fn.cdim;
					q->tn.op = FREE;
					if( tydown(l) )
					{
							l = doptim(l);
					}
					p->in.left = l;
			}
	}

	p->in.op = INIT;
	p = optim(p);

	if( pstk->in_sz < 0 )
	{
		  /* bit field */
		sz = -pstk->in_sz;
	}
	else 
	{
		sz = tsize( t, d, s );
	}

 	inforce( pstk->in_off );

# ifndef NOFLOAT
	if( p->in.left->in.op == FCON )
		fincode( p->in.left->fpn.dval, sz );
	else
# endif
		if( p->in.left->in.op != ICON )
		{
			uerror( "illegal initialization" );
			inoff += sz;
		}
		else 
		{
			/* a constant is being used as initializer */
			if( sz < SZINT )
			{
				 /* special case: bit fields, etc. */
			incode( p->in.left, sz );
			}
			else 
			{
# ifdef MYINIT
				MYINIT( optim(p), sz );
# else
				ecode( optim(p) );
				inoff += sz;
# endif
			}
		}

	gotscal();

leave:
	tfree(p);
}

/* Note that we don't push next member onto instk for UNIONTY: bk */
gotscal()
{
	register t, ix;
	register n, id;
	register struct symtab *p;
	register OFFSZ temp;

	for( ; pstk > instack; ) 
	{

		if( pstk->in_fl ) ++ibseen;

		--pstk;
		--cur_level;       /* roger: keep track of nesting brackets */

		t = pstk->in_t;

		if( t == STRTY )
		{
			ix = ++pstk->in_x;
			if( (id=dimtab[ix]) < 0 ) continue;

			/* otherwise, put next element on the stack */

			p = &stab[id];
			instk( id, p->stype, p->stypeq? p->stypeq: pstk->in_tq, 
					p->dimoff, p->sizoff, p->offset+pstk->in_off );
			return;
		}
		else if( ISARY(t) )
		{
			TWORD temp2;

			n = ++pstk->in_n;
			if( n >= dimtab[pstk->in_d] &&  pstk > instack ) continue;

			/* put the new element onto the stack */

			temp = pstk->in_sz;
			temp2 = (TWORD)DECQUAL(pstk->in_tq)?(TWORD)DECQUAL(pstk->in_tq):pstk->in_tq;
			instk( pstk->in_id, (TWORD)DECREF(pstk->in_t), 
							    temp2,pstk->in_d+1, pstk->in_s,
								pstk->in_off+n*temp );
			return;
		}

	}

}

ilbrace()
{
	 /* process an initializer's left brace */
	register t;
	register struct instk *temp;
	register int incr;
	register int temp2;

	temp = pstk;
	temp2 = cur_level;

       if ( mode_chk( MODE_TOPDOWN ) ) {
               pstk = instack;
               cur_level = 0;
               incr = 1;
       } else {
               incr = -1;
       }
       for( ; (incr>0) ? pstk < temp : pstk > instack ; pstk+=incr , cur_level +=incr)
       {
		t = pstk->in_t;
		if( t != STRTY && !ISARY(t) ) continue; /* not an aggregate */
		if( pstk->in_fl )
		{
			if ( incr<0 )			/* i.e. !mode_chk(MODE_TOPDOWN) */
				if( pstk->in_n ) uerror( "illegal {" );
			continue;
		}

                if (incr<0)
                {
                 pstk->in_fl = 1;
                 break;
                }

		/* roger: check for indentation: we set the flag only if */
		/* we have the first element in an array or structure */
		if ( pstk->in_off == (pstk+1)->in_off)
		{
		 pstk->in_fl = 1;
	     	 brack[cur_level] = 1;
		 break;
		}
	}

	/* cannot find one */
	/* ignore such right braces */

	pstk = temp;
	cur_level = temp2;
}

irbrace()
{
	/* called when a '}' is seen */

# ifndef NODBG
	if( idebug ) printf( "irbrace(): paramno = %d on entry\n", paramno );
# endif

	if( ibseen ) 
	{
		--ibseen;
		return;
	}

	for( ; pstk >= instack; --pstk, --cur_level )	/*bk: changed from pstk > instack */
	{
		if( !pstk->in_fl ) continue;

		/* we have one now */

		pstk->in_fl = 0;  /* cancel { */
		gotscal();  /* take it away... */
		return;
	}

	/* these right braces match ignored left braces: throw out */

}

upoff( size, alignment, poff )
register alignment, *poff; 
{
	/* update the offset pointed to by poff; return the
	** offset of a value of size `size', alignment `alignment',
	** given that off is increasing 
	*/

	register off;

	off = *poff;
	SETOFF( off, alignment );
	if( (maxoffset-off) < size )
	{
		if( instruct!=INSTRUCT )cerror("too many local variables");
		else cerror("Structure too large");
	}
	*poff = off+size;
	return( off );
}

oalloc( p, poff, spec_align )
register struct symtab *p; 
register *poff; 
register spec_align;
{
	/* allocate p with offset *poff, and update *poff */
	register al, off, tsz;
	int noff;

#ifdef m68k
	if (spec_align)
		/* Special m68k structure alignment - function arguments
		   need single alignment in case of calls to previously built 
		   object routines; the same is true for alignment inside
		   structures.
	        */
		al = salign( p->stype, p->sizoff );
	else
#endif
	al = talign( p->stype, p->sizoff );

	noff = off = *poff;
	tsz = tsize( p->stype, p->dimoff, p->sizoff );
#ifdef BACKAUTO
	if( p->sclass == AUTO )
	{
		if( (maxoffset-off) < tsz ) cerror("too many local variables");
#ifdef m68k
		/* align large AUTO items for efficient access */
		if ((tsz>=SZINT) && (al<ALINT)) al = ALINT;
#endif
		noff = off + tsz;
		SETOFF( noff, al );
		off = -noff;
	}
	else
#endif
#ifdef BACKARGS
		/* this is rather nonstandard, and may be buggy in some cases */
		/* in particular, won't work with ARGSRET and ARGALLRET */
		if( p->sclass == PARAM )
		{
			if( (maxoffset-off) < tsz ) cerror("too many local variables");
			if( tsz < SZINT ) 
			{
				noff = off + SZINT;
				SETOFF( noff, ALINT );
				off = -noff;
# ifndef RTOLBYTES
				off += tsz;
#endif
			}
			else 
			{
				noff = off + tsz;
				SETOFF( noff, al );
				off = -noff;
			}
		}
		else
#endif
			if( p->sclass == PARAM && ( tsz < SZINT ) )
			{
				off = upoff( SZINT, ALINT, &noff );
# ifndef RTOLBYTES
				off = noff - tsz;
#endif
			}
			else
			{
				off = upoff( tsz, al, &noff );
			}

	if( p->sclass != REGISTER )
	{
		 /* in case we are allocating stack space for register arguments */
		if( p->offset == NOOFFSET ) p->offset = off;
			else if( off != p->offset ) return(1);
	}

	*poff = noff;
	return(0);
}

falloc( p, w, new, pty )
register struct symtab *p; 
register NODE *pty; 
{
	/* allocate a field of width w */
	/* new is 0 if new entry, 1 if redefinition, -1 if alignment */

	register al,sz,type;

	type = (new<0)? pty->in.type : p->stype;

	/* this must be fixed to use the current type in alignments */
	switch( new<0?pty->in.type:p->stype )
	{

	case ENUMTY:
		{
			register s;
			s = new<0 ? pty->fn.csiz : p->sizoff;
			al = dimtab[s+2];
			sz = dimtab[s];
			break;
		}

	case CHAR:
	case UCHAR:
		al = ALCHAR;
		sz = SZCHAR;
		break;

	case SHORT:
	case USHORT:
		al = ALSHORT;
		sz = SZSHORT;
		break;

	case INT:
	case UNSIGNED:
		al = ALINT;
		sz = SZINT;
		break;
#ifdef LONGFIELDS

	case LONG:
	case ULONG:
		al = ALLONG;
		sz = SZLONG;
		break;
#endif

	default:
# ifdef FUNNYFLDS
		if( new < 0 ) 
		{
# endif
			uerror( "illegal field type" );
			al = ALINT;
# ifdef FUNNYFLDS
		}
		else 
		{
			al = fldal( p->stype );
			sz =SZINT;
		}
# endif
	}

	if( w > sz ) 
	{
		uerror( "field too big");
		w = sz;
	}

	if( w == 0 )
	{
		 /* align only */
		SETOFF( strucoff, al );
		if( new >= 0 ) uerror( "zero size field");
		return(0);
	}

	/* hoss#14 */
	/* comment out next statement to allow bit fields to cross
	** word/long boundaries; i.e. NO alignment for bit fields */
	/* repair incode in local.c to observe this */
	/* Lets try again.  Not all cases are handled here!!  Turns
	** out that some code generated uses bfextu %d0{&28:&5}, %d1
	** In this case the bits which cross the word boundry
	** are GONE GONE GONE!!  Alignment saves all ( except space )
	*/

	if( strucoff%al + w > sz ) SETOFF( strucoff, al );

	if( new < 0 ) 
	{
		if( (maxoffset-strucoff) < w )
			cerror("structure too large");
		strucoff += w;  /* we know it will fit */
		return(0);
	}

	/* establish the field */

	if( new == 1 ) 
	{
		 /* previous definition */
		if( p->offset != strucoff || p->sclass != (FIELD|w) ) return(1);
	}
	p->offset = strucoff;
	if( (maxoffset-strucoff) < w ) cerror("structure too large");
	strucoff += w;
	p->stype = type;
	return(0);
}

/* bk: special adaptation of oalloc() to compute value for __argsize() */
/* I'm not sure we need to go to all this trouble; perhaps just calling */
/* indtype would suffice.  Note that ftn_arg is always turned on...  */
/* i.e. _argsize does not pretend to reflect prototype arg sizes, just */
/* default arg sizes. */
argsize( ty, d, s )
register TWORD ty; 
{
	register al, tsz, ityp;
	int noff;
	extern int ftn_arg;

	ftn_arg = 1;						/* take care of float->double */
	ityp = indtype( ty );	
	ftn_arg = 0;
	if ( ityp == DOUBLE && s == FLOAT )
		s = DOUBLE;

	noff = 0;
	al = salign( ityp, s );
	tsz = tsize( ityp, d, s );
	if( tsz < SZINT ) {
		tsz = SZINT;
		al = ALINT;
	}
	upoff( tsz, al, &noff );

	return( noff );
}


# ifndef EXCLASS
# define EXCLASS EXTERN
# endif

nidcl( p )
register NODE *p; 
{
	 /* handle unitialized declarations */
	/* assumed to be not functions */
	register class;
	register commflag;  /* flag for labelled common declarations */
	int prevclass;		/* bk: helps with ANSI 3.7.2 */

	commflag = 0;

	/* compute class */
	if( (class=curclass) == SNULL )
	{
		if( blevel > 1 ) class = AUTO;
		else if( blevel != 0 || instruct ) cerror( "nidcl error" );
		else 
		{
			 /* blevel = 0 */
			if( (class = EXCLASS) == EXTERN )  {
				if ( (prevclass = stab[p->tn.rval].sclass) != EXTDEF &&
						prevclass != STATIC )		/*bk: ANSI 3.7.2*/
					commflag = 1;
				if ( prevclass == STATIC ) {
					uerror( "linkage disagreement" );
					return;
				}
			}
		}
	}

	defid( p, class );

	if( class==EXTDEF || class==STATIC ) 
	{

# ifndef ALLCOMM
		/* simulate initialization by 0 */
		beginit( p->tn.rval );
		endinit();
# else
		commflag = 1;
# endif

	}
	if( commflag ) 
		if ( ! ISFTN( stab[p->tn.rval].stype ) )		/*bk*/
			commdec( p->tn.rval );
}


/* bk */
TWORD
types()
{
	extern char chktype[];
	register TWORD  noun, adj, unsg, typ;
	int i;

	unsg = INT;  /* SIGNED or UNSIGNED */
	noun = UNDEF;  /* INT, CHAR, FLOAT, DOUBLE, or VOID */
	adj = INT;  /* INT, LONG, or SHORT */

	for ( i=0; i < SIGNED; i++ ) {
		if ( chktype[i] ) {
			switch ( typ = i+1 ) {
				case CHAR:
				case INT:
				case FLOAT:
				case DOUBLE:
				case VOID:
					if( noun != UNDEF ) goto bad;
					noun = typ;
					break;
				case SIGNED:
				case UNSIGNED:
					if( unsg != INT ) goto bad;
					unsg = typ;
					break;
				case SHORT:
				case LONG:
					if( adj != INT ) goto bad;
					adj = typ;
					break;
				default:
					cerror( "case in switch, types" );
					break;;
			}
			chktype[i] = 0;
		}
	}

	/* now, construct final type */
	if( noun == UNDEF ) noun = INT;
	else if( ( noun == FLOAT ) || ( noun == DOUBLE ) )		/*bk*/
	{
		if( unsg != INT || adj == SHORT ) goto bad;
		if ( noun == DOUBLE ) {
			if ( adj == SHORT )			/* Ansi outlaws "long float" */
				goto bad;
			return( DOUBLE );	/* sorry, we don't really have "long double" */
		} else {
			if ( adj != INT )			/* Ansi outlaws "long float" */
				goto bad;
			else 
				return( FLOAT );
		}
	}
	else if( noun == CHAR && adj != INT ) goto bad;

	/* now, noun is INT or CHAR */
	if( adj != INT ) noun = adj;
	if( unsg == UNSIGNED ) return( noun + (UNSIGNED-INT) );
	else return( noun );

bad:
	uerror( "illegal type combination" );
	return( INT );
}


/* bk */	/* (prevent "CONST_TYPEDEF const x") */
type_quals( tq, newtq, tqshift )
TWORD tq;					/* current type qualifier mask */
int newtq;					/* yylval of type qualifier just parsed */
int tqshift;
{
	int qval = 0;
	extern NODE *type_def;
	TWORD tdtype;

	if( tqshift )
		qval = newtq <<  ( BTSHIFT + ((tqshift-1) * TSHIFT) );
	else
		qval = newtq;

	if ( !qval )
		cerror( "type_quals" );

					/* typequal associated with type category, ANSI 3.1.2.5 */
					/* see Plum Hall errtests/m35041.c */
# define ISDT(x) (((x)&TMASK) != 0)
	if (( type_def ) && ( ISDT(tdtype = type_def->in.type) ))
	{			/* analogous to INCREF */
		if ( qval == CONST )
			qval = ( CONST << type_category(tdtype) );
		else if ( qval == VOLATILE )
			qval = ( VOLATILE << type_category(tdtype) );
	}

	if ( qval & tq )
			uerror( "illegal type qualifier combination" );
	else
		tq |= qval;

	return( tq );
}

NODE *
tymerge( typ, idp )
register NODE *typ, *idp; 

{
	/* merge type typ with identifier idp  */

	register unsigned t;
	register i;

	if( typ->in.op != TYPE && 
		typ->in.op != ARGTYPE) 		/* bk */
		cerror( "tymerge: arg 1" );
	if(idp == NIL ) return( NIL );

# ifndef NODBG
	if( ddebug > 2 ) eprint(idp);
# endif

	idp->in.type = typ->in.type;
	idp->in.proto = typ->in.proto;		/*bk*/
	idp->in.typeq = typ->in.typeq;		/*roger*/
	idp->fn.cdim = curdim;
	tyreduce( idp );
	idp->fn.csiz = typ->fn.csiz;

	for( t=typ->in.type, i=typ->fn.cdim; t&TMASK; t = DECREF(t) )
	{
		if( ISARY(t) ) dstash( dimtab[i++] );
	}

	/* now idp is a single node: fix up type */

		/* bk: maintain distinction between LONG/INT for now (until doptim) */
/*	idp->in.type = ctype( idp->in.type ); */

	if( (t = BTYPE(idp->in.type)) != STRTY && t != UNIONTY && t != ENUMTY )
	{
		idp->fn.csiz = t;  /* in case ctype has rewritten things */
	}

	return( idp );
}

tyreduce( p )
register NODE *p; 

{

	/* build a type, stash away dimensions, from a declaration parse tree */
	/* the type is built top down, the dimensions bottom up */

	register o, temp;
	register unsigned t;

	o = p->in.op;
	p->in.op = FREE;

	if( o == NAME ) return;

	t = INCREF( p->in.type );
	if( o == UNARY CALL ) t += (FTN-PTR);
	else if( o == LB )
	{
		t += (ARY-PTR);
		temp = p->in.right->tn.lval;
		p->in.right->in.op = FREE;
	}

	p->in.left->in.type = t;
	tyreduce( p->in.left );

	if( o == LB ) dstash( temp );

	p->tn.rval = p->in.left->tn.rval;
	p->in.type = p->in.left->in.type;

}

fixtype( p, class )
register NODE *p; 
register class;
{
	register unsigned t, type;
	register mod1, mod2;
	/* fix up the types, and check for legality */

	if( (type = p->in.type) == UNDEF ) return;
	if( mod2 = (type&TMASK) )
	{
		t = DECREF(type);
		while( mod1=mod2, mod2 = (t&TMASK) )
		{
			if( mod1 == ARY && mod2 == FTN )
			{
				uerror( "array of functions is illegal" );
				type = 0;
			}
			else if( mod1==FTN && ( mod2==ARY || mod2==FTN ) )
			{
				uerror( "function returns illegal type" );
				type = 0;
			}
			t = DECREF(t);
		}
	}

	/* detect function arguments, watching out for structure declarations */
	/* for example, beware of f(x) struct [ int a[10]; } *x; { ... } */
	/* the danger is that "a" will be converted to a pointer */

	if( class==SNULL && blevel==1 && !(instruct&(INSTRUCT|INUNION)) )
		class = PARAM;
	if( class == PARAM || ( class==REGISTER && blevel==1 ) )
	{
		if( type == FLOAT ) { type = DOUBLE; promoted_floats = 1; }
		else if( ISARY(type) )
		{
			++p->fn.cdim;
			type += (PTR-ARY);
		}
		else if( ISFTN(type) )
		{
			werror( W_DFLT, "a function is declared as an argument" );
			type = INCREF(type);
		}
	}

	if( instruct && ISFTN(type) )
	{
		uerror( "function illegal in structure or union" );
		type = INCREF(type);
	}
	p->in.type = type;
}


# ifndef MYCTYPE
TWORD
ctype( t )
register TWORD t; 
{
	register TWORD bt;
	bt = BTYPE(t);

# ifdef NOFLOAT
	if( bt==FLOAT || bt==DOUBLE ) cerror( "ctype:f" );
# endif

# ifdef NOSHORT
# ifdef NOUNSIGNED
	if( bt==SHORT || bt==USHORT ) MODTYPE(t,INT);
# else
	if( bt==SHORT ) MODTYPE(t,INT);
	else if( bt==USHORT) MODTYPE(t,UNSIGNED);
# endif
# endif

# ifdef NOLONG
# ifdef NOUNSIGNED
	if( bt==LONG || bt==ULONG ) MODTYPE(t,INT);
# else
	if( bt==LONG ) MODTYPE(t,INT);
	else if( bt==ULONG) MODTYPE(t,UNSIGNED);
# endif
# endif
	return( t );
}
# endif

uclass( class ) 
register class; 
{
	/* give undefined version of class */
	if( class == SNULL ) return( EXTERN );
	else if( class == STATIC ) return( USTATIC );
	else if( class == FORTRAN ) return( UFORTRAN );
	else return( class );
}

fixclass( class, type, regflag )
register TWORD type; 
register class;
register int *regflag;
{

	/* first, fix null class */

	if( class == SNULL )
	{
		if( instruct&INSTRUCT ) class = MOS;
		else if( instruct&INUNION ) class = MOU;
		else if( blevel == 0 ) class = EXTDEF;
		else if( blevel == 1 ) class = PARAM;
		else class = AUTO;

	}

	/* now, do general checking */

	if( ISFTN( type ) )
	{
		switch( class ) 
		{
		default:
			uerror( "function has illegal storage class" );
		case AUTO:
			class = EXTERN;
		case EXTERN:
		case EXTDEF:
		case FORTRAN:
#ifdef IN_LINE
		case INLINE:
#endif
		case TYPEDEF:
		case STATIC:
		case UFORTRAN:
		case USTATIC:
			;
		}
	}

	if( class&FIELD )
	{
		if( !(instruct & (INSTRUCT|INUNION)) ) 	/*bk: UNION */
			uerror( "illegal use of field" );
		return( class );
	}

	switch( class )
	{

	case MOU:
		if( !(instruct&INUNION) ) uerror( "illegal class" );
		return( class );

	case MOS:
		if( !(instruct&INSTRUCT) ) uerror( "illegal class" );
		return( class );

	case MOE:
		if( instruct & (INSTRUCT|INUNION) ) uerror( "illegal class" );
		return( class );

	case REGISTER:
		if( blevel == 0 ) {
			uerror( "illegal register declaration" );
		} else {
			/* if cisreg returns 1, nextrvar has the reg. number */
			switch ( cisreg( type ) ) {
				case 0:				/* no more regs available */
					*regflag = 1;
					break;
				case -1:			/* type incompatible for reg */
					uerror( "illegal register declaration" );
					break;
				default:
					return( class );
					break;
			}
		}
		if( blevel == 1 ) return( PARAM );
		else return( AUTO );

	case AUTO:
	case LABEL:
	case ULABEL:
		if( blevel < 2 ) uerror( "illegal class" );
		return( class );

	case PARAM:
		if( blevel != 1 ) uerror( "illegal class" );
		return( class );

#ifdef IN_LINE
	case INLINE:
		if (!ISFTN(type))
			uerror("inline declaration must apply to function");
		else
		{
			type = DECREF(type);
			if (ISFTN(type) || ISARY(type))
				uerror("inline function has bad type");
		}
		return (class);
#endif

	case UFORTRAN:
	case FORTRAN:
# ifdef NOFORTRAN
		NOFORTRAN;    /* a condition which can regulate the FORTRAN usage */
# endif
		if( !ISFTN(type) ) uerror( "fortran declaration must apply to function" );
		else 
		{
			type = DECREF(type);
			if( ISFTN(type) || ISARY(type) || ISPTR(type) ) 
			{
				uerror( "fortran function has wrong type" );
			}
		}
	case STNAME:
	case UNAME:
	case ENAME:
	case EXTERN:
	case STATIC:
	case EXTDEF:
	case TYPEDEF:
	case USTATIC:
		return( class );

	default:
		cerror( "illegal class: %d", class );
		/* NOTREACHED */
	}
}

struct symtab *
mknonuniq(idindex)
register *idindex; 
{
	/* locate a symbol table entry for */
	/* an occurrence of a nonunique structure member name */
	/* or field */
	register i;
	register struct symtab * sp;

	sp = & stab[ i= *idindex ]; /* position search at old entry */
	while( sp->stype != TNULL )
	{
		 /* locate unused entry */
		if( ++i >= SYMTSZ )
		{
			/* wrap around symbol table */
			i = 0;
			sp = stab;
		}
		else ++sp;
		if( i == *idindex ) cerror("Symbol table full");
	}
	sp->sflags = SNONUNIQ | SMOS;
	sp->sname = stab[*idindex].sname; /* old entry name */
	*idindex = i;
# ifndef NODBG
	if( ddebug )
	{
		printf( "\tnonunique entry for %s from %d to %d\n",
		sp->sname, *idindex, i );
	}
# endif
	return ( sp );
}

lookup( name, s)
register char *name; 
{
	/* look up name: must agree with s w.r.t. SLAB, STAG, SMOS and SHIDDEN*/

	register i, ii;
	register struct symtab *sp;
	register labels;

	/* compute initial hash index */
# ifndef NODBG
	if( ddebug > 2 )
	{
		printf( "lookup( %s, %d ), stwart=0x%lx struct=0x%lx\n",
			((int)name)?name:" ", s, stwart, instruct );
	}
# endif

	i = ((unsigned long) name) % SYMTSZ;

	sp = &stab[ii=i];
	labels = sp->labels;

	for(;;)
	{
		struct symtab *empty = NULL;

		 /* look for name */
		if( sp->stype == TNULL )
		{
		/* We keep looking if what we are looking for is a label */
		/* and we still are marked as having labels to look at.  */
		/* If this is the first empty slot we keep track of it   */
		/* for use later if in fact this IS an undefined label   */
			if ( ( s & SLAB ) && labels )
			{
				if ( empty == NULL )
					empty = sp;
				goto next;
			}

		/* empty slot */
		/* set SLAB, STAG, SMOS if needed, turn off all others */
enter:
			/* mark a new label from a different hash bucket */
			if ( s & SLAB )
				stab[ ii ].labels++;

			sp->sflags = s;
			sp->sname = name;
			sp->stype = UNDEF;
			sp->sclass = SNULL;
			return( i );
		}
		if( ( sp->sflags & ( SLAB | STAG | SMOS | SHIDDEN ) ) != s )
			goto next;

		if ( sp->sname == name )
			return( i );

		/* 
		** have we looked at a label which hashes to the original spot?
		*/
		if (	( sp->sflags & SLAB )
				&& 
			( ii == ((unsigned long) sp->sname) % SYMTSZ )
		)
			/* if we have looked at the last label AND
			** we have an empty slot, use it
			*/
			if ( ( --labels == 0 ) && empty )
			{
				sp = empty;
				goto enter;
			}
next:
		if( ++i >= SYMTSZ )
		{
			i = 0;
			sp = stab;
		}
		else
			++sp;

		if( i == ii ) cerror( "symbol table full" );
	}
}

#ifndef checkst
/* if not debugging, make checkst a macro */
checkst(lev)
{
	register int s, i, j;
	register struct symtab *p, *q;

	for( i=0, p=stab; i<SYMTSZ; ++i, ++p )
	{
		if( p->stype == TNULL ) continue;
		j = lookup( p->sname, p->sflags&(SMOS|STAG|SLAB) );
		if( j != i )
		{
			q = &stab[j];
			if( q->stype == UNDEF ||
			    q->slevel <= p->slevel )
			{
				cerror( "check error: %s", q->sname );
			}
		}
		else if( p->slevel > lev ) cerror( "%s check at level %d", p->sname, lev );
	}
}
#endif

struct symtab *
relook(p)
register struct symtab *p; 

{
	  /* look up p again, and see where it lies */
	register struct symtab *q;

	/* I'm not sure that this handles towers of several hidden definitions in all cases */
	q = &stab[lookup( p->sname, p->sflags&(SLAB|STAG|SMOS|SHIDDEN) )];
	/* make relook always point to either p or an empty cell */
	if( q->stype == UNDEF )
	{
		q->stype = TNULL;
		return(q);
	}
	while( q != p )
	{
		if( q->stype == TNULL ) break;
		if( ++q >= &stab[SYMTSZ] ) q=stab;
	}
	return(q);
}

clearst( lev )
{
	 /* clear entries of internal scope  from the symbol table */
	extern struct symtab *scopestack[];
	register struct symtab *p, *q, *r;

# ifdef STABS
	/* do this first, so structure members don't get clobbered
	** before they are printed out... 
	*/
	aobeg();
	p = scopestack[blevel];
	while (p)
	{
		aocode(p);
		p = p->scopelink;
	}
	aoend();
# endif

	/* first, find an empty slot to prevent newly hashed entries from
	** being slopped into... 
	*/

	p = scopestack[lev];
	while (p)
	{
# ifndef NODBG
		if (ddebug)
			printf("removing %s = stab[ %d], flags %o level %d\n",
			    p->sname,p-stab,p->sflags,p->slevel);
# endif
                if( p->sclass == ULABEL && lev <= 2 )
                {
                        uerror("%s undefined label",p->sname); /*bk#8*/
                }

		/* if a label, decrement label count in head hash bucket */
		if ( p->sclass == LABEL )
		{
# ifndef NODBG
			if (ddebug)
			{
				int index =
					((unsigned long) p->sname) % SYMTSZ;
				printf(
			"decrement stab[%d].labels %d for label %s:%s\n"
					,index,
					stab[ index ].labels,
					stab[ index ].sname,
					p->sname
				);
			}
# endif
			stab[ ((unsigned long) p->sname) % SYMTSZ ].labels--;
		}

		if( p->sflags & SHIDES )
			unhide(p);
		if( (r=relook(p)) != p )
			movestab( r, p );
		p->stype = TNULL;
		p->stypeq = 0;	/*bk: temporary, someone's setting this by mistake */
		p = p->scopelink;
	}
	scopestack[lev] = (struct symtab *) NULL;
}

movestab( p, q )
register struct symtab *p, *q; 
{
	/* structure assignment: *p = *q; */
	p->stype = q->stype;
	p->sclass = q->sclass;
	p->slevel = q->slevel;
	p->offset = q->offset;
	p->sflags = q->sflags;
	p->dimoff = q->dimoff;
	p->sizoff = q->sizoff;
	p->sproto = q->sproto;
	p->suse = q->suse;
	p->sname = q->sname;
	p->stypeq = q->stypeq;
}

hide( p )
register struct symtab *p; 
{
	register struct symtab *q;
	for( q=p+1; ; ++q )
	{
		if( q >= &stab[SYMTSZ] ) q = stab;
		if( q == p ) cerror( "symbol table full" );
		if( q->stype == TNULL ) break;
	}
	movestab( q, p );
	p->sflags |= SHIDDEN;
	q->sflags = (p->sflags&(SMOS|STAG|SLAB)) | SHIDES;
	werror( W_LINT, "%s hides earlier namesake", p->sname );
# ifndef NODBG
	if( ddebug ) printf( "	%d hidden by %d\n", p-stab, q-stab );
# endif
	return( idname = q-stab );
}

/*bk#8*/
/* Move given label symbol to a new home: calling code sets pflags=SLAB */
mkspace( p )
register struct symtab *p; 
{
	register struct symtab *q;
	for( q=p+1; ; ++q )
	{
		if( q >= &stab[SYMTSZ] ) q = stab;
		if( q == p ) cerror( "symbol table full" );
		if( q->stype == TNULL ) break;
	}
	movestab( q, p );
	werror( W_LINT, "%s hides earlier namesake", p->sname );
# ifndef NODBG
	if( ddebug ) printf( "	%d hidden (mkspace) in %d\n", p-stab, q-stab );
# endif
	return( idname = q-stab );
}

unhide( p )
register struct symtab *p; 
{
	register struct symtab *q;
	register s;

	s = p->sflags & (SMOS|STAG);
	q = p;

	for(;;)
	{

		if( q == stab ) q = &stab[SYMTSZ-1];
		else --q;

		if( q == p ) break;

		if( (q->sflags&(SMOS|STAG)) == s )
		{
			if ( p->sname == q->sname )
			{
				 /* found the name */
				q->sflags &= ~SHIDDEN;
# ifndef NODBG
				if( ddebug ) printf( "unhide uncovered %d from %d\n", q-stab,p-stab);
# endif
				return;
			}
		}

	}
	cerror( "unhide fails" );
}

/* bk */
inscope(chksym,lvl)
struct symtab *chksym;
int lvl;
{
	/* check for presence of a symbol in given block's scope */
	register struct symtab *p;
 	extern struct symtab *scopestack[];

	p = scopestack[lvl];

	while ( p ) {
		if ( p == chksym )
			return 1;
		p = p->scopelink;
	}
	return 0;						/* symbol not in scope */ 
}

/* Return outermost derivation of derived type of given typeword,
 * or zero if the typeword consists of no derived types.
 * This can be used to determine a type's "type category" ANSI 3.1.2.5
 * The return value is the bit position in the encoded TWORD
 * organization corresponding to the type's type-category (this can
 * be used in a bit shift).
 */         
# define MAXDT	13
int 
type_category( ty )
TWORD ty;
{
	int i;

	if ( ISDT( ty ) == 0 )
		return 0;

			/* start from highest possible level of derived type, and
			 * continually dereference (decrease level of derived type)
			 * until we find a derived type specifier.
			 */
	for ( i=(MAXDT-1)*2; i>=0; i-=2 ) {
		if ( (ty >> i) & TMASK )
			break;
	}

	if ( i < 0 )
		cerror( "invalid type in type_category" );

	return i+BTSHIFT;
}

#ifndef NODBG
/*bk: more debugging help */
qprint( tq )
register TWORD tq; 
{
	int i = 0;

	if ( tq ) {
		do {
			if ( tq & CONST )
				printf( "const%d ", i );
			if ( tq & VOLATILE )
				printf( "volatile%d ", i );
			i++;
		} while( tq = DECQUAL(tq) );
	}
}
#endif /* NODBG */

int
is_const(q)
register TWORD q; 
{
	for(; q; q = DECQUAL(q) )
	{
	    if (q == CONST)
			return (1);
	}
	return (0);
}

int
is_ary(t)
register TWORD t; 
{
	for(;; t = DECREF(t) )
	{
	    if (ISARY(t))
		return (1);
	    else if (ISFTN(t) || ISPTR(t))
		continue;
	    else 
		return (0);;
	}
}

int
is_ftn(t)
register TWORD t; 
{
	for(;; t = DECREF(t) )
	{
	    if (ISFTN(t))
		return (1);
	    else if (ISARY(t) || ISPTR(t))
		continue;
	    else 
		return (0);;
	}
}

int
is_ptr(t)
register TWORD t; 
{
	for(;; t = DECREF(t) )
	{
	    if (ISPTR(t))
		return (1);
	    else if (ISFTN(t) || ISARY(t))
		continue;
	    else 
		return (0);;
	}
}
