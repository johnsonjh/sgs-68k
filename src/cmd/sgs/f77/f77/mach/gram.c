# define SEOS 3
# define SCOMMENT 4
# define SLABEL 5
# define SUNKNOWN 6
# define SHOLLERITH 7
# define SICON 8
# define SRCON 9
# define SDCON 10
# define SBITCON 11
# define SOCTCON 12
# define SHEXCON 13
# define STRUE 14
# define SFALSE 15
# define SNAME 16
# define SNAMEEQ 17
# define SFIELD 18
# define SSCALE 19
# define SINCLUDE 20
# define SLET 21
# define SASSIGN 22
# define SAUTOMATIC 23
# define SBACKSPACE 24
# define SBLOCK 25
# define SCALL 26
# define SCHARACTER 27
# define SCLOSE 28
# define SCOMMON 29
# define SCOMPLEX 30
# define SCONTINUE 31
# define SDATA 32
# define SDCOMPLEX 33
# define SDIMENSION 34
# define SDO 35
# define SDOUBLE 36
# define SELSE 37
# define SELSEIF 38
# define SEND 39
# define SENDFILE 40
# define SENDIF 41
# define SENTRY 42
# define SEQUIV 43
# define SEXTERNAL 44
# define SFORMAT 45
# define SFUNCTION 46
# define SGOTO 47
# define SASGOTO 48
# define SCOMPGOTO 49
# define SARITHIF 50
# define SLOGIF 51
# define SIMPLICIT 52
# define SINQUIRE 53
# define SINTEGER 54
# define SINTRINSIC 55
# define SLOGICAL 56
# define SNAMELIST 57
# define SOPEN 58
# define SPARAM 59
# define SPAUSE 60
# define SPRINT 61
# define SPROGRAM 62
# define SPUNCH 63
# define SREAD 64
# define SREAL 65
# define SRETURN 66
# define SREWIND 67
# define SSAVE 68
# define SSTATIC 69
# define SSTOP 70
# define SSUBROUTINE 71
# define STHEN 72
# define STO 73
# define SUNDEFINED 74
# define SWRITE 75
# define SLPAR 76
# define SRPAR 77
# define SEQUALS 78
# define SCOLON 79
# define SCOMMA 80
# define SCURRENCY 81
# define SPLUS 82
# define SMINUS 83
# define SSTAR 84
# define SSLASH 85
# define SPOWER 86
# define SCONCAT 87
# define SAND 88
# define SOR 89
# define SNEQV 90
# define SEQV 91
# define SNOT 92
# define SEQ 93
# define SLT 94
# define SGT 95
# define SLE 96
# define SGE 97
# define SNE 98

# line 99 "gram.in"
#	include "defs"

#ifdef SDB
#	include <a.out.h>
extern int fnstline;
#	if TARGET == PDP11
char *stabline();
#	endif
#	ifdef UCBVAXASM
		char *stabdline();
#	endif
#endif

static int nstars;
static int ndim;
static int vartype;
static ftnint varleng;
static struct { expptr lb, ub; } dims[MAXDIM+1];
static struct Labelblock *labarray[MAXLABLIST];
static int lastwasbranch = NO;
static int thiswasbranch = NO;
extern ftnint yystno;
extern flag intonly;

ftnint convci();
double convcd();
Addrp nextdata();
expptr mklogcon(), mkaddcon(), mkrealcon(), mkstrcon(), mkbitcon();
expptr mkcxcon();
struct Listblock *mklist();
struct Listblock *mklist();
struct Impldoblock *mkiodo();
struct Extsym *comblock();


# line 137 "gram.in"
typedef union 	{
	int ival;
	char *charpval;
	chainp chval;
	tagptr tagval;
	expptr expval;
	struct Labelblock *labval;
	struct Nameblock *namval;
	struct Eqvchain *eqvval;
	struct Extsym *extval;
	} YYSTYPE;
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
yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 6,
	45, 12,
	-2, 11,
-1, 21,
	3, 33,
	-2, 217,
-1, 25,
	3, 37,
	-2, 217,
-1, 149,
	3, 233,
	-2, 181,
-1, 169,
	3, 254,
	80, 254,
	-2, 181,
-1, 220,
	79, 167,
	-2, 133,
-1, 242,
	76, 217,
	-2, 214,
-1, 268,
	3, 275,
	-2, 137,
-1, 272,
	3, 284,
	80, 284,
	-2, 139,
-1, 318,
	79, 168,
	-2, 135,
-1, 348,
	3, 256,
	16, 256,
	76, 256,
	80, 256,
	-2, 182,
-1, 411,
	93, 0,
	94, 0,
	95, 0,
	96, 0,
	97, 0,
	98, 0,
	-2, 147,
-1, 428,
	3, 278,
	80, 278,
	-2, 137,
-1, 430,
	3, 280,
	80, 280,
	-2, 137,
-1, 432,
	3, 282,
	80, 282,
	-2, 137,
-1, 484,
	80, 278,
	-2, 137,
	};
# define YYNPROD 289
# define YYLAST 1279
yytabelem yyact[]={

   234,   271,   231,   450,   441,   397,   449,   346,   390,   442,
   307,   448,   263,   389,   347,   287,   249,   219,   316,   127,
   197,   179,   275,   226,   261,   185,     6,   270,   201,   199,
   461,   267,   195,   115,   117,   290,    18,   193,   178,   208,
   326,   101,    94,   447,   257,    99,   103,   305,   446,   233,
   163,   164,   324,   325,   326,   332,   189,   120,    95,    96,
    97,   333,   335,   334,   337,   336,   338,   105,   324,   325,
   326,   110,   255,   256,   257,   156,   303,   156,   163,   164,
   324,   325,   326,   332,   331,   330,   119,   136,   118,   333,
   335,   334,   337,   336,   338,   129,   130,   131,   132,   297,
   134,   103,   113,   425,   296,   103,   491,   102,   163,   164,
   255,   256,   257,   258,   467,   119,   116,   118,   181,   182,
   128,   518,   360,   517,   154,   402,   154,   163,   164,   255,
   256,   257,   258,   494,   180,   163,   164,   324,   325,   326,
   188,   183,   227,   237,   237,   163,   164,   255,   256,   257,
   258,   504,   465,   156,   421,   466,   254,   156,   502,   240,
   236,   238,   285,   217,   212,   396,   216,   156,   210,   289,
   215,   100,   468,   289,   272,   272,   439,   422,   156,   440,
   421,   436,   102,   102,   102,   102,   211,   184,   103,   435,
   394,   434,   187,   395,   190,   191,   192,   276,   277,   278,
   381,   204,   154,   382,   274,   283,   154,   281,   282,   361,
   375,   310,   311,   376,   427,   269,   154,   190,   213,   214,
   314,   349,   318,   309,   350,   320,   293,   154,   323,   229,
   299,   418,   340,   242,   292,   244,   342,   343,   323,   304,
   302,   344,   284,   339,   321,   403,   254,   322,   265,   156,
   387,   386,   262,   367,   352,   156,   156,   156,   156,   156,
   254,   254,   163,   164,   324,   325,   326,   332,   366,   313,
   365,   102,   323,   266,   288,   340,   272,     5,   205,    98,
   254,   163,   164,   255,   256,   257,   172,   362,   345,   111,
   363,   364,   109,   116,   108,   308,   107,   106,   154,   104,
   190,   370,   371,   372,   154,   154,   154,   154,   154,   373,
   377,   323,   323,   100,   340,   323,   218,   399,   385,   323,
   374,   323,   247,   509,   227,   407,   408,   409,   410,   411,
   412,   413,   414,   415,   416,   220,   103,   494,   508,   507,
   503,   323,   506,   323,   323,   323,   406,   501,   495,   380,
   493,   156,   254,   323,   148,   473,   254,   254,   254,   254,
   254,   401,   475,   420,   103,   291,   186,   423,   272,   272,
   272,   301,   243,   239,   426,   223,   220,   232,   194,   209,
   190,   443,   207,   166,   140,   451,   319,   452,   472,    90,
   103,   393,   438,    30,   437,   391,   265,   149,   470,   169,
   154,   268,   268,   206,   237,   477,   469,   404,   323,   323,
   323,   323,   323,   323,   323,   323,   323,   323,   463,   474,
   478,   476,   135,   155,   200,   254,   122,   241,   480,   222,
   165,   167,   171,    92,   455,     7,   481,   272,   272,   272,
   426,   246,   443,   471,   323,   488,    80,    79,   451,    78,
   452,    77,   153,   168,   153,   492,    76,    75,   483,   490,
   341,    74,    59,    48,   288,   485,   486,   487,   482,    47,
    44,   323,   454,    32,   462,   464,   497,   237,   323,   500,
   496,   112,   203,   308,   505,   388,   202,   379,   198,   369,
   378,   511,   451,   384,   452,   513,   515,   455,   512,   510,
   383,   455,   499,   368,   133,   306,   323,   114,    27,    26,
    25,   198,   323,    24,    23,   462,   323,   516,    22,   519,
   323,    10,   462,   462,   462,     9,     8,     2,   294,    21,
   153,   453,    50,   520,   153,   454,   514,   286,   225,   454,
   317,   455,   341,   348,   153,   264,   315,   392,   163,   164,
   255,   256,   257,   258,   298,   153,   498,   196,    91,   464,
   252,    52,   159,   160,   161,   162,   327,    20,    54,   157,
   158,   103,   295,    36,     4,   221,   245,     3,   300,   454,
   259,   429,   431,   433,   159,   160,   161,   162,     1,     0,
   260,   157,   158,   103,     0,   428,   430,   432,     0,   489,
     0,   279,   163,   164,   324,   325,   326,   332,   331,   330,
   329,   328,     0,   333,   335,   334,   337,   336,   338,     0,
   159,   160,   161,   162,     0,     0,   153,   157,   158,   103,
     0,   230,   153,   153,   153,   153,   153,   163,   164,   353,
   264,   354,     0,   264,   264,     0,     0,   235,   479,     0,
   429,   431,   433,   230,   224,     0,     0,     0,     0,   163,
   164,   228,     0,     0,   484,   430,   432,     0,     0,   235,
     0,     0,   351,     0,     0,   198,     0,     0,   355,   356,
   357,   358,   359,   419,     0,     0,   341,     0,   163,   164,
   324,   325,   326,   332,   331,   330,   329,   328,     0,   333,
   335,   334,   337,   336,   338,   417,     0,     0,     0,     0,
   163,   164,   324,   325,   326,   332,   331,   330,   329,   328,
     0,   333,   335,   334,   337,   336,   338,     0,   153,     0,
     0,     0,     0,     0,     0,   405,     0,     0,     0,   264,
   163,   164,   324,   325,   326,   332,   331,   330,   329,   328,
     0,   333,   335,   334,   337,   336,   338,     0,     0,     0,
   445,     0,     0,     0,   460,     0,    13,     0,     0,     0,
     0,     0,     0,     0,   424,     0,     0,     0,     0,     0,
    11,    55,    45,    72,    84,    15,    60,    69,    89,    37,
    65,    46,    41,    67,    71,    31,    66,    34,    33,    12,
    86,    35,    19,    40,    38,   264,    17,    56,    57,    58,
    49,    53,    42,    87,    63,    39,    68,    43,    88,    29,
    61,    83,    14,     0,    81,    64,    51,    85,    28,    73,
    62,    16,   398,     0,    70,    82,     0,   163,   164,   324,
   325,   326,   332,   331,   330,   329,   328,     0,   333,   335,
   334,   337,   336,   338,   163,   164,   324,   325,   326,   332,
   331,   330,   329,   328,     0,   333,   335,   334,   337,   336,
   338,   163,   164,   324,   325,   326,   332,   331,     0,     0,
     0,     0,   333,   335,   334,   337,   336,   338,   159,   160,
   161,   162,     0,     0,     0,   157,   158,   103,     0,   159,
   160,   161,   162,     0,     0,     0,   157,   158,   103,     0,
   159,   160,   161,   162,     0,     0,     0,   157,   158,   103,
     0,   159,   160,   161,   162,     0,     0,     0,   157,   158,
   103,     0,   159,   160,   161,   162,     0,     0,     0,   157,
   158,   103,     0,   159,   160,   161,   162,     0,     0,     0,
   157,   158,   103,     0,     0,     0,     0,   230,     0,     0,
     0,     0,     0,   163,   164,   444,     0,     0,   230,     0,
     0,     0,     0,   235,   163,   164,   228,     0,     0,   230,
     0,     0,     0,     0,   235,   163,   164,   400,    93,     0,
   230,     0,     0,     0,     0,   235,   163,   164,     0,     0,
     0,   273,     0,     0,     0,     0,   235,   163,   164,     0,
     0,     0,   312,     0,     0,     0,     0,   235,   163,   164,
   121,     0,   124,   125,   126,     0,     0,     0,   235,     0,
     0,     0,     0,     0,   137,   138,     0,     0,   139,     0,
   141,   142,   143,     0,     0,   144,   145,   146,     0,   147,
   159,   160,   161,   162,     0,     0,     0,   157,   158,   103,
   253,     0,     0,     0,     0,     0,     0,     0,   173,   174,
   175,   176,   177,   159,   160,   161,   162,     0,     0,     0,
   157,   158,   103,     0,   159,   160,   161,   162,   459,   458,
   457,   157,   158,   103,     0,     0,     0,     0,     0,     0,
     0,     0,   159,   160,   161,   162,     0,     0,     0,   157,
   158,   103,     0,     0,     0,     0,     0,     0,     0,   248,
     0,     0,     0,     0,     0,   163,   164,   250,     0,   251,
     0,     0,     0,     0,     0,     0,    55,    45,     0,    84,
     0,    60,   152,    89,     0,     0,    46,     0,   163,   164,
   150,     0,   151,   456,     0,    86,     0,     0,     0,   163,
   164,     0,    56,    57,    58,    49,     0,     0,    87,     0,
     0,   248,     0,    88,     0,    61,    83,   163,   164,    81,
     0,    51,    85,     0,     0,    62,     0,   123,     0,     0,
    82,   159,   160,   161,   162,     0,     0,     0,   157,   158,
   103,   159,   160,   161,   162,     0,     0,     0,   157,   158,
   103,    72,     0,     0,     0,    69,     0,     0,    65,     0,
     0,    67,    71,     0,    66,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    63,     0,    68,     0,     0,     0,     0,     0,
     0,     0,     0,    64,     0,     0,     0,    73,     0,     0,
   248,     0,    70,     0,     0,     0,   163,   164,   280,     0,
   152,     0,     0,     0,     0,     0,   163,   164,   170 };
