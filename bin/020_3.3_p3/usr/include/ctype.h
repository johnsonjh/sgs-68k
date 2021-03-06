/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#ifndef _CTYPE_H 
#ident	"@(#)ctype.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define _CTYPE_H
#pragma SYS

/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#if !__STRICT_STDC__

#if !lint

#define	_toupper(c)     ((_ctype + 258)[c])
#define	_tolower(c)	((_ctype + 258)[c])
#define	toascii(c)	((c) & 0177)

#if _SYSV

#define	_U	01		/* Upper case */
#define	_L	02		/* Lower case */
#define	_N	04		/* Numeral (digit) */
#define	_S	010		/* Spacing character */
#define	_P	020		/* Punctuation */
#define	_C	040		/* Control character */
#define	_B	0100	/* Blank */
#define	_X	0200	/* heXadecimal digit */

#define	isalpha(c)	((_ctype + 1)[c] & (_U | _L))
#define	isupper(c)	((_ctype + 1)[c] & _U)
#define	islower(c)	((_ctype + 1)[c] & _L)
#define	isdigit(c)	((_ctype + 1)[c] & _N)
#define	isxdigit(c)	((_ctype + 1)[c] & _X)
#define	isalnum(c)	((_ctype + 1)[c] & (_U | _L | _N))
#define	isspace(c)	((_ctype + 1)[c] & _S)
#define	ispunct(c)	((_ctype + 1)[c] & _P)
#define	isprint(c)	((_ctype + 1)[c] & (_P | _U | _L | _N | _B))
#define	isgraph(c)	((_ctype + 1)[c] & (_P | _U | _L | _N))
#define	iscntrl(c)	((_ctype + 1)[c] & _C)
#define	isascii(c)	(!((c) & ~0177))

#endif	/* _SYSV */

#endif	/* !lint */

#endif	/* !__STRICT_STDC__ */


/*****************************************************************************
                   E X T E R N A L    D E C L A R A T I O N S 
 ****************************************************************************/

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

#if !lint
extern unsigned char	_ctype[];
#endif	/* !lint */

#if !_SYSV

int isalnum _PARMS((int));
int isalpha _PARMS((int));
int iscntrl _PARMS((int));
int isdigit _PARMS((int));
int isgraph _PARMS((int));
int islower _PARMS((int));
int isprint _PARMS((int));
int ispunct _PARMS((int));
int isspace _PARMS((int));
int isupper _PARMS((int));
int isxdigit _PARMS((int));
int tolower _PARMS((int));
int toupper _PARMS((int));

#endif	/* _SYSV */

#pragma noSYS
#endif	/* _CTYPE_H */
