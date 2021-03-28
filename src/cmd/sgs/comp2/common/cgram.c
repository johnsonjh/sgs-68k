static	char	SCCSID[] = "@(#) cgram.y:	2.1 83/07/08";
#ident	"@(#)cgram.y	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
# define NAME 2
# define STRING 3
# define ICON 4
# define FCON 5
# define PLUS 6
# define PLUSASG 7
# define MINUS 9
# define MINUSASG 10
# define MUL 12
# define MULASG 13
# define AND 15
# define ANDASG 16
# define OR 18
# define ORASG 19
# define ER 20
# define ERASG 21
# define QUEST 22
# define COLON 23
# define ANDAND 24
# define OROR 25
# define ASOP 26
# define RELOP 27
# define EQUOP 28
# define DIVOP 29
# define DIVOPASG 30
# define SHIFTOP 31
# define SHIFTOPASG 32
# define INCOP 33
# define UNOP 34
# define STROP 35
# define TYPE 36
# define TYPEQ 37
# define CLASS 38
# define STRUCT 39
# define RETURN 40
# define GOTO 41
# define IF 42
# define ELSE 43
# define SWITCH 44
# define BREAK 45
# define CONTINUE 46
# define WHILE 47
# define DO 48
# define FOR 49
# define DEFAULT 50
# define CASE 51
# define SIZEOF 52
# define ENUM 53
# define LP 54
# define RP 55
# define LC 56
# define RC 57
# define LB 58
# define RB 59
# define CM 60
# define SM 61
# define ASSIGN 62
# define ELLIPSIS 63
# define ASM 64
# define ARGTYPE 65
# include "mfile1.h"
# include <limits.h>				/* bk: for INT_MAX */
# include "modes.h"
unsigned short int leftcons = 1;
static int optid = -1;
int ftn_call;			/* bk */
int ftnsave;
int argspace;
int noilist;
int tentative_flag;		/* to help manage ANSI rules, 3.7.2 */
int xnfdecl;			/* helps prevents printing sdb info prematurely */
/*extern int pstk_indx;	/ helps disentagle cast from prototype grammar /
int save_stkx;			/ (same) */
int maybe_cast;         /* flag to mark possible start 
of a cast type, used by rstruct() */
int saveclass;			/* remember curclass before declspecs */
						/* in parmtype productions clobber it. */
						/* This doubles as an inside-proto flag. */
int incit;				/* related to "null_decl-> NAME LP opt_plist RP" */
extern int seen_typespec;
extern int enum_chk;
extern int vaflag;		/* LINT VARARGS comment from scanner */
/******* declaration flags *********/
int newstyle;
int tqshift;
NODE *str_type, *type_def, *arg_type;
#define DECL_RESET	typequal = curclass = indcl = seen_typespec = \
					maybe_cast = tqshift = struc_qual[sulvl] = 0
#define DECL_RESET2	typequal = curclass = seen_typespec = tqshift = 0;
#define DECL_RESET3	typequal = seen_typespec = tqshift = 0;
#define DECL_RESET4	seen_typespec = 0;
/***********************************/
char chktype[SIGNED];
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
typedef int yytabelem;
	extern NODE *ProtoDeclare(), *ProtoId(), *do_argty();
	int sulvl=0; /* counts depth of nesting of structure and
			     ** union declarations.  Used to index into
			     ** "is_struc". */
	TWORD struc_qual[16];	/* remember structure type qualifiers */
	static char is_struc[16]; /* nonzero if current STRUCT is 'struct'
				 ** and not 'union'.  Allows 15 deep nesting */
	static int fake = 0;
	static char fakename[NCHNAM+1];
# define YYERRCODE 256


NODE *
mkty( t, d, s, tq ) unsigned t; {
	return( block( TYPE, NIL, NIL, t, d, s, tq ) );
	}

NODE *
bdty( op, p, v ) NODE *p; {
	register NODE *q;

	q = block( op, p, NIL, INT, 0, INT, 0 );

	switch( op ){

	case UNARY MUL:
	case UNARY CALL:
		break;

	case LB:
		q->in.right = bcon(v);
		break;

	case NAME:
		q->tn.rval = v;
		break;

	default:
		cerror( "bad bdty" );
		}

	return( q );
	}

dstash( n ){ /* put n into the dimension table */
	if( curdim >= DIMTABSZ-1 ){
		cerror( "dimension table overflow");
		}
	dimtab[ curdim++ ] = n;
	}

savebc() {
	if( psavbc > & asavbc[BCSZ-4 ] ){
		cerror( "whiles, fors, etc. too deeply nested");
		}
	*psavbc++ = brklab;
	*psavbc++ = contlab;
	*psavbc++ = flostat;
	*psavbc++ = swx;
	flostat = 0;
	}

resetbc(mask){

	swx = *--psavbc;
	flostat = *--psavbc | (flostat&mask);
	contlab = *--psavbc;
	brklab = *--psavbc;

	}

addcase(p) NODE *p; { /* add case to switch */

	p = optim( p );  /* change enum to ints */
	if( p->in.op != ICON ){
		uerror( "non-integer constant case expression");
		return;
		}
	if( swp == swtab ){
		uerror( "case not in switch");
		return;
		}
	if( swp >= &swtab[SWITSZ] ){
		cerror( "switch table overflow");
		}
	icon_overflow( p, 1 );				/* bk: check for unsigned > INT_MAX */
	swp->sval = p->tn.lval;
	deflab( swp->slab = getlab() );
	++swp;
	tfree(p);
	}

adddef(){ /* add default case to switch */
	if( swp == swtab ){
		uerror( "default not inside switch");
		return;
		}
	if( swtab[swx].slab >= 0 ){
		uerror( "duplicate default in switch");
		return;
		}
	deflab( swtab[swx].slab = getlab() );
	}

swstart(){
	/* begin a switch block */
	if( swp >= &swtab[SWITSZ] ){
		cerror( "switch table overflow");
		}
	swx = swp - swtab;
	swp->slab = -1;
	++swp;
	}

swend(){ /* end a switch block */

	register struct sw *swbeg, *p, *q, *r, *r1;
	CONSZ temp;
	int tempi;

	swbeg = &swtab[swx+1];

	/* sort */

	r1 = swbeg;
	r = swp-1;

	while( swbeg < r ){
		/* bubble largest to end */
		for( q=swbeg; q<r; ++q ){
			if( q->sval > (q+1)->sval ){
				/* swap */
				r1 = q+1;
				temp = q->sval;
				q->sval = r1->sval;
				r1->sval = temp;
				tempi = q->slab;
				q->slab = r1->slab;
				r1->slab = tempi;
				}
			}
		r = r1;
		r1 = swbeg;
		}

	/* it is now sorted */

	for( p = swbeg+1; p<swp; ++p ){
		if( p->sval == (p-1)->sval ){
			uerror( "duplicate case in switch, %d", tempi=p->sval );
			return;
			}
		}

	reached = 1;
	genswitch( swbeg-1, (int)(swp-swbeg) );
	swp = swbeg-1;
	}



int 
decomp_type(n)
NODE * n;
{
  int ty;

  ty=n->in.type;
  switch ( ty ) {

   case ULONG:
   case UCHAR:
   case USHORT: if ( chktype[UNSIGNED -1 ] || chktype[ ty -UNSIGNED + INT -1] )
			uerror( "illegal type combination" );
		else {
			chktype[UNSIGNED -1 ]  = 1; 
			chktype[ ty - UNSIGNED + INT -1] = 1; 
			return(0);
		}
		break;
   case INT:
   case FLOAT:
   case DOUBLE:
   case LONG:
   case CHAR:
   case SHORT: if ( chktype[ ty -1 ] )
			uerror( "illegal type combination" );
		else {
			chktype[ ty -1 ]  = 1; 
			return(0);
		}
		break;
   default: return(-1);

   }
}


yytabelem yyexca[] ={
-1, 1,
	0, -1,
	2, 25,
	12, 25,
	36, 28,
	37, 28,
	38, 28,
	39, 28,
	53, 28,
	54, 25,
	61, 25,
	65, 28,
	-2, 0,
-1, 14,
	56, 13,
	-2, 114,
-1, 15,
	60, 115,
	61, 115,
	-2, 15,
-1, 17,
	36, 104,
	37, 104,
	38, 104,
	39, 104,
	53, 104,
	65, 104,
	-2, 0,
-1, 20,
	62, 111,
	-2, 113,
-1, 22,
	62, 112,
	-2, 118,
-1, 24,
	37, 41,
	-2, 39,
-1, 27,
	56, 12,
	-2, 114,
-1, 28,
	60, 115,
	61, 115,
	-2, 14,
-1, 76,
	36, 28,
	37, 28,
	38, 28,
	39, 28,
	53, 28,
	65, 28,
	-2, 21,
-1, 83,
	56, 65,
	-2, 61,
-1, 85,
	56, 55,
	-2, 53,
-1, 102,
	55, 208,
	-2, 104,
-1, 142,
	57, 69,
	61, 69,
	-2, 0,
-1, 243,
	57, 63,
	-2, 28,
-1, 262,
	55, 222,
	58, 222,
	-2, 104,
	};
