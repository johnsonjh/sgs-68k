#ident	"@(#)symbols2.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.2	89/06/29 14:57:36 hossein
	increased NHASH from 4001 to 5001 to allow for more symbols in the user program.

    1.1	89/06/08 00:40:10 root
	date and time created 89/06/08 00:40:10 by root

*/
/*	OLD MOT:symbols2.h	7.1		*/
/*	static	char	sccsid[] = "OLD MOT: symbols2.h: 2.1 6/4/83";			*/



/*
 *	NINSTRS	= number of legal assembler instructions (from ops.out)
 *	NHASH	= the size of the hash symbol table
 *	NSYMS	= the number of symbols allowed in a user program
 */

#ifdef M68881
#define N881 215	/* total number of M68881 instructions */
#else
#define N881 0
#endif

#define NINSTRS 250+N881	/* M68020 supports M68881 */

#define NHASH	8001 /* hoss#27 */

#define NSYMS	(NHASH - NINSTRS - 1)
