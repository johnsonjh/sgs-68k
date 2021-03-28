/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) stp_cdb.h: version 24.1 created on 10/28/91 at 17:54:37	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)stp_cdb.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/************************************************
*						*
*  scsi sequential device cdb definitions	*
*						*
************************************************/


#include	"cdb.h"

/************************************************
     command codes for Sequential device only
************************************************/

#define CDB_REWIND		0x01
#define CDB_READ_BLKLI		0x05
#define CDB_TRACK_SELECT 	0x0B
#define CDB_READ_REV		0x0F
#define CDB_WRITE_FILEMARKS 	0x10
#define CDB_SPACE 		0x11
#define CDB_TAPE_VERIFY 	0x13
#define CDB_REC_BUF_DATA	0x14
#define CDB_ERASE		0x19
#define CDB_LOAD 		0x1B
#define CDB_UNLOAD 		0x1B



/************************************************
     		CDB definitions
************************************************/
 
/* 
 *	read/write/write file mark
*/
#define xfer_len_msb	cdp0
#define xfer_len	cdp1
#define xfer_len_lsb	cdp2

 
/* lun_flags definitions */
#define	FIXED	1		/* fixed block size */
#define SILI	2		/* surpress illegal record size */

/*
 * load/unload cdb  
*/

#define reten_load 	cdp2



/* Load/unload */
#define UNL		0x00
#define	LD		0x01
#define RETEN		0x02	/* tension */

/* 
 *	erase cdb
*/
/* using g0_cdb */

/* lun_flags definitions */
#define ENTIRE	1		/* erase entire tape */


/* media type */
#define  EXABYTEMASK	0x80
/*density code */
#define  NRZI	0x01		/* 800 bpi for reel tape  */
#define  PE	0x02		/* 1600 bpi for reel tape */
#define  GCR	0x03		/* 6250 bpi for reel tape */
#define  QIC_11 0x04		/* 8000 bpi for cartridge tape */
#define  QIC_24 0x05		/* 8000 bpi for cartridge tape */
#define  DDPE	0x06		/* 3200 bpi for reel tape */
#define  QIC_120 0x0F		/* 10000 bpi for cartridge tape */
#define  QIC_150 0x10		/* 10000 bpi for cartridge tape */
#define  DAT 	 0x13		/* digital autdio tape */

/*
 * prevent/allow media removal
*/

#define prvnt 	cdp2

/* prvnt definition */
#define ALLOW		0x00
#define PREVENT		0x01

/*
 * read block limit
*/

/* using g0_cdb struct */

/* returning data format */
typedef struct {
	unchar	reserve;
	unchar  max_msb;
	unchar  max;
	unchar	max_lsb;
	unchar  min_msb;
	unchar	min_lsb;
} BLK_LIMIT;


/*
 * rewind
*/

/* using g0_cdb struct */

/* lun_flags definition */
#define	IMMED	1		/* status  is to be returned as soon as 
				   operation is initiated */
/*
 * space
*/


#define count_msb	cdp0
#define count		cdp1
#define count_lsb	cdp2

/* lun_code definitions */
/* space code */
#define S_BLK		0x00	/* space blocks */
#define S_FMK		0x01	/* space file marks */
#define S_SFM		0x02	/* space sequential file marks */
#define S_EOD		0x03	/* space to end of recorded data */

#define TAPE_SENSE_DATA_LEN  MODEHEAD_LEN+DSCPTR_LEN

/* sense data specific to xbyte */
#define remain_msb	resv[5]
#define remain_mid	resv[6]
#define remain_lsb	resv[7]