# define YYNPROD 226
# define YYLAST 1329
yytabelem yyact[]={

   218,   101,   302,   104,   248,   260,    20,   131,   261,   216,
   125,   205,    55,   136,     5,   322,    59,   141,    37,   136,
   204,   323,    23,    24,   145,    61,    64,    23,    16,    24,
    36,    23,    24,    23,   133,    18,    10,    24,    13,   258,
   133,    24,   250,    24,     6,   311,    26,    50,   295,   310,
   211,   210,    50,    73,   290,    69,    50,    49,   109,    87,
    39,    43,    81,    45,   129,   137,   106,   380,   378,    57,
   123,   137,   122,   367,    19,    75,   348,    47,   130,    19,
   120,   214,   132,    19,   306,    19,    25,   148,   305,    48,
    12,   128,   149,   297,   151,   243,   163,    11,   211,   153,
   154,   155,   156,   157,   158,   159,   171,   127,   249,   257,
   212,   135,   176,   152,   256,   169,   370,   150,   114,   143,
   355,   138,   179,   111,   173,   206,   170,   148,   112,   165,
    39,    43,    38,    45,    53,   118,   121,   213,    54,    51,
   117,    67,    61,    52,    64,    68,    53,    47,   108,   132,
    54,   253,    67,   116,   359,   103,    68,   333,   333,    48,
   177,   148,   247,    51,    67,   245,   264,    52,    68,   362,
   340,   135,   325,   135,   246,   320,    24,   215,   135,    84,
    82,   265,   266,   267,   268,   269,   270,   271,   272,   273,
   274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
   284,   285,   286,   287,   288,   147,     4,   244,   206,   289,
    33,   381,   136,   263,   376,   358,   294,   357,   262,   317,
   346,   331,    24,   292,   251,   178,   303,   172,   291,   168,
   308,   298,   299,   300,   301,   119,   304,    23,   110,   107,
   371,   318,   363,   312,   338,   209,   316,    24,   315,   314,
   164,   313,   164,    70,   327,   345,   344,   332,    43,   319,
   146,   321,   328,   324,   137,   247,   247,   134,   336,   337,
    60,   335,    77,   134,   326,   334,    22,   246,   246,   255,
   143,    22,   220,    78,   309,    22,   254,    22,   186,    19,
   174,   206,    31,    85,    34,   259,   342,    83,   263,   263,
   132,   343,   341,   328,   360,   182,   375,   102,   115,    79,
    80,   105,   233,    74,   349,   350,   303,   352,   351,    32,
   207,    96,    99,    97,    98,    91,    21,   183,    90,   135,
   184,   354,   100,   186,    89,   183,    58,   356,   184,    56,
   361,   186,   328,   293,   239,    99,    97,    98,    91,   242,
   182,    90,    93,    92,    88,   140,   364,    89,   182,   166,
   185,   144,   372,    72,   373,    71,    14,    27,   303,    35,
   374,    94,    76,   126,   139,    93,    92,   379,    62,    65,
    30,    29,   229,   230,   234,     3,   238,   227,   228,   235,
   236,   237,   241,   240,    94,     2,   126,    42,    33,   217,
     9,     8,   113,   231,   162,    95,   219,   239,    99,    97,
    98,    91,   183,    40,    90,   184,    41,    88,   186,   183,
    89,   188,   184,   142,   189,   186,   190,     7,   201,   175,
   191,   192,    17,   180,   187,   182,    44,   185,    93,    92,
   180,   187,   182,    46,   185,   229,   230,   234,   226,   238,
   227,   228,   235,   236,   237,   241,   240,    94,   224,   126,
   225,    33,   223,   221,   183,   195,   231,   184,   196,   219,
   186,   193,   222,   188,   198,     1,   189,   199,   190,   200,
   201,   330,   191,   192,   202,   180,   187,   182,   194,   185,
   197,   183,     0,     0,   184,     0,     0,   186,     0,     0,
   188,     0,     0,   189,     0,   190,     0,     0,     0,   191,
     0,     0,   180,   187,   182,     0,   185,   329,   181,     0,
   203,   183,   195,     0,   184,   196,     0,   186,   193,     0,
   188,   198,     0,   189,   199,   190,   200,   201,     0,   191,
   192,   202,   180,   187,   182,   194,   185,   197,   167,     0,
     0,   183,     0,     0,   184,    15,    28,   186,     0,     0,
   188,     0,     0,   189,     0,   190,     0,    63,    66,     0,
   377,     0,   180,   187,   182,   181,   185,   203,     0,     0,
     0,   183,   195,     0,   184,   196,     0,   186,   193,     0,
   188,   198,     0,   189,   199,   190,   200,   201,   232,   191,
   192,   202,   180,   187,   182,   194,   185,   197,   183,     0,
     0,   184,     0,     0,   186,     0,     0,   188,   183,     0,
     0,   184,   190,     0,   186,     0,     0,   188,     0,   180,
   187,   182,     0,   185,   369,   181,     0,   203,     0,   180,
   187,   182,     0,   185,   183,   195,     0,   184,   196,     0,
   186,   193,     0,   188,   198,     0,   189,   199,   190,   200,
   201,   232,   191,   192,   202,   180,   187,   182,   194,   185,
   197,   183,     0,     0,   184,     0,     0,   186,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   180,   368,   182,     0,   185,     0,   181,     0,
   203,   183,   195,     0,   184,   196,     0,   186,   193,     0,
   188,   198,     0,   189,   199,   190,   200,   201,     0,   191,
   192,   202,   180,   187,   182,   194,   185,   197,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   366,     0,     0,     0,     0,   181,     0,   203,   183,   195,
     0,   184,   196,     0,   186,   193,     0,   188,   198,     0,
   189,   199,   190,   200,   201,     0,   191,   192,   202,   180,
   187,   182,   194,   185,   197,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   365,     0,     0,
     0,     0,   181,     0,   203,   183,   195,     0,   184,   196,
     0,   186,   193,     0,   188,   198,     0,   189,   199,   190,
   200,   201,   353,   191,   192,   202,   180,   187,   182,   194,
   185,   197,   183,   195,     0,   184,   196,     0,   186,   193,
     0,   188,   198,     0,   189,   199,   190,   200,   201,     0,
   191,   192,   202,   180,   187,   182,   194,   185,   197,   181,
     0,   203,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   181,   347,   203,   183,
   195,     0,   184,   196,     0,   186,   193,     0,   188,   198,
     0,   189,   199,   190,   200,   201,   339,   191,   192,   202,
   180,   187,   182,   194,   185,   197,   183,   195,     0,   184,
   196,     0,   186,   193,     0,   188,   198,     0,   189,   199,
   190,   200,   201,     0,   191,   192,   202,   180,   187,   182,
   194,   185,   197,   181,     0,   203,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   181,   296,   203,   183,   195,     0,   184,   196,     0,   186,
   193,     0,   188,   198,     0,   189,   199,   190,   200,   201,
     0,   191,   192,   202,   180,   187,   182,   194,   185,   197,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   252,     0,     0,     0,     0,   181,     0,   203,
   183,   195,     0,   184,   196,     0,   186,   193,     0,   188,
   198,     0,   189,   199,   190,   200,   201,     0,   191,   192,
   202,   180,   187,   182,   194,   185,   197,   183,   195,     0,
   184,   196,     0,   186,   193,     0,   188,   198,     0,   189,
   199,   190,   200,   201,     0,   191,   192,   202,   180,   187,
   182,   194,   185,   197,   181,     0,   203,    96,    99,    97,
    98,    91,     0,     0,    90,     0,     0,    88,     0,     0,
    89,    96,    99,    97,    98,    91,     0,     0,    90,     0,
     0,    88,     0,   203,    89,     0,     0,     0,    93,    92,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    93,    92,     0,     0,     0,    94,     0,   126,
     0,     0,     0,     0,     0,     0,   307,    96,    99,    97,
    98,    94,     0,   126,     0,   208,    96,    99,    97,    98,
    91,     0,     0,    90,     0,     0,    88,     0,     0,    89,
    96,    99,    97,    98,    91,     0,     0,    90,    93,    92,
    88,     0,     0,    89,     0,     0,     0,    93,    92,     0,
     0,     0,     0,     0,     0,     0,     0,    94,     0,   126,
     0,    93,    92,     0,     0,     0,    94,     0,   126,     0,
   124,    96,    99,    97,    98,    91,     0,     0,    90,     0,
    94,    88,   126,   161,    89,    96,    99,    97,    98,    91,
     0,     0,    90,     0,     0,    88,     0,     0,    89,     0,
     0,     0,    93,    92,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    93,    92,     0,     0,
     0,    94,     0,   126,     0,     0,    96,    99,    97,    98,
    91,     0,     0,    90,     0,    94,    88,   160,     0,    89,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    93,    92,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    94,     0,    86 };
