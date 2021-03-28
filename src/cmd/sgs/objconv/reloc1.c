#ident	"@(#)reloc1.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"

/*
 * txtbegin=location of beginning of text (sheader.s_scnptr)
 * relbegin=beginning of relocation entries (sheader.s_relptr)
 * relnum=number of relocation entries (sheader.s_nreloc)
 * sym=head point to beginning of symbol table entries
 *
 * quick fix for data reloc, see reloc.c
 */
void
relocate1(outp)
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
	FREAD(&aheader,sizeof(aheader),1,outp);
	FSEEK(outp,sizeof(aheader)+(long)aheader.f_opthdr,0L);
	FREAD(&sheader,sizeof(sheader),1,outp);/* go past text header */
	FREAD(&sheader,sizeof(sheader),1,outp);/* get data header */
	relptr = sheader.s_relptr;
	if(!relptr)
		return;
	scnptr = sheader.s_scnptr;
	nreloc = sheader.s_nreloc;

	FSEEK(outp,relptr,0L);
	while (nreloc--){
		FREAD(&reloc,sizeof(reloc),1,outp);
		index = ftell(outp);
		if(reloc.r_symndx != sym->entrynum) {
			here=sym;
			sym=sym->next;
			while( (reloc.r_symndx != sym->entrynum) && 
			sym!=here){
				sym=sym->next;
			}
		}

		if(reloc.r_symndx !=sym->entrynum)
			continue;


		if( (SCLASS(sym) == C_EXT)  && (SCNUM(sym) == 0)
		   && ( VAL(sym) )){
			unsigned long value;
			FSEEK(outp,scnptr+reloc.r_vaddr-sheader.s_vaddr,0L);
			FREAD(&value,sizeof(value),1,outp);
			value +=VAL(sym);
			FSEEK(outp,scnptr+reloc.r_vaddr-sheader.s_vaddr,0L);
/*
fprintf(stderr,"D[ %14s ][ %04d ][ %#08x ]:[ %#08x ]+[ %#08x ]=[ %#08x ]\n",sym->sname,sheader.s_nreloc-nreloc,ftell(outp),VAL(sym),value-VAL(sym),value);
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
