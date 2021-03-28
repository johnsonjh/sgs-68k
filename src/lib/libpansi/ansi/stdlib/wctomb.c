#ident	"@(#)wctomb.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984, 1986, 1987, 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#include <ctype.h>
#include <stdlib.h>
#include <_ctype.h>

#define _multibyte       (_ctype[520]>1)
#define _eucw1   _ctype[514]
#define _eucw2   _ctype[515]
#define _eucw3   _ctype[516]
#define _EUCMASK 0x30000000
#define _P00     0x00000000      /* Code Set 0 */
#define _P11     0x30000000      /* Code Set 1 */
#define _P01     0x10000000      /* Code Set 2 */
#define _P10     0x20000000      /* Code Set 3 */
#define _SS2     0x8e
#define _SS3     0x8f
/* start for MNLS 3.2 by AT&T UP  Nov.16,1988 */
/*
 *	The followings are for 2 bytes of multi-byte code definition.
 */
#define _H_EUCMASK 0x8080	/* mask bit for 16 bits */
#define _H_P00     0x0000	/* Code Set 0 for 2 bytes code */
#define _H_P11     0x8080	/* Code Set 1 for 2 bytes code */
#define _H_P01     0x0080	/* Code Set 2 for 2 bytes code */
#define _H_P10     0x8000	/* Code Set 3 for 2 bytes code */
/* end for MNLS 3.2 */

int wctomb(char *s, wchar_t wchar)
{
        char *olds = s;
        register int size, index;
        unsigned char d;
/* start for MNLS 3.2 by AT&T UP  Nov.16,1988 */
	short	pcw;	/* process code width */
/* end for MNLS 3.2 */
        if(!s)
                return(0);
        if(!_multibyte || wchar <= 0177 || wchar <= 0377 && iscntrl(wchar)) {
                *s++ = (char)wchar;
                return(1);
        }
/* start for MNLS 3.2 by AT&T UP  Nov.16,1988 */
	if ( _eucw1 > sizeof(unsigned short) ||
	     _eucw2 > sizeof(unsigned short) ||
	     _eucw3 > sizeof(unsigned short))
		pcw = sizeof(unsigned long);	/* 32 bits process code */
	else
		pcw = sizeof(unsigned short);	/* 16 bits process code */
/* end for MNLS 3.2 */
/* start for MNLS 3.2 by AT&T UP  Nov.16,1988 */
	if (pcw > sizeof(unsigned short)) {
        switch(wchar & _EUCMASK) {
                
                case _P11:
                        size = _eucw1;
                        break;
                
                case _P01:
                        *s++ = (char)_SS2;
                        size = _eucw2;
                        break;
                
                case _P10:
                        *s++ = (char)_SS3;
                        size = _eucw3;
                        break;
                
                default:
                        return(-1);
        }
	} else {
        switch(wchar & _H_EUCMASK) {
                
                case _H_P11:
                        size = _eucw1;
                        break;
                
                case _H_P01:
                        *s++ = (char)_SS2;
                        size = _eucw2;
                        break;
                
                case _H_P10:
                        *s++ = (char)_SS3;
                        size = _eucw3;
                        break;
                
                default:
                        return(-1);
        }
	}
/* end for MNLS 3.2 */
        if((index = size) <= 0)
                return -1;      
        while(index--) {
                d = wchar | 0200;
/* start for MNLS 3.2 by AT&T UP  Nov.16,1988 */
/*              wchar >>= 7; */
		if (pcw > sizeof(unsigned short))
                	wchar >>= 7;
		else
	                wchar >>= 8;
/* end for MNLS 3.2 */
                if(iscntrl(d))
                        return(-1);
                s[index] = d;
        }
        return(s + size - olds);
}
