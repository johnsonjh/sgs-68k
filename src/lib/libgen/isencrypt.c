/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) isencrypt.c: version 25.1 created on 12/2/91 at 19:32:52	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)isencrypt.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libgen:isencrypt.c	1.1"		*/

#ident	"@(#)lib/libgen:isencrypt.c	25.1"

#define FBSZ 64
char *getenv();

/*
 * function that uses heuristics to determine if
 * a file is encrypted
 */

isencrypt(fbuf, ninbuf)
register char *fbuf;
int ninbuf;
{
	register char *fp;
	char *p;
	int crflag = 0;
	int i;
	if(ninbuf == 0)
		return 0;
	fp = fbuf;
	while (fp < &fbuf[ninbuf])
	/* Check if file has non-ASCII characters */
		if(*fp++ & 0200) {
			crflag = 1;
			break;
		}
	if(crflag == 0)
		/* If all characters are ASCII, assume file is cleartext */
		return(0);
	p = getenv("CHRCLASS");
	if(p == 0 || *p == 0 || strcmp(p, "ascii") == 0) 
	/*
	 * If CHRCLASS is unset, null, or ascii, and non-ascii
	 * characters are in file, assume file is encrypted
	 */
		return(1);
	if(ninbuf >= 64){
		/*
		 * We are in non-ASCII environment; use
		 * chi-square test to determine if file 
		 * is encrypted if there are more
		 * than 64 characters in buffer.
		 */

		int bucket[8];
		float cs;

		for(i=0; i<8; i++) bucket[i] = 0;

		for(i=0; i<64; i++) bucket[(fbuf[i]>>5)&07] += 1;

		cs = 0.;
		for(i=0; i<8; i++) cs += (bucket[i]-8)*(bucket[i]-8);
		cs /= 8.;

		if(cs <= 24.322)
			return 1;
		return 0;
	}

	/* 
	 * If file has nulls, assume it is encrypted
	 */
	
	for(i = 0; i< ninbuf; i++) 
		if(fbuf[i] == '\0')
			return(1);

	/* 
	 * If last character in buffer is not a new-line,
	 * assume file is encrypted
	 */
	
	if(fbuf[ninbuf - 1] != '\n')
		return 1;
	return 0;
}
