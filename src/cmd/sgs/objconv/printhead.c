#ident	"@(#)printhead.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"
/* print out the titles for printname, if this is verbose mode */
printheader(){
register long i;
putchar('\n');
for(i=0;i<(maxname*2)+23;i++)putchar('=');
putchar('\n');
printf("Symbols from  %s :\n\n",infile);
printf(" %-*s ",maxname,"OLD NAME");
printf("| %-*s ",maxname,"NEW NAME");
printf("| %3s   |","ERR");
printf(" %8s |\n","TYPE");
/*
for(i=0;i<(maxname*2)+23;i++)putchar('=');
putchar('\n');
*/
}
