/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) scsi_cmdbx.h: version 24.1 created on 10/28/91 at 17:52:44	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)scsi_cmdbx.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* The structure of command mail box */

/* this structure is shared by both virtural driver and scsi driver */


struct cmbox {
	struct cmbox	*next;	/* next item in link list	*/
	uint	cmd_flags;	/* the flags to show the current status */

/* this section is filled out by device driver	*/

	uint	retry_cnt;	/* command retry count */

	ushort cmd_type;	/* scsi command type */
	ushort	unit_id;	/* the unit ID	*/
	ushort cmd_dsc_len;	/* the length of scsi command 
				   descriptor block */
	unchar  cmd_dsc_blk[14]; /* scsi command descriptor block */
 	void   (*service)();   /* procedure to call when after scsi
				     command done by scsi driver */		
	uint	tag;		/* corresponding event to wakeup if necessary */
	uint    timer;		/* number of clock ticks to wait after command
				   has been issued */
	unchar	*dma_buffer;	/* the dma buffer pointer	*/
	void	*dma_proc;      
	uint    dma_buf_len;	/* number of bytes for dma to transfer */
	uint	dma_flags;	/* the dma flags such as read,write */ 

/* this section is filled out by scsi driver */
	
	ushort    err_code;	/* the error code */
	ushort	  err_len;	/* the size of error status buffer */	
	unchar 	error[28];	/* the error status buffer */
	uint	dma_resid;	/* the dma complete transfer count */
	uint	act;		/* the time period starting command initiated
				   till command finished */
	uint	disc_count;	/* number of disconnect happened in processing */
};

typedef struct cmbox	CMD_BOX;

/* definition of cmd_flags */
#define CB_NEXT		0x01		/* box is the next command */
#define CB_READY	0x02		/* box is in ready q	*/
#define CB_RUNNING	0x04		/* box is in service	*/
#define CB_COMP		0x08		/* box has been done by scsi driver */
#define CB_SERV		0x10		/* box return to device driver */
#define	CB_RET		0x20		/* box return to free pool */
#define CB_DISC		0x40		/* command is not on line */
#define CB_ABORT	0x80		/* command is abort */
#define CB_SENSE	0x100		/* command is trying to get sense key */

#define CB_MASK		0xFFFFFF00	/* clear last byte */

/* definition of dma_flags */
#define DMA_READ	0x00000001	/* dma read */

/* definition of cmd_type */
#define CMD_RELEASE_ATTN 0x01		/* clear atn */

/* the next section is not used for the time being	*/
/* definition of scmd */
#define SCMD_DEVICE	0x00		/* scsi type of command */
#define SCMD_BLOCK	0x01		/* block device */
#define SCMD_UNBLOCK	0x02		/* unblock  device */
#define SCMD_RESET	0x03		/* reset scsi bus  */

/* command mailbox allocation */
#define MAXALLOC	2	/* the max number of mailbox allocation for
					each device */

/* command  mailbox control block */
typedef struct  {
	uint flags;
	CMD_BOX *cm;
} CM_CTL;
#define  CM_BUSY 	1
#define	 CM_FREE	0
