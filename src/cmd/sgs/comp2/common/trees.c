#ident	"@(#)trees.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.3	89/08/25 11:42:25 barryk
	(bk#5) catch non-scalar conditional expressions

    1.2	89/06/26 15:35:38 hossein
	changes related to "const" type qualifier for POSIX.

    1.1	89/06/08 00:38:39 root
	date and time created 89/06/08 00:38:39 by root

*/
/*      @(#)trees.c     7.2             */
static  char    SCCSID[] = "@(#)        @(#)trees.c     7.2     ";

# include "mfile1.h"
# include "proto.h"
# include <limits.h>		/* bk: for "loseval_chk" overflow checking */
# include "modes.h"			/* bk: for pragma control info */

/*      some special actions, used in finding the type of nodes */
# define NCVT 01
# define PUN 02
# define TYPL 04
# define TYPR 010
# define TYMATCH 040
# define LVAL 0100
# define CVTO 0200
# define CVTL 0400
# define CVTR 01000
# define PTMATCH 02000
# define OTHER 04000
# define NCVTR 010000
# define FCVT 020000

/* node conventions:
**
** NAME:        rval>0 is stab index for external
** rval<0 is -inlabel number
** lval is offset in address units
** (NAME really means "STATIC VARIABLE")
** ICON:        lval has the value
** rval has the STAB index, or - label number,
** if a name whose address is in the constant
** rval = NONAME means no name
** VAUTO:       automatic name: lval has offset in address units
** VPARAM:      parameter: lval has offset in address units
** REG: rval is reg. number
**
*/
extern int maxarg;
extern unsigned short int leftcons;
extern int ftn_call;				/* bk */

TWORD
indtype( t )
register TWORD t;
{
	extern int ftn_arg;		/*bk: for default (unprototype'd) float->double */
							/* promotion of function arguments */

        /* return the type of an intermediate expression of type t */
        switch( t )
        {

        case CHAR:
        case SHORT:
                return( INT );

        case UCHAR:
        case USHORT:
			if ( mode_chk(MODE_UNSIGNED) )	
				return( UNSIGNED );			/* unsigned preserving */
			else
                return( INT );				/* bk: ANSI 3.2.1.1 */

        case FLOAT:
			if ( ftn_arg || !mode_chk(MODE_FLOAT) )
                return( DOUBLE ); 			/* bk: ANSI 3.2.1.5 */

        }
        return( t );
}

int bdebug = 0;
extern ddebug;

# ifndef XI
NODE *
xicolon( l, r1, r2 ) register NODE *l, *r1, *r2;

{
        uerror( "syntax error: colon in subscript" );
        tfree( r2 );
        return( buildtree( LB, l, r1 ) );
}
# endif

NODE *
buildtree( o, l, r )
register NODE *l, *r;
register o;
{
        register NODE *p, *q;
        register actions;
        register opty;
        register struct symtab *sp;
        register NODE *lr, *ll;
        int     i, pchk =0;

# ifndef NODBG
        if( bdebug )
                printf( "buildtree( %s, %d, %d )\n", opst[o], l-node, r-node );
# endif

        /* special case to recognize subscripting explicitly */

# ifdef XI
        if( o == LB && l->tn.type == LONG )
                return( xicolon( l, r, (NODE *) 0 ) );
# endif

        if( o == LB ) {
			if ( not_intval(l) && not_intval(r) ) /* bk: ANSI 3.3.2.1 */
				uerror( "non-integral array subscript" );
			return( buildtree( STAR, buildtree( PLUS, l, r ), NIL ) );
		}

        opty = optype(o);

        /* check for constants */

        if( o == NOT && l->in.op == ICON )
        {
                werror( W_LINT, "constant argument to NOT" );
        }

        else if( o==UNARY MINUS && l->in.op==FCON )
        {
                l->fpn.dval = -l->fpn.dval;
                return(l);
        }
        else if( o==UNARY PLUS && l->in.op==FCON )		/*bk*/
        {
                return(l);
        }

        else if( o==QUEST && l->in.op==ICON && l->tn.rval==NONAME )
        {
                l->in.op = FREE;
                r->in.op = FREE;
				/* bk: for typeq inheritance, see ANSI 3.3.15 */
                if( l->tn.lval )
                {
                        tfree( r->in.right );
						r->in.left->in.typeq |= r->in.right->in.typeq;
                        return( r->in.left );
                }
                else
                {
                        tfree( r->in.left );
						r->in.right->in.typeq |= r->in.left->in.typeq;
                        return( r->in.right );
                }
        }

        else if( (o==ANDAND || o==OROR) && (l->in.op==ICON||r->in.op==ICON) )
		{	
			if (   l->in.op == ICON 	/* left side is ICON */
				&& l->tn.rval == NONAME /* not a symbol */
				&& (   (o==ANDAND && l->tn.lval==0)
										/* && statically false */
					|| (o==OROR && l->tn.lval!=0)
										/* || statically true */
				   )
			   )
			{	/* we know result of this operator */
        		p = block( o, l, r, INT, 0, INT,  0 );
				(void) opact( p );		/* operand constraint checks */
				p->in.op = FREE;
				tfree( r );	/* remove right tree */
				l->tn.lval = (o==ANDAND) ? 0 : 1;	/* force to correct value */
				werror( W_LINT, "constant in conditional context" );
				return ( l );	/* return the constant */
			}
            goto ccwarn;
		}

        else if( opty == BITYPE && l->in.op == ICON && r->in.op == ICON )
        {

                switch( o )
                {

                case ULT:
                case UGT:
                case ULE:
                case UGE:
                case LT:
                case GT:
                case LE:
                case GE:
                case EQ:
                case NE:
                case ANDAND:
                case OROR:
                case CBRANCH:

ccwarn:
					werror( W_LINT, "constant in conditional context" );
                }
        }

        /* we make a real node, and look for shrinking later */

        p = block( o, l, r, INT, 0, INT, 
				o==CAST ? typequal : 0 );			/* bk */

        actions = opact(p);
# ifdef MYOPACT
        actions = MYOPACT(p,actions);
# endif

        if( actions&LVAL )
        {
                 /* check left descendent */
                if( i = notlval(p) )		/*bk*/
                {
					if ( i == 1 )
                        uerror( "illegal lhs of assignment operator" );
					else if ( i == 2 )
                        uerror( "illegal use of const-qualified object" );
                }
        }

        if( actions & NCVTR )
        {
                p->in.left = pconvert( p->in.left, 0 );
        }
        else if( !(actions & NCVT ) )				/* bk */
        {
                switch( opty )
                {

                case BITYPE:
						if ( o != CALL )			/* bk */
	                        p->in.right = pconvert( p->in.right, 
													ftn_call && o==CM );
                case UTYPE:
                        if( !(actions&LVAL) )
                                p->in.left = pconvert( p->in.left,
													ftn_call && o==CM );

                }
        } else if ( actions & FCVT) {				/* bk */
							/* sorry... had to say NCVT */
							/* to prevent default integral widening */
				if (ISFTN( p->in.right->tn.type ))	/* actual arg type is 010x*/
					p->in.right = pconvert( p->in.right, 0 );
				else if ( ISARY( p->in.right->tn.type ) )
					p->in.right = pconvert( p->in.right, 0 );
				if ( ISARY( p->in.left->tn.type ) )
					p->in.left = pconvert( p->in.left, 0 );
		}

        if( (actions&PUN) && (o!=CAST || warn_level >= W_LINT ) )
        {
                chkpun(p);
        }

        if( actions & (TYPL|TYPR) )
        {

                q = (actions&TYPL) ? p->in.left : p->in.right;

                p->in.type = q->in.type;
                p->in.typeq = q->in.typeq;		/* bk */
                p->fn.cdim = q->fn.cdim;
                p->fn.csiz = q->fn.csiz;
        }

		if( actions & CVTL ) p = convert( p, CVTL );
		if( actions & CVTR ) p = convert( p, CVTR );

        if( actions & TYMATCH ) p = tymatch(p);
        if( actions & PTMATCH ) p = ptmatch(p);

        if( actions & SPFLG ) p = clocal(p);

        if( actions & OTHER )
        {
                l = p->in.left;
                r = p->in.right;

                switch(o)
                {

				case LS:			/* check for reasonable shift count */
				case RS:
				case ASG LS:
				case ASG RS:
					if ( r->in.op == ICON ) {
						int max;
						switch ( r->in.type ) {
							case CHAR: case UCHAR: 
								max = SZCHAR; 
								break;
							case SHORT: case USHORT: 
								max = SZSHORT; 
								break;
							case UNSIGNED: case ULONG: 
							case INT: case LONG: 
								max = SZINT; 
								break;
							default:	
								max = 0; 
								break;
						}
						if ( max && r->tn.lval > 32 )
							werror( W_DFLT, "unreasonably large shift count" );
					}
					break;

                case NAME:
                        sp = &stab[idname];
                        if( sp->stype == UNDEF )
                        {
                                uerror( "%s undefined", sp->sname );
                                /* make p look reasonable */
                                p->in.type = p->fn.cdim = p->fn.csiz = INT;
                                p->tn.rval = idname;
                                p->tn.lval = 0;
                                defid( p, SNULL );
                                break;
                        }
                        p->in.type = sp->stype;
						p->in.typeq = sp->stypeq;
						p->in.proto = sp->sproto;		/*bk*/
                        p->fn.cdim = sp->dimoff;
                        p->fn.csiz = sp->sizoff;
                        /* special case: MOETY is really an ICON... */
                        if( p->in.type == MOETY )
                        {
                                p->tn.rval = NONAME;
                                p->tn.lval = sp->offset;
                                p->fn.cdim = 0;
                                p->in.type = INT;
                                p->in.op = ICON;
                        }
                        else
                        {
                                switch( sp->sclass )
                                {

                                case AUTO:
                                        p->in.op = VAUTO;
                                        p->tn.rval = NONAME;
                                        p->tn.lval = BITOOR(sp->offset);
												/* bk: SREG kluge */
										p->tn.goal = sp->sflags;
                                        break;

                                case PARAM:
                                        p->in.op = VPARAM;
                                        p->tn.rval = NONAME;
                                        p->tn.lval = BITOOR(sp->offset);
												/* bk: SREG kluge */
										p->tn.goal = sp->sflags;
                                        break;

                                case REGISTER:
                                        p->in.op = REG;
                                        p->tn.lval = 0;
                                        p->tn.rval = sp->offset;
                                        break;

                                case ULABEL:
                                case LABEL:
                                case STATIC:
                                        if( sp->slevel != 0 )
                                        {
                                                p->tn.lval = 0;
                                                p->tn.rval = -sp->offset;
                                                break;
                                        }
                                        /* FALLTHRU */

                                default:
                                        p->tn.lval = 0;
                                        p->tn.rval = idname;
                                }
                        }
                        break;

                case ICON:
                        p->in.type = INT;
                        p->fn.cdim = 0;
                        p->fn.csiz = INT;
                        break;

                case STRING:
                        p->in.op = NAME;
                        p->in.type = CHAR+ARY;
								/* bk: would that ANSI had made this clear! */
/* #define CONSTSTR (TWORD) 0x8000 */
/*                      	p->in.typeq = CONSTSTR;	/ bk: string "constant" */
                        p->tn.lval = 0;
                        p->tn.rval = NOLAB;
                        p->fn.cdim = curdim;
                        p->fn.csiz = CHAR;
                        break;

                case FCON:
                        p->tn.lval = 0;
                        p->tn.rval = 0;
                        p->in.type = DOUBLE;
                        p->fn.cdim = 0;
                        p->fn.csiz = DOUBLE;
                        break;

                case STREF:
                        /* p->x turned into *(p+offset) */
                        /* rhs must be a name; check correctness */

                        i = r->tn.rval;
                        if(i<0 || i>=SYMTSZ || ((sp= &stab[i])->sclass != MOS &&
                            sp->sclass != MOU && !(sp->sclass&FIELD)) )
                        {
                                uerror( "structure/union member required" );
                                break;
                        }
                        else
                                /* if this name is non-unique, find right one */
                                if( stab[i].sflags&SNONUNIQ && (l->in.type==PTR+STRTY ||
                                    l->in.type == PTR+UNIONTY) &&
                                    (l->fn.csiz+1) >= 0 && dimtab[l->fn.csiz+1] >= 0)
                        {
                                /* nonunique name && structure defined */
                                char * memnam, * tabnam;
                                register j;
                                int memi;
                                j=dimtab[l->fn.csiz+1];
                                for( ; (memi=dimtab[j]) >= 0; ++j )
                                {
                                        tabnam = stab[memi].sname;
                                        memnam = stab[i].sname;
# ifndef NODBG
                                        if( ddebug>1 )
                                        {
                                                printf("member %s==%s?\n",
                                                        memnam, tabnam);
                                        }
# endif
                                        if( stab[memi].sflags & SNONUNIQ )
                                        {
                                                if ( memnam != tabnam )
                                                        continue;
                                                r->tn.rval = i = memi;
                                                break;
                                        }
                                        continue;
                                }
                                if( memi < 0 )
                                        uerror("illegal member use: %s",
                                        stab[i].sname);
                        }
                        else
                        {
                                register j;
                                if( l->in.type != PTR+STRTY &&
                                    l->in.type != PTR+UNIONTY )
                                {
                                        if( stab[i].sflags & SNONUNIQ )
                                        {
                                                uerror(
                                                "nonunique name demands struct/union or struct/union pointer" );
                                        }
                                        else werror( W_DFLT,
                                        "struct/union or struct/union pointer required" );
                                }
                                else if( (j=l->fn.csiz+1)<0 )
                                        cerror( "undefined structure or union");
                                else if( !chkstr( i, dimtab[j],
                                        DECREF(l->in.type) ) )
                                {
                                        uerror( "illegal member use: %s",
                                        stab[i].sname );
                                }
                        }
                        p = stref( p );
                        break;

                case STAR:
                        if( l->in.op == UNARY AND )
                        {
                                p->in.op = l->in.op = FREE;
                                p = l->in.left;
                        }
                        if ( !ISPTR(l->in.type) )
							uerror("illegal indirection");
/*						if ( leftcons && (l->in.typeq & CONSTVAR) ) */
						p->in.typeq = l->in.typeq;	/*bk: inherit const */
                        p->in.type = DECREF(l->in.type);
                        p->fn.cdim = l->fn.cdim;
                        p->fn.csiz = l->fn.csiz;
                        break;

                case UNARY AND:
                        switch( l->in.op )
                        {

                        case STAR:
                                p->in.op = l->in.op = FREE;
                                p = l->in.left;
	/* bk: I'm keeping CONV nodes around longer than before */
	/* so we can do lvalue checks (e.g. & (int *) cp; ). */
	/* Before, makety would just rewrite pointer type conversions */
	/* and drop the CONV node, which prevented us from every doing */
	/* such lvalue checks.  Here we have a case where an lvalue can */
	/* be generated (via STAR) from a CONV (cast) expression, */
	/* and hence the special check. */
								if ( p->in.op == CONV && 
									ISPTR(p->in.type) &&
									ISPTR(p->in.left->in.type) ) {

									TWORD t;
									int d,s;
									t = p->in.left->in.type;
									d = p->in.left->fn.cdim;
									s = p->in.left->fn.csiz;
									p->in.op = FREE;
									p = p->in.left;
									p->in.type = t;	/* see makety */
									p->fn.cdim = d;
									p->fn.csiz = s;
								}
								/* FALLTHROUGH */
                        case VAUTO:
                        case VPARAM:
								if ( l->tn.goal & SREG ) {	/* bk */
                                	uerror( "unacceptable operand of &" );
									break;
								}
                        case NAME:
                        case RNODE:
                        case STCALL:
                        case UNARY STCALL:
						case PROTOTYP:
                                p->in.type = INCREF( l->in.type );
								p->in.typeq = l->in.typeq;		/* bk */
                                p->fn.cdim = l->fn.cdim;
                                p->fn.csiz = l->fn.csiz;
                                break;

                        case COMOP:
                                lr = buildtree( UNARY AND, l->in.right, NIL );
                                p->in.op = l->in.op = FREE;
                                p = buildtree( COMOP, l->in.left, lr );
                                break;

                        case QUEST:
                                lr = buildtree( UNARY AND,
                                l->in.right->in.right, NIL );
                                ll = buildtree( UNARY AND,
                                l->in.right->in.left, NIL );
                                p->in.op = l->in.op = l->in.right->in.op = FREE;
                                p = buildtree( QUEST, l->in.left,
                                buildtree( COLON, ll, lr ) );
                                break;

                        default:
                                uerror( "unacceptable operand of &" );
                                break;
                        }
                        break;

                case RETURN:
                case ASSIGN:
                case CAST:
                case HIDASG:			/*bk*/
                        /* structure assignment */
                        /* take the addresses of the two sides; then make an
                        ** operator using STASG and
                        ** the addresses of left and right
                        */

                        {
                                register TWORD t;
                                register d, s, sz;

                                if( l->fn.csiz != r->fn.csiz )
                                        uerror(
                                        "assignment of different structures" );

                                r = buildtree( UNARY AND, r, NIL );
                                l = buildtree( UNARY AND, l, NIL );
                                t = r->in.type;
                                d = r->fn.cdim;
                                s = r->fn.csiz;
#ifdef ENDSTRUCT
                                sz = tsize( STRTY, d, s );
                                l = aadjust( l, sz );
                                r = aadjust( r, sz );
#endif

                                l = block( STASG, l, r, t, d, s, 0 );

								if( o == HIDASG )			/*bk*/
									l->in.op = HIDASG;

                                if( o == RETURN )
                                {
                                        p->in.op = FREE;
                                        p = l;
                                        break;
                                }

                                p->in.op = STAR;
                                p->in.left = clocal(l);
                                p->in.right = NIL;
                                break;
                        }
                case COLON:
                        /* structure colon */

                        if( l->fn.csiz != r->fn.csiz )
                                uerror( "type clash in conditional" );
                        break;

                case CALL:
                        pchk = 0;
						ProtoChk(p);			/* prototype checks */
						pchk = 1;
						i = 0;
                        p->in.right = r = strargs( p->in.right, &i );
                        if( i > maxarg ) maxarg = i;
                case UNARY CALL:
						if ( pchk == 0 )
							ProtoChk(p);
                        if( !ISPTR(l->in.type)) uerror("illegal function");
                        p->in.type = DECREF(l->in.type);
                        if( !ISFTN(p->in.type)) uerror("illegal function");
                        p->in.type = DECREF( p->in.type );
                        p->fn.cdim = l->fn.cdim;
                        p->fn.csiz = l->fn.csiz;
                        if( l->in.op == UNARY AND &&
                            l->in.left->in.op == NAME &&
                            l->in.left->tn.rval >= 0 &&
                            l->in.left->tn.rval != NONAME &&
                            ((i=stab[l->in.left->tn.rval].sclass)==FORTRAN
                            || i == UFORTRAN ) )
                        {
                                p->in.op += (FORTCALL-CALL);
                        }
#ifdef IN_LINE
                        if( l->in.op == UNARY AND &&
                            l->in.left->in.op == NAME &&
                            l->in.left->tn.rval >= 0 &&
                            l->in.left->tn.rval != NONAME &&
                            stab[l->in.left->tn.rval].sclass == INLINE )
                        {
                                p->in.op += (INCALL-CALL);
                        }
#endif
                        if( p->in.type == STRTY || p->in.type == UNIONTY )
                        {
                                /* function returning structure */
                                /*  function really returns ptr with * */

# ifdef ARGSRET
                                /* set aside argument space for return val */
                                i = tsize( p->in.type, p->fn.cdim, p->fn.csiz );
                                /* alignment??? */
                                if( i > maxarg ) maxarg = i;
# endif
                                p->in.op += STCALL-CALL;
                                p->in.type = INCREF( p->in.type );
                                p = buildtree( STAR, p, NIL );
                                break;
                        }

                        /* fix up type of return value from call */
                        {
                                register TWORD t;
                                t = indtype( p->tn.type );
                                if( t != p->tn.type )
                                {
                                        p->fn.csiz = p->tn.type = t;
                                }
                        }
# ifdef ARGALLRET
                        /* set aside argument space for return val */
                        i = tsize( p->in.type, p->fn.cdim, p->fn.csiz );
                        /* alignment??? */
                        if( i > maxarg ) maxarg = i;
# endif
                        break;

                default:
                        cerror( "other code %d", o );
                }

        }

        if( actions & CVTO ) p = oconvert(p);
        p = clocal(conval(p));

# ifndef NODBG
        if( bdebug ) eprint(p);
# endif
        return(p);

}

NODE *
strargs( p, off )
register NODE *p;
register *off;
{
         /* rewrite arguments */
        /* allocate the arguments at *off, and update *off */
        register TWORD t;

        t = p->fn.type;
        if( p->in.op == CM )
        {
                p->in.left = strargs( p->in.left, off );
                p->in.right = strargs( p->in.right, off );
                return( p );
        }

        p = block( STARG, p, NIL, t, p->fn.cdim, p->fn.csiz, 0 );
        if( t == STRTY || t == UNIONTY )
        {
                p->in.left = buildtree( UNARY AND, p->in.left, NIL );
        }
        else
        {
                p->in.op = FUNARG;
        }
        SETOFF( *off, talign( t, p->fn.csiz ) );
        p->tn.rval = *off;
        *off += tsize( t, p->fn.cdim, p->fn.csiz );
        return( clocal(p) );
}

chkstr( i, j, type )
register TWORD type;
register i,j;
{
        /* is the MOS or MOU at stab[i] OK for strict reference by a ptr */
        /* i has been checked to contain a MOS or MOU */
        /* j is the index in dimtab of the members... */
        register k, kk;

        extern int ddebug;

# ifndef NODBG
        if( ddebug > 1 ) printf( "chkstr( %s(%d), %d )\n", stab[i].sname, i, j );
# endif
        if( (k = j) < 0 ) uerror( "undefined structure or union" );
        else
        {
                for( ; (kk = dimtab[k] ) >= 0; ++k )
                {
                        if( kk >= SYMTSZ )
                        {
                                cerror( "gummy structure" );
                                return(1);
                        }
                        if( kk == i ) return( 1 );
                }
        }
        return( 0 );
}

NODE *
conval( p )
register NODE *p;
{
        register NODE *l, *r;
        register o, i, f, u;
        register CONSZ val;
		int sl, sr, sres;
		extern int seen_oror, seen_andand;			/* bk */

#define NNCON(p) (p->tn.op == ICON && p->tn.rval == NONAME && \
					!ISPTR(p->tn.type))

        switch( optype(o = p->tn.op) )
        {
        case BITYPE:
                l = p->in.left;
                r = p->in.right;
                break;

        case UTYPE:
                r = l = p->in.left;
                break;

        case LTYPE:
                return( p );
        }

        f = (p->tn.type==FLOAT || p->tn.type == DOUBLE);
        u = ISUNSIGNED(p->tn.type);

		/* Motorola 4.1 */
		/* binary logical op with one or both operands an FCON */
		if (	!f
							/* operation is not floating-point */
			&& (logop(o))
							/* this is logical operator */
			&& (optype(o)) == BITYPE
							/* of binary type */
			&& (   (l->tn.op == FCON && r->tn.op == FCON)
							/* both sides are FCONs */
				|| (l->tn.op == FCON && NNCON(r))
				|| (NNCON(l) && r->tn.op == FCON)
							/* FCON mixed with noname ICON */
			   )
		   )
		{			/* XOPEN C extension ... */
			/* Situation: we have a binary logical operator 
			** (other than && or ||) with (a) two floating-point constant
			** FCONS, or (b) we have the && or || operator with one
			** or both FCON operands.  The latter (b) should not occur.
			*/
			int	newleft, newright;		/* new left/right integer values */

			if (o == ANDAND || o == OROR)
			{	/* float operands must be converted to integer */
				newleft = (l->tn.op == FCON) ? l->fpn.dval : l->tn.lval;
				newright = (r->tn.op == FCON) ? r->fpn.dval : r->tn.lval;
			} 
			else 
			{ /* sneak FCONs into ICONs giving equivalent result */
				newleft = newright = 0;	/* default to both equal */
				if (l->fpn.dval > r->fpn.dval)
			  		newleft = 1;		/* left greater than right */
				else
				if (l->fpn.dval < r->fpn.dval)
			  		newright = 1;		/* left less than right */
			}
			l->tn.lval = newleft;	/* set left ICON value */
			r->tn.lval = newright;	/* set right ICON value */
			l->in.op = ICON;		/* force any FCONs into ICONs */
			r->in.op = ICON;
			l->tn.type = p->tn.type;	/* insure same type as parent op */
			r->tn.type = p->tn.type;
			l->tn.rval = NONAME;	/* ICONs without a name */
			r->tn.rval = NONAME;
		}

				/* operands must be constants */
        if( l->tn.op != ( f ? FCON : ICON ) ) return( p );
        if( r->tn.op != ( f ? FCON : ICON ) ) return( p );

        if( !f )
        {
                /* weed out unprofitable cases */
                val = r->tn.lval;
#ifdef NEVER
				/* bk: sorry, this code  							*/
				/* 1) won't be used, as far as I can see, since 	*/
				/*    the rval's must be NONAME (n'est-ce pas?) 	*/
				/* 2) even if the code was useful, it could eliminate */
				/*    volatile stuff. 								*/
				/* Motorola 4.1 */
                if( l->tn.rval != NONAME && r->tn.rval != NONAME )
				    {	/* XOPEN extension ... */
					if (   o == MINUS
								/* subtract operator */
						&& l->tn.rval==r->tn.rval
								/* both operands same symbol */
						&& l->tn.lval==r->tn.lval
								/* both operands same offset */
					   )
					  {	/* named ICONs identical; result zero */
						r->tn.op = FREE;	/* free right side */
						l->tn.lval = 0;	/* set left ICON value */
						l->tn.type = p->tn.type; /* insure same type */
						l->tn.rval = NONAME; /* ICON with no name */
						p->tn.op = FREE;	/* free minus node */
						return ( l );	/* return ICON as the result */
						}
					return(p);	/* cannot handle two named ICONs */
					}
#endif /* NEVER */
                if( l->tn.rval != NONAME && r->tn.rval != NONAME ) return(p);
                if( r->tn.rval != NONAME && o!=PLUS ) return(p);
                if( l->tn.rval != NONAME && o!=PLUS && o!=MINUS ) return(p);
        }
        else if( logop(o) ) return( p );

			/* bk: should add ARITHFLG to dope table */
#define binarith_op(o) (o==MUL || o==DIV || o==MOD || o==PLUS /*|| o==MINUS */ )
		if ( !u && binarith_op(o) ) {
			sl =  ( f ? l->fpn.dval < 0.0 : l->tn.lval < 0 );
			sr =  ( f ? r->fpn.dval < 0.0 : r->tn.lval < 0 );
		}

        switch( o )
        {

		case ANDAND:				/*bk: Ansi 3.4 */
                l->tn.lval = (l->tn.lval && r->tn.lval) ? 1 : 0;
				break;		/* this ICON will replace && tree */

		case OROR:				/*bk: Ansi 3.4 */
                l->tn.lval = (l->tn.lval || r->tn.lval) ? 1 : 0;
				break;		/* this ICON will replace || tree */

        case PLUS:
                if( f )
                {
                        l->fpn.dval += r->fpn.dval;
                        break;
                }
                l->tn.lval += val;
                if( l->tn.rval == NONAME )
                {
                        l->tn.rval = r->tn.rval;
                        l->in.type = r->in.type;
                }
                break;

        case MINUS:
                if( f ) l->fpn.dval -= r->fpn.dval;
                else l->tn.lval -= val;
                break;

        case MUL:
                if( f ) l->fpn.dval *= r->fpn.dval;
                else l->tn.lval *= val;
                break;

        case DIV:
                if( f )
                {
	                if( r->fpn.dval == 0.0 ) {
						/* bk: could be dead child of &&, || op */
						werror( W_DFLT, "division by 0" );
						return( p );			/* let it pass */
					} else l->fpn.dval /= r->fpn.dval;
                }
                else
                {
					if( val == 0 ) {  /* bk: could be dead child of &&, || op */
						werror( W_DFLT, "division by 0" );
						return( p );			/* let it pass */
					} else  {
						if( u )
                                l->tn.lval = (unsigned long)l->tn.lval / val;
                        else l->tn.lval /= val;
					}
                }
                break;

        case MOD:
                if( val == 0 ) {	/* bk: could be dead child of &&, || op */
					werror( W_DFLT, "division by 0" );
					return( p );			/* let it pass */
				}
                else if( u ) l->tn.lval = (unsigned long)l->tn.lval % val;
                else l->tn.lval %= val;
                break;

        case AND:
                l->tn.lval &= val;
                break;

        case OR:
                l->tn.lval |= val;
                break;

        case ER:
                l->tn.lval ^=  val;
                break;

        case LS:
                i = val;
                l->tn.lval = l->tn.lval << i;
                break;

        case RS:
                i = val;
                if( u ) l->tn.lval = (unsigned long)l->tn.lval >> i;
                else l->tn.lval = l->tn.lval >> i;
                break;

        case UNARY MINUS:
                if( f ) l->fpn.dval = -l->fpn.dval;
                else { 
					l->tn.lval = - l->tn.lval;
					if ( ISUNSIGNED(l->tn.type) ) {		/* bk */
                		p->in.type = l->tn.type = DEUNSIGN(l->tn.type);
						werror( W_LINT, "unary minus produces signed result" );
						if ( (unsigned) val > INT_MAX+1 )
							werror( W_STRICT, "integer constant overflow" );
					}
				}
                break;

        case COMPL:
                l->tn.lval = ~l->tn.lval;
                break;

        case NOT:
                if( l->tn.type == FLOAT || l->tn.type == DOUBLE )
                {
                        l->tn.op = ICON;
                        l->tn.lval = !l->fpn.dval;
                }
                else l->tn.lval = !l->tn.lval;
                break;

        case LT:
                l->tn.lval = l->tn.lval < val;
                break;

        case LE:
                l->tn.lval = l->tn.lval <= val;
                break;

        case GT:
                l->tn.lval = l->tn.lval > val;
                break;

        case GE:
                l->tn.lval = l->tn.lval >= val;
                break;

			/* bk: changed comparisons involving unsigned  */
        case ULT:
/*                l->tn.lval = (l->tn.lval-val)<0; */
                l->tn.lval = (unsigned long) l->tn.lval < (unsigned long) val ;
                break;

        case ULE:
                l->tn.lval = (unsigned long) l->tn.lval <= (unsigned long) val;
                break;

        case UGE:
/*                l->tn.lval = (l->tn.lval-val)>=0; */		
                l->tn.lval = (unsigned long) l->tn.lval >= (unsigned long) val;
                break;

        case UGT:
/*                l->tn.lval = (l->tn.lval-val)>0; */
                l->tn.lval = (unsigned long) l->tn.lval > (unsigned long) val;
                break;

        case EQ:
                l->tn.lval = l->tn.lval == val;
                break;

        case NE:
                l->tn.lval = l->tn.lval != val;
                break;

        default:
                return(p);
        }

		if ( !u && binarith_op(o) ) {
			sres =  ( f ? l->fpn.dval < 0.0 : l->tn.lval < 0 );
			if ( (sl ^ sr) ^ sres  )
				werror( W_STRICT, "%s constant overflow, op %s", 
						f ? "floating" : "integer", opst[p->tn.op] );
		}

        if( l != r ) r->tn.op = FREE; /* don't clobber unary answer */
        l = makety( l, p->tn.type, p->fn.cdim, p->fn.csiz );

        p->tn.op = FREE;
        return( l );
}


