/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sdkcfg.h: version 24.1 created on 10/28/91 at 17:51:00	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sdkcfg.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* sdkcfg.h -- definition of sector 0 of IOPM scsi disk	*/

typedef struct {

	uint	open_flag;	/* the logic drive status 	*/
	uint	offset;		/* starting block number	*/
	uint	noblk;		/* size of partition		*/
} OLD_SLSIZE;

typedef struct sector0  {
	unchar	vol_id[8];	/* volume ID	*/
	OLD_SLSIZE sltable[16];	/* slice table */
	ushort	open_flags;	/* indicate which slice is opened */
	uint	phyblkno;	/* physical block number for logical block
				   number 0 */
	uint	numblocks;	/* number of data blocks	*/
} SECTOR0;

#define	CONFIG_SECTOR	0	/* Sector where SECTOR0 lives */
