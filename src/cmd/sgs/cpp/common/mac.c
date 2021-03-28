#ident	"@(#)mac.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

#include "mac.h"

/** Global Definitions *****************************************/
char *macscan;

/** Static Declarations *****************************************/
static char *lowp;
static char *highp;
#define MAC_NEST 100	/* maximum number of nested macros we want to handle */
#define DEACT_OFF 1
typedef struct s_macdeact {
	char *name;
	char *low;
	char *high;
	char flag;
} t_macdeact;
static t_macdeact macdeact[MAC_NEST];
static t_macdeact *rescan[MAC_NEST];
static char *argend[MAC_NEST];
static int rlevel;		/* recursion level of macro arg pre-expansion */
static char * pushback();
static t_macdeact *mac_deact();
static char *mac_pretend();
/************************************************************/

char *
subst( p, sp )
register char *p;
struct symtab *sp;
{
	static char match[] = "%s: argument mismatch";
	register char *ca, *vp;
	int params;
	char *actual[MAXFRM]; /* actual[n] is text of nth actual */
	char acttxt[BUFSIZ]; /* space for actuals */
	static char *macstack[10];		/*bk*/
	int ex_blank, wspace, argstart;
	char *savep, *cp;
	int wasslow = isslo;			/*bk*/
	char expbuf[512], *expptr, *saveinp, *savelowp;
	t_macdeact *dptr;


	if ( 0 == ( vp = sp->value ) )
		return( p );

/**	if ( rix > MAC_NEST-1 ) {
		pperror( "Too many levels of macro expansion" );
		exit(1);
	} **/

	maclvl = 0;	/* level decreased */
	macstack[maclvl++] = sp->name;
	macforw = p;	/* new target for decrease in level */
	macdam = 0;
	macnam = sp->name;
#ifdef CXREF
	ref(macnam, lineno[ifno]);
#endif

	expptr = NULL;
	saveinp = NULL;
	if ( rlevel == 0 ) 						/* can't depend on flslvl */
		dump();

	if ( rlevel > 0 ) {						/* we're expanding a macro arg */ 
		if ( lowp == NULL ) {
			pperror( "subst: expecting non-null lowp" ); 
			exit(2);
		}
		if ( inp > lowp ) {					/* save before more pushback */
			saveinp = inp;					/* high end of buffered expansion */
			cp = lowp;
			savelowp = lowp;
			expptr = expbuf;
			do {
				*expptr++ = *cp++;
			} while( cp < inp );
			deact_save( lowp, saveinp, 0 ); 
		}
	} 