compatible_types(t1, t2)
TWORD t1, t2;
{
	int compatible;
	int uflag = 0;
	TWORD bt1 = BTYPE(t1);
	TWORD bt2 = BTYPE(t2);

	if ( ISUNSIGNED(bt1) ) {	/* don't complain about unsigned vs. signed */
		bt1 = DEUNSIGN(bt1);
		MODTYPE(t1,bt1);
	}
	if ( ISUNSIGNED(bt2) ) {
		bt2 = DEUNSIGN(bt2);
		MODTYPE(t2,bt2);
	}

	if ( bt1 == LONG )				/* map LONG onto INT */ 
		MODTYPE(t1,INT);
	if ( bt2 == LONG )
		MODTYPE(t2,INT);

	compatible = (t1 == t2);

	if ( compatible ) {				/* warning for long vs. int */
		if( bt1 != bt2 )
			werror( W_STRICT, 
				"non-portable type combination");
	}
	return(compatible);
}

    
chkpun(p)
register NODE *p;
{
        /* checks p for the existance of a pun */
        /* this is called when the op of p is ASSIGN, RETURN, CAST, COLON,
        ** or relational (or PROTOCHK)
        */

        /* one case is when enumerations are used: this applies only to lint */
        /* in the other case, one operand is a pointer, the other integer */
        /* we check that this integer is in fact a constant zero... */
        /* if it is, we redo the type of the 0 to match the other side */

        /* with ASSIGN, assignment of pointer to integer is illegal */
        /* this falls out, because the LHS is never 0 */

        register NODE *q1, *q2;
        register t1, t2;
        register d1, d2;
		extern int proto_argno;

        q1 = p->in.left;
        q2 = p->in.right;
        t1 = q1->in.type;
        t2 = q2->in.type;

        if( t1==ENUMTY || t2==ENUMTY )
        {
			 /* check for enumerations */
			 /* bk: warn vs: */
			 /*		combo of enum constants from different enum types */
			 /* 	combo of enum constant and int, unless int is lhs of asgop*/

			if( t1==ENUMTY && t2==ENUMTY && q1->fn.csiz==q2->fn.csiz )
				return;
			else {
					 /* in switch statements, lhs is a temp INT */
					/* hoss#6 */
				if ( t1==INT && asgop( p->in.op ) )
					return; /* assignment of ENUMTY to INT */
				else 
	/* note: in "enum COLOR var = BLACK;", BLACK is changed to ICON, */
	/* but csiz retains vital link to ENUM type */
					if ( (t1==INT || t2==INT) && q1->fn.csiz==q2->fn.csiz )
						return;
			}
			if ( p->in.op == PROTOCHK ) {
				werror( W_DFLT, 
					"enumeration type clash, prototype arg %d", proto_argno );
			} else {
				werror( W_DFLT,
					"enumeration type clash, operator %s", opst[p->in.op]);
			}
			return;
        }

        if( ISPTR(t1) || ISARY(t1) )
        {       /* switch roles: q1 should be non-pointer */
                q1 = q2;
                q2 = p->in.left;
        }

        if( !ISPTR(q1->in.type) && !ISARY(q1->in.type) )
        {
                if( q1->in.op == ICON && q1->tn.lval == 0 && q1->tn.rval == NONAME )
                {       /* make the 0 have the type of the other side */
                        q1->fn.type = q2->fn.type;
                        q1->fn.cdim = q2->fn.cdim;
                        q1->fn.csiz = q2->fn.type;
                }
                else
                {
                        combo( "pointer/integer", p );
                }
                return;
        }
        else
        {       /* q1 and q2 still mean left and right */
                d1 = q1->fn.cdim;
                d2 = q2->fn.cdim;
                for( ;; )
                {

						if( compatible_types(t1,t2) )
						{
							switch ( BTYPE(t1) ) {
							case STRTY:
							case UNIONTY:
							case ENUMTY:
								if(q1->fn.csiz!=q2->fn.csiz)
								{
									combo( "structure pointer", p );
								}
								break;
							}
							return;
						}
						if( ISPTR(t1) && ISPTR(t2) )	/*bk*/
						{
							if ( (BTYPE(t2) == VOID) ||
								(BTYPE(t1) == VOID) )
								return;
						}
                        else if( ISARY(t1) || ISPTR(t1) )
                        {
                                if( !ISARY(t2) && !ISPTR(t2) ) break;
                                if( ISARY(t1) && ISARY(t2) &&
                                    dimtab[d1] != dimtab[d2] )
                                {
                                        combo( "array size", p );
                                        return;
                                }
                                if( ISARY(t1) ) ++d1;
                                if( ISARY(t2) ) ++d2;
                        }
                        else break;
                        t1 = DECREF(t1);
                        t2 = DECREF(t2);
                }
                combo( "pointer", p );
        }
}

