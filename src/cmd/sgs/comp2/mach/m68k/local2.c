#ident	"@(#)local2.c	23.3	2/25/91 Copyright (c) 1991 by Arix Corp."
/*
    1.2	89/08/14 15:25:18 hossein
	fixed the bug related to structure assignments.

    1.1	89/06/08 00:38:54 root
	date and time created 89/06/08 00:38:54 by root

*/
/*	@(#)local2.c	7.6		*/
static	char	SCCSID[] = "@(#)local2.c	7.6	";

# include "mfile2.h"

#define RNDUP(x,y) ((x+y-1)&~(y-1))

int rsmask;
int	stle32 = 0;
int noregsave;	/* bk: if true, don't generate code to save/restore registers */

/* the stack frame looks as follows:
** 
** incoming args
** saved PC (4 bytes)
** saved old FP (%a6) (4 bytes)
** fp->
** autos (maxboff keeps track of this)
** temps (maxtemp keeps track of this)
** outgoing args (4 byte hole left for the first one )
** saved regs (the masks are M%n and FP%n)
** sp->
** 
** the first saved reg is at F%n(%fp)
** the first incoming arg is at 8(%fp)
** F%n is the size of the frame (argument to "link" instruction)
** FPO%n is the floating point register save area (negative) offset from sp
*/

lineid( l, fn )
char *fn;
{
	/* identify line l and file fn */
	printf( "%s  line %d, file %s\n", COMMENTSTR, l, fn );
}

eobl2()
{
	register off;
	register rsave;
	register i, m, n;
	int	 tfpmask,
		 fpmask,
		 fpsave;

	tfpmask = rsmask >> 16;	/* get separate fp reg mask */
	/* count bits in fpmask */
	for( fpmask=fpsave=i=0, m=1, n=0x80; i<8; ++i, m<<=1, n>>=1 )
	{
		if( m&tfpmask ) 
		{
			fpsave += 12;
			fpmask |= n;
		}
	}

	rsmask &= 0xffff;	/* remove fp regs from M68020/40 reg mask */
	/* count bits in rsmask */
	for( rsave=i=0, m=1; i<16; ++i, m<<=1 )
	{
		if( m&rsmask ) rsave += 4;
	}
	maxboff /= SZCHAR;
	maxtemp /= SZCHAR;
	maxarg /= SZCHAR;

	/* MOTOROLA 4.1 */
	/* issue optimizer frame size */
	off = maxboff + maxtemp;
	printf("	set	O%%%d,%d\n", ftnno, RNDUP(off,4) );

	off = maxboff + maxtemp + rsave + fpsave + 4;
	printf( "	set	F%%%d,%d\n", ftnno, -RNDUP(off,4) );
	if( is_m68881 )
	{
		printf( "	set	FPO%%%d,%d\n", ftnno, rsave-RNDUP(off,4) );
		printf( "	set	FPM%%%d,0x%.4x\n", ftnno, fpmask );
	}
	printf( "	set	M%%%d,0x%.4x\n", ftnno, rsmask );
	rsmask = 0;
}

char *rnames[] =
{
	/* keyed to register number tokens */
	"0", "1", "2", "3", "4", "5", "6", "7", "?8", "?9", "?10"
};

char *arnames[] =
{
	/* MOTOROLA 4.1: no more frame pointer */
	/* keyed to register number tokens */
	"%a0","%a1","%a2","%a3","%a4","%a5","%a6","%sp","%fp","?9","?10"
};

char *drnames[] =
{
	/* keyed to register number tokens */
	"%d0","%d1","%d2","%d3","%d4","%d5","%d6","%d7","?8","?9","?10"
};

char *fprnames[] =
{
	/* keyed to register number tokens */
	"%fp0","%fp1","%fp2","%fp3","%fp4","%fp5","%fp6","%fp7",
	"?8","?9","?10"
};

special( sh, p )
NODE *p;
register sh;
{
	register int val;

	sh &= SVMASK;		/* clear all but the special code */
	switch (sh)
	   {
	   case 1:		/* power of 2 */
	   case 2:		/* +/- multiple powers of 2 */

		if (p->tn.op != ICON || p->tn.name != (char *) 0)
		   return(0);
		val = p->tn.lval;
		if (sh == 1)	/* power of 2 */
		   return (val > 0 && (val & (val-1)) == 0);
		/* multiple powers of 2 (for multiply replacement) */
		if (val > 1 || val < -1)
		    return ( iconmult(val) );
		else
		    return ( 0 );
		break;

	   default:
		e2print(p);
		cerror( "special called!" );
		return( INFINITY );
	   }
}

static int	toff;

popargs( size )
register size;
{
	/* pop arguments from the stack */
	toff -= size/2;

	if(!size)
		return;

	printf( "	add.w	&%d,%%sp\n", size );
}

/* print the address of p+d */
next_address( p, d )
register NODE *p;
register int d;	/* the displacement */
{
       register NODE *ptemp,*r,*l;

       if (d) {
	  ptemp = talloc();
	  *ptemp= *p;
	  switch ( ptemp->in.op )
	  {
	    case REG:
	       ptemp->tn.rval += 1;
	       break;
	    case TEMP: 	/* temps may be in registers */
	       if (ptemp->tn.rval) 
		  goto bad;
	       /* else FALLTHRU */
	    case VAUTO:
	    case VPARAM:
	       ptemp->tn.lval += d;
	       break;
	    case NAME:
	       printf("%d+", d);	/* d+L%xx */
	       break;
	    case STAR:
	       /* if dereference of ((+/-) ANY ICON) then fold the */
	       /* displacement (d) into the ICON, else insert nodes */
	       /* to add the displacement */

	       p = p->in.left;
	       switch( p->in.op )
	       {
	       case MINUS:
		  if (p->in.right->in.op != ICON)
			goto bad;
		  d = -d;
		  /* FALL THRU */
	       case PLUS:
		  if (p->in.right->in.op == ICON)
		     {
				   /* Make copy of address tree with ICON */
				   /* modified by adding d */
		     l = ptemp->in.left = talloc();
		     *l = *p;		/* copy PLUS node */
		     r = l->in.right = talloc();
		     *r= *(p->in.right);	/* copy ICON node */
		     if ( r->in.op != ICON ) goto bad;
		     r->tn.lval += d;
		     l->in.left = cptree( p->in.left );  /* copy subtree */
		     break;

		     }
		  else if (p->in.right->in.op == REG)
		     {
				   /* Make copy of address tree with ICON */
				   /* adding d */
		     l = ptemp->in.left = talloc();
		     *l = *p;		/* copy PLUS node */
		     r = l->in.right = talloc();
		     *r = *p;
		     r->tn.op = ICON;
		     r->tn.lval = d;
		     l->in.left = talloc();

		     /* copy subtree rooted at original PLUS node */
		     l->in.left = cptree( p );
		     break;
		     }

	    	case TEMP: 
	    	case VAUTO:
	    	case VPARAM:
	    	case NAME:
			if( !( TPOINT & p->in.type ) )
				goto bad;
			/* ELSE FALL THRU */
		default:
			  /* form ( STAR (PLUS (old left subtree) ICON(4) ) */
		  l = ptemp->in.left = talloc();
		  *l = *p;		/* copy type and other info */
		  l->tn.op = PLUS;
		  l->in.left = cptree( p );	/* copy old subtree */
		  r = l->in.right = talloc();	/* node for icon */
		  *r = *p;			/* copy type and other info */
		  r->tn.op = ICON;
		  r->tn.name = NULL;
		  r->tn.lval = d;
		  break;

	       };
	       break;
	    default:
	       goto bad;
	  } /* switch */
	  adrput( ptemp );
	  tfree( ptemp );
       }
       else
	  adrput(p);
       return;
bad:
       e2print( ptemp );
       cerror("Illegal byte address (ZB)\n");
       return(-1);
}

