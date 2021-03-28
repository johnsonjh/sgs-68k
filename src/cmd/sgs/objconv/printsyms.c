#ident	"@(#)printsyms.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"
/* for debugging, print out the symbol info */
printsyms()
{
#ifdef DEBUG
	if(dflag) printf("printsyms\n");
#endif /* DEBUG */
	runner = head;
	do{
		printf("\nSymbol Number [ %d ]\n",runner->entrynum);
		if(!ZEROES(runner)){
			printf("\tOFFST[ %x ]\n",OFFSET(runner));
			printf("\tFLEX [ %s ]\n",runner->sname);
		}else{
			printf("\n\t     [ %s ]\n",NAME(runner));
		}
		printf("	AUX  [ %d ]\n",(int)NUMAUX(runner));

		printtype(SCLASS(runner));putchar('\n');
	runner = runner->next;
	}while( runner != head );
}/* end printsyms */
