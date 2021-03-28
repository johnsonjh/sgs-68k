#ident	"@(#)inline1.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*       @(#)pcc2:common/inline1.c       3.0      */

static char SCCSID[] = "@(#) inline1.c: 3.0  86/03/30";
/* inline1.c -- produce in-line code for ASM psuedo functions */

#include "mfile1.h"

#ifdef IN_LINE

/* An implementation of inline asm expansion with arguments.
/* 'asm' pseudo-function definitions, whose bodies contain code 
/* to be expanded into the assembly code output depending on the
/* storage locations of their arguments, are recognized by the
/* compiler front end and stored away.
/* 
/* This version, implemented for PCC2, does *NOT* have the alternate option
/* of inserting a macro line into the assembly code output, as does the
/* original version implemented in PCC1.
/*
/* The rest of the PCC2 compiler has to be compiled with the IN_LINE
/* symbol defined in order to turn on the asm psuedo-function expansion
/* feature.
 */

#ifdef TWOPASS
FILE *inlfp = 0;         /* file to store asm 'function' bodies */
int asmdebug;  /* set by env ASMDEBUG=YES */
/*
/* inlargs[] collects the asm function argument names from the formal
/* parameter list.  The names will be written into the  asm expansion
/* storage file by squirrelmacro().  inlargs[] will also be used
/* to read the formal parameter names back in when an asm call is expanded.
*/
char inlargs[ BUFSIZ ] = "" ;
int ninlargs = 0;
#else
	/* these are defined in inline2.c for 1 pass version */
extern FILE *inlfp;     /* file to store asm 'function' bodies */
extern int asmdebug;    /* set by env AMDEBUG=YES */
/*
/* inlargs[] collects the asm function argument names from the formal
/* parameter list.  The names will be written into the  asm expansion
/* storage file by squirrelmacro().  inlargs[] will also be used
/* to read the formal parameter names back in when an asm call is expanded.
*/
extern char inlargs[];
extern int ninlargs;
#endif

squirrelmacro(name) 	/* called from grammar to put it away */
char *name;
{
	int c;

	/* use external form of name, because that's what we'll see in
	** pass2 when we expand the macro
	*/
	fprintf(inlfp, "$%s\n", exname(name));
#ifndef NODBG
	if (asmdebug)
	{
		printf( "\n++++++ asm squirrel file written to:\n");
		printf("$%s\n", name);
	}
#endif

	/* write out number of formal arguments and
	/* formal argument names, if any, collected in inlargs[] by 
	/* ftnarg().  line has formal param names delimited 
        /* by '#'.  
        */

	if (inlfp==NULL)
	{
		cerror("Missing temporary file for in-line processing\n");
		/*NOTREACHED*/
	}
	fprintf(inlfp,"#%d#%s\n",ninlargs,inlargs);
#ifndef NODBG
	if (asmdebug)
		printf("#%d#%s\n",ninlargs,inlargs);
#endif

	while ((c = getchar()) != '}')
	{
		/* hoss#13 */
		/* read intervening fields surrounded by {} */
		/* for instructions like "bfffo <ea>{....},%reg" */

		if (c == '{') 
		{
			putc(c, inlfp);
			while (( c = getchar()) != '}')
				putc(c, inlfp);
		}
		if (c == EOF)
		{
			cerror("EOF in asm pseudo-function definition");
			/*NOTREACHED*/
		}
		putc(c, inlfp);
#ifndef NODBG
	if (asmdebug)
		putchar(c);
#endif
		if (c == '\n')
		{
			++lineno;
			if ((c = getchar()) == '%')
			{
				fprintf(inlfp, "%% " );
#ifndef NODBG
				if (asmdebug)
					printf("%% " );
#endif
			}
			else
				ungetc(c, stdin);
		}
	}
	/* throw closing '%' into file to as endmarker */
	fputs("%\n", inlfp);
#ifndef NODBG
	if (asmdebug)
	{
		puts("%\n");
		printf("++++++\n\n");
	}
#endif
	/* make parser see "{}" to belive it's a function */
	ungetc('}', stdin);	
	/* clear argument list and count */
	inlargs[0] = '\0';
	ninlargs = 0;
}
#endif