zzzcode( p, ppc , q)
register NODE *p;
register char **ppc;
OPTAB *q;
{
	register int c;
	register int ii;
	register NODE *newp, *tnode;
	char lngth;

	static int labels[10];	/* labels for use in patterns */

	switch( c= *++(*ppc) )
	{
#ifndef	M68881
	case 'p':
		popargs( 4 );
		return;
	case 'q':
		popargs( 8 );
		return;
#endif
	case 'c':
		popargs( p->stn.argsize/SZCHAR );
		return;

	case 'F':
		/* push FLTDBL arg on to stack, adjust stack clean up value */
		if( p->in.left->tn.type == TDOUBLE )
		        /* double is 2 words plus longword case */
			toff+=2;
		/* else is longword */

	case '2':
		/* longword is one word plus word case */
		++toff;
		
	case '1':
		/* word case */
		++toff;
		printf( "-(%%sp)" );
		return;

	case '0':
		/* like 2 but doesn't print -(%%sp) */
		/* used for pea. */
		toff += 2;
		return;

				/* MOTOROLA 4.1 */
	case '3':	/* print short without leading '&' (for pea Cc) */
		newp = getadr(p, ppc);
		printf("%ld", newp->tn.lval);
		return;

	case 's':  /* like s, but needs value afterwards */
	case 'S':  /* structure assignment */
		if( p->in.op == STASG )
		{
			stas( p, p->stn.stsize, c );
		}
		else if( p->in.op == STARG )
		{
			/* store an arg onto the stack */
			star( p->in.left, p->stn.stsize ,q);
		}
		else
		{
			e2print(p);
			cerror( "ZS bad" );
		}
		break;

	case 'z':  /* adjust register on right if temporary register */
		{
			register long adj = p->stn.stsize;
			if( adj == SZSHORT || adj == SZLONG ) return;
			/* hoss#36 */
			adj = ((adj+SZSHORT)/SZLONG) - stle32;
			if( adj && (p->in.right->tn.rval < NRGS) )
			{
				printf( "\tsub.l\t&%ld,%%a%d\n",
					(SZLONG/SZCHAR)*adj,
				p->in.right->tn.rval );
			}
		}
		return;

	case 'a':
#ifdef M68881
		if ( FP_TYPE( p->in.left->bn.type ) )
		  genfpscc( p->bn.lop );
		else
#endif
		  genscc( p->bn.lop );
		return;

	case 'I':
		if(  FP_TYPE( p->bn.type ) && is_m68881 )
			fp_cbgen( p->bn.lop, p->bn.label, c);
		else
			cbgen( p->bn.lop, p->bn.label, c );
		return;

	case 'i':
		c = p->in.left->in.left->tn.rval;  /* register number */
		dbccgen( p->bn.lop, p->bn.label, c );
		return;

	case 'b':
		++*ppc;
		p = getadr(p,ppc);
		if((ii = ispow2(p->tn.lval))>=0)
		{
			printf( "&%d", ii );
			break;
		}
		cerror( "Zb but not power of 2" );

	case 'n':
		++*ppc;
		next_address( getadr(p,ppc), 4 );
		break;


	case 'e':	/* replace multiply of constant by shift/add/... */
		++*ppc;
		newp = getadr(p, ppc);
		c = newp->tn.lval;
		ii = *++(*ppc);		/* get size character */
		++*ppc;
		newp = getadr(p,ppc);		/* get source register */
		++*ppc;
		tnode = getadr(p,ppc);		/* get destination register */
		iconmulg(c, ii, newp, tnode);	/* generate the instructions */
		break;


	case 'B':	/* byte address for a bset, bclr */
		++*ppc;
		newp = getadr(p, ppc);
		if((ii = ispow2(newp->tn.lval)) < 0)
		   {
		   cerror( "ZB but not power of 2" );
		   return;
		   }
		printf("&%d,", ii & 7);	/* output the bit number (mod 8) */
		++*ppc;
		newp = getadr(p, ppc);
		if (newp->tn.type & (TLONG | TULONG))	/* calc displacement */
		   c = 3 - (ii >> 3);
		else if (newp->tn.type & (TSHORT | TUSHORT))
		   c = 1 - (ii >> 3);
		else
		   c = 0;
		next_address(newp, c);
		break;

	case 't':	/* take address of temp */
		++*ppc;
		newp = getadr(p, ppc);		/* the temp */
		++*ppc;
		p = getadr(p, ppc);		/* the result reg */
		if (newp->tn.rval == 0)		/* memory temp? */
		   {
		   printf("\n\tlea.l\t");
		   adrput(newp);
		   putchar(',');
		   adrput( p );
		   putchar('\n');
		   break;
		   }
		tnode = talloc();		/* the memory temp */
		*tnode = *newp;			/* copy stuff */
		tnode->tn.lval = freetemp(1);	/* get a memory temp */
		tnode->tn.lval = BITOOR(tnode->tn.lval);
		tnode->tn.rval = 0;		/* not in register */
		if (tnode->tn.type & (TPOINT | TLONG | TULONG | 
		    TFLOAT | TDOUBLE))
		   c = 'l';			/* move long */
		else if (tnode->tn.type & (TSHORT | TUSHORT))
		   c = 'w';			/* move word */
		else 
		   c = 'b';			/* move byte */
		printf("\tmov.%c\t", c);
		adrput(newp);
		putchar(',');
		adrput( tnode );
		printf("\n\tlea.l\t");
		adrput(tnode);
		putchar(',');
		adrput( p );
		putchar('\n');
		tfree( tnode );
		break;

	case 'L':	/* first use of label in pattern */
	case 'l':	/* other uses of label in pattern */
		ii = *++(*ppc) - '0';	/* label number */
		if (ii < 0 || ii > 9)
			cerror( "illegal %c code: %c", c, ii+'0');
		if (c == 'L')		/* define it */
			labels[ii] = getlab();
#ifdef TWOPASS
		if (labels[ii] < 0 )
			printf("L%%_%d", -labels[ii] );
	  	else
#endif
		printf("L%%%d", labels[ii]);
		break;

	case 'h':	/* print field offset for bit field instructions */
		newp = getadr( p, ppc );
		printf( "%ld", UPKFOFF( newp->tn.rval ) );
		break;
#ifdef m68k
	case 'w':	/* print float or high order part of double as const */
		newp = getadr( p, ppc);
		printf( "&0x%x", ((FLTDBL *)&newp->fp.dval)->lg.l );
		return;
	case 'x':	/* print low order part of double as const */
		newp = getadr( p, ppc);
		printf( "&0x%x", ((FLTDBL *)&newp->fp.dval)->lg.l2 );
		return;
	case 'T':	/* append M68881 instruction size */

		newp = getadr( p, ppc );
	 
		while( newp->tn.op == CONV )
			newp = newp->in.left;

		if((newp->tn.op==REG || (newp->tn.op==TEMP && newp->tn.rval))&& 
		   (FP_TYPE(newp->tn.type)))
			lngth = 'x';
		else
		{
			switch( newp->tn.type )
			{
			case TFLOAT:
				lngth = 's';
				break;
			case TDOUBLE:
				lngth = 'd';
				break;
			case TCHAR:
			case TUCHAR:
				lngth = 'b';
				break;
			case TINT:
			case TUNSIGNED:
			case TLONG:
			case TULONG:
				lngth = 'l';
				break;
			case TSHORT:
			case TUSHORT:
				lngth = 'w';
				break;
			}
		}
		printf( "%c", lngth );
		break;
	case  'N':
		newp = getadr( p, ppc );
		intr_881instr_put( newp );
		break;
#ifdef FORT
	case  'C':
		newp = getadr( p, ppc );
		intr_instr_put( newp );
		break;
#endif
#endif
	case  'o':						/* generate optimizer assist comment */
		switch (*++(*ppc))			/* for the peephole optimizer */
		{ case '0':
			printf("%sOPTF\n", COMMENTSTR);		/* flag move to float memory */
			break;
		  case '1':
			printf("%sNOPTF\n", COMMENTSTR);	/* unflag float move */
			break;
		  default:
			cerror("Zo bad op");
		}
		break;

	/*roger: used for the floating plojnt emulated instructions */
    case 'U':   /* insert M68881 operand size number in bytes */         

        newp = getadr( p, ppc );
        
        while( newp->tn.op == CONV )
            newp = newp->in.left;
            
        if((newp->tn.op==REG || (newp->tn.op==TEMP && newp->tn.rval))&&
           (FP_TYPE(newp->tn.type)))
            ii = 12;
        else
        {   
            switch( newp->tn.type )
            {
            case TFLOAT:
                ii = 4;
                break;
            case TDOUBLE:
                ii = 8;
                break;
            case TCHAR:
            case TUCHAR:
                ii = 1;
                break;
            case TINT:
            case TUNSIGNED:
            case TLONG:
            case TULONG:
                ii = 4;
                break;
            case TSHORT:
            case TUSHORT:
                ii = 2;
                break;
            }   
        }       
        printf("%d", ii);
        break;

	default:
		e2print(p);
		cerror( "illegal zzzcode: %c", c );
	}
}

