#ident	"@(#)strxfrm.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1987 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	This module is modified for MNLS on Oct 12 1988	*/


#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include "_locale.h"

#define		_TWOTOONE	0x4000
#define		_SECONDARY	0x3f00
#define		_PRIMARY	0x00ff

/*  Structure of replacement strings  */

static struct {
	char		*regexp;
	char		*repl_string;
	unsigned short	repl_length;
	int		regexp_length;
} subs[] = { 0 };


/*
 *	ANSI 4.11.4.5 strxfrm function
 */

size_t strxfrm ( char *s1, char *s2, size_t maxlen )
{
	/*  Local Declarations  */
	register int	i, len;

  	register char	*next;
  	char	*secondary, *start_pr, *start_sec;
	int	matchfound;
	short 	prim_sec;
	unsigned short temp;
	int n_sec = 0;
	int n_prim = 0;

	/* allocate space for the secondary weights */
  	if ((secondary = (char *)malloc(maxlen/2 + 1)) == NULL) {
		return maxlen+1 ;
	}
	*secondary = '\0';

	/* initialize pointers to the strings containing the s1 and secondary weights */
	start_pr = s1;
	start_sec = secondary;
	/*  This is where all the work begins  */
	for( ; *s2 != '\0'; s2++) {
		prim_sec = _colltbl[*s2 & 0x0FF];
		i = 0;
		if (prim_sec < 0) { /* we have 1 to n mapping */
	
			/* Start looking through substitute strings array */

			while (_1_to_n[i] != *s2 && _1_to_n[i] != 0) 
				i += strlen( (char *) &_1_to_n[i+1]) + 2;
				
			if (_1_to_n[i] == 0)
				continue; /* No match found */
			for (++i; _1_to_n[i] != '\0'; i++) {
				*s1  = _colltbl[_1_to_n[i]] & _PRIMARY;
				n_prim++;
				s1++;
				if (_colltbl[_1_to_n[i]] & _SECONDARY) {
					n_sec++;
					*secondary++ = (_colltbl[_1_to_n[i]] & _SECONDARY) >> 8 ;
				}
				if ((n_prim + n_sec) >= maxlen)
					return maxlen+1;
			}

		/* If the input character is the first one of 
		 * a two to one mapping sequence, then look for 
		 * the next character in the sequence
		 */

		} else if (prim_sec & _TWOTOONE) /* 2 to 1 mapping */ {
			matchfound = 0;
			while (_2_to_1[i] != 0 && (*(s2+1) != 0)) {
				temp = *(s2+1) << 8;
				temp |= *s2;
				if( temp == _2_to_1[i]) {
					matchfound = 1;
					*s1 = _val_2_to_1[i];
					n_prim++;
					if (_val_2_to_1[i] & _SECONDARY) {
						n_sec++;
						*secondary++ = (_val_2_to_1[i] & _SECONDARY)  >> 8;
					}
					if ((n_prim + n_sec) >= maxlen)
						return maxlen+1;
					s1++;
					s2++;
					break;
				}
			i++;
			}
			if (!matchfound)  {
				*s1 = prim_sec & _PRIMARY;
				n_prim++;
				if (prim_sec & _SECONDARY) {
					n_sec++;
					*secondary++ = (prim_sec & _SECONDARY)  >> 8;
				}
				s1++;
				if ((n_prim + n_sec) >= maxlen)
					return maxlen+1;
			}
			else
				continue; /* to get next char */
						

		} else {
			if (prim_sec != 1) {
				*s1 = prim_sec & _PRIMARY;
				n_prim++;
				if (prim_sec & _SECONDARY) {
					n_sec++;
					*secondary++ = (prim_sec & _SECONDARY)  >> 8;
				}
				if ((n_prim + n_sec) >= maxlen)
					return maxlen+1;
				s1++;
			}
		}
	}

	*s1 = '\0';
	*secondary = '\0';
	n_prim++;
	if ((n_prim + n_sec) > maxlen)
		return maxlen+1;

	/*	Now merge the Primary and Secondary strings */

	if (strlen(start_sec) != NULL)
		strcat(start_pr, start_sec);
	free(start_sec);
	return strlen(start_pr) ;
}


/*
 *	ANSI 4.11.4.3 strcoll() function
 *
 *  The strcoll function compares the string pointed to by s1 (str1) to the
 *  string pointed to by s2 (str2).
 *	Both strings are interpreted as appropriate to the LC_COLLATE category
 *  of the current locale.
 * 
 *  Returns an integer according to the following
 *			if str1 > str2 then int > 0 returned
 *			if str1 = str2 then 0 returned	
 *			if str1 < str2 then int < 0 returned
 */

int strcoll ( char *s1, char *s2 )
{
	char buf1[512], buf2[512]; /* Not malloc'ed for speed purposes */

	if (strxfrm(buf1, s1, 512) >= 512) 
		return (0) ;
	if (strxfrm(buf2, s2, 512) >= 512) 
		return (0) ;
	return(strcmp(buf1, buf2));
}

