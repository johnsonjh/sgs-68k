/*	%W%	*/
/*	symbol.h	1.1	*/
/*	3.0 SID #	1.1	*/
#ident	"%Z%uts/head:%M%	%I%"


/*
 * Structure of a symbol table entry
 */

struct	symbol {
	char	sy_name[8];
	char	sy_type;
	int	sy_value;
};