combo( s, p )
char *s;
NODE *p;
{
        char buf[100];
		extern int proto_argno;

		if ( p->tn.op == PROTOCHK ) {
	        sprintf( buf, "%s clash, prototype arg %d", s, proto_argno );
		} else {
			switch ( p->in.left->in.op )		/* special cases */
			{
				case SNODE:
		        	strcpy( buf, "non-scalar switch condition" );
					break;
				case RNODE:
		        	strcpy( buf, "incompatible return type" );
					break;
				default:
		        	sprintf( buf, "illegal %s combination, op %s", s, 
											opst[p->tn.op] );
					break;
			}
		}
        werror( W_DFLT, buf );
}

/* bk: issue warning if assignment may result in loss of value due to */
/* conversion down-sizeing. If rhs is a constant, we can eliminate */
/* many cases as innocuous. */
void
loseval_chk( p )
NODE *p;
{
        char buf[100];
		extern int proto_argno;
		NODE *r, *l;
		int max;
		int sizel,sizer;

		l = p->in.left;			
		r = p->in.right;			
		sizel = bigsize(l->tn.type);
		sizer = bigsize(r->tn.type);

		if ( r->tn.op == FLD ||
			sizer < sizel ||
			((sizer == sizel) && !ISUNSIGNED(r->tn.type)) )
			return;

		if ( r->tn.op == ICON ) {
			switch ( l->in.type ) {
				case CHAR:
					max = CHAR_MAX;
					break;
				case SHORT:
					max = SHRT_MAX;
					break;
				case INT:
					max = INT_MAX;
					break;
				default:
					return;
			}
			if ( (unsigned) r->tn.lval <= max )	/* within bounds? */
				return;							/* yes: no harm done */
		} else {
				 /* note that case float handled by fpe_catch, debug.c*/
			if ( r->tn.op == FCON )
				return;
		}

		if ( p->tn.op == PROTOCHK ) {
	        sprintf( buf, "possible loss of value, arg %d", proto_argno );
		} else {
	        sprintf( buf, "possible loss of value, op %s", opst[p->tn.op] );
		}
        werror( W_LINT, buf );
}

