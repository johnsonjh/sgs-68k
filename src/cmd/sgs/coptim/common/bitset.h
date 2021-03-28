#ident	"@(#)bitset.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)bitset.h	7.1		*/
/*	Copyright 1985 Motorola Inc.			*/

typedef struct {
	int   setlen; /* no of elements in the bit set */
	char *set;    /* Array of elements             */
       } SET;

#define FREESET(x) (void) free((char *)x);

#define IS_IN(i,x) (i<0 || i>=x->setlen ? bitrangeerr() : x->set[i/8]&(1<<i%8))

#define SET_BIT(i,x) if (i<0 || i>=x->setlen)	\
			(void) bitrangeerr();	\
		     else x->set[i/8] |= 1<<i%8;

#define CLR_BIT(i,x) if (i<0 || i>=x->setlen)	\
			(void) bitrangeerr();	\
		     else x->set[i/8] &= (0xff-1<<i%8);

#define SET_ALL(x)  {int i; for (i=0; i<(x->setlen+8)/8; i++) x->set[i]=0xff;}

#define CLR_ALL(x)  {int i; for (i=0; i<(x->setlen+8)/8; i++) x->set[i]=0;}
