/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#ifndef _STRING_H
#ident	"@(#)string.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define _STRING_H
#pragma SYS

/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#define NULL	0		/* Also defined in locale, stddef, stdio, stdlib */
						/* and time										 */


/*****************************************************************************
                                  T Y P E S
 ****************************************************************************/

#if  !defined _STDDEF_H && !defined _STDIO_H && !defined _STDLIB_H && !defined _TIME_H && !defined SYS_TYPES_H
typedef unsigned int size_t;	/* Also defined in stddef, stdio, stdlib */
								/* and time								 */
#endif


/*****************************************************************************
                   E X T E R N A L    D E C L A R A T I O N S 
 ****************************************************************************/

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

char *strcpy _PARMS((char *,const char *));
char *strncpy _PARMS((char *,const char *,size_t));
char *strcat _PARMS((char *,const char *));
char *strncat _PARMS((char *,const char *,size_t));
int memcmp _PARMS((const void *,const void *,size_t));
int strcmp _PARMS((const char *,const char *));
int strcoll _PARMS((const char *,const char *)); 
int strncmp _PARMS((const char *,const char *,size_t));
size_t strxfrm _PARMS((char *,const char *,size_t));
char *strchr _PARMS((const char *,int));
char *strpbrk _PARMS((const char *,const char *));
char *strrchr _PARMS((const char *,int));
char *strstr _PARMS((const char *,const char *));
char *strtok _PARMS((char *,const char *));
char *strerror _PARMS((int));

#if defined _SYSV
int strlen();
int strspn();
int strcspn();
char *memcpy();
char *memset();
char *memchr();
#else
void *memmove _PARMS((void *,const void *,size_t));
void *memcpy _PARMS((void *,const void *,size_t));
void *memset _PARMS((void *,int,size_t));
void *memchr _PARMS((const void *,int,size_t));
size_t strcspn _PARMS((const char *,const char *));
size_t strspn _PARMS((const char *,const char *));
size_t strlen _PARMS((const char *));
#endif /* _SYSV */

#if !defined __STRICT_STDC__
char *memccpy _PARMS((void *, const void *, int, int ));
char *strdup _PARMS((char * ));

#endif	/* ! __STRICT_STDC__ */


#pragma noSYS
#endif	/* _STRING_H */
