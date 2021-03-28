/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) bio.h: version 23.1 created on 11/15/90 at 13:18:01	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)bio.h	23.1	11/15/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/com:bio.h	23.1"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
	/*	bio.h: OLD MOT:bio.h	6.1  */

#define BRSIZ	512
struct brbuf {
	int	nl, nr;
	char	*next;
	char	b[BRSIZ];
	int	fd;
};
long lseek();
