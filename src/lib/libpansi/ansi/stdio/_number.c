#ident	"@(#)_number.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include "_stdio.h"
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>
#include "_locale.h"
#include "values.h"

extern int _number(int stow,int type,int len,int size,FILE *iop,va_list *listp)
{
	char numbuf[64], inchar, lookahead;
	register char *np = numbuf;
	char *pp;
	register int c, base;
	int digitseen = 0, dotseen = 0, expseen = 0, floater = 0, negflg = 0;
	long lcval = 0;
	switch(type) 
	{
	case 'e':
	case 'E':
	case 'f':
	case 'g':
	case 'G': floater++;
	case 'd':
	case 'u':
    case 'i': base = 10; break;
	case 'o': base = 8; break;
	case 'p':
	case 'x':
	case 'X': base = 16; break;
	default: return(0); /* unrecognized conversion character */
	}
	switch(c = _locgetc()) 
	{
	case '-': negflg++;
	case '+': /* fall-through */
		if (--len <= 0) break;
		if ( (c = _locgetc()) != '0') break;
    case '0':
        if ( (type != 'i') || (len <= 1) )  break;
	    if ( ((inchar = _locgetc()) == 'x') || (inchar == 'X') || 
											   (inchar == 'p')) 
	        {
		      /* If not using sscanf and *
		       * at the buffer's end     *
		       * then LOOK ahead         */

#ifndef DYNAMIC
		      if ( (iop->_file == FOPEN_MAX) || (iop->_cnt != 0) )
#else /* DYNAMIC */
		      if ((iop->_flag & _IOSTRING) || (iop->_cnt != 0))
#endif /* DYNAMIC */
		               lookahead = _locgetc();
		      else
		   { if ( read(_fileno(iop),np,1) == 1) lookahead = *np;
                      						else lookahead = EOF;
                      _chcount += 1;
		   }    
		   if ( isxdigit(lookahead) ) {
		       base =16;
			   if ( len <= 2) { _locungetc(lookahead); len -= 1;}
			                  /* Take into account the 'x'*/ 
		                 else { c = lookahead; len -= 2;}
			   				  /* Take into account '0x'*/
           }
	       else { _locungetc(lookahead); _locungetc(inchar); }
		}
	    else { _locungetc(inchar); base = 8; }
	}
	for (; --len  >= 0 ; *np++ = c, c = _locgetc()) 
	{
		if (np > numbuf + 62) return 0;
		if (isdigit(c) || base == 16 && isxdigit(c)) 
		{
			int digit = c - (isdigit(c)?'0':isupper(c)?'A'-10:'a'-10);
			if (digit >= base) break;
			if (stow && !floater) lcval = base * lcval + digit;
			digitseen++; continue;
		}
		if (!floater) break;
		if ( c == '.' && !dotseen++ ) continue;
		if ( (c == 'e' || c == 'E') && digitseen && !expseen++) {
			*np++ = c; c = _locgetc();
			if (isdigit(c) || c == '+' || c == '-' || isspace(c)) continue;
		}
		break;
	}
	if (stow && digitseen)
		if(floater) 
		{
			register long double dval;
	
			*np = '\0'; dval = atof(numbuf);
			if (negflg) dval = -dval;
			if (size == 'l') *va_arg(*listp,double*) = (double)dval;
				else if (size == 'L') *va_arg(*listp,long double*) = dval;
					 			 else *va_arg(*listp,float*) = (float)dval;
		}
		else
		{
			/* suppress possible overflow on 2's-comp negation */
			if (negflg && lcval != HIBITL) lcval = -lcval;
			if (size == 'l') *va_arg(*listp,long*) = lcval;
				else if (size == 'h') *va_arg(*listp,short*) = (short)lcval;
								 else *va_arg(*listp,int*) = (int)lcval;
		}
	if (_locungetc(c) == EOF) _flag_eof=1;
	return (digitseen); /* successful match if non-zero */
}
