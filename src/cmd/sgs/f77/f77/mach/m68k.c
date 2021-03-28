/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) m68k.c: version 25.1 created on 12/2/91 at 17:38:16	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)m68k.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	m68k.c	7.2		*/

#ident	"@(#)f77/f77/mach:m68k.c	25.1"

#include "defs"

#ifdef SDB
#	include <a.out.h>
extern int types2[];
extern char * sdb_func_name;
#endif

#include "pccdefs"

#define SZCHAR 8
#define DOUBLES
#undef  ONEFLOAT
/*
	M68K - SPECIFIC ROUTINES
*/


int maxregvar = MAXREGVAR;
int regnum[] =  { 7, 6, 5, 4, 3, 2} ;
int regmask[] = { 0, 0x0080, 0x00c0, 0x00e0, 0x00f0, 0x00f8, 0x00fc };
int rsmask = 0;

extern int maxboff;	/* pcc2 auto var length */


ftnint intcon[14] =
	{ 2, 2, 2, 2,
	  15, 31, 24, 56,
	  -128, -128, 127, 127,
	  32767, 2147483647 };

#if HERE == VAX || HERE==M68K
	/* then put in constants in octal */
long realcon[6][2] =
	{
		{ 0200, 0 },
		{ 0200, 0 },
		{ 037777677777, 0 },
		{ 037777677777, 037777777777 },
		{ 032200, 0 },
		{ 022200, 0 }
	};
#else
double realcon[6] =
	{
	2.9387358771e-39,
	2.938735877055718800e-39
	1.7014117332e+38,
	1.701411834604692250e+38
	5.960464e-8,
	1.38777878078144567e-17,
	};
#endif

prlabel(fp, lk)
FILEP fp;
int lk;
{
fprintf(fp, "L%%%d:\n", lk);
}



prconi(fp, type, n)
FILEP fp;
int type;
ftnint n;
{
fprintf(fp, "\t%s\t%ld\n", (type==TYSHORT ? "short" : "long"), n); /* ken#0 */
}


prnicon(fp, type, num, n)
register FILEP fp;
int type;
register int n;
ftnint num;
{
char buf[20];

sprintf(buf, "%ld", num);
fprintf(fp, "\t%s\t%s", (type==TYSHORT ? "short" : "long"), buf); /* ken#0 */
while (--n)
	fprintf(fp, ",%s", buf);
putc('\n', fp);
}



prcona(fp, a)
FILEP fp;
ftnint a;
{
fprintf(fp, "\tlong\tL%%%ld\n", a);
}



prnacon(fp, a, n)
register FILEP fp;
ftnint a;
register int n;
{
char buf[20];

sprintf(buf, "L%%%ld", a);
fprintf(fp, "\tlong\t%s", buf);
while (--n)
	fprintf(fp, ",%s", buf);
putc('\n', fp);
}

prconr(fp, type, x)
FILEP fp;
int type;
float x;
{
fincode( fp, x, type );
fprintf( fp, "\n" );
}

prnrcon(fp, type, x, n)
register FILEP fp;
int type;
register int n;
float x;
{

while (n--)
   {
	fincode( fp, x, type );
	fprintf( fp, "\n" );
   }
}

prncxcon(fp, type, real, imag, n)
register FILEP fp;
int type;
register int n;
double real, imag;
{
char buf[50];

while (n--)
    {
	fincode( fp, real, type );
	fprintf( fp,"; " );
	fincode( fp, imag, type );
	fprintf( fp,"\n" );
    }
}


union flt_dbl
{
	float  f;
	double d;
	struct { long l,l2;	 } lg;
	struct { short slo, shi; } sh;
};

