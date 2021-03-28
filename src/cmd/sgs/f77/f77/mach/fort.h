/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) fort.h: version 25.1 created on 12/2/91 at 17:37:19	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)fort.h	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
/*	fort.h	7.1		*/
/*	machine dependent file  */

#ident	"@(#)f77/f77/mach:fort.h	25.1"

label( n ){
#if TARGET==M68K
	printf( "L%%%d:\n", n );
#else
  	printf( ".L%d:\n", n );
#endif
	}

tlabel(){
	cerror("code generator asked to make label via tlabel\n");
	}
