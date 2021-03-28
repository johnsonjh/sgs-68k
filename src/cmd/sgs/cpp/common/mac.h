#ident	"@(#)mac.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/* mac.h: information common to cpp.c and mac.c */
#include <stdio.h>
#include <ctype.h>
#include "sgs.h"			/* for sgs-global warning info */
#include "protos.h"		/* function prototypes */

# define DROP '\376' /* special character not legal ASCII or EBCDIC */
# define TOKSEP '\375' /* bk: special character ???? */
# define WARN DROP
#define SALT	'#'
#define STRINGIZE 0x80	/*bk*/
#define TOKENPASTE 0x40	/*bk*/

#define eob(a)		( ( a ) >= pend )
#define bob(a)		( pbeg >= ( a ) )

#undef BUFSIZ
#define BUFSIZ 2048

#ifdef pdp11
#	define SBSIZE 24000
#else
/* The idea here is to define the buffer to have a certain size
** (the literal number) plus enough room for slop and taking side buffers
** out to hold processing interrupted by #include's.  The cost of side
** buffers is particularly noticable on an Amdahl, where BUFSIZ is 4096.
*/
#	define SBSIZE (200000 + (MAXFRE+1)*BUFSIZ) /* ken#0 */
	/* hoss#16 */
	/* increased 70000 to 120000 to compile ORACLE version #6 */
	/* hoss#24 */
	/* increased yet again from 120000 to 150000 */
	/* bk */
	/* increased yet again from 150000 to 200000 */
#endif

#if pdp11 | vax | 1  /* sw0 */
#	define COFF 128
#else
#	define COFF 0
#endif


# define MAXFRE 14	/* max buffers of macro pushback */
# define MAXFRM 31	/* max number of formals/actuals to a macro */
#define isslo		( ptrtab == ( slotab + COFF ) )
#define isid(a)		( ( fastab + COFF )[a] & IB )
#define isquo(a)	( ( fastab + COFF )[a] & QB )
#define iswarn(a)	( ( fastab + COFF )[a] & WB )
#define isspc(a)	( ptrtab[a] & SB )
#define isnum(a)	( ( fastab + COFF )[a] & NB )
#define iscom(a)	( ( fastab + COFF )[a] & CB )
#define fasscan()	ptrtab = fastab + COFF
#define sloscan()	ptrtab = slotab + COFF
#define IB 1
#define SB 2
#define NB 4
#define CB 8
#define QB 16
#define WB 32
#define BLANK 1
#define IDENT 2
#define NUMBR 3


# if gcos
#	define ALFSIZ 512	/* alphabet size */
# else
#	define ALFSIZ 256	/* alphabet size */
# endif


struct symtab
{
	char	*name;
	char	*value;
} *lookup(), *slookup();

#define BLANK 1
#define IDENT 2


extern char	sbf[ ];
extern char macbit[ ALFSIZ + 11 ];
extern char	*savch;
extern char toktyp[ ];
extern char fastab[ ];
extern char slotab[ ];
extern char *ptrtab;
extern int maclin;	/* line number of macro call requiring actuals */
extern char *macfil;	/* file name of macro call requiring actuals */
extern char *macnam;	/* name of macro requiring actuals */
extern int maclvl;	/* # calls since last decrease in nesting level */
extern int macdam;	/* offset to macforw due to buffer shifting */
extern char *macforw;	/* pointer which must be exceeded to */
						/* decrease nesting level */


extern int plvl;	/* parenthesis level during scan for macro actuals */
extern char *pbeg;
extern int	lineno[ ];
extern char	*fnames[ ];
extern int	ifno;
extern	FILE	*fout;

extern char *outp, *inp;
extern char *newp;
extern char warnc;

extern	struct symtab *ulnloc;
extern	struct symtab *uflloc;
extern struct symtab *errloc; /*bk*/
extern 	int err_flag;			/*bk*/
extern	int	flslvl;
extern int	passcom;	/* don't delete comments */
extern char *macscan;
extern int mode;		/* compiler mode */

extern char * skipbl();		/* get next non-blank token */
extern char * unfill();
extern char * cotoken();
extern void deact_restore();
extern void deact_save();
extern char * skipbl();		/* get next non-blank token */
extern char * unfill();
extern char * cotoken();
extern char *macflush();
extern char *macflush();
extern char *dodef();
extern char *subst();


/* a superimposed code is used to reduce the number of calls to the
/* symbol table lookup routine.  (if the kth character of an identifier
/* is 'a' and there are no macro names whose kth character is 'a'
/* then the identifier cannot be a macro name, hence there is no need
/* to look in the symbol table.)  'scw1' enables the test based on
/* single characters and their position in the identifier.  'scw2'
/* enables the test based on adjacent pairs of characters and their
/* position in the identifier.  scw1 typically costs 1 indexed fetch,
/* an AND, and a jump per character of identifier, until the identifier
/* is known as a non-macro name or until the end of the identifier.
/* scw1 is inexpensive.  scw2 typically costs 4 indexed fetches,
/* an add, an AND, and a jump per character of identifier, but it is also
/* slightly more effective at reducing symbol table searches.
/* scw2 usually costs too much because the symbol table search is
/* usually short; but if symbol table search should become expensive,
/* the code is here.
/* using both scw1 and scw2 is of dubious value.
*/
#define scw1 1
#define scw2 0

#if scw1
#	define tmac1( c, bit )		if ( !xmac1( c, bit, & ) ) \
						goto nomac
#	define xmac1( c, bit, op )	( ( macbit + COFF )[c] op ( bit ) )
#else
#	define tmac1( c, bit )
#	define xmac1( c, bit, op )
#endif

#if scw2
#	define tmac2( c0, c1, cpos )	if ( !xmac2( c0, c1, cpos, & ) ) \
						goto nomac
#	define xmac2( c0, c1, cpos, op ) \
		( ( macbit + COFF )[ ( t21 + COFF )[c0] + \
		( t22 + COFF )[c1]] op ( t23 + COFF + cpos )[c0] )
#else
#	define tmac2( c0, c1, cpos )
#	define xmac2( c0, c1, cpos, op )
#endif