star( p, sz , q)
register NODE *p;
register sz;
OPTAB *q;
{
	/* put argument onto the stack */
	/* p is a REG */

	sz /= SZCHAR;

	if( p->tn.op != REG ) uerror( "star of non-REG" );
	while( (sz -= 4) >= 0 )
	{
		printf( "\tmov.l\t%d", sz );
		expand( p, INREG, "U.,Z2\n" , q);
	}
	if( sz == -2 )
	{
		expand( p, INREG, "\tmov.w\t0U.,Z1\n" , q);
	}
	else if( sz != -4 )
	{
		e2print(p);
		cerror( "ZS sz %d", sz );
	}
}

stas( p, sz, c )
register NODE *p;
register sz,c;
{
	/* p is an STASG node; left and right are regs */
	/* all structures are aligned on int boundaries */

	register rn, ln, zz;

	stle32 = 0;
	rn = p->in.right->tn.rval;
	if( c == 's' )
	{
		/* lhs is a temp moved into a reg: kluge! */
		ln = getlr( p, '1' )->tn.rval;  /* A1 */
	}
	else ln = p->in.left->tn.rval;

	zz = (sz /= SZCHAR);
	if (zz > 32)	/* move structs > 32 bytes in loop */
		{
		register int l;
		register int regno;	/* holds regster number for count */
		
		regno = getlr( p, '1' )->tn.rval;  /* A1 */
		l = getlab();
		printf("\tmov.l\t&%d,%%d%d\n", zz/4 - 1, regno);
#ifdef TWOPASS
		if (l < 0 )
			printf("L%%_%d:\n", -l );
	  	else
#endif
		printf("L%%%d:\n", l);
		printf("\tmov.l\t(%%a%d)+,(%%a%d)+\n",  rn, ln );
		printf("\tdbra\t%%d%d,", regno);
#ifdef TWOPASS
		if (l < 0 )
			printf("L%%_%d\n", -l );
	  	else
#endif
		printf("L%%%d\n", l);
		sz -= (zz/4) * 4;
		}
	/** hoss#36 **/
	else		/* produce in-line moves */
		{
		stle32++;
		for( ; sz > 4; sz -= 4 )
			printf( "\tmov.l\t(%%a%d)+,(%%a%d)+\n", rn, ln );
			}

	if( sz == 4 ) printf( "\tmov.l\t(%%a%d),(%%a%d)\n", rn, ln );
	else if (sz == 2) printf( "\tmov.w\t(%%a%d),(%%a%d)\n", rn, ln );

	if( ln >= NRGS )
	{
		/* restore clobbered register variable */
		if( zz > sz ) printf( "\tsub.w\t&%d,%%a%d\n", zz-sz, ln);
	}
	if( rn >= NRGS )
	{
		/* restore clobbered register variable */
		if( zz > sz ) printf( "\tsub.w\t&%d,%%a%d\n", zz-sz, rn);
	}
}

