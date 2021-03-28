#ident	"@(#)zmcrt.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-mach:crt/zmcrt.c	1.1"	*/
	
#if SHLIB
long (*_libc_rd881_control)() =0;
void (*_libc_wr881_control)() =0;
#endif
