/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#ifndef _LOCALE_H 
#ident	"@(#)locale.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define _LOCALE_H
#pragma SYS

/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#define NULL		0	/* Also defined in stddef, stdio, stdlib, string */
						/* and time										 */
#define	LC_ALL		6
#define	LC_COLLATE	0
#define LC_CTYPE	1
#define LC_MONETARY	2
#define LC_NUMERIC	3
#define	LC_TIME		4
#define LC_NAMELEN	15
#define LC_MAX		5


/*****************************************************************************
                                  T Y P E S
 ****************************************************************************/

struct	lconv {
    char	*currency_symbol;
    char	*decimal_point;
    char	frac_digits;
    char	*grouping;
    char	*int_curr_symbol;
    char	*mon_decimal_point;
    char	*mon_grouping;
    char	int_frac_digits;
    char	*mon_thousands_sep;
    char	n_cs_precedes;
    char	n_sep_by_space;
    char	n_sign_posn;
    char	*negative_sign;
    char	p_cs_precedes;
    char	p_sep_by_space;
    char	p_sign_posn;
    char	*positive_sign;
    char	*thousands_sep;
};


/*****************************************************************************
                   E X T E R N A L    D E C L A R A T I O N S 
 ****************************************************************************/

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

struct lconv *localeconv _PARMS((void));
char *setlocale _PARMS((int,const char *));


#pragma noSYS
#endif	/* _LOCALE_H */
