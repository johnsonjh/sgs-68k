/*	memory.h	1.2	*/


#ifndef MEMORY_H
#ident	"@(#)memory.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define MEMORY_H

#if defined _SYSV
extern char
#else
extern void
#endif	/* _SYSV */
	*memchr(),
	*memcpy(),
	*memset();
extern char *memccpy();
extern int memcmp();

#endif /* MEMORY_H */
