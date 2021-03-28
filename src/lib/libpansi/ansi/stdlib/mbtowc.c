#ident	"@(#)mbtowc.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984, 1986, 1987, 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <_ctype.h>

#define _SS2     0x8e
#define _SS3     0x8f
#define _EUCMASK 0x30000000
#define _P00     0x00000000      /* Code Set 0 */
#define _P11     0x30000000      /* Code Set 1 */
#define _P01     0x10000000      /* Code Set 2 */
#define _P10     0x20000000      /* Code Set 3 */
#define _multibyte       (_ctype[520]>1)
#define _eucw1   _ctype[514]
#define _eucw2   _ctype[515]
#define _eucw3   _ctype[516]
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


int mbtowc(wchar_t *wchar, char *s, size_t n)
{
        register int length;
        register wchar_t intcode;
        register c;
        char *olds = (char *)s;
        wchar_t mask;
/* start for MNLS 3.2 by AT&T UP  Nov.16,1988 */
	short	pcw;	/* process code width */
/* end for MNLS 3.2 */
        
        if(s == (char *)0)
                return 0;

        c = (unsigned char)*s++;
        if(!_multibyte || c < 0200) {
                if(wchar)
                        *wchar = c;
                return(c ? 1 : 0);
        }

        if(n == 0)
                return(-1);

/* start for MNLS 3.2 by AT&T UP  Nov.16,1988 */
	if ( _eucw1 > sizeof(unsigned short) ||
	     _eucw2 > sizeof(unsigned short) ||
	     _eucw3 > sizeof(unsigned short))
		pcw = sizeof(unsigned long);	/* 32 bits process code */
	else
		pcw = sizeof(unsigned short);	/* 16 bits process code */
/* end for MNLS 3.2 */
        intcode = 0;
        if (c == _SS2) {
                if(!(length = _eucw2)) 
                        goto lab1;
/* start for MNLS 3.2 by AT&T UP  Nov.16,1988 */
/*              mask = _P01; */
		if ( pcw > sizeof(unsigned short))
                	mask = _P01;
		else
			mask = _H_P01;
/* end for MNLS 3.2 */
                goto lab2;
        } else if(c == _SS3) {
                if(!(length = _eucw3)) 
                        goto lab1;
/* start for MNLS 3.2 by AT&T UP  Nov.16,1988 */
/*              mask = _P10; */
		if ( pcw > sizeof(unsigned short))
                	mask = _P10;
		else
			mask = _H_P10;
/* end for MNLS 3.2 */
                goto lab2;
        } 
lab1:
        if(iscntrl(c)) {
                if(wchar)
                        *wchar = c;
                return(1);
        }
        length = _eucw1 - 1;
/* start for MNLS 3.2 by AT&T UP  Nov.16,1988 */
/*      mask = _P11; */
	if ( pcw > sizeof(unsigned short))
               	mask = _P11;
	else
		mask = _H_P11;
/* end for MNLS 3.2 */
        intcode = c & 0177;
lab2:
        if(length + 1 > n || length < 0)
                return(-1);
        while(length--) {
                if((c = (unsigned char)*s++) < 0200 || iscntrl(c))
                        return(-1);
/* start for MNLS 3.2 by AT&T UP  Nov.16,1988 */
/*              intcode = (intcode << 7) | (c & 0177); */
		if ( pcw > sizeof(unsigned short))
	                intcode = (intcode << 7) | (c & 0177);
		else
                	intcode = (intcode << 8) | (c & 0177);
/* end for MNLS 3.2 */
        }
        if(wchar)
                *wchar = intcode | mask;
        return((char *)s - olds);
}       
