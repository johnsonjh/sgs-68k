/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) nsquery.c: version 25.1 created on 12/2/91 at 19:43:02	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)nsquery.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libns:nsquery.c	1.2"	*/
	
#ident	"@(#)libns:nsquery.c	25.1"

#include  <stdio.h>
#include  "nserve.h"

extern	int	errno;
extern	int	optind,	opterr;
extern	char	*optarg;

main(argc,argv)
int	argc;
char	*argv[];
{


	int	chr;
	int	errflg;
	int	hflg;
	char	*name = "*";
	char 	*usage = "nsquery [-h] [<name>]";


	errflg = hflg = 0;
	if (argc != 1) {
		while ((chr = getopt(argc,argv,"h")) != EOF)
			switch(chr) {
	 		case 'h':
				hflg++;	
				break;
			case '?':
				errflg++;
				break;
			}
	}
	if (argv[optind] != NULL)
		name = argv[optind];
	
	if (errflg) {
		fprintf(stderr,"%s\n",usage);
		exit(1);
	}

	if (!hflg)
		fprintf(stdout,"   Resource               Description                Server\n");

	if (ns_info(name) == FAILURE) {
		fprintf(stderr,"nsquery: error\n");
		exit(1);
	}
}
