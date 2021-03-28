#ident	"@(#)tv.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	OLD MOTtv.h	2.1		*/
/*static char ID_tvh[] = "OLD MOT tv.h:	2.1 83/07/08";*/


struct tventry {
	long	tv_addr;
	};

#define TVENTRY struct tventry
#define TVENTSZ sizeof(TVENTRY)
#define MAXTVSIZE	0x7ffc		/* Maximum size of Mc68 SGS 
					   transfer vector using 
					   MC_IND16 reloc */

