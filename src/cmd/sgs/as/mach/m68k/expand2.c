#ident	"@(#)expand2.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	OLD MOT:expand2.c	7.1			*/
/*
static	char	sccsid[] = "OLD MOT: expand2.c 7.1 5/30/85";
/* static	char	sccsid[] = "OLD MOT: expand2.c: 2.3 6/30/83"; */



#include	"expand2.h"
#include	"gendefs.h"

rangetag	range[NITYPES] =
		{
			{-32759L,32759L},	/* SDI1   BRA	*/
			{-32759L,32759L},	/* SDI2   BCC	*/
			{-32759L,32759L},	/* SDI3   BSR	*/
			{-32759L,32759L},	/* SDI4   ABS	*/
			{-32759L,32759L},	/* SDI5   IS	*/
			{-32759L,32759L}	/* SDI6   IL	*/
		};

char	pcincr[NITYPES] =
	{
		4,
		4,
		4,
		4,
		6,
		8
	};

char	idelta[NITYPES] =
	{
		2,
		2,
		2,
		2,
		2,
		2
	};
