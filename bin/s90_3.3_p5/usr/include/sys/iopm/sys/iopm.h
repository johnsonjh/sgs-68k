/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iopm.h: version 24.1 created on 10/28/91 at 17:59:55	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iopm.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef SYS_IOPM_H
#define SYS_IOPM_H

#define MAJOR_TBL           (uint)0xf8000800
#define MINOR_TBL           (uint)0xf8000880

#define OWN_STACK           (uint)0xf9000000		/* virtual address */
#define TCBSTACKS           (uint)0xfa000000		/* virtual address */
#define PHYSIO_WINDOW       (uint)0xfb000000		/* virtual address */

extern unsigned char   int_ctrl_reg;	/* set for IOPM in ml/build_addr.c */
extern unsigned char   led_ctrl_reg;	/* set for IOPM in ml/build_addr.c */

#endif /* SYS_IOPM_H */