/* Check if ICON node will result in lost value when converted to signed int */
int
icon_overflow(p, dfltmsg )
NODE *p;
int dfltmsg;
{
	int ret;

	ret = p->tn.op == ICON && 
		  ISUNSIGNED(p->tn.type) &&
		  (unsigned) p->tn.lval > INT_MAX ;

	if ( ret && dfltmsg )
		 werror( W_STRICT, "integer constant overflow" );

	return ret;
}



NODE *
stref( p )
register NODE *p;
{
        register TWORD t;
        register d, s, dsc, align;
        register OFFSZ off;
        register struct symtab *q;
		TWORD tqual;

        /* make p->x */
        /* this is also used to reference automatic variables */

        q = &stab[p->in.right->tn.rval];
        p->in.right->in.op = FREE;
        p->in.op = FREE;
        p = pconvert( p->in.left, 0 );

		tqual = p->in.typeq;					/* bk */

        /* make p look like ptr to x */

        if( !ISPTR(p->in.type))
        {
                p->in.type = PTR+UNIONTY;
        }

        t = INCREF( q->stype );
        d = q->dimoff;
        s = q->sizoff;

        p = makety( p, t, d, s );

        /* compute the offset to be added */

        off = q->offset;
        dsc = q->sclass;

        if( dsc & FIELD )
        {
                  /* normalize offset */
                switch(q->stype)
                {

                case CHAR:
                case UCHAR:
                        align = ALCHAR;
                        s = CHAR;
                        break;

                case SHORT:
                case USHORT:
                        align = ALSHORT;
                        s = SHORT;
                        break;

                case ENUMTY:
                case INT:
                case UNSIGNED:
                        align = ALINT;
                        s = INT;
                        break;

# ifdef LONGFIELDS
                case LONG:
                case ULONG:
                        align = ALLONG;
                        s = LONG;
                        break;
# endif

                default:
                        cerror( "undefined bit field type" );
                }
                off = (off/align)*align;
        }
        if( off != 0 )
                p = clocal( block( PLUS, p, offcon( off, t, d, s ), t, d, s, 0 ) );

/* bk: structure member qualifiers = member_qualifiers + struct var qualifiers.
 * Although ANSI 3.5.3 and 3.5.4.1 don't give an explicit example one way or 
 * the other, we find it logical (given the rule in 3.5.4.1 regarding 
 * "const int_ptr constant_ptr") to make a structure qualifier qualify 
 * the member's "type category".  So "const struct {int *cpi;} s;" contains
 * a constant pointer to modifiable integer.
 */
		switch ( tqual ) {
			case CONST:
			case VOLATILE:
			case CONST|VOLATILE:
				tqual = ( tqual << type_category(q->stype) );
				break;
			case 0:
				break;
			default:
				cerror( "stref: invalid structure qualifier" );
				break;
		}
		p->in.typeq = tqual | q->stypeq;

        p = buildtree( STAR, p, NIL );

        /* if field, build field info */

        if( dsc & FIELD )
        {
                p = block( FLD, p, NIL, q->stype, 0, q->sizoff, 0 );
                p->tn.rval = PKFIELD( dsc&FLDSIZ, q->offset%align );
        }

        return( clocal(p) );
}

