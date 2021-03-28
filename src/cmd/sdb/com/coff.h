/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) coff.h: version 23.1 created on 11/15/90 at 13:18:04	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)coff.h	23.1	11/15/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/com:coff.h	23.1"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
	/*	OLD MOT:coff.h	6.1		*/

/*		common object file format #include's */

/*	requires a.out.h (which is #included by head.h) */
#include "mach/sgs.h"
#define OMAGIC	AOUT2MAGIC	/* combined text and data segments */
#define NMAGIC	AOUT1MAGIC	/* separate text and data segments */
#define	PMAGIC	AOUT3MAGIC	/* Paging aout header magic number */

/*	 added for convenience */
#define ISTELT(c)	((c==C_MOS) || (c==C_MOU) || (c==C_MOE) || (c==C_FIELD))
#define ISREGV(c)	((c==C_REG) || (c==C_REGPARM))
#define ISARGV(c)	((c==C_ARG) || (c==C_REGPARM))
#define ISTRTYP(c)	((c==T_STRUCT) || (c==T_UNION) || (c==T_ENUM))
#define ISSTTAG(c)	((c==C_STRTAG) || (c==C_UNTAG) || (c==C_ENTAG))
#define ISSTACK(c)	((c==C_REG) || (c==C_REGPARM) || (c==C_ARG) || (c==C_AUTO))

#define MAXAUXENT	1	/* max number auxilliary entries */
