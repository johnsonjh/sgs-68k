#ident	"@(#)printname.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"
/* just print out the name change and type 
 * if there was some problem, print that too
 */
printname(name,type, size,vers)
register char * name;
register char type;
register long size;
register char vers;
{
/*       1         2         3         4         5         6         7        */
/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/
/*oldname			| C_STRTAG | newname			      */
/*oldname			| C_STRTAG | newname			      */

#ifdef DEBUG
	if(dflag) printf("printname\n");
#endif /* DEBUG */

	if (size > 8 ){
		printf(" %-*s ",maxname,name);
	}else{
		printf(" %-8s ",name);
		if(maxname-8>0)printf("%-*s",maxname-8," ");
	}
	/* if you are printing out the part that is the new name
	 * then see if there was any problem in changing the name
	 * and print that out too
	 * NEWSYM, means that it was found in a table of new style names
	 * NEWFILE, means that some symbol was seen that says this is
	 * definitely a new symbol
	 */
	if(vers == NEWNAME ){
		switch(conflict){
		case NEWTBL:printf("|NEWSYM |");break;
		case NEWFILE:printf("|NEWFILE|");break;
		case 0:	printf("|       |");break;
		default: printf("| 0x%3x |",conflict);break;
		}
		printtype(type);
		putchar('|');
		putchar('\n');
	}else putchar('|');
} /* end printnew */