yytabelem yypact[]={

 -1000,   -50, -1000, -1000, -1000,    36,    29,    25, -1000,   154,
    94, -1000, -1000,    -4,   109,    92, -1000,    14,   235,   235,
   110,    -7, -1000,   199, -1000, -1000,    -8,   109,    92, -1000,
   154, -1000, -1000, -1000, -1000,    94,    94,    94, -1000, -1000,
 -1000, -1000, -1000,   221,   124,   295,   123,   291,  1274, -1000,
 -1000, -1000,    96,   184,    89,   183,    68, -1000, -1000, -1000,
 -1000,   110,   109,    92,    98,    85,    80,   180,    77,  1164,
 -1000, -1000,   221, -1000, -1000,    17, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000,   258, -1000,  1219,    59,  1219,  1219,
  1219,  1219,  1219,  1219,  1233,  1178,   198, -1000, -1000, -1000,
    31,   174, -1000, -1000,    67,  1219,   172, -1000, -1000,    65,
 -1000, -1000,   288,    52, -1000, -1000, -1000, -1000, -1000, -1000,
   170, -1000,    63,  1061,  1109,    59,  1219, -1000, -1000,   -10,
 -1000, -1000,    87, -1000, -1000,   210, -1000,   210,    20,   342,
    34, -1000,    11,    24,    48, -1000,   -20,   169,   977, -1000,
  1219,   284, -1000,    59,    59,    59,    59,    59,    59,    59,
  1219, -1000,    54,  1061, -1000, -1000,   109,    92, -1000, -1000,
 -1000,  1061, -1000, -1000, -1000, -1000,   -24,   164, -1000, -1000,
  1219,  1219,  1219,  1219,  1219,  1219,  1219,  1219,  1219,  1219,
  1219,  1219,  1219,  1219,  1219,  1219,  1219,  1219,  1219,  1219,
  1219,  1219,  1219,  1219,    -6, -1000,  1061,  1109, -1000,   168,
 -1000, -1000, -1000, -1000, -1000,   -13, -1000, -1000,   920,    32,
 -1000,   405,   405,   405,   405,  1219,   405,    27,    23,  1095,
   282, -1000,   -12,   405,   197,   195, -1000,   194,   192,   196,
  1219,   236,   118, -1000,    38, -1000,    24,    24,   115,   258,
 -1000,   319, -1000,   458, -1000,   166,  1219, -1000, -1000, -1000,
   100, -1000,   164,   164,   190,   329,  1061, -1000,   276,   276,
   321, -1000,   665,   413,   602,   612,   545,   485,  1061,  1061,
  1061,  1061,  1061,  1061,  1061,  1061,   893,  1061,  1061,   113,
  1109,    -6,  1219,    11, -1000, -1000, -1000, -1000,   213, -1000,
 -1000,   208,   165,  1034, -1000, -1000, -1000, -1000,   836,    15,
 -1000, -1000, -1000,  1219,  1219,  1219,  1219, -1000,   809, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,    59, -1000,
  1219,  1155,  1061,    61,   162,   160,    99,   100, -1000,  1219,
 -1000, -1000,   112, -1000, -1000,   188,   405, -1000, -1000,   752,
   695,    12,   638, -1000,   575, -1000,    57, -1000, -1000,   186,
 -1000,   406, -1000,  1219, -1000, -1000, -1000,  1219, -1000, -1000,
 -1000, -1000,   159,   515,     7, -1000, -1000,     6, -1000,   156,
 -1000, -1000 };
yytabelem yypgo[]={

     0,   475,     3,   472,   463,   462,   460,   458,   448,   443,
   436,   432,   429,     8,   283,    30,   272,    18,    21,    15,
     0,     2,    10,    44,   427,   423,   416,   413,   118,     5,
   405,     7,   548,     6,   404,    12,     1,   402,   401,   400,
   359,   397,   395,   385,    38,   381,   282,   380,    64,   374,
     9,   372,    36,   369,   365,   363,    35,   361,     4,    24,
   355,   349,    17,   343,   339,   336,    28,   332,   326,    20,
    11,   320,   319,   312,   311,   308,   307,   306,   304 };
yytabelem yyr1[]={

     0,     1,     1,    42,    42,    42,    43,    43,    43,    43,
    43,    43,    39,    39,    38,    38,    45,    47,    47,    47,
    49,    49,    51,    51,    51,    24,    23,    25,    52,    14,
    14,    14,    16,    16,    18,    18,    19,    19,    53,    54,
    54,    55,    17,    17,    15,    15,    15,    15,    41,    41,
    56,    56,    26,    26,     9,     9,    57,    57,    59,    59,
    27,    27,    61,    61,    10,    10,    60,    60,    62,    62,
    48,    63,    48,    31,    31,    31,    31,    33,    33,    33,
    33,    33,    33,    33,    40,    40,    40,    40,    40,    40,
    40,    32,    32,    32,    32,    32,    32,    32,    12,    12,
    11,    35,    37,    37,    65,    64,    64,    64,    44,    67,
    44,    68,    68,    66,    66,    66,    66,    66,    66,    69,
    69,    70,    70,    58,    58,    71,    46,    72,    50,    50,
    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
    50,    50,    50,    50,    50,    50,    73,    73,    73,     7,
     4,     3,     5,     6,     8,    74,     2,    21,    21,    34,
    34,    20,    20,    20,    20,    20,    20,    20,    20,    20,
    20,    20,    20,    20,    20,    20,    20,    20,    20,    20,
    20,    20,    20,    20,    20,    20,    22,    22,    22,    22,
    22,    22,    22,    22,    22,    22,    22,    22,    22,    22,
    22,    22,    22,    22,    22,    22,    28,    75,    36,    36,
    29,    29,    29,    77,    29,    29,    29,    29,    78,    29,
    29,    13,    13,    76,    30,    30 };
yytabelem yyr2[]={

     0,     5,     1,     3,     3,     5,     5,     5,     7,     7,
     5,     5,     5,     5,     5,     5,     5,     9,     7,     1,
     5,     1,     7,     9,     0,     1,     5,     5,     1,     5,
     5,     5,     1,     2,     4,     5,     1,     2,     3,     0,
     4,     1,     3,     5,     3,     3,     3,     3,     9,     5,
     4,     6,    11,     5,     3,     5,     2,     6,     3,     7,
    11,     5,     1,     2,     3,     5,     2,     6,     5,     3,
     3,     1,     9,     2,     7,     5,     3,     5,     7,     9,
     7,     9,     3,     7,     9,     7,     5,     9,     7,     9,
     7,     5,     7,     7,     9,     7,     7,     5,     4,     1,
     5,     7,     3,     7,     1,     3,     7,     2,     2,     1,
     8,     3,     2,     3,     3,     3,     7,    13,     2,     2,
     6,     3,     9,     0,     2,     3,     9,     3,     5,     5,
     2,     5,     5,     5,    15,     9,     5,     5,     5,     5,
     7,     7,     2,     4,     4,     4,     5,     7,     5,     3,
     9,     7,     9,    13,     9,     1,     5,     2,     1,     2,
     7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
     7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
     7,     7,    11,     7,     7,     2,     5,     5,     5,     5,
     5,     5,     5,     5,     9,     9,     9,    13,     5,     7,
     7,     3,     3,     3,     3,     7,     7,     1,     3,     5,
     3,     7,     7,     1,    15,     5,     7,     9,     1,    11,
     7,     3,     1,     1,     5,     5 };
yytabelem yychk[]={

 -1000,    -1,   -42,   -43,   256,    64,   -23,   -24,   -38,   -39,
   -52,    61,    61,   -44,   -40,   -32,   -66,   -11,   -56,    54,
   -33,   -68,   256,     2,    12,    61,   -44,   -40,   -32,   -45,
   -47,   -46,   -72,    56,   -14,   -53,   -15,   -17,    38,    36,
   -27,   -26,   -41,    37,   -10,    39,    -9,    53,    65,    61,
    60,    54,    58,    54,    58,   -35,   -64,    55,   -65,     2,
   256,   -33,   -40,   -32,   -33,   -40,   -32,    54,    58,    62,
    54,   -54,   -55,    61,   -46,   -23,   -51,   -16,   -14,   -16,
   -16,   -17,    56,     2,    56,     2,    54,   -22,    12,    15,
     9,     6,    34,    33,    52,   -30,     2,     4,     5,     3,
   -67,   -36,   -76,    59,    -2,   -74,   -35,    55,    59,    -2,
    55,    55,    60,   -37,   -28,   -75,    55,    55,    55,    55,
   -35,    59,    -2,   -20,    56,   -22,    54,   -56,   -17,   -48,
    61,   -31,   -33,    23,   256,   -56,     2,    54,   -23,   -49,
   -60,   -62,   -25,   -52,   -57,   -59,     2,   -28,   -20,    33,
    58,    35,    54,   -22,   -22,   -22,   -22,   -22,   -22,   -22,
    54,    55,   -34,   -20,    54,   -66,   -40,   -32,    55,   -35,
    59,   -20,    55,    59,     2,   -12,    60,   -23,    55,    59,
    27,    60,    29,     6,     9,    31,    12,    28,    15,    18,
    20,    24,    25,    13,    30,     7,    10,    32,    16,    19,
    21,    22,    26,    62,   -69,   -70,   -20,   -71,    56,   -28,
    61,    60,    23,    -2,    61,   -44,   -50,    57,   -20,    64,
   -46,    -4,    -3,    -5,    -7,    -6,    -8,    45,    46,    40,
    41,    61,   256,   -73,    42,    47,    48,    49,    44,     2,
    51,    50,   -61,    61,   -48,   -18,   -15,   -17,   -58,    60,
    62,    55,    55,   -20,     2,   -28,    60,    55,    63,   -28,
   -29,   -13,    54,   -56,     2,   -20,   -20,   -20,   -20,   -20,
   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,
   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,   -58,
    60,   -69,    55,   -63,    -2,    61,    61,    61,   -50,   -50,
   -50,   -50,   -21,   -20,   -50,    61,    61,    61,   -20,     2,
    61,    57,   -50,    54,    54,    54,    54,    23,   -20,    23,
    57,   -62,   -19,   -18,   -19,    57,   -59,    -2,   -22,    59,
    23,    55,   -20,    58,   -35,   -13,   -29,   -29,    54,    23,
    57,   -70,   -58,   -31,    43,    47,    55,    61,    61,   -20,
   -20,   -21,   -20,    23,   -20,    59,    -2,    55,    55,    55,
   -78,   -20,    57,    54,   -50,    55,    55,    61,    55,    59,
    59,    54,   -36,   -20,   -21,   -77,    55,    55,    61,   -36,
    61,    55 };
