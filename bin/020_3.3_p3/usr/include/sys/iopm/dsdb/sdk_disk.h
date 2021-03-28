/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sdk_disk.h: version 24.1 created on 10/28/91 at 17:50:34	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sdk_disk.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* sdk_disk.h - iopm scsi disk layout	*/

/*********************************************************
*							 *
* The following disk layout is for the IOPM SCSI disk    *
* drive.						 *
* Some of the special sections which were defined in EDT *
* disk are either gone or included in disk slices.  The  * 
* boot image and defect list become disk slices,         *
* while the skip track list and spare sector table no    *
* longer exist.						 *
* The number of slices is extended to 32. However, only  *
* the first 16 slices are visiable under the UNIX        *
* environment.				 	         * 
* The slice entries with all 0s is a empty slice.         *
* The default boot image slice is the first slice with  *
* LD_BOOT_SLICE type (5).			         *
*							 *
*********************************************************/

#define MAX_IOPM_SLICES	32	/* max number of slices */
#define	CONFIG_BLOCK	0	/* Block where BLOCK0 lives */
#define CONFIG_BLOCK_BKUP 1	/* backup BLOCK0 */
#define FIRST_DATA_BLOCK_NUM 2	/* first user block number */
#define ENTRY_LEN	16	/* slice table entry length */
/* default defect list slice */

#define DEF_DEFCT_SLICE	16
#define DEF_DEFCT_LIST	FIRST_DATA_BLOCK_NUM	
#define DEF_DEFCT_SIZE	10

#define VERSION_DATE	0x19890828 /* version number */
#define BASE_DATE	0x19890828 /* base version number */

/*	disk layout diagram	*/

/*---------------------------------------------------+
| block 0   |           config block                 | 
|-----------|----------------------------------------|
| block 1   |           config block                 |
|           |           (for backup use)             |
|-----------|----------------------------------------|
| block 2   |                                        |
| to end of |           disk slices area             |
|   disk    |                                        |
+----------------------------------------------------*/

/* block 0 structure	*/

struct sdk_block_0  {
/* miscellaneous disk information */
	union {
		unchar sec1[512];		/* reserve this section
						   to 512 bytes */
		struct {
			unchar	vol_id[8];	/* volume ID "IOPMDISK"	*/
			uint	date; 		/* 0xyyyymmdd	*/
			ushort	cylns;		/* number of cyls on the disk */
        		ushort  bps;		/* number of bytes per sector */
        		unchar	heads;		/* number of heads */
		} disk_info;
	} misc;

/* slice table */
	union {
		unchar entry[ENTRY_LEN]; /* reserve 16 byte for each entry */
		struct  slice_entry {

			uint	ld_strt; /* start block #	*/
			uint	ld_size; /* number of blocks    */
			unchar	ld_type; /* slice type		*/
			unchar	ld_spt;	 /* sector per track    */
		} ldrv;
	} slice_table[MAX_IOPM_SLICES];
};


/* ld_type definition	*/

#define LD_UNUSED_SLICE	0	/* disk partition for none used slice   */
#define	LD_UNIX_SLICE	1	/* data section for UNIX system	        */
#define LD_SWAP_SLICE	2	/* swap device				*/
#define LD_DIAG_SLICE	3	/* diagnostic working area		*/
#define LD_DEFCT_SLICE	4	/* disk bad block list			*/
#define LD_BOOT_SLICE	5	/* boot image				*/
#define LD_ERR_LOG	6	/* error log slice			*/
