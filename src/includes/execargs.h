/*	execargs.h	1.2	*/


#ifndef EXECARGS_H
#ident	"@(#)execargs.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define EXECARGS_H

#if m68 || m68k || M68020
char **execargs = (char**)(0xfefffc);
#endif

#if vax
char **execargs = (char**)(0x7ffffffc);
#endif

#if pdp11
char **execargs = (char**)(-2);
#endif

#if u3b || u3b5
/* empty till we can figure out what to do for the shell */
#endif

#endif /* EXECARGS_H */
