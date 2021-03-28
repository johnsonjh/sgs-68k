#ident	"@(#)systems.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	OLD MOT:systems.h	7.1		*/
/*	OLD MOT:systems.h	2.1		*/
/*	static	char	sccsid[] = "OLD MOT: systems.h: 2.4 7/1/83";			*/



#define M4ON		1
#define NODEFS		1
#define DEBUG		0
#define TRANVEC		0
#define DCODGEN		0
#define ONEPROC		1
#define FLDUPDT		1	/* "field update" is really complete
				** function information
				*/
#define VAX		0
#define MACRO		0
#define MULTSECT	1
#define MC68		1
#define FLEXNAMES	1

#define CM4DEFS 	"/usr/lib"
#define CM4TVDEFS 	"/usr/lib"


/* Extend size of span-dependent optimizations table */

/* define this directly in ../common/expand1.c where used */
/* define	MAXSS	350 */
