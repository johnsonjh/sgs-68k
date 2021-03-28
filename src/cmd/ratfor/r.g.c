
# line 10 "r.g.y"
#ident	"@(#)ratfor:r.g.y	25.1"
extern int transfer;
extern	int	indent;
# define IF 257
# define ELSE 258
# define FOR 259
# define WHILE 260
# define BREAK 261
# define NEXT 262
# define DIGITS 263
# define DO 264
# define GOK 265
# define DEFINE 266
# define INCLUDE 267
# define REPEAT 268
# define UNTIL 269
# define RETURN 270
# define SWITCH 271
# define CASE 272
# define DEFAULT 273
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
typedef int yytabelem;
# define YYERRCODE 256

# line 75 "r.g.y"

yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 46,
	272, 27,
	273, 27,
	125, 27,
	-2, 0,
-1, 49,
	125, 25,
	-2, 0,
	};
# define YYNPROD 36
# define YYLAST 229
yytabelem yyact[]={

    14,    32,    45,    32,    47,    40,    41,    39,     1,    30,
     2,    31,    44,    42,    26,    27,    14,    33,    34,    35,
    29,    19,    36,    16,    37,    11,     8,    38,     7,     6,
    28,     5,     4,     3,     0,     0,     0,     0,     0,    43,
    46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    49,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    15,     0,    48,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    15,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    17,    18,     0,
    21,    20,     9,    10,    24,    23,    12,     0,     0,    22,
     0,    13,    25,    17,    18,     0,    21,    20,     9,    10,
    24,    23,    12,     0,     0,    22,     0,    13,    25 };
yytabelem yypact[]={

 -1000,   -43, -1000,   -43,   -43,  -269,   -43,   -43,   -43, -1000,
 -1000,   -43, -1000, -1000, -1000, -1000,   -43, -1000, -1000,  -116,
 -1000, -1000, -1000, -1000, -1000, -1000,  -253, -1000,  -119,  -271,
 -1000, -1000, -1000, -1000, -1000,  -265, -1000,   -59, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000,   -43, -1000, -1000,   -43 };
yytabelem yypgo[]={

     0,     8,    10,    33,    32,    31,    30,    29,    28,    26,
    25,    23,    21,    20,    13,     9,    12,    11 };
yytabelem yyr1[]={

     0,     1,     1,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     5,
    12,     6,     6,    13,    13,    14,    16,    15,    17,    11,
     3,     4,     7,     8,     9,    10 };
yytabelem yyr2[]={

     0,     4,     0,     5,     5,     7,     5,     5,     7,     5,
     3,     3,     5,     3,     3,     2,     6,     4,     3,     4,
     3,     3,     5,     2,     4,     4,     3,     4,     3,     3,
     3,     7,     3,     3,     3,     3 };
yytabelem yychk[]={

 -1000,    -1,    -2,    -3,    -4,    -5,    -7,    -8,    -9,   261,
   262,   -10,   265,   270,    59,   123,   -11,   256,   257,   -12,
   260,   259,   268,   264,   263,   271,    -2,    -2,    -6,   -13,
   -15,   -17,   272,    -2,    -2,    -2,    -2,    -1,    -2,   123,
   258,   125,   -14,   -15,   -16,   273,    -1,   269,   125,    -1 };
yytabelem yydef[]={

     2,    -2,     1,     0,     0,     0,     0,     0,     0,    10,
    11,     0,    13,    14,    15,     2,     0,    18,    30,     0,
    32,    33,    34,    35,    29,    20,     3,     4,     0,    21,
    23,     2,    28,     6,     7,     9,    12,     0,    17,    19,
    31,     5,    22,    24,     2,    26,    -2,     8,    16,    -2 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"IF",	257,
	"ELSE",	258,
	"FOR",	259,
	"WHILE",	260,
	"BREAK",	261,
	"NEXT",	262,
	"DIGITS",	263,
	"DO",	264,
	"GOK",	265,
	"DEFINE",	266,
	"INCLUDE",	267,
	"REPEAT",	268,
	"UNTIL",	269,
	"RETURN",	270,
	"SWITCH",	271,
	"CASE",	272,
	"DEFAULT",	273,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"statl : statl stat",
	"statl : /* empty */",
	"stat : if stat",
	"stat : ifelse stat",
	"stat : switch fullcase '}'",
	"stat : while stat",
	"stat : for stat",
	"stat : repeat stat UNTIL",
	"stat : repeat stat",
	"stat : BREAK",
	"stat : NEXT",
	"stat : do stat",
	"stat : GOK",
	"stat : RETURN",
	"stat : ';'",
	"stat : '{' statl '}'",
	"stat : label stat",
	"stat : error",
	"switch : sw '{'",
	"sw : SWITCH",
	"fullcase : caselist",
	"fullcase : caselist defpart",
	"caselist : casepart",
	"caselist : caselist casepart",
	"defpart : default statl",
	"default : DEFAULT",
	"casepart : case statl",
	"case : CASE",
	"label : DIGITS",
	"if : IF",
	"ifelse : if stat ELSE",
	"while : WHILE",
	"for : FOR",
	"repeat : REPEAT",
	"do : DO",
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
# line 26 "r.g.y"
{ indent--; outcont(yypvt[-1]); } break;
case 4:
# line 27 "r.g.y"
{ indent--; outcont(yypvt[-1]+1); } break;
case 5:
# line 28 "r.g.y"
{ endsw(yypvt[-2], yypvt[-1]); } break;
case 6:
# line 29 "r.g.y"
{ whilestat(yypvt[-1]); } break;
case 7:
# line 30 "r.g.y"
{ forstat(yypvt[-1]); } break;
case 8:
# line 31 "r.g.y"
{ untils(yypvt[-2],1); } break;
case 9:
# line 32 "r.g.y"
{ untils(yypvt[-1],0); } break;
case 10:
# line 33 "r.g.y"
{ breakcode(); } break;
case 11:
# line 34 "r.g.y"
{ nextcode(); } break;
case 12:
# line 35 "r.g.y"
{ dostat(yypvt[-1]); } break;
case 13:
# line 36 "r.g.y"
{ gokcode(yypvt[-0]); } break;
case 14:
# line 37 "r.g.y"
{ retcode(); } break;
case 18:
# line 41 "r.g.y"
{ errcode(); yyclearin; } break;
case 20:
# line 45 "r.g.y"
{ swcode(); } break;
case 21:
# line 47 "r.g.y"
{ yyval = 0; } break;
case 22:
# line 48 "r.g.y"
{ yyval = 1; } break;
case 26:
# line 55 "r.g.y"
{ getdefault(); } break;
case 28:
# line 59 "r.g.y"
{ getcase(); } break;
case 29:
# line 61 "r.g.y"
{ transfer = 0; outcode(yypvt[-0]); } break;
case 30:
# line 63 "r.g.y"
{ ifcode(); } break;
case 31:
# line 65 "r.g.y"
{ elsecode(yypvt[-2]); } break;
case 32:
# line 67 "r.g.y"
{ whilecode(); } break;
case 33:
# line 69 "r.g.y"
{ forcode(); } break;
case 34:
# line 71 "r.g.y"
{ repcode(); } break;
case 35:
# line 73 "r.g.y"
{ docode(); } break;
	}
	goto yystack;		/* reset registers in driver code */
}
