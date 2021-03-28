/*	symbol.h	1.1	*/
/*	3.0 SID #	1.1	*/

#ifndef SYMBOL_H
#ident	"@(#)symbol.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define SYMBOL_H

/*
 * Structure of a symbol table entry
 */

struct	symbol {
	char	sy_name[8];
	char	sy_type;
	int	sy_value;
};

#endif /* SYMBOL_H */
