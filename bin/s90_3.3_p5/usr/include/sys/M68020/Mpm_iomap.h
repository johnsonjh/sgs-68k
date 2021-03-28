#ifndef	SYS_M68020_MPM_IOMAP_H
#define SYS_M68020_MPM_IOMAP_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Mpm_iomap.h: version 24.1 created on 10/28/91 at 18:30:12	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Mpm_iomap.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#ifndef SYS_LIO_H
#include "sys/lio.h"
#endif

extern uint	*pm_int_req_regs[];

/* Defines for routines using the IO_SPACE of the PM20 */

#define	UintBits	32	/* number of bits in a uint */

/* How mapper works on addresses */ 
#define	PM_UnmappedBits	20	/* lower 20 bits don't get mapped */
#define	PM_MapInBits	10	/* next 10 bits get mapped */
#define	PM_MapOutBits	16	/* to 16 bits */

/* Layout of mapper register */
#define	PM_MapOutShft	16	/* offset of MapOut in map register */

#define PM_MapSlotShft	28	/* offset of sbus slot in map register */


#define	PM_IOMAP_SIZE	(KIO_SIZE / IOMAP_MANY) /* size of a mapped region */


#define	PM_IO_MAPPER	((uint *)PM_IOMAP_BASE)	/* array of mapping registers */
#define	PM_IO_SPACE	((unsigned char *)KIO_START)	/* mapped region */

#define Upper_Mbyte	0x0fff0000	/* value to place in map register to
					 * set upper megabyte as offset
					 */

#define	PM_MyMap		16	/* map that iomap uses */

#define PM_Int_Req_Reg_Index	17	/* map for use in clearing pending
					 * interrupts.  see iomap_init().
					 */

#define Int_Disp_Index		18	/* map entry dedicated to addressing
					 * the interrupt dispatcher, both for
					 * writing and ack'ing
					 */
#define PIOMAP_START_INDEX	200

#define	LowBits(nbits, value)	((value) & ((1 << (nbits)) - 1))
#define	HighBits(nbits, value)	((value) >> (nbits))

#define ICB_MAP_ENTRY	68	/* map entry number reserved for first ICB */
#define NEXT_ICB_MAP	16	/* offset in iomap to next ICB map pair */

/* Currently a define exists for 68040.  Create one for 68020 */
#define iomap_is_valid(X)	(1)	/* PM20 maps are always valid */

#endif /* SYS_M68020_MPM_IOMAP_H */