	if ( sp == ulnloc )
	{
		vp = acttxt;
		*vp++ = '\0';
		sprintf( vp, "%d", lineno[ifno] );
		while ( *vp++ )
			;
	}
	else if ( sp == uflloc )
	{
		vp = acttxt;
		*vp++ = '\0';
		sprintf( vp, "\"%s\"", fnames[ifno] );
		while ( *vp++ )
			;
	}
	if ( 0 != ( params = *--vp & 0xFF ) )	/*definition calls for params */
	{
		register char **pa;
		int dparams;		/* parameters in definition */

		ca = acttxt;
		pa = actual;
		if ( params == 0xFF )	/* #define foo() ... */
			params = 0;
		dparams = params;
		sloscan();
		++flslvl; /* no expansion during search for actuals */
/*		plvl = -1; */			/*bk*/
		plvl = 0;
		maclin = lineno[ifno];
		macfil = fnames[ifno];
		macscan = outp;			/* might be here by mistake */
		do {
			p = skipbl(p);
		} while ( *inp == '\n' );

		if ( rlevel > 0 ) {			/* expanding argument? */
			if ( argend[rlevel-1] ) {		/* curtail search for lparen */
				if (inp > argend[rlevel-1])
					goto pretend;
			} else {
				pperror( "subst expecting non-null argend" );
			}
		}

		if ( *inp == '(' )
		{
			macscan = NULL;
			for ( plvl = 1; plvl != 0; )
			{
				*ca++ = '\0';
				argstart = 1;
				ex_blank = 0;
				for ( ;; )
				{
					do {
						outp = inp = p;
						p = cotoken( p );
						wspace = ( (( toktyp + COFF )[*inp] == BLANK) ||
									 *inp == '\n' );
						if ( ! argstart )
							ex_blank |= wspace;
					} while ( wspace );

					if ( *inp == '(' )
						++plvl;
					if ( *inp == ')' && --plvl == 0 )
					{
						if ( ca > acttxt+1 )
							--params;
						break;
					}
					if ( plvl == 1 && *inp == ',' )
					{
						--params;
						while ( isspace(*p) )	/*bk*/
							++p;
						break;
					}

						/* bk: reduce arbitrary whitespace to 1 space */
					if ( ex_blank ) {
						*ca++ = ' ';
						ex_blank = 0;
					}
					argstart = 0;

					while ( inp < p )			/* collect actual arg token */
						*ca++ = *inp++;

					if ( ca > &acttxt[BUFSIZ] )
						pperror( "%s: actuals too long", sp->name );
				}
				if ( pa >= &actual[MAXFRM] )
					ppwarn( W_DFLT, match, sp->name );
				else
					*pa++ = ca;
			}
		}
		else {		/* get out: use of ftn-like macro symbol without
					 * lparen constitutes ordinary text... let's pretend
					 * we never came here. (bk: we used to complain)
					 */
pretend:
			ppwarn( W_DFLT, "ftn-like macro %s ignored: missing ()", sp->name );
			p = mac_pretend();		/* decrements flslvl */
			if (!wasslow)
				fasscan();
			return( NULL );
		}
		if ( maclin != lineno[ifno] )	/* embedded linefeeds in macro call */
		{
			int i, j = lineno[ifno];

			for ( i = sizeof( int ) / sizeof( char ); --i >= 0; )
			{
				if ( bob( p ) )
				{
					outp = inp = p;
					p = unfill( p );
				}
				*--p = j;
				++macdam;	/*bk*/
				j >>= 8;
			}
			if ( bob( p ) )
			{
				outp = inp = p;
				p = unfill( p );
			}
			*--p = warnc;
			++macdam;	/*bk*/
		}
		if ( params != 0 )
			ppwarn( W_DFLT, match, sp->name );
		while ( --params >= 0 )
			*pa++ = "" + 1;	/* null string for missing actuals */
		--flslvl;
	}		/* done collecting actual arguments */

	if ( rlevel == 0 )				/* remember starting point of pushback */
		if ( highp == NULL )
			highp = p;

	savep = p;							/* high end of expansion */
	p = pushback( p, vp, actual, sp );		/* substitute macro text */


								/* deactivate symbol over expansion range */
	dptr = mac_deact(sp->name,p,savep-1);

	save_rescan( dptr );

	if( expptr != NULL ) {
		while( --expptr >= expbuf ) {	/* restore saved expansion */
			if( bob(p) ) {
				outp = inp = p;
				p = unfill(p);
			} 
			*--p = *expptr;
		}
		deact_restore( savelowp, saveinp, savelowp - p );
	}

	lowp = p;							/* low extent of macro pushback */

	if (!wasslow)
		fasscan();

	outp = inp = p;
	return( p );
}

static
save_rescan( dptr )
t_macdeact *dptr;
{
	if ( rescan[rlevel] && rescan[rlevel]->name ) {
		if ( dptr->high != rescan[rlevel]->high )
			rescan[ rlevel ] = dptr;
	} else 
		rescan[ rlevel ] = dptr;
}

/* Note: we assume we're in sloscan mode */
static char *
pushback( p, vp, actual, sp )
register char *p;
register char *vp;
char *actual[]; /* actual[n] is text of nth actual */
struct symtab *sp;
{
	int stringize;/*bk*/
	register char *ca;
	char *largend;
	int escapeit;

