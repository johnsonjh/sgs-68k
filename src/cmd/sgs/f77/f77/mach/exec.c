/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) exec.c: version 25.1 created on 12/2/91 at 17:36:33	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)exec.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	exec.c	7.2		*/

#ident	"@(#)f77/f77/mach:exec.c	25.1"

#include "defs"

/*   Logical IF codes
*/


exif(p)
expptr p;
{
pushctl(CTLIF);
#if TARGET==M68K
ctlstack->elselabel = getlab();
#else
ctlstack->elselabel = newlabel();
#endif
putif(p, ctlstack->elselabel);
}



exelif(p)
expptr p;
{
if(ctlstack->ctltype == CTLIF)
	{
	if(ctlstack->endlabel == 0)
#if TARGET==M68K
		ctlstack->endlabel = getlab();
#else
  		ctlstack->elselabel = newlabel();
#endif
	putgoto(ctlstack->endlabel);
	putlabel(ctlstack->elselabel);
#if TARGET==M68K
	ctlstack->elselabel = getlab();
#else
	ctlstack->elselabel = newlabel();
#endif
	putif(p, ctlstack->elselabel);
	}

else	execerr("elseif out of place", CNULL);
}





exelse()
{
if(ctlstack->ctltype==CTLIF)
	{
	if(ctlstack->endlabel == 0)
#if TARGET==M68K
		ctlstack->endlabel = getlab();
#else
  		ctlstack->endlabel = newlabel();
#endif
	putgoto( ctlstack->endlabel );
	putlabel(ctlstack->elselabel);
	ctlstack->ctltype = CTLELSE;
	}

else	execerr("else out of place", CNULL);
}


exendif()
{
if(ctlstack->ctltype == CTLIF)
	{
	putlabel(ctlstack->elselabel);
	if(ctlstack->endlabel)
		putlabel(ctlstack->endlabel);
	popctl();
	}
else if(ctlstack->ctltype == CTLELSE)
	{
	putlabel(ctlstack->endlabel);
	popctl();
	}

else
	execerr("endif out of place", CNULL);
}



LOCAL pushctl(code)
int code;
{
register int i;

if(++ctlstack >= lastctl)
	many("loops or if-then-elses", 'c');
ctlstack->ctltype = code;
for(i = 0 ; i < 4 ; ++i)
	ctlstack->ctlabels[i] = 0;
++blklevel;
}


LOCAL popctl()
{
if( ctlstack-- < ctls )
	fatal("control stack empty");
--blklevel;
}



LOCAL poplab()
{
register struct Labelblock  *lp;

for(lp = labeltab ; lp < highlabtab ; ++lp)
	if(lp->labdefined)
		{
		/* mark all labels in inner blocks unreachable */
		if(lp->blklevel > blklevel)
			lp->labinacc = YES;
		}
	else if(lp->blklevel > blklevel)
		{
		/* move all labels referred to in inner blocks out a level */
		lp->blklevel = blklevel;
		}
}



/*  BRANCHING CODE
*/

exgoto(lab)
struct Labelblock *lab;
{
putgoto(lab->labelno);
}







exequals(lp, rp)
register struct Primblock *lp;
register expptr rp;
{
if(lp->tag != TPRIM)
	{
	err("assignment to a non-variable");
	frexpr(lp);
	frexpr(rp);
	}
else if(lp->namep->vclass!=CLVAR && lp->argsp)
	{
	if(parstate >= INEXEC)
		err("statement function amid executables");
	else
		mkstfunct(lp, rp);
	}
else if (lp->namep->vdovar)
	err("redifinition of do variable");
else
	{
	if(parstate < INDATA)
		enddcl();
	puteq(mklhs(lp), fixtype(rp));
	}
}



