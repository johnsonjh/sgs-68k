/*	%W%	*/
/*	fatal.h	1.2	*/

#ident	"%Z%uts/head:%M%	%I%"

extern	int	Fflags;
extern	char	*Ffile;
extern	int	Fvalue;
extern	int	(*Ffunc)();
/* must include setjmp.h for jmp_buf definition */
#ifndef _JBLEN
#include <setjmp.h>
#endif
extern	jmp_buf	Fjmp;

# define FTLMSG		0100000
# define FTLCLN		 040000
# define FTLFUNC	 020000
# define FTLACT		    077
# define FTLJMP		     02
# define FTLEXIT	     01
# define FTLRET		      0

# define FSAVE(val)	SAVE(Fflags,old_Fflags); Fflags = val;
# define FRSTR()	RSTR(Fflags,old_Fflags);
