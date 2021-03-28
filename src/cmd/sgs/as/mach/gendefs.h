#ident	"@(#)gendefs.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	OLD MOT:gendefs.h	7.1		*/
/*	static	char	sccsid[] = "OLD MOT: gendefs.h: 2.4 6/23/83";			*/


#include <stdio.h>
#include "symbols.h"
#include "codeout.h"
#include "expand.h"
#include "instab.h"
#include "protos.h"

#define NO		0
#define YES		1

#define TESTVAL		-2


#define NCHPS		8	/* Non-flexname character limit.	*/
#define BITSPBY		8
#define BITSPOW		16
#define OUTWTYPE	unsigned short
#define OUT(a,b)	fwrite((char *)(&a),sizeof(OUTWTYPE),1,b)

#define SCTALIGN	4L	/* Byte alignment for sections (M68020).*/
#define TXTFILL		0x4e71	/* Text region "fill" nop instruction.	*/
#define FILL		0L
#define NULLSYM		((symbol *) NULL)
#define NSECTIONS 9
#define NFILES	NSECTIONS + 6	/* number of section files + other files */

/* index of action routines in modes array */

#define	NOACTION	0
#define	SETFILE		1
#define	LINENBR		2
#define	LINENUM		3
#define	LINEVAL		4
#define	DEFINE		5
#define	ENDEF		6
#define	SETVAL		7
#define	SETTYP		8
#define	SETLNO		9
#define	SETSCL		10
#define	SETTAG		11
#define	SETSIZ		12
#define	SETDIM1		13
#define	SETDIM2		14
#define	NEWSTMT		15
#define GENRELOC	16
#define BRLOC		17
#define BRAOPT		18
#define BCCOPT		19
#define BSROPT		20
#define ABSOPT		21
#define SWBEG		22
#define MOVE		23
#define IOPT		24
#define INSTI		25
#define ABSBR		26
#define NDXRELOC	27
#define IABSBR		28
#define BRBYT		29

#ifndef  M68881
#define NACTION		29
#else
#define CPBCCOPT	30
#define NACTION		30
#endif /*M68881*/

