# include "stdio.h"
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX 200
# define output(c) putc(c,yyout)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):efgetc)==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
# define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng; extern char yytext[];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin = {stdin}, *yyout = {stdout};
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/
/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/
# define DOTSON 2
/*	ATT: ident	"efl:lex.l	1.3"			*/

#ident	"@(#)efl:lex.l	25.1"

#include <ctype.h>
#include "defs"
#include "tokdefs"

typedef union { int ival; ptr pval; } YYSTYPE;
extern YYSTYPE yylval;
YYSTYPE prevl;
int prevv;
char *copys();
static ptr p;
static ptr q;
static FILE *fd;
static int quoted, k;
static int rket	= 0;
FILE *opincl();
ptr mkdef(), mkcomm(), mkname(), mkimcon();

#define RET(x)	{ RETI(x,x);  }

#define RETL(yv,yl) {yylval=prevl=yl;igeol=comneed=0;return(prevv=yv); }
#define RETP(yv,yl) {yylval.pval=prevl.pval=yl;igeol=comneed=0;return(prevv=yv); }
#define RETI(yv,yl) {yylval.ival=prevl.ival=yl;igeol=comneed=0;return(prevv=yv); }
#define REL(n)  {  RETI(RELOP, OPREL+n);}
#define AS(n)  {  RETI(ASGNOP, OPASGN+n); }
#define RETC(x) { RETP(CONST, mkconst(x,yytext) );  }
#define RETZ(x) { yytext[yyleng-1] = '\0'; RETP(CONST, mkimcon(x,yytext) ); }

# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
if(pushlex)
	if(pushlex==1)
		{
		pushlex = 2;
		yylval.ival = 0;
		return(EOS);
		}
	else	{
		pushlex = 0;
		if(rket == 2)
			rket = 1;
		else	RETL(prevv,prevl);
		}
if(rket > 0)
	{
	if(rket==1)
		{
		rket = 2;
		RET(RBRACK);
		}
	else	{
		rket = 0;
		RET(EOS);
		}
	}
if(eofneed) return(0);
if(forcerr) return(-1);
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:
{
		   lower(yytext);
		   if(lettneed && yyleng==1)
			{ RETI(LETTER, yytext[0]); }
		   else if(defneed)
			{
			register char *q1, *q2;
			for(q2=q1=yytext+yyleng+1 ; (*q1 = efgetc)!='\n' ; ++q1)
				;
			*q1 = '\0';
			p = mkdef(yytext, q2);
			defneed = 0;
			++yylineno;
			unput('\n');
			}
		   else if(optneed)
			{ RETP(OPTNAME, copys(yytext)); }
		   else if(comneed && ( (q=name(yytext,1))==NULL || q->tag!=TDEFINE) )
			{ RETP(COMNAME, mkcomm(yytext) ); }
		   else if(q = name(yytext,1)) switch(q->tag)
			{
			case TDEFINE:
				filelines[filedepth] = yylineno;
				filemacs[filedepth] = efmacp;
				pushchars[filedepth] = (yysptr>yysbuf?
						*--yysptr : -1);
				if(++filedepth >= MAXINCLUDEDEPTH)
					fatal("macro or include too deep");
				filelines[filedepth] = yylineno = 1;
				efmacp = q->varp->valp;
				filenames[filedepth] = NULL;
				break;	/*now process new input */

			case TSTRUCT:
				RETP(STRUCTNAME, q);

			case TNAME:
				RETP(NAME, q);

			case TKEYWORD:
				if(q->subtype == END)
					{
					register int c;
					eofneed = YES;
					while((c=input())!=';'&&c!='\n'&&c!=EOF)
						;
					NLSTATE;
					}
				RET(q->subtype);

			default:
				fatal1("lex: impossible type code %d", q->tag);
			}
		   else  RETP(NAME, mkname(yytext) );
		}