conput( p )
register NODE *p;
{
	switch( p->in.op )
	{
	case ICON:
		acon( p );
		return;
	case REG:
		/* MOTOROLA 4.1 */
		if( TPOINT&p->tn.type || p->tn.lval == USE_AREG ) 
			printf( "%s", arnames[p->tn.rval] );
		else if( FP_TYPE(p->tn.type) && is_m68881 )
			printf( "%s", fprnames[p->tn.rval] );
		else 
			printf( "%s", drnames[p->tn.rval]);
		if( p->tn.rval > 7 ) cerror( "bad register output" );
		return;
	default:
		e2print(p);
		cerror( "illegal conput" );
	}
}

intr_881instr_put( p )
NODE *p;
{
	/* put out M68881 instruction for C intrinsic */
	char *intr_nm;
	int i;

	extern struct
	{   char *intr_name;    /* C intrinsic name */
	    char *fpu_inst;     /* '881 instruction */
	} intr_881decode[];

	intr_nm = p->tn.name;

	for( i = 0; *(intr_881decode[i].intr_name) != NULL; i++ )
	{
		if( strcmp( intr_nm, intr_881decode[i].intr_name ) == 0 )
		{
			printf( "%s", intr_881decode[i].fpu_inst );
			return;
		}
	}

	cerror( "illegal intrinsic function name %s", intr_nm );
}

#ifdef FORT

struct trig_dcode
{
	char *intr_name;	/* fortran intrinsic function name */
	char *fpu_instr;	/* corresponding M68881 instruction */
} intr_decode[] =	{ "r_int", "fintrz",
			  "d_int", "fintrz",
			  "r_nint", "fint",
			  "d_nint", "fint",
			  "h_nint", "fint",
			  "i_nint", "fint",
			  "h_dnnt", "fint",
			  "i_dnnt", "fint",
			  "r_abs", "fabs",
			  "d_abs", "fabs",
			  "r_sqrt", "fsqrt",
			  "d_sqrt", "fsqrt",
			  "r_exp", "fetox",
			  "d_exp", "fetox",
			  "r_log", "flogn",
			  "d_log", "flogn",
			  "r_lg10", "flog10",
			  "d_lg10", "flog10",
			  "r_sin", "fsin",
			  "d_sin","fsin",
			  "r_cos", "fcos",
			  "d_cos", "fcos",
			  "r_tan", "ftan",
			  "d_tan", "ftan",
			  "r_asin", "fasin",
			  "d_asin", "fasin",
			  "r_acos",  "facos",
			  "d_acos", "facos",
			  "r_atan", "fatan",
			  "d_atan", "fatan",
			  "r_sinh", "fsinh",
			  "d_sinh", "fsinh",
			  "r_cosh", "fcosh",
			  "d_cosh", "fcosh",
			  "r_tanh", "ftanh",
			  "d_tanh", "ftanh",
			  "",""
			  };

intr_instr_put( p )
NODE *p;
{
	/* put out M68881 instruction for fortran intrinsic */
	char *intr_nm;
	int i;

	if( p->tn.op != NAME || p->tn.name == NULL )
		cerror( "illegal intrinsic function call tree" );

	intr_nm = p->tn.name;

	for( i = 0; intr_decode[i].intr_name != NULL; i++ )
	{
		if( strcmp( intr_nm, intr_decode[i].intr_name ) == 0 )
		{
			printf( "%s", intr_decode[i].fpu_instr );
			return;
		}
	}

	cerror( "illegal intrinsic function name %s", intr_nm );
}
#endif

insput( p )
NODE *p;
{
	e2print(p);
	cerror( "insput" );
}

upput( pp )
register NODE *pp;
{
	/* output the address of the second word in the
	** pair pointed to by p (for LONGs)
	*/
	register long v;
	register NODE *r, *l, *p;

	v=0;
	p = pp;
	if( p->tn.op == PLUS )
	{
		v=0;
		r = p->in.right;
		l = p->in.left;
		if( r->tn.op == ICON &&  l->tn.op == PLUS )
		{
			v = r->tn.lval;
			p = l;
			r = p->in.right;
			l = p->in.left;
		}
		if( r->tn.op == REG )
		{
			l = p->in.left;
			if( l->tn.op == REG )
			{
				printf( "(%ld,%%a%d,%%d%d.l)", v, r->tn.rval,
					l->tn.rval );
				return;
			}
			if( l->tn.op != CONV ) goto ill;
			l = l->in.left;
			if( l->tn.op != REG ) goto ill;

/*	 MOTOROLA 4.1
 *   If the index is in an address register output 'a' instead of a 'd'
 *   This is for the case where an INT, UNSIGNED, LONG, or ULONG was 
 *   promoted to an address register.
 */
				if ( l->tn.lval == USE_AREG )
				     printf( "(%ld,%%a%d,%%a%d.l)",
						 v, r->tn.rval, l->tn.rval );
				else
				     printf( "(%ld,%%a%d,%%d%d.l)",
						 v, r->tn.rval, l->tn.rval );
			return;
		}
		if( r->tn.op != ICON ) goto ill;
		v = r->tn.lval;
		p = p->in.left;
	}
	else if( p->tn.op == MINUS )
	{
		r = p->in.right;
		if( r->tn.op != ICON || r->in.name != (char *) 0 ) goto ill;
		v = -r->tn.lval;
		p = p->in.left;
	}
	else if( p->tn.op == ASG MINUS )
	{
		r = p->in.right;
		if( r->tn.op != ICON ) goto ill;
		r = p->in.left;
		if( r->tn.op != REG ) goto ill;
		/* always do the side effect */
		else printf( "-(%s)", arnames[r->tn.rval] );
		sideff = 1;  /* cream it */
		return;
	}
	else if( p->tn.op == INCR )
	{
		r = p->in.right;
		if( r->tn.op != ICON ) goto ill;
		r = p->in.left;
		if( r->tn.op != REG ) goto ill;
		if( sideff ) printf( "(%s)+", arnames[r->tn.rval] );
		else printf( "(%s)", arnames[r->tn.rval] );
		return;
	}
	if( p->tn.op != REG ) goto ill;
	if( v )
		printf( "(%ld,%s)", v, arnames[p->tn.rval] );
	else
		printf( "(%s)", arnames[p->tn.rval] );
	return;
ill:
	e2print(pp);
	cerror( "illegal address: upput" );
}

