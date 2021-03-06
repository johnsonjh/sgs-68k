#ident	"@(#)ldshread.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libld:common/ldshread.c	1.3"	*/
	

#include	<stdio.h>
#include	"filehdr.h"
#include	"scnhdr.h"
#include	"ldfcn.h"

int
ldshread(ldptr, sectnum, secthdr)

LDFILE	*ldptr;
unsigned short	sectnum;
SCNHDR	*secthdr; 

{
    extern int		fseek( );
    extern int		fread( );

    extern int		vldldptr( );

    if (vldldptr(ldptr) == SUCCESS) {
	if ((sectnum != 0) && (sectnum <= HEADER(ldptr).f_nscns)) {
	    if (FSEEK(ldptr,
		FILHSZ + HEADER(ldptr).f_opthdr + (sectnum - 1L) * SCNHSZ,
		BEGINNING) == OKFSEEK) {
		if (FREAD(secthdr, SCNHSZ, 1, ldptr) == 1) {
		    return(SUCCESS);
		}
	    }
	}
    }

    return(FAILURE);
}