yytabelem yypact[]={

 -1000,    21,   432,   760,   344, -1000, -1000, -1000, -1000, -1000,
 -1000,   426, -1000, -1000, -1000, -1000, -1000, -1000,   233,   374,
   219,   217,   216,   214,   212,    91,   209,    17,    30, -1000,
 -1000, -1000,  1115, -1000, -1000, -1000,    36, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000,   374, -1000, -1000, -1000, -1000, -1000,
   308, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000,  1066,   307,  1194,   307,   206, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000,   374,   374,   374,   374, -1000,   374,
 -1000,   290, -1000, -1000,   374,     1,   374,   374,   374,   302,
   348, -1000, -1000,   374,   198, -1000, -1000, -1000, -1000,   387,
   306, -1000, -1000, -1000,   303, -1000, -1000, -1000, -1000,    30,
   374,   374,   302,   348, -1000,   238,   300,   421, -1000,   299,
   577,   914,   914,   297,   419,   374,   296,   374, -1000, -1000,
 -1000, -1000,  1043, -1000, -1000,    63,  1095, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000,  1043,   172,   193, -1000,
 -1000,   925,   925, -1000, -1000, -1000, -1000,  1184, -1000, -1000,
 -1000,   290,   290,   374, -1000, -1000,    85,   289,    91, -1000,
   289, -1000, -1000, -1000,   374, -1000,    19, -1000, -1000, -1000,
   348, -1000,   295,  1188,    -9,    30,   -38,   374,   419,   914,
   936, -1000,   374, -1000, -1000, -1000, -1000, -1000,   914, -1000,
   914,   313, -1000,   914, -1000,   167, -1000,   772,   419, -1000,
   914, -1000, -1000, -1000,   914,   914, -1000,   772, -1000,   914,
 -1000, -1000,    91,   419, -1000,   466,   144, -1000,  1095, -1000,
 -1000, -1000,   555, -1000,  1095,  1095,  1095,  1095,  1095,   -42,
    45,   129,   320, -1000, -1000,   320,   320, -1000,   190,   188,
   173,   772, -1000,   925, -1000, -1000, -1000, -1000, -1000,    63,
 -1000, -1000, -1000,   290,   289, -1000,   133, -1000, -1000, -1000,
    36, -1000,   374, -1000,   123, -1000, -1000,   348,   171,   170,
 -1000,   379, -1000,   374, -1000, -1000,   113, -1000,    87, -1000,
   755,   772,   903, -1000,   772,   284,    46,   165,   772,   374,
   658, -1000,   892,   914,   914,   914,   914,   914,   914,   914,
   914,   914,   914, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
   628,   151,   -46,   -32,   606,   287,   100, -1000, -1000, -1000,
  1043,    26,   772, -1000, -1000,   -12,   -42,   -42,   -42,   199,
 -1000,   320,   129,   134,   129,   925,   925,   925,   111,   109,
   101, -1000, -1000, -1000,    36, -1000,    89, -1000,    99,   881,
 -1000, -1000,   374,   -37,  1077, -1000,   374,   374,    75, -1000,
    31, -1000,    92, -1000, -1000,   374,   914,    91,   316, -1000,
   278,   286,   914,   914, -1000,   419, -1000,   -16,   -46,   -46,
   -46,   180,    -4,    -4,   789,   -32,    53, -1000,   914, -1000,
   419,   419,    91, -1000,    63, -1000, -1000,   320, -1000, -1000,
 -1000, -1000, -1000, -1000,   925,   925,   925, -1000, -1000, -1000,
   881, -1000, -1000,   520, -1000, -1000, -1000,  1077, -1000, -1000,
    22,   613, -1000, -1000, -1000, -1000,   914, -1000, -1000, -1000,
 -1000,   273,   259,   271,    55, -1000,   379,   379,   374, -1000,
   772,   374, -1000, -1000, -1000,   914,   270,   772,    78,   263,
    74, -1000,   914,   265,   263,   262,   261,   246, -1000,   881,
 -1000,  1077, -1000, -1000,   914, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000,   419, -1000, -1000,   772, -1000, -1000, -1000, -1000,
 -1000,   772, -1000, -1000,    43,   772,    41,   914,   419,   772,
 -1000 };
yytabelem yypgo[]={

     0,   588,   577,    14,   575,   574,   215,    12,    19,    36,
   573,   568,   567,     8,     0,   566,   561,   560,   558,    20,
   557,   554,    29,   547,   546,   540,    30,   538,    25,   537,
   536,    24,    31,    27,    17,    87,    15,   532,    56,   377,
     1,   229,    18,   423,   322,     9,     3,     6,    23,     2,
    49,   531,   529,    34,    21,    38,   528,   527,   526,   525,
   521,   988,    42,   518,   514,   513,   510,   509,   508,   507,
    57,   505,    45,    35,    39,     5,    37,   504,    32,   500,
   493,    11,    33,    10,   490,   487,     4,     7,    28,   486,
   485,   482,    13,   481,   393,   473,   470,   469,   463,   462,
   461,   354,   457,   456,   453,   451,   449,   447,    22,   446,
   441,    16 };
yytabelem yyr1[]={

     0,     1,     1,    57,    57,    57,    57,    57,    57,    57,
    57,     2,     5,    58,    58,    58,    58,    58,    58,    62,
    54,    35,    55,    55,    28,    28,    28,    29,    29,    36,
    36,    18,    59,    59,    59,    59,    59,    59,    59,    59,
    59,    59,    59,    12,    12,     9,    10,    11,    11,    11,
    11,    11,    11,    11,    11,    11,    11,    11,     8,     8,
     8,    52,    52,    52,    52,    53,    53,    63,    63,    64,
    64,    65,    65,    76,    56,    56,    66,    66,    77,    78,
    80,    79,    79,    81,    81,    47,    47,    47,    47,    69,
    69,    82,    82,    71,    71,    83,    38,    20,    20,    19,
    19,    21,    21,    22,    22,    73,    73,    85,    84,    84,
    86,    86,    45,    45,    87,    87,     3,    67,    67,    88,
    91,    89,    90,    90,    92,    92,    13,    68,    68,    93,
    23,    23,    70,    24,    24,    25,    25,    40,    40,    40,
    41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
    41,    41,    41,    41,    14,    14,    15,    15,    15,    15,
    15,    15,    39,    39,    39,    39,    34,    42,    42,    46,
    46,    50,    50,    50,    50,    50,    50,    49,    51,    51,
    51,    43,    43,    44,    44,    44,    44,    44,    44,    44,
    44,    60,    60,    60,    60,    60,    60,    60,    95,    26,
    94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
    94,     4,    96,    97,    97,    97,    97,    72,    72,    37,
    27,    27,    48,    48,    16,    16,    30,    30,    61,    74,
    75,    98,    99,    99,    99,    99,    99,    99,    99,    99,
    99,    99,    99,    99,    99,    99,   100,   107,   107,   107,
   102,   109,   109,   109,   104,   104,   101,   101,   110,   110,
   111,   111,   111,   111,   111,   111,    17,   103,   105,   106,
   106,    31,    31,     7,     7,    32,    32,    32,    33,    33,
    33,    33,    33,    33,     6,     6,     6,     6,   108 };