#define EXPSHIFT 7
#define	EXPMASK	(0xff << EXPSHIFT)	/* mask to access exponent on VAX */
#define EXPLORD ( 1L << EXPSHIFT )	/* low order exponent bit on VAX */
#define SIGNMASK ( 1L << 15 )		/* Vax sign bit */
fincode( fp, d, sz )
register FILEP fp;
double d;
{
	/* initialization of floats */
	union flt_dbl x;
	long z;
	long exponent,fract1,fract2,sign;

	x.d = d;
#ifdef vax
	    				/* decr. exponent by two to get IEEE
					** format
					*/
	dbltolong( &x, sz );
	if ( sz != TYREAL )
	{
	   fprintf(fp, "	long	0x%lx,0x%lx", x.lg.l, x.lg.l2);
	}
	else
	{
	   fprintf(fp, "	long	0x%lx", x.lg.l );
	}
#else		/* m68k */
#ifndef ONEFLOAT
	if ( sz != TYREAL )
	{
	   x.d = d;
	   fprintf(fp, "	long	0x%lx,0x%lx", x.lg.l, x.lg.l2);
	}
	else
#endif
	{
	   x.f = d;
	   fprintf( fp, "	long	0x%lx", x.lg.l );
	}
#endif
}

#ifdef vax
dbltolong ( x, sz )
union flt_dbl *x;
int sz;
{
	long z;
	register long exponent,fract1,fract2,sign;

	    				/* decr. exponent by two to get IEEE
					** format
					*/
	exponent = ( (x->lg.l&EXPMASK) >> EXPSHIFT ) -2;
	if ( x->d == 0e0 ) exponent=0;	/* zero exponent for 0.0 */
	sign     = ( x->lg.l&SIGNMASK ) << 16;	/* vax sign bit to high bit */
	fract1   = ( (x->lg.l&0x7F)<<16 ) | (  ( x->lg.l >> 16 )&0xFFFF );
	fract2   = ( (x->lg.l2&0xFFFF)<<16 ) | (  ( x->lg.l2 >> 16 )&0xFFFF );
#ifndef ONEFLOAT
	if ( sz != TYREAL )
	{
	   exponent = exponent + 896;	/* convert to double bias */
	   if ( x->d == 0e0 ) exponent=0;	/* zero exponent for 0.0 */
	   if ( (exponent&0x7ff) != exponent )
	     {
	     cerror("Bad convert to double");
	     }

	   x->lg.l = ( sign ) |
		    (  ( exponent&0x7ff ) << 20 ) |
		    ( (fract1 >>3 ) & 0x1fffffff );
	   x->lg.l2= ( (fract1&0x7)<<29 ) |
		    ( (fract2 >> 3) & 0x1fffffff );

	}
	else
#endif
	{
	   x->lg.l =   sign  |
		    (exponent<<23) |
		    ( fract1 );
	}
}
#endif




praddr(fp, stg, varno, offset)
FILE *fp;
int stg, varno;
ftnint offset;
{
char *memname();

if(stg == STGNULL)
	fprintf(fp, "\tlong\t0\n");
else
	{
	fprintf(fp, "\tlong\t%s", memname(stg,varno));
	if(offset)
		fprintf(fp, "+%ld", offset);
	fprintf(fp, "\n");
	}
}



/*
prtext(file)
FILEP *file;
{
fprintf(file, "#B1b.prtext\n\ttext\n");
}
*/

preven(k)
int k;
{
fprintf(asmfile, "\teven\n");
}

char *memname(stg, mem)
int stg, mem;
{
static char s[20];

switch(stg)
	{
	case STGCOMMON:
	case STGEXT:
		sprintf(s, "%s", varstr(XL, extsymtab[mem].extname) );
		break;

	case STGBSS:
	case STGINIT:
		sprintf(s, "v%%%d", mem);
		break;

	case STGCONST:
		if (mem == 0)
		/* for fixed the undefined symbol problem */
			sprintf(s, "%d", mem);
		else
			sprintf(s, "L%%%d", mem); 
		break;

	case STGEQUIV:
		sprintf(s, "q%%%d", mem+eqvstart);
		break;

	default:
		badstg("memname", stg);
	}
return(s);
}




prlocvar(s, len, align)
char *s;
ftnint len;
int align;
{
fprintf(asmfile, "\tlcomm\t%s,%ld\n", s, len);
}



prext(name, leng, init)
char *name;
ftnint leng;
int init;
{
if(leng == 0)
	fprintf(asmfile, "\tglobal\t%s\n", name);
else
	fprintf(asmfile, "\tcomm\t%s,%ld\n", name, leng);
}





prendproc()
{
}




prtail()
{
}