adrput( p )
register NODE *p;
{
	/* output an address, with offsets, from p */

	while( p->in.op == FLD || p->in.op == CONV )
	{
		p = p->in.left;
	}
	switch( p->in.op )
	{

	case NAME:
		acon( p );
		sideff = 0;
		return;

#ifdef m68k
	case FCON:
		printf( "&0x%.8x", ((FLTDBL *)&p->fp.dval)->lg.l );
		if( p->in.type == TDOUBLE )
			printf( "%.8x", ((FLTDBL *)&p->fp.dval)->lg.l2 );
		sideff = 0;
		return;
#endif
	case ICON:
		/* addressable value of the constant */
		printf( "&" );
		acon( p );
		sideff = 0;
		return;

	case REG:
		conput( p );
		sideff = 0;
		return;

	case STAR:
		indiradr( p->in.left );
		return;

		/* MOTOROLA 4.1 */
	case VAUTO:
	case VPARAM:
		printf( "%ld(%%fp)", p->tn.lval );
		sideff = 0;
		return;

	case TEMP:
		if (p->tn.rval)
		   {
		   register int i = (int)p->tn.lval;

		   if( i > 16 )
			printf( "%s", fprnames[i - 16] );
		   else if( i > 8 )
			printf( "%s", arnames[i - 8] );
		   else
			printf( "%s", drnames[i] );
		   }
		else
		{
			/* MOTOROLA 4.1 */
			printf( "%ld(%%fp)", p->tn.lval );
		}
		sideff = 0;
		return;

	default:
		e2print(p);
		cerror( "illegal address" );
		return;
	}
}

acon( p )
register NODE *p;
{
	/* print out a constant */

	if( p->in.name == (char *) 0)
	{
		/* constant only */
		printf( "%ld", p->tn.lval);
	}
	else if( p->tn.lval == 0 )
	{
		/* name only */
		printf( "%s", p->in.name );
	}
	else
	{
		/* name ( + or - ) offset */
		printf( "%s", p->in.name );
		if( p->tn.lval < 0 )
			printf( "-%ld", -p->tn.lval );
		else
			printf( "+%ld", p->tn.lval );
	}
}

static char *scctbl[] =
{	"st",
	"seq",
	"sne",
	"sle",
	"slt",
	"sge",
	"sgt",
	"smi",
	"spl",
	"sls",
	"slo",
	"shs",
	"shi",
};

genscc( o )
register o;
{
	if ( o == 0 ) o = EQ -1;
	printf(scctbl[o-EQ+1]);
}

static char *fscctbl[] =
{	"fst",
	"fseq",
	"fsneq",
	"fsle",
	"fslt",
	"fsge",
	"fsgt",
	"fslt",		/* MI for fpccr */
	"fsge",		/* PL for fpccr */
	"fsls",
	"fslo",
	"fshs",
	"fshi",
};

genfpscc( o )
register o;
{
	if ( o == 0 ) o = EQ -1;
	printf(fscctbl[o-EQ+1]);
}

static char	*ccbranches[] =
{
	"	beq	",
	"	bne	",
	"	ble	",
	"	blt	",
	"	bge	",
	"	bgt	",
	"       bmi     ",
	"       bpl     ",
	"	bls	",
	"	blo	",
	"	bhs	",
	"	bhi	",
};

cbgen( o, lab, mode )
register o,lab;
{
	/* printf conditional and unconditional branches */
	if( o == 0 ) printf( "	bra	" );	/* MOTOROLA 4.1: before was "br" */
	else if( o > UGT ) cerror( "bad conditional branch: %s", opst[o] );
	else printf( ccbranches[o-EQ] );
#ifdef TWOPASS
	if (lab < 0 )
		printf("L%%_%d\n", -lab );
	  else
#endif
		printf("L%%%d\n", lab );
}


static char	*fpbranches[] =
{
	"	fbeq	",
	"	fbneq	",
	"	fble	",
	"	fblt	",
	"	fbge	",
	"	fbgt	",
	"       fblt    ", /* really fbmi */
	"       fbge    ", /* really fbpl */
};

fp_cbgen( o, lab, mode )
register o,lab;
{
	/* printf conditional and unconditional branches */
	if( o == 0 ) printf( "	bra	" );	/* MOTOROLA 4.1: before was "br" */
	else if( o > PL ) cerror( "bad floating point branch: %s", opst[o] );
	else printf( fpbranches[o-EQ] );
#ifdef TWOPASS
	if (lab < 0 )
		printf("L%%_%d\n", -lab );
	  else
#endif
		printf("L%%%d\n", lab );
}
static char	*dbccops[] =
{
	"	dbeq	%%d%d,",
	"	dbne	%%d%d,",
	"	dble	%%d%d,",
	"	dblt	%%d%d,",
	"	dbge	%%d%d,",
	"	dbgt	%%d%d,",
	"       dbmi    %%d%d,",
	"       dbpl    %%d%d,",
	"	dbls	%%d%d,",
	"	dblo	%%d%d,",
	"	dbhs	%%d%d,",
	"	dbhi	%%d%d,",
};

dbccgen( o, lab, reg )
register o,reg,lab;
{
	/* printf conditional and unconditional branches */
	if( o == 0 ) printf( "	dbt	%%d%d,", reg );
	else if( o > UGT ) cerror( "bad conditional branch: %s", opst[o] );
	else printf( dbccops[o-EQ], reg );
#ifdef TWOPASS
	if ( lab < 0 )
		printf("L%%_%d\n", -lab );
	   else
#endif
		printf("L%%%d\n", lab );
}

/* see if an unused register can be used for a temp */

#define DREGS (0xff & (0xff<<NRGS))	/* the available D registers */
#define AREGS (0x3f00 & (0xff00<<NRGS))	/* the available A registers */
#define FPREGS (0xff0000 & (0xff0000<<NRGS)) /* available FP registers */

int regtmp = 0;