yytabelem yyr2[]={

     0,     0,     6,     5,     5,     4,     5,     7,     7,     5,
     3,     3,     3,     7,     7,     9,     9,    11,     7,     1,
     3,     3,     1,     2,     1,     5,     7,     3,     7,     3,
     3,     3,     2,     2,     2,     2,     2,     2,     2,     2,
     3,     5,    11,    13,    11,     5,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     1,     9,
    13,     7,     9,    11,     7,     3,     7,     7,     7,     7,
     7,     6,     6,     7,     3,     7,     6,     6,     1,     9,
     1,     4,     6,     3,     7,     2,     5,     2,     2,     2,
     6,     3,     3,     2,     6,     7,     5,     3,     7,     3,
     2,     3,     7,    11,    11,     1,     6,     1,     4,     6,
     3,     7,     3,     2,     3,     7,     3,     7,     6,     8,
     1,     5,     2,     6,     3,     7,     3,     2,     4,     9,
     3,     7,     1,     1,     2,     3,     7,     2,     7,     2,
     2,     2,     7,     7,     7,     7,     5,     7,     7,     7,
     7,     7,     5,     7,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     5,     9,    11,    11,     1,     2,     3,
     2,     3,     3,     3,     3,     3,     3,    11,     3,     3,
     3,     2,     7,     2,     2,     7,     7,     7,     7,     5,
     7,     2,    15,     5,     4,    13,     5,     5,    11,     7,
     9,    11,     4,     2,     3,    21,     3,     7,     9,     7,
     7,     3,     3,     7,     7,    15,    15,     0,     2,     7,
     3,     7,     2,     5,     3,     3,     3,     7,     1,     1,
     1,     3,     4,     5,     5,     5,     4,     5,     5,     7,
     9,     9,     5,     7,     3,     7,     6,     3,     3,     3,
     6,     3,     3,     3,     3,     3,     7,     7,     2,     6,
     3,     3,     3,     5,     5,     5,     3,     7,     7,     9,
     9,     3,     7,     3,    11,     3,     3,     2,     7,     7,
     7,     7,     7,     7,     3,    11,    11,    11,     1 };
yytabelem yychk[]={

 -1000,    -1,   -57,    -2,    -5,   256,     5,     3,   -58,   -59,
   -60,    20,    39,     6,    62,    25,    71,    46,    -9,    42,
   -12,   -52,   -63,   -64,   -65,   -66,   -67,   -68,    68,    59,
   -94,    35,   -95,    38,    37,    41,   -10,    29,    44,    55,
    43,    32,    52,    57,   -96,    22,    31,   -97,   -98,    50,
   -37,    66,   -16,    51,   -11,    21,    47,    48,    49,   -99,
    26,    60,    70,    54,    65,    30,    36,    33,    56,    27,
    74,    34,    23,    69,  -100,  -102,  -103,  -105,  -106,  -107,
  -109,    64,    75,    61,    24,    67,    40,    53,    58,    28,
    45,   -18,     7,   -61,   -62,   -62,   -62,   -62,    46,   -72,
    80,   -54,   -35,    16,    80,   -72,    80,    80,    80,    80,
   -72,    80,   -93,    85,   -69,   -82,   -35,   -53,    87,    85,
   -70,   -61,   -94,    72,   -61,   -61,   -61,    -8,    84,   -70,
   -70,   -70,   -70,   -77,   -70,   -39,   -35,   -61,   -61,   -61,
    76,   -61,   -61,   -61,   -61,   -61,   -61,   -61,  -101,   -44,
    84,    86,    76,   -39,   -50,   -43,   -14,    14,    15,     7,
     8,     9,    10,    82,    83,  -101,    76,  -101,  -104,   -44,
    84,  -101,    80,   -61,   -61,   -61,   -61,   -61,   -55,   -54,
   -55,   -54,   -54,   -62,   -35,   -28,    76,   -35,   -53,   -38,
   -35,   -35,   -35,   -76,    76,   -78,   -20,   -19,   -39,   -22,
    76,   -88,   -89,   -91,   -35,    80,    16,    76,   -74,    76,
   -74,   -38,   -53,   -35,   -35,   -76,   -78,   -88,    78,   -34,
    76,    -4,     8,    76,    77,   -27,   -48,   -40,    84,   -41,
    76,   -49,   -39,   -50,   -14,    92,   -42,   -40,   -42,    76,
    -3,     8,   -35,    76,   -35,   -43,  -110,   -44,    76,  -111,
    84,    86,   -17,    17,   -14,    84,    85,    86,    87,   -43,
   -43,   -31,    80,    -7,   -39,    76,    80,   -32,   -41,    -6,
   -33,   -40,   -49,    76,   -32,  -108,  -108,  -108,  -108,   -43,
    84,   -28,   -28,   -54,   -70,    77,   -29,   -36,   -35,    84,
   -73,    76,   -72,   -73,   -56,   -39,    85,    80,   -21,   -22,
   -39,    76,    -9,    85,   -82,    85,   -71,   -83,   -35,    -3,
   -40,   -40,    76,   -38,   -40,   -24,   -42,   -25,   -40,    73,
   -40,    77,    80,   -14,    84,    85,    86,   -15,    91,    90,
    89,    88,    87,    93,    95,    94,    97,    96,    98,    -3,
   -40,   -41,   -40,   -40,   -40,   -72,   -87,    -3,    77,    77,
    80,   -43,   -40,    84,    86,   -43,   -43,   -43,   -43,   -43,
    77,    80,   -31,   -31,   -31,    80,    80,    80,   -41,    -6,
   -33,  -108,  -108,   -28,   -73,    77,    80,    -8,   -84,   -85,
   -38,    77,    80,   -79,   -80,   -19,    80,    80,   -90,   -92,
   -13,    16,   -23,   -35,    77,    80,    78,   -75,    77,   -75,
    84,    77,    79,    80,   -35,    77,   -48,   -40,   -40,   -40,
   -40,   -40,   -40,   -40,   -40,   -40,   -40,    77,    80,    77,
    76,    80,    77,  -111,   -43,    77,    -7,    80,   -41,    -6,
   -41,    -6,   -41,    -6,    80,    80,    80,    -8,   -36,    77,
    80,   -86,   -45,   -40,    84,   -39,    85,    80,   -81,   -47,
   -46,   -14,   -49,   -51,   -35,   -50,    76,    13,    12,    11,
   -39,   -26,   -35,   -26,   -35,    77,    80,    83,    80,   -83,
   -40,   -72,    72,    77,   -34,    76,   -42,   -40,    -3,   -41,
   -87,    -3,   -72,   -26,   -41,   -26,   -26,   -26,   -86,    79,
   -81,    84,   -46,    77,    78,    77,   -92,   -13,   -35,   -26,
   -75,    77,    80,    77,    77,   -40,    77,    77,    77,    77,
   -45,   -40,   -47,   -46,   -30,   -40,    -3,    80,    80,   -40,
    -3 };
