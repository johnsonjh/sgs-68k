#ifndef SYS_IUNIT_H
#define SYS_IUNIT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iunit.h: version 24.1 created on 10/28/91 at 18:34:40	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iunit.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * iunit.h -- a structure to specify which unit for I/O
 *		used for communication between SPM and kernel
 */

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#define IUNIT_NO_DEV	(~(uint)0)	/* no such device or address	*/
#define IUNIT_NO_SNS	0xffff		/* no slot/sub-slot		*/
#define IUNIT_NONE	0xff		/* no such component		*/

typedef union iunit_u {
	struct iunit_s {
		uchar	slot;		/* CSS slot number		*/
		uchar	subslot;	/* sub-slot number (if any)	*/
		uchar	phys;		/* physical drive number	*/
		uchar	log;		/* logical device number	*/
	} s;
	struct iunit_w {
		ushort	sns;		/* Slot aNd Subslot		*/
		ushort	phys_log;
	} w;
	uint		i;
} iunit_t;

#endif /* SYS_IUNIT_H */
