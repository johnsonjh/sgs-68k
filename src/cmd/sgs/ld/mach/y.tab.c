
# line 2 "./../../ld/common/ld.yac"
#ident	"@(#)ld.yac	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "system.h"
#include "extrns.h"
#include "bool.h"
#include "attributes.h"
#include "list.h"

#if TRVEC
#include "tv.h"
#include "ldtv.h"
#endif

#include "sgs.h"
#include "sgsmacros.h"
#include "ldmacros.h"



extern char inline[];


/*
 *
 *	There is a very important variable, called
 *
 *		in_y_exp
 *
 *	that is used to help recognize filenames.
 *
 *	The problems is this:  a UNIX filename can contain
 *	virtually any character, and the nodes in the pathname
 *	are separated by slashes.  Unfortunately, slashes also
 *	occur in expressions, comments, and the /= assignment
 *	operator.  Moreover, the LEX rules ignore white space,
 *	which is important to knowing where one filename stops
 *	and the next begins.
 *
 *	The resolution is this.  Lex doesn't know enough alone to
 *	recognize a filename; so we give it some help.  Whenever we
 *	are in an expression, we set in_y_exp to TRUE, and then lex knows
 *	that any slash is a slash, and should not be kept as part of
 *	a filename.  a/b is a divided by b, not the file a/b.
 *
 *	Consequently, whenever a slash must be kept as a slash,
 *	in_y_exp will be TRUE.  Otherwise, it will be FALSE, and the
 *	lexical analyzer will treat a/b as a filename.
 */

int in_y_exp;

static secnum;					/* number of sections */
static char *fnamptr;				/* latest input file name */
static int nsecspcs, fillflag;			/* parsing status flags */
static ACTITEM *aiptr, *afaiptr, *grptr;	/* pointers to action items */

enum scope scope_directive = __undefined;/* hide or export */

extern char *curfilnm;				/* name of current ifile */

#if TRVEC
static TVASSIGN *tvslotn,		/* ptr to last  in list of tv slots */
		*slotptr;		/* temp for traversing list	    */
#endif

static int tempi;				/* temporary int */
/*eject*/

# line 178 "./../../ld/common/ld.yac"
typedef union  {
	int ivalue;	/* yylval values */
	char *sptr;
	long *lptr;

	ACTITEM *aitem;	/* nonterminal values */
	ENODE	*enode;
	} YYSTYPE;
# define NAME 2
# define LONGINT 3
# define INT 4
# define ALIGN 5
# define DOT 6
# define LEN 7
# define MEMORY 8
# define ORG 9
# define REGIONS 10
# define SECTIONS 11
# define PHY 12
# define AND 13
# define ANDAND 14
# define BNOT 15
# define COLON 16
# define COMMA 17
# define DIV 18
# define EQ 19
# define EQEQ 20
# define GE 21
# define GT 23
# define LBRACE 24
# define LE 26
# define LPAREN 27
# define LSHIFT 28
# define LT 29
# define MINUS 30
# define MULT 31
# define NE 32
# define NOT 33
# define OR 34
# define OROR 35
# define PC 36
# define PLUS 37
# define RBRACE 38
# define RPAREN 39
# define RSHIFT 40
# define SEMICOL 41
# define DIVEQ 42
# define MINUSEQ 43
# define MULTEQ 44
# define PLUSEQ 45
# define FILENAME 46
# define TV 47
# define SPARE 48
# define DSECT 49
# define NOLOAD 50
# define COPY 51
# define INFO 52
# define OVERLAY 53
# define BLOCK 54
# define UMINUS 55
# define GROUP 56
# define RANGE 57
# define ASSIGN 58
# define SIZEOF 59
# define ADDR 60
# define NEXT 61
# define BIND 62
# define COMMN 63
# define LSQBRACK 64
# define RSQBRACK 65
# define EXPORT 66
# define HIDE 67
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
typedef int yytabelem;
# define YYERRCODE 256

# line 817 "./../../ld/common/ld.yac"

/*eject*/
/*VARARGS*/
yyerror(format, a1, a2, a3, a4)
char *format;
{

/*
 * Issue a parsing error message
 */

	char *p;

	p = sname(curfilnm);	/* strip off directories from path name */

/*
 * For any purely YACC-generated error, also print out the current
 * line, up to the point of the error
 */

	if( strcmp(format, "syntax error") == 0 )
		lderror(1, lineno, p, "%s : scanned line = (%s)", format, inline, a1, a2 );
	else
		lderror(1, lineno, p, format, a1, a2, a3, a4);
}
yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 2,
	0, 1,
	-2, 0,
-1, 19,
	19, 136,
	42, 136,
	43, 136,
	44, 136,
	45, 136,
	-2, 138,
-1, 90,
	17, 79,
	38, 79,
	-2, 0,
-1, 163,
	20, 0,
	21, 0,
	23, 0,
	26, 0,
	29, 0,
	32, 0,
	-2, 113,
-1, 164,
	20, 0,
	21, 0,
	23, 0,
	26, 0,
	29, 0,
	32, 0,
	-2, 114,
-1, 165,
	20, 0,
	21, 0,
	23, 0,
	26, 0,
	29, 0,
	32, 0,
	-2, 115,
-1, 166,
	20, 0,
	21, 0,
	23, 0,
	26, 0,
	29, 0,
	32, 0,
	-2, 116,
-1, 167,
	20, 0,
	21, 0,
	23, 0,
	26, 0,
	29, 0,
	32, 0,
	-2, 117,
-1, 168,
	20, 0,
	21, 0,
	23, 0,
	26, 0,
	29, 0,
	32, 0,
	-2, 118,
	};
