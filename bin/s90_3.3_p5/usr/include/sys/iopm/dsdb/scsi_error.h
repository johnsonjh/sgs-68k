/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) scsi_error.h: version 24.1 created on 10/28/91 at 17:52:47	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)scsi_error.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifdef OLD_ERR

#define DEVERR_MARK_DETECTED	0x30
#define DEVERR_FM_DETECTED	0x31
#define DEVERR_EOM_DETECTED	0x32
#define DEVERR_ILI_DETECTED	0x33
#define DEVERR_BOM		0x34

#endif 

/* scsi driver error code definition */
/* software errors */
#define SCSIERR_NOERR	   0x00	/* no error */
#define SCSIERR_INVAL      0x01	/* invaild parameter in command box */
#define SCSIERR_UNIT_ID    0x02	/* invalid unit id		    */
#define SCSIERR_ATN	   0x03	/* scsi device is in unit_attention state */

/* SCSI bus errors */
#define SCSIERR_BUSTIMEOUT 0x10	/* scsi bus accessing timeout	    */
#define SCSIERR_ILLSEQ	   0x11	/* illegal select sequence number	*/
#define	SCSIERR_PHASE	   0x12	/* illegal phase changed */
#define SCSIERR_PARITY     0x13	/* parity error		*/
#define	SCSIERR_BUSRESET   0x14	/* bus reset	*/
#define SCSIERR_DISC	   0x15	/* unexpected disconnect  */
#define SCSIERR_NODEV	   0x16	/* scsi device is not on line	    */
#define SCSIERR_CMDTIMEOUT 0x17 /* command excution timeout	*/

/* device relative errors */
#define SCSIERR_UXPCTMSG   0x20	/* unexpected message received */
#define SCSIERR_NOCCS	   0x21	/* message was rejected */
#define SCSIERR_FATAL	   0x22	/* fatal error such as get check condition
					on request sense command */
#define SCSIERR_SENSE	   0x23 /* device error, request sense data in 
				   error buffer */
#define SCSIERR_BUSY	   0x24 /* device busy */

/* DMA relative errors */
#define SCSIERR_DMABUS     0x30	/* dma bus error */
#define SCSIERR_DMACNT     0x31	/* DMA count error */




