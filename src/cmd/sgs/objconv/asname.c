#ident	"@(#)asname.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"
/* modify a name to be assembly language accesible only
 * by adding % signs after it.
 * Add one percent sign, then make sure it doesn't clash with
 * any name found in the new table.
 * If it does, then add another % sign. If it still clashes
 * with something in the new table, the warn the user
 */
char *
asname(name)
register char * name;
{
#ifdef DEBUG
	if(dflag) printf("asname\n");
#endif DEBUG
	strcpy(buf,name);
	strcat(buf,"%");
	if(cmptbl(buf,newtbl)){
		strcat(buf,"%");
		if(cmptbl(buf,newtbl))
			conflict=NEWTBL;
	}
	return(buf);
}/* end asname */
