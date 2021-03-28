/*	%W%	*/
/*	execargs.h	1.2	*/

#ident	"%Z%uts/head:%M%	%I%"

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