	/* remember: separate tokens remain separate unless catenated with ## */
	/* Among other things, this requires that we skip TOKSEP during */
	/* macro substitution of pasted arguments */
	if ( bob( p ) )
	{
		outp = inp = p;
		p = unfill( p );
	}
	/* if ( mode != MODE_CUC ) */
		*--p = TOKSEP;

/*	if ( *p != ' ' ) {
		if ( bob( p ) )
		{
			outp = inp = p;
			p = unfill( p );
		}
		*--p = ' ';
	} */

	for ( ;; )	/* push definition onto front of input stack */
	{
		while ( !iswarn( *--vp ) )
		{
			if ( bob( p ) )
			{
				outp = inp = p;
				p = unfill( p );
			}
			*--p = *vp;
		}
		if ( *vp == warnc )	/* insert actual param */
		{
			largend = argend[rlevel] = p;	/* point at end of argument text */

			if ( stringize = (*--vp & (STRINGIZE|TOKENPASTE)) ) { 		/*bk*/
				*vp &= ~(STRINGIZE|TOKENPASTE);
				if (stringize & STRINGIZE)
					*--p = '\"';		/* insert terminating quote */
			}

			ca = actual[ *vp - 1];
			
			escapeit = isquo(*(ca-1)); 	/* char constant or string literal? */
			while ( *--ca )
			{
				if ( *ca == TOKSEP && stringize & TOKENPASTE )
					continue;
				if ( bob( p ) )
				{
					outp = inp = p;
					p = unfill( p );
				}
				*--p = *ca;
				if ( (stringize & STRINGIZE) && escapeit &&
						isquo(*ca) ) {			/* escape quotes */
					*--p = '\\';
				}
			}
			if ( stringize ) {		/* no arg expansion for these exceptions */
				*vp |= stringize;	/* reset flag */
				if (stringize & STRINGIZE)
					*--p = '\"';	/* insert beginning quote */
			} else {
				/* would like to use flslvl to suspend dumps, */
				/* but that would prevent macro expansion */
				++rlevel;			/* prevent dumps */
				lowp = p;			/* remember position to resume */
				do {				/* recursively expand argument */
					p = cotoken( p );
					if ( outp > inp )
						outp = inp;
				} while ( p < largend );
				--rlevel;
				p = lowp;			/* resume pushback */
			}
		}
		else
			break;
	}

	if ( bob( p ) )
	{
		outp = inp = p;
		p = unfill( p );
	}

/*	if ( mode != MODE_CUC ) */ 
		*--p = TOKSEP;
	return( p );
}

/* has outp progressed beyond point of a macro's prior expansion? */
react_chk(inp)
char *inp;
{
	int i;
	struct s_macdeact *deact;

	deact = &macdeact[MAC_NEST-1];
	for( i = MAC_NEST-1; i >= 0; --i, --deact ) {
		if ( deact->name == NULL )
			continue;
		if ( inp > deact->high ) {
			int j;
#ifndef NODBG
			fprintf( stderr, "Reactivating %s\n",deact->name );
			fprintf( stderr, "\t0x%x <<- (0x%x) ->> 0x%x\n", 
							deact->low, inp, deact->high );
#endif
			deact->name = NULL;
			for (j=0; j<MAC_NEST; j++ ) {		/* clear rescan[] pointer */
				if ( rescan[j] == deact ) {
					rescan[j] = NULL;
					break;
				}
			}
/*			rix--; */
		}
	}
}


