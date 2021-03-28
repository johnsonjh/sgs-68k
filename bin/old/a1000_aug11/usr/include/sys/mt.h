#ifndef SYS_MT_H
#define SYS_MT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mt.h: version 2.1 created on 5/22/89 at 19:08:58	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mt.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys:mt.h	2.1"

/* Special Peripheral Device ioctl */

#define PERIOC  ('P'<<8)
#define PERGET  (PERIOC|0) 	/* get the tape drive type */
#define PERSET  (PERIOC|1)

/*Responses from ioctl call PERGET */

#define ARCHIVE   2		/*Archive tape drive on line*/
#define TRACK9    4		/*9 track tape drive on line*/

	/* TAPE DRIVE COMMANDS */

#define READ		1
#define WRITE 		2
#define REWIND 	   	3
#define WEOF 	    	4		/*write end of file*/
#define REOF 	    	5		/*read end of file*/
#define TAPESTAT  	6		/*get tape status */
					/*tape drive type is returned in count*/
#define TENSION   	7		/*not available on 9track*/
#define ERASE    	8		/*for archive, open the tension on*/
					/* open device for writing. */
					/*for 9track, use ioctl call (PERSET) */
					/*putting the value of ERASE in the */
				        /*argument. */
#define RECFWD      	0xa		/*forward one record (9track)*/
#define RECBACK     	0xb		/*back one record (9track)*/
#define QIC11     	0xd		/*change to QIC11 format (archive)*/
#define QIC24       	0xe		/*change to QIC24 format (archive)*/
#define DENSITY     	0xf
#define TAPESPEED   	0x10		/*(9track)*/
#define TAPE_MODE   	0x11		/*set 9track to start/stop or stream */
					/* mode using ioctl call (PERSET).*/
					/* OR the value SET_MODE with the */
					/* value for the desired mode and put*/
					/* it in the argument */
#define REVSFM		0x12		/* reverse to last filemark */
#define RESET9     	0x13		/* reset the nine track tape drive to
					   clear the EOM bit */
#define LASTSTAT   	0x14		/* return tape status from last tape
					   operation */
#define DRIVETYPE 	0x15		/* return drive type TRACK9 or ARCH */

	/*Other tape bit definitions */

#define TAPECMD     	(4<<13)
#define ROC	      	0x40		/*rewind on close*/

/*9TRACK ONLY*/
#define DENSITY_BIT   	8		/*high density bit in minor dev*/
#define DENSITY_MASK  	4		/*density bits in minor device
					  00 - 1600 bpi
					  01 - no density required
					  10 - 3200/6250 bpi
					*/
#define TAPE_CACHE    	0x4		/*tape drive has on board cache memory*/
#define SPEED_BIT     	0x80
#define SET_MODE      	0x2000		/*set the 9 track mode*/
#define START_STOP    	1		/*used with SET_MODE for 9track*/
#define STREAM        	2		/*used with SET_MODE for 9track*/
#define DENS_1600     	1		/*set tape density to 1600bpi*/
#define DENS_6250     	2		/*set tape density to 6250bpi*/
#define DENS_800      	4		/*set tape density to 800bpi*/
#define TAPE_SPEED    	0X4000		/*set the nine track tape speed*/
#define HI_SPEED      	1		/*set tape speed to high*/
#define LOW_SPEED     	2		/*set tape speed to low*/
#define MAX_TP_DRV    	32		/*maximum number of 9track tape drives*/

/*ARCHIVE ONLY*/
#define TENONOP       	0x80		/*tension on open */
#define DRVPERBD      	4
#define FORMAT_BIT    	8		/*Qic11/24 format bit in minor dev*/
#define T150M_BIT	4	       /*this minor device is for 150Mb drive*/ 
#define MAXTAPE       	16		/* maximum number of tape drives  */
					/* per system*/

	/*
	 * TAPE DRIVE RETURN CODES - these are the values found in rc1 and rc2,
	 *			     and also returned to the user via the
	 *			     TAPESTAT, and LASTSTAT ioctl's.
	 */

	/*
	 * changes here must be reflected in hsdterror.h
	 * and visa versa
	 */

