#ident	"@(#)shlib.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc:inc/shlib.h	1.2"	*/
	
/*
 * This header file contains all the macros and definitons
 *  needed for importing symbols for libc_s
 * 
 */
#if SHLIB
#include <stdio.h>
extern char  (* _libc__ctype)[];
extern char * (* _libc_sys_errlist) ;
extern int    (* _libc_sys_nerr) ;

extern struct group * (* _libc_getgrent)() ;
extern struct group * (* _libc_fgetgrent)() ;
extern void  (* _libc_setgrent)() ;
extern void  (* _libc_endgrent)() ;
extern char * (* _libc_malloc)() ;
extern void (* _libc_free)()   ;  
extern char * (* _libc_realloc)();

extern struct passwd * (* _libc_getpwent)() ;
extern struct passwd * (* _libc_fgetpwent)() ;
extern void  (*_libc_setpwent)() ;
extern void  (*_libc_endpwent)() ;


extern char * (* _libc_realloc)() ;
extern int (*_libc__doprnt)() ;
extern int (*_libc__cerror)() ;
extern int (*_libc_isnand)() ; 

extern unsigned char (* _libc__sibuf)[] , (* _libc__sobuf)[] ;

extern unsigned char (* _libc__smbuf)[][_SBFSIZ] ;

extern FILE (* _libc__iob)[] ;

extern FILE * (* _libc__lastbuf) ;

extern unsigned char * (* _libc__bufendtab)[] ; 
extern long (*_libc_rd881_control)() ;
extern void (*_libc_wr881_control)() ;

extern int (* _libc_end) ;  

extern void (* _libc__cleanup)() ;

extern char ** (* _libc_environ) ;

#define _ctype	(* _libc__ctype)
#define _getflthw	(* _libc__getflthw)
#define _cleanup (* _libc__cleanup)
#define environ  (* _libc_environ)		
#define end	 (* _libc_end) 
#define malloc	(* _libc_malloc)		
#define free	(* _libc_free)	
#define realloc (* _libc_realloc)
#define _sibuf  (* _libc__sibuf)
#define _sobuf  (* _libc__sobuf)
#define _smbuf  (* _libc__smbuf)
#define _iob 	(* _libc__iob)
#define _lastbuf	(* _libc__lastbuf)
#define _bufendtab	(* _libc__bufendtab)
#define _libc_doprnt    (*_libc__doprnt)
#define _libc_cerror  (*_libc__cerror)
#define _libc_isnand  (*_libc_isnand)
#define _libc_rd881_control (*_libc_rd881_control)
#define _libc_wr881_control (*_libc_wr881_control)
#endif
