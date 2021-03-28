/*	core.h	1.2	*/


#ifndef CORE_H
#ident	"@(#)core.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define CORE_H

/* machine dependent stuff for core files */

#if m68 || m68k || M68020
/* must include param.h for PAGESIZE
            and user.h  for USIZE
*/
#define TXTRNDSIZ PAGESIZE
#define stacktop(siz) (0x1000000L - ( USIZE * PAGESIZE ))
#define stackbas(siz) (0x1000000L - ( USIZE * PAGESIZE ) - siz)
#endif

#if vax
#define TXTRNDSIZ 512L
#define stacktop(siz) (0x80000000L)
#define stackbas(siz) (0x80000000L-siz)
#endif

#if pdp11
#define TXTRNDSIZ 8192L
#define stacktop(siz) (0x10000L)
#define stackbas(siz) (0x10000L-siz)
#endif


#if u3b
#define TXTRNDSIZ 0x20000
#define stacktop(siz) 0xC0000
#define stackbas(siz) (0xC0000 + siz)
#endif

#if u3b5
#define TXTRNDSIZ 2048L
#define stacktop(siz) (0xF00000 + siz)
#define stackbas(siz) 0xF00000
#endif

#endif /* CORE_H */
