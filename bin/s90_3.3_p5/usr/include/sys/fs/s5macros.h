#ifndef SYS_FS_S5MACROS_H
#define SYS_FS_S5MACROS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) s5macros.h: version 24.1 created on 10/28/91 at 18:32:57	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)s5macros.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/fs/s5macros.h	10.1"	*/


#define FsBSIZE(bsize)		BSIZE
#define FsBSHIFT(bsize)		BSHIFT
#define FsNINDIR(bsize)		NINDIR
#define FsBMASK(bsize)		BMASK
#define FsINOPB(bsize)		INOPB
#define FsSECINOPB(isize)	(SECINOPB(isize))
#define FsLTOP(bsize, b)	((bsize==0x1000) ? b<<2: b)
#define FsPTOL(bsize, b)	((bsize==0x1000) ? b>>2: b)
#define FsNMASK(bsize)       	((bsize==0x1000) ? 01777: 0377)
#define FsNSHIFT(bsize)         ((bsize==0x1000) ? 10: 8)

#define FsITOD(bsize, x)	(daddr_t)(((unsigned)(x)+(2*INOPB-1))>>INOSHIFT)
#define FsITOO(bsize, x)	(daddr_t)(((unsigned)(x)+(2*INOPB-1))&(INOPB-1))
#define FsINOS(bsize, x)	(((x)&~(INOPB-1))+1)
#define FsSECITOD(isize, x)    (daddr_t)(((unsigned)(x)+(2*SECINOPB(isize)-1)) \
	>>SECINOSHIFT(isize))
#define FsSECITOO(isize, x)    (daddr_t)(((unsigned)(x)+(2*SECINOPB(isize)-1)) \
	&(SECINOPB(isize)-1))
#define FsSECINOS(isize, x)	(((x)&~(SECINOPB(isize)-1))+1)

#endif /* SYS_FS_S5MACROS_H */