notlval(p)
register NODE *p;
{
		/*bk*/
        /* return 0 if p an lvalue, 2 if const-qualified,
		 * 3 if we put message out here, 1 otherwise */
	NODE *l;

	l = p->in.left;

again:
        switch( l->in.op )
        {

        case CONV:				/* bk: ANSI 3.3.4 */
			uerror( "cast expression is not an lvalue" );
			return 3;
					
        case FLD:
                l = l->in.left;
                goto again;

        case STAR:
				/* fix the &(a=b) bug, given that a and b are structures */
/*                if( l->in.left->in.op == STASG ) return( 1 );	**bk **/
        case NAME:
        case VAUTO:
        case VPARAM:
        case RNODE:
        case QNODE:
        case SNODE:
			if( ISARY(l->in.type) || 
				ISFTN(l->in.type) ) return(1);

						/* check for structure expressions */
			if ( str_expr(l) ) return(3);	

						/* tests for "const" */
						/* 1: test if type category is const */
			if ( !indcl ) {

				if ( CONST_QUALIFIED( l->in.type, l->in.typeq ) )
						return(2);
			}
			 
						/* 2: test if pointed-to type is const */
						/* (note that casts, as always, are an exception) */
			if ( ISPTR( l->tn.type ) && (p->in.op != CAST) )
			{
				NODE *r;
				TWORD tqleft, tqright;
	/*
     * ANSI 3.3.16.1:  type pointed to by the left must have
	 * all the qualifiers of the type pointed to by the right.
	 */

					r = p->in.right;
					tqleft = l->in.typeq;
					tqright = r->in.typeq;

					if ( (tqleft & tqright) != tqright )
						return(2);

			}
        case REG:
                return(0);

        default:
                return(1);

        }

}

/* bk: return 1 if type of node is not an integral value */
int 
not_intval( p )
register NODE *p;
{
	int bad;

	bad = ISARY( p->in.type );

	if ( bad ) 
		str_expr( p );

	if ( !bad ) {
		switch (BTYPE(p->in.type) )
		{
			case DOUBLE:
			case FLOAT:
			case VOID:
			case STRTY:
			case UNIONTY:
				bad = 1;
				break;
			default:
				break;
		}
	}

	return( bad );
}

/* bk: ANSI 3.3.2.3: catch non-lvalue structure expressions */
int
str_expr( p )
NODE *p;
{
	NODE *l, *r;
	int ret = 0;
	int op;

	for ( l = p; optype( l->in.op ) != LTYPE && !ret; ) {

		switch ( op = l->in.left->in.op ) {

			case QUEST:
			case COMOP:
				r = l->in.left->in.right;
				if ( BTYPE( r->in.type ) == STRTY ) {/* stref: dot bad, -> ok */
					if ( callop(r->in.op) ) {
						ret = ( r->tn.op == STCALL ||
								r->tn.op == UNARY STCALL );
					} else {
						if ( op == QUEST )
							r = r->in.right;
						ret = ( r->tn.op == UNARY AND );
					}
					if ( ret )
						uerror( "non-lvalue structure expression" );
				}
				break;

			case STASG:
			case STCALL:
			case UNARY STCALL:
				uerror( "non-lvalue structure expression" );
				ret = 1;
				break;
			default:
				break;
		}
		l = l->in.left;
	}
	return( ret );
}


NODE *
bcon( i )
{
         /* make a constant node with value i */
        register NODE *p;

        p = block( ICON, NIL, NIL, INT, 0, INT, 0 );
        p->tn.lval = i;
        p->tn.rval = NONAME;
        return( clocal(p) );
}

NODE *
bpsize(p)
register NODE *p;
{
        return( offcon( psize(p), p->in.type, p->fn.cdim, p->fn.csiz ) );
}

OFFSZ
psize( p )
register NODE *p;
{
        /* p is a node of type pointer; psize returns the
        ** size of the thing pointed to
        */
					/*  bk:   put in manifest.h!!!  */ 
#define IS_FTNPTR(x)	( ISPTR(x) && ((x&TMASK1)==FTN<<TSHIFT) )

        if( !ISPTR(p->in.type) )
        {
                uerror( "pointer required");
                return( SZINT );
        } else if( IS_FTNPTR(p->in.type) )			/* bk */
        {
                uerror( "object pointer required");
                return( SZINT );
        }
        /* note: no pointers to fields */
        return( tsize( DECREF(p->in.type), p->fn.cdim, p->fn.csiz ) );
}

NODE *
unconvert( p, q )
register NODE *p, *q;
{
        /* p is divided by the size of q */
        /* q had better have type pointer */

# ifdef MYPDIV
        return( clocal( block( PDIV, p, bpsize(q), INT, 0, INT, 0 ) ) );
# else
        p = makety( p, PTRTYPE, 0, PTRTYPE );
        p = clocal( buildtree( DIV, p, bpsize(q) ) );

        return( makety( p, PTRTYPE, 0, PTRTYPE ) );
# endif
}

NODE *
convert( p, f )
register NODE *p;
register f;
{
        /*  convert an operand of p
        ** f is either CVTL or CVTR
        ** operand has type int, and is converted by the size of the other side
        */

        register NODE *q, *r;

        if( f == CVTL )
        {
                q = p->in.left;
                r = bpsize( p->in.right );
        }
        else
        {
                q = p->in.right;
                r = bpsize( p->in.left );
        }
# ifdef MYPMUL
        r = clocal( block( PMUL, q, r, PTRTYPE, 0, PTRTYPE, 0 ) );
# else
        /* if PTRTYPE is defined, make q this size; then, the MUL will be */
        q = makety( q, PTRTYPE, 0, PTRTYPE );
        r = clocal( buildtree( MUL, q, r ) );
# endif
        if( f == CVTL )
                p->in.left = r;
        else
                p->in.right = r;
        return(p);

}

econvert( p )
register NODE *p;
{

        /* change enums to ints, or appropriate types */

        register TWORD ty;

        if( (ty=BTYPE(p->in.type)) == ENUMTY || ty == MOETY )
        {
                if( dimtab[ p->fn.csiz ] == SZCHAR ) ty = CHAR;
                else if( dimtab[ p->fn.csiz ] == SZINT ) ty = INT;
                else if( dimtab[ p->fn.csiz ] == SZSHORT ) ty = SHORT;
                else ty = LONG;
                ty = ctype( ty );
                p->fn.csiz = ty;
                MODTYPE(p->in.type,ty);
                if( p->in.op == ICON && ty != LONG )
                        p->in.type = p->fn.csiz = INT;
        }
}

NODE *
pconvert( p, ftnarg )
register NODE *p;
int ftnarg;	/*bk: don't do default promotions for ftnargs (wait for ProtoChk) */
{
        register TWORD t;

        /* if p should be changed into a pointer, do so */
        /* also, widen p so it can be used as an operand */

        if( ISARY( p->in.type) )
        {
                p->in.type = DECREF( p->in.type );
                ++p->fn.cdim;
                return( buildtree( UNARY AND, p, NIL ) );
        }
        if( ISFTN( p->in.type) )
                return( buildtree( UNARY AND, p, NIL ) );

/*		if (ftnarg)				/ bk /
			return(p); */

        t = indtype( p->tn.type );
        if( t == p->tn.type ) return( p );
        if( p->tn.op == CONV && cbigger( p ) )
        {
                p->tn.type = p->fn.csiz = t;
        }
        else p = makety( p, t, 0, (int) t );
        return( p );
}

bigsize( t )
TWORD   t; 
{
        /* return a type size for t */
        switch( t ) 
        {

        case CHAR:
        case UCHAR:
                return( SZCHAR );

        case SHORT:
        case USHORT:
                return( SZSHORT );

        case VOID:
        case INT:
        case UNSIGNED:
                return( SZINT );

        case LONG:
        case ULONG:
                return( SZLONG );

        case FLOAT:
                return( SZLONG+1 );  /* must appear > long */
        case DOUBLE:
                return( SZLONG+2 );

		case ENUMTY:				/* bk */
				return( SZINT );	/* see ENUMSIZE, pftn.c, dclstruct() */

        }

        if( ISPTR(t) || ISARY(t) ) return( SZPOINT );
        cerror( "bad bigsize: 0%o", t );
/*NOTREACHED*/
}

cbigger( p )
register NODE *p; 
{
        /* returns 1 if the conversion op p makes things bigger */

        if( p->tn.op != CONV ) cerror( "cbigger" );
        return( bigsize( p->tn.type ) > bigsize( p->in.left->tn.type ) );
}

NODE *
oconvert(p)
register NODE *p;

{
        /* convert the result itself: used for pointer and unsigned */
        register TWORD t;

        switch(p->in.op)
        {

        case LE:
        case LT:
        case GE:
        case GT:
                t = p->in.left->in.type;
                if( ISUNSIGNED(t) || ISPTR(t) || ISARY(t) )
                {
                        p->in.op += (ULE-LE);
                }
                else
                {
                        t = p->in.right->in.type;
                        if( ISUNSIGNED(t) || ISPTR(t) || ISARY(t) )
                        {
                                p->in.op += (ULE-LE);
                        }
                }
        case EQ:
        case NE:
        case ULE:
        case ULT:
        case UGE:
        case UGT:
                return( p );

        case MINUS:
                return( unconvert( p, p->in.left ) );
        }

        cerror( "illegal oconvert: %d", p->in.op );

        return(p);
}