/* Return value 1 if given symbol is temporarily suspended */
/* at given point in input.  Note that a single symbol can have */
/* more than one deactive entry. */
int 
is_deact( sname )
char *sname;					/* ptr to macro name & current input position */
{
int i;
struct s_macdeact *deact;

deact = &macdeact[MAC_NEST-1];
for( i = MAC_NEST-1; i >= 0; --i, --deact ) {
	if ( deact->name == NULL )
		continue;
	if ( ( deact->flag != DEACT_OFF ) &&
			( strcmp( deact->name, sname ) == 0 ) ) {
			if ( ( sname >= deact->low ) &&
				( sname <= deact->high ) ) {
				ppwarn( W_DFLT, "Ignoring recursive macro %s", deact->name );
#ifndef NODBG
				fprintf( stderr, "Ignoring %s\n",deact->name );
				fprintf( stderr, "\t0x%x <<- 0x%x ->> 0x%x (0x%x)\n", 
								deact->low, sname, deact->high, outp );
#endif
				return( 1 );	/* symbol currently deactivated */
			}
		}
	}
	return( 0 );			/* symbol is active */
}


/* Create entry to effectively deactivate a symbol within */
/* a given input range. */
static t_macdeact *
mac_deact( mac, low, high )
char *mac;
char *low, *high;
{
	int i;
	t_macdeact *deact;

	deact = &macdeact[MAC_NEST-1];
	for( i = MAC_NEST-1; i >= 0; --i, --deact ) {
		if ( deact->name == NULL ) {
			deact->name = mac;	/* suspend macro definition */
			deact->low = low;
			deact->high = high;
			deact->flag = 0;
#ifndef NODBG
			fprintf( stderr, "Deactivating %s\n",deact->name );
			fprintf( stderr, "\t0x%x <<- 0x%x ->> 0x%x\n", 
								deact->low, mac, deact->high );
#endif
			return( deact );
		}
	}
	pperror( "Simplify macro expression" );
#ifndef NODBG
	fprintf( stderr, "inp = 0x%x, outp = 0x%x (0x%x, 0x%x)\n", inp,outp, low, high );
	deact = &macdeact[MAC_NEST-1];
	for( i = MAC_NEST-1; i >= 0; --i, --deact )
		fprintf( stderr, "\t0x%x <<- %s ->> 0x%x\n", 
								deact->low, deact->name, deact->high );
#endif
	exit(2);
}

void 
deact_save( low, high, unfill )
char *low, *high;
int unfill;			/* true when called from unfill() */
{
	int i;
	t_macdeact *deact;

	deact = &macdeact[MAC_NEST-1];
	for( i = MAC_NEST-1; i >= 0; --i, --deact ) {
		if ( deact->name == NULL )
			continue;
		if ( ((int) deact->low >= (int) low) &&
			((int) deact->high <= (int) high) ) {
			deact->flag = DEACT_OFF;
			if ( unfill ) {
				extern char *pbuf;
				deact->high -= (deact->low - pbuf);
				deact->low = pbuf;
			}
#ifndef NODBG
			fprintf( stderr, "Saving (DEACT_OFF) %s\n", deact->name );
			fprintf( stderr, "\t0x%x (0x%x) : 0x%x (0x%x)\n", 
							low, deact->low, high, deact->high );
#endif
		}
	}
}

void
deact_restore( low, high, diff )
char *low, *high;
int diff;
{
	int i;
	struct s_macdeact *deact;

	diff -= 2;

	if ( low == NULL ) {
		pperror( "Null arg to deact_restore" );
		exit(2);
	}

	deact = &macdeact[MAC_NEST-1];
	for( i = MAC_NEST-1; i >= 0; --i, --deact ) {
		if ( deact->name == NULL )
			continue;
		if ( ((int) deact->low >= (int) low) &&
			((int) deact->high <= (int) high) ) {

/*					if ( deact->flag != DEACT_OFF ) {
				pperror( "deact_fix trouble (2)" );
				exit(1);
			} */

			deact->flag = 0;
			deact->low -= diff;
			deact->high -= diff;
#ifndef NODBG
			fprintf( stderr, "Restoring %s\n", deact->name );
			fprintf( stderr, "\t0x%x (0x%x) : 0x%x (0x%x)\n", 
						low, deact->low, high, deact->high );
#endif
		}
	}
}


