/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ctest.c: version 25.1 created on 12/2/91 at 18:42:00	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ctest.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libI77:ctest.c	1.4"	*/
	
#ident	"@(#)libI77:ctest.c	25.1"

#include "stdio.h"
char buf[256];
main()
{	int w,dp,sign;
	char *s;
	double x;
	for(;;)
	{
		scanf("%d %lf",&w,&x);
		if(feof(stdin)) exit(0);
		s=fcvt(x,w,&dp,&sign);
		strcpy(buf,s);
		printf("%d,%f:\t%d\t%s\n",w,x,dp,buf);
		s=ecvt(x,w,&dp,&sign);
		printf("\t\t%d\t%s\n",dp,s);
	}
}
