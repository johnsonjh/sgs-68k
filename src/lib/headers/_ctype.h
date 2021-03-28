#ifndef __CTYPE_H
#ident	"@(#)_ctype.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

#define	_U	01		/* Upper case 			*/
#define	_L	02		/* Lower case 			*/
#define	_N	04		/* Numeral (digit) 		*/
#define	_S	010		/* Spacing character 	*/
#define	_P	020		/* Punctuation 			*/
#define	_C	040		/* Control character 	*/
#define	_B	0100	/* Blank 				*/
#define	_X	0200	/* heXadecimal digit 	*/

#define _SZ_CTYPE	(257 + 257)		/* is* and to{upp,low}er tables 	*/
#define _SZ_CODESET	7				/* bytes for codeset information 	*/
#define _SZ_NUMERIC	2				/* bytes for numeric editing 		*/
#define _SZ_TOTAL	(_SZ_CTYPE + _SZ_CODESET)

extern unsigned char _ctype[_SZ_TOTAL];

#define __CTYPE_H

#endif /* __CTYPE_H */