prolog(ep, argvec)
struct Entrypoint *ep;
Addrp  argvec;
{
int i, argslot;
int size;
register chainp p;
register Namep q;
register struct Dimblock *dp;
expptr tp, mkaddcon();

if(procclass == CLBLOCK)
	return;
putlabel( ep->backlabel );
if(argvec)
	{
	argloc = argvec->memoffset->constblock.cons.ci + SZINT;
			/* first slot holds count */
	if(proctype == TYCHAR)
		{
		mvarg(TYADDR, 0, chslot);
		mvarg(TYLENG, SZADDR, chlgslot);
		argslot = SZADDR + SZLENG;
		}
	else if( ISCOMPLEX(proctype) )
		{
		mvarg(TYADDR, 0, cxslot);
		argslot = SZADDR;
		}
	else
		argslot = 0;

	for(p = ep->arglist ; p ; p =p->nextp)
		{
		q = (Namep) (p->datap);
		mvarg(TYADDR, argslot, q->vardesc.varno);
		argslot += SZADDR;
		}
	for(p = ep->arglist ; p ; p = p->nextp)
		{
		q = (Namep) (p->datap);
		if(q->vtype==TYCHAR && q->vclass!=CLPROC)
			{
			if(q->vleng && ! ISCONST(q->vleng) )
				mvarg(TYLENG, argslot,
					q->vleng->addrblock.memno);
			argslot += SZLENG;
			}
		}
	adjustframe( argloc );
	}

for(p = ep->arglist ; p ; p = p->nextp)
	{
	q = (Namep) (p->datap);
	if(dp = q->vdim)
		{
		for(i = 0 ; i < dp->ndim ; ++i)
			if(dp->dims[i].dimexpr)
				puteq( fixtype(cpexpr(dp->dims[i].dimsize)),
					fixtype(cpexpr(dp->dims[i].dimexpr)));
		size = typesize[ q->vtype ];
		if(q->vtype == TYCHAR)
			if( ISICON(q->vleng) )
				size *= q->vleng->constblock.cons.ci;
			else
				size = -1;

		/* on VAX, get more efficient subscripting if subscripts
		   have zero-base, so fudge the argument pointers for arrays.
		   Not done if array bounds are being checked.
		*/
		if(dp->basexpr)
			puteq( 	cpexpr(fixtype(dp->baseoffset)),
				cpexpr(fixtype(dp->basexpr)));

		}
	}

if(typeaddr)
	puteq( cpexpr(typeaddr), mkaddcon(ep->typelabel) );
putgoto(ep->entrylabel);
}




prhead(fp)
FILEP fp;
{
	printf( "[%3.3d\t%8.8d\t%8.8d\t\n", procno, autoleng*SZCHAR, rsmask);
}



prdbginfo()
{
  if( sdbflag && strcmp(sdb_func_name,"BLOCK_DATA")!=0 )
	printf("\tdef\t%s;\tval\t~;\tscl\t-1;\tendef\n", sdb_func_name );
}

#ifdef SDB
prstab(name, val, stg, type)
char *name, *val;
int stg, type;
{
	char buf[BUFSIZ];

	if ((type & 0x0f) == T_ENUM)	/* got to be complex */
		sprintf(buf, "\tdef\t%s;\tval\t%s;\tscl\t%d;\ttype\t010;\ttag\tComplex;\tsize\t8;\tendef",
		    name, val, stg);
	else if ((type & 0x0f) == T_UCHAR)	/* got to be double complex */
		sprintf(buf, "\tdef\t%s;\tval\t%s;\tscl\t%d;\ttype\t010;\ttag\tDcomplex;\tsize\t16;\tendef",
		    name, val, stg);
	else
	   sprintf(buf,"\tdef\t%s;\tval\t%s;\tscl\t%d;\ttype\t0x%x;\tendef"
		, name, val, stg, type);
	p2pass(buf);
}


