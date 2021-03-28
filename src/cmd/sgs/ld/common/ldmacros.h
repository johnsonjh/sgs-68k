#ident	"@(#)ldmacros.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

	

#define MAXKEEPSIZE	1024
#define SYM_EQUATE_LEN 40		/* max length of symbolic equate */ 

extern char	stat_name[];
#if FLEXNAMES
#define FLX_NAME(x)	((x) == 0L || (x) == -1L )
#define	PTRNAME(x)	((x)->n_zeroes == 0L || (x)->n_zeroes == -1L ) \
				? (x)->n_nptr \
				: strncpy( stat_name, (x)->n_name, 8 )
#define SYMNAME(x)	(x.n_zeroes == 0L || x.n_zeroes == -1L) \
				? x.n_nptr \
				: strncpy( stat_name, x.n_name, 8 )
#else
#define PTRNAME(x)	strncpy( stat_name, (x)->n_name, 8 )
#define SYMNAME(x)	strncpy( stat_name, x.n_name, 8 )
#endif

#define OKSCNNAME(x)	((tvflag && (!rflag || aflag)) || strncmp( x, _TV, 8 ))