# define YYNPROD 141
# define YYLAST 464
yytabelem yyact[]={

    88,   105,   113,    59,    46,   116,   101,    42,   112,   109,
   236,   107,    59,    30,   110,    46,   104,   108,    99,   100,
   111,    42,   106,   114,   102,    98,    19,    83,   103,   115,
    24,    58,    14,    19,    13,    16,   221,    24,   214,   212,
    80,    52,   140,    77,    66,    54,    48,    18,   189,   132,
   150,    63,    51,    64,    21,    76,    65,    60,    70,    24,
   240,    58,   147,   190,    93,    71,   219,    94,    69,   134,
    20,    15,   244,   245,   246,   247,   248,    20,   134,    49,
    72,   265,    34,    67,   101,    45,    68,    39,   262,    89,
    23,    22,   180,    41,    89,   261,    99,   100,   260,   259,
   191,   224,   102,    98,   258,    33,    35,    36,    37,   203,
   131,   238,    73,    75,    74,   202,   232,   201,   200,   135,
   136,   118,   119,   120,   123,    96,    95,    89,   135,   136,
    81,    20,   186,   149,    78,    18,   143,   179,    62,    61,
   188,   138,   196,   241,   185,   184,   152,   153,   194,   193,
   154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
   164,   165,   166,   167,   168,   169,   170,   172,    84,   105,
   113,   126,   125,   171,   101,   124,   112,   109,   122,   107,
   121,    86,   110,   182,   104,   108,    99,   100,   111,   101,
   106,   192,   102,    98,    89,    89,   103,   195,    91,   104,
   198,    99,   100,    84,   197,   228,   101,   102,    98,    90,
    85,   103,   233,    29,    26,   187,    87,    25,   239,   100,
   222,   273,   267,   263,   102,   146,   215,   234,    10,   207,
   251,   216,   205,    18,   143,   220,   183,   181,   177,    89,
   231,   243,   227,   128,   117,    31,    79,    38,   225,   178,
   242,   235,   127,   264,   250,    76,   271,    56,    47,    24,
   151,    43,   274,   237,   269,   266,    56,   268,   254,    47,
   272,    97,   252,   253,   229,    43,   210,   255,   209,   257,
    12,   105,   113,   208,   204,   175,   101,   145,   112,   109,
   137,   107,    28,   256,   110,   213,   104,   108,    99,   100,
   111,   230,   106,   114,   102,    98,   218,   249,   103,   105,
   113,   176,   144,   174,   101,     9,   112,   109,   129,   107,
    92,   217,   110,   142,   104,   108,    99,   100,   111,   141,
   106,   114,   102,    98,   139,   226,   103,   105,   113,    55,
    57,   148,   101,    53,   112,   109,    50,   107,   211,   133,
   110,   206,   104,   108,    99,   100,   111,   130,   106,   114,
   102,    98,    27,   199,   103,   105,   113,    44,    17,    40,
   101,    11,   112,   109,     8,   107,     7,     6,   110,     5,
   104,   108,    99,   100,   111,     4,   106,   114,   102,    98,
   105,   173,   103,     3,     2,   101,     1,   112,   109,   270,
   107,   223,    82,   110,    32,   104,   108,    99,   100,   111,
     0,   106,     0,   102,    98,   105,     0,   103,     0,     0,
   101,     0,   112,   109,     0,   107,     0,     0,   110,     0,
   104,   108,    99,   100,   111,     0,     0,     0,   102,    98,
     0,   101,   103,   112,   109,     0,   107,     0,     0,   110,
     0,   104,   108,    99,   100,   111,     0,     0,     0,   102,
    98,     0,     0,   103 };
yytabelem yypact[]={

 -1000, -1000,    24, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000,   193,   190,   289,   189,   -11,    63, -1000,
 -1000,    85, -1000, -1000, -1000,    19,    13,    25, -1000,     1,
   101,   100,    53, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
     5, -1000,   230, -1000,     2, -1000,   176, -1000,   186,   154,
   178, -1000, -1000, -1000, -1000,   185, -1000,   174,   318,    64,
    64, -1000, -1000,   -12, -1000, -1000, -1000,    53,    53,    53,
   153,   151,    53,   148,   145,   144, -1000, -1000, -1000,   243,
 -1000, -1000,   227, -1000,   316,    62,   287, -1000,     1, -1000,
    31,    10, -1000,   255, -1000,   119,   255, -1000,    53,    53,
    53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
    53,    53,    53,    53,    53, -1000, -1000, -1000, -1000, -1000,
 -1000,    53,   253,   352,   311,   282,   309,   219,   240,    98,
    54,   218, -1000,    71,   217,   118,   117,    93, -1000,   177,
 -1000,    36,    36, -1000, -1000, -1000, -1000, -1000,   110, -1000,
    25,   115,    53,    25,   188,   188, -1000, -1000, -1000,    66,
    66,   423,   402,   171,   171,   171,   171,   171,   171,   377,
   156,   324,    79, -1000,    78,    76,    70,   281,   213, -1000,
   210,   280, -1000,   275,   273,   293, -1000,   207,    31, -1000,
   304,     3, -1000,    10,   197,    74,    53,   296,   226, -1000,
 -1000, -1000, -1000, -1000,   222,   271, -1000,   299, -1000, -1000,
   223,    77, -1000,   208,   197,     7, -1000,    72, -1000,    -5,
 -1000, -1000,   141,   225,    23,   268, -1000, -1000,   247,   222,
 -1000,   269, -1000,   293,   265, -1000, -1000, -1000,   207,   291,
   207, -1000, -1000, -1000,    65,    60,    59,    56,    49, -1000,
   204,   246,    42, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000,   262,   203, -1000,   222,   261,   250,   222,
 -1000,   202, -1000,   259, -1000 };
yytabelem yypgo[]={

     0,   312,   404,    27,    44,    51,   225,    38,    45,    36,
   402,    46,    64,    50,   401,    53,   399,   396,   394,   393,
   385,   379,   377,   376,   374,   371,   369,    93,     0,   368,
   367,    85,   362,   357,   351,    49,   349,   348,    39,   346,
    52,    41,   343,   341,   340,   339,   334,    42,   329,    48,
   323,   321,   271 };
yytabelem yyr1[]={

     0,    17,    18,    18,    19,    19,    19,    19,    19,    19,
    19,    19,    19,    20,    26,    26,    27,    27,    24,    24,
    29,    29,    28,    28,    16,    16,    21,    30,    30,    31,
    31,    10,    10,     3,    22,    33,    33,    35,    35,    36,
    36,    36,    34,    34,    37,    37,    37,    38,    32,    32,
    23,    39,    39,    40,    40,    40,    43,    43,    42,    44,
     8,    41,    41,    45,    11,    11,    12,    12,    12,    13,
    13,    14,    14,    14,    14,    14,    14,    46,    46,    46,
    47,    47,    47,    47,    47,    48,    50,    49,    49,    49,
    51,    51,     9,     9,     9,     7,     7,     7,     1,     2,
     2,     2,     2,     2,     5,     5,     5,     5,     5,     5,
     5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
     5,     5,    52,    52,    52,    15,    15,    15,    15,    15,
    15,    15,    15,    15,    15,    15,     4,     4,     6,     6,
    25 };
