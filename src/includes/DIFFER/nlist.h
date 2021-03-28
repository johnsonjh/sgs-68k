/*	%W%	*/
/*
*	nlist.h: 1.2 3/19/83"
*/

#ident	"%Z%uts/head:%M%	%I%"



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

