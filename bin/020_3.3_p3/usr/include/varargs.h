/*	varargs.h	1.2	*/


#ifndef _VARARGS_H
#ident	"@(#)varargs.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define _VARARGS_H

#if !defined _STDARG_H && !defined _STDIO_H
typedef char *va_list; /* Also defined in stdio.h & stdarg.h	*/
#endif

#define va_dcl int va_alist;
#define va_start(list) list = (char *) &va_alist
#define va_end(list)
#ifdef u370
#define va_arg(list, mode) ((mode *)(list = \
	(char *) ((int)list + 2*sizeof(mode) - 1 & -sizeof(mode))))[-1]
#else
#define va_arg(list, mode) ((mode *)(list += sizeof(__argtype(mode))))[-1]
#endif

#endif	/* _VARARGS_H */