yytabelem yydef[]={

     1,    -2,     0,     0,     0,    10,    -2,     2,     3,     5,
     6,     0,   228,     9,    19,    19,    19,    19,   217,     0,
    32,    -2,    34,    35,    36,    -2,    38,    39,    40,   132,
   191,   228,     0,   228,   228,   228,    58,   132,   132,   132,
   132,    78,   132,   127,     0,   228,   228,   203,   204,   228,
   206,   228,   228,   228,    46,   212,   228,   228,   228,   231,
   228,   224,   225,    47,    48,    49,    50,    51,    52,    53,
    54,    55,    56,    57,     0,     0,     0,     0,   244,   228,
   228,   228,   228,   228,   247,   248,   249,   251,   252,   253,
     4,     7,    31,     8,    22,    22,     0,     0,    19,     0,
   218,    24,    20,    21,     0,     0,   218,     0,     0,     0,
     0,   120,   128,     0,    41,    89,    91,    92,    65,     0,
     0,   229,   193,   194,     0,   196,   197,    45,   229,     0,
     0,     0,     0,     0,   120,     0,   162,     0,   202,     0,
     0,   167,   167,     0,     0,     0,     0,     0,   232,    -2,
   234,   235,     0,   183,   184,     0,     0,   171,   172,   173,
   174,   175,   176,   154,   155,   236,     0,   237,   238,    -2,
   255,   242,     0,   288,   288,   288,   288,     0,    13,    23,
    14,    24,    24,     0,   132,    18,     0,   105,   217,    64,
   105,    68,    70,    72,     0,    77,     0,    97,    99,   100,
     0,   118,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    61,     0,    67,    69,    71,    76,   117,     0,   163,
    -2,     0,   211,     0,   207,     0,   220,   222,     0,   137,
     0,   139,   140,   141,     0,     0,   209,   168,   210,     0,
   213,   116,    -2,     0,   219,   260,     0,   181,     0,   258,
   261,   262,     0,   266,     0,     0,     0,     0,     0,   189,
   260,   239,     0,   271,   273,     0,     0,   243,    -2,   276,
   277,     0,    -2,     0,   245,   246,   250,   267,   268,   288,
   288,    15,    16,    24,   105,    25,     0,    27,    29,    30,
    58,   107,     0,    96,     0,    74,    80,     0,     0,     0,
   101,     0,   121,     0,    90,    66,     0,    93,     0,   230,
     0,   230,     0,    62,   200,     0,     0,   134,    -2,     0,
     0,   208,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   156,   157,   158,   159,   160,   161,   223,
     0,   137,   146,   152,     0,     0,     0,   114,    -2,   257,
     0,     0,   263,   264,   265,   185,   186,   187,   188,   190,
   256,     0,   241,     0,   240,     0,     0,     0,   137,     0,
     0,   269,   270,    17,    58,    26,     0,    44,     0,     0,
    63,    73,     0,     0,     0,    98,     0,     0,     0,   122,
   124,   126,   129,   130,    42,     0,     0,   217,     0,    59,
     0,   164,   167,     0,   201,     0,   221,   142,   143,   144,
   145,    -2,   148,   149,   150,   151,   153,   138,     0,   198,
     0,     0,   217,   259,   260,   182,   272,     0,    -2,   279,
    -2,   281,    -2,   283,     0,     0,     0,    43,    28,   106,
     0,   108,   110,   113,   112,    75,    79,     0,    81,    83,
    85,     0,    87,    88,   169,   170,     0,   178,   179,   180,
   102,     0,   162,     0,     0,   119,     0,     0,     0,    94,
    95,     0,   195,   230,   165,   167,     0,   136,     0,   137,
     0,   115,     0,     0,    -2,     0,     0,     0,   109,     0,
    82,     0,    86,   103,     0,   104,   123,   125,   131,   192,
    60,   166,     0,   177,   215,   216,   274,   285,   286,   287,
   111,   113,    84,    85,   199,   226,     0,     0,     0,   227,
   205 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"SEOS",	3,
	"SCOMMENT",	4,
	"SLABEL",	5,
	"SUNKNOWN",	6,
	"SHOLLERITH",	7,
	"SICON",	8,
	"SRCON",	9,
	"SDCON",	10,
	"SBITCON",	11,
	"SOCTCON",	12,
	"SHEXCON",	13,
	"STRUE",	14,
	"SFALSE",	15,
	"SNAME",	16,
	"SNAMEEQ",	17,
	"SFIELD",	18,
	"SSCALE",	19,
	"SINCLUDE",	20,
	"SLET",	21,
	"SASSIGN",	22,
	"SAUTOMATIC",	23,
	"SBACKSPACE",	24,
	"SBLOCK",	25,
	"SCALL",	26,
	"SCHARACTER",	27,
	"SCLOSE",	28,
	"SCOMMON",	29,
	"SCOMPLEX",	30,
	"SCONTINUE",	31,
	"SDATA",	32,
	"SDCOMPLEX",	33,
	"SDIMENSION",	34,
	"SDO",	35,
	"SDOUBLE",	36,
	"SELSE",	37,
	"SELSEIF",	38,
	"SEND",	39,
	"SENDFILE",	40,
	"SENDIF",	41,
	"SENTRY",	42,
	"SEQUIV",	43,
	"SEXTERNAL",	44,
	"SFORMAT",	45,
	"SFUNCTION",	46,
	"SGOTO",	47,
	"SASGOTO",	48,
	"SCOMPGOTO",	49,
	"SARITHIF",	50,
	"SLOGIF",	51,
	"SIMPLICIT",	52,
	"SINQUIRE",	53,
	"SINTEGER",	54,
	"SINTRINSIC",	55,
	"SLOGICAL",	56,
	"SNAMELIST",	57,
	"SOPEN",	58,
	"SPARAM",	59,
	"SPAUSE",	60,
	"SPRINT",	61,
	"SPROGRAM",	62,
	"SPUNCH",	63,
	"SREAD",	64,
	"SREAL",	65,
	"SRETURN",	66,
	"SREWIND",	67,
	"SSAVE",	68,
	"SSTATIC",	69,
	"SSTOP",	70,
	"SSUBROUTINE",	71,
	"STHEN",	72,
	"STO",	73,
	"SUNDEFINED",	74,
	"SWRITE",	75,
	"SLPAR",	76,
	"SRPAR",	77,
	"SEQUALS",	78,
	"SCOLON",	79,
	"SCOMMA",	80,
	"SCURRENCY",	81,
	"SPLUS",	82,
	"SMINUS",	83,
	"SSTAR",	84,
	"SSLASH",	85,
	"SPOWER",	86,
	"SCONCAT",	87,
	"SAND",	88,
	"SOR",	89,
	"SNEQV",	90,
	"SEQV",	91,
	"SNOT",	92,
	"SEQ",	93,
	"SLT",	94,
	"SGT",	95,
	"SLE",	96,
	"SGE",	97,
	"SNE",	98,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"program : /* empty */",
	"program : program stat SEOS",
	"stat : thislabel entry",
	"stat : fmtlabel SFORMAT",
	"stat : thislabel spec",
	"stat : thislabel exec",
	"stat : thislabel SINCLUDE filename",
	"stat : thislabel SEND end_spec",
	"stat : thislabel SUNKNOWN",
	"stat : error",
	"thislabel : SLABEL",
	"fmtlabel : SLABEL",
	"entry : SPROGRAM new_proc progname",
	"entry : SBLOCK new_proc progname",
	"entry : SSUBROUTINE new_proc entryname arglist",
	"entry : SFUNCTION new_proc entryname arglist",
	"entry : type SFUNCTION new_proc entryname arglist",
	"entry : SENTRY entryname arglist",
	"new_proc : /* empty */",
	"entryname : name",
	"name : SNAME",
	"progname : /* empty */",
	"progname : entryname",
	"arglist : /* empty */",
	"arglist : SLPAR SRPAR",
	"arglist : SLPAR args SRPAR",
	"args : arg",
	"args : args SCOMMA arg",
	"arg : name",
	"arg : SSTAR",
	"filename : SHOLLERITH",
	"spec : dcl",
	"spec : common",
	"spec : external",
	"spec : intrinsic",
	"spec : equivalence",
	"spec : data",
	"spec : implicit",
	"spec : namelist",
	"spec : SSAVE",
	"spec : SSAVE savelist",
	"spec : SPARAM in_dcl SLPAR paramlist SRPAR",
	"dcl : type opt_comma name in_dcl dims lengspec",
	"dcl : dcl SCOMMA name dims lengspec",
	"type : typespec lengspec",
	"typespec : typename",
	"typename : SINTEGER",
	"typename : SREAL",
	"typename : SCOMPLEX",
	"typename : SDOUBLE",
	"typename : SDCOMPLEX",
	"typename : SLOGICAL",
	"typename : SCHARACTER",
	"typename : SUNDEFINED",
	"typename : SDIMENSION",
	"typename : SAUTOMATIC",
	"typename : SSTATIC",
	"lengspec : /* empty */",
	"lengspec : SSTAR intonlyon expr intonlyoff",
	"lengspec : SSTAR intonlyon SLPAR SSTAR SRPAR intonlyoff",
	"common : SCOMMON in_dcl var",
	"common : SCOMMON in_dcl comblock var",
	"common : common opt_comma comblock opt_comma var",
	"common : common SCOMMA var",
	"comblock : SCONCAT",
	"comblock : SSLASH SNAME SSLASH",
	"external : SEXTERNAL in_dcl name",
	"external : external SCOMMA name",
	"intrinsic : SINTRINSIC in_dcl name",
	"intrinsic : intrinsic SCOMMA name",
	"equivalence : SEQUIV in_dcl equivset",
	"equivalence : equivalence SCOMMA equivset",
	"equivset : SLPAR equivlist SRPAR",
	"equivlist : lhs",
	"equivlist : equivlist SCOMMA lhs",
	"data : SDATA in_data datalist",
	"data : data opt_comma datalist",
	"in_data : /* empty */",
	"datalist : datavarlist SSLASH vallist SSLASH",
	"vallist : /* empty */",
	"vallist : val",
	"vallist : vallist SCOMMA val",
	"val : value",
	"val : simple SSTAR value",
	"value : simple",
	"value : addop simple",
	"value : complex_const",
	"value : bit_const",
	"savelist : saveitem",
	"savelist : savelist SCOMMA saveitem",
	"saveitem : name",
	"saveitem : comblock",
	"paramlist : paramitem",
	"paramlist : paramlist SCOMMA paramitem",
	"paramitem : name SEQUALS expr",
	"var : name dims",
	"datavarlist : datavar",
	"datavarlist : datavarlist SCOMMA datavar",
	"datavar : lhs",
	"datavar : dataimpldo",
	"dataitems : lhs",
	"dataitems : dataitems SCOMMA lhs",
	"dataimpldo : SLPAR dataitems SCOMMA dospec SRPAR",
	"dataimpldo : SLPAR dataimpldo SCOMMA dospec SRPAR",
	"dims : /* empty */",
	"dims : SLPAR dimlist SRPAR",
	"dimlist : /* empty */",
	"dimlist : dim",
	"dimlist : dimlist SCOMMA dim",
	"dim : ubound",
	"dim : expr SCOLON ubound",
	"ubound : SSTAR",
	"ubound : expr",
	"labellist : label",
	"labellist : labellist SCOMMA label",
	"label : SICON",
	"implicit : SIMPLICIT in_dcl implist",
	"implicit : implicit SCOMMA implist",
	"implist : imptype SLPAR letgroups SRPAR",
	"imptype : /* empty */",
	"imptype : type",
	"letgroups : letgroup",
	"letgroups : letgroups SCOMMA letgroup",
	"letgroup : letter",
	"letgroup : letter SMINUS letter",
	"letter : SNAME",
	"namelist : SNAMELIST",
	"namelist : namelist namelistentry",
	"namelistentry : SSLASH name SSLASH namelistlist",
	"namelistlist : name",
	"namelistlist : namelistlist SCOMMA name",
	"in_dcl : /* empty */",
	"funarglist : /* empty */",
	"funarglist : funargs",
	"funargs : expr",
	"funargs : funargs SCOMMA expr",
	"expr : uexpr",
	"expr : SLPAR expr SRPAR",
	"expr : complex_const",
	"uexpr : lhs",
	"uexpr : simple_const",
	"uexpr : expr addop expr",
	"uexpr : expr SSTAR expr",
	"uexpr : expr SSLASH expr",
	"uexpr : expr SPOWER expr",
	"uexpr : addop expr",
	"uexpr : expr relop expr",
	"uexpr : expr SEQV expr",
	"uexpr : expr SNEQV expr",
	"uexpr : expr SOR expr",
	"uexpr : expr SAND expr",
	"uexpr : SNOT expr",
	"uexpr : expr SCONCAT expr",
	"addop : SPLUS",
	"addop : SMINUS",
	"relop : SEQ",
	"relop : SGT",
	"relop : SLT",
	"relop : SGE",
	"relop : SLE",
	"relop : SNE",
	"lhs : name",
	"lhs : name substring",
	"lhs : name SLPAR funarglist SRPAR",
	"lhs : name SLPAR funarglist SRPAR substring",
	"substring : SLPAR opt_expr SCOLON opt_expr SRPAR",
	"opt_expr : /* empty */",
	"opt_expr : expr",
	"simple : name",
	"simple : simple_const",
	"simple_const : STRUE",
	"simple_const : SFALSE",
	"simple_const : SHOLLERITH",
	"simple_const : SICON",
	"simple_const : SRCON",
	"simple_const : SDCON",
	"complex_const : SLPAR uexpr SCOMMA uexpr SRPAR",
	"bit_const : SHEXCON",
	"bit_const : SOCTCON",
	"bit_const : SBITCON",
	"fexpr : unpar_fexpr",
	"fexpr : SLPAR fexpr SRPAR",
	"unpar_fexpr : lhs",
	"unpar_fexpr : simple_const",
	"unpar_fexpr : fexpr addop fexpr",
	"unpar_fexpr : fexpr SSTAR fexpr",
	"unpar_fexpr : fexpr SSLASH fexpr",
	"unpar_fexpr : fexpr SPOWER fexpr",
	"unpar_fexpr : addop fexpr",
	"unpar_fexpr : fexpr SCONCAT fexpr",
	"exec : iffable",
	"exec : SDO end_spec intonlyon label intonlyoff opt_comma dospec",
	"exec : logif iffable",
	"exec : logif STHEN",
	"exec : SELSEIF end_spec SLPAR expr SRPAR STHEN",
	"exec : SELSE end_spec",
	"exec : SENDIF end_spec",
	"logif : SLOGIF end_spec SLPAR expr SRPAR",
	"dospec : name SEQUALS exprlist",
	"iffable : let lhs SEQUALS expr",
	"iffable : SASSIGN end_spec assignlabel STO name",
	"iffable : SCONTINUE end_spec",
	"iffable : goto",
	"iffable : io",
	"iffable : SARITHIF end_spec SLPAR expr SRPAR label SCOMMA label SCOMMA label",
	"iffable : call",
	"iffable : call SLPAR SRPAR",
	"iffable : call SLPAR callarglist SRPAR",
	"iffable : SRETURN end_spec opt_expr",
	"iffable : stop end_spec opt_expr",
	"assignlabel : SICON",
	"let : SLET",
	"goto : SGOTO end_spec label",
	"goto : SASGOTO end_spec name",
	"goto : SASGOTO end_spec name opt_comma SLPAR labellist SRPAR",
	"goto : SCOMPGOTO end_spec SLPAR labellist SRPAR opt_comma expr",
	"opt_comma : /* empty */",
	"opt_comma : SCOMMA",
	"call : SCALL end_spec name",
	"callarglist : callarg",
	"callarglist : callarglist SCOMMA callarg",
	"callarg : expr",
	"callarg : SSTAR label",
	"stop : SPAUSE",
	"stop : SSTOP",
	"exprlist : expr",
	"exprlist : exprlist SCOMMA expr",
	"end_spec : /* empty */",
	"intonlyon : /* empty */",
	"intonlyoff : /* empty */",
	"io : io1",
	"io1 : iofmove ioctl",
	"io1 : iofmove unpar_fexpr",
	"io1 : iofmove SSTAR",
	"io1 : iofmove SPOWER",
	"io1 : iofctl ioctl",
	"io1 : read ioctl",
	"io1 : read infmt",
	"io1 : read ioctl inlist",
	"io1 : read infmt SCOMMA inlist",
	"io1 : read ioctl SCOMMA inlist",
	"io1 : write ioctl",
	"io1 : write ioctl outlist",
	"io1 : print",
	"io1 : print SCOMMA outlist",
	"iofmove : fmkwd end_spec in_ioctl",
	"fmkwd : SBACKSPACE",
	"fmkwd : SREWIND",
	"fmkwd : SENDFILE",
	"iofctl : ctlkwd end_spec in_ioctl",
	"ctlkwd : SINQUIRE",
	"ctlkwd : SOPEN",
	"ctlkwd : SCLOSE",
	"infmt : unpar_fexpr",
	"infmt : SSTAR",
	"ioctl : SLPAR fexpr SRPAR",
	"ioctl : SLPAR ctllist SRPAR",
	"ctllist : ioclause",
	"ctllist : ctllist SCOMMA ioclause",
	"ioclause : fexpr",
	"ioclause : SSTAR",
	"ioclause : SPOWER",
	"ioclause : nameeq expr",
	"ioclause : nameeq SSTAR",
	"ioclause : nameeq SPOWER",
	"nameeq : SNAMEEQ",
	"read : SREAD end_spec in_ioctl",
	"write : SWRITE end_spec in_ioctl",
	"print : SPRINT end_spec fexpr in_ioctl",
	"print : SPRINT end_spec SSTAR in_ioctl",
	"inlist : inelt",
	"inlist : inlist SCOMMA inelt",
	"inelt : lhs",
	"inelt : SLPAR inlist SCOMMA dospec SRPAR",
	"outlist : uexpr",
	"outlist : other",
	"outlist : out2",
	"out2 : uexpr SCOMMA uexpr",
	"out2 : uexpr SCOMMA other",
	"out2 : other SCOMMA uexpr",
	"out2 : other SCOMMA other",
	"out2 : out2 SCOMMA uexpr",
	"out2 : out2 SCOMMA other",
	"other : complex_const",
	"other : SLPAR uexpr SCOMMA dospec SRPAR",
	"other : SLPAR other SCOMMA dospec SRPAR",
	"other : SLPAR out2 SCOMMA dospec SRPAR",
	"in_ioctl : /* empty */",
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
		
case 3:
# line 183 "gram.in"
{ lastwasbranch = NO; } break;
case 4:
# line 187 "gram.in"
{ fmtstmt(fmtlabel); setfmt(fmtlabel);} break;
case 6:
# line 190 "gram.in"
{ if(yypvt[-1].labval && (yypvt[-1].labval->labelno==dorange))
			enddo(yypvt[-1].labval->labelno);
		  if(lastwasbranch && thislabel==NULL)
			warn("statement cannot be reached");
		  lastwasbranch = thiswasbranch;
		  thiswasbranch = NO;
		  if(yypvt[-1].labval)
			{
			if(yypvt[-1].labval->labtype == LABFORMAT)
				err("label already that of a format");
			else
				yypvt[-1].labval->labtype = LABEXEC;
			}
		} break;
case 7:
# line 205 "gram.in"
{ doinclude( yypvt[-0].charpval ); } break;
case 8:
# line 207 "gram.in"
{ lastwasbranch = NO;  endproc(); } break;
case 9:
# line 209 "gram.in"
{ execerr("unclassifiable statement", CNULL);  flline(); } break;
case 10:
# line 211 "gram.in"
{ flline();  needkwd = NO;  inioctl = NO; 
		  yyerrok; yyclearin; } break;
case 11:
# line 216 "gram.in"
{
#ifdef SDB
		char buff[10];
		if( sdbflag )
			{
#	ifdef UCBVAXASM
			p2pass( stabdline(N_SLINE, f_lineno) );
#	else
#		if TARGET == PDP11
			sprintf(buff,"LL%d", ++dbglabel);
			p2pass( stabline(0, N_SLINE, f_lineno, buff) );
			p2pi("LL%d:\n", dbglabel);
#		else
#		if TARGET == M68K
			p2pi("\tln\t%d", f_lineno - fnstline);
#		else
			p2pi("\t.ln\t%d", f_lineno - fnstline);
#		endif
#		endif /* sw19 */
#	endif
			}
#endif

		if(yystno != 0)
			{
			yyval.labval = thislabel =  mklabel(yystno);

			if(parstate == OUTSIDE)
						/* if we get here, the */
						/* first line of the file */
						/* has a label, therefore */
						/* it is a MAIN	 */
				{
				newproc();
				startproc((struct Extsym *) NULL, procclass);
				}
 
			if(thislabel->labdefined)
				execerr("label %s already defined",
					convic(thislabel->stateno) );
			else	{
				if(thislabel->blklevel!=0 && thislabel->blklevel<blklevel
				    && thislabel->labtype!=LABFORMAT)
					warn1("there is a branch to label %s from outside block",
					      convic( (ftnint) (thislabel->stateno) ) );
				thislabel->blklevel = blklevel;
				thislabel->labdefined = YES;
				if(thislabel->labtype != LABFORMAT)
					putlabel(thislabel->labelno);
				}
			}
		else    yyval.labval = thislabel = NULL;
		} break;
case 12:
# line 274 "gram.in"
{
#ifdef SDB
		char buff[10];
		if (sdbflag) {
			p2pi("\tln\t%d",f_lineno - fnstline);
			}
#endif
		if (yystno != 0) {
			yyval.labval = fmtlabel = mklabel(yystno);
			if (fmtlabel->labdefined)
				execerr("label %s already defined",
					convic(fmtlabel->stateno));
			else {
				fmtlabel->labdefined = YES;
				fmtlabel->labtype = LABFORMAT;
				fmtlabel->blklevel = blklevel;
				}
			}
		} break;
case 13:
# line 296 "gram.in"
{startproc(yypvt[-0].extval, CLMAIN); } break;
case 14:
# line 298 "gram.in"
{ if(yypvt[-0].extval) NO66("named BLOCKDATA");
		  startproc(yypvt[-0].extval, CLBLOCK); } break;
case 15:
# line 301 "gram.in"
{ entrypt(CLPROC, TYSUBR, (ftnint) 0,  yypvt[-1].extval, yypvt[-0].chval); } break;
case 16:
# line 303 "gram.in"
{ entrypt(CLPROC, TYUNKNOWN, (ftnint) 0, yypvt[-1].extval, yypvt[-0].chval); } break;
case 17:
# line 305 "gram.in"
{ entrypt(CLPROC, yypvt[-4].ival, varleng, yypvt[-1].extval, yypvt[-0].chval); } break;
case 18:
# line 307 "gram.in"
{ if(parstate==OUTSIDE || procclass==CLMAIN
			|| procclass==CLBLOCK)
				execerr("misplaced entry statement", CNULL);
		  entrypt(CLENTRY, 0, (ftnint) 0, yypvt[-1].extval, yypvt[-0].chval);
		} break;
case 19:
# line 315 "gram.in"
{ newproc(); } break;
case 20:
# line 319 "gram.in"
{ yyval.extval = newentry(yypvt[-0].namval); } break;
case 21:
# line 323 "gram.in"
{ yyval.namval = mkname(toklen, token); } break;
case 22:
# line 326 "gram.in"
{ yyval.extval = NULL; } break;
case 24:
# line 331 "gram.in"
{ yyval.chval = 0; } break;
case 25:
# line 333 "gram.in"
{ NO66(" () argument list");
		  yyval.chval = 0; } break;
case 26:
# line 336 "gram.in"
{yyval.chval = yypvt[-1].chval; } break;
case 27:
# line 340 "gram.in"
{ yyval.chval = (yypvt[-0].namval ? mkchain((tagptr) yypvt[-0].namval,CHNULL) : CHNULL ); } break;
case 28:
# line 342 "gram.in"
{ if(yypvt[-0].namval) yypvt[-2].chval = yyval.chval = hookup(yypvt[-2].chval, mkchain((tagptr) yypvt[-0].namval,CHNULL)); } break;
case 29:
# line 346 "gram.in"
{ if(yypvt[-0].namval->vstg!=STGUNKNOWN && yypvt[-0].namval->vstg!=STGARG)
			dclerr("name declared as argument after use", yypvt[-0].namval);
		  yypvt[-0].namval->vstg = STGARG;
		} break;
case 30:
# line 351 "gram.in"
{ NO66("altenate return argument");
		  yyval.namval = 0;  substars = YES; } break;
case 31:
# line 358 "gram.in"
{
		char *s;
		s = copyn(toklen+1, token);
		s[toklen] = '\0';
		yyval.charpval = s;
		} break;
case 40:
# line 375 "gram.in"
{ NO66("SAVE statement");
		  saveall = YES; } break;
case 41:
# line 378 "gram.in"
{ NO66("SAVE statement"); } break;
case 42:
# line 380 "gram.in"
{ NO66("PARAMETER statement"); } break;
case 43:
# line 384 "gram.in"
{ settype(yypvt[-3].namval, yypvt[-5].ival, yypvt[-0].ival);
		  if(ndim>0) setbound(yypvt[-3].namval,ndim,dims);
		} break;
case 44:
# line 388 "gram.in"
{ settype(yypvt[-2].namval, yypvt[-4].ival, yypvt[-0].ival);
		  if(ndim>0) setbound(yypvt[-2].namval,ndim,dims);
		} break;
case 45:
# line 394 "gram.in"
{ varleng = yypvt[-0].ival; } break;
case 46:
# line 398 "gram.in"
{ varleng = (yypvt[-0].ival<0 || yypvt[-0].ival==TYLONG ? 0 : typesize[yypvt[-0].ival]); } break;
case 47:
# line 401 "gram.in"
{ yyval.ival = TYLONG; } break;
case 48:
# line 402 "gram.in"
{ yyval.ival = TYREAL; } break;
case 49:
# line 403 "gram.in"
{ yyval.ival = TYCOMPLEX; } break;
case 50:
# line 404 "gram.in"
{ yyval.ival = TYDREAL; } break;
case 51:
# line 405 "gram.in"
{ NOEXT("DOUBLE COMPLEX statement"); yyval.ival = TYDCOMPLEX; } break;
case 52:
# line 406 "gram.in"
{ yyval.ival = TYLOGICAL; } break;
case 53:
# line 407 "gram.in"
{ NO66("CHARACTER statement"); yyval.ival = TYCHAR; } break;
case 54:
# line 408 "gram.in"
{ yyval.ival = TYUNKNOWN; } break;
case 55:
# line 409 "gram.in"
{ yyval.ival = TYUNKNOWN; } break;
case 56:
# line 410 "gram.in"
{ NOEXT("AUTOMATIC statement"); yyval.ival = - STGAUTO; } break;
case 57:
# line 411 "gram.in"
{ NOEXT("STATIC statement"); yyval.ival = - STGBSS; } break;
case 58:
# line 415 "gram.in"
{ yyval.ival = varleng; } break;
case 59:
# line 417 "gram.in"
{
		  expptr p;
		  p = yypvt[-1].expval;
		  NO66("length specification *n");
		  if( ! ISICON(p) || p->constblock.cons.ci<0 )
			{
			yyval.ival = 0;
			dclerr("length must be a positive integer constant",
				(Namep) NULL);
			}
		  else yyval.ival = p->constblock.cons.ci;
		} break;
case 60:
# line 430 "gram.in"
{ NO66("length specification *(*)"); yyval.ival = -1; } break;
case 61:
# line 434 "gram.in"
{ incomm( yyval.extval = comblock(0, CNULL) , yypvt[-0].namval ); } break;
case 62:
# line 436 "gram.in"
{ yyval.extval = yypvt[-1].extval;  incomm(yypvt[-1].extval, yypvt[-0].namval); } break;
case 63:
# line 438 "gram.in"
{ yyval.extval = yypvt[-2].extval;  incomm(yypvt[-2].extval, yypvt[-0].namval); } break;
case 64:
# line 440 "gram.in"
{ incomm(yypvt[-2].extval, yypvt[-0].namval); } break;
case 65:
# line 444 "gram.in"
{ yyval.extval = comblock(0, CNULL); } break;
case 66:
# line 446 "gram.in"
{ yyval.extval = comblock(toklen, token); } break;
case 67:
# line 450 "gram.in"
{ setext(yypvt[-0].namval); } break;
case 68:
# line 452 "gram.in"
{ setext(yypvt[-0].namval); } break;
case 69:
# line 456 "gram.in"
{ NO66("INTRINSIC statement"); setintr(yypvt[-0].namval); } break;
case 70:
# line 458 "gram.in"
{ setintr(yypvt[-0].namval); } break;
case 73:
# line 466 "gram.in"
{
		struct Equivblock *p;
		if(nequiv >= maxequiv)
			many("equivalences", 'q');
		p  =  & eqvclass[nequiv++];
		p->eqvinit = NO;
		p->eqvbottom = 0;
		p->eqvtop = 0;
		p->equivs = yypvt[-1].eqvval;
		} break;
case 74:
# line 479 "gram.in"
{ yyval.eqvval=ALLOC(Eqvchain);
		  yyval.eqvval->eqvitem.eqvlhs = (struct Primblock *)yypvt[-0].expval;
		} break;
case 75:
# line 483 "gram.in"
{ yyval.eqvval=ALLOC(Eqvchain);
		  yyval.eqvval->eqvitem.eqvlhs = (struct Primblock *) yypvt[-0].expval;
		  yyval.eqvval->eqvnextp = yypvt[-2].eqvval;
		} break;
case 78:
# line 494 "gram.in"
{ if(parstate == OUTSIDE)
			{
			newproc();
			startproc((struct Extsym *) NULL, CLMAIN);
			}
		  if(parstate < INDATA)
			{
			enddcl();
			parstate = INDATA;
			}
		} break;
case 79:
# line 508 "gram.in"
{ ftnint junk;
		  enum Boolean bunk;
		  if(nextdata(&junk,&junk,&bunk,&junk,1) != NULL)
			{
			err("too few initializers");
			curdtp = NULL;
			}
		  frdata(yypvt[-3].chval);
		  frrpl();
		} break;
case 80:
# line 520 "gram.in"
{ toomanyinit = NO; } break;
case 83:
# line 525 "gram.in"
{ dataval((expptr) NULL, yypvt[-0].expval); } break;
case 84:
# line 527 "gram.in"
{ dataval(yypvt[-2].expval, yypvt[-0].expval); } break;
case 86:
# line 532 "gram.in"
{ if( yypvt[-1].ival==OPMINUS && ISCONST(yypvt[-0].expval) )
			consnegop((Constp) yypvt[-0].expval);
		  yyval.expval = yypvt[-0].expval;
		} break;
case 91:
# line 545 "gram.in"
{ int k;
		  yypvt[-0].namval->vsave = YES;
		  k = yypvt[-0].namval->vstg;
		if( ! ONEOF(k, M(STGUNKNOWN)|M(STGBSS)|M(STGINIT)) )
			dclerr("can only save static variables", yypvt[-0].namval);
		} break;
case 92:
# line 552 "gram.in"
{ yypvt[-0].extval->extsave = 1; } break;
case 95:
# line 560 "gram.in"
{ if(yypvt[-2].namval->vclass == CLUNKNOWN)
			{ yypvt[-2].namval->vclass = CLPARAM;
			  ( (struct Paramblock *) (yypvt[-2].namval) )->paramval = yypvt[-0].expval;
			}
		  else dclerr("cannot make %s parameter", yypvt[-2].namval);
		} break;
case 96:
# line 569 "gram.in"
{ if(ndim>0) setbound(yypvt[-1].namval, ndim, dims); } break;
case 97:
# line 574 "gram.in"
{ curdtp = yypvt[-0].chval; curdtelt = 0; } break;
case 98:
# line 576 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, yypvt[-0].chval); } break;
case 99:
# line 581 "gram.in"
{
		Namep np;
		np = ( (struct Primblock *) yypvt[-0].expval) -> namep;
		vardcl(np);
		if (np->vstg == STGCOMMON)
			extsymtab[np->vardesc.varno].extinit = YES;
		else if (np->vstg==STGEQUIV)
			eqvclass[np->vardesc.varno].eqvinit = YES;
		else if(np->vstg!=STGINIT && np->vstg!=STGBSS)
			dclerr("inconsistent storage classes", np);
		yyval.chval = mkchain((tagptr) yypvt[-0].expval, CHNULL);
		} break;
case 101:
# line 597 "gram.in"
{
		Namep np;
		np = ( (struct Primblock *) yypvt[-0].expval) -> namep;
		vardcl(np);
		if (np->vstg == STGCOMMON)
			extsymtab[np->vardesc.varno].extinit = YES;
		else if (np->vstg==STGEQUIV)
			eqvclass[np->vardesc.varno].eqvinit = YES;
		else if(np->vstg!=STGINIT && np->vstg!=STGBSS)
			dclerr("inconsistent storage classes", np);
		yyval.chval = mkchain((tagptr) yypvt[-0].expval, CHNULL);
		} break;
case 102:
# line 610 "gram.in"
{
		Namep np;
		chainp dtitem;
		np = ( (struct Primblock *) yypvt[-0].expval) -> namep;
		vardcl(np);
		if (np->vstg == STGCOMMON)
			extsymtab[np->vardesc.varno].extinit = YES;
		else if (np->vstg==STGEQUIV)
			eqvclass[np->vardesc.varno].eqvinit = YES;
		else if(np->vstg!=STGINIT && np->vstg!=STGBSS)
			dclerr("inconsistent storage classes", np);

	/* Traverse the list of array references to be initialized in the
	   data implied do loop so that a new one (for "lhs") may be added : */

		for (dtitem = (chainp) yypvt[-2].chval; dtitem->nextp != CHNULL; dtitem = dtitem->nextp);
		dtitem->nextp = mkchain((tagptr) yypvt[-0].expval, CHNULL);
		yyval.chval = yypvt[-2].chval;
		} break;
case 103:
# line 637 "gram.in"
{
		chainp p;
		struct Impldoblock *q;
		q = ALLOC(Impldoblock);
		q->tag = TIMPLDO;
		q->varnp = (Namep) (yypvt[-1].chval->datap);
		p = yypvt[-1].chval->nextp;
		if (p)  { q->implb = (expptr)(p->datap); p = p->nextp; }
		if (p)  { q->impub = (expptr)(p->datap); p = p->nextp; }
		if (p)  { q->impstep = (expptr)(p->datap); p = p->nextp; }
		frchain( & (yypvt[-1].chval) );
		yyval.chval = mkchain((tagptr) q, CHNULL);
		q->datalist = hookup(yypvt[-3].chval, yyval.chval);
		} break;
case 104:
# line 652 "gram.in"
{
		chainp p;
		struct Impldoblock *q;
		q = ALLOC(Impldoblock);
		q->tag = TIMPLDO;
		q->varnp = (Namep) (yypvt[-1].chval->datap);
		p = yypvt[-1].chval->nextp;
		if (p)  { q->implb = (expptr)(p->datap); p = p->nextp; }
		if (p)  { q->impub = (expptr)(p->datap); p = p->nextp; }
		if (p)  { q->impstep = (expptr)(p->datap); p = p->nextp; }
		frchain( & (yypvt[-1].chval) );
		yyval.chval = mkchain((tagptr) q, CHNULL);
		q->datalist = hookup(yypvt[-3].chval, yyval.chval);
		} break;
case 105:
# line 668 "gram.in"
{ ndim = 0; } break;
case 107:
# line 672 "gram.in"
{ ndim = 0; } break;
case 110:
# line 677 "gram.in"
{ if(ndim == maxdim)
			err("too many dimensions");
		  else if(ndim < maxdim)
			{ dims[ndim].lb = 0;
			  dims[ndim].ub = yypvt[-0].expval;
			}
		  ++ndim;
		} break;
case 111:
# line 686 "gram.in"
{ if(ndim == maxdim)
			err("too many dimensions");
		  else if(ndim < maxdim)
			{ dims[ndim].lb = yypvt[-2].expval;
			  dims[ndim].ub = yypvt[-0].expval;
			}
		  ++ndim;
		} break;
case 112:
# line 697 "gram.in"
{ yyval.expval = 0; } break;
case 114:
# line 702 "gram.in"
{ nstars = 1; labarray[0] = yypvt[-0].labval; } break;
case 115:
# line 704 "gram.in"
{ if(nstars < MAXLABLIST)  labarray[nstars++] = yypvt[-0].labval; } break;
case 116:
# line 708 "gram.in"
{ yyval.labval = execlab( convci(toklen, token) ); } break;
case 117:
# line 712 "gram.in"
{ NO66("IMPLICIT statement"); } break;
case 120:
# line 719 "gram.in"
{ needkwd = 1; } break;
case 121:
# line 720 "gram.in"
{ vartype = yypvt[-0].ival; } break;
case 124:
# line 728 "gram.in"
{ setimpl(vartype, varleng, yypvt[-0].ival, yypvt[-0].ival); } break;
case 125:
# line 730 "gram.in"
{ setimpl(vartype, varleng, yypvt[-2].ival, yypvt[-0].ival); } break;
case 126:
# line 734 "gram.in"
{ if(toklen!=1 || token[0]<'a' || token[0]>'z')
			{
			dclerr("implicit item must be single letter",(Namep) NULL);
			yyval.ival = 0;
			}
		  else yyval.ival = token[0];
		} break;
case 129:
# line 748 "gram.in"
{
		if(yypvt[-2].namval->vclass == CLUNKNOWN)
			{
			yypvt[-2].namval->vclass = CLNAMELIST;
			yypvt[-2].namval->vtype = TYINT;
			yypvt[-2].namval->vstg = STGINIT;
			yypvt[-2].namval->varxptr.namelist = yypvt[-0].chval;
			yypvt[-2].namval->vardesc.varno = ++lastvarno;
			}
		else dclerr("cannot be a namelist name", yypvt[-2].namval);
		} break;
case 130:
# line 762 "gram.in"
{ yyval.chval = mkchain((tagptr) yypvt[-0].namval, CHNULL); } break;
case 131:
# line 764 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, mkchain((tagptr) yypvt[-0].namval, CHNULL)); } break;
case 132:
# line 768 "gram.in"
{ switch(parstate)	
			{
			case OUTSIDE:	newproc();
					startproc((struct Extsym *)NULL,CLMAIN);
			case INSIDE:	parstate = INDCL;
			case INDCL:	break;

			default:
				dclerr("declaration among executables", (Namep) NULL);
			}
		} break;
case 133:
# line 782 "gram.in"
{ yyval.chval = 0; } break;
case 135:
# line 787 "gram.in"
{ yyval.chval = mkchain(yypvt[-0].expval, CHNULL); } break;
case 136:
# line 789 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].expval,CHNULL) ); } break;
case 138:
# line 794 "gram.in"
{ yyval.expval = yypvt[-1].expval; } break;
case 142:
# line 801 "gram.in"
{ yyval.expval = mkexpr(yypvt[-1].ival, yypvt[-2].expval, yypvt[-0].expval); } break;
case 143:
# line 803 "gram.in"
{ yyval.expval = mkexpr(OPSTAR, yypvt[-2].expval, yypvt[-0].expval); } break;
case 144:
# line 805 "gram.in"
{ yyval.expval = mkexpr(OPSLASH, yypvt[-2].expval, yypvt[-0].expval); } break;
case 145:
# line 807 "gram.in"
{ yyval.expval = mkexpr(OPPOWER, yypvt[-2].expval, yypvt[-0].expval); } break;
case 146:
# line 809 "gram.in"
{ if(yypvt[-1].ival == OPMINUS)
			yyval.expval = mkexpr(OPNEG, yypvt[-0].expval, ENULL);
		  else 	yyval.expval = yypvt[-0].expval;
		} break;
case 147:
# line 814 "gram.in"
{ yyval.expval = mkexpr(yypvt[-1].ival, yypvt[-2].expval, yypvt[-0].expval); } break;
case 148:
# line 816 "gram.in"
{ NO66(".EQV. operator");
		  yyval.expval = mkexpr(OPEQV, yypvt[-2].expval,yypvt[-0].expval); } break;
case 149:
# line 819 "gram.in"
{ NO66(".NEQV. operator");
		  yyval.expval = mkexpr(OPNEQV, yypvt[-2].expval, yypvt[-0].expval); } break;
case 150:
# line 822 "gram.in"
{ yyval.expval = mkexpr(OPOR, yypvt[-2].expval, yypvt[-0].expval); } break;
case 151:
# line 824 "gram.in"
{ yyval.expval = mkexpr(OPAND, yypvt[-2].expval, yypvt[-0].expval); } break;
case 152:
# line 826 "gram.in"
{ yyval.expval = mkexpr(OPNOT, yypvt[-0].expval, ENULL); } break;
case 153:
# line 828 "gram.in"
{ NO66("concatenation operator //");
		  yyval.expval = mkexpr(OPCONCAT, yypvt[-2].expval, yypvt[-0].expval); } break;
case 154:
# line 832 "gram.in"
{ yyval.ival = OPPLUS; } break;
case 155:
# line 833 "gram.in"
{ yyval.ival = OPMINUS; } break;
case 156:
# line 836 "gram.in"
{ yyval.ival = OPEQ; } break;
case 157:
# line 837 "gram.in"
{ yyval.ival = OPGT; } break;
case 158:
# line 838 "gram.in"
{ yyval.ival = OPLT; } break;
case 159:
# line 839 "gram.in"
{ yyval.ival = OPGE; } break;
case 160:
# line 840 "gram.in"
{ yyval.ival = OPLE; } break;
case 161:
# line 841 "gram.in"
{ yyval.ival = OPNE; } break;
case 162:
# line 845 "gram.in"
{ yyval.expval = mkprim(yypvt[-0].namval, PNULL, CHNULL); } break;
case 163:
# line 847 "gram.in"
{ NO66("substring operator :");
		  yyval.expval = mkprim(yypvt[-1].namval, PNULL, yypvt[-0].chval); } break;
case 164:
# line 850 "gram.in"
{ yyval.expval = mkprim(yypvt[-3].namval, mklist(yypvt[-1].chval), CHNULL); } break;
case 165:
# line 852 "gram.in"
{ NO66("substring operator :");
		  yyval.expval = mkprim(yypvt[-4].namval, mklist(yypvt[-2].chval), yypvt[-0].chval); } break;
case 166:
# line 857 "gram.in"
{ yyval.chval = mkchain(yypvt[-3].expval, mkchain(yypvt[-1].expval,CHNULL)); } break;
case 167:
# line 861 "gram.in"
{ yyval.expval = 0; } break;
case 169:
# line 866 "gram.in"
{ if(yypvt[-0].namval->vclass == CLPARAM)
			yyval.expval = (expptr) cpexpr(
				( (struct Paramblock *) (yypvt[-0].namval) ) -> paramval);
		} break;
case 171:
# line 873 "gram.in"
{ yyval.expval = mklogcon(1); } break;
case 172:
# line 874 "gram.in"
{ yyval.expval = mklogcon(0); } break;
case 173:
# line 875 "gram.in"
{ if ( toklen )
				yyval.expval = mkstrcon(toklen, token);
			  else
				yyval.expval = mkstrcon(1, ""); 
			} break;
case 174:
# line 880 "gram.in"
 { yyval.expval = mkintcon( convci(toklen, token) ); } break;
case 175:
# line 881 "gram.in"
 { yyval.expval = mkrealcon(TYREAL, convcd(toklen, token)); } break;
case 176:
# line 882 "gram.in"
 { yyval.expval = mkrealcon(TYDREAL, convcd(toklen, token)); } break;
case 177:
# line 886 "gram.in"
{ yyval.expval = mkcxcon(yypvt[-3].expval,yypvt[-1].expval); } break;
case 178:
# line 890 "gram.in"
{ NOEXT("hex constant");
		  yyval.expval = mkbitcon(4, toklen, token); } break;
case 179:
# line 893 "gram.in"
{ NOEXT("octal constant");
		  yyval.expval = mkbitcon(3, toklen, token); } break;
case 180:
# line 896 "gram.in"
{ NOEXT("binary constant");
		  yyval.expval = mkbitcon(1, toklen, token); } break;
case 182:
# line 902 "gram.in"
{ yyval.expval = yypvt[-1].expval; } break;
case 185:
# line 908 "gram.in"
{ yyval.expval = mkexpr(yypvt[-1].ival, yypvt[-2].expval, yypvt[-0].expval); } break;
case 186:
# line 910 "gram.in"
{ yyval.expval = mkexpr(OPSTAR, yypvt[-2].expval, yypvt[-0].expval); } break;
case 187:
# line 912 "gram.in"
{ yyval.expval = mkexpr(OPSLASH, yypvt[-2].expval, yypvt[-0].expval); } break;
case 188:
# line 914 "gram.in"
{ yyval.expval = mkexpr(OPPOWER, yypvt[-2].expval, yypvt[-0].expval); } break;
case 189:
# line 916 "gram.in"
{ if(yypvt[-1].ival == OPMINUS)
			yyval.expval = mkexpr(OPNEG, yypvt[-0].expval, ENULL);
		  else	yyval.expval = yypvt[-0].expval;
		} break;
case 190:
# line 921 "gram.in"
{ NO66("concatenation operator //");
		  yyval.expval = mkexpr(OPCONCAT, yypvt[-2].expval, yypvt[-0].expval); } break;
case 192:
# line 928 "gram.in"
{
		if(yypvt[-3].labval->labdefined)
			execerr("no backward DO loops", CNULL);
		yypvt[-3].labval->blklevel = blklevel+1;
		exdo(yypvt[-3].labval->labelno, yypvt[-0].chval);
		} break;
case 193:
# line 935 "gram.in"
{ exendif();  thiswasbranch = NO; } break;
case 195:
# line 938 "gram.in"
{ exelif(yypvt[-2].expval); lastwasbranch = NO; } break;
case 196:
# line 940 "gram.in"
{ exelse(); lastwasbranch = NO; } break;
case 197:
# line 942 "gram.in"
{ exendif(); lastwasbranch = NO; } break;
case 198:
# line 946 "gram.in"
{ exif(yypvt[-1].expval); } break;
case 199:
# line 950 "gram.in"
{ yyval.chval = mkchain(yypvt[-2].namval, yypvt[-0].chval); } break;
case 200:
# line 954 "gram.in"
{ exequals(yypvt[-2].expval, yypvt[-0].expval); } break;
case 201:
# line 956 "gram.in"
{ exassign(yypvt[-0].namval, yypvt[-2].labval); } break;
case 204:
# line 960 "gram.in"
{ inioctl = NO; } break;
case 205:
# line 962 "gram.in"
{ exarif(yypvt[-6].expval, yypvt[-4].labval, yypvt[-2].labval, yypvt[-0].labval);  thiswasbranch = YES; } break;
case 206:
# line 964 "gram.in"
{ excall(yypvt[-0].namval, PNULL, 0, labarray); } break;
case 207:
# line 966 "gram.in"
{ excall(yypvt[-2].namval, PNULL, 0, labarray); } break;
case 208:
# line 968 "gram.in"
{ if(nstars < MAXLABLIST)
			excall(yypvt[-3].namval, mklist(yypvt[-1].chval), nstars, labarray);
		  else
			err("too many alternate returns");
		} break;
case 209:
# line 974 "gram.in"
{ exreturn(yypvt[-0].expval);  thiswasbranch = YES; } break;
case 210:
# line 976 "gram.in"
{ exstop(yypvt[-2].ival, yypvt[-0].expval);  thiswasbranch = yypvt[-2].ival; } break;
case 211:
# line 980 "gram.in"
{ yyval.labval = mklabel( convci(toklen, token) ); } break;
case 212:
# line 984 "gram.in"
{ if(parstate == OUTSIDE)
			{
			newproc();
			startproc(PNULL, CLMAIN);
			}
		} break;
case 213:
# line 993 "gram.in"
{ exgoto(yypvt[-0].labval);  thiswasbranch = YES; } break;
case 214:
# line 995 "gram.in"
{ exasgoto(yypvt[-0].namval);  thiswasbranch = YES; } break;
case 215:
# line 997 "gram.in"
{ exasgoto(yypvt[-4].namval);  thiswasbranch = YES; } break;
case 216:
# line 999 "gram.in"
{ if(nstars < MAXLABLIST)
			putcmgo(fixtype(yypvt[-0].expval), nstars, labarray);
		  else
			err("computed GOTO list too long");
		} break;
case 219:
# line 1011 "gram.in"
{ nstars = 0; yyval.namval = yypvt[-0].namval; } break;
case 220:
# line 1015 "gram.in"
{ yyval.chval = (yypvt[-0].expval ? mkchain(yypvt[-0].expval,CHNULL) : CHNULL); } break;
case 221:
# line 1017 "gram.in"
{ if(yypvt[-0].expval)
			if(yypvt[-2].chval) yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].expval,CHNULL));
			else yyval.chval = mkchain(yypvt[-0].expval,CHNULL);
		  else
			yyval.chval = yypvt[-2].chval;
		} break;
