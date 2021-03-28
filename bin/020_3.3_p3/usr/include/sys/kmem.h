#ifndef	SYS_KMEM_H
#define SYS_KMEM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) kmem.h: version 24.1 created on 10/28/91 at 18:34:47	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)kmem.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifdef M68020
#include "sys/M68020/Mkmem.h"
#endif /* M68020 */

#ifdef M68040
#include "sys/M68040/Mkmem.h"
#endif /* M68040 */

#endif /* SYS_KMEM_H */
