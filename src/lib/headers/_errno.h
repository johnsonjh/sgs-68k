#ifndef __ERRNO_H
#ident	"@(#)_errno.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

#ifdef __STRICT_STDC__
extern char *_sys_errlist[];
extern int _sys_nerr;
#else
extern char *sys_errlist[];
extern int sys_nerr;
#endif /* __STRICT_STDC__ */

#define __ERRNO_H

#endif /* __ERRNO_H */
