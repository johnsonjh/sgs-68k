/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) llvl_drv.h: version 2.1 created on 4/17/90 at 14:01:18	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)llvl_drv.h	2.1	4/17/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/* file name: llvl_drv.h */

#define	DMA_PAGE		1024
#define	BLOCK_SIZE		1024

#define NOBUSIDS	8   /* number of bus id in each scsi bus */
#define NOINITIATORS	1	/* number of initiators in each bus */
#define NOLUNS		1	/* number of logic unit in each target */
#define NOCHANNELS	2	/* number of  chennels		*/

#define	MAXCHANTRGS	(NOBUSIDS - NOINITIATORS) /* number of targets */
#define MAXCHANDEVS	(MAXCHANTRGS*NOLUNS) /* number of devices in each channel */
#define MAXDEVS		(NOBUSIDS*NOCHANNELS)	/* max number of devices */

/* device control block definition */

struct dev_ctl_blk {
/* 0 */		struct dev_ctl_blk *next;	/* point to next item */
/* 4 */		ushort		   bus_id;	/* scsi bus id 		*/
/* 6 */		ushort		   reserved;
/* 8 */		struct scsi_ctb    *sc;	 /* SCSI BUS control table pointer */	
/* c */		uint 		   status; /* device status */
/* 10 */	uint 		   flags;  /* the scsi protocol supported flag */
/* 14 */	CMD_BOX 	   *cur_cmd; /* the current command */
/* 18 */	uint               cur_resid; /* the number of bytes not transfered when disconnect happen */
/* 1c */	uint		   save_resid; /* save pointer */
/* 20 */	uint		   timer_id; /* timer id		*/
/* 24 */	uint		   xferp;	/* transfer period */
/* 28 */	uint		   window; /* sync xfer window */
#ifdef TIME_DEBUG
/* 2c */	uint		   time_stamp;	/* time stamp */
/* 30 */	uint		   pc;
#endif /* TIME_DEBUG */
} ;

typedef struct dev_ctl_blk DCTB;

/* device flags definition	*/

#define DF_INIT_STATE		0x01	/* device initialization state */
#define DF_ABORT		0x02	/* request abort	*/
#define DF_RESET		0x04	/* device just been reset */
#ifdef TIME_DEBUG
#define DF_STAMPED		0x10	/* timer is running */
#endif /* TIME_DEBUG */

/* device status */
#define DS_NO_OPTMSG		0x01	/* target does not support optional
				           message		*/
#define DS_SYNC			0x02	/* device support sync transfer */

/* register set for each channel */
typedef struct {	
	SCSI_R	*sr;		/* NCR chip register set */
	uint	*bcr_w;
	uint	*pcr_w;
	uint	*pcr_r;
	uint	*mmr_r;
} CHAN_R;



/* scsi bus/dma control block definition	*/

struct scsi_ctb {
/* 0 */		unchar	chan_id;	/* channel ID	*/
/* 1 */		unchar  init_id;	/* initiator BUS ID */
/* 2 */		unchar	int_status;	/* ncr chip's interrupt status */
/* 3 */		unchar  status;		/* ncr chip's status */
/* 4 */		unchar	step;		/* ncr sequence step */
/* 5 */		unchar 	espcmd;		/* last ESP command	*/
/* 6 */		unchar 	sbyte;		/* target status byte */
/* 7 */		unchar	active;		/* active devices */

/* 8 */		short	msgout_len;	/* # bytes in msgout buffer */
/* a */		unchar 	msgout[6]; 	/* last massage been sent out */
/* 10 */	short	msgin_len;	/* # bytes in msgin buffer */
/* 12 */	unchar 	msgin[6];  	/* last received message */
/* 18 */	ushort	phase;		/* last phase	*/
/* 1a */	ushort	retry_cnt;	/* esp command retry count */

/* 1c */	uint 	flags;		/* flags , flow control */
/* 20 */	CMD_BOX *cm;		/* current mail box pointer   */
/* 24 */	DCTB 	*cur_dev; 	/* target's current pointer */
/* 28 */	unchar	**ptable_1;	/* page table pointer	*/
/* 2c */	unchar 	**ptable_2;	/* page table pointer	*/
/* 30 */	unchar	**cur_ptable;	/* current page table	*/
/* 34 */	uint	dma_count;	/* number of byte in dma transfer */
/* 38 */	uint	timer;		/* timer id	*/
/* 3c */	uint	nice_buf;	/* a 4 bytes buffer loacate at
					   long word boundary */
/* 40 */	uint	bcrcmd;		/* bcr command */

/* 44 */	unchar	*save_dmap;	/* save area for dma pointer */
/* 48 */	uint	save_dmac;	/* save dma count */
/* 4c */	Q_HDR  	ready_q; 	/* command ready queue */
/* 54 */	Q_HDR	abort_q;	/* device waiting abort */
/* 5c */	CHAN_R 	ch_r;		/* registers set */
};
typedef struct scsi_ctb SCSI_CTB;