break;
case 2:
RET(COMMA);
break;
case 3:
RET(EOS);
break;
case 4:
RET(LPAR);
break;
case 5:
RET(RPAR);
break;
case 6:
case 7:
RET(LBRACK);
break;
case 8:
case 9:
{ if(iobrlevel>0) RET(RBRACK); rket = 1;  RET(EOS); }
break;
case 10:
RET(COMMA);
break;
case 11:
RET(COLON);
break;
case 12:
RET(REPOP);
break;
case 13:
case 14:
RETI(OR,OPOR);
break;
case 15:
case 16:
RETI(OR,OP2OR);
break;
case 17:
case 18:
RETI(AND,OPAND);
break;
case 19:
case 20:
RETI(AND,OP2AND);
break;
case 21:
case 22:
RETI(NOT,OPNOT);
break;
case 23:
RETI(NOT,OPNOT);
break;
case 24:
case 25:
REL(OPLT);
break;
case 26:
case 27:
REL(OPLE);
break;
case 28:
case 29:
REL(OPGT);
break;
case 30:
case 31:
REL(OPGE);
break;
case 32:
case 33:
REL(OPEQ);
break;
case 34:
case 35:
case 36:
REL(OPNE);
break;
case 37:
RET(ARROW);
break;
case 38:
RET(QUALOP);
break;
case 39:
RETI(ADDOP, OPPLUS);
break;
case 40:
RETI(ADDOP, OPMINUS);
break;
case 41:
RETI(MULTOP, OPSTAR);
break;
case 42:
RETI(MULTOP, OPSLASH);
break;
case 43:
case 44:
RETI(POWER, OPPOWER);
break;
case 45:
RETI(DOUBLEADDOP, OPPLUS);
break;
case 46:
RETI(DOUBLEADDOP, OPMINUS);
break;
case 47:
AS(OPASGN);
break;
case 48:
AS(OPPLUS);
break;
case 49:
AS(OPMINUS);
break;
case 50:
AS(OPSTAR);
break;
case 51:
AS(OPSLASH);
break;
case 52:
case 53:
AS(OPPOWER);
break;
case 54:
AS(OPAND);
break;
case 55:
AS(OP2AND);
break;
case 56:
AS(OPOR);
break;
case 57:
AS(OP2OR);
break;
case 58:
case 59:
{ yytext[yyleng-1] = '\0'; p = mkconst(TYCHAR,yytext+1);
		  RETP(CONST,p); }
break;
case 60:
{ /* nh construct */
		int i, n;  char c;
		yytext[yyleng-1] = '\0';  n = convci(yytext);
		for(i = 0; i<n ; ++i)
			if( (c=yytext[i]=input()) == '\n' || c=='\0') break;
		yytext[i] = '\0';
		p = mkconst(TYCHAR,yytext);
		p->vtypep = mkint(i);
		RETP(CONST, p);
		}
break;
case 61:
	RETC(TYINT);
break;
case 62:
case 63:
RETC(TYREAL);
break;
case 64:
case 65:
RETC(TYREAL);
break;
case 66:
case 67:
RETC(TYLREAL);
break;
case 68:
	{ yytext[yyleng-1] = '.';
		  RETP(CONST,mkimcon(TYCOMPLEX,yytext)); }
break;
case 69:
case 70:
RETZ(TYCOMPLEX);
break;
case 71:
case 72:
RETZ(TYCOMPLEX);
break;
case 73:
case 74:
RETZ(TYLCOMPLEX);
break;
case 75:
{ if(! nocommentflag) goto litline; }
break;
case 76:
{ if(thisexec) thisexec->nftnst += 2;
	  if(inproc)
		{
		unput('\n');
		RETP(ESCAPE, copys(yytext));
		}

	litline:	p = mkchain( copys(yytext), CHNULL);
			if(inproc==0 && yytext[0]=='%')
				prevcomments = hookup(prevcomments, p);
			else
				comments =  hookup(comments,p);
	}
break;
case 77:
;
break;
case 78:
;
break;
case 79:
;
break;
case 80:
;
break;
case 81:
{ if(igeol) { igeol=0; prevv = NEWLINE; }
	  else if(prevv>=NAME || prevv==RPAR || prevv==RBRACK
			|| prevv== -1 || prevv==QUALOP)
		RET(EOS); }
