#ident	"@(#)writesyms.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"

/* after you're all done changing symbols or whatever
 * then write out the new symbol table and string table
 * Also, change the relocation information, if necessary
 */
void
writesyms(name,inp)
char * name;
register FILE * inp; /* should be pointing at header of file*/
{
register FILE * outp;
register long begin;
register char notfirst;
register struct symelt * sym = head;

#ifdef DEBUG
	if(dflag) printf("writesyms\n");
#endif /* DEBUG */

	begin = ftell(inp); /* find out offset from 0 of input file*/
	if (!( outp = fopen( name, "w+" ))){
		fprintf(stderr,
			"%s: cannot open output file %s for writing\n",
			utilname,name);
		exit(1);
	}
	{
		register long bytes=symptr;
#ifdef DEBUG
		if(dflag)
			printf("symptr[ %#x ] delta[ %#x ] mult[ %#x ]\n",symptr,symptr%1024,symptr/1024);
#endif /* DEBUG */
		/* coppy the file contents up to the symbol stuff */
		if(bytes % 1024){
			FREAD( buf,bytes % 1024,1,inp);
			FWRITE(buf,bytes % 1024,1,outp);
		}
		bytes /= 1024;
		while ( bytes-- ){
			FREAD( buf,1024,1,inp);
			FWRITE(buf,1024,1,outp);
		}
	}
	if(strtabexist){
		long offset=4; /* leave room for the length of stringtable*/
		do{
			if(!ZEROES(sym)){
				FSEEK(outp,strtab+offset,0L);
				FWRITE(sym->sname,sym->namelen,1,outp);
				FWRITE("\0",1,1,outp);
				OFFSET(sym)=offset;
				offset+=sym->namelen+1;
			} /* ZEROES */
			sym=sym->next;
		} while ( sym != head);
		FSEEK(outp,strtab,0L);
		FWRITE(&offset,sizeof(offset),1,outp);
	} /* strtabexist */

	FSEEK(outp,symptr,0L);
	if((symptr != ftell(outp)) || (symptr != (ftell(inp)-begin))){
		fprintf(stderr,
			"Someone not at the beginning of symtable\n");
		fprintf(stderr,"symptr[ %#x ] inp[ %#x ] outp[ %#x ]\n",
			symptr,ftell(inp)-begin,ftell(outp));
		exit(1);
	}
	{
		register long numaux ;
		do{
			/* write out the symbol and copy 
			 * any auxilliary information it might have
			 */
			FWRITE(&sym->sym,sizeof(struct syment),1,outp);
			FSEEK(inp,sizeof(struct syment),1);
			if( numaux = NUMAUX(sym)){
				FREAD( buf,numaux*sizeof(struct syment),1,inp);
				FWRITE(buf,numaux*sizeof(struct syment),1,outp);
			}
			sym = sym->next;
		}while(sym != head);
	}
	if(iflag && !rflag && !mflag)
		fprintf(stderr,"%s: Relocation information in file %s unmodified\n",utilname,name);
	else if(mflag)fixmagic(outp);
	else {relocate(outp);/* relocate text stuff while you're at it */
	relocate1(outp);/* relocate data stuff while you're at it */
	}
	fclose(outp);
}/* end writesyms */

