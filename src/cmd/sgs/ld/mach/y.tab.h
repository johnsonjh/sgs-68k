
typedef union  {
	int ivalue;	/* yylval values */
	char *sptr;
	long *lptr;

	ACTITEM *aitem;	/* nonterminal values */
	ENODE	*enode;
	} YYSTYPE;
extern YYSTYPE yylval;
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