mkstfunct(lp, rp)
struct Primblock *lp;
expptr rp;
{
register struct Primblock *p;
register Namep np;
chainp args;

np = lp->namep;
if(np->vclass == CLUNKNOWN)
	np->vclass = CLPROC;
else
	{
	dclerr("redeclaration of statement function", np);
	return;
	}
np->vprocclass = PSTFUNCT;
np->vstg = STGSTFUNCT;
impldcl(np);
args = (lp->argsp ? lp->argsp->listp : CHNULL);
np->varxptr.vstfdesc = mkchain(args , rp );

for( ; args ; args = args->nextp)
	if( args->datap->tag!=TPRIM ||
		(p = (struct Primblock *) (args->datap) )->argsp ||
		p->fcharp || p->lcharp )
		err("non-variable argument in statement function definition");
	else
		{
		args->datap = (tagptr) (p->namep);
		vardcl(p->namep);
		free(p);
		}
}



excall(name, args, nstars, labels)
Namep name;
struct Listblock *args;
int nstars;
struct Labelblock *labels[ ];
{
register expptr p;

settype(name, TYSUBR, ENULL);
p = mkfunct( mkprim(name, args, CHNULL) );
p->exprblock.vtype = p->exprblock.leftp->headblock.vtype = TYINT;
if(nstars > 0)
	putcmgo(p, nstars, labels);
else putexpr(p);
}



exstop(stop, p)
int stop;
register expptr p;
{
char *q;
int n;
expptr mkstrcon();

	if(p)
	{
		if( ! ISCONST(p) )
		{
			execerr("pause/stop argument must be constant", CNULL);
			frexpr(p);

			/* hoss#20 -- first arg 1 & not 0 */ 
			p = mkstrcon(1, "");
		}
		else if( ISINT(p->constblock.vtype) )
		{
			q = convic(p->constblock.cons.ci);
			n = strlen(q);
			if(n > 0)
			{
				p->constblock.cons.ccp = copyn(n, q);
				p->constblock.vtype = TYCHAR;
				p->constblock.vleng = (expptr) ICON(n);
			}
			else
				/* hoss#20 -- first arg 1 & not 0 */
				p = (expptr) mkstrcon(1, "");
		}
		else if(p->constblock.vtype != TYCHAR)
		{
			execerr("pause/stop argument must be integer or string"
				, CNULL);

			/* hoss#20 -- first arg 1 & not 0 */
			p = (expptr) mkstrcon(1, "");
		}
	}
	else	
		/* hoss#20 -- first arg 1 & not 0 */
		p = (expptr) mkstrcon(1, "");

	putexpr( call1(TYSUBR, (stop ? "s_stop" : "s_paus"), p) );
}

/* DO LOOP CODE */

#define DOINIT	par[0]
#define DOLIMIT	par[1]
#define DOINCR	par[2]

#define VARSTEP	0
#define POSSTEP	1
#define NEGSTEP	2

int lvl = 1;	/* nesting level, must be >= 2 inside of a procedure */