NODE *
ptmatch(p)
register NODE *p;
{

        /* makes the operands of p agree; they are
        ** either pointers or integers, by this time
        */
        /* with MINUS, the sizes must be the same */
        /* with COLON, the types must be the same */

        register TWORD t1, t2, t;
        register o, d2, d, s2, s;

        o = p->in.op;
        t = t1 = p->in.left->in.type;
        t2 = p->in.right->in.type;
        d = p->in.left->fn.cdim;
        d2 = p->in.right->fn.cdim;
        s = p->in.left->fn.csiz;
        s2 = p->in.right->fn.csiz;

        switch( o )
        {

        case ASSIGN:
        case RETURN:
		case PROTOCHK:
        case CAST:
                break;

        case MINUS:
                if( psize(p->in.left) != psize(p->in.right) )
                {
                        uerror( "illegal pointer subtraction");
                }
                p->tn.type = p->fn.csiz = PTRTYPE;
                p->fn.cdim = 0;
                return( clocal(p) );

        case COLON:
                if( t1 != t2 ) {
#define GENPTR(ty) (ty == PTR|VOID)			/*bk*/
					if ( GENPTR(t1) || GENPTR(t2) )
						;
					else 
						uerror( "illegal types in :");
				}
                break;

        default:
                if( !ISPTR(t1) )
                {
                        t = t2;
                        d = d2;
                        s = s2;
                        break;
                }
                if( !ISPTR(t2) )
                {
                        break;
                }

                /* both are pointers */
                if( talign(t2,s2) < talign(t,s) )
                {
                        t = t2;
                        s = s2;
                }
                break;
        }

        p->in.left = makety( p->in.left, t, d, s );
        p->in.right = makety( p->in.right, t, d, s );
        if( !logop(o) )
        {
			if ( o==CAST )			/* bk: should add typeq as arg to makety? */
				p->in.right->in.typeq = p->in.typeq;
			else if ( o == COLON ) 	/* bk: ANSI 3.3.15 */
				p->in.typeq |= (p->in.right->in.typeq | p->in.left->in.typeq );

			p->in.type = t;
			p->fn.cdim = d;
			p->fn.csiz = s;
        }

        return(clocal(p));
}

int tdebug = 0;

NODE *
tymatch(p)
register NODE *p;
{

        /* satisfy the types of various arithmetic binary ops */

        /* rules are:
        ** if any float or doubles, make double
        ** if any longs, make long
        ** if any ints, make int
        ** if any shorts, make short
        ** otherwise, make char
        ** if either operand is unsigned, the result is...
        ** if a simple assignment, type = type of lhs
        ** if an asg. op, type = type of original lhs
        **      (under CONV, if any)
        */

        register o;
        register TWORD t1, t2;
        TWORD t, tu;
        register NODE *l, *r;
        int u;

        o = p->in.op;

        t1 = (l=p->in.left)->in.type;
        t2 = (r=p->in.right)->in.type;

	/* bk: no more "unsigned-preserving" conversions, unless MODE_UNSIGNED */

        /* constants have a kind of "flexible" type */

        if( r->tn.op == ICON && r->tn.rval == NONAME &&
            (t1==CHAR || t1==SHORT || 
			( mode_chk(MODE_UNSIGNED) && (t1==UCHAR || t1==USHORT) )) )
        {
                /* if the constant retains its value when cast to the
                ** type of the lhs, assume it has the lhs type
                */
                if( r->tn.lval == ccast( r->tn.lval, t1 ) )
                {
                        r->in.type = t2 = t1;
                        r->fn.cdim = l->fn.cdim;
                        r->fn.csiz = l->fn.csiz;
                }
        }

        /* this is the opposite case from the above (it is too early
                to assume that constants are on the right) */

        if( l->tn.op == ICON && l->tn.rval == NONAME &&
            (t2==CHAR || t2==SHORT || 
			( mode_chk(MODE_UNSIGNED) && (t2==UCHAR || t2==USHORT) )) )
        {
                /* if the constant retains its value when cast to the
                ** type of the rhs, assume it has the rhs type
                */
                if( l->tn.lval == ccast( l->tn.lval, t2 ) )
                {
                        l->in.type = t1 = t2;
                        l->fn.cdim = r->fn.cdim;
                        l->fn.csiz = r->fn.csiz;
                }
        }
        if( (t1==VOID || t2==VOID) && o!=CAST )
                uerror("void type illegal in expression");

        u = 0;
        if( ISUNSIGNED(t1) )
        {
                u = 1;
                t1 = DEUNSIGN(t1);
        }
        if( ISUNSIGNED(t2) )
        {
                u = 1;
                t2 = DEUNSIGN(t2);
        }

			/*bk: float->double promotion only if user explicitly asks for it */
        if( t1==DOUBLE || t2==DOUBLE ) t = DOUBLE;
        else if( t1==FLOAT || t2==FLOAT ) t = mode_chk(MODE_FLOAT) ? 
												FLOAT : DOUBLE;
        else if( t1==LONG || t2==LONG ) t = LONG;
        else if( t1==INT || t2==INT ) t = INT;
        else if( t1==SHORT || t2==SHORT ) t = SHORT;
        else t = CHAR;

        if( o == ASSIGN || o == CAST || o == RETURN  || o == PROTOCHK  )
        {
                tu = t1;
                if( o == RETURN )
                {
                        if( tu==FLOAT ) tu = DOUBLE;
                        else if( tu==CHAR || tu==SHORT ) tu = INT;
                }
                t = t1 = tu;  /* t1 set to avoid lhs conversion */
				if( ISUNSIGNED(l->tn.type) ) u=1;
				else {
					if ( u ) {
						loseval_chk( p );	/* warn of unsigned->signed conv */
						u=0;
					}
				}
                t2 = r->tn.type;  /* back to reality... */
				/* bk */
	        	tu = (u && UNSIGNABLE(t))?ENUNSIGN(t):t;
        } else {

							/* bk: signed preserving, ANSI 3.2.1.1 */
			if ( !mode_chk(MODE_UNSIGNED) &&
				(ISUNSIGNED(l->tn.type) ^ ISUNSIGNED(r->tn.type)) ) {
											/* iff 1 op unsigned */
				if ( ISUNSIGNED(l->tn.type) ) {	
					if ( bigsize(l->tn.type) < bigsize(r->tn.type) )
						tu = r->tn.type;
					else 
						tu = l->tn.type;
				} else {
					if ( bigsize(r->tn.type) < bigsize(l->tn.type) )
						tu = l->tn.type;
					else 
						tu = r->tn.type;
				}
			} else 
		        tu = (u && UNSIGNABLE(t))?ENUNSIGN(t):t;
		}

        if( tu != t1 ) p->in.left = makety( l, tu, 0, (int)tu );
        if( tu != t2 ) p->in.right = makety( r, tu, 0, (int)tu );

        if( !logop(o) ) /* the type of the node = type of the operation */
        {
                p->in.type = tu;
                p->fn.cdim = 0;
                p->fn.csiz = t;
        }

        if( asgop(o) && p->in.left->tn.op == CONV  )
        {
                /* original, unconverted type */
                /* this will get rewritten to A = A op B, in most cases */
                /* if the op is an unsigned op, change the lhs to unsigned */
                l = p->in.left->in.left;
                tu = l->in.type;
				p->in.type=tu;	/* LHS represents result! ken#3 */
                if( u && (UNSIGNABLE(tu)) ) tu = ENUNSIGN(tu);
                /* p->in.type = l->in.type = tu;  / ken#3 - old code */
				/* jcl: breaks ( uchar OP= unsigned; ) */
                /*l->in.type = tu;  		/ ken#3 - new code */
                p->fn.cdim = l->fn.cdim;
                p->fn.csiz = l->fn.csiz;
        }

# ifndef NODBG
        if( tdebug )
        {
                printf( "tymatch(%o): %o '%s' %o => %o\n",p,t1,opst[o],t2,tu );
                e2print(p);
        }
# endif

        return(p);
}

NODE *
makety( p, t, d, s )
register NODE *p;
register TWORD t;
register d,s;
{
        /* make p into type t by inserting a conversion */
        register TWORD pt;

        pt = p->in.type;

        if( pt == ENUMTY && p->in.op == ICON )
        {
                econvert(p);
                pt = p->in.type;
        }

		/* non-simple type */
		if( ISPTR(pt)
# ifdef TWOPTRS
			&& TWOPTRS(t) == TWOPTRS(pt)
# else				/* bk */
			&& ( t == pt )
# endif
			)
		{
			if ( BTYPE(t) != STRTY || 			/* bk */
				p->fn.csiz == s && p->fn.cdim == d ) {

				/* don't generate CONV: just rewrite type */
				goto rew;
			} else
				return( block( CONV, p, NIL, t, d, s, 0 ) );
		}


		if( t == pt )
		{
rew:
                p->fn.type = t;
                p->fn.cdim = d;
                p->fn.csiz = s;

/*      for really heavy debugging
        printf( "block( %s, %d, %d, 0%o, %d, %d %d ) yields %d\n",
                opst[o], l-node, r-node, t, d, s, p-node, p->fn.typeq );
*/

                return( p );
        }

        if( t & TMASK )		/* if true, must be ftn or array, by this point */
        {
                return( block( CONV, p, NIL, t, d, s, 0 ) );
        }

        if( p->in.op == ICON && p->tn.rval==NONAME )
        {
                if( t==DOUBLE||t==FLOAT )
                {
                        p->in.op = FCON;
                        if( ISUNSIGNED(pt) )
                        {
								/* MOTOROLA 4.1 */
                                p->fpn.dval = (unsigned long) p->tn.lval;
                        }
                        else
                        {
                                p->fpn.dval = p->tn.lval;
                        }
                        goto rew;
                }
                p->tn.lval = ccast( p->tn.lval, t );
                goto rew;
        }
        return( block( CONV, p, NIL, t, d, s, 0 ) );
}

NODE *
block( o, l, r, t, d, s, tq )
register NODE *l, *r;
register TWORD t;
register o,d,s;
{

        register NODE *p;

        p = talloc();
        p->in.op = o;
        p->tn.goal = 0;
        p->in.left = l;
        p->in.right = r;
        p->in.type = t;
        p->fn.cdim = d;
        p->fn.csiz = s;
		p->fn.typeq = tq;
		p->in.proto = 0;
        return(p);
}

icons(p)
register NODE *p;
{
        /* if p is an integer constant, return its value */
        register val;

        if( p->in.op != ICON )
        {
                uerror( "integral constant expected");
                val = 1;
        }
        else
        {
                val = p->tn.lval;
                if( val != p->tn.lval )
                        uerror( "constant too big for cross-compiler" );
        }
        tfree( p );
        return(val);
}

/*      the intent of this table is to examine the
** operators, and to check them for
** correctness.
**
** The table is searched for the op and the
** modified type (where this is one of the
** types INT (includes char and short), LONG,
** DOUBLE (includes FLOAT), and POINTER
**
** The default action is to make the node type integer
**
** The actions taken include:
** PUN    check for puns
** CVTL   convert the left operand
** CVTR   convert the right operand
** TYPL   the type is determined by the left operand
** TYPR   the type is determined by the right operand
** TYMATCH        force type of left and right to match, by inserting conversions
** PTMATCH        like TYMATCH, but for pointers
** LVAL   left operand must be lval
** CVTO   convert the op
** NCVT   do not convert the operands
** FCVT   don't do default conversions, but do ftn-to-ftnptr conversion(bk)
** OTHER          handled by code
** NCVTR          convert the left operand, not the right...
**
*/