tempreg(rnum, nlongs, type)
	register int *rnum;	/* returns regiser number if one allocated */
	register int nlongs;	/* number of long words needed for the temp */
	register TWORD type;	/* type of the value to be put in the temp */
	{
	register int r, rbit, mask;
	extern int regvar;

	if((!is_m68881 && nlongs != 1)|| ((type & TSTRUCT) && !(type & TPOINT)))
		return (0);	/* can't be in a register */
	mask = ~(regvar | regtmp);	/* consider all unallocated registers */

	if( FP_TYPE(type) )
		mask &= FPREGS;
	else if ((type & TPOINT) && (mask & AREGS))
		mask &= AREGS;	/* prefer pointers in A registers */
				/* but can be put in D registers if no A's */
	else
		mask &= DREGS;	/* non-pointers restricted to D registers */

	if (mask == 0)
		return(0);	/* none available! */

	for (r = 0, rbit = 1; (mask & rbit) == 0; r++)
		rbit <<= 1;
	/* mark it used */
	regtmp |= rbit;
	SETREGS(rbit);
	*rnum = r;
	return (1);	/* success! */
	}

/*
 *	BEGIN: ported from VAX pcc to make two pass version
 */

# ifdef TWOPASS

main( argc, argv ) char *argv[]; {
	return( mainp2( argc, argv ) );
	}

/*
 * code form trees.c to make twopass version 
 */


# ifndef MYLABELS
getlab()
{
	static crslab = -10;
	return( --crslab );
}
# endif

tprint( t )
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
		"?", "?"
	};

	for(;; t = DECREF(t) )
	{
		if( ISPTR(t) ) printf( "PTR " );
		else if( ISFTN(t) ) printf( "FTN " );
		else if( ISARY(t) ) printf( "ARY " );
		else 
		{
			printf( "%s", tnames[t] );
			return;
		}
	}
}
/*
 * 	Copied form common/optim to make twopass version.
 */

ispow2( c ) 
register CONSZ c; 
{
	register i;
	if( c <= 0 || (c&(c-1)) ) return(-1);
	for( i=0; c>1; ++i) c >>= 1;
	return(i);
}

deflab(lab)
	int lab;
	{
	if (lab < 0)
		printf("L%%_%d:\n", -lab);
	else
		printf("L%%%d:\n", lab);
	}

# endif


/*
 *	END: ported from VAX pcc to make two pass version
 */

#ifdef ITREES
/*
/*	copied from local.c for ITREES mode
*/

/* output the prolog code for a procedure */
doprolog(p)
register NODE *p;
{
	/*
	** code needed for M68000 to check stack growth
	** against current splimit%.  if we are about to
	** exceed current limit, ask system for more stack.
	**
	*/

	if (Arix[ARIX_PROBE]) {
	/*
	** ken#2 
	** code needed for the ARETE'--NCR stack probe
	*/
		printf("\ttst.b\tF%%%d-%d(%%sp)	#local2.c\n",p->tn.rval,STACKSAFETY);
		/* used to be F%ftnno */
	}

	/* routine prolog */

	printf( "	link.l	%%fp,&F%%%d\n", p->tn.rval );
	if ( !noregsave ) {
		printf( "	movm.l	&M%%%d,(F%%%d,%%fp)\n", 
			p->tn.rval, p->tn.rval );
		if( is_m68881 && !Arix[ARIX_RMFMOVM])	/* ken#5 */
		{
			printf( "	fmovm	&FPM%%%d,(FPO%%%d,%%fp)\n",
				p->tn.rval, p->tn.rval );
		}
	}
#ifdef	PROFILE
	/*
	** code to provide profiling via prof(1)
	**
	*/
	if( p->tn.lval & 1)	/* do profiling */
	{
		register int i;

		i = getlab();
#ifdef TWOPASS
		if (i<0)
			i = -i;
#endif
		printf("	mov.l	&L%%%c%d,-(%%sp)\n",'P',i); /* ken#8 */
		printf("	jsr	mcount%%\n");
		printf("	add.w	&4,%%sp\n");		/* ken#8 */
		printf("	data\n");
		printf("	even\n");
		printf("L%%%c%d:\n", 'P', i);
		printf("	long	0\n");
		printf("	text\n");
	}
#endif

}

/* output the epilog code for a procedure */

doepilog(p)
	register NODE *p;
	{
	extern int is_m68881u;
	int lftntype = p->tn.rval,
	    lstrtfn  = p->tn.lval;

	/* MOTOROLA 4.1 */
	/* tell optimizer type of returned value for accurate live/dead */
	if (ISPTR(lftntype) || lstrtfn != 0)
		printf("%sOPTP\n", COMMENTSTR);		/* POINTER or structure returned */
	else
	if (lftntype == FLOAT || lftntype == DOUBLE)
		printf("%sOPTX\n", COMMENTSTR);		/* FLOAT returned */
	else
	if (lftntype != TNULL)
		printf("%sOPTD\n", COMMENTSTR);		/* INTEGER returned */

		  /* copy output (in r0) to caller */
	if( lstrtfn )	/* returns structure */
	{
		  /* copy output (in r0) to caller */
		printf( "	mov.l	&__StRet,%%a0\n" );
	}

	if (ISPTR(lftntype)||lstrtfn)	/* returns pointer or structure */
	{
		/* copy A0 to D0 for more reliability in returns */
		/* from functions that return pointers.		 */
		printf("	mov.l	%%a0,%%d0\n");
	}
#ifdef M68881
	if (is_m68881u) /* Return floats/doubles both in fp0 and d0/d1 */
			/* so that old object may use this 881 code.   */
		{
		if (lftntype==FLOAT)
			printf( "	fmov.s	%%fp0,%%d0\n");
		if (lftntype==DOUBLE)
			{
			printf( "	fmov.d	%%fp0,-(%%sp)\n");
			printf( "	mov.l	(%%sp)+,%%d0\n");
			printf( "	mov.l	(%%sp)+,%%d1\n");
			}
		}
#endif
	if ( !noregsave ) {
		if( is_m68881 && !Arix[ARIX_RMFMOVM])	/* ken#5 */
		{
			printf( "	fmovm	(FPO%%%d,%%fp),&FPM%%%d\n",
				ftnno, ftnno );
		}
		printf( "	movm.l	(F%%%d,%%fp),&M%%%d\n", ftnno, ftnno );
	}
	printf( "	unlk	%%fp\n" );
	printf( "	rts\n" );
	}

/* output a switch structure passed from the front end */

