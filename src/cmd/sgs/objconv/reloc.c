#ident	"@(#)reloc.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"

/*
 * txtbegin=location of beginning of text (sheader.s_scnptr)
 * relbegin=beginning of relocation entries (sheader.s_relptr)
 * relnum=number of relocation entries (sheader.s_nreloc)
 * sym=head point to beginning of symbol table entries
 *
 * For some reason, things in text and data, that are external and
 * and have an undeclared section, must have any relocation info
 * associated with them, modified. Just sum the current info in the
 * position pointed to by the reloc info, with the value of the symbol.
 *
 * This file fixes text information only. Another changes data only
 * these may be combined, given time, in the future.
 */
void
relocate(outp)
register FILE * outp;
{
	register long nreloc;
	register long relptr;
	register long scnptr;
	register long index;

	register struct symelt * sym = head;
	register struct symelt * here;

#ifdef DEBUG
	if(dflag)fprintf(stderr,"RELOCATE\n");
#endif

	FSEEK(outp,0,0L);
	FREAD(&aheader,sizeof(aheader),1,outp);/* seek past the headers */
	FSEEK(outp,sizeof(aheader)+(long)aheader.f_opthdr,0L);
	FREAD(&sheader,sizeof(sheader),1,outp); /*get the text section header */
	relptr = sheader.s_relptr; /* this is the pointer to the rel commands */
	if(!relptr)		   /* no relocation commands if zero, quit */
		return;
	scnptr = sheader.s_scnptr; /* pointer to the text section */
	nreloc = sheader.s_nreloc; /* number of commands to process */

	FSEEK(outp,relptr,0L);
	while (nreloc--){
		FREAD(&reloc,sizeof(reloc),1,outp);
		index = ftell(outp);
		/* find the right symbol entry */
		if(reloc.r_symndx != sym->entrynum) {
			here=sym;
			sym=sym->next;
			while( (reloc.r_symndx != sym->entrynum) && 
			sym!=here){
				sym=sym->next;
			}
		}

		/* symbol not found anywhere, so keep going */
		if(reloc.r_symndx !=sym->entrynum)
			continue;


		/* the symbol must pass all the tests
		 * it must be global (C_EXT), 
		 * it must not be assigned to any section
		 * (SCNUM is 0)
		 * and to make it worth while, the value
		 * of the sym must be greater than 0
		 */
		if( (SCLASS(sym) == C_EXT)  && (SCNUM(sym) == 0)
		   && ( VAL(sym) )){
			unsigned long value;
			FSEEK(outp,scnptr+reloc.r_vaddr,0L);
			/* get the existing value from text */
			FREAD(&value,sizeof(value),1,outp);
			/* add to it the value of the symbol */
			value +=VAL(sym);
			/* then write it back out to the text loc */
			FSEEK(outp,scnptr+reloc.r_vaddr,0L);
/*
fprintf(stderr,"T[ %14s ][ %04d ][ %#08x ]+[ %#08x ]=[ %#08x ]\n",sym->sname,sheader.s_nreloc-nreloc,VAL(sym),value-VAL(sym),value);
*/
			switch(reloc.r_type){
				case R_RELBYTE:
					FWRITE(&value,1,1,outp);
					break;
				case R_RELWORD:
					FWRITE(&value,2,1,outp);
					break;
				case R_RELLONG:
					FWRITE(&value,4,1,outp);
/*printf("name[ %s ] value[ %#x ] loc[ %#x ]\n",ZEROES(sym)?NAME(sym):sym->sname,value,reloc.r_vaddr);*/
					break;
				default:
					fprintf(stderr,"UNKNOWN RELOC TYPE [ %d ] \n",reloc.r_type);
					break;
			}
			FSEEK(outp,index,0L);
		}
	}
}