# define MINT 01  /* integer */
# define MDBI 02   /* integer or double */
# define MSTR 04  /* structure */
# define MPTR 010  /* pointer */
# define MPTI 020  /* pointer or integer */
# define MENU 040 /* enumeration variable or member */

# define SCALAR_TYPES 	(MINT | MDBI | MPTR | MPTI | MENU)			/* bk */

opact( p )
register NODE *p;
{
        register mt12, mt1, mt2, o;

        mt12 = 0;

        switch( optype(o=p->in.op) )
        {

        case BITYPE:
                mt12=mt2 = moditype( p->in.right->in.type );
        case UTYPE:
                mt12 &= (mt1 = moditype( p->in.left->in.type ));

        }

        switch( o )
        {

        case NAME :
        case STRING :
        case ICON :
        case FCON :
        case CALL :
        case UNARY CALL:
        case STAR:
                {
                          return( OTHER );
                }
        case UNARY MINUS:
        case UNARY PLUS:			/*bk*/
                if( mt1 & MDBI ) return( TYPL );
                break;

        case COMPL:
                if( mt1 & MINT ) return( TYPL );
                break;

        case NOT:				/* bk: must be scalar */
        case CBRANCH:
                if( mt1 & SCALAR_TYPES ) return( NCVT );
                break;

        case UNARY AND:
                {
                          return( NCVT+OTHER );
                }
        case INIT:
        case CM:
                return( 0 );

        case ANDAND:			/* bk: both opnds must be scalar */
                if( mt12 & SCALAR_TYPES ) return( NCVT );
				break;
        case OROR:				/* but allow rhs of OROR to be void */
                if( mt12 & SCALAR_TYPES || 
					mt1 & SCALAR_TYPES && p->in.right->in.type == VOID )
					return( NCVT );
				break;

        case MUL:
        case DIV:
                if( mt12 & MDBI ) return( TYMATCH );
                break;

        case MOD:
        case AND:
        case OR:
        case ER:
                if( mt12 & MINT ) return( TYMATCH );
                break;

        case LS:
        case RS:
                if( mt12 & MINT ) return( TYMATCH+OTHER );
                break;

        case EQ:
        case NE:
        case LT:
        case LE:
        case GT:
        case GE:
                if( (mt1&MENU)||(mt2&MENU) ) return( PTMATCH+PUN+NCVT );
                if( mt12 & MDBI ) return( TYMATCH+CVTO+NCVT );
                else if( mt12 & MPTR ) return( PTMATCH+PUN+CVTO );
                else if( mt12 & MPTI ) return( PTMATCH+PUN+CVTO );
                else break;

        case QUEST:
				if ( mt1 & SCALAR_TYPES ) return( TYPR+NCVTR );
				break;

        case COMOP:						/*bk*/
                return( TYPR|NCVT|FCVT );

        case STREF:
                return( NCVTR+OTHER );

        case COLON:
                if( mt12 & MENU ) return( NCVT+PUN+PTMATCH );
                else if( mt12 & MDBI ) return( TYMATCH );
                else if( mt12 & MPTR ) return( TYPL+PTMATCH+PUN );
                else if( (mt1&MINT) && (mt2&MPTR) ) return( TYPR+PUN );
                else if( (mt1&MPTR) && (mt2&MINT) ) return( TYPL+PUN );
		else if( (mt1&MINT) && (mt2&MENU) ) return( TYPL+PUN );
		else if( (mt1&MENU) && (mt2&MINT) ) return( TYPR+PUN );
                else if( mt12 & MSTR ) return( NCVT+TYPL+OTHER );
				else if( mt1==0 && mt2==0 ) return( TYPL );	/*bk*/
                break;

		case HIDASG:			/* bk: hidden static allocator */
                return( NCVT+OTHER );
				break;

        case ASSIGN:
        case RETURN:
                if( mt12 & MSTR ) return( LVAL+NCVT+TYPL+OTHER );
        case CAST:
                
		if(o==CAST 
			&& p->in.right->in.type  == p->in.left->in.type 
			&& p->in.right->fn.csiz  == p->in.left->fn.csiz
			&& p->in.right->in.typeq == p->in.left->in.typeq)
			return(NCVT); /* roger: if cast and object casted are the same type do nothing */
                if(o==CAST && mt1==0)return(TYPL+TYMATCH);
                if( mt12 & MDBI ) return( TYPL+LVAL+TYMATCH );
                else if( (mt1&MENU)||(mt2&MENU) )
                {
                        return( LVAL+NCVT+TYPL+PTMATCH+PUN );
                }
                else if( mt12 == 0 ) break;
                else if( mt1 & MPTR ) return( LVAL+PTMATCH+PUN );
                else if( mt12 & MPTI ) return( TYPL+LVAL+TYMATCH+PUN );
                break;

		case PROTOCHK:
                if( mt12 & MSTR ) return( NCVT+TYPL);
                if(/* o==CAST && */ mt1==0)return(NCVT+TYPL+TYMATCH);
                if( mt12 & MDBI ) return( NCVT+TYPL+TYMATCH );
                else if( (mt1&MENU)||(mt2&MENU) )
                {
                        return( NCVT+TYPL+PTMATCH+PUN );
                }
                else if( mt12 == 0 ) break;
                else if( mt1 & MPTR ) return( NCVT+TYPL+PTMATCH+PUN+FCVT );
                else if( mt12 & MPTI ) return( NCVT+TYPL+TYMATCH+PUN );
                break;

        case ASG MUL:
        case ASG DIV:
                if( mt12 & MDBI ) return( LVAL+TYMATCH );
                break;

        case ASG MOD:
        case ASG AND:
        case ASG OR:
        case ASG ER:
                if( mt12 & MINT ) return( LVAL+TYMATCH );
                break;

        case ASG LS:
        case ASG RS:
                if( mt12 & MINT ) return( LVAL+TYMATCH+OTHER );
                break;

        case ASG PLUS:
        case ASG MINUS:
        case INCR:
        case DECR:
                if( mt12 & MDBI ) return( TYMATCH+LVAL );
                else if( (mt1&MPTR) && (mt2&MINT) ) return( TYPL+LVAL+CVTR );
/* bk */ 		else if( (mt1&MENU) && (mt2&MINT) ) return( TYPL+LVAL ); 
                break;

        case MINUS:
                if( mt12 & MPTR ) return( CVTO+PTMATCH+PUN );
                if( mt2 & MPTR ) break;
        case PLUS:
                if( mt12 & MDBI ) return( TYMATCH );
                else if( (mt1&MPTR) && (mt2&MINT) ) return( TYPL+CVTR );
                else if( (mt1&MINT) && (mt2&MPTR) ) return( TYPR+CVTL );
				break;				/* bk: ANSI 3.3.6 */

                /* special operators */
        case UOP0:
        case UOP1:
        case UOP2:
        case UOP3:
        case UOP4:
        case UOP5:
        case UOP6:
        case UOP7:
        case UOP8:
        case UOP9:
                return( TYPL );
        }

		/* bk: let's try to issue some meaningful messages */
		if ( p->in.left->tn.op == RNODE ) {	/* special case */
        	uerror( "incompatible return type" );
		} else {
			switch ( o ) {
				case PROTOCHK: {
					extern int proto_argno;
					uerror( "incompatible type, prototype arg %d", proto_argno);
					break;
				}
				case CBRANCH:
					uerror("non-scalar conditional expression");
					break;
				default:
		        	uerror( "operands of %s have incompatible types", opst[o] );
					break;
			}
		}
        return( NCVT );
}

moditype( ty )
register TWORD ty;
{

        switch( ty )
        {

        case UNDEF:
        case VOID:
                return(0); /* type is void */
        case ENUMTY:
        case MOETY:
                return( MENU );

        case STRTY:
        case UNIONTY:
                return( MSTR );

        case CHAR:
        case SHORT:
        case UCHAR:
        case USHORT:
                return( MINT|MPTI|MDBI );

        case UNSIGNED:
        case ULONG:
        case INT:
        case LONG:
                return( MINT|MDBI|MPTI );

        case FLOAT:
        case DOUBLE:
                return( MDBI );

        default:
                return( MPTR|MPTI );
        }
}

# ifndef MYCCAST
CONSZ
ccast( v, t )
register CONSZ v;
register TWORD t;
{
        /* cast value v into simple type t */
        /* must be done as it would be on the target machine */

        switch( t )
        {

        case CHAR:
# ifdef CHSIGN
                if( v&ONEBIT(SZCHAR-1) )
                {
                        return( v | ~BITMASK(SZCHAR) );
                }
# endif
        case UCHAR:
                return( v & BITMASK(SZCHAR) );

        case SHORT:
                if( v&ONEBIT(SZSHORT-1) )
                {
                        return( v | ~BITMASK(SZSHORT) );
                }
        case USHORT:
                return( v & BITMASK(SZSHORT) );

        case INT:
                if( v&ONEBIT(SZINT-1) )
                {
                        return( v | ~BITMASK(SZINT) );
                }
        case UNSIGNED:
                return( v & BITMASK(SZINT) );

        default:
                return( v );
        }
}
# endif


NODE *
doszof( p )
register NODE *p;
{
        /* do sizeof p */
        register i;

		if ( p->in.op == FLD ) {		/* bk: ANSI 3.3.3.4 */
			uerror( "cannot take sizeof bit field object" );
			i = 0;
		} else if ( ISFTN( p->in.type ) ) {		/* bk: ANSI 3.3.3.4 */
			uerror( "cannot take sizeof function expression" );
			i = 0;
		} else {
			i = tsize( p->in.type, p->fn.cdim, p->fn.csiz )/SZCHAR;
			if( i <= 0 ) werror( W_DFLT, "sizeof returns 0" );
		}

       	tfree(p);
        p = bcon(i);
        p->tn.type = UNSIGNED;  /* damn dmr anyhow! */
        return( p );
}

/* bk: for __argtype: should integrate into buildtree like any other op */
NODE *
do_argty( p )
register NODE *p;
{
	extern int ftn_arg;
	TWORD ityp;

	ftn_arg = 1;						/* take care of float->double */
	ityp = indtype( p->in.type );	
	ftn_arg = 0;

	if ( ityp != p->in.type )
		p->fn.csiz = BTYPE(ityp);
	p->in.type = ityp;
	p->in.op = ARGTYPE;

	return( p );
}

