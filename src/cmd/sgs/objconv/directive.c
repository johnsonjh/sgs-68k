#ident	"@(#)directive.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"
/* look for linker directives found in the object file 
 * if they are of the new format, then warn the user that
 * he is processing a new format file,
 */
long
directive(name)
char * name;
{
	register char * ptr = name;
#ifdef DEBUG
	if(dflag) printf("directive\n");
#endif /* DEBUG */
	*ptr++;
	if(*ptr == 'L'){
		*ptr++;
		while( isdigit(*ptr)) *ptr++;
		if(*ptr == '\0')
			return(1);
	}else{
		while(isdigit(*ptr)) *ptr++;
		if( !strcmp(ptr,"fake")){
			conflict = NEWFILE;
			return(1);
		}
	}
	return(cmptbl(name,dirtbl));
}/* directive */