yytabelem yyr2[]={

     0,     3,     4,     1,     2,     2,     2,     2,     2,     3,
     3,     2,     3,     9,     2,     5,    23,     3,     7,     7,
     3,     3,     2,     1,     7,     1,     9,     2,     5,    23,
     3,     3,     1,     7,    15,     7,     2,     4,     0,     7,
    13,     8,     5,     0,     2,     6,     0,     7,     3,     0,
     9,     2,     6,     2,     2,     2,     2,     6,    11,    11,
     5,    13,     3,    13,     9,     1,     3,     9,     1,     9,
     1,     7,     7,     7,     7,     7,     1,     2,     6,     0,
     4,     4,     3,     3,     3,     3,     3,     9,     9,     0,
     3,     7,     5,     5,     1,     5,     1,     5,     9,     3,
     3,     3,     3,     3,     7,     7,     7,     7,     7,     7,
     7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
     7,     3,     2,     2,     3,     3,     3,     5,     5,     5,
     9,     9,     7,     9,     9,     9,     3,     3,     3,     3,
     5 };
yytabelem yychk[]={

 -1000,   -17,   -18,   -19,   -20,   -21,   -22,   -23,   -24,    -1,
    -6,   -25,   256,    10,     8,    47,    11,   -29,    -4,     2,
    46,    30,    67,    66,     6,    24,    24,   -32,     3,    24,
    24,   256,    -2,    42,    19,    43,    44,    45,    -6,     2,
   -26,   -27,     2,   256,   -30,   -31,     2,   256,   -11,    54,
   -39,   -40,   -41,   -42,    -8,   -45,   256,   -44,    30,     2,
    56,    38,    38,    -5,   -15,     3,    -4,    30,    33,    15,
     5,    12,    27,    59,    61,    60,     2,    38,   -27,    16,
    38,   -31,   -10,    -3,    27,    24,    27,    38,   -28,    17,
    24,    24,     2,   -12,     3,    62,   -12,   -52,    37,    30,
    31,    18,    36,    40,    28,    13,    34,    23,    29,    21,
    26,    32,    20,    14,    35,    41,    17,   256,   -15,   -15,
   -15,    27,    27,    -5,    27,    27,    27,     9,    16,     2,
   -33,    48,   -35,   -36,     7,    57,    58,     3,   -40,   -46,
   -47,   -48,   -50,    -8,    -1,   256,    -6,    31,   -43,   -41,
   -13,     5,    27,   -13,    -5,    -5,    -5,    -5,    -5,    -5,
    -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,
    -5,    -5,    -4,    39,     2,     3,     2,    19,     9,    39,
    38,    19,   -35,    19,    27,    27,    39,    38,   -28,   -49,
    27,    64,   -49,   -28,    38,   -11,    27,    -5,   -11,    39,
    39,    39,    39,    39,     3,    19,   -34,    19,     3,     3,
     3,   -37,   -38,     2,    -7,    19,   -47,   -51,     2,    63,
   -41,    -9,    23,   -14,    27,    -5,    39,    16,   -28,     3,
     2,    17,    39,   -28,    19,    -9,     3,   256,    39,   -28,
    65,     2,    -3,    16,    49,    50,    51,    52,    53,    39,
     7,   -28,     3,   -38,     3,    -7,     2,    -7,    39,    39,
    39,    39,    39,    19,     7,    39,     3,    19,   -28,     3,
   -16,     6,   -28,    19,     3 };
