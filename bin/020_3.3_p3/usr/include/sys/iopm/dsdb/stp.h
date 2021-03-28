/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) stp.h: version 4.1 created on 5/2/90 at 19:38:37	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)stp.h	4.1	5/2/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#define ARCHIVE   2		/*Archive tape drive on line*/
#define TRACK9    4		/*9 track tape drive on line*/
#if	defined(EXABYTE)
#define XABYTE	  8		/* exabyte tape drive */
#endif	/* EXABYTE */

#define BLOCKSIZE	512

#if	defined(EXABYTE)
struct xb_device_table {
#else	/* EXABYTE */

#if	defined(TP9TRK)
struct hp_device_table {
#else
struct tp_device_table {
#endif  /* TP9TRK */
#endif	/* EXABYTE */
/* 0 */		uint	b_flags;
/* 4 */		uint	status;
/* 8 */		buf_t	*own_bp;	 /* internal used buffer header */
/* c */		unchar	type;	/* the device type should be 1 */
/* d */		unchar  subtype; /* nine track,catridge */
#if	defined(EXABYTE)

/* e */		unchar	mode_flags; /* tape density code */
/* f */		unchar  motion_thres; /* motion threshold */
/* 10 */	unchar  recon_thres; /* reconnect threshold */
/* 11 */	unchar  gap_thres; /* gap threshold */
/* 12 */	unchar  has_cache;	/* buffered mode indecation */
/* 13 */	unchar  tapemark; /* tape mark detected */
/* 14 */	unchar	reserved;
/* 15 */ 	dev_t	b_dev;
/* 16 */	unchar	pad;
#else	/* EXABYTE */
/* e */		unchar	density; /* tape density code */
/* f */		unchar  speed;		/* tape speed */
/* 10 */	unchar  has_cache;	/* buffered mode indecation */
/* 11 */	unchar  tapemark; /* tape mark detected */
/* 12 */	unchar	pad[2];
/* 14 */	ushort	blk_size; /* size of block only for fix length  drive */
/* 16 */ 	dev_t	b_dev;
#endif	/* EXABYTE */
/* 18 */	uint	minblklen; /* only used for nine track tape */
/* 1c */	uint	maxblklen; /* only used for nine track tape */
/* 20 */	buf_t	*b_actf;
/* 24 */	buf_t	*b_actl;
/* 28 */	buf_t	*outstanding;
/* 2c */	CMD_BOX *cm;
/* 30 */	CMD_BOX *cm_bkup;
/* 34 */	uint	qcnt;	/* number of requests */
/* 38 */	struct iotime	*sar;
};


/* bit definition of b_flags in device table */
#define	B_ONCE		1	/* device has not been initialize */
#define B_ALLOC		2	/* resource allocated */
#define	B_WP		4	/* media is write protect */
#define	B_BSY		8	/* device busy */
#define	B_NO_PRINT	0x10	/* do not print message if unit attention err */
#define B_SETTP		0x20 	/* request set tape media */
#define B_DENSITY	0x40 	/* flag for density change if settape set wrong
				   value, this flag use to prevent locking */



/* definition of tapemark */
#define TM_EOM			0x01
#define TM_DETECTED		0x02
#define TM_REPORTED		0x04
#define TM_NOT_DETECTED		0x08

#define NO_CACHE	0
#define DO_CACHE	1


/* local macro defination */


#define CHANID(x)	((x>>3)&0x01)	/* get channel id */
#define BUSID(x)	(x&0x07)	/* get bus id */
#define	UNIT(x)		(x >>4)&0x0F	/* get unit number	*/

/* bit definition in minor device number */
#define	ROC		1	/* rewind on close */
#if	!defined(EXABYTE)
#define QIC120_XDT 	2	/* QC_120 format with 600XDT tape */
#endif	/* EXABYTE */


#if	defined(EXABYTE)

#define	BASIC_TIME_SLICE	(60*HZ+1)
#define	UNIT_TIME_SLICE		(60*HZ+1)
#define	REWIND_TIME_SLICE	(20*60*HZ+1)
#define SKIP_TIME_SLICE		(30*60*HZ+1)
#define RDWR_TIME_SLICE		(40*HZ+1)
#define BUSY_TIME_SLICE		(2*HZ+1) /* every 2 secs check drive is busy */

#define GETDP(x)		&sxbtab[x]

#define	DEVICE_TABLE		xb_device_table
#define	TAPE_TAB		sxbtab
#define	TAPE_INTR		sxbintr
#define TAPE_START		sxbstart	
#define	TAPE_DISPLAY_ERR	sxb_display_err
#define TAPE_OPEN		sxbopen
#define	TAPE_CLOSE		sxbclose
#define	TAPE_IOCTL		sxbioctl
#define	TAPE_STRATEGY		sxbstrategy
#define TAPE_COMMAND		sxbcommand

#define LOG_TAPE		0xa0

#else	/* EXABYTE */

/* definition of state */
#define ST_NORMAL		0
#define ST_WAIT_UNIT_READY	1
#define ST_WAIT_REWIND		2
#define ST_WAIT_SELECT		3

/* nine track tape speed code */
#define	 DENS_DEFAULT	0	/* device's default or only density */
#define  DENS_800	1
#define  DENS_1600	2
#define  DENS_6250	3
#define  DENS_3200	6

/* speed */
#define	DEFAULT_SPEED	0	/* for catridge tape	*/ 
#define LOW_SPEED	1	/* 50 ips ,start/stop mode */
#define	HI_SPEED	2	/* 100 ips, streaming mode */

#define	BASIC_TIME_SLICE	(20*HZ+1)
#define	UNIT_TIME_SLICE		(20*HZ+1)
#define	REWIND_TIME_SLICE	(5*60*HZ+1)
#define SKIP_TIME_SLICE		(15*60*HZ+1)
#define RDWR_TIME_SLICE		(8*HZ+1)	/* 1k data */
#define BUSY_TIME_SLICE		(HZ+1)

#define	LOG_TAPE		0x10

#if	defined(TP9TRK)

#define GETDP(x)		&hptab[x]
#define	DEVICE_TABLE		hp_device_table
#define	TAPE_TAB		hptab
#define	TAPE_INTR		hpintr
#define TAPE_START		hpstart	
#define	TAPE_DISPLAY_ERR	hp_display_err
#define TAPE_OPEN		hpopen
#define	TAPE_CLOSE		hpclose
#define	TAPE_IOCTL		hpioctl
#define	TAPE_STRATEGY		hpstrategy
#define	TAPE_COMMAND		hpcommand

#else /* TP9TRK */

#define GETDP(x)		&stptab[x]
#define	DEVICE_TABLE		tp_device_table
#define	TAPE_TAB		stptab
#define	TAPE_INTR		stpintr
#define TAPE_START		stpstart	
#define	TAPE_DISPLAY_ERR	stp_display_err
#define TAPE_OPEN		stpopen
#define	TAPE_CLOSE		stpclose
#define	TAPE_IOCTL		stpioctl
#define	TAPE_STRATEGY		stpstrategy
#define	TAPE_COMMAND		stpcommand

#endif  /* TP9TRK */
#endif	/* EXABYTE */

