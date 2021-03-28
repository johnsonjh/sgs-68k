/*
*	nlist.h: 1.2 3/19/83"
*/


#ifndef NLIST_H
#ident	"@(#)nlist.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define NLIST_H

/* symbol table entry structure */

struct nlist
{
	char		*n_name;	/* symbol name */
	long		n_value;	/* value of symbol */
	short		n_scnum;	/* section number */
	unsigned short	n_type;		/* type and derived type */
	char		n_sclass;	/* storage class */
	char		n_numaux;	/* number of aux. entries */
};

#endif /* NLIST_H */