yytabelem yydef[]={

     3,    -2,    -2,     2,     4,     5,     6,     7,     8,     9,
    10,    11,    12,     0,     0,    49,     0,     0,     0,    -2,
   139,     0,    20,    21,   137,     0,     0,    65,    48,     0,
     0,     0,     0,    99,   100,   101,   102,   103,   140,   138,
     0,    14,     0,    17,     0,    27,    32,    30,     0,     0,
    23,    51,    53,    54,    55,     0,    62,     0,     0,    68,
    68,    18,    19,     0,   121,   125,   126,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   136,    13,    15,     0,
    26,    28,     0,    31,     0,    38,     0,    50,     0,    22,
    -2,     0,    60,    70,    66,     0,    70,    98,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   122,   123,   124,   127,   128,
   129,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    36,    38,     0,     0,     0,     0,    52,    23,
    77,    89,    89,    82,    83,    84,    85,    86,    23,    56,
    65,     0,     0,    65,   104,   105,   106,   107,   108,   109,
   110,   111,   112,    -2,    -2,    -2,    -2,    -2,    -2,   119,
   120,     0,     0,   132,     0,     0,     0,     0,     0,    33,
    43,     0,    37,     0,     0,    46,    64,    96,     0,    80,
     0,     0,    81,     0,    94,    76,     0,     0,     0,   130,
   131,   133,   134,   135,    23,     0,    34,     0,    35,    39,
     0,    23,    44,     0,    94,     0,    78,    23,    90,     0,
    57,    58,     0,     0,     0,     0,    67,    59,     0,    23,
    42,     0,    41,     0,     0,    61,    95,    97,    96,     0,
    96,    92,    93,    63,     0,     0,     0,     0,     0,    69,
     0,     0,     0,    45,    47,    87,    91,    88,    71,    72,
    73,    74,    75,     0,     0,    40,    23,     0,    25,    23,
    16,     0,    29,     0,    24 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"NAME",	2,
	"LONGINT",	3,
	"INT",	4,
	"ALIGN",	5,
	"DOT",	6,
	"LEN",	7,
	"MEMORY",	8,
	"ORG",	9,
	"REGIONS",	10,
	"SECTIONS",	11,
	"PHY",	12,
	"AND",	13,
	"ANDAND",	14,
	"BNOT",	15,
	"COLON",	16,
	"COMMA",	17,
	"DIV",	18,
	"EQ",	19,
	"EQEQ",	20,
	"GE",	21,
	"GT",	23,
	"LBRACE",	24,
	"LE",	26,
	"LPAREN",	27,
	"LSHIFT",	28,
	"LT",	29,
	"MINUS",	30,
	"MULT",	31,
	"NE",	32,
	"NOT",	33,
	"OR",	34,
	"OROR",	35,
	"PC",	36,
	"PLUS",	37,
	"RBRACE",	38,
	"RPAREN",	39,
	"RSHIFT",	40,
	"SEMICOL",	41,
	"DIVEQ",	42,
	"MINUSEQ",	43,
	"MULTEQ",	44,
	"PLUSEQ",	45,
	"FILENAME",	46,
	"TV",	47,
	"SPARE",	48,
	"DSECT",	49,
	"NOLOAD",	50,
	"COPY",	51,
	"INFO",	52,
	"OVERLAY",	53,
	"BLOCK",	54,
	"UMINUS",	55,
	"GROUP",	56,
	"RANGE",	57,
	"ASSIGN",	58,
	"SIZEOF",	59,
	"ADDR",	60,
	"NEXT",	61,
	"BIND",	62,
	"COMMN",	63,
	"LSQBRACK",	64,
	"RSQBRACK",	65,
	"EXPORT",	66,
	"HIDE",	67,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"wholefile : cmds",
	"cmds : cmds cmd",
	"cmds : /* empty */",
	"cmd : regspecs",
	"cmd : memspecs",
	"cmd : tvspecs",
	"cmd : secspecs",
	"cmd : changescope",
	"cmd : assnment",
	"cmd : filename",
	"cmd : flagstmt",
	"cmd : error",
	"regspecs : REGIONS LBRACE reglist RBRACE",
	"reglist : rgnspc",
	"reglist : reglist rgnspc",
	"rgnspc : NAME COLON ORG EQ LONGINT com LEN EQ LONGINT com vaddr",
	"rgnspc : error",
	"changescope : scopehead LBRACE RBRACE",
	"changescope : scopehead error RBRACE",
	"scopehead : HIDE",
	"scopehead : EXPORT",
	"com : COMMA",
	"com : /* empty */",
	"vaddr : DOT EQ LONGINT",
	"vaddr : /* empty */",
	"memspecs : MEMORY LBRACE memlist RBRACE",
	"memlist : memspc",
	"memlist : memlist memspc",
	"memspc : NAME opt_attr COLON ORG EQ LONGINT com LEN EQ LONGINT com",
	"memspc : error",
	"opt_attr : attr",
	"opt_attr : /* empty */",
	"attr : LPAREN NAME RPAREN",
	"tvspecs : TV opt_long opt_block LBRACE tvlist RBRACE tvfill",
	"tvlist : SPARE EQ LONGINT",
	"tvlist : tvdirlist",
	"tvdirlist : tvdir tvdirlist",
	"tvdirlist : /* empty */",
	"tvdir : LEN EQ LONGINT",
	"tvdir : RANGE LPAREN LONGINT COMMA LONGINT RPAREN",
	"tvdir : ASSIGN LPAREN slotlist RPAREN",
	"tvfill : EQ NAME",
	"tvfill : /* empty */",
	"slotlist : slotspec",
	"slotlist : slotlist com slotspec",
	"slotlist : /* empty */",
	"slotspec : NAME EQ LONGINT",
	"opt_long : LONGINT",
	"opt_long : /* empty */",
	"secspecs : SECTIONS LBRACE sglist RBRACE",
	"sglist : sec_or_grp",
	"sglist : sglist com sec_or_grp",
	"sec_or_grp : scnspc",
	"sec_or_grp : groupspc",
	"sec_or_grp : flgstmt2",
	"seclist : scnspc",
	"seclist : seclist com scnspc",
	"groupspc : grprfx LBRACE seclist RBRACE inclspc",
	"grprfx : GROUP opt_org opt_aln opt_block COLON",
	"flgstmt2 : MINUS NAME",
	"scnspc : scnprfx LBRACE scnstmts RBRACE fillspc inclspc",
	"scnspc : error",
	"scnprfx : NAME opt_org opt_aln opt_block opt_type COLON",
	"opt_block : BLOCK LPAREN LONGINT RPAREN",
	"opt_block : /* empty */",
	"opt_org : LONGINT",
	"opt_org : BIND LPAREN expr RPAREN",
	"opt_org : /* empty */",
	"opt_aln : ALIGN LPAREN expr RPAREN",
	"opt_aln : /* empty */",
	"opt_type : LPAREN DSECT RPAREN",
	"opt_type : LPAREN NOLOAD RPAREN",
	"opt_type : LPAREN COPY RPAREN",
	"opt_type : LPAREN INFO RPAREN",
	"opt_type : LPAREN OVERLAY RPAREN",
	"opt_type : /* empty */",
	"scnstmts : scnstmt",
	"scnstmts : scnstmts com scnstmt",
	"scnstmts : /* empty */",
	"scnstmt : fname insecspc",
	"scnstmt : star insecspc",
	"scnstmt : flgstmt2",
	"scnstmt : assnment",
	"scnstmt : error",
	"fname : filename",
	"star : MULT",
	"insecspc : LPAREN inseclist RPAREN fillspc",
	"insecspc : LSQBRACK COMMN RSQBRACK fillspc",
	"insecspc : /* empty */",
	"inseclist : NAME",
	"inseclist : inseclist com NAME",
	"inclspc : GT NAME",
	"inclspc : GT attr",
	"inclspc : /* empty */",
	"fillspc : EQ LONGINT",
	"fillspc : /* empty */",
	"fillspc : EQ error",
	"assnment : ename assnop expr exend",
	"assnop : DIVEQ",
	"assnop : EQ",
	"assnop : MINUSEQ",
	"assnop : MULTEQ",
	"assnop : PLUSEQ",
	"expr : expr PLUS expr",
	"expr : expr MINUS expr",
	"expr : expr MULT expr",
	"expr : expr DIV expr",
	"expr : expr PC expr",
	"expr : expr RSHIFT expr",
	"expr : expr LSHIFT expr",
	"expr : expr AND expr",
	"expr : expr OR expr",
	"expr : expr GT expr",
	"expr : expr LT expr",
	"expr : expr GE expr",
	"expr : expr LE expr",
	"expr : expr NE expr",
	"expr : expr EQEQ expr",
	"expr : expr ANDAND expr",
	"expr : expr OROR expr",
	"expr : term",
	"exend : SEMICOL",
	"exend : COMMA",
	"exend : error",
	"term : LONGINT",
	"term : ename",
	"term : MINUS term",
	"term : NOT term",
	"term : BNOT term",
	"term : ALIGN LPAREN expr RPAREN",
	"term : PHY LPAREN ename RPAREN",
	"term : LPAREN expr RPAREN",
	"term : SIZEOF LPAREN NAME RPAREN",
	"term : NEXT LPAREN LONGINT RPAREN",
	"term : ADDR LPAREN NAME RPAREN",
	"ename : NAME",
	"ename : DOT",
	"filename : NAME",
	"filename : FILENAME",
	"flagstmt : MINUS filename",
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
		
case 1:
# line 189 "./../../ld/common/ld.yac"
 ; break;
case 3:
# line 193 "./../../ld/common/ld.yac"
 ; break;
case 9:
# line 200 "./../../ld/common/ld.yac"
 {
			bldexp(yypvt[-0].enode, &explist);
			} break;
case 10:
# line 203 "./../../ld/common/ld.yac"
 {
			filespec( savefn( yypvt[-0].sptr ) );
			} break;
case 12:
# line 207 "./../../ld/common/ld.yac"
 {
			for( tempi = lineno; tempi == lineno; )
				yychar = yylex();
			yyerrok;
			yyclearin;
			} break;
case 13:
# line 215 "./../../ld/common/ld.yac"
 {
#if USEREGIONS == 0
			yyerror("REGIONS command not allowed in a %s load", SGS);
#endif
			} break;
case 15:
# line 223 "./../../ld/common/ld.yac"
 ; break;
case 16:
# line 225 "./../../ld/common/ld.yac"
 {
#if USEREGIONS
			long *orgp, *lenp, *vaddrp, vzero;
			REGION *rgnp;
			orgp = yypvt[-6].lptr;
			lenp = yypvt[-2].lptr;
			if (*lenp <= 0)
				yyerror("region %s has invalid length",yypvt[-10].sptr);
			if ( (vaddrp = yypvt[-0].lptr) == NULL )
				vzero = *orgp;
			else
				vzero = *orgp - *vaddrp;
			if ( (vzero & 0xfL) != 0 ) {
				yyerror("virtual 0 (paddr %.1lx) of region %s is not a multiple of 16",
					vzero, yypvt[-10].sptr);
				vzero &= ~0xfL;
				}
			rgnp = (REGION *) myalloc(sizeof(REGION));
			rgnp->rgorig = *orgp;
			rgnp->rglength = *lenp;
			rgnp->rgvaddr = (vaddrp == NULL) ? 0L : *vaddrp;
			if( (rgnp->rgvaddr + rgnp->rglength) > 0x10000L ) {
				yyerror("addresses of region %s (vaddr=%.1lx  len=%.1lx) exceed 64K",
					yypvt[-10].sptr, rgnp->rgvaddr, *lenp);
				rgnp->rglength = 0x10000L - rgnp->rgvaddr;
				}
			strncpy( rgnp->rgname, yypvt[-10].sptr, 8 );
			listadd(l_REG, &reglist, rgnp);
#endif
			} break;
case 17:
# line 255 "./../../ld/common/ld.yac"
 {
			yyerror("REGIONS specification ignored");
			} break;
case 18:
# line 260 "./../../ld/common/ld.yac"
 {
 			scope_directive = __undefined;
 			} break;
case 19:
# line 263 "./../../ld/common/ld.yac"
 {
 			yyerror("Error in scope directive, skipping past '}'");
 			scope_directive = __undefined;
 			} break;
case 20:
# line 269 "./../../ld/common/ld.yac"
 {
 			scope_directive = __hidden;
 			} break;
case 21:
# line 272 "./../../ld/common/ld.yac"
 {
 			scope_directive = __exported;;
			} break;
case 23:
# line 279 "./../../ld/common/ld.yac"
 ; break;
case 24:
# line 281 "./../../ld/common/ld.yac"
 {
#if USEREGIONS
			if ( *(long *) yypvt[-0].lptr > 0xffffL ) {
				yyerror("virtual address %.1lx exceeds 0xffff: truncated to %.1lx",
					*(long *) yypvt[-0].lptr, *(long *) yypvt[-0].lptr & 0xffffL);
				*(long *) yypvt[-0].lptr &= 0xffffL;
				}
			yyval.lptr = yypvt[-0].lptr;
#endif
			} break;
case 25:
# line 292 "./../../ld/common/ld.yac"
 {
			yyval.lptr = NULL;
			} break;
case 26:
# line 297 "./../../ld/common/ld.yac"
 
		; break;
case 28:
# line 301 "./../../ld/common/ld.yac"
 ; break;
case 29:
# line 303 "./../../ld/common/ld.yac"
 {
			long *orgp, *lenp;
			MEMTYPE *memp;
			orgp = yypvt[-5].lptr;
			lenp = yypvt[-1].lptr;
			memp = (MEMTYPE *) mycalloc(sizeof(MEMTYPE));
			memp->mtorig = *orgp;
			memp->mtlength = *lenp;
			memp->mtattr = yypvt[-9].ivalue;
			strncpy( memp->mtname, yypvt[-10].sptr, 8 );
			listadd(l_MEM, &memlist, memp);
			} break;
case 30:
# line 315 "./../../ld/common/ld.yac"
 {
			yyerror("MEMORY specification ignored");
			} break;
case 31:
# line 320 "./../../ld/common/ld.yac"
 {
			yyval.ivalue = yypvt[-0].ivalue;
			} break;
case 32:
# line 323 "./../../ld/common/ld.yac"
 {			/* empty */
			yyval.ivalue = att_R | att_W | att_X | att_I;
			} break;
case 33:
# line 328 "./../../ld/common/ld.yac"
  { 
			int attflgs;
			char *p;
			attflgs = 0;
			for( p = yypvt[-1].sptr; *p; p++ )
				switch (*p) {
				case 'R': attflgs |= att_R; break;
				case 'W': attflgs |= att_W; break;
				case 'X': attflgs |= att_X; break;
				case 'I': attflgs |= att_I; break;
				default:  yyerror("bad attribute value in MEMORY directive: %c", *p);
				}
			yyval.ivalue = attflgs;
			} break;
case 34:
# line 344 "./../../ld/common/ld.yac"
 {
#if TRVEC
			int length;

			tvspec.tvinflnm = curfilnm;
			tvspec.tvinlnno = lineno;
			tvspec.tvosptr->osblock = *(yypvt[-4].lptr);

			length = 0;
			for( slotptr = tvslot1; slotptr != NULL; slotptr = slotptr->nxtslot ) {
				TVASSIGN *slotptr2;
				length = max(length, slotptr->slot);
				for (slotptr2 = tvslot1; slotptr2 != slotptr; slotptr2 = slotptr2->nxtslot )
					if (slotptr2->slot == slotptr->slot) {
						lderror(1,0,NULL,
						"function %s assigned to tv slot %d which is already in use", slotptr->funname, slotptr->slot);
						break;
						}
				}
			if( tvspec.tvlength > 0 ) {
				if( length > tvspec.tvlength ) {
					yyerror("ASSIGN slot %d exceeds total TV size of %d",
						length, tvspec.tvlength);
					tvspec.tvlength = length;
					}
				if( tvspec.tvrange[1] > tvspec.tvlength ) {
					yyerror("RANGE value %d exceeds total TV size of %d",
						tvspec.tvrange[1], tvspec.tvlength);
					tvspec.tvlength = tvspec.tvrange[1];
					}
				}
			else
				tvspec.tvlength = max(length, tvspec.tvrange[1]);
#else
			yyerror("usage of unimplemented syntax");
#endif
			} break;
case 35:
# line 383 "./../../ld/common/ld.yac"
 {
#if TRVEC
			tvspec.tvosptr->oshdr.s_size += *(yypvt[-0].lptr) * TVENTSZ;
#else
			yyerror("usage of unimplemented syntax");
#endif
			} break;
case 39:
# line 397 "./../../ld/common/ld.yac"
 {
#if TRVEC
			if( tvspec.tvlength > 0 )
				yyerror("illegal multiple LENGTH fields in the TV directive");
			tvspec.tvlength = (int) (*(long *) yypvt[-0].lptr);
#else
			yyerror("usage of unimplemented syntax");
#endif
			} break;
case 40:
# line 406 "./../../ld/common/ld.yac"
 {
#if TRVEC
			if( tvspec.tvrange[1] > 0 )
				yyerror("illegal multiple RANGE fields in the TV directive");
			if( tvspec.tvrange[0] < 0 || tvspec.tvrange[0] > tvspec.tvrange[1] )
				yyerror("illegal RANGE syntax: r[0]<0 or r[0]>r[1]");
			else {
				tvspec.tvrange[0] = (int) (*(long *) yypvt[-3].lptr);
				tvspec.tvrange[1] = (int) (*(long *) yypvt[-1].lptr);
				}
#else
			yyerror("usage of unimplemented syntax");
#endif
			} break;
case 42:
# line 423 "./../../ld/common/ld.yac"
 {
#if TRVEC
#if FLEXNAMES
			tvspec.tvfnfill = savefn(yypvt[-0].sptr);
#else
			strncpy(tvspec.tvfnfill, yypvt[-0].sptr, 8 );
#endif
#else
			yyerror("usage of unimplemented syntax");
#endif
			} break;
case 47:
# line 441 "./../../ld/common/ld.yac"
 {
#if TRVEC
			if( (int) (*(long *) yypvt[-0].lptr) == 0 )
				yyerror("illegal ASSIGN slot number (0)");
			if( tvslot1 == NULL )
				tvslot1 = tvslotn = (TVASSIGN *) myalloc(sizeof(TVASSIGN));
			else
				tvslotn = tvslotn->nxtslot = (TVASSIGN *) myalloc(sizeof(TVASSIGN));
#if FLEXNAMES
			tvslotn->funname = savefn(yypvt[-2].sptr);
#else
			strncpy(tvslotn->funname, yypvt[-2].sptr, 8);
#endif
			tvslotn->slot = (int) (*(long *) yypvt[-0].lptr);
			tvslotn->nxtslot = NULL;
#else
			yyerror("usage of unimplemented syntax");
#endif
		} break;
case 48:
# line 462 "./../../ld/common/ld.yac"
 {
			long org;
			org = *(long *) yypvt[-0].lptr;
#if TRVEC
			chktvorg(org, &(tvspec.tvbndadr));
#endif
			} break;
case 50:
# line 472 "./../../ld/common/ld.yac"
 
		; break;
case 58:
# line 488 "./../../ld/common/ld.yac"
 {
			listadd(l_AI,&bldoutsc,grptr);
			grptr = NULL;
			} break;
case 59:
# line 494 "./../../ld/common/ld.yac"
 {
			if ( yypvt[-3].enode != NULL && yypvt[-2].enode != NULL )
				yyerror("bonding excludes alignment");
			grptr = dfnscngrp(AIDFNGRP, yypvt[-3].enode, yypvt[-2].enode, (long *)yypvt[-1].lptr);
			strncpy( grptr->dfnscn.ainame, "*group*", 8 );
			} break;
case 60:
# line 502 "./../../ld/common/ld.yac"
 {
			char *fp;
			fp = yypvt[-0].sptr;
			if (fp[0] == 'l')	/* library flag */
				fnamptr = library(fp);
			else
				yyerror("bad flag value in SECTIONS directive: -%s", yypvt[-0].sptr);
			} break;
case 61:
# line 512 "./../../ld/common/ld.yac"
 {
			if (aiptr == NULL)
				goto scnerr;
			aiptr->dfnscn.aifill = yypvt[-1].ivalue;
			aiptr->dfnscn.aifillfg = fillflag;
			listadd(l_AI,grptr ? &grptr->dfnscn.sectspec : &bldoutsc,aiptr);
			if (grptr && yypvt[-0].ivalue)
				yyerror("can not specify an owner for section within a group");
			aiptr = NULL;
		} break;
case 62:
# line 522 "./../../ld/common/ld.yac"
 {
			if (aiptr)
				yyerror("section %s not built", aiptr->dfnscn.ainame);
		scnerr:
			aiptr = NULL;
			} break;
case 63:
# line 530 "./../../ld/common/ld.yac"
 {
			secnum++;
			if (!strncmp(yypvt[-5].sptr,_TV,8))
				yyerror("%s is a reserved section name", yypvt[-5].sptr);
			if ( yypvt[-4].enode != NULL && yypvt[-3].enode != NULL )
				yyerror("bonding excludes alignment");
			if ( yypvt[-1].ivalue == (STYP_OVER | STYP_DSECT) && yypvt[-4].enode == NULL)
				yyerror("OVERLAY section must be BINDed");
			aiptr = dfnscngrp(AIDFNSCN, yypvt[-4].enode, yypvt[-3].enode, (long *)yypvt[-2].lptr);
			if (grptr && aiptr->dfnscn.aibndadr)
				yyerror("can not bond a section within a group");
			if (grptr && aiptr->dfnscn.aialign)
				yyerror("can not align a section within a group");
			aiptr->dfnscn.aisctype = yypvt[-1].ivalue;
			strncpy( aiptr->dfnscn.ainame, yypvt[-5].sptr, 8 );
			} break;
case 64:
# line 548 "./../../ld/common/ld.yac"
 {
			yyval.lptr = yypvt[-1].lptr;
			} break;
case 65:
# line 551 "./../../ld/common/ld.yac"
 {			/* empty */
			yyval.lptr = NULL;
			} break;
case 66:
# line 556 "./../../ld/common/ld.yac"
 {
			yyval.enode = cnstnode(* (long *) yypvt[-0].lptr);
			} break;
case 67:
# line 559 "./../../ld/common/ld.yac"
 {
			yyval.enode = yypvt[-1].enode;
			} break;
case 68:
# line 562 "./../../ld/common/ld.yac"
 {			/* empty */
			yyval.enode = NULL;
			} break;
case 69:
# line 567 "./../../ld/common/ld.yac"
 {
			yyval.enode = yypvt[-1].enode;
			} break;
case 70:
# line 570 "./../../ld/common/ld.yac"
 {			/* empty */
			yyval.enode = NULL;
			} break;
case 71:
# line 575 "./../../ld/common/ld.yac"
 {
			yyval.ivalue = STYP_DSECT;
			} break;
case 72:
# line 578 "./../../ld/common/ld.yac"
 {
			yyval.ivalue = STYP_NOLOAD;
			} break;
case 73:
# line 581 "./../../ld/common/ld.yac"
 {
			yyval.ivalue = STYP_COPY | STYP_DSECT;
			} break;
case 74:
# line 584 "./../../ld/common/ld.yac"
 {
			yyval.ivalue = STYP_INFO | STYP_DSECT;
			} break;
case 75:
# line 587 "./../../ld/common/ld.yac"
 {
			yyval.ivalue = STYP_OVER | STYP_DSECT;
			} break;
case 76:
# line 590 "./../../ld/common/ld.yac"
 {			/* empty */
			yyval.ivalue = STYP_REG;
			} break;
case 82:
# line 602 "./../../ld/common/ld.yac"
 {
			bldadfil( fnamptr, aiptr );
			} break;
case 83:
# line 605 "./../../ld/common/ld.yac"
 {
			bldexp(yypvt[-0].enode,&aiptr->dfnscn.sectspec);
			} break;
case 84:
# line 608 "./../../ld/common/ld.yac"
 {
			in_y_exp = 0;
			yyerror("statement ignored");
			for( tempi = lineno; ((int)yychar > 0) && (tempi == lineno) ; )
				yychar = yylex();
			if ((int) yychar <= 0)
				lderror(2,0,NULL, "unexpected EOF");
			yyerrok;
			yyclearin;
			} break;
case 85:
# line 620 "./../../ld/common/ld.yac"

		{
			fnamptr = savefn(yypvt[-0].sptr);
			nsecspcs = 0;
			filespec( fnamptr );
			afaiptr = bldadfil(fnamptr,aiptr);
		} break;
case 86:
# line 628 "./../../ld/common/ld.yac"
 {
			nsecspcs = 0;
			fnamptr = (char *) 0;
			afaiptr = bldadfil(fnamptr, aiptr);
		} break;
case 87:
# line 634 "./../../ld/common/ld.yac"
 {
			afaiptr->adfile.ainadscs = nsecspcs;
			afaiptr->adfile.aifilflg = fillflag;
			afaiptr->adfile.aifill2 = yypvt[-0].ivalue;
			} break;
case 88:
# line 639 "./../../ld/common/ld.yac"
 {
			COMFILE *comfile;
			if (!fnamptr)
				yyerror("*[COMMON] may not be used (hint: try *(.bss))");
			comfile = (COMFILE *)commlist.head;
			while (comfile) {
				if (!strcmp(fnamptr,comfile->fnamptr))
					lderror(2,0,NULL,"COMMON symbols from file %s already used",
						fnamptr);
				comfile=comfile->comnext;
				}
			afaiptr->adfile.ainadscs = 1;
			afaiptr->adfile.aifilflg = fillflag;
			afaiptr->adfile.aifill2 = yypvt[-0].ivalue;
			bldadscn(".common",fnamptr,aiptr);
			comfile = (COMFILE *) myalloc(sizeof(COMFILE));
			comfile->fnamptr = fnamptr;
			comfile->dfnscnptr = aiptr;
			listadd(l_CM,&commlist,comfile);
			} break;
case 90:
# line 662 "./../../ld/common/ld.yac"
 {
			nsecspcs++;
			bldadscn(yypvt[-0].sptr,fnamptr,aiptr);
			} break;
case 91:
# line 666 "./../../ld/common/ld.yac"
 {
			nsecspcs++;
			bldadscn(yypvt[-0].sptr,fnamptr,aiptr);
			} break;
case 92:
# line 672 "./../../ld/common/ld.yac"
 {
			if (grptr)
				strncpy( grptr->dfnscn.aiowname, yypvt[-0].sptr, 8 );
			else
				strncpy( aiptr->dfnscn.aiowname, yypvt[-0].sptr, 8 );
			yyval.ivalue = 1;
			} break;
case 93:
# line 679 "./../../ld/common/ld.yac"
 {
			if (grptr)
				grptr->dfnscn.aiattown = yypvt[-0].ivalue;
			else
				aiptr->dfnscn.aiattown = yypvt[-0].ivalue;
			yyval.ivalue = 1;
			} break;
case 94:
# line 686 "./../../ld/common/ld.yac"
 {				/* empty */ 
			yyval.ivalue = 0;
			} break;
case 95:
# line 691 "./../../ld/common/ld.yac"
 {
			fillflag = 1;
			yyval.ivalue = (int) (*(long *)yypvt[-0].lptr);
			} break;
case 96:
# line 695 "./../../ld/common/ld.yac"
 {				/* empty */
			fillflag = 0;
			yyval.ivalue = 0;
			} break;
case 97:
# line 699 "./../../ld/common/ld.yac"
 {
			yyerror("bad fill value");
			fillflag = 0;
			yyerrok;
			yyclearin;
			yyval.ivalue = 0;
			} break;
case 98:
# line 708 "./../../ld/common/ld.yac"
 {
			in_y_exp = 0;
			if ( yypvt[-2].ivalue == EQ )
				yyval.enode = buildtree(EQ, yypvt[-3].enode, yypvt[-1].enode);
			else {
				ENODE *p,*ndp;
				ndp = buildtree(yypvt[-2].ivalue,yypvt[-3].enode,yypvt[-1].enode);
				p = (ENODE *) myalloc(sizeof(ENODE));
				*p = *yypvt[-3].enode;
				yyval.enode = buildtree(EQ,p,ndp);
				}
			} break;
case 99:
# line 722 "./../../ld/common/ld.yac"
		{ yyval.ivalue = DIV;	in_y_exp = TRUE; } break;
case 100:
# line 723 "./../../ld/common/ld.yac"
		{ yyval.ivalue = EQ;	in_y_exp = TRUE; } break;
case 101:
# line 724 "./../../ld/common/ld.yac"
	{ yyval.ivalue = MINUS;	in_y_exp = TRUE; } break;
case 102:
# line 725 "./../../ld/common/ld.yac"
	{ yyval.ivalue = MULT;	in_y_exp = TRUE; } break;
case 103:
# line 726 "./../../ld/common/ld.yac"
	{ yyval.ivalue = PLUS;	in_y_exp = TRUE; } break;
case 104:
# line 729 "./../../ld/common/ld.yac"
 {
			bop:
				yyval.enode = buildtree(yypvt[-1].ivalue,yypvt[-2].enode,yypvt[-0].enode);
			} break;
case 105:
# line 733 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 106:
# line 734 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 107:
# line 735 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 108:
# line 736 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 109:
# line 737 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 110:
# line 738 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 111:
# line 739 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 112:
# line 740 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 113:
# line 741 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 114:
# line 742 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 115:
# line 743 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 116:
# line 744 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 117:
# line 745 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 118:
# line 746 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 119:
# line 747 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 120:
# line 748 "./../../ld/common/ld.yac"
 { goto bop; } break;
case 121:
# line 749 "./../../ld/common/ld.yac"
 {
			yyval.enode = yypvt[-0].enode;
			} break;
case 124:
# line 756 "./../../ld/common/ld.yac"
 {
			yyerror ("semicolon required after expression");
			} break;
case 125:
# line 761 "./../../ld/common/ld.yac"
 {
			yyval.enode = cnstnode(*(long *) yypvt[-0].lptr);
			} break;
case 126:
# line 764 "./../../ld/common/ld.yac"
 {
			yyval.enode = yypvt[-0].enode;
			} break;
case 127:
# line 767 "./../../ld/common/ld.yac"
 {
			yyval.enode = buildtree(UMINUS,yypvt[-0].enode,NULL);
			} break;
case 128:
# line 770 "./../../ld/common/ld.yac"
 {
			yyval.enode = buildtree(yypvt[-1].ivalue,yypvt[-0].enode,NULL);
			} break;
case 129:
# line 773 "./../../ld/common/ld.yac"
 {
			yyval.enode = buildtree(BNOT,yypvt[-0].enode,NULL);
			} break;
case 130:
# line 776 "./../../ld/common/ld.yac"
 {
			yyval.enode = buildtree(yypvt[-3].ivalue,yypvt[-1].enode,NULL);
			} break;
case 131:
# line 779 "./../../ld/common/ld.yac"
 {
			yyval.enode = buildtree(yypvt[-3].ivalue,yypvt[-1].enode,NULL);
			} break;
case 132:
# line 782 "./../../ld/common/ld.yac"
 {
			yyval.enode = yypvt[-1].enode;
			} break;
case 133:
# line 785 "./../../ld/common/ld.yac"
 {
			yyval.enode = sectnode( yypvt[-3].ivalue, yypvt[-1].sptr );
			} break;
case 134:
# line 788 "./../../ld/common/ld.yac"
 {
			yyval.enode = cnstnode( *(long *) yypvt[-1].lptr );
			yyval.enode->vnode.exop = NEXT;
			} break;
case 135:
# line 792 "./../../ld/common/ld.yac"
 {
			yyval.enode = sectnode( yypvt[-3].ivalue, yypvt[-1].sptr );
			} break;
case 136:
# line 797 "./../../ld/common/ld.yac"
	{
			yyval.enode = symnode(yypvt[-0].sptr);
			} break;
case 137:
# line 800 "./../../ld/common/ld.yac"
	{
			yyval.enode = symnode(NULL);
			} break;
case 138:
# line 805 "./../../ld/common/ld.yac"
 {
			yyval.sptr = yypvt[-0].sptr;
			} break;
case 139:
# line 808 "./../../ld/common/ld.yac"
 {
			yyval.sptr = yypvt[-0].sptr;
			} break;
case 140:
# line 813 "./../../ld/common/ld.yac"
 {
			pflags(yypvt[-0].sptr, TRUE);
			} break;
	}
	goto yystack;		/* reset registers in driver code */
}
