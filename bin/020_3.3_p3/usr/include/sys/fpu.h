#ifndef SYS_FPU_H
#define SYS_FPU_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) fpu.h: version 24.1 created on 10/28/91 at 18:32:38	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)fpu.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/


#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif


/* The following is information specific to the M68881 floating point chip.
** WARNING: If this file is changed and your kernel selects M68881 support,
** 	    you are advised to build the kernel again from scratch, as this
**	    file is included by file "user.h", which is used everywhere in
**	    the kernel!.
*/

/* bit masks for various exceptions in M68881 %status, %control registers */
#define EXC_MASK	0xff00
#define EXC_INX1	0x0100
#define EXC_INX2 	0x0200
#define EXC_DZ 		0x0400
#define EXC_UNFL	0x0800
#define EXC_OVFL 	0x1000
#define EXC_OPER 	0x2000
#define EXC_SNAN	0x4000
#define EXC_BSUN 	0x8000

/* bit masks for condition codes in M68881's %status register */

#define FPCC_NAN	0x01000000
#define FPCC_I		0x02000000
#define FPCC_Z		0x04000000
#define FPCC_N		0x08000000

/* bit sets for rounding mode and precision in 881's %control register */
#define RM_MASK		0x30
#define RM_NEAR		0x00
#define RM_ZERO		0x10
#define RM_NGINF	0x20
#define RM_PSINF	0x30
#define RP_MASK		0xc0
#define RP_X		0x00
#define RP_S		0x40
#define RP_D		0x80

	typedef long freg[3];   /* 4*3 bytes or 96 bits per FPi */

	typedef struct {	/* M68881 programmer's model */
	       freg  reg[8];	/* holds fp0..fp7 */
	       long  control,
		     status,
		     iaddr;	/* 32-bit 881 system registers */
	       } fregs;

/* The following are masks used in looking at the "ustate" status word
** in the "sys_info" structure.  They indicate the condition of 881
** info. inside the u-vector.
*/
#define UFPRWRT 1		/* Flags the need to update the 881 programmer's
				** model from the u-vector values before going 
				** through normal context switch processing.
				*/

	typedef struct {
	       ulong  ustate;	/* Flag word to remember condition of 881
			    	** info inside the u-vector.
				** (offset 0) */
	       freg   excop;	/* Set on exceptions to the value of the 
				** offending EXCeptional OPerand, if relevant.
				** (offset 4) */
	       fregs regs;	/* holds fp0..fp7, control, status, iaddr 
				** (offset 16 ) */
	       uchar fsave[184];/* area where latest FSAVE info (881 internal
				** state info) is kept.  Put last because if
				** it is trashed, it is least likely to contain
				** useful info.
				** (offset 124) */
	       } fpu_info;


#define	FPU_VERSION	0
#define	FPU_FORMAT	1

#define	FPU_VERSION_NULL 0x00

#ifdef	M68020

#define	FPU_FORMAT_IDLE	0x18
#define	FPU_FORMAT_BUSY	0xb4

#endif	/* M68020 */

#ifdef	M68040

#define	FPU_FORMAT_IDLE		0x00
#define	FPU_FORMAT_UNIMP	0x28
#define	FPU_FORMAT_BUSY		0x60

#endif	/* M68040 */



#endif /* SYS_FPU_H */
