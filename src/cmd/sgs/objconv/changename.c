#ident	"@(#)changename.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"
/* modifiy the name in sym entry, to newname
 * update the rest of the sym structure
 * all the mucking with the name length helps determine
 * whether the name will be inside the sym structure or
 * put in the string table in the output file
 */
void
changename(sym,newname)
register struct symelt *sym;
register char * newname;
{
	register char * ptr;
	register long olen;
	register long nlen;
	register long adj;

#ifdef DEBUG
	if(dflag) printf("changename\n");
#endif DEBUG
	olen = sym->namelen;
	nlen = strlen(newname);
	if(olen < 9 && nlen > 8)strtabexist++;
	else if(olen > 8 && nlen < 9) strtabexist--;
	if(iflag||nflag||vflag || ( conflict && !wflag)){
		printname(sym->sname,SCLASS(sym),olen,OLDNAME);
		printname(newname,SCLASS(sym),nlen,NEWNAME);
	}
	if(nlen < 9){
		strncpy (NAME(sym),newname,8);
		strncpy (sym->sname,newname,8);
		/*free(sym->sname);
		sym->sname = NULL;*/
		adj = (olen < 9) ? 0 : -olen;
	}else{
		ptr = malloc(nlen);
		strncpy(ptr,newname,nlen);
		free(sym->sname);
		sym->sname = ptr;
		if (olen < 9 ){
			adj =nlen;
			ptr = NAME(sym);
			ZEROES(sym) = OFFSET(sym) = 0;
		}else adj = nlen - olen;
	}
	sym->adj =adj;
	sym->namelen = nlen;
}/* end changename */