doswitch(t)
register NODE *t;
{
	register int i, j, n, dlab, swlab;
	unsigned long range;            /* room for MAXINT - (-MAXINT) */
	struct sw {
	   CONSZ sval;
	   int slab;
	   } swtab[SWITSZ];
	register struct sw *p = swtab;

	n = 0;
	p[0].slab = -1;
	for (t = t->in.left; t->tn.op != SWEND; t = t->in.left)
	   if (t->in.op == SWDEF)
	      p[0].slab = t->tn.rval;
	   else if (t->in.op == SWCASE)
	      {
	      n++;
	      p[n].slab = t->tn.rval;
	      p[n].sval = t->tn.type;
	      }

	if( n == 0 )
		range = 0;
	else
	range = p[n].sval - p[1].sval;

	/* table is faster & shorter for density > 20% - JGH */
	/*    direct: overhead=2 (for br); increment: 5*n */
	/*    table:  overhead=15; increment: 1*(range+1) */

	if(range+14 <= 5*n)
	{
		 /* implement a direct switch */
		dlab = p->slab >= 0 ? p->slab : getlab();

		if( p[1].sval )
		{
#ifdef	INT16BIT
			printf( "	sub.w	&%ld,%%d0\n", p[1].sval );
#else
			printf( "	sub.l	&%ld,%%d0\n", p[1].sval );
#endif
		}

		/* note that this is a logical compare; it thus checks
		** for numbers below range as well as out of range.
		*/
#ifdef	INT16BIT
		printf( "	cmp.w	%%d0,&%ld\n", range );
#else
		printf( "	cmp.l	%%d0,&%ld\n", range );
#endif

#ifdef TWOPASS
		if (dlab < 0)
			printf( "	bhi	L%%_%d\n", -dlab );
		else
#endif
		printf( "	bhi	L%%%d\n", dlab );

/* Assume "swbeg &n" takes 4 bytes:  2 for swbeg, 2 for # of cases */
		printf( "	mov.l	(%%pc,%%d0.w*2,8.b),%%d0\n" );/*kludge, M68020 pc rel bug */
		printf( "	jmp	(%%pc,%%d0.w,6.b)\n" );
		printf(	"	swbeg	&%d\n", range+1 );
				/* range runs from zero, so add 1 for cases */
		deflab( swlab = getlab() );


		/* output table */

		for( i=1, j=p[1].sval;  i<=n;  ++j )
		{
#ifdef TWOPASS
			register result= (j == p[i].sval) ? p[i++].slab : dlab;
			if (result < 0)
			   printf( "\tshort\tL%%_%d-L%%_%d\n", -result, -swlab);
			else
			   printf( "\tshort\tL%%%d-L%%_%d\n", result, -swlab);
#else
			printf( "	short	L%%%d-L%%%d\n",
				(j == p[i].sval) ? p[i++].slab : dlab, swlab );
#endif
		}

		if( p->slab < 0 )
			deflab( dlab );
		return;
	}
	/* out for the moment
	** if( n >= 4 )
	**	werror( 1, "inefficient switch: %d, %d", n, (int) (range/n) );
	*/

	/* simple switch code */

	for( i=1; i<=n; ++i )
	{
		/* already in 0 */
		if( p[i].sval )
#ifdef	INT16BIT
			printf( "	cmp.w	%%d0,&%ld\n", p[i].sval );
		else
			printf( "	tst.w	%%d0\n" );
#else
			printf( "	cmp.l	%%d0,&%ld\n", p[i].sval );
		else
			printf( "	tst.l	%%d0\n" );
#endif

		printf( "	beq	L%%%d\n", p[i].slab );
	}

	if( p->slab >= 0 )
		printf( "	bra	L%%%d\n", p->slab ); /* MOTOROLA 4.1: before was "br" */

}
#endif

indiradr( pp )
register NODE *pp;
{
	static EADESC ea;

	int 	histflg = 0;		/* history flag,i.e. what important */
					/* operators have been scanned  */

	ea.basereg = ea.inxreg = NONE;
	ea.autoincr = ea.autodecr = ea.inxlong
	       = ea.scale = ea.dblindir = ea.inxpost = ea.basepost= 0;
	ea.idisp = ea.odisp = 0;

	bldea( pp, &ea, histflg );
	oprndadr( &ea);

	if( ea.idisp != (NODE *)0 )
		tfree( ea.idisp );

	if( ea.odisp != (NODE *)0 )
		tfree( ea.odisp );
}

bldea( pp, eap, histflg )
register  NODE   *pp;
register  EADESC *eap; 
int 		histflg;
{

	histflg = recrd_node( pp, eap, histflg );

	switch( optype( pp->in.op ) )
	{
	case BITYPE:
		bldea( pp->in.right, eap, histflg );
		/* FALL THROUGH */
	case UTYPE:
		bldea( pp->in.left, eap, histflg );
	}

}
		
NODE *
blddisp( eap, pp, histflg )
EADESC *eap;
NODE *pp;
int histflg;
{

	/* if the appropriate dispalcement ICON has been allocated */
	/* add in the new constant, otherwise, allocate the ICON   */
	/* and copy the current node */

	if( histflg & IN_INDIR )
	{
		if( eap->idisp == (NODE *)0 )
			*( eap->idisp = talloc() ) = *pp;
		else
			eap->idisp->tn.lval += pp->tn.lval;
		return( eap->idisp );
	}
	else
	{
		if( eap->odisp == (NODE *)0 )
			*( eap->odisp = talloc() ) = *pp;
		else
			eap->odisp->tn.lval += pp->tn.lval;
		return( eap->odisp );
	}
}
		
