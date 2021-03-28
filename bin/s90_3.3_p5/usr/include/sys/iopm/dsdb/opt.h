/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) opt.h: version 24.1 created on 10/28/91 at 17:53:10	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)opt.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* opt.h -- the structure of scsi optical disk configuration table */

#include	"sys/cmn_err.h"


/* if lsb of minor number is set, then eject on last close */
#define	OPT_EOC	1

struct	optcfg {
	unchar	wp;		/* write protected indicator */	 
	unchar	blk_ratio;	/* system block size (1k) : drive block size */
	uint	blk_len;	/* # of bytes per block		*/
	uint	lst_blkno;	/* last block number of drive */
};
typedef struct optcfg OPTCFG;

struct optiobuf {
	int	b_flags;	/* device status flags */
	struct	buf *b_actf;	/* head of I/O queue */
	struct	buf *b_actl;	/* tail of I/O queue */
	struct	buf *b_acts;	/* starting pointer of last group
					in the I/O queue */
#ifdef C_SCAN
	struct	buf *b_acte;	/* for end of first sorting sweep */
#endif	/* C_SCAN */
	unchar	unit_id;		/* device unit id */
	unchar	wait;
	ushort	open_cnt;      /* open count */
	uint	qcnt;       	/* number of request in the queue */
	int	b_outcnt;	/* outstanding request count */
	int	ot_flg;		/* open type for media */
	CMD_BOX	*cm[2];
	struct	iotime *sar;
	unchar	pg1[32];     /* page 01 data buffer */
	uint  pg1_len;
	CMD_BOX	ms_cm;       /* set page 01 command box */
	CMD_BOX	*reset_q;    /* reset waiting queue */
	OPTCFG	cfg;
};

/* definition of b_flags  iobuf */
/* driver status flags */
#define B_ONCE		0x0001	/* flag for once only driver operations */
#define B_OPENING	0x0002	/* device is opening */
#define B_LOCKED	0x0004	/* drive spin up flag	*/
#define B_ALLOC	0x0008	/* buffer has been allocated flag */
#define B_RESET	0x0010	/* Reset being processed */
#define B_FMT	0x4000	/* Format being processed */
#define B_NEW_MED_OK 0x0020	/* don't report error if media changed */
/* drive info */
#define B_RMB		0x0040	/* removable media drive */
#define B_WP		0x0080	/* media is write protected */
#define B_NEWMDM	0x0100	/* new media */
#define B_ERS		0x0200 	/* Multifunction drive unit with erasable medium */
#define B_SONY_MF	0x0400 	/* Sony multifunctional drive */
#define B_MAXTOR	0x0800 	/* Maxtor drive */
/* user settings */
#define B_SWAP		0x1000	/* byte swap data on read */
#define B_EOC		0x2000	/* eject on close */

/* definitions of b_outcnt	*/
#define CMB0_IN_USE	1	/* command box 0 is in use */
#define CMB1_IN_USE	2	/* command box 1 is in use */
#define BOTH_IN_USE	3	/* two cmd boxes are in use */

/* macro definition */
#define GETDP(x)	&opttbl[x]		/* get device table pointer */
#define UNIT(x)		((x) >>4)&0x0F		/* get unit number	*/
#define GET_PRA(x)	(((uint)(x)) >> 8)	/* get parameters */
#define CHANID(x)	((x>>3)&0x01)		/* get channel id */
#define BUSID(x)	(x&0x07)		/* get bus id */
#define DISPLAY_DRV_ID(x) printf("Optical Drive error on channel %d bus id %d: ",CHANID(x),BUSID(x))

#define WARNING_DRV_ID(x) cmn_err(CE_NOTE, "Worm Drive error on channel id %d bus id %d: ",CHANID(x),BUSID(x))
#define BLK_SHIFT	10

/* if buffer requested header is allocated by driver, then 2nd and 3rd bytes
   of b_flags will be the scsi command */
/* redefine  of b_flags in struct buf */
#define SCSI_CMD	0xFF000000 /* scsi command byte */
#define SCSI_PRA	0x00FFFFFF /* parameters with scsi command */

/* redefine buf structure	*/
#define b_sblock av_back	/* physical block number */
#define b_cmd b_blkno		/* scsi command */

/* definition of timer */
#define INQUIRY_TIMER	(5 * HZ )	/* 2 seconds for inquiry command */
#define BASIC_TIMER	(15 * HZ) /* the basic timer is used for the command
				   without data xfer */
#define SENSE_TIMER	(5 * HZ + 1)	/* request sense command */
#define MODE_TIMER 	(5 * HZ + 1)	/* mode sense command */
#define RWUNIT_TIMER	1	/* one block data transfer */
#define RDCP_TIMER 	(5 * HZ )	/* read capacity */
#define START_TIMER	(60 * HZ + 1)	/* spin up timer */
#define SONY_FORMAT_TIMER	(60 * 60 * HZ + 1)	/* spin up timer */
#define SSTEP	64		/* sorting range */
