
typedef union  {				/*bk*/
	struct {
		long val;
		int unsign;
	} numb;
	int op;
} YYSTYPE;
extern YYSTYPE yylval;
# define number 257
# define stop 258
# define DEFINED 259
# define EQ 260
# define NE 261
# define LE 262
# define GE 263
# define LS 264
# define RS 265
# define ANDAND 266
# define OROR 267
# define UMINUS 268
