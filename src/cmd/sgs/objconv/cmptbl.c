#ident	"@(#)cmptbl.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"

/* try to find the string name in the table pointed to by base */
/* return the index into the table, if a match is found */
/* else return 0 */
long
cmptbl(name,base)
register char * name;
register char *base[];
{
	register long index = 1;
	register char **ptr = base;
#ifdef DEBUG
	if(dflag){
		printf("cmptbl");
		if(ptr == newtbl)
			printf(" NEWTBL\n");
		else if(ptr == oldtbl)
			printf(" OLDTBL\n");
		else if(ptr == dirtbl)
			printf(" DIRTBL\n");
	}
#endif /* DEBUG */
	while (*base != '\0'){
		if(strcmp(name,*base))index++,*base++;
		else return(index);
		}

#ifdef DEBUG
	if(dflag) printf("return 0 from cmptbl %x\n",index);
#endif /* DEBUG */
	return(0);
}/* end cmptbl */