exdo(range, spec)
int range;
chainp spec;
{
register expptr p, q;
expptr q1;
register Namep np;
chainp cp;
register int i;
int dotype, incsign;
Addrp dovarp, dostgp;
expptr par[3];

pushctl(CTLDO);
dorange = ctlstack->dolabel = range;
np = (Namep) (spec->datap);
ctlstack->donamep = NULL;
if(np->vdovar)
	{
	errstr("nested loops with variable %s", varstr(VL,np->varname));
	ctlstack->donamep = NULL;
	return;
	}

dovarp = mkplace(np);
/*
Mon Jun  2 09:58:13 PDT 1986
hanna
if( ! ONEOF(dovarp->vtype, M(TYLONG)|MSKREAL) )
page 4-25 (308-278) of att fortran manual DOES say that long int only 
is allowed for do variable.  MSKINT will include short AND int.
*/
if( ! ONEOF(dovarp->vtype, MSKINT|MSKREAL) )
	{
	err("bad type on do variable");
	return;
	}
ctlstack->donamep = np;

np->vdovar = YES;
if( enregister(np) )
	{
	/* stgp points to a storage version, varp to a register version */
	dostgp = dovarp;
	dovarp = mkplace(np);
	}
else
	dostgp = NULL;
#if defined(SGS2) && !defined(FONEPASS)
++lvl;		/* incr block level number */
prhead();	/* right bracket informs code generator of reg usage*/
#endif
dotype = dovarp->vtype;

for(i=0 , cp = spec->nextp ; cp!=NULL && i<3 ; cp = cp->nextp)
	{
	p = par[i++] = fixtype(cp->datap);
	if( ! ONEOF(p->headblock.vtype, MSKINT|MSKREAL) )
		{
		err("bad type on DO parameter");
		return;
		}
	}

frchain(&spec);
switch(i)
	{
	case 0:
	case 1:
		err("too few DO parameters");
		return;

	default:
		err("too many DO parameters");
		return;

	case 2:
		DOINCR = (expptr) ICON(1);

	case 3:
		break;
	}

#if TARGET==M68K
ctlstack->endlabel = getlab();
ctlstack->dobodylabel = getlab();
#else
ctlstack->endlabel = newlabel();
ctlstack->dobodylabel = newlabel();
#endif

if( ISCONST(DOLIMIT) )
	ctlstack->domax = mkconv(dotype, DOLIMIT);
else
	ctlstack->domax = (expptr) mktemp(dotype, PNULL);

if( ISCONST(DOINCR) )
	{
	ctlstack->dostep = mkconv(dotype, DOINCR);
	if( (incsign = conssgn(ctlstack->dostep)) == 0)
		err("zero DO increment");
	ctlstack->dostepsign = (incsign > 0 ? POSSTEP : NEGSTEP);
	}
else
	{
	ctlstack->dostep = (expptr) mktemp(dotype, PNULL);
	ctlstack->dostepsign = VARSTEP;
#if TARGET==M68K
	ctlstack->doposlabel = getlab();
	ctlstack->doneglabel = getlab();
#else
  	ctlstack->doposlabel = newlabel();
  	ctlstack->doneglabel = newlabel();
#endif
	}

if( ISCONST(ctlstack->domax) && ISCONST(DOINIT) && ctlstack->dostepsign!=VARSTEP)
	{
	puteq(cpexpr(dovarp), cpexpr(DOINIT));
	if( onetripflag )
		frexpr(DOINIT);
	else
		{
		q = mkexpr(OPPLUS, ICON(1),
			mkexpr(OPMINUS, cpexpr(ctlstack->domax), cpexpr(DOINIT)) );
		if(incsign != conssgn(q) && 
		!( ISICON(DOINIT) && ISICON(ctlstack->domax)&&
	(ctlstack->domax->constblock.cons.ci == DOINIT->constblock.cons.ci)))
	/* 256-14 */
			{
			warn("DO range never executed");
			putgoto(ctlstack->endlabel);
			}
		frexpr(q);
		}
	}
else if(ctlstack->dostepsign!=VARSTEP && !onetripflag)
	{
	if( ISCONST(ctlstack->domax) )
		q = (expptr) cpexpr(ctlstack->domax);
	else
		q = mkexpr(OPASSIGN, cpexpr(ctlstack->domax), DOLIMIT);

	q1 = mkexpr(OPASSIGN, cpexpr(dovarp), DOINIT);
	q = mkexpr( (ctlstack->dostepsign==POSSTEP ? OPLE : OPGE), q1, q);
	putif(q, ctlstack->endlabel);
	}
else
	{
	if(! ISCONST(ctlstack->domax) )
		puteq( cpexpr(ctlstack->domax), DOLIMIT);
	q = DOINIT;
	if( ! onetripflag )
		q = mkexpr(OPMINUS, q,
			mkexpr(OPASSIGN, cpexpr(ctlstack->dostep), DOINCR) );
	puteq( cpexpr(dovarp), q);
	if(onetripflag && ctlstack->dostepsign==VARSTEP)
		puteq( cpexpr(ctlstack->dostep), DOINCR);
	}

if(ctlstack->dostepsign == VARSTEP)
	{
	if(onetripflag)
		putgoto(ctlstack->dobodylabel);
	else
		putif( mkexpr(OPGE, cpexpr(ctlstack->dostep), ICON(0)),
			ctlstack->doneglabel );
	putlabel(ctlstack->doposlabel);
	putif( mkexpr(OPLE,
		mkexpr(OPPLUSEQ, cpexpr(dovarp), cpexpr(ctlstack->dostep)),
		cpexpr(ctlstack->domax) ),
			ctlstack->endlabel);
	}
putlabel(ctlstack->dobodylabel);
if(dostgp)
	puteq(dostgp, cpexpr(dovarp));
frexpr(dovarp);
}