case 223:
# line 1027 "gram.in"
{ if(nstars<MAXLABLIST) labarray[nstars++] = yypvt[-0].labval; yyval.expval = 0; } break;
case 224:
# line 1031 "gram.in"
{ yyval.ival = 0; } break;
case 225:
# line 1033 "gram.in"
{ yyval.ival = 1; } break;
case 226:
# line 1037 "gram.in"
{ yyval.chval = mkchain(yypvt[-0].expval, CHNULL); } break;
case 227:
# line 1039 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].expval,CHNULL) ); } break;
case 228:
# line 1043 "gram.in"
{ if(parstate == OUTSIDE)
			{
			newproc();
			startproc(PNULL, CLMAIN);
			}
		  if(parstate < INDATA) enddcl();
		} break;
case 229:
# line 1052 "gram.in"
{	intonly = YES;	} break;
case 230:
# line 1055 "gram.in"
{	intonly = NO;	} break;
case 231:
# line 1061 "gram.in"
{ endio(); } break;
case 233:
# line 1066 "gram.in"
{ ioclause(IOSUNIT, yypvt[-0].expval); endioctl(); } break;
case 234:
# line 1068 "gram.in"
{ ioclause(IOSUNIT, PNULL); endioctl(); } break;
case 235:
# line 1070 "gram.in"
{ ioclause(IOSUNIT, IOSTDERR); endioctl(); } break;
case 237:
# line 1073 "gram.in"
{ doio(PNULL); } break;
case 238:
# line 1075 "gram.in"
{ doio(PNULL); } break;
case 239:
# line 1077 "gram.in"
{ doio(yypvt[-0].chval); } break;
case 240:
# line 1079 "gram.in"
{ doio(yypvt[-0].chval); } break;
case 241:
# line 1081 "gram.in"
{ doio(yypvt[-0].chval); } break;
case 242:
# line 1083 "gram.in"
{ doio(PNULL); } break;
case 243:
# line 1085 "gram.in"
{ doio(yypvt[-0].chval); } break;
case 244:
# line 1087 "gram.in"
{ doio(PNULL); } break;
case 245:
# line 1089 "gram.in"
{ doio(yypvt[-0].chval); } break;
case 247:
# line 1096 "gram.in"
{ iostmt = IOBACKSPACE; } break;
case 248:
# line 1098 "gram.in"
{ iostmt = IOREWIND; } break;
case 249:
# line 1100 "gram.in"
{ iostmt = IOENDFILE; } break;
case 251:
# line 1107 "gram.in"
{ iostmt = IOINQUIRE; } break;
case 252:
# line 1109 "gram.in"
{ iostmt = IOOPEN; } break;
case 253:
# line 1111 "gram.in"
{ iostmt = IOCLOSE; } break;
case 254:
# line 1115 "gram.in"
{
		ioclause(IOSUNIT, PNULL);
		ioclause(IOSFMT, yypvt[-0].expval);
		endioctl();
		} break;
