/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) nfort.c: version 25.1 created on 12/2/91 at 17:38:48	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)nfort.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	nfort.c	7.2		*/

#ident	"@(#)f77/f77/mach:nfort.c	25.1"

# define FORT
#if TARGET==M68K
# include "mfile2.h"
#else
# include "mfile2"
#endif
#ifdef FONEPASS
# include "fort.h"
#endif

/*	new opcode definitions */

# define FORTOPS 200
# define FTEXT 200
# define FEXPR 201
# define FSWITCH 202
# define FLBRAC 203
# define FRBRAC 204
# define FEOF 205
# define FARIF 206
# define LABEL 207

#if TARGET!=M68K
extern unsigned int offsz;
#endif
gencode(np)
register NODE *np;
{

	switch (np->in.op)
	{  /* switch on opcode */

	case 0:
		fprintf( stderr, "null opcode ignored\n" );
		break;
	case FRBRAC:
#if TARGET!=M68K
  		SETOFF(maxoff, ALSTACK);
		eobl2();
#endif
		break;

	case REG:
#ifdef FONEPASS
		rbusy( np->tn.rval, np->in.type );
#endif
	case ICON:
	case NAME:
#ifndef SGS2
  	case OREG:
#endif
	case FEXPR:
	bump:
#if TARGET==M68K
		ecode( np );
#else
  		tmpoff = baseoff;
  		nrecur = 0;
  		if (edebug)
  			fwalk(np, eprint, 0);
  		delay(np);
#endif
#ifdef FONEPASS
		reclaim(np, RNULL, 0);
		allchk();
#else
		tfree(np);
		tcheck();
#endif
		return;

#ifdef FONEPASS
	case LABEL:
		if (np->in.type)
			tlabel();
		else
			putlabel( np->tn.rval );
		break;

	case GOTO:
		if( np->in.type )  /* unconditional branch */
		{
			cbgen( 0, (int) np->tn.rval, 'I' );
			break;
		}
#endif
	default:
		switch (optype(np->in.op))
		{

		case BITYPE:
			goto bump;

		case UTYPE:
			goto bump;

		case LTYPE:
			uerror("illegal leaf node: %d", np->in.op);
			exit(1);
		}
	}
	np->in.op = STRING;	/* fake out tfree since it doesn't */
				/* understand FORTOPS, call it a   */
				/* random LTYPE guy...all NODEs    */
				/* that get here are leaves	   */
	tfree(np);
	tcheck();
}
