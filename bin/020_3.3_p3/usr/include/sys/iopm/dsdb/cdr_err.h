/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) cdr_err.h: version 24.1 created on 10/28/91 at 17:49:49	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)cdr_err.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* extended sense codes */
#define EXSNS_MEDIA_CH	0x28
#define EXSNS_RESET	0x29
#define EXSNS_MODE_CH	0x2a


/* macro definition */
#define CHANID(x)	((x>>3)&0x01)	/* get channel id */
#define BUSID(x)	(x&0x07)	/* get bus id */
#define DIS_CD_ID(x) printf("Cdrom error on channel id %d bus id %d: ",CHANID(x),BUSID(x))
/* the default error recovery flags value	*/
#define DEF_ERR_FLAGS	0x88	/* AWRE,EEC */			
#define DEF_RETRY_CNT	0x0A	/* target retry count value	*/


/* if buffer requested header is allocated by driver, then 2nd and 3rd bytes
   of b_flags will be the scsi command */
/* redifine  of b_flags in struct buf */
#define	SCSI_CMD	0xFF000000 /* scsi command byte */
#define SCSI_PRA	0x00FFFFFF /* parameters with scsi command */

