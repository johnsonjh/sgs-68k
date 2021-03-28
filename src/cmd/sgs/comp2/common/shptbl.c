#ident	"@(#)shptbl.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)shptbl.c	7.1		*/
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