# ifndef NODBG
eprint(p)
NODE *p;
{
        printf("\n++++++++\n");
        e1print(p,"T");
        printf("---------\n");
}
e1print( p ,s)
register NODE *p;
char *s;
{
        register ty, d;
        static down = 0;

        ty = optype( p->tn.op );

        if( ty == BITYPE )
        {
                ++down;
                e1print( p->in.right ,"R");
                --down;
        }

        for( d=down; d>1; d -= 2 ) printf( "\t" );
        if( d ) printf( "    " );

        printf("%s=%d) %s, ", s, (int) (p-node), opst[p->in.op] );
        if( ty == LTYPE )
        {
                printf( "lval=%ld", p->tn.lval );
                printf( ", rval=%d, ", p->tn.rval );
        }

		if ( p->in.typeq ) {
			printf("qual=");				/* more debugging */
			qprint( p->in.typeq );
		}
		if ( HASPROTO(p->in.proto) )
			printf( " %s", stab[ proto_lists[p->in.proto].lsymidx ].sname );

        printf(", type=");
        tprint(stdout, p->in.type );	/* ken#4 : designate stdout */
        printf( ", dim=%d, siz=%d\n", p->fn.cdim, p->fn.csiz );
        if( ty != LTYPE )
        {
                ++down;
                e1print( p->in.left ,"L");
                --down;
        }
}

tprint( out, t )
FILE *out;	/* ken#4  new arg to allow print to stderr as well as stdout */
register TWORD t; 
{
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
                "strty",
                "unionty",
                "enumty",
                "moety",
                "uchar",
                "ushort",
                "unsigned",
                "ulong",
                "void", "?"
        };

        for(;; t = DECREF(t) )
        {
		/* ken#4 changed printf(...) to fprintf(out,...) so can print
		** on stderr as well as stdout */
                if( ISPTR(t) ) fprintf(out, "PTR " );
                else if( ISFTN(t) ) fprintf(out, "FTN " );
                else if( ISARY(t) ) fprintf(out, "ARY " );
                else 
                {
                        fprintf(out, "%s", tnames[t] );
                        return;
                }
        }
}

# endif

# ifndef MYLOCCTR
locctr(l)
register l;
{
        register temp, lt;
        /* look in locnames; print out the location counter name */
        /* null means use the next; all null, don't print */
        for( lt=l; lt<=STRWRT && !locnames[lt]; ++lt )
        {
                 /* EMPTY */
        }
        if( lt == curloc ) return( lt );
        temp = curloc;
        if( lt > STRWRT ) lt = l;
        else printf( locnames[lt] );
        curloc = lt;
        return( temp );
}
# endif

# ifndef NOFLOAT

prtdcon( p )
register NODE *p;
{
        register i;
        register TWORD t;

        if( p->in.op == FCON )
        {
                locctr( DATA );
                t = p->tn.type;
                defalign( t==DOUBLE?ALDOUBLE:ALFLOAT );
                deflab( i = getlab() );
                fincode( p->fpn.dval, t==DOUBLE?SZDOUBLE:SZFLOAT );
                p->tn.lval = 0;
                p->tn.rval = -i;
                p->in.op = NAME;
        }
        if( (i = optype(p->in.op)) == BITYPE ) prtdcon( p->in.right );
        if( i != LTYPE ) prtdcon( p->in.left );
}
# endif

# ifndef MYLABELS
getlab()
{
        static crslab = 10;
        return( ++crslab );
}
# endif

int edebug = 0;
ecomp( p )
register NODE *p;
{
		extern hidasg;				/*bk*/
# ifndef NODBG
        if( edebug ) eprint(p);
# endif
        if( !reached )
        {
                werror( W_LINT, "statement not reached" );
                reached = 1;
        }
# ifdef CLOCAL
        p = clocal(p);
# endif

        p = optim(p);
# ifndef NOFLOAT
# ifdef M68881
        if( !is_m68881 )
# endif
                prtdcon(p);
# endif
        locctr( PROG );
        ecode(p);
        tfree(p);
}

# ifndef MYECODE
ecode( p )
register NODE *p;
{
        /* standard version of writing the tree nodes */
        if( nerrors ) return;
# ifdef SDB
        sdbline();
# endif

		grey( p );		/* diagnose grey expressions */

/*
 *      BEGIN: ported from VAX pcc to make two pass version
 */

# ifdef TWOPASS
        printf( ".%d\t%s\n", lineno, ftitle );
        prtree( p );
# else                  /* else we have a ONEPASS */
        p2tree( p );
        p2compile( p );
#endif

/*
 *      END: ported from VAX pcc to make two pass version
 */
}
# endif

# ifdef STDPRTREE /****************************************/

# ifndef RNODNAME
# define RNODNAME LABFMT
# endif

# ifndef ONEPASS

prtree(p) register NODE *p; {

        register struct symtab *q;
        register ty;
        char temp[32];                  /* place to dump label stuff */
        union flt_dbl x;                /* double/float constant union */
		int hidasg = 0;		/*bk*/


# ifdef MYPRTREE
        MYPRTREE(p);  /* local action can be taken here; then return... */
#endif

        ty = optype(p->in.op);

		if ( p->in.op == HIDASG ) {			/* bk: almost same as STASG */
        	register NODE *l;
			hidasg = 1;
			p->in.op = STASG;
			l = p->in.left;
			p->stn.stsize = tsize(DECREF(l->fn.type),l->fn.cdim,l->fn.csiz);
			p->stn.stalign = talign(DECREF(l->fn.type),l->fn.csiz);
		}

        printf( "%d\t", p->in.op );

        if( ty == LTYPE ) {
                printf( CONFMT, p->tn.lval );
                printf( "\t" );
                }
        if( ty != BITYPE ) {
                if( p->in.op == NAME || p->in.op == ICON || p->in.op == FCON)
                        printf( "0\t" );
                else printf( "%d\t", p->tn.rval );
                }

        printf( "%o\t", p->in.type );

        /* handle special cases */

        switch( p->in.op ){

		case VAUTO:				/* bk: print type qualifier info */
		case VPARAM:
       		printf( "%o\t\n", p->in.typeq );
			break;

        case NAME:
        case ICON:
                /* print external name */
                if( p->tn.rval == NONAME ) printf( "\n" );
                else if( p->tn.rval >= 0 ){
                        q = &stab[p->tn.rval];
#ifndef FLEXNAMES
                        printf(  "%.8s\n", exname(q->sname) );
#else
                        printf(  "%s\n", exname(q->sname) );
#endif
                        }
                else if( p->tn.rval == - strftn )
                {
                        printf( RNODNAME, -p->tn.rval );
                        printf("\n");
                }
                else { /* label */
                        printf( LABFMT, -p->tn.rval );
                        printf("\n");
                        }
                break;

        case STARG:
        case STASG:
        case STCALL:
        case UNARY STCALL:
                /* print out size */
                /* use lhs size, in order to avoid hassles with the structure `.' operator */

                /* note: p->in.left not a field... */
				if ( hidasg ) {				/*bk*/
					printf( CONFMT, p->stn.stsize );
					printf( "\t%d\t\n", p->stn.stalign );
				} else {
					printf( CONFMT, (CONSZ) tsize( 
						STRTY, p->in.left->fn.cdim, p->in.left->fn.csiz ) );
					printf( "\t%d\t\n", talign( 
						STRTY, p->in.left->fn.csiz ) );
				}
                break;
#ifdef M68881
        case FCON:
#if vax
                x.d = p->fpn.dval;
                dbltolong( &x, (p->fpn.type == DOUBLE? SZDOUBLE: SZFLOAT) );
                if(p->fpn.type == DOUBLE )
                {
                        printf( "%d\t%d\t\n", x.lg.l, x.lg.l2 );
                }
                else
                {
                        printf( "%d\t\n", x.lg.l );
                }
#else
                if(p->fpn.type == DOUBLE )
                {
                        x.d = p->fpn.dval;
                        printf( "%d\t%d\t\n", x.lg.l, x.lg.l2 );
                }
                else
                {
                        x.f = p->fpn.dval;
                        printf( "%d\t\n", x.lg.l );
                }
#endif
                break;
#endif
        default:
                printf(  "\n" );
                }

        if( ty != LTYPE ) prtree( p->in.left );
        if( ty == BITYPE ) prtree( p->in.right );

        }

#endif

p2tree(p)
register NODE *p;
{
        register ty;
        register NODE *l;
        register o;
        char temp[32];                  /* place to dump label stuff */

# ifdef MYP2TREE
        MYP2TREE(p);  /* local action can be taken here; then return... */
# endif

        /* this routine sits painfully between pass1 and pass2 */
        ty = optype(o=p->in.op);
        p->tn.goal = 0;  /* an illegal goal, just to clear it out */
#ifndef TWOPASS
# ifdef ITREES
        if (p->in.op != SWCASE)
                /* KLUGE type is not really a type in swcase */
# endif
        p->tn.type = ttype( p->tn.type );  /* type gets second-pass (bits) */
#endif

        switch( o )
        {

        case TEMP:
        case NAME:
        case ICON:
        case VAUTO:
        case VPARAM:
                if( p->tn.rval == NONAME )
                        p->in.name = (char *) 0;
                else if( p->tn.rval >= 0 )
                {
                         /* copy name from exname */
                        register char *cp;
                        cp = exname( stab[p->tn.rval].sname );
                        p->in.name = tstr( cp );
                }
                else if( p->tn.rval == - strftn )
                {
                        sprintf( temp, RNODNAME, -p->tn.rval );
                        p->in.name = tstr( temp );
                }
                else
                {
                        sprintf( temp, LABFMT, -p->tn.rval );
                        p->in.name = tstr( temp );
                }
                break;


        case STARG:
        case STASG:
        case STCALL:
        case UNARY STCALL:
                /* set up size parameters */
                l = p->in.left;
                p->stn.stsize = tsize(STRTY,l->fn.cdim,l->fn.csiz);
                p->stn.stalign = talign(STRTY,l->fn.csiz);
                break;

# ifndef TWOPASS
                /* this should do something only if temporary regs are
                /* built into the tree by machine-dependent actions */
        case REG:
                rbusy( p->tn.rval, p->in.type );
#endif
        default:
                p->in.name = (char *) 0;
        }

        if( ty != LTYPE ) p2tree( p->in.left );
        if( ty == BITYPE ) p2tree( p->in.right );
}

# endif /******************************************/

/* begin ken#4 */