recrd_node( pp, eap, histflg )
register NODE *pp;
register EADESC *eap;
int           histflg;
{
	static char aname[10];	/* holds AUTO or TEMP offset variable name */
	static char tname[10];	/* holds AUTO or TEMP offset variable name */
	char t_strg[10];	/* temp string for name manipulations */
	int  i;
	NODE *r,
	     *q;
	

	switch( pp->in.op )
	{
	case PLUS:
		break;
	case MINUS:
		/* negate tyhe constant and change operator to PLUS */
		r = pp->in.right;
		if( r->in.op != ICON )
			cerror( "recrd_node: subtract non-ICON" );

		/* negate the constant */
		if( r->tn.lval )
			r->tn.lval = -(r->tn.lval);
		else
		{
			t_strg[0] = NULL;
			strcat( t_strg, "-" );
			strcat( t_strg, r->tn.name );
			strcpy( r->tn.name, t_strg );
		}

		pp->in.op = PLUS;
		break;
	case STAR:
		histflg |= IN_INDIR;
		eap->dblindir = TRUE;
		break;
	case LS:
		histflg |= IN_SCL;
		break;
	case UNARY AND:
		histflg |= IN_UAND;
		break;
	case REG:
		i = pp->tn.rval + 1;
		if( ( eap->basereg == NONE ) && !(histflg & IN_SCL ) &&
		    ( TPOINT & pp->tn.type ) )
		{
			eap->basereg = (i<<AREGOFF);
			if( !( histflg & IN_INDIR ) )
				eap->basepost = TRUE;
		}
		else	/* index register */
		{
			/* MOTOROLA 4.1 */
			if ((TPOINT & pp->tn.type) || 
				/* bk: Motorola code was (USE_AREG & pp->tn.lval) */
				(USE_AREG == pp->tn.lval))
			     eap->inxreg = i<<AREGOFF;
			else eap->inxreg = i;

			if (  pp->in.type&TLONG
			   || pp->in.type&TULONG
			   || pp->in.type&TPOINT
			   )
				eap->inxlong = TRUE;
			if( !( histflg & IN_INDIR ) )
				eap->inxpost = TRUE;
		}
		break;
	case ICON:
		if( histflg & IN_SCL )
		{
			eap->scale = pp->tn.lval;
			break;
		}

		/* constant is a displacement */
		blddisp( eap, pp, histflg );
		break;
	case TEMP:
		if( pp->tn.rval )
		{
			register int i = (int)pp->tn.lval;

			if( i > 16 )
			{
				eap->basereg = (i - 15) << FPREGOFF;
			}
			else if( i > 8)
				eap->basereg = (i - 7) << AREGOFF;
			else
			{
				eap->inxreg = i + 1;	/* temp in D reg*/
				if( pp->tn.type&TLONG || pp->tn.type&TULONG )
					eap->inxlong = TRUE;
			}
			break;
		};
		/* ELSE FALL THROUGH and handle as register displacement */
	case VAUTO:
	case VPARAM:
	case NAME:
		/* MOTOROLA 4.1: %fp shift changed from 7 to 9 */
		if( pp->in.op == VAUTO || pp->in.op == VPARAM )
			eap->basereg = (9<<AREGOFF); /*frame pntr is base reg */

		/* AUTOs, PARAMs, and non-reg TEMPS are treated like  */
		/* l-values so their dereference implies double indirection, */
		/* the address of (unary &) removes one level of indirection */

		if(!( histflg & IN_UAND ) )
		{
			histflg |= IN_INDIR;
			eap->dblindir = TRUE;
		}

		q = blddisp( eap, pp, histflg );

		/* if auto, param, or temp then name holds the frame area */
		/* indicator (ie. S%n or T%n ) */

		if( pp->in.op == VAUTO )
		{
			sprintf( aname, "%s%ld", "S%", ftnno );
			q->tn.name = aname;
		}
		if( pp->in.op == TEMP )
		{
			sprintf( tname, "%s%ld", "T%", ftnno );
			q->tn.name = tname;
		}
		/* seems that this case was not handled before??!! RLS */
		if( pp->in.op == VPARAM )
		{
			q->tn.name = 0;
		}
		break;
	case INCR:
		eap->autoincr = TRUE;
		break;
	case ASG MINUS:
		eap->autodecr = TRUE;
		break;
	case CONV:
		break;
	default:
		cerror( "recrd_node: illegal indirect node\n " );
	}

	return( histflg );
}


oprndadr( eap )
register EADESC *eap;
{

	if( eap->dblindir )
	{
		printf( "([" );
		if( eap->idisp != (NODE *)0 )
			putdisp( eap->idisp, 0 );

		if( eap->basereg != -1 && ( !eap->basepost ) )
		{
			if( eap->idisp != (NODE *)0 )
				printf( "," );
			putregnm( eap->basereg );
		}

		if( eap->inxreg != -1 && ( !eap->inxpost )  )
		{
			if( ( eap->idisp != (NODE *)0 ) ||
		            ( eap->basereg != -1 && ( !eap->basepost ) ) )
				printf( "," );
			putinx( eap );
		}

		printf( "]" );

		if( eap->basereg != -1 && eap->basepost )
		{
			printf( "," );
			putregnm( eap->basereg );
 			if( eap->inxreg == -1 )
 			/* A reg is index reg, must be sized */
 				printf( ".l" );
		}

		if( eap->inxreg != -1 && eap->inxpost )
		{
			printf( "," );
			putinx( eap );
		}

	      	if( eap->odisp  != (NODE *) 0 )
		{
			printf( "," );
			putdisp( eap->odisp , 0);
		}
	}
	else	/* address is not memory indirect */
	{
		if( eap->autoincr )
		{
			printf( "(" );
			putregnm( eap->basereg );
			printf( ")+" );
			return;
		}

		if( eap->autodecr )
		{
			printf( "-(" );
			putregnm( eap->basereg );
			printf( ")" );
			return;
		}

		printf( "(" );
		if( eap->basereg != -1 )
		{
			putregnm( eap->basereg );
			if( ( eap->inxreg != -1 ) || 
			    ( eap->odisp != (NODE *)0 ) )
				printf( "," );
		}
		if( eap->inxreg != -1 )
		{
			putinx( eap );
			if( eap->odisp != (NODE *)0 )
				printf( "," );
		}
	      	if( eap->odisp  != (NODE *) 0 )
			putdisp( eap->odisp , 
			 ((eap->inxreg!=-1)&&(eap->inxreg!=-1)?1:0) );
	}

	printf( ")" );
}

putinx( eap )
register EADESC *eap;
{
	int i,
	    scl;

	putregnm( eap->inxreg );
	printf( "%s", (eap->inxlong ? ".l" : ".w" ) );
	for( i = eap->scale, scl = 1; i != 0; i-- )
		scl = scl*2;
	if( eap->scale != 0 )
		printf( "*%d", scl );
}

putregnm( reg )
int reg;
{
	if( reg & FPREGMSK)
		printf( "%s", fprnames[(reg>>FPREGOFF)-1] ); /* FP register */
	else if( reg & AREGMSK)
		printf( "%s", arnames[(reg>>AREGOFF)-1] ); /* A register */
	else
		printf( "%s", drnames[reg-1] ); /* D register */
}


putdisp( p, allow_b )
register NODE *p;
int allow_b;
{
	if( p->tn.name != (char *)0 )
	{
		if( (strncmp( p->tn.name, "T%", 2 ) == 0 ) ||
	    	    (strncmp( p->tn.name, "S%", 2 ) == 0 ) )
		{
			/* MOTOROLA 4.1 */
			printf( "%ld", p->tn.lval );
		}
		else	/* name not S%n or T%n */
		{
			acon( p );
		}
	}
	else 	/* no name */
	{
		acon( p );
		if( allow_b && IS_BYTE( p->tn.lval ) )
			printf( ".b" );
		else if( IS_WORD( p->tn.lval ) )
			printf( ".w" );
		else
			printf( ".l" );
	}
			   


}