case 255:
# line 1121 "gram.in"
{
		ioclause(IOSUNIT, PNULL);
		ioclause(IOSFMT, PNULL);
		endioctl();
		} break;
case 256:
# line 1129 "gram.in"
{ if(yypvt[-1].expval->headblock.vtype == TYCHAR)
			{
			ioclause(IOSUNIT, PNULL);
			ioclause(IOSFMT, yypvt[-1].expval);
			}
		  else
			ioclause(IOSUNIT, yypvt[-1].expval);
		  endioctl();
		} break;
case 257:
# line 1139 "gram.in"
{ endioctl(); } break;
case 260:
# line 1147 "gram.in"
{ ioclause(IOSPOSITIONAL, yypvt[-0].expval); } break;
case 261:
# line 1149 "gram.in"
{ ioclause(IOSPOSITIONAL, PNULL); } break;
case 262:
# line 1151 "gram.in"
{ ioclause(IOSPOSITIONAL, IOSTDERR); } break;
case 263:
# line 1153 "gram.in"
{ ioclause(yypvt[-1].ival, yypvt[-0].expval); } break;
case 264:
# line 1155 "gram.in"
{ ioclause(yypvt[-1].ival, PNULL); } break;
case 265:
# line 1157 "gram.in"
{ ioclause(yypvt[-1].ival, IOSTDERR); } break;
case 266:
# line 1161 "gram.in"
{ yyval.ival = iocname(); } break;
case 267:
# line 1165 "gram.in"
{ iostmt = IOREAD; } break;
case 268:
# line 1169 "gram.in"
{ iostmt = IOWRITE; } break;
case 269:
# line 1173 "gram.in"
{
		iostmt = IOWRITE;
		ioclause(IOSUNIT, PNULL);
		ioclause(IOSFMT, yypvt[-1].expval);
		endioctl();
		} break;