yytabelem yydef[]={

     2,    -2,     1,     3,     4,     0,     0,     0,    19,     0,
     0,     5,     6,     0,    -2,    -2,   108,    -2,     0,     0,
    -2,     0,    -2,    82,    -2,     7,     0,    -2,    -2,    10,
    28,    11,    24,   127,    26,    32,    32,    32,    38,    44,
    45,    46,    47,    42,     0,    64,     0,    54,     0,     8,
   109,   223,   155,   104,   155,     0,     0,    97,   207,   105,
   107,    77,    86,    91,     0,     0,     0,   104,   155,     0,
   100,    50,     0,     9,    16,     0,    -2,    29,    33,    30,
    31,    43,    28,    -2,     0,    -2,   207,    49,     0,     0,
     0,     0,     0,     0,     0,     0,   201,   202,   203,   204,
     0,     0,    -2,    88,     0,     0,     0,    92,    93,     0,
    85,    96,     0,    99,   102,    28,    83,    90,    95,    78,
     0,    80,     0,   116,     0,   185,   207,    51,    40,     0,
    18,    70,    73,   155,    76,     0,    82,     0,     0,     0,
    62,    66,    -2,     0,   123,    56,    58,     0,     0,   186,
     0,     0,   225,   187,   188,   189,   190,   191,   192,   193,
   207,   198,     0,   159,   224,   110,   114,   115,    87,   209,
    89,   156,    84,    94,   106,   101,   207,   222,    79,    81,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   123,   119,   121,     0,   125,     0,
    17,    71,   155,    75,    22,     0,    20,   126,     0,     0,
   130,     0,     0,     0,     0,   158,     0,     0,     0,     0,
     0,   142,     0,     0,     0,     0,   149,     0,     0,   201,
     0,     0,     0,    -2,    68,    27,    36,    36,     0,   124,
   155,    48,   205,     0,   200,     0,     0,   199,    98,   103,
   206,   210,    -2,   222,   221,   161,   162,   163,   164,   165,
   166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
   176,   177,   178,   179,   180,   181,     0,   183,   184,     0,
   124,   123,     0,     0,    74,    23,   128,   129,   131,   132,
   133,     0,     0,   157,   136,   137,   138,   139,     0,     0,
   143,   144,   145,     0,     0,   158,     0,   146,     0,   148,
    60,    67,    34,    37,    35,    52,    57,    59,   194,   196,
     0,   195,   160,   155,     0,   210,     0,   215,   218,     0,
   117,   120,     0,    72,   151,     0,     0,   140,   141,     0,
     0,     0,     0,   147,     0,   216,     0,   211,   212,   220,
   223,   182,   122,     0,   135,   150,   152,   158,   154,   197,
   217,   213,     0,     0,     0,   223,   219,     0,   153,     0,
   134,   214 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"NAME",	2,
	"STRING",	3,
	"ICON",	4,
	"FCON",	5,
	"PLUS",	6,
	"PLUSASG",	7,
	"MINUS",	9,
	"MINUSASG",	10,
	"MUL",	12,
	"MULASG",	13,
	"AND",	15,
	"ANDASG",	16,
	"OR",	18,
	"ORASG",	19,
	"ER",	20,
	"ERASG",	21,
	"QUEST",	22,
	"COLON",	23,
	"ANDAND",	24,
	"OROR",	25,
	"ASOP",	26,
	"RELOP",	27,
	"EQUOP",	28,
	"DIVOP",	29,
	"DIVOPASG",	30,
	"SHIFTOP",	31,
	"SHIFTOPASG",	32,
	"INCOP",	33,
	"UNOP",	34,
	"STROP",	35,
	"TYPE",	36,
	"TYPEQ",	37,
	"CLASS",	38,
	"STRUCT",	39,
	"RETURN",	40,
	"GOTO",	41,
	"IF",	42,
	"ELSE",	43,
	"SWITCH",	44,
	"BREAK",	45,
	"CONTINUE",	46,
	"WHILE",	47,
	"DO",	48,
	"FOR",	49,
	"DEFAULT",	50,
	"CASE",	51,
	"SIZEOF",	52,
	"ENUM",	53,
	"LP",	54,
	"RP",	55,
	"LC",	56,
	"RC",	57,
	"LB",	58,
	"RB",	59,
	"CM",	60,
	"SM",	61,
	"ASSIGN",	62,
	"ELLIPSIS",	63,
	"ASM",	64,
	"ARGTYPE",	65,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"ext_def_list : ext_def_list external_def",
	"ext_def_list : /* empty */",
	"external_def : data_def",
	"external_def : error",
	"external_def : ASM SM",
	"data_def : declspecs SM",
	"data_def : odeclspecs SM",
	"data_def : declspecs init_dcl_list SM",
	"data_def : odeclspecs init_dcl_list SM",
	"data_def : fdef_pref function_body",
	"data_def : fdef_newpref compoundstmt",
	"fdef_newpref : odeclspecs new_style",
	"fdef_newpref : declspecs new_style",
	"fdef_pref : odeclspecs fdeclarator",
	"fdef_pref : declspecs fdeclarator",
	"function_body : arg_dcl_list compoundstmt",
	"arg_dcl_list : arg_dcl_list declspecs declarator_list SM",
	"arg_dcl_list : arg_dcl_list declspecs SM",
	"arg_dcl_list : /* empty */",
	"stmt_list : stmt_list statement",
	"stmt_list : /* empty */",
	"dcl_stat_list : dcl_stat_list declspecs SM",
	"dcl_stat_list : dcl_stat_list declspecs init_dcl_list SM",
	"dcl_stat_list : /* empty */",
	"odeclspecs : /* empty */",
	"declspecs : decl_init bldspecs",
	"structspecs : decl_init specqual",
	"decl_init : /* empty */",
	"bldspecs : class obldspecs",
	"bldspecs : type obldspecs",
	"bldspecs : typequal_list obldspecs",
	"obldspecs : /* empty */",
	"obldspecs : bldspecs",
	"specqual : type ospecqual",
	"specqual : typequal_list ospecqual",
	"ospecqual : /* empty */",
	"ospecqual : specqual",
	"class : CLASS",
	"otypequal_list : /* empty */",
	"otypequal_list : tq_shift typequal_list",
	"tq_shift : /* empty */",
	"typequal_list : TYPEQ",
	"typequal_list : TYPEQ typequal_list",
	"type : TYPE",
	"type : struct_dcl",
	"type : enum_dcl",
	"type : argtype",
	"argtype : ARGTYPE LP cast_type RP",
	"argtype : ARGTYPE term",
	"pointer : MUL otypequal_list",
	"pointer : MUL otypequal_list pointer",
	"enum_dcl : enum_head LC moe_list optcomma RC",
	"enum_dcl : ENUM NAME",
	"enum_head : ENUM",
	"enum_head : ENUM NAME",
	"moe_list : moe",
	"moe_list : moe_list CM moe",
	"moe : NAME",
	"moe : NAME ASSIGN con_e",
	"struct_dcl : str_head LC type_dcl_list optsemi RC",
	"struct_dcl : STRUCT NAME",
	"optsemi : /* empty */",
	"optsemi : SM",
	"str_head : STRUCT",
	"str_head : STRUCT NAME",
	"type_dcl_list : type_declaration",
	"type_dcl_list : type_dcl_list SM type_declaration",
	"type_declaration : structspecs declarator_list",
	"type_declaration : structspecs",
	"declarator_list : declarator",
	"declarator_list : declarator_list CM",
	"declarator_list : declarator_list CM declarator",
	"declarator : nfdeclarator",
	"declarator : nfdeclarator COLON con_e",
	"declarator : COLON con_e",
	"declarator : error",
	"nfdeclarator : pointer nfdeclarator",
	"nfdeclarator : nfdeclarator LP RP",
	"nfdeclarator : nfdeclarator LP parmtype_list RP",
	"nfdeclarator : nfdeclarator LB RB",
	"nfdeclarator : nfdeclarator LB con_e RB",
	"nfdeclarator : NAME",
	"nfdeclarator : LP nfdeclarator RP",
	"new_style : fdeclarator LP parmtype_list RP",
	"new_style : name_lp parmtype_list RP",
	"new_style : pointer new_style",
	"new_style : new_style LP opt_parmtype_list RP",
	"new_style : new_style LB RB",
	"new_style : new_style LB con_e RB",
	"new_style : LP new_style RP",
	"fdeclarator : pointer fdeclarator",
	"fdeclarator : fdeclarator LP RP",
	"fdeclarator : fdeclarator LB RB",
	"fdeclarator : fdeclarator LB con_e RB",
	"fdeclarator : LP fdeclarator RP",
	"fdeclarator : name_lp name_list RP",
	"fdeclarator : name_lp RP",
	"optellipse : CM ELLIPSIS",
	"optellipse : /* empty */",
	"name_lp : NAME LP",
	"parmtype_list : proto_init parmtypes optellipse",
	"parmtypes : cast_type",
	"parmtypes : parmtypes CM cast_type",
	"proto_init : /* empty */",
	"name_list : NAME",
	"name_list : name_list CM NAME",
	"name_list : error",
	"init_dcl_list : init_declarator",
	"init_dcl_list : init_dcl_list CM",
	"init_dcl_list : init_dcl_list CM init_declarator",
	"xnfdeclarator : nfdeclarator",
	"xnfdeclarator : error",
	"init_declarator : nfdeclarator",
	"init_declarator : new_style",
	"init_declarator : fdeclarator",
	"init_declarator : xnfdeclarator ASSIGN e",
	"init_declarator : xnfdeclarator ASSIGN LC init_list optcomma RC",
	"init_declarator : error",
	"init_list : initializer",
	"init_list : init_list CM initializer",
	"initializer : e",
	"initializer : ibrace init_list optcomma RC",
	"optcomma : /* empty */",
	"optcomma : CM",
	"ibrace : LC",
	"compoundstmt : begin dcl_stat_list stmt_list RC",
	"begin : LC",
	"statement : e SM",
	"statement : ASM SM",
	"statement : compoundstmt",
	"statement : ifprefix statement",
	"statement : ifelprefix statement",
	"statement : whprefix statement",
	"statement : doprefix statement WHILE LP e RP SM",
	"statement : forprefix .e RP statement",
	"statement : switchpart statement",
	"statement : BREAK SM",
	"statement : CONTINUE SM",
	"statement : RETURN SM",
	"statement : RETURN e SM",
	"statement : GOTO NAME SM",
	"statement : SM",
	"statement : error SM",
	"statement : error RC",
	"statement : label statement",
	"label : NAME COLON",
	"label : CASE e COLON",
	"label : DEFAULT COLON",
	"doprefix : DO",
	"ifprefix : IF LP e RP",
	"ifelprefix : ifprefix statement ELSE",
	"whprefix : WHILE LP e RP",
	"forprefix : FOR LP .e SM .e SM",
	"switchpart : SWITCH LP e RP",
	"con_e : /* empty */",
	"con_e : e",
	".e : e",
	".e : /* empty */",
	"elist : e",
	"elist : elist CM e",
	"e : e RELOP e",
	"e : e CM e",
	"e : e DIVOP e",
	"e : e PLUS e",
	"e : e MINUS e",
	"e : e SHIFTOP e",
	"e : e MUL e",
	"e : e EQUOP e",
	"e : e AND e",
	"e : e OR e",
	"e : e ER e",
	"e : e ANDAND e",
	"e : e OROR e",
	"e : e MULASG e",
	"e : e DIVOPASG e",
	"e : e PLUSASG e",
	"e : e MINUSASG e",
	"e : e SHIFTOPASG e",
	"e : e ANDASG e",
	"e : e ORASG e",
	"e : e ERASG e",
	"e : e QUEST e COLON e",
	"e : e ASOP e",
	"e : e ASSIGN e",
	"e : term",
	"term : term INCOP",
	"term : MUL term",
	"term : AND term",
	"term : MINUS term",
	"term : PLUS term",
	"term : UNOP term",
	"term : INCOP term",
	"term : SIZEOF term",
	"term : LP cast_type RP term",
	"term : SIZEOF LP cast_type RP",
	"term : term LB e RB",
	"term : term LB e COLON e RB",
	"term : funct_idn RP",
	"term : funct_idn elist RP",
	"term : term STROP NAME",
	"term : NAME",
	"term : ICON",
	"term : FCON",
	"term : STRING",
	"term : LP e RP",
	"cast_type : SaveClass declspecs null_decl",
	"SaveClass : /* empty */",
	"opt_parmtype_list : proto_push",
	"opt_parmtype_list : proto_push parmtype_list",
	"null_decl : optname",
	"null_decl : LP parmtype_list RP",
	"null_decl : LP optname RP",
	"null_decl : LP null_decl RP LP",
	"null_decl : LP null_decl RP LP opt_parmtype_list RP",
	"null_decl : pointer null_decl",
	"null_decl : null_decl LB RB",
	"null_decl : null_decl LB con_e RB",
	"null_decl : NAME LP",
	"null_decl : NAME LP opt_parmtype_list RP",
	"null_decl : LP null_decl RP",
	"optname : NAME",
	"optname : /* empty */",
	"proto_push : /* empty */",
	"funct_idn : NAME LP",
	"funct_idn : term LP",
};
#endif /* YYDEBUG */
#ident	"@(#)yaccpar	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
	


