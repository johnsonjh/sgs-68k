/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) shptbl.c: version 25.1 created on 12/2/91 at 17:43:26	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)shptbl.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sgs/comp2/common:shptbl.c	1.1"

/*	OLD MOT: shptbl.c	7.1		*/
# ifndef NSTYSHPS
# define NSTYSHPS 15000
# endif

typedef struct {
	int	sop,
		sleft,
		sright,
		ssh,
		scost,
		scnt;
	char	shname[8];
} STYSHP;

STYSHP	shp[NSTYSHPS];
