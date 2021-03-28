/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) scsi_log.h: version 2.1 created on 4/17/90 at 14:03:19	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)scsi_log.h	2.1	4/17/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifdef SCSI_LOG
/* error log definition */
#define LOG_DSK		0x00	/* 0x00 - 0x0F */
#define LOG_TP		0x10	/* 0x10 - 0x1F */
#define LOG_DRV 	0x20	/* 0x20 - 0x2F */
#define LOG_SRV		0x30	/* 0x30	- 0x3F */
#define LOG_DB		0x40    /* 0x40 - 0x4F */
#define LOG_TMO		0x50	/* 0x50 - 0x5F	*/
#define LOG_DMA		0x60	/* 0x60 - 0x6F	*/
#define LOG_FSM		0x70	/* 0x70 - 0x8F	*/
#define LOG_DI		0x80	/* 0x80 - 0x8F  */
#define LOG_XTP		0xA0	/* 0xA0 - 0xAF	*/
#define LOG_TMER	0xB0
#define LOG_PR		0xD0	/* log parameter */

/* device driver */
#define LOG_INIT	0x00	/* log init	*/
#define LOG_OPEN	0x01	/* log open */
#define LOG_CLOSE	0x02	/* log close */
#define LOG_STRA	0x03	/* log strategy */
#define LOG_IOCTL	0x04	/* log ioctl	*/
#define LOG_START      	0x05	/* log start	*/
#define LOG_XSTART	0x06	/* log xstart	*/
#define LOG_INT		0x07	/* log interrupt routine */
#define LOG_XCMD	0x08	/* ???command */
#define LOG_RESTART	0x09	/* log restart routine */
#define LOG_INITSL   	0x0A	/* log init slice	*/
#define LOG_SETMD   	0x0B	/* log set mode		*/
#define LOG_SL   	0x0C	/* log set mode		*/
#define LOG_HERR   	0x0D	/* log error set 	*/
#define LOG_FMT   	0x0E	/* log format disk	*/


/* scsi_drv and simulation */
#define LOG_SSEND	0x00	/* scsi_send_cmd */
#define LOG_SEND	0x01	/* issue_cmd	*/
#define LOG_GET		0x02	/* scsi_get_cmd */
#define LOG_PUT		0x03	/* scsi_put_cmd */

/* the following item is used for simulation */ 
#define LOG_DCMD	0x04	/* disk_cmd */
#define LOG_TCMD	0x05	/* tape_cmd */
#define LOG_NTCMD	0x06	/* ni_tape_cmd */
#define LOG_ATCMD	0x07	/* ar_tape_cmd */
#define LOG_NR		0x08	/* nine_track_read */
#define LOG_NW		0x09	/* nine_track_write */

#define LOG_ESP		0x04	/* the esp command just send */
#define LOG_DONE	0x05	/* scsi_done */
#define LOG_RES		0x06	/* reselect */
#define LOG_DIS		0x07	/* disconnect */
#define LOG_MGI		0x08	/* message in */
#define LOG_XIN		0x09	/* transfer information */
#define LOG_STS		0x0A	/* check status */
#define LOG_ERR		0x0B	/* parity error */
#define LOG_XTR		0x0C	/* extra bytes */

/* interrupt */
#define	LOG_S		0x00	/* scsi interrupt */
#define LOG_D		0x01	/* device board interrupt */

/* timeout */
#define LOG_DCB		0x00	/* timeout of DCB */
#define LOG_XFR		0x01	/* timeout of xfer */
#define LOG_STO		0x02	/* stimeout	*/
#define LOG_UST		0x03	/* suntimeout	*/
/* dma */
#define LOG_DMP		0x00	/* dump extesion fifo */
#define LOG_SDP		0x01	/* setup page table */
#define LOG_XNP		0x02	/* transfering next page table */

/* finite state machine */
#define LOG_SF		0x00	/* scsi finite state machine */
#define LOG_DF		0x01	/* disc mode */
#define LOG_ITF		0x02	/* initiator mode */
#define LOG_IF		0x03	/* information finite state */
#define LOG_CNT		0x04	/* remain count */

/* timer */
#define TIMEOUT		0x00
#define UNTIME		0x01
/* scsi driver init routine */
#define LOG_SC		0x00	/* init device board */
#define LOG_CH		0x01	/* init channel */
#define LOG_RST		0x02	/* reset scsi bus */

#define RBEGIN		0x00	/* routine begin */
#define RFINE		0x01    /* no error	*/
#define RERROR	        0x02	/* error happen */
#define REND		0x0F	/* routine end */

#define LOG_LONG_WORD(x,u,y,z) \
log((x), u, (int)y>>24, z); \
log(LOG_PR, (int)y>>16, (int)y>>8, (int)y)

#else	/* null the log function */
#define log(U,X,Y,Z)
#define LOG_LONG_WORD(x,u,y,z) 
#endif /* SCSI_LOGA */	
