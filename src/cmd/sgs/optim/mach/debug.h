#ident	"@(#)debug.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.1	89/06/08 00:41:29 root
	date and time created 89/06/08 00:41:29 by root

*/
extern int eflag;
#define TRACE(F)	if (eflag) printf("# F \n")