#define TP_NOTAPE	0x30	/* cartridge not in place */
#define TP_NRDY		0x31	/* tape drive not ready */
#define TP_WPROT	0x32	/* write protected */
#define TP_EOM		0x33	/* end of media */
#define TP_DATER	0x34	/* unrecoverable data err */
				/* or BIE(block in error) not located */
				
#define TP_FMD		0x35	/* file mark detected */
#define TP_NDATA	0x36	/* no data detected */
#define TP_8RETRY	0x37	/* 8 or more retries */
#define TP_BOM		0x38	/* beginning of media */
#define TP_HANG		0x39	/* tape drive is hung */
#define TP_RSET		0x3a	/* tape drive was reset */
#define TP_NONL		0x3b	/* tape not online */
#define TP_OVRUN	0x3c	/* data overrun, tapeblk bigger than request*/
#define TP_CDATER	0x3d	/* 9 track, error during write operation */
#define TP_ILLCMD	0x3e	/* illegal tape cmd */
#define TP_INVIF	0x3f	/* invalid io interface */
#define TP_ABORT	0x40	/* tape request abort,due to ups */
#define TP_NONINE	0x41    /* no nine track tape on this controller */
#define TP_NOBRST	0x42    /* no burst id detected */
#define TP_DENS		0x43    /* invalid density selection */
#define TP_SPEED	0x44    /* invalid tape speed selection */
#define TP_MODE		0x45    /* invalid tape mode selection */
#define TP_BUSY		0x46	/* tape drive is in the process of performing
				   a read/write request */
#define TP_MEM		0x47	/* invalid main memory address */
#define TP_CNT		0x48	/* invalid byte count in request */


#define LAST_TAPE_ERROR		0x48	/* used by driver for error msgs */
#define FIRST_TAPE_ERROR	0x30	/* used by driver for error msgs */



#define TRACK9OL  	2		/*nine track tape drive online*/
#define ARCHOL    	1		/*archive tape drive online*/
#define SIZE9   	0x4000
#define TAPE9   	0x82		/*device type for nine track*/
#define Q11     	1		/*tape format was changed to QIC11*/
#define Q24     	2		/*tape format was changed to QIC24*/


/* defines for looking at the 2 byte pertec status info as a short */

/* ARCHIVE */

		/*tape status byte 1 */

#define ST_EX1		0x0008		/* exception byte 1.
					 * set when any other bit except BOM
					 * is set in status byte 1.
					 */
#define ST_ILLCMD	0x0040		/* illegal command */
#define ST_NODATA	0x0020		/* no data detected */
#define ST_8RETRY	0x0010		/* 8 or more read retry */
#define ST_BOM		0x0008		/* beginning of media (not reset)*/
/**********		0x0004		RESERVED *************/
/**********		0x0002		RESERVED *************/
#define ST_POWER	0x0001		/* reset/power up */


		/* tape status byte 0	*/ 

#define ST_EX0		0x8000		/* exception byte 0.
					 * set when any other bit except BOM
					 * is set in status byte 0.
					 */
#define ST_NOCART	0x4000		/* cartridge not in place (not reset) */
#define ST_DRNOT	0x2000		/* drive not online (not reset) */
#define ST_WRPROT	0x1000		/* write protected (not reset) */
#define ST_EOM		0x0800		/* end of media (not reset) */
#define ST_URECOV	0x0400		/* unrecoverable data error */
#define ST_BIE		0x0200		/* BIE not located */
#define ST_FMD		0x0100		/* file mark detected */


/* NINE TRACX */



#define ST_T9_IRDY	0x8000		/* drive ready */
#define ST_T9_IONL	0x4000		/* drive on line */
#define ST_T9_IRWD	0x2000		/* rewind in process */
#define ST_T9_ILDP	0x1000		/* beginning of tape */
#define ST_T9_IEOT	0x0800		/* end of tape */
#define ST_T9_IFPT	0x0400		/* file protected */
#define ST_T9_ISPEED	0x0200		/* high speed */
#define ST_T9_IFBY	0x0100		/* format busy */
#define ST_T9_LIDENT	0x0010		/* burst ID written*/
#define ST_T9_LHER	0x0008		/* hard error */
#define ST_T9_LCER	0x0004		/* corrected error */
#define ST_T9_LFMK	0x0002		/* file mark */
#define ST_T9_OVRUN	0x0001		/* over run */

#endif /* SYS_MT_H */