case 270:
# line 1180 "gram.in"
{
		iostmt = IOWRITE;
		ioclause(IOSUNIT, PNULL);
		ioclause(IOSFMT, PNULL);
		endioctl();
		} break;
case 271:
# line 1189 "gram.in"
{ yyval.chval = mkchain(yypvt[-0].tagval, CHNULL); } break;
case 272:
# line 1191 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].tagval, CHNULL)); } break;
case 273:
# line 1195 "gram.in"
{ yyval.tagval = (tagptr) yypvt[-0].expval; } break;
case 274:
# line 1197 "gram.in"
{ yyval.tagval = (tagptr) mkiodo(yypvt[-1].chval,yypvt[-3].chval); } break;
case 275:
# line 1201 "gram.in"
{ yyval.chval = mkchain(yypvt[-0].expval, CHNULL); } break;
case 276:
# line 1203 "gram.in"
{ yyval.chval = mkchain(yypvt[-0].tagval, CHNULL); } break;
case 278:
# line 1208 "gram.in"
{ yyval.chval = mkchain(yypvt[-2].expval, mkchain(yypvt[-0].expval, CHNULL) ); } break;
case 279:
# line 1210 "gram.in"
{ yyval.chval = mkchain(yypvt[-2].expval, mkchain(yypvt[-0].tagval, CHNULL) ); } break;
case 280:
# line 1212 "gram.in"
{ yyval.chval = mkchain(yypvt[-2].tagval, mkchain(yypvt[-0].expval, CHNULL) ); } break;
case 281:
# line 1214 "gram.in"
{ yyval.chval = mkchain(yypvt[-2].tagval, mkchain(yypvt[-0].tagval, CHNULL) ); } break;
case 282:
# line 1216 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].expval, CHNULL) ); } break;
case 283:
# line 1218 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].tagval, CHNULL) ); } break;
case 284:
# line 1222 "gram.in"
{ yyval.tagval = (tagptr) yypvt[-0].expval; } break;
case 285:
# line 1224 "gram.in"
{ yyval.tagval = (tagptr) mkiodo(yypvt[-1].chval, mkchain(yypvt[-3].expval, CHNULL) ); } break;
case 286:
# line 1226 "gram.in"
{ yyval.tagval = (tagptr) mkiodo(yypvt[-1].chval, mkchain(yypvt[-3].tagval, CHNULL) ); } break;
case 287:
# line 1228 "gram.in"
{ yyval.tagval = (tagptr) mkiodo(yypvt[-1].chval, yypvt[-3].chval); } break;
case 288:
# line 1232 "gram.in"
{ startioctl(); } break;
	}
	goto yystack;		/* reset registers in driver code */
}