break;
case 82:
{ char * linerr();
	  fprintf(diagfile, "Bad input character %c %s\n", yytext[0], linerr());
	  ++nerrs;
	}
break;
case 83:
{ /* Include statement */
	char *q1;
	register char *q2;
	for(q1=yytext ; *q1==' ' || *q1=='\t' ; ++q1) ;
	quoted = NO;
	for(q1 += 7 ; *q1==' ' || *q1=='\t' ||
		*q1=='\'' || *q1=='"' || *q1=='(' ; ++q1 )
			if(*q1=='"' || *q1=='\'')
				quoted = YES;
	for(q2=q1 ; *q2!='\0' &&  *q2!=' ' && *q2!='\n' &&
		*q2!='\'' && *q2!='"' && *q2!=')' ; ++q2 )
			;
	*q2 = '\0';
	if( ! quoted)
		for(k=0; (q = name(q1,1)) && q->tag==TDEFINE ; ++k)
			{
			if(k > MAXINCLUDEDEPTH)
				fatal1("Macros too deep for %s", yytext);
			q1 = q->varp->valp;
			}
	if( (fd = opincl(&q1)) == NULL)
		{
		fprintf(diagfile, "Cannot open file %s.  Stop.\n", q1);
		exit(2);
		}
	filelines[filedepth] = yylineno;
	pushchars[filedepth] = '\n';
	if(++filedepth >= MAXINCLUDEDEPTH)
		fatal("macro or include too deep");
	fileptrs[filedepth] = yyin = fd;
	filenames[filedepth] = copys(q1);
	filelines[filedepth] = yylineno = 1;
	filemacs[filedepth] = NULL;
	}
break;
case -1:
break;
default:
fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */

yywrap()
{
if(filedepth == 0)
	{
	ateof = 1;
	return(1);
	}

if(efmacp == 0)
	{
	fclose(yyin);
	cfree(filenames[filedepth]);
	}

--filedepth;
if( filemacs[filedepth] )
	efmacp = filemacs[filedepth];
else	{
	yyin = fileptrs[filedepth];
	efmacp = 0;
	}
yylineno = filelines[filedepth];
if(pushchars[filedepth] != -1)
	unput( pushchars[filedepth] );
return(0);
}



lower(s)	/* replace upper with lower case letters */
register char *s;
{
register char *t;
for(t=s ; *t ; ++t)
	if( isupper(*t) )
		*s++ = tolower(*t);
	else if(*t != '_')
		*s++ = *t;
}




setdot(k)
int k;
{
if(k)
	BEGIN DOTSON;
else	BEGIN 0;
}




FILE *opincl(namep)
char **namep;
{
#ifndef unix
	return( fopen(*namep, "r") );
#else

	/* On Unix, follow the C include conventions */
	
	register char *s, *lastslash;
	char *dir, *name, temp[100];
	int i;
	FILE *fp;
	
	name = *namep;
	if(name[0] == '/')
		return( fopen(name, "r") );
	
	dir = basefile;
	for(i = filedepth ; i>=0 ; --i)
		if( filemacs[i] == NULL)
			{
			dir = filenames[i];
			break;
			}

	lastslash = NULL;
	for(s = dir ; *s ; ++s)
		if(*s == '/')
			lastslash = s;
	if(lastslash)
		{
		*lastslash = '\0';
		sprintf(temp, "%s/%s", dir, name);
		*lastslash = '/';
		if( fp = fopen(temp, "r") )
			*namep = temp;
		}
	else
		fp = fopen(name, "r");
	
	if(fp == NULL)
		{
		sprintf(temp, "/usr/include/%s", name);
		fp = fopen(temp, "r");
		*namep = temp;
		}
	return(fp);

#endif
}

