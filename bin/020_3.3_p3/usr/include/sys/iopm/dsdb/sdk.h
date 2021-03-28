/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sdk.h: version 24.1 created on 10/28/91 at 17:50:27	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sdk.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* sdk.h -- the structure of scsi disk configuration table */

#include	"sys/cmn_err.h"

/* configuration table structure for winchester disk */
#define  STATIC	 static
#define  MAX_UNIX_SLICES	16

struct dcfg {
	unchar	dev_type;	/* medeia type	*/
	unchar	wp;		/* write protected indicator */	 
	unchar	blk_ratio;	/* system block size : drive block size */
	unchar	heads;		/* # of heads 			*/

	ushort	bps;		/* # of bytes per sector	*/
	ushort	intl;		/* interleave factor */

	uint	blk_len;	/* # of bytes per block		*/
	uint	cylns;		/* # of cylinders */
#ifdef OLD_DISK
	uint	start;		/* starting blk no. for data	*/
	uint	numblks;	/* # of blocks	*/
#else
	uint	lst_blkno;	/* last block number of drive */
#endif /* OLD_DISK */
	SLSIZE 	logdrv[MAX_UNIX_SLICES];	/* slice table	*/
} ;
typedef	struct dcfg	DCFG;

#ifdef OLD_DISK
struct log_st {
	uint	open_flags;
	uint	open_cnt;
};
typedef struct log_st LOG_STATUS;
#endif /* OLD_DISK */
 
struct dskiobuf
{
	int	b_flags;		/* device status flags */
	struct	buf *b_actf;		/* head of I/O queue */
	struct 	buf *b_actl;		/* tail of I/O queue */
	struct	buf *b_acts;		/* starting pointer of last group
					   in the I/O queue */
#ifdef C_SCAN
	struct	buf *b_acte;		/* for end of first sorting sweep */
#endif /* C_SCAN */
	unchar	b_dev;			/* device unit id */
	unchar  wait;			
	ushort	open_cnt;		/* open count */
	uint	qcnt;			/* number of request in the queue */
	int	b_outcnt;		/* outstanding request count */
	int	ot_flg[MAX_UNIX_SLICES]; /* open type for slice */
	CMD_BOX *cm[2];
	struct iotime *sar;
	unchar  pg1[32];		/* page 01 data buffer */
	uint	pg1_len;
	CMD_BOX ms_cm;			/* set page 01 command box */
	CMD_BOX *reset_q;		/* reset waiting queue */
	DCFG	cfg;
};


/* definition of b_flags  iobuf */
#define	B_ONCE	 0x01	/* flag for once only driver operations */
#define B_OPENIN 0x02	/* device is openning */
#define B_RMB	 0x04	/* removable media drive */
#define	B_WP 	 0x08	/* device is in format process */
#define B_NEWMDM 0x10	/* new media */
#define B_LOCKED 0x20	/* drive spin up flag	*/
#define B_ALLOC	 0x40 /* buffer has been allocated flag */
#define B_IBM	 0x80	/* IBM disk		*/
#define B_ODD	 0x0100	/* eraseable optical disk drive	*/
#define B_EDT	 0x0200	/* edt disk format 	*/
#define B_OLD	 0x0400 /* old iopm disk layout */
#define B_TNX	 0x0800  /* TEN X interface unit */
#define B_RESET  0x1000 /* Reset is processing */
#define B_NO_ERR_NEWMDIA 0x2000 /* don't report error if media changed */

/* definitions of b_outcnt	*/
#define CMB0_IN_USED	1   	/* command box 0 is in use */
#define CMB1_IN_USED	2	/* command box 1 is in use */
#define BOTH_IN_USED	3	/* This means two cmd boxes has been used */

/* macro definition */
#define	SLICE(x)	((x) & 0x0F)	/* get slice number */
#define GETDP(x)	&devtbl[x] /* get device table pointer */
#define	UNIT(x)		((x) >>4)&0x0F	/* get unit number	*/
#define	GET_PRA(x)	(((uint)(x)) >> 8) /* get parameters */
#define is_IBM_disk(x)	(x->b_flags&B_IBM)	/* verify vendor */
#define CHANID(x)	((x>>3)&0x01)	/* get channel id */
#define BUSID(x)	(x&0x07)	/* get bus id */
#define DISPLAY_DRV_ID(x) printf("Disk error on channel id %d bus id %d: ",CHANID(x),BUSID(x))

#define WORNING_DRV_ID(x) cmn_err(CE_NOTE, "Disk error on channel id %d bus id %d: ",CHANID(x),BUSID(x))
#define BLK_SHIFT	10

/* if buffer requested header is allocated by driver, then 2nd and 3rd bytes
   of b_flags will be the scsi command */
/* redifine  of b_flags in struct buf */
#define	SCSI_CMD	0xFF000000 /* scsi command byte */
#define SCSI_PRA	0x00FFFFFF /* parameters with scsi command */

/* sdkslice's function flags */
#define	SLICE_CLOSE	0x01
#define	SLICE_OPEN	0x02

/* redefine buf structure	*/
#define b_sblock av_back	/* physical block number */
#define b_cmd b_blkno		/* scsi command */



/* format parameters definition	*/
#define	INTERFACTOR	0	/* use default interleave factor */
#define	NO_FMTDATA	0	/* format with P & C */

#define IBM_SPT		48
#define IBM_BPS		512
#define IBM_CYLNS	949
#define IBM_HEADS	14

/* definition of timer */
#define INQUIRY_TIMER	(5 * HZ )	/* 2 seconds for inquiry command */
#define BASIC_TIMER	(15 * HZ) /* the basic timer is used for the command
				   without data xfer */
#define SENSE_TIMER	(5 * HZ + 1)	/* request sense command */
#define MODE_TIMER 	(5 * HZ + 1)	/* mode sense command */
#define RWUNIT_TIMER	(HZ/1000 + 1)	/* one block data transfer */
#define RDCP_TIMER 	(5 * HZ )	/* read capacity */
#define FORMAT_TIMER 	(30 * 60 * HZ + 1) /* 30 mins for format unit command */
#define START_TIMER	(60 * HZ + 1)	/* spin up timer */
#define	SSTEP	64		/* sorting range */
