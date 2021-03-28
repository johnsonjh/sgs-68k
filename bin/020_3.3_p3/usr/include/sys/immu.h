#ifndef	SYS_IMMU_H
#define	SYS_IMMU_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) immu.h: version 24.1 created on 10/28/91 at 18:33:36	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)immu.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/* convert bytes to megabytes */
#define MNUMSHFT	20
#define mnum(X)		((uint)(X) >> MNUMSHFT)
#define mb_to_b(X)	((uint)(X) << MNUMSHFT)

#ifdef	M68020
#include	"sys/M68020/Mimmu.h"
#endif	/* M68020 */

#ifdef	M68040
#include	"sys/M68040/Mimmu.h"
#endif	/* M68040 */


#endif	/* SYS_IMMU_H */
