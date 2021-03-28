#ident	"@(#)mbstowcs.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


/*
 * mbstowcs.c
 * 30/6/88
 * UEL
 */

#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <_ctype.h>

#define _SS2     0x8e
#define _SS3     0x8f
#define _EUCMASK 0x30000000
#define _P00     0x00000000    /* Code Set 0 */
#define _P11     0x30000000    /* Code Set 1 */
#define _P01     0x10000000    /* Code Set 2 */
#define _P10     0x20000000    /* Code Set 3 */
#define _multibyte       (_ctype[520]>1)
#define _eucw1   _ctype[514]
#define _eucw2   _ctype[515]
#define _eucw3   _ctype[516]

size_t mbstowcs(wchar_t *wchar, char *s, size_t n)
{
    register int length;
    register wchar_t intcode;
    register c;
    wchar_t mask;
    int size;
        
        if(n <= 0)
                return(-1);
        if(s == (char *)0)
                return 0;

        size = 0;
        while (n-- > 0) {
                c = (unsigned char)*s++;
                intcode = 0;
                length = 0;
                mask = 0;
                if (!_multibyte || c < 0200) 
                        intcode = c; 
                else
                if (c == _SS2) {
                        if((length = _eucw2)) 
                                mask = _P01;
                } else
                if (c == _SS3) {
                        if((length = _eucw3)) 
                                mask = _P10;
                } else
                if (iscntrl(c)) 
                        intcode = c;
                else {
                        length = _eucw1-1;
                        mask = _P11;
                        intcode = c & 0177;
                }
                size++;
                if(length) {
                        if ( (n-length-1) < 0) {
                                *wchar = 0;
                                return size;
                        }
                        while(length--) {
                                if((c=(unsigned char)*s++) < 0200 || iscntrl(c))
                                                        return(-1);
                                intcode = (intcode << 7) | (c & 0177);
                        }
                } 
                *wchar++ = intcode | mask;
                if (intcode == 0) {
                        size--;
                        return size;
                }
        }       
        return size;
}
