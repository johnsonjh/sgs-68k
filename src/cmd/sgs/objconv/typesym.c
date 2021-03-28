#ident	"@(#)typesym.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"

/* determine whether a name is a c symbol, an assembly symbol or
 * some kind of linker directive
 * if it is a symbol that maps from an old name to a new, return that
 */
long 
typesym(name)
register char * name;
{
	register long retval;
	register char *ptr;

#ifdef DEBUG
	if(dflag) printf("typesym [ %s ]\n",name);
#endif /* DEBUG */

	if ( retval = cmptbl(name, oldtbl) )
		return (retval); /* a match found in the tables */
	if ( cmptbl(name, newtbl) ) /* trouble, a match found in the new table */
		conflict = NEWTBL;
	if (( *name == '.' ) && directive( name ) ) /* is it linker directive */
		return( NOCHANGE );
	if ( *name == '_' ){ /* some kind of c name */
		ptr = name;
		if ( *++ptr == '.' ){
			register char * fptr = ptr;
			while( isdigit(*++fptr) );
			if ( !strcmp( fptr, "fake" ) )
				return(CNAME);
		}
		while (iscalpha (*ptr)) *ptr++;
		if(*ptr == '\0')
			return(CNAME);
		while (iscalnum (*ptr)) *ptr++;
		if(*ptr == '\0')
			return(CNAME);
	}
	if( strchr(name, (int)'.')) /* some kind of local symbol */
		return(NOCHANGE);
	if( strchr(name,(int)'%')){ /* already in new format, trouble */
		conflict = NEWFILE;
		return(ASNAME); /* what should you return */
	}
	ptr = name;
/*	printf("typesym(something missing? FELL THROUGH\n"); */
	return (ASNAME); /* by default */
} /* end typesym */