/* define scsi controller bus id */
#define	DEFAULT_ID		0x00
/* scsi control flags */
#define SF_WAIT_DISC		0x0002	/* wait for disconnected */
#define SF_IGNR_ILLCMD		0x0008	/* ignore illegal command */

#define SF_BUS_BUSY		0x0010	/* bus is not  in free  phase */
#define SF_BLOCK		0x0020	/* block scsi bus, waiting
					   device doing reset */
/* the state of channel */
#define SF_CHAN_INITED		0x0800	/* SCSI has been initializeed */
#define SF_CHAN_UNINIT		0x0000	/* CHANNEL uninitialized	*/

#define SF_NORESEL		0x010000 /* single device on bus, initiator
					    does not want to support resel */
#define SF_CONN			0x020000 /* scsi bus connection has setup */
#define SF_DIFF			0x040000 /* differential mode */

/* mask of status */
#define SCB_STATE_MASK		0x0F00	

/* reset state bits mask */
#define SF_RESET_MASK		0xFFFFFE00

/* device response status definition,e.g. sbyte */

#define TRGT_GOOD		0x00	/* good condition */
#define TRGT_CHECK_CON		0x02	/* Any error,REQUEST SENSE condition
					   should be issued */
#define TRGT_CON_MET		0x04	/* The search data commands shall return
					   this status whenever a search con-
					   dition is satisfied */
#define TRGT_BUSY		0x08	/* Target is busy	*/
#define TRGT_INTERM		0x10    /* intermediate command response */
#define TRGT_RESERVE		0x18	/* logical unit conflict	*/

typedef struct {
	int	*pcrhi_w;
	int	*status;
} DUAL_REGS;

#define SCSI_CLK_RATE	50	   /* 50 ns per clock */
#define INIT_TIME	HZ/10000+1 /* initialize timer */
#define NCR_TCP		1000000000/HZ /* ns */
#define CCF	 	4 	/* clock convert factor for 20 MHZ cpu */
#define SEL_TIMEOUT	250000000/(8192*200)   /* select timeout 250 ms  */
#define XFER_CYCL	5 	/* sync xfer clock cycles */
#define XFER_PERIOD	((XFER_CYCL * SCSI_CLK_RATE)/4 + 1)/* sync xfer period = m * 4 ns */	
#define XFER_OFFSET	15
#define INIT_DEVICE_TIME (4*HZ+1)		/* initialize device timeout */
#define BLOCK_TIMEOUT	(HZ/50+1)  	        /* 20 ms	 	*/
#define BASIC_TIMEOUT	(10*HZ + 1)		/* 10 secs 	*/
#define SELECT_TIMEOUT  (HZ/2 + 1)		/* 500 ms 	*/
#define COMMAND_TIMEOUT SELECT_TIMEOUT+BASIC_TIMEOUT /* time slice for any 
							simple command without
							dma involved */
#define MIN_RECON_TIMER (30*HZ)			/* force to setup reconnect
						   in select operation, if
						   command timeout value is
						   great than MIN_RECON_TIMER
						 */
#define RESET_CHIP_DELAY	(HZ/50 + 1) /* chip reset delay time */
#define RESET_BD_DELAY	 	(HZ/50 + 1) /* chip reset delay time */
#define BUS_RESET_DELAY	 	(HZ*4 + 1) /* chip reset delay time */

/* page table setup parameters */
#define FULL_TABLE	128
#define HALF_TABLE	64

/* ncr fifo clean flag */
#define FF_C    0x00	/* clean fifo before stuffing */
#define FF_NC   0x01	/* do not clean fifo */

#define DELAY	 1	/* do time delay */
#define NO_DELAY 0	/* no time delay */
