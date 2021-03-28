#ident	"@(#)fixsyms.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"
/* according to the type of symbol type
 * returned by typesym, change the name 
 * appropriately
 */
void
fixsyms()
{
	register struct symelt * sym = head;
	register long x;
#ifdef DEBUG
	if(dflag) printf("fixsyms\n");
#endif /* DEBUG */

	if(vflag)printheader();
	do{
		conflict = 0;
		x=typesym(sym->sname);	/* get the action for change */
		switch(x){
			case NOCHANGE: 
	if(iflag||nflag||vflag || ( conflict && !wflag)){
	/*				if(vflag){*/
						printname(sym->sname,
							  SCLASS(sym),
							  sym->namelen,
							  OLDNAME);
						printname(sym->sname,
							  SCLASS(sym),
							  sym->namelen,
							  NEWNAME);
					}
			/* update the symbol structure as changename would */
					if (sym->namelen < 9){
						free(sym->sname);
						sym->sname = NULL;
						}
					break;
			case CNAME:	/* a c name, take off the underbar */
					changename(sym,(sym->sname)+1);
					break;
			case ASNAME:	/* assembly name, add a % or two */
					changename(sym,asname(sym->sname));
					break;
			default:	/* change name to one found in table */
					changename(sym,newtbl[x-1]);
					break;
		}/* end case */

	sym = sym->next;
	}while(sym != head);
}/* end fixsyms */