/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-1000)

/*
** global variables used by the parser
*/
YYSTYPE yyv[ YYMAXDEPTH ];	/* value stack */
int yys[ YYMAXDEPTH ];		/* state stack */

YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
int
yyparse()
{
	register YYSTYPE *yypvt;	/* top of value stack for $vars */

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

	goto yystack;
	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ YYMAXDEPTH ] )	/* room on stack? */
		{
			yyerror( "yacc stack overflow" );
			YYABORT;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
				yynerrs++;
			skip_init:
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 1:{
				extern int null_input;
				null_input = 0;
			} break;
case 2:{
#ifndef LINT
				bg_file();
#endif
				ftnend();
			} break;
case 3:{ blevel = 0; DECL_RESET; vaflag = 0; argno = 0;} break;
case 4:{ blevel = 0; DECL_RESET; vaflag = 0; argno = 0;} break;
case 5:{ asmout();  blevel = 0; DECL_RESET; argno = 0;} break;
case 6:{  
				yypvt[-1].nodep->in.op = FREE; DECL_RESET; 
			} break;
case 7:{  
				yypvt[-1].nodep->in.op = FREE; DECL_RESET; 
				if ( indcl != -1 )
					werror( W_STRICT, 
					"empty external declaration" );
			} break;
case 8:{  
				yypvt[-2].nodep->in.op = FREE; DECL_RESET; 
			} break;
case 9:{  
				yypvt[-2].nodep->in.op = FREE; DECL_RESET; 
				if ( indcl != -1 )
					werror( W_STRICT, 
					"non-function external must have specifiers" );
			} break;
case 10:{  
			    if( blevel ) cerror( "function level error" );
			    if( reached ) retstat |= NRETVAL; 
			    yypvt[-1].nodep->in.op = FREE;
			    ftnend();
		    } break;
case 11:{	
			newdef:
				regvar = typequal = 0;   /* clear out arguments */
			    if( blevel ) cerror( "function level error" );
			    if( reached ) retstat |= NRETVAL; 
			    yypvt[-1].nodep->in.op = FREE;
			    ftnend();
		    } break;
case 12:{ 
						typequal = 0; seen_typespec = 0; 
						newstyle = 1;
						goto fcode; 
					} break;
case 13:{ 
						typequal = 0; seen_typespec = 0; 
						newstyle = 1;
						goto fcode; 
					} break;
case 14:{		goto fcode; } break;
case 15:{ 
			fcode:
				indcl = seen_typespec = 0; 
#ifdef IN_LINE
	/* got a function, is it an asm inline function? */
	/* squirelmacro will purge stdin up to closing '}', then it'll */
	/* try to get the parser to se '}' by unget('}') to believe */
	/* the "function body" part (it's already seen the '{') */

			    if (curclass == INLINE)
			    {
				defid(tymerge(yypvt[-1].nodep, yypvt[-0].nodep), INLINE);
				squirrelmacro(stab[yypvt[-0].nodep->tn.rval].sname);
			    }    
			    else
#endif
				{
					ProtoDefine(yypvt[-0].nodep);		/* got identifiers for all types? */
					switch ( curclass ) 		/* bk */
					{ 
						case SNULL:
						case EXTERN:
						case STATIC:
						case FORTRAN:
							break;
						default:
							uerror( "function has illegal storage class" );
					}
					defid( tymerge(yypvt[-1].nodep,yypvt[-0].nodep),
						curclass==STATIC?STATIC:EXTDEF );
					if ( newstyle ) {
						blevel = 1;
						newstyle = 0;
						curclass = SNULL;
					}
				}
			} break;
case 16:{	regvar = 0; typequal = 0; } break;
case 17:{ yypvt[-2].nodep->in.op = FREE; DECL_RESET; } break;
case 18:{ yypvt[-1].nodep->in.op = FREE; DECL_RESET; } break;
case 19:{  blevel = 1; DECL_RESET; } break;
case 20:{ constid = 0; leftcons = 1;} break;
case 21:{
			   if (curclass != INLINE)
			   {  bccode();
			       locctr(PROG);
			   }
			} break;
case 22:{  
				int t = BTYPE(yypvt[-1].nodep->in.type); 
				if ( t != ENUMTY && t != STRTY && t != UNIONTY )
					uerror( "missing declarator" );
				yypvt[-1].nodep->in.op = FREE;  DECL_RESET;
			} break;
case 23:{  yypvt[-2].nodep->in.op = FREE;  DECL_RESET; } break;
case 25:{  yyval.nodep = mkty(INT,0,INT,0);  
				curclass = SNULL; } break;
case 26:{	
				if ( yypvt[-0].intval != -1 ) {		/* -1 means struct_dcl or type_def */
					int bldtype = types();			/* synthesize chktype */	
					yyval.nodep = mkty( (TWORD) bldtype, 0, bldtype, typequal );
				} else {
					if ( type_def ) {
						yyval.nodep = type_def;
						type_def = 0;
						if ( str_type ) {
							str_type->in.op = FREE;
							str_type = 0;
						}
					} else {
						if ( arg_type ) {
							yyval.nodep = arg_type;
							arg_type = 0;
						} else {
							yyval.nodep = str_type;
							str_type = 0;
						}
					}
				}
			} break;
case 27:{	
				if ( yypvt[-0].intval != -1 ) {		/* -1 means struct_dcl or type_def */
					int bldtype = types();			/* synthesize chktype */	
					yyval.nodep = mkty( (TWORD) bldtype, 0, bldtype, 0 );
				} else {
					if ( type_def ) {
						yyval.nodep = type_def;
						type_def = 0;
						if ( str_type ) {
							str_type->in.op = FREE;
							str_type = 0;
						}
					} else {
						if ( arg_type ) {
							yyval.nodep = arg_type;
							arg_type = 0;
						} else {
							yyval.nodep = str_type;
						}
					}
				}
			} break;
case 28:{ indcl = -1; } break;
case 29:{ yyval.intval = ( yypvt[-0].intval == -1 ) ? -1 : 0 ; } break;
case 30:{ 
				if ( yypvt[-0].intval == -1 ) {		/* e.g. short struct foo { ... }; */
					if ( str_type )
						uerror( "illegal type specifiers" );
				}
			} break;
case 31:{ yyval.intval = ( yypvt[-0].intval == -1 ) ? -1 : 0 ; } break;
case 32:{ yyval.intval = 0; } break;
case 35:{ yyval.intval = ( yypvt[-0].intval == -1 ) ? -1 : 0 ; } break;
case 36:{ yyval.intval = 0; } break;
case 38:{  
				if ( curclass != 0 )
					uerror( "extra storage class specifier" );
				curclass = yypvt[-0].intval;
			} break;
case 41:{ ++tqshift; } break;
case 42:{ typequal = type_quals( typequal, yypvt[-0].intval, tqshift ); } break;
case 43:{	typequal = type_quals( typequal, yypvt[-1].intval, tqshift ); } break;
case 44:{ 
					if ( !type_def ) {
						if (chktype[ yypvt[-0].nodep->in.type - 1 ] )
							uerror( "illegal type combination" );
						chktype[ yypvt[-0].nodep->in.type - 1 ] = 1;
						yypvt[-0].nodep->in.op = FREE;
						yyval.intval=0;
					}
					else {
					 if ( yypvt[-0].nodep != type_def ) /* e.g.:  "TYPEDEF_NAME int x;" */
						uerror( "illegal combination of type and typedef" );
					 if ( decomp_type(yypvt[-0].nodep) )
					   yyval.intval = -1;
					 else {
					   yyval.intval = 0;
					   yypvt[-0].nodep->in.op = FREE;
					   type_def = 0 ; 
					 }
					}
			} break;
case 45:{
			str_chk:
				yyval.intval = -1;
				str_type = yypvt[-0].nodep;
			} break;
case 46:{ goto str_chk; } break;
case 47:{ 
				arg_type = yypvt[-0].nodep;
				yyval.intval = -1; 
		   } break;
case 48:{	
				if (optid>0) { 
					uerror( "declarator not allowed in __argtype" ); 
					optid = -1; 
				}
				yyval.nodep = do_argty( yypvt[-1].nodep );
				ProtoMirage();
			} break;
case 49:{  yyval.nodep = do_argty( yypvt[-0].nodep ); } break;
case 52:{ yyval.nodep = dclstruct(yypvt[-4].intval); } break;
case 53:{  yyval.nodep = rstruct(yypvt[-0].intval,0);  stwart = instruct; } break;
case 54:{  yyval.intval = bstruct(-1,0); stwart = SEENAME; } break;
case 55:{  yyval.intval = bstruct(yypvt[-0].intval,0); stwart = SEENAME; } break;
case 58:{  moedef( yypvt[-0].intval ); } break;
case 59:{	strucoff = yypvt[-0].intval;  enum_chk = 0; moedef( yypvt[-2].intval ); } break;
case 60:{ yyval.nodep = dclstruct(yypvt[-4].intval);  
			  if (is_struc[sulvl]) strucdef--; /*finished struc def
						    ** ... may be nested. */
			  sulvl--;
			  typequal = 0;
		        } break;
case 61:{  yyval.nodep = rstruct(yypvt[-0].intval,yypvt[-1].intval); } break;
case 62:{ werror( W_STRICT, "missing semi-colon in structure member list" ); } break;
case 64:{  
				struc_qual[sulvl] = typequal;
				typequal = 0;
			   sulvl++;
			   is_struc[sulvl] = (yypvt[-0].intval == INSTRUCT);
			   if (is_struc[sulvl])
			      /* struct def, not union def */
			      strucdef++;
			   yyval.intval = bstruct(-1,yypvt[-0].intval);  stwart=0; } break;
case 65:{  
				struc_qual[sulvl] = typequal;
				typequal = 0;
			   sulvl++;
			   is_struc[sulvl] = (yypvt[-1].intval == INSTRUCT);
			   if (is_struc[sulvl])
			      /* struct def, not union def */
			      strucdef++;
			   yyval.intval = bstruct(yypvt[-0].intval,yypvt[-1].intval);  stwart=0;  } break;
case 68:{ 
				stwart=0; yypvt[-1].nodep->in.op = FREE; 
				DECL_RESET2;
			} break;
case 69:{	
				seen_typespec = 0;
				if( curclass != MOU ) {
					curclass = SNULL;
				} else {
					sprintf( fakename, "$%dFAKE", fake++ );
					defid( tymerge(yypvt[-0].nodep, bdty(NAME,NIL,
						lookup( fakename, SMOS ))), curclass );
					werror( W_DFLT, "union member must be named" );
				}
			    stwart = 0;
			    yypvt[-0].nodep->in.op = FREE;
				DECL_RESET2;
		    } break;
case 70:{ defid( tymerge(yypvt[-1].nodep,yypvt[-0].nodep), curclass);
				stwart = instruct; } break;
case 71:{yyval.nodep=yypvt[-2].nodep;} break;
case 72:{ defid( tymerge(yypvt[-4].nodep,yypvt[-0].nodep), curclass);
				stwart = instruct; } break;
case 74:{  if( !(instruct & (INSTRUCT|INUNION)) )	/*bk*/
				uerror( "field outside of structure" );
			    if( yypvt[-0].intval<0 || yypvt[-0].intval >= FIELD ){
				uerror( "illegal field size" );
				yypvt[-0].intval = 1;
				}
			    defid( tymerge(yypvt[-3].nodep,yypvt[-2].nodep), FIELD|yypvt[-0].intval );
			    yyval.nodep = NIL;
			    } break;
case 75:{   if( !(instruct & (INSTRUCT|INUNION)) )
				uerror( "field outside of structure" );
			    /* alignment or hole */
			    falloc( stab, yypvt[-0].intval, -1, yypvt[-2].nodep );
			    yyval.nodep = NIL;
			    } break;
case 76:{  yyval.nodep = NIL; } break;
case 77:{  umul:
				yyval.nodep = bdty( UNARY MUL, yypvt[-0].nodep, 0 ); } break;
case 78:{  uftn:
				yyval.nodep = bdty( UNARY CALL, yypvt[-2].nodep, 0 ); } break;
case 79:{ 
				yyval.nodep = bdty( UNARY CALL, yypvt[-3].nodep, 0 );
				stwart = 0;
				ProtoLink( yypvt[-3].nodep, 0 );		 /* tie protolist to ftn-ptr id */
				ProtoDeclare(yypvt[-3].nodep);		/* just an id, not a function def'n */
				seen_typespec = 0;
			} break;
case 80:{  
				seen_typespec = 0;
			uary:
				yyval.nodep = bdty( LB, yypvt[-2].nodep, 0 ); } break;
case 81:{  
				seen_typespec = 0;
			bary:
				if( (int)yypvt[-1].intval <= 0 )
					werror( W_DFLT, "zero or negative subscript" );
				yyval.nodep = bdty( LB, yypvt[-3].nodep, yypvt[-1].intval ); 
				} break;
case 82:{  seen_typespec = 0; yyval.nodep = bdty( NAME, NIL, yypvt[-0].intval ); } break;
case 83:{ yyval.nodep=yypvt[-1].nodep; seen_typespec = 0; } break;
case 84:{		/* e.g. "int (*foo(int x))(char *y)" */
				yyval.nodep = bdty( UNARY CALL, yypvt[-3].nodep, 0 );
				stwart = 0;
				ProtoPush();	/* we don't know name of pointed-to ftn */
				ProtoLink(yypvt[-3].nodep,1);
			} break;
case 85:{
				NODE *idn;
				argspace = 0;
				yyval.nodep = bdty( UNARY CALL, bdty(NAME,NIL,yypvt[-2].intval), 0 );
				stwart = 0;
				ProtoLink(idn=bdty(NAME,NIL,yypvt[-2].intval),0); /*link ptypes to ftn id*/
				idn->in.op = FREE;
				} break;
case 86:{  goto umul; } break;
case 87:{  
				yyval.nodep = bdty( UNARY CALL, yypvt[-3].nodep, 0 );
				stwart = 0;
				if ( yypvt[-1].nodep ) {
					ProtoPush();
					ProtoLink(yypvt[-3].nodep,1);
				}
			} break;
case 88:{  goto uary; } break;
case 89:{  goto bary; } break;
case 90:{ yyval.nodep = yypvt[-1].nodep; } break;
case 91:{  goto umul; } break;
case 92:{  goto uftn; } break;
case 93:{  goto uary; } break;
case 94:{  goto bary; } break;
case 95:{ yyval.nodep = yypvt[-1].nodep; } break;
case 96:{
				argspace = 0;
				if( blevel!=0 )
					uerror(
					"function declaration in bad context");
				yyval.nodep = bdty( UNARY CALL, bdty(NAME,NIL,yypvt[-2].intval), 0 );
				stwart = 0;
				} break;
case 97:{
				argspace = 0;
				yyval.nodep = bdty( UNARY CALL, bdty(NAME,NIL,yypvt[-1].intval), 0 );
				stwart = 0;
				} break;
case 99:{ yyval.intval = 0; } break;
case 100:{
				/* turn off typedefs for argument names */
/*				stwart = SEENAME; / bk: to allow typedef prototype */
				if( stab[yypvt[-1].intval].sclass == SNULL )
				    stab[yypvt[-1].intval].stype = FTN;
				argspace = 1;			/*bk*/
				seen_typespec = 0;
				} break;
case 101:{		
				ProtoEnd(yypvt[-0].intval);			/* mark end of prototype list */
				tfree(yypvt[-1].nodep);				/* done with proto list */
				--blevel;				/* back down from protolist scope */
				yyval.nodep = yypvt[-1].nodep;
			} break;
case 102:{ 
				if ( curclass && curclass != REGISTER )
					uerror( 
				"\"register\" is only valid storage class in a parameter list");
				if (optid >= 0) optid = ftnarg( optid ); 
				ProtoType(yypvt[-0].nodep,optid);
				curclass = saveclass;
				DECL_RESET3;
				optid = -1;
			} break;
case 103:{ 
				if ( curclass && curclass != REGISTER )
					uerror( 
				"\"register\" is only valid storage class in a parameter list");
				if (optid >= 0) optid = ftnarg( optid ); 
				ProtoType(yypvt[-0].nodep,optid);
				curclass = saveclass;
				DECL_RESET3;
				optid = -1;
				goto bop;
			} break;
case 104:{ ++blevel; } break;
case 105:{ ftnarg( yypvt[-0].intval );  stwart = SEENAME; } break;
case 106:{ ftnarg( yypvt[-0].intval );  stwart = SEENAME; } break;
case 109:{yyval.nodep=yypvt[-2].nodep;} break;
case 111:{ 	/* bk: was previous decl tentative? (ANSI 3.7.2) */
				yypvt[-0].nodep = tymerge(yypvt[-1].nodep,yypvt[-0].nodep);
				tentative_flag = 1;	/* be on guard for tentative def */
				xnfdecl = 1;	/* some type info may come via initializer */
				defid( yypvt[-0].nodep, curclass );
			    beginit(yypvt[-0].nodep->tn.rval);
				xnfdecl = 0;
				tentative_flag = 0;
				DECL_RESET4;
			    } break;
case 113:{  
				struct symtab *p;

				yypvt[-0].nodep = tymerge( yypvt[-1].nodep,yypvt[-0].nodep ); 
				tentative_flag = 1;	/* be on guard for tentative def */
				nidcl( yypvt[-0].nodep );

				p = &stab[yypvt[-0].nodep->tn.rval];
				if ( ( CONST_QUALIFIED( yypvt[-0].nodep->tn.type, p->stypeq ) ) &&
						p->sclass != TYPEDEF )
					werror( W_DFLT, "unitialized const-qualified object" );
				tentative_flag = 0;
			} break;
case 114:{ defid( 
				ProtoDeclare(tymerge(yypvt[-1].nodep,yypvt[-0].nodep)), 
							uclass(curclass) );
			} break;
case 115:{ ProtoMix(yypvt[-0].nodep); defid( tymerge(yypvt[-1].nodep,yypvt[-0].nodep), uclass(curclass) ); } break;
case 116:{  
				/*bk:  struct { ... } x = expr; */
				/* char p[] = "hi", e above == NIL */
				if ( yypvt[-0].nodep && yypvt[-0].nodep->in.type == STRTY ) 
					noilist = 1;

					doinit( yypvt[-0].nodep );
					endinit();
					noilist = 0;			/*bk*/
			} break;
case 117:{  endinit(); } break;
case 121:{  doinit( yypvt[-0].nodep ); } break;
case 122:{  irbrace(); } break;
case 125:{  ilbrace(); } break;
case 126:{
			    clearst(blevel);
			    if (--blevel == 1)
			    {
				clearst(blevel);
				blevel = 0;
			    }
			    checkst(blevel);
			    autooff = *--psavbc;
			    regvar = *--psavbc;
#ifdef MC68K
			    eccode(blevel);	/* called at end of block */
#endif
			} break;
case 127:{  if( blevel == 1 ) dclargs();
			    ++blevel;
			    if( psavbc > &asavbc[BCSZ-2] )
					cerror( "nesting too deep" );
			    *psavbc++ = regvar;
			    *psavbc++ = autooff;
			    } break;
case 128:{ ecomp( yypvt[-1].nodep ); } break;
case 129:{ asmout(); } break;
case 131:{  
				deflab(yypvt[-1].intval);
			   	reached = 1;
			   } break;
case 132:{ 	
				if( yypvt[-1].intval != NOLAB ){
				deflab( yypvt[-1].intval );
				reached = 1;
				}
			    } break;
case 133:{   
		            branch(  contlab );
			    deflab( brklab );
			    if( (flostat&FBRK) || !(flostat&FLOOP)) reached = 1;
			    else reached = 0;
			    resetbc(0);
			    } break;
case 134:{ 	
			    deflab( contlab );
			    if( flostat & FCONT ) reached = 1;
			    ecomp( buildtree( CBRANCH,
			    buildtree( NOT, yypvt[-2].nodep, NIL ), bcon( yypvt[-6].intval ) ) );
			    deflab( brklab );
			    reached = 1;
			    resetbc(0);
			    } break;
case 135:{   
			    deflab( contlab );
			    if( flostat&FCONT ) reached = 1;
			    if( yypvt[-2].nodep ) ecomp( yypvt[-2].nodep );
			    branch( yypvt[-3].intval );
			    deflab( brklab );
			    if( (flostat&FBRK) || !(flostat&FLOOP) ) reached=1;
			    else reached=0;
			    resetbc(0);
			    } break;
case 136:{   
			    if( reached ) branch( brklab );
			    deflab( yypvt[-1].intval );
			    swend();
			    deflab(brklab);
			    if( (flostat&FBRK) || !(flostat&FDEF) ) reached=1;
			    resetbc(FCONT);
			    } break;
case 137:{  if( brklab == NOLAB ) uerror( "illegal break");
			    else if(reached) branch( brklab );
			    flostat |= FBRK;
			    if( brkflag ) goto rch;
			    reached = 0;
			    } break;
case 138:{   if( contlab == NOLAB ) uerror( "illegal continue");
			    else branch( contlab );
			    flostat |= FCONT;
			    goto rch;
			    } break;
case 139:{   retstat |= NRETVAL;
			    branch( retlab );
			rch:
			    if( !reached ) werror( W_LINT, "statement not reached" );
			    reached = 0;
			    } break;
case 140:{   register NODE *temp;
			    TWORD indtype();
			    idname = curftn;
			    temp = buildtree( NAME, NIL, NIL );
			    if(temp->in.type == (FTN|VOID))
				uerror(
				"void function %s cannot return value",
					stab[idname].sname);
			    temp->in.type = DECREF( temp->in.type );
			    temp->in.type = indtype( temp->in.type );
			    temp->tn.op = RNODE;  /* node for return */
			    temp = buildtree( ASSIGN, temp, yypvt[-1].nodep );
			    ecomp( temp );
			    retstat |= RETVAL;
			    branch( retlab );
			    reached = 0;
			    } break;
case 141:{  register NODE *q;
			    q = block( FREE, NIL, NIL, INT|ARY, 0, INT, 0 );
			    q->tn.rval = idname = yypvt[-1].intval;
			    defid( q, ULABEL );
			    stab[idname].suse = -lineno;
			    branch( stab[idname].offset );
			    goto rch;
			    } break;
case 146:{  register NODE *q;
			    q = block( FREE, NIL, NIL, INT|ARY, 0, LABEL, 0 );
			    q->tn.rval = yypvt[-1].intval;
			    defid( q, LABEL );
			    reached = 1;
			    } break;
case 147:{  addcase(yypvt[-1].nodep);
			    reached = 1;
			    } break;
case 148:{  reached = 1;
			    adddef();
			    flostat |= FDEF;
			    } break;
case 149:{  savebc();
			    if( !reached ) werror( W_LINT, "loop not entered at top");
			    brklab = getlab();
			    contlab = getlab();
			    deflab( yyval.intval = getlab() );
			    reached = 1;
			    } break;
case 150:{
			 ecomp( buildtree( CBRANCH, yypvt[-1].nodep, bcon( yyval.intval=getlab()) ) ) ;
			    reached = 1;
			    } break;
case 151:{
			 	if( reached ) branch( yyval.intval = getlab() );
			    else yyval.intval = NOLAB;
			    deflab( yypvt[-2].intval );
			    reached = 1;
			    } break;
case 152:{
			 	savebc();
			    if( !reached ) werror( W_LINT, "loop not entered at top");
			    if( yypvt[-1].nodep->in.op == ICON && yypvt[-1].nodep->tn.lval != 0 ) flostat = FLOOP;
			    deflab( contlab = getlab() );
			    reached = 1;
			    brklab = getlab();
			    if( flostat == FLOOP ) tfree( yypvt[-1].nodep );
			    else ecomp( buildtree( CBRANCH, yypvt[-1].nodep, bcon( brklab) ) );
			    } break;
case 153:{
			 	if( yypvt[-3].nodep ) ecomp( yypvt[-3].nodep );
			    else if( !reached ) werror( W_LINT, "loop not entered at top");
			    savebc();
			    contlab = getlab();
			    brklab = getlab();
			    deflab( yyval.intval = getlab() );
			    reached = 1;
			    if( yypvt[-1].nodep )
				ecomp( buildtree( CBRANCH, yypvt[-1].nodep, bcon( brklab) ));
			    else flostat |= FLOOP;
			    } break;
case 154:{
			   register NODE *temp;
			    savebc();
			    temp = block( SNODE, NIL, NIL, INT, 0, INT, 0 );
			    temp = buildtree( ASSIGN, temp, yypvt[-1].nodep );
			    brklab = getlab();
			    ecomp( temp );
			    branch( yyval.intval = getlab() );
			    swstart();
			    reached = 0;
			    } break;
case 155:{ yyval.intval=instruct; stwart=instruct=0; } break;
case 156:{	
				icon_overflow( yypvt[-0].nodep, 1 );		/* bk */
				yyval.intval = icons( yypvt[-0].nodep );  
				instruct=yypvt[-1].intval; 
			} break;
case 158:{ yyval.nodep=0; } break;
case 160:{  goto bop; } break;
case 161:{
			preconf:
			    if( yychar==RELOP || yychar==EQUOP || yychar==AND
					|| yychar==OR || yychar==ER ){
			precplaint:
				werror( W_LINT,
				"precedence confusion possible: parenthesize!"
					);
				}
			bop:
			    yyval.nodep = buildtree( yypvt[-1].intval, yypvt[-2].nodep, yypvt[-0].nodep );
			    } break;
case 162:{  yypvt[-1].intval = COMOP;
			    goto bop;
			    } break;
case 163:{  goto bop; } break;
case 164:{  if(yychar==SHIFTOP) goto precplaint; else goto bop; } break;
case 165:{  if(yychar==SHIFTOP ) goto precplaint; else goto bop; } break;
case 166:{  if(yychar==PLUS||yychar==MINUS) goto precplaint; else goto bop; } break;
case 167:{  goto bop; } break;
case 168:{  goto preconf; } break;
case 169:{  if( yychar==RELOP||yychar==EQUOP ) goto preconf;  else goto bop; } break;
case 170:{  if(yychar==RELOP||yychar==EQUOP) goto preconf; else goto bop; } break;
case 171:{  if(yychar==RELOP||yychar==EQUOP) goto preconf; else goto bop; } break;
case 172:{  goto bop; } break;
case 173:{  goto bop; } break;
case 174:{  abop:
				yyval.nodep = buildtree( yypvt[-1].intval, yypvt[-2].nodep, yypvt[-0].nodep );
				} break;
case 175:{   yyval.nodep = buildtree( ASG yypvt[-1].intval, yypvt[-2].nodep, yypvt[-0].nodep ); } break;
case 176:{  goto abop; } break;
case 177:{  goto abop; } break;
case 178:{  yyval.nodep = buildtree( ASG yypvt[-1].intval, yypvt[-2].nodep, yypvt[-0].nodep ); } break;
case 179:{  goto abop; } break;
case 180:{  goto abop; } break;
case 181:{  goto abop; } break;
case 182:{  yyval.nodep=buildtree(QUEST, yypvt[-4].nodep, buildtree( COLON, yypvt[-2].nodep, yypvt[-0].nodep ) );
			    } break;
case 183:{  werror( W_LINT, "old-fashioned assignment operator" );
				goto bop; } break;
case 184:{	goto bop; } break;
case 186:{  yyval.nodep = buildtree( yypvt[-0].intval, yypvt[-1].nodep, bcon(1) ); } break;
case 187:{ ubop:
			    if( constid && leftcons ) 
			    {
			    	yyval.nodep = buildtree( UNARY yypvt[-1].intval, yypvt[-0].nodep, NIL );
				leftcons = 0; 
			    }
			    else
			    	yyval.nodep = buildtree( UNARY yypvt[-1].intval, yypvt[-0].nodep, NIL );
			} break;
case 188:{  
				if ( modes[ MODE_ANSIKEYW ].lxmval )
				{
					if ( str_expr( yypvt[-0].nodep ) ) {
						yyval.nodep = yypvt[-0].nodep;
					}
					else {
						goto ubop;
					}
				}
				else
					if ( ISFTN(yypvt[-0].nodep->in.type) || 
					   ISARY(yypvt[-0].nodep->in.type) ) {
						yyval.nodep = yypvt[-0].nodep;
					}
					else
						if ( str_expr ( yypvt[-0].nodep ) ) {
							yyval.nodep = yypvt[-0].nodep;
						}
						else
							goto ubop;
			} break;
case 189:{  goto ubop; } break;
case 190:{
					/*would like to do zip, but 
					may need default integral promotions*/
				goto ubop; 
			} break;
case 191:{
			    yyval.nodep = buildtree( yypvt[-1].intval, yypvt[-0].nodep, NIL );
			    } break;
case 192:{  yyval.nodep = buildtree( yypvt[-1].intval==INCR ? ASG PLUS : ASG MINUS,
						yypvt[-0].nodep,
						bcon(1)  );
			    } break;
case 193:{  yyval.nodep = doszof( yypvt[-0].nodep ); } break;
case 194:{
/*				NODE *bfk;
				bfk = $2;				** bk: pardon me?? */
				if (optid > 0) { 
					uerror( "declarator not allowed in cast" ); 
					optid = -1; 
				}
				yyval.nodep = buildtree( CAST, yypvt[-2].nodep, yypvt[-0].nodep );
			    yyval.nodep->in.left->in.op = FREE;
			    yyval.nodep->in.op = FREE;
			    yyval.nodep = yyval.nodep->in.right;
				ProtoMirage();
			    } break;
case 195:{	
				if (optid>0) { 
					uerror( "declarator not allowed in sizeof" ); 
					optid = -1; 
				}
				yyval.nodep = doszof( yypvt[-1].nodep );
				ProtoMirage();
			} break;
case 196:{  yyval.nodep = buildtree( LB, yypvt[-3].nodep, yypvt[-1].nodep ); } break;
case 197:{  yyval.nodep = xicolon( yypvt[-5].nodep, yypvt[-3].nodep, yypvt[-1].nodep ); } break;
case 198:{  ftn_call = 0; yyval.nodep=buildtree(UNARY CALL,yypvt[-1].nodep,NIL);  } break;
case 199:{ ftn_call = 0;  yyval.nodep=buildtree(CALL,yypvt[-2].nodep,yypvt[-1].nodep);} break;
case 200:{  if( yypvt[-1].intval == DOT ){
/*				if( notlval( $1 ) )uerror(
				       "structure reference must be addressable"
					); */				/* bk: see str_expr in trees.c */
				yypvt[-2].nodep = buildtree( UNARY AND, yypvt[-2].nodep, NIL );
				}
			    idname = yypvt[-0].intval;
			    yyval.nodep = buildtree( STREF, yypvt[-2].nodep,
					buildtree( NAME, NIL, NIL ) );
			    } break;
case 201:{  idname = yypvt[-0].intval;
			    /* recognize identifiers in initializations */
			    if( blevel==0 && stab[idname].stype == UNDEF ) {
				register NODE *q;
				werror( W_DFLT, "undeclared initializer name %s",
					stab[idname].sname );
				q = block( FREE, NIL, NIL, INT, 0, INT, 0 );
				q->tn.rval = idname;
				defid( q, EXTERN );
				}

			    if ( constid && yypvt[-1].intval != MUL)  leftcons = 0;

			    yyval.nodep=buildtree(NAME,NIL,NIL);
			    stab[yypvt[-0].intval].suse = -lineno;
			} break;
case 202:{  yyval.nodep=bcon(0);
			    yyval.nodep->tn.lval = lastcon;
			    yyval.nodep->tn.rval = NONAME;
			    if( yypvt[-0].intval ) yyval.nodep->fn.csiz = yyval.nodep->in.type = ctype( ULONG );	/*bk */
			    } break;
case 203:{  yyval.nodep=buildtree(FCON,NIL,NIL);
			    yyval.nodep->fpn.dval = dcon;
				if ( yypvt[-0].intval ) yyval.nodep->fn.csiz = yyval.nodep->fpn.type = ctype(FLOAT);	/*bk*/
			    } break;
case 204:{  yyval.nodep = getstr(); /* get string contents */ } break;
case 205:{ yyval.nodep=yypvt[-1].nodep; } break;
case 206:{
				yyval.nodep = tymerge( yypvt[-1].nodep, yypvt[-0].nodep );
				yyval.nodep->in.op = NAME;
				yypvt[-1].nodep->in.op = FREE;
				if (incit) {
					incit = 0;
					yyval.nodep->in.type = INCREF(yyval.nodep->in.type);
				}
				seen_typespec = 0;
				maybe_cast = 0;
			} break;
case 207:{ saveclass = curclass; maybe_cast = 1;
			curclass=SNULL; } break;
case 208:{	yyval.nodep = (NODE *) 0; } break;
case 209:{ yyval.nodep = yypvt[-0].nodep; } break;
case 210:{ yyval.nodep = bdty( NAME, NIL, yypvt[-0].intval ); } break;
case 211:{ yyval.nodep = buildtree( UNARY CALL, bdty(NAME,NIL,-1),0); } break;
case 212:{ yyval.nodep = bdty( UNARY CALL, bdty(NAME,NIL,yypvt[-1].intval),0); } break;
case 213:{ ftnsave = optid; } break;
case 214:{ 
				optid = ftnsave;	/* optnames in opt_parm are comments only */
				yypvt[-5].nodep->in.type = FTN|INT;
				yyval.nodep = bdty( UNARY CALL, yypvt[-5].nodep, 0 ); 
				if ( yypvt[-1].nodep )					/* any prototype? */
					ProtoLink(yypvt[-5].nodep,0);		/* link prototypes to funct id */
			} break;
case 215:{  goto umul; } break;
case 216:{  goto uary; } break;
case 217:{  goto bary;  } break;
case 218:{ ftnsave = yypvt[-1].intval; } break;
case 219:{ 
				NODE *wrk;
				optid = ftnsave;	/* optnames in opt_parm are comments only */
				wrk = bdty( NAME, NIL, yypvt[-4].intval );
				wrk->in.type = FTN|INT;
				incit = 1;					/* do INCREF once we have TWORD */
				yyval.nodep = bdty( UNARY CALL, wrk, 0 ); 
				if ( yypvt[-1].nodep )					/* any prototype? */
					ProtoLink(wrk,0);		/* link prototypes to funct id */
			} break;
case 220:{ yyval.nodep = yypvt[-1].nodep; } break;
case 221:{ optid = yypvt[-0].intval; seen_typespec = 0; } break;
case 222:{ optid = -1; yyval.intval = -1; seen_typespec = 0; } break;
case 223:{ ProtoPush(); } break;
case 224:{  if( stab[yypvt[-1].intval].stype == UNDEF ){
				register NODE *q;
				q = block( FREE, NIL, NIL, FTN|INT, 0, INT, 0 );
				q->tn.rval = yypvt[-1].intval;
				defid( q, EXTERN );
				}
			    idname = yypvt[-1].intval;
			    yyval.nodep=buildtree(NAME,NIL,NIL);
			    stab[idname].suse = -lineno;
				ftn_call = 1;			/* bk */
			} break;
case 225:{ 	/* bk: ANSI 3.3.2.2  */
				if ( (yypvt[-1].nodep->in.type & ( FTN<<TSHIFT | PTR )) == 0 )	
					if ( ( yypvt[-1].nodep->in.type & FTN ) == 0 )
						uerror( "illegal function expression" );
				ftn_call = 1; 
			} break;
	}
	goto yystack;		/* reset registers in driver code */
}