enddo(here)
int here;
{
register struct Ctlframe *q;
register expptr t;
Namep np;
Addrp ap;
register int i;

while(here == dorange)
	{
	if(np = ctlstack->donamep)
		{
		t = mkexpr(OPPLUSEQ, mkplace(ctlstack->donamep),
			cpexpr(ctlstack->dostep) );
	
		if(ctlstack->dostepsign == VARSTEP)
			{
			putif( mkexpr(OPLE, cpexpr(ctlstack->dostep), ICON(0)), ctlstack->doposlabel);
			putlabel(ctlstack->doneglabel);
			putif( mkexpr(OPLT, t, ctlstack->domax), ctlstack->dobodylabel);
			}
		else
			putif( mkexpr( (ctlstack->dostepsign==POSSTEP ? OPGT : OPLT),
				t, ctlstack->domax),
				ctlstack->dobodylabel);
		putlabel(ctlstack->endlabel);
		if(ap = memversion(np))
			puteq(ap, mkplace(np));
		for(i = 0 ; i < 4 ; ++i)
			ctlstack->ctlabels[i] = 0;
		deregister(ctlstack->donamep);
#if defined(SGS2) && !defined(FONEPASS)
		p2rbrack(lvl--);	/* close of block started in exdo */
#endif
		ctlstack->donamep->vdovar = NO;
		frexpr(ctlstack->dostep);
		}

	popctl();
	poplab();
	dorange = 0;
	for(q = ctlstack ; q>=ctls ; --q)
		if(q->ctltype == CTLDO)
			{
			dorange = q->dolabel;
			break;
			}
	}
}

exassign(vname, labelval)
Namep vname;
struct Labelblock *labelval;
{
Addrp p;
expptr mkaddcon();

p = mkplace(vname);
if( ! ONEOF(p->vtype, MSKINT|MSKADDR) )
	err("noninteger assign variable");
else
	puteq(p, mkaddcon(labelval->labelno) );
}



exarif(expr, neglab, zerlab, poslab)
expptr expr;
struct Labelblock *neglab, *zerlab, *poslab;
{
register int lm, lz, lp;

lm = neglab->labelno;
lz = zerlab->labelno;
lp = poslab->labelno;
expr = fixtype(expr);

if( ! ONEOF(expr->headblock.vtype, MSKINT|MSKREAL) )
	{
	err("invalid type of arithmetic if expression");
	frexpr(expr);
	}
else
	{
	if(lm == lz)
		exar2(OPLE, expr, lm, lp);
	else if(lm == lp)
		exar2(OPNE, expr, lm, lz);
	else if(lz == lp)
		exar2(OPGE, expr, lz, lm);
	else
		prarif(expr, lm, lz, lp);
	}
}



LOCAL exar2(op, e, l1, l2)
int op;
expptr e;
int l1, l2;
{
putif( mkexpr(op, e, ICON(0)), l2);
putgoto(l1);
}


exreturn(p)
register expptr p;
{
if(procclass != CLPROC)
	warn("RETURN statement in main or block data");
if(p && (proctype!=TYSUBR || procclass!=CLPROC) )
	{
	err("alternate return in nonsubroutine");
	p = 0;
	}

if(p)
	{
	putforce(TYINT, p);
	putgoto(retlabel);
	}
else
	putgoto(proctype==TYSUBR ? ret0label : retlabel);
}



exasgoto(labvar)
struct Hashentry *labvar;
{
register Addrp p;

p = mkplace(labvar);
if( ! ISINT(p->vtype) )
	err("assigned goto variable must be integer");
else
#if TARGET==M68K
	putassgoto( p );
#else
	putbranch(p);
#endif
}