prarstab(name, val, stg, type, np)
char *name, *val;
int stg, type;
register Namep np;
{
	register int i, ts;
	char buf[BUFSIZ];

	sprintf(buf, "\tdef\t%s;\tval\t%s-0x%x;\tscl\t%d;\ttype\t",
	    name, val, np->vdim->baseoffset->constblock.cons.ci *
	    (ts = typesize[(int) np->vtype]), stg);
	if ((i = type & 0x0f) == T_ENUM )
		strcat(buf, "010;\ttag\tComplex;");
	else if (i == T_UCHAR)
		strcat(buf, "010;\ttag\tDcomplex;");
	else
		sprintf(buf + strlen(buf), "0x%x;", type);
	sprintf(buf + strlen(buf), "\tsize\t0x%x;\tdim\t",
	    np->vdim->nelt->constblock.cons.ci * ts);
	for (i = 0; i != np->vdim->ndim; i++)
		sprintf(buf + strlen(buf), "%d,",
		    np->vdim->dims[i].dimsize->constblock.cons.ci);
	sprintf(buf + strlen(buf) - 1, ";\tendef");
	p2pass(buf);
}


prststab(name, val, stg, type, tag, size)
char *name, *val, *tag;
int stg, type, size;
{
	char buf[BUFSIZ];

	sprintf(buf, "\tdef\t%s;\tval\t%s;\tscl\t%d;\ttype\t%d;\ttag\t%s;\tsize\t%d;\tendef",
	    name, val, stg, type, tag, size);
	p2pass(buf);
}
/*
 *
 *
 *#	ifdef UCBVAXASM
 *	char *stabdline(code, type)
 *	int code;
 *	int type;
 *	{
 *	static char buff[30];
 *
 *	sprintf(buff, "\tstabd\t0%o,0,0%o\n", code, type);
 *	return(buff);
 *	}
 *#	endif
 *
 *
 *prstab(s, code, type, loc)
 *char *s, *loc;
 *int code, type;
 *{
 *char * stabline();
 *
 *if(sdbflag)
 *	fprintf(asmfile, stabline(s,code,type,loc) );
 *}
 *
 *
 *
 *char *stabline(s, code, type, loc)
 *register char *s;
 *int code;
 *int type;
 *char *loc;
 *{
 *static char buff[50]	= "\tstab\t\t";
 *register char *t;
 *register int i = 0;
 *
 *#ifdef UCBVAXASM
 *t = buff + 8;
 *if(s == NULL)
 *	buff[6] = 'n';	/* stabn line */
/*else
 *	{
 *	buff[6] = 's';  /* stabs line */
/*	*t++ = '"';
 *	while(*s!='\0' && *s!=' ' && i<8)
 *		{
 *		*t++ = *s++;
 *		++i;
 *		}
 *	*t++ = '"';
 *	*t++ = ',';
 *	}
 *
 *#else
 *	t = buff + 7;
 *	if(s)
 *		while( *s!='\0' && *s!=' ' && i<8 )
 *			{
 *			*t++ = '\'';
 *			*t++ = *s++;
 *			*t++ = ',';
 *			++i;
 *			}
 *	for( ; i<8 ; ++i)
 *		{
 *		*t++ = '0';
 *		*t++ = ',';
 *		}
 *#endif
 *
 *
 *sprintf(t, "0%o,0,0%o,%s\n", code,  type, (loc? loc : "0") );
 *return(buff);
 *}
 *
 *
 *
 *prstleng(np, leng)
 *register Namep np;
 *ftnint leng;
 *{
 *ftnint iarrlen();
 *
 *prstab( varstr(VL,np->varname), N_LENG, 0, convic(leng) );
 *}
 *
 */
 
stabtype(p)
register Namep p;
{
register int type;
register int shift = 0;
register int i;

if (p->vtype == TYCOMPLEX)
	type = T_ENUM;			/* fake out to get Complex st tag */
else if (p->vtype == TYDCOMPLEX)
	type = T_UCHAR;			/* same for Dcomplex tag */
else
	type = types2[p->vtype];
if(p->vdim)
	{
	for (i = p->vdim->ndim, shift = (i << 1) - 2; i > 0; --i)
		type |= 0xc << (i << 1);	/* .stab code for array */
	}
else if(p->vclass == CLPROC)
	{
	type |= 040;	/* stab code for function */
	shift = 2;
	}

if(p->vstg == STGARG)
	{
	type &= ~0x30 << shift; /* clear out 1 dimension if array */
	type |= 020 << shift;	/* code for pointer-to */
	}
return(type);
}
/*
 *
 *
 *
 *prstssym(np)
 *register Namep np;
 *{
 *prstab(varstr(VL,np->varname), N_SSYM,
 *	stabtype(np), convic(np->voffset) );
 *}
 */
#endif