static char *
mac_pretend()
{
	plvl = 0;
	--flslvl;
	if ( (flslvl == 0) && (rlevel == 0) ) {
		outp = macscan;
		while ( outp < inp ) {
			putc( *outp, fout );
			++outp;
		}
	}
	macscan = NULL;
	return( inp );
}

char *
dodef( p )		/* process '#define' */
	char *p;
{
	register char *pin, *psav, *cf;
	char **pf, **qf;
	int b, c, params;
	int ex_blank;	/* used to ignore extra blanks in token-string */
	int sav_passcom = passcom;	/* saved passcom, used to reset it */
	struct symtab *np;
	char *oldval, *oldsavch;
	char *formal[MAXFRM]; /* formal[n] is name of nth formal */
	char formtxt[BUFSIZ]; /* space for formal names */
	int stringize, lastpaste, tlin; /*bk*/
	char *lastformal = 0;

	if ( savch > sbf + SBSIZE - BUFSIZ )
	{
		pperror( "too much defining" );
		return( p );
	}
	oldsavch = savch; /* to reclaim space if redefinition */
	++flslvl;	/* prevent macro expansion during 'define' */
	p = skipbl( p );
	pin = inp;
	if ( err_flag ) {	/*bk*/
		np = errloc;
		params = 0;
		oldval = NULL;
		p = inp;
		goto collect;
	} else {
		if ( ( toktyp + COFF )[*pin] != IDENT )
		{
			if (*pin == '\n') --lineno[ifno];
			ppwarn( W_DFLT, "illegal/missing macro name" );
			if (*pin == '\n') ++lineno[ifno];
			while ( *inp != '\n' )
				p = skipbl( p );
			--flslvl;		/* restore expansion */
			return( p );
		}
	}
	np = slookup( pin, p, 1 );
	if ( oldval = np->value )	/* was previously defined */
		savch = oldsavch;
#ifdef CXREF
 	def(np->name, lineno[ifno]);
#endif
	b = 1;
	cf = pin;
	while ( cf < p )		/* update macbit */
	{
		c = *cf++;
		xmac1( c, b, |= );
		b = ( b + b ) & 0xFF;
		if ( cf != p )
			xmac2( c, *cf, -1 + ( cf - pin ), |= );
		else
			xmac2( c, 0, -1 + ( cf - pin ), |= );
	}
	params = 0;
	outp = inp = p;
	p = cotoken( p );
	pin = inp;
	if ( *pin == '(' )	/* with parameters; identify the formals */
	{
#ifdef CXREF
 		newf(np->name, lineno[ifno]);
#endif
		cf = formtxt;
		pf = formal;
		for ( ;; )
		{
			p = skipbl( p );
			pin = inp;
			/* roger: added this 'if' to handle backslashes in
			between the formals of the macro */
			if ( *pin == '\\' && pin[1] == '\n' )	/* ignore escaped lf */
			{
				if ( !ex_blank )	/* replace it with a blank */
				{
					*psav++ = ' ';
					ex_blank = 1;
				}
				putc( '\n', fout );
				continue;
			}
			if ( *pin == '\n' )
			{
				--lineno[ifno];
				--p;
				pperror( "macro %s: missing )", np->name );
				break;
			}
			if ( *pin == ')' )
				break;
			if ( *pin == ',' )
				continue;
			if ( ( toktyp + COFF )[*pin] != IDENT )
			{
				c = *p;
				*p = '\0';
				pperror( "bad formal: %s", pin );
				*p = c;
			}
			else if ( pf >= &formal[MAXFRM] )
			{
				c = *p;
				*p = '\0';
				pperror( "too many formals: %s", pin );
				*p = c;
			}
			else
			{
				int i;

				*pf++ = cf;
				while ( pin < p )
					*cf++ = *pin++;
				*cf++ = '\0';
				for (i = params; i>0; i-- ) {	/*bk: ANSI 3.8.3 ConP5S1 */
					if ( !strcmp( formal[params], formal[i-1] ) ) {
						ppwarn( W_DFLT, "non-unique parameter identifiers" );
						break;
					}
				}
				++params;
#ifdef CXREF
 				def(*(pf-1), lineno[ifno]);
#endif
			}
		}
		if ( params == 0 )	/* #define foo() ... */
			--params;
	}
	else if ( *pin == '\n' )
	{
		--lineno[ifno];
		--p;
	}
	else
	    p = inp;			/* back up to scan non-blank next token */
	/*
	* remember beginning of macro body, so that we can
	* warn if a redefinition is different from old value.
	*/
collect:		/*bk*/
	oldsavch = psav = savch;
	passcom = 1;	/* make cotoken() return comments as tokens */
	ex_blank = 1;	/* must have some delimiter - might as well be blank */
	stringize = 0;	/*bk*/
	for ( ;; )	/* accumulate definition until linefeed */
	{
		outp = inp = p;
		p = cotoken( p );
		pin = inp;
		if ( *pin == '\\' && pin[1] == '\n' )	/* ignore escaped lf */
		{
			if ( !ex_blank )	/* replace it with a blank */
			{
				*psav++ = ' ';
				ex_blank = 1;
			}
			putc( '\n', fout );
			continue;
		}
		if ( *pin == '\n' )
			break;

		if ( *pin == SALT ) {	/*bk*/
			if ((pin[-1] == SALT) || (pin[1] == SALT)) { 	/* paste-op? */
				if (pin[1] == SALT) {		
					if (psav == savch) 	/* first token in replacement-list? */
						pperror( "illegal use of paste operator" );
					else {
						if ( *--psav != ' ' ) {	/* catenate tokens*/
							++psav;
							ex_blank = 1;		/* clear up to 2nd operand */
						}
					}
				} else 
					stringize |= TOKENPASTE;
			} else {
								/* e.g. "# #": 1st # wasted, try 2nd */
				if (stringize & STRINGIZE)
					pperror( "missing parameter for string-ize operator" );
				else
					stringize |= STRINGIZE;
			}
			continue;
		}

		if ( ( toktyp + COFF )[*pin] == BLANK )	/* skip extra blanks */
		{
			if ( ex_blank )
				continue;
			*pin = ' ';	/* force it to be a "real" blank */
			ex_blank = 1;
		}
		else if ( *pin == '/' && pin[1] == '*' )	/* skip comment */
		{					/* except for \n's */
			if (  mode != MODE_CUC ) {
				if ( ex_blank == 0 ) {	
					ex_blank = 1;
					*psav++ = ' ';
				}
			} 
			while ( pin < p )
				if ( *pin++ == '\n' )
					putc( '\n', fout );
			continue;
		}
		else
			ex_blank = 0;

		if ( lastpaste = stringize & TOKENPASTE )
/*			stringize &= TOKENPASTE*/;
		if ( params )	/* mark the appearance of formals in the definiton */
		{
			if ( ( toktyp + COFF )[*pin] == IDENT )
			{
				for ( qf = pf; --qf >= formal; )
				{
					if ( equfrm( *qf, pin, p ) )
					{
#ifdef CXREF
#ifndef NO_MACRO_FORMAL
 						ref(*qf, lineno[ifno]);
#endif
#endif
						if ( lastformal )
							*lastformal |= lastpaste;
						lastformal = psav;

						*psav++ = (qf - formal + 1) | stringize;
						stringize = 0;
						*psav++ = WARN;
						pin = p;
						break;
					}
				}
			}
							/* inside quotation marks, too */
							/* bk: see ANSI Rationale, 3.8.3.2 */
			else if (( *pin == '\'' ) ||
					( *pin == '"' )
# if gcos
					|| ( *pin == '`' )
# endif
						)
			{
				char quoc = *pin;

				for ( *psav++ = *pin++; pin < p && *pin != quoc; )
				{
					while ( pin < p && !isid( *pin ) )
					{
						if ( *pin == '\n'
							&& pin[-1] == '\\' )
						{
							putc( '\n', fout );
							psav--;	/* no \ */
							pin++;	/* no \n */
						}
						else
							*psav++ = *pin++;
					}
					cf = pin;
					while ( cf < p && isid( *cf ) )
						++cf;
					for ( qf = pf; --qf >= formal; )
					{
						if ( equfrm( *qf, pin, cf ) )
						{
		/* Yes, the hell with the RATIONALE: let 'em stringize the old */
		/* way if they want to! */
							if ( quoc == '\"' && mode != MODE_CUC )
								ppwarn( W_DFLT, 
					"Argument implicitly stringized: please use # operator" );
							*psav++ = (qf - formal + 1) | stringize;
							stringize = 0;
							*psav++ = WARN;
							pin = cf;
							break;
						}
					}
					while ( pin < cf )
						*psav++ = *pin++;
				}
			}
		}
		if (!ex_blank) {
			if (stringize & STRINGIZE) {	/* '#' gets dropped: bk*/
				pperror( "missing parameter for string-ize operator" );
				stringize &= ~STRINGIZE; /*reset */
			} else {
				if (*pin != SALT)
					stringize &= ~TOKENPASTE;		/* make sure */
			}
		}
		if ( ( toktyp + COFF )[*pin] == IDENT ) {		/*bk*/
			if ( equfrm( np->name, pin, p ) ) {
				if (( *p == '(' ) && ( psav != savch )) {
					ppwarn( W_LINT, "Macro recursion (%s) rebuffed", np->name );
					continue;
				}
			}
		} 
		while ( pin < p )
			if ( *pin == '\n' && pin[-1] == '\\' )
			{
				putc( '\n', fout );
				psav--;	/* no \ */
				pin++;	/* no \n */
			}
			else
				*psav++ = *pin++;
	}
	if (stringize & STRINGIZE) { 	/* '#' last before newline: dropped - bk*/
		tlin = lineno[ifno]--;
		pperror( "missing parameter for string-ize operator" );
		lineno[ifno] = tlin;
	}

	passcom = sav_passcom;	/* restore to "real" value */
	if ( psav[-1] == ' ' )	/* if token-string ended with a blank */
		psav--;		/* then it is unnecessary - throw away */

	if ( stringize & TOKENPASTE ) {	/* trailing paste-op? bk */
		tlin = lineno[ifno]--;
		pperror( "illegal use of paste operator" );
		lineno[ifno] = tlin;
	}

	*psav++ = params;
	*psav++ = '\0';
	if ( ( cf = oldval ) != NULL )		/* redefinition */
	{
		int redef;

		--cf;	/* skip no. of params, which may be zero */
		while ( *--cf )		/* go back to the beginning */
			;
		redef = strcmp( ++cf, oldsavch ); /* redefinition different from old */

		if( !redef ) {	/* bk: macro body the same, so check spelling of parms*/
			int i;
			for( cf = oldval+1, i = params;  i>0;  i-- ) {
				if ( strcmp( cf, formal[i-1] ) ) {
					redef = 1;
					break;
				}
				while ( *cf++ )
					;
			}
		}

		if( redef )
		{
			--lineno[ifno];
			if (!err_flag)
				ppwarn( W_DFLT, "%s redefined", np->name );
			++lineno[ifno];
			np->value = psav - 1;
		}
		else
			psav = oldsavch; /* identical redef.; reclaim space */
	}
	else
		np->value = psav - 1;

	if ( psav != oldsavch ) {				/* new definition? */
		for ( ; params>0; params-- ) {		/* bk: save formal param id's */
			strcpy( psav, formal[params-1] );	/* saved in reverse order */
			while( *psav++ )
				;
		}
	}

	--flslvl;
	inp = pin;
	savch = psav;
	return( p );
}

mac_flush()
{
	memset( macdeact, 0, sizeof(macdeact) );
}
