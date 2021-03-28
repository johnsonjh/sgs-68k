#ident	"@(#)_doscan.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <stdio.h>
#include "_stdio.h"
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>

extern int _doscan(FILE *iop,unsigned char *fmt,va_list va_alist)
{
	char tab[_NCHARS];
	register int ch;
	int nmatch = 0, len, inchar, stow, size;
	char *pp;
	_chcount=0; _flag_eof=0;

	/*******************************************************
	 * Main loop: reads format to determine a pattern,
	 *		and then goes to read input stream
	 *		in attempt to match the pattern.
	 *******************************************************/
	for ( ; ; ) 
	{
		if ( (ch = *fmt++) == '\0')
			return(nmatch); /* end of format */
		if (isspace(ch)) 
		{
		  	if (!_flag_eof) 
			{
			   while (isspace(inchar = _locgetc()))
	 		        ;
			   if (_locungetc(inchar) == EOF)
				_flag_eof = 1;
		        }
		  continue;
                }
		if (ch != '%' || (ch = *fmt++) == '%') 
                {
			if ( (inchar = _locgetc()) == ch )
				continue;
			if (_locungetc(inchar) != EOF)
				return(nmatch); /* failed to match input */
			break;
		}
		/* We now have a conversion specification */
		if (ch == '*') 
		{
			stow = 0;
			ch = *fmt++;
		}
		else
			stow = 1;

		for (len = 0; isdigit(ch); ch = *fmt++)
			len = len * 10 + ch - '0';
		if (len == 0)
			len = INT_MAX;
		if ( (size = ch) == 'l' || (size == 'h') || (size == 'L'))
			ch = *fmt++;
		if (ch == '\0' ||
		    ch == '[' && (fmt = _setup(fmt, tab)) == NULL)
			return(EOF); /* unexpected end of format */
		if (ch!= 'n' && !_flag_eof)
		  if (ch != 'p' && ch != 'c' && ch != '[') 
		  {
			while (isspace(inchar = _locgetc()))
				;
			if(_locungetc(inchar) == EOF )
				break;
		  }
		switch(ch)
		{
		 case 'c':
		 case 's':
		 case '[':
			  size = _string(stow,ch,len,tab,iop,&va_alist);
			  break;
         case 'n':
			  if (size == 'h')
				*va_arg(va_alist,short*) = (short) _chcount;
		          else if (size == 'l') 
				*va_arg(va_alist,long*) = (long) _chcount;
			      else
			  	*va_arg(va_alist,int*) = (int) _chcount;
			  continue;
		 case 'p':
		 case 'i':
         default :
			 size = _number(stow, ch, len, size, iop, &va_alist);
			 break;
         }
	     if (size) nmatch += stow;   
               else return ((_flag_eof && !nmatch) ? EOF : nmatch);
         continue;
	}
	return (nmatch != 0 ? nmatch : EOF); /* end of input */
}
