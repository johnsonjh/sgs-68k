#ident	"@(#)printtype.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include"decls.h"
/* for print name, print out the type of symbol being processed */
/* look in syms.h  for desc */
printtype(type)
register char type;
{

#ifdef DEBUG
	if(dflag) printf("printtype\n");
#endif /* DEBUG */
		switch(type){
			case C_EFCN: printf(" %8s ","C_EFCN");break;
			case C_NULL: printf(" %8s ","C_NULL");break;
			case C_AUTO: printf(" %8s ","C_AUTO");break;
			case C_EXT: printf(" %8s ","C_EXT");break;
			case C_STAT: printf(" %8s ","C_STAT");break;
			case C_REG: printf(" %8s ","C_REG");break;
			case C_EXTDEF: printf(" %8s ","C_EXTDEF");break;
			case C_LABEL: printf(" %8s ","C_LABEL");break;
			case C_ULABEL: printf(" %8s ","C_ULABEL");break;
			case C_MOS: printf(" %8s ","C_MOS");break;
			case C_ARG: printf(" %8s ","C_ARG");break;
			case C_STRTAG: printf(" %8s ","C_STRTAG");break;
			case C_MOU: printf(" %8s ","C_MOU");break;
			case C_UNTAG: printf(" %8s ","C_UNTAG");break;
			case C_TPDEF: printf(" %8s ","C_TPDEF");break;
			case C_USTATIC: printf(" %8s ","C_USTATIC");break;
			case C_ENTAG: printf(" %8s ","C_ENTAG");break;
			case C_MOE: printf(" %8s ","C_MOE");break;
			case C_REGPARM: printf(" %8s ","C_REGPARM");break;
			case C_FIELD: printf(" %8s ","C_FIELD");break;
			case C_BLOCK: printf(" %8s ","C_BLOCK");break;
			case C_FCN: printf(" %8s ","C_FCN");break;
			case C_EOS: printf(" %8s ","C_EOS");break;
			case C_FILE: printf(" %8s ","C_FILE");break;
			case C_LINE: printf(" %8s ","C_LINE");break;
			case C_ALIAS: printf(" %8s ","C_ALIAS");break;
			case C_HIDDEN: printf(" %8s ","C_HIDDEN");break;
			default: printf("default(%x)",type);break;
		}/* end switch */
} /* end printtype */
