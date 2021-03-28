#ident	"@(#)readsyms.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"

/* read the symbol structures into a looped, linked list
 * get their names from the string tabel if necessary
 * get their name length, and gather info about auxilliary entries
 */
readsyms(){
register struct symelt * newsym;
register struct symelt * oldsym;
register long reg;
register long index = nsyms;		/* general counter */

#ifdef DEBUG
	if(dflag) printf("readsyms\n");
#endif /* DEBUG */

FSEEK(symp,symptr,0L);
for(index=0;index<nsyms;index++){
	if( NULL == ( newsym = (struct symelt *) 
		malloc(sizeof (struct symelt)))){
		fprintf(stderr,
		"%s: Malloc out of space. Symbol number [ %d ]\n",utilname,index);
		exit(1);
		}
	if(!index) head = oldsym = newsym;
	FREAD(&newsym->sym, sizeof(struct syment ),1,symp);
	if (!ZEROES(newsym)){	/* a long name in the string table */
		register long templen,symptell;
		strtabexist++;
		symptell=ftell(symp);
		FSEEK(symp,strtab + OFFSET(newsym),0);
		FREAD(buf,1,128,symp);
		FSEEK(symp,symptell,0);
		templen =newsym->namelen=strlen(buf);
		newsym->sname = malloc( (templen<8) ? 9 : templen+1);
		strcpy(newsym->sname, buf);
	}else {
		newsym->sname = malloc(9);
		strncpy(newsym->sname,NAME(newsym),8);
		*((newsym->sname)+8)='\0';
		newsym->namelen = strlen(newsym->sname);
	}
/* keep track of the longest name, so that verbose mode can print out nicely */
	maxname = (maxname > newsym->namelen)? maxname : newsym->namelen;

	newsym->entrynum = index;
	if (reg = NUMAUX(newsym)){
/*
		static AUXENT auxent;
		index += reg;
		while(reg--){
		FREAD(&auxent, AUXESZ,1,symp);
		printf("name[ %s ]\n",newsym->sname);
		printf("scnlen[ %#x ] nreloc[ %#x ] nlinno[ %#x ]\n",
		auxent.x_scn.x_scnlen,
		auxent.x_scn.x_nreloc,
		auxent.x_scn.x_nlinno);

		}
*/
		/* go past the auxilliary info */
		FSEEK(symp, reg * sizeof (struct syment), 1);
		index += reg;
		}
	newsym->adj = 0;
	/* link it in */
	if(index == (nsyms-1)) newsym->next = head;
	oldsym->next= newsym;
	oldsym = newsym;
	}
	/* give maxname a little leaway */
	maxname +=3;
}/* end readsyms */
