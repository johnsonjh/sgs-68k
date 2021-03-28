#ident	"@(#)interact.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"
/* interactive allows the user to change one symbol at a time
 * he must supply the proper old name
 * user must hit control D to exit interactive mode
 */
void
interactive()
{
	register struct symelt * sym;
	register struct symelt * tmp;
	register long x;
	register long count;

	sym = head;
	printf("OLD NAME: ");
	count=scanf("%s",buf);
	while( count != -1 ){
		tmp = sym;
		do{
			if(sym->sname == NULL)
				x=strcmp(buf,NAME(sym));
			else x = strcmp(buf, sym->sname);
			if (x) sym = sym->next;
		}while( (sym != tmp) && x );
		if(!x){
			do{
				printf("NEW NAME: ");
				count = scanf("%s",buf);
			}while(count <= 0);
			changename(sym,buf);
		}else printf("%s NOT FOUND\n",buf);
		buf[0]='\0';
		printf("OLD NAME: ");
		count=scanf("%s",buf);
	}
	printf("\n");return;
}/* end changename */
