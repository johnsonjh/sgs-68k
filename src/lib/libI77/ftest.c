/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ftest.c: version 25.1 created on 12/2/91 at 18:42:37	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ftest.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libI77:ftest.c	1.4"	*/
	
#ident	"@(#)libI77:ftest.c	25.1"

#include "fio.h"
#define FLOAT double
cilist x,y;
main(argc,argv) char **argv;
{	FLOAT dd[20];
	int ret[22];
	long one=1;
	int i,n,j;
	if(argc<2)
	{	fprintf(stderr,"%s infmt [outfmt] [n]\n",argv[0]);
		exit(1);
	}
	setcilist(&x,5,argv[1],0,1,1);
	argc--;
	argv++;
	if(argc<2 || argv[1][0]!='(')
		setcilist(&y,6,argv[0],0,1,1);
	else
	{	setcilist(&y,6,argv[1],0,1,1);
		argc--;
		argv++;
	}
	if(argc>=2) n=atoi(argv[1]);
	else n=1;
	j=0;
	ret[j++]=s_rsfe(&x);
	for(i=0;i<n;i++)
	{	ret[j++]=do_fio(&one,&dd[i],(long)sizeof(FLOAT));
	}
	ret[j++]=e_rsfe();
	for(i=0;i<j;i++) fprintf(stderr,"%d ",ret[i]);
	putc('\n',stderr);
	j=0;
	ret[j++]=s_wsfe(&y);
	for(i=0;i<n;i++)
		ret[j++]=do_fio(&one,&dd[i],(long)sizeof(FLOAT));
	ret[j++]=e_wsfe();
	for(i=0;i<j;i++) fprintf(stderr,"%d ",ret[i]);
	putc('\n',stderr);
	f_exit();
	exit(0);
}