int yyvstop[] = {
0,

82,
0,

78,
82,
0,

81,
0,

79,
82,
0,

77,
82,
0,

23,
82,
0,

82,
0,

75,
82,
0,

12,
82,
0,

18,
82,
0,

82,
0,

4,
82,
0,

5,
82,
0,

41,
82,
0,

39,
82,
0,

2,
10,
82,
0,

40,
82,
0,

38,
82,
0,

42,
82,
0,

61,
82,
0,

11,
82,
0,

3,
82,
0,

25,
82,
0,

47,
82,
0,

29,
82,
0,

1,
82,
0,

6,
82,
0,

8,
82,
0,

44,
82,
0,

82,
0,

7,
82,
0,

14,
82,
0,

9,
82,
0,

22,
82,
0,

78,
82,
0,

77,
82,
0,

76,
82,
0,

1,
82,
0,

38,
82,
0,

36,
0,

59,
0,

75,
0,

20,
0,

54,
0,

58,
0,

43,
0,

50,
0,

45,
0,

48,
0,

46,
0,

49,
0,

37,
0,

63,
0,

51,
0,

62,
0,

61,
0,

60,
0,

68,
0,

27,
0,

33,
0,

31,
0,

1,
0,

53,
0,

80,
0,

56,
0,

16,
0,

35,
0,

76,
0,

1,
0,

55,
0,

52,
0,

70,
0,

62,
63,
0,

69,
0,

66,
0,

64,
0,

57,
0,

1,
0,

67,
0,

65,
0,

69,
70,
0,

73,
0,

71,
0,

1,
0,

32,
0,

30,
0,

28,
0,

26,
0,

24,
0,

34,
0,

13,
0,

74,
0,

72,
0,

66,
67,
0,

64,
65,
0,

1,
0,

17,
0,

15,
0,

21,
0,

73,
74,
0,

71,
72,
0,

1,
0,

19,
0,

1,
0,

83,
0,
0};
# define YYTYPE int
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,5,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,6,	1,7,	
45,0,	1,8,	47,0,	34,72,	
34,73,	50,0,	79,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	1,9,	1,10,	1,11,	
1,12,	1,13,	34,72,	1,14,	
1,15,	1,16,	1,17,	1,18,	
1,19,	1,20,	1,21,	1,22,	
1,23,	1,24,	0,0,	0,0,	
14,48,	19,54,	0,0,	0,0,	
0,0,	18,52,	105,127,	1,25,	
1,26,	1,27,	1,28,	1,29,	
10,44,	23,60,	1,30,	1,30,	
1,30,	1,30,	1,30,	19,55,	
1,30,	1,30,	1,30,	14,49,	
18,53,	1,30,	27,67,	1,30,	
1,30,	2,39,	1,30,	1,30,	
2,8,	1,30,	1,30,	28,68,	
29,69,	33,71,	38,76,	36,74,	
1,31,	48,88,	1,32,	1,33,	
1,34,	42,80,	39,77,	52,89,	
75,99,	106,128,	107,129,	78,100,	
2,40,	2,10,	80,101,	2,12,	
2,13,	2,41,	2,14,	108,130,	
2,16,	2,17,	2,18,	109,131,	
2,20,	2,21,	2,22,	2,23,	
21,56,	39,77,	110,132,	3,8,	
1,35,	1,36,	1,37,	1,38,	
81,102,	42,80,	2,25,	2,26,	
2,27,	2,28,	2,29,	78,100,	
21,57,	21,58,	80,101,	112,134,	
124,143,	126,145,	133,146,	3,9,	
3,10,	2,42,	3,12,	3,13,	
144,151,	3,14,	0,0,	3,16,	
3,17,	3,18,	36,75,	3,20,	
3,21,	3,43,	3,23,	83,105,	
81,102,	87,112,	39,78,	2,31,	
0,0,	2,32,	2,33,	96,120,	
0,0,	3,25,	3,26,	3,27,	
3,28,	3,29,	22,59,	22,59,	
22,59,	22,59,	22,59,	22,59,	
22,59,	22,59,	22,59,	22,59,	
0,0,	0,0,	0,0,	98,121,	
0,0,	4,39,	0,0,	83,105,	
4,8,	87,112,	39,78,	2,35,	
2,36,	2,37,	2,38,	96,120,	
82,103,	0,0,	3,31,	0,0,	
3,32,	3,33,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
4,40,	4,10,	82,104,	4,12,	
4,13,	4,41,	4,14,	98,121,	
4,16,	4,17,	4,18,	0,0,	
4,20,	4,21,	4,43,	4,23,	
59,90,	59,91,	84,106,	0,0,	
82,103,	59,92,	3,35,	3,36,	
3,37,	3,38,	4,25,	4,26,	
4,27,	4,28,	4,29,	11,45,	
0,0,	84,107,	82,104,	86,110,	
85,108,	0,0,	100,122,	11,45,	
11,0,	4,42,	93,117,	93,118,	
101,123,	86,111,	0,0,	93,119,	
59,90,	59,91,	84,106,	85,109,	
102,124,	59,92,	103,125,	104,126,	
0,0,	0,0,	111,133,	4,31,	
0,0,	4,32,	4,33,	0,0,	
11,46,	84,107,	0,0,	86,110,	
85,108,	11,45,	100,122,	0,0,	
114,135,	11,45,	93,117,	93,118,	
101,123,	86,111,	11,45,	93,119,	
12,47,	0,0,	0,0,	85,109,	
102,124,	116,136,	103,125,	104,126,	
12,47,	12,0,	111,133,	4,35,	
4,36,	4,37,	4,38,	11,45,	
11,45,	11,45,	11,45,	11,45,	
122,141,	11,45,	11,45,	11,45,	
114,135,	0,0,	11,45,	123,142,	
11,45,	11,45,	125,144,	11,45,	
11,45,	12,47,	11,45,	11,45,	
0,0,	116,136,	12,47,	0,0,	
0,0,	138,147,	12,47,	0,0,	
140,148,	11,45,	0,0,	12,47,	
0,0,	15,50,	0,0,	0,0,	
122,141,	141,149,	142,150,	0,0,	
149,152,	15,50,	15,0,	123,142,	
150,153,	0,0,	125,144,	0,0,	
12,47,	12,47,	12,47,	12,47,	
12,47,	0,0,	12,47,	12,47,	
12,47,	138,147,	0,0,	12,47,	
140,148,	12,47,	12,47,	152,154,	
12,47,	12,47,	15,50,	12,47,	
12,47,	141,149,	142,150,	15,51,	
149,152,	0,0,	0,0,	15,50,	
150,153,	63,95,	12,47,	63,95,	
15,50,	0,0,	63,96,	63,96,	
63,96,	63,96,	63,96,	63,96,	
63,96,	63,96,	63,96,	63,96,	
0,0,	0,0,	0,0,	152,154,	
0,0,	15,50,	15,50,	15,50,	
15,50,	15,50,	0,0,	15,50,	
15,50,	15,50,	0,0,	0,0,	
15,50,	0,0,	15,50,	15,50,	
0,0,	15,50,	15,50,	0,0,	
15,50,	15,50,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	24,61,	15,50,	
24,62,	24,62,	24,62,	24,62,	
24,62,	24,62,	24,62,	24,62,	
24,62,	24,62,	95,96,	95,96,	
95,96,	95,96,	95,96,	95,96,	
95,96,	95,96,	95,96,	95,96,	
24,63,	24,64,	0,0,	0,0,	
24,65,	24,66,	64,97,	0,0,	
64,97,	0,0,	0,0,	64,98,	
64,98,	64,98,	64,98,	64,98,	
64,98,	64,98,	64,98,	64,98,	
64,98,	30,70,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
30,70,	30,70,	30,70,	0,0,	
24,63,	24,64,	0,0,	0,0,	
24,65,	24,66,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
0,0,	0,0,	0,0,	0,0,	
30,70,	0,0,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
30,70,	30,70,	30,70,	30,70,	
41,79,	0,0,	0,0,	0,0,	
0,0,	90,113,	0,0,	90,113,	
41,79,	41,0,	90,114,	90,114,	
90,114,	90,114,	90,114,	90,114,	
90,114,	90,114,	90,114,	90,114,	
97,98,	97,98,	97,98,	97,98,	
97,98,	97,98,	97,98,	97,98,	
97,98,	97,98,	0,0,	0,0,	
0,0,	41,79,	0,0,	0,0,	
0,0,	0,0,	41,79,	0,0,	
0,0,	0,0,	41,79,	91,115,	
0,0,	91,115,	0,0,	41,79,	
91,116,	91,116,	91,116,	91,116,	
91,116,	91,116,	91,116,	91,116,	
91,116,	91,116,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
41,79,	41,79,	41,79,	41,79,	
41,79,	0,0,	41,79,	41,79,	
41,79,	0,0,	0,0,	41,79,	
0,0,	41,79,	41,79,	0,0,	
41,79,	41,79,	0,0,	41,79,	
41,79,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	41,79,	43,59,	
43,59,	43,59,	43,59,	43,59,	
43,59,	43,59,	43,59,	43,59,	
43,59,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
43,81,	0,0,	43,82,	0,0,	
43,83,	0,0,	43,84,	0,0,	
0,0,	0,0,	0,0,	43,85,	
0,0,	43,86,	43,87,	0,0,	
0,0,	0,0,	0,0,	61,93,	
61,93,	61,93,	61,93,	61,93,	
61,93,	61,93,	61,93,	61,93,	
61,93,	0,0,	0,0,	0,0,	
43,81,	0,0,	43,82,	0,0,	
43,83,	0,0,	43,84,	61,63,	
61,64,	0,0,	0,0,	43,85,	
61,94,	43,86,	43,87,	113,114,	
113,114,	113,114,	113,114,	113,114,	
113,114,	113,114,	113,114,	113,114,	
113,114,	115,116,	115,116,	115,116,	
115,116,	115,116,	115,116,	115,116,	
115,116,	115,116,	115,116,	0,0,	
0,0,	0,0,	0,0,	61,63,	
61,64,	117,137,	0,0,	117,137,	
61,94,	0,0,	117,138,	117,138,	
117,138,	117,138,	117,138,	117,138,	
117,138,	117,138,	117,138,	117,138,	
118,139,	0,0,	118,139,	0,0,	
0,0,	118,140,	118,140,	118,140,	
118,140,	118,140,	118,140,	118,140,	
118,140,	118,140,	118,140,	137,138,	
137,138,	137,138,	137,138,	137,138,	
137,138,	137,138,	137,138,	137,138,	
137,138,	139,140,	139,140,	139,140,	
139,140,	139,140,	139,140,	139,140,	
139,140,	139,140,	139,140,	153,154,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	153,154,	
153,155,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
153,154,	0,0,	0,0,	0,0,	
0,0,	153,154,	0,0,	0,0,	
0,0,	153,154,	0,0,	0,0,	
0,0,	0,0,	153,153,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	153,153,	
153,153,	153,153,	153,153,	153,153,	
0,0,	153,153,	153,153,	153,153,	
0,0,	0,0,	153,153,	0,0,	
153,153,	153,153,	154,154,	153,153,	
153,153,	0,0,	153,153,	153,153,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	153,153,	0,0,	154,154,	
154,154,	154,154,	154,154,	154,154,	
0,0,	154,154,	154,154,	154,154,	
0,0,	0,0,	154,154,	0,0,	
154,154,	154,154,	0,0,	154,154,	
154,154,	0,0,	154,154,	154,154,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	154,154,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-72,	yysvec+1,	0,	
yycrank+-111,	yysvec+1,	0,	
yycrank+-180,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+0,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+0,	0,		yyvstop+8,
yycrank+0,	0,		yyvstop+11,
yycrank+3,	0,		yyvstop+14,
yycrank+-242,	0,		yyvstop+17,
yycrank+-291,	0,		yyvstop+19,
yycrank+0,	0,		yyvstop+22,
yycrank+14,	0,		yyvstop+25,
yycrank+-340,	0,		yyvstop+28,
yycrank+0,	0,		yyvstop+30,
yycrank+0,	0,		yyvstop+33,
yycrank+15,	0,		yyvstop+36,
yycrank+10,	0,		yyvstop+39,
yycrank+0,	0,		yyvstop+42,
yycrank+75,	0,		yyvstop+46,
yycrank+126,	0,		yyvstop+49,
yycrank+4,	0,		yyvstop+52,
yycrank+388,	0,		yyvstop+55,
yycrank+0,	0,		yyvstop+58,
yycrank+0,	0,		yyvstop+61,
yycrank+17,	0,		yyvstop+64,
yycrank+26,	0,		yyvstop+67,
yycrank+27,	0,		yyvstop+70,
yycrank+429,	0,		yyvstop+73,
yycrank+0,	0,		yyvstop+76,
yycrank+0,	0,		yyvstop+79,
yycrank+28,	0,		yyvstop+82,
yycrank+6,	0,		yyvstop+85,
yycrank+0,	0,		yyvstop+87,
yycrank+30,	0,		yyvstop+90,
yycrank+0,	0,		yyvstop+93,
yycrank+29,	0,		yyvstop+96,
yycrank+89,	0,		yyvstop+99,
yycrank+0,	yysvec+39,	yyvstop+102,
yycrank+-551,	0,		yyvstop+105,
yycrank+19,	yysvec+30,	yyvstop+108,
yycrank+599,	0,		yyvstop+111,
yycrank+0,	0,		yyvstop+114,
yycrank+-2,	yysvec+11,	0,	
yycrank+0,	0,		yyvstop+116,
yycrank+-4,	yysvec+12,	yyvstop+118,
yycrank+32,	0,		yyvstop+120,
yycrank+0,	0,		yyvstop+122,
yycrank+-7,	yysvec+15,	0,	
yycrank+0,	0,		yyvstop+124,
yycrank+38,	0,		yyvstop+126,
yycrank+0,	0,		yyvstop+128,
yycrank+0,	0,		yyvstop+130,
yycrank+0,	0,		yyvstop+132,
yycrank+0,	0,		yyvstop+134,
yycrank+0,	0,		yyvstop+136,
yycrank+0,	0,		yyvstop+138,
yycrank+160,	yysvec+22,	yyvstop+140,
yycrank+0,	0,		yyvstop+142,
yycrank+635,	0,		yyvstop+144,
yycrank+0,	yysvec+24,	yyvstop+146,
yycrank+342,	0,		0,	
yycrank+419,	0,		0,	
yycrank+0,	0,		yyvstop+148,
yycrank+0,	0,		yyvstop+150,
yycrank+0,	0,		yyvstop+152,
yycrank+0,	0,		yyvstop+154,
yycrank+0,	0,		yyvstop+156,
yycrank+0,	yysvec+30,	yyvstop+158,
yycrank+0,	0,		yyvstop+160,
yycrank+0,	yysvec+34,	0,	
yycrank+0,	0,		yyvstop+162,
yycrank+0,	0,		yyvstop+164,
yycrank+39,	0,		yyvstop+166,
yycrank+0,	0,		yyvstop+168,
yycrank+0,	yysvec+39,	0,	
yycrank+25,	0,		0,	
yycrank+-8,	yysvec+41,	yyvstop+170,
yycrank+39,	yysvec+30,	yyvstop+172,
yycrank+50,	0,		0,	
yycrank+135,	0,		0,	
yycrank+78,	0,		0,	
yycrank+161,	0,		0,	
yycrank+179,	0,		0,	
yycrank+178,	0,		0,	
yycrank+79,	0,		0,	
yycrank+0,	0,		yyvstop+174,
yycrank+0,	0,		yyvstop+176,
yycrank+514,	0,		0,	
yycrank+552,	0,		0,	
yycrank+0,	0,		yyvstop+178,
yycrank+186,	yysvec+61,	yyvstop+180,
yycrank+0,	0,		yyvstop+183,
yycrank+398,	0,		0,	
yycrank+94,	yysvec+95,	yyvstop+185,
yycrank+524,	0,		0,	
yycrank+114,	yysvec+97,	yyvstop+187,
yycrank+0,	0,		yyvstop+189,
yycrank+183,	0,		0,	
yycrank+180,	yysvec+30,	yyvstop+191,
yycrank+196,	0,		0,	
yycrank+188,	0,		0,	
yycrank+185,	0,		0,	
yycrank+12,	0,		0,	
yycrank+55,	0,		0,	
yycrank+56,	0,		0,	
yycrank+65,	0,		0,	
yycrank+69,	0,		0,	
yycrank+76,	0,		0,	
yycrank+186,	0,		0,	
yycrank+93,	0,		0,	
yycrank+663,	0,		0,	
yycrank+211,	yysvec+113,	yyvstop+193,
yycrank+673,	0,		0,	
yycrank+224,	yysvec+115,	yyvstop+195,
yycrank+694,	0,		0,	
yycrank+709,	0,		0,	
yycrank+0,	0,		yyvstop+197,
yycrank+0,	0,		yyvstop+200,
yycrank+0,	0,		yyvstop+202,
yycrank+236,	0,		0,	
yycrank+234,	yysvec+30,	yyvstop+204,
yycrank+94,	0,		0,	
yycrank+254,	0,		0,	
yycrank+95,	0,		0,	
yycrank+0,	0,		yyvstop+206,
yycrank+0,	0,		yyvstop+208,
yycrank+0,	0,		yyvstop+210,
yycrank+0,	0,		yyvstop+212,
yycrank+0,	0,		yyvstop+214,
yycrank+0,	0,		yyvstop+216,
yycrank+96,	0,		0,	
yycrank+0,	0,		yyvstop+218,
yycrank+0,	0,		yyvstop+220,
yycrank+0,	0,		yyvstop+222,
yycrank+719,	0,		0,	
yycrank+260,	yysvec+137,	yyvstop+224,
yycrank+729,	0,		0,	
yycrank+263,	yysvec+139,	yyvstop+227,
yycrank+260,	0,		0,	
yycrank+278,	yysvec+30,	yyvstop+230,
yycrank+0,	0,		yyvstop+232,
yycrank+102,	0,		0,	
yycrank+0,	0,		yyvstop+234,
yycrank+0,	0,		yyvstop+236,
yycrank+0,	0,		yyvstop+238,
yycrank+0,	0,		yyvstop+241,
yycrank+280,	0,		0,	
yycrank+283,	yysvec+30,	yyvstop+244,
yycrank+0,	0,		yyvstop+246,
yycrank+302,	0,		0,	
yycrank+-786,	0,		yyvstop+248,
yycrank+-818,	yysvec+153,	0,	
yycrank+0,	0,		yyvstop+250,
0,	0,	0};
struct yywork *yytop = yycrank+913;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,011 ,012 ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
011 ,01  ,'"' ,01  ,01  ,01  ,01  ,047 ,
01  ,01  ,01  ,'+' ,01  ,'+' ,01  ,01  ,
'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,
'0' ,'0' ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,'A' ,'B' ,'C' ,'D' ,'E' ,'B' ,'G' ,
'H' ,'I' ,'B' ,'B' ,'L' ,'B' ,'N' ,'O' ,
'B' ,'Q' ,'R' ,'B' ,'T' ,'U' ,'B' ,'B' ,
'B' ,'B' ,'B' ,01  ,01  ,01  ,01  ,'_' ,
01  ,'A' ,'B' ,'C' ,'D' ,'E' ,'B' ,'G' ,
'H' ,'I' ,'B' ,'B' ,'L' ,'B' ,'N' ,'O' ,
'B' ,'Q' ,'R' ,'B' ,'T' ,'U' ,'B' ,'B' ,
'B' ,'B' ,'B' ,01  ,01  ,01  ,01  ,01  ,
0};
char yyextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	ATT:#ident	"libl:lib/ncform	1.3"	*/
	
#ident	"@(#)libl/lib:ncform	25.1"

int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
char yytext[YYLMAX];
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
yylook(){
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych, yyfirst;
	struct yywork *yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	yyfirst=1;
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank && !yyfirst){  /* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
			*yylastch++ = yych = input();
			yyfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (int)yyt > (int)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)yyt < (int)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
yyback(p, m)
	int *p;
{
if (p==0) return(0);
while (*p)
	{
	if (*p++ == m)
		return(1);
	}
return(0);
}
	/* the following are only used in the lex library */
yyinput(){
	return(input());
	}
yyoutput(c)
  int c; {
	output(c);
	}
yyunput(c)
   int c; {
	unput(c);
	}
