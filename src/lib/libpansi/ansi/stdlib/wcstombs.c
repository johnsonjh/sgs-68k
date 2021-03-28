#ident	"@(#)wcstombs.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


/*
 * wcstombs.c
 * 30/6/88
 * UEL
 */

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

size_t wcstombs(char *s, wchar_t *wchar, size_t n)
{
        register int no_bytes = 0;
        register int size = 0;
        register int index;
        unsigned char d;
        register int shift_char = 0;
        register int wc_val;

        if(!s)
                return(0);
        while (n > 0) {
                if (*wchar == 0) {
                        *s = 0;
                        return no_bytes;
                }
                if(!_multibyte||*wchar<0200||*wchar<=0377&&iscntrl(*wchar)) {
                        *s++ = *wchar++;
                        no_bytes++;
                        n--;
                } else {
                        switch(*wchar & _EUCMASK) {

                                case _P11:
                                        size = _eucw1;
                                        shift_char = 0;
                                        break;

                                case _P01:
                                        size = _eucw2 + 1;
                                        shift_char = _SS2;
                                        break;

                                case _P10:
                                        size = _eucw3 + 1;
                                        shift_char = _SS3;
                                        break;

                                default:
                                        return(-1);
                        }
                        n -= size;
                        if (n < 0) {
                                *s = 0;
                                return no_bytes;
                        }
                        no_bytes += size;
                        if (shift_char) {
                                *s++ = shift_char;
                                size--;
                        }
                        if((index = size) <= 0)
                                return(-1);
                        wc_val = *wchar++;
                        while(index--) {
                                d = wc_val | 0200;
                                wc_val = wc_val >> 7;
                                if(iscntrl(d))
                                        return(-1);
                                s[index] = d;
                        }
                        s += size;
                }
        }
        return no_bytes;
}
