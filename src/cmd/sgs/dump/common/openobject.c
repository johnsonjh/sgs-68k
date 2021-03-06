#ident	"@(#)openobject.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

	

#include	<stdio.h>
#include	"filehdr.h"
#include	"ldfcn.h"
#include	"sgs.h"
#include	"filedefs.h"


FILELIST *
openobject(filename)

char		*filename;

{
    extern		fprintf( ),
			exit( );
    extern char		*calloc( );

    extern LDFILE	*ldopen( );
    extern int		ldaclose( );

    LDFILE		*objptr;
    FILELIST		*listptr;


    if ((objptr = ldopen(filename, NULL)) == NULL) {
	fprintf(stderr, "%sdump:  cannot open %s\n", SGS, filename);
	return(NULL);
    } 

    if (!ISCOFF(HEADER(objptr).f_magic)) {
	fprintf(stderr, "%sdump:  bad magic %s\n", SGS, filename);
	ldaclose(objptr);
	return(NULL);
    }

    if ((listptr = (FILELIST *) calloc(1, LISTSZ)) == NULL) {
	fprintf(stderr, "%sdump:  cannot allocate memory", SGS);
	exit(0);
    }

    listptr->file = filename;
    listptr->ldptr = objptr;
    listptr->nextitem = NULL;

    return(listptr);
}

/*
*/
