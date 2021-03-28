/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) scsi_ncr.h: version 24.1 created on 10/28/91 at 17:52:33	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)scsi_ncr.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* Define NCR 53c90 registers set */                  
/* Each register should locate at long word boundry */
struct ncr_regs {
	union {
		struct {
			uint xfercnt_lo;  /* transfer counter high byte */
			uint xfercnt_hi;  /* transfer count low byte */
			uint fifo;	   /* FIFO register	*/
			uint cmd;	   /* command register */
			uint status;	   /* status register */
			uint int_status;   /* interrupt status register */
			uint seq_stp;	   /* sequence step register */
			uint fifo_flag;	   /* FIFO flags register when read */
			uint config;	   /* configuration register	*/
		} r;			   /* the registers read only */
		struct {
			uint xfercnt_lo;   /* transfer counter high byte */
			uint xfercnt_hi;   /* transfer count low byte */
			uint fifo;	   /* FIFO register	*/
			uint cmd;	   /* command register */
			uint bus_id;	   /* bus id register */
			uint timeout;  	   /* sel/resel timeout register */
			uint sync;	   /* sync xfer period register */
			uint sync_offset;  /* sync offset register	*/
			uint config;	   /* configuration register	*/
			uint ccf;	   /* clock conversion register */
			uint test;	   /* test register		*/
		} w;
	  } un;
	};	
		
			
	
typedef struct ncr_regs SCSI_R;

/* interrupt status register definition  */

#define	INT_SCSI_RESET		0x80  /* scsi bus reset detected */
#define INT_ILLEGAL_CMD		0x40  /* illegal command 	*/
#define INT_DISCONNECT		0x20  /* disconnected		*/
#define INT_BUS_SERV		0x10  /* bus service 	        */
#define INT_FUNC_COMP		0x08  /* esp command commpleted */
#define INT_RESELECTED		0x04  /* target reselected	*/
#define INT_SELECTED_ATN	0x02  /* selected with atn	*/
#define INT_SELECTED		0x01  /* selected without atn 	*/

/* Status register definition */
#define ST_ERROR_MASK		0x60   /* bit 6 and 5 are error indications */
#define	ST_GROSS_ERR		0x40   /* gross error	*/
#define ST_PARITY_ERR		0x20	/* parity error */

#define ST_XFER_CNT_0		0x10	/* transfer count is 0 */
#define ST_XFER_COMP		0x08     /* transfer completed */

#define ST_XFER_MASK		0x07	/* the last 3 bits in status register
					   are transfer phases */
#define ST_DATAOUT		0x00	/* data out phase	*/
#define ST_DATAIN		0x01	/* data in phase	*/
#define ST_COMMAND		0x02	/* command phase	*/
#define ST_STATUS		0x03	/* status phase		*/
#define ST_MSGOUT		0x06	/* message out phase	*/
#define ST_MSGIN		0x07	/* message in phase	*/


/* bit definition of configuration register	*/
#define CF_SLOW_CABLE	0x80 /* Slow cable mode		     */
#define CF_SCSI_RID	0x40 /* SCSI Reset Interrupt Disable */
#define CF_PARITY_MODE	0x20 /* Parity test mode	     */
#define CF_PARITY  	0x10 /* Parity Enable		     */
#define CF_TEST_MODE	0x08 /* Chip test mode		     */
#define CF_BUS_ID	0x07 /* The initiator bus ID	     */


/* definition of command register -- SCSI_CMD_REG */
#define ESP_DMA		0x80	/* enable DMA		*/
#define ESP_CMD_ZONE  	0x7F  	/* command zone		*/
#define ESP_MODE_ZONE 	0x70  	/* command mode zone	*/
#define ESP_DISC_MODE 	0x40	/* disconnected mode	*/
#define ESP_TRGT_MODE 	0x20	/* target mode		*/
#define ESP_INIT_MODE 	0x10	/* initiator mode	*/

/* MISC Commands */
#define	ESP_NOP		0		/* no operation */
#define	ESP_FLSH_FIFO	1		/* flush FIFO	*/ 
#define	ESP_RESET_CHIP 	2		/* reset chip   */
#define	ESP_RESET_BUS	3		/* reset scsi bus */

/* commands in disconnected mode */
#define ESP_RECON	0x40		/* reconnect sequence */
#define ESP_SEL		0x41		/* select without ATN */
#define ESP_SELATN	0x42		/* select with ATN    */
#define ESP_SELATNSTP	0x43		/* select with ATN & STOP */
#define ESP_ENSEL	0x44		/* enable selection/reselection */
#define ESP_DISSEL 	0x45		/* disable selection/reselection */

/* commands in initiator mode */
#define ESP_XFER_INFO	0x10		/* transfer information */
#define ESP_CMD_CMP	0x11		/* initiator command complete */
#define ESP_MSG_ACPT	0x12		/* message accepted		*/
#define ESP_XFER_PAD	0x18		/* transfer pad			*/
#define ESP_SET_ATN	0x1a		/* set ATN			*/



/* the selection sequence step definition	*/
#define SEQ_MASK	0x07

#define STEP_0		0x0020	/* Arb completed,Sel timeout;disconnected */
#define STEP_05		0x0018  /* Sel completed, ATN on,not msgout phase */
#define STEP_1		0x0118  /* Sent one message byte ATN on		  */ 
#define STEP_2		0x0218  /* Message out phase;sent one byte.atn off */
#define STEP_3		0x0318  /* Command phase,no command byte sent	*/
#define STEP_4		0x0418  /* Command phase completed		*/

#define MAXXFERCNT	0x10000  /* the max transfer count for ncr53c90 */

/*
 * Mask for valid bits in the FIFO flags register: the least significant
 * five bits indicate the number of bytes remaining in the fifo.
 * The remaining bits are reserved.
 */
#define FIFO_FLAG_MASK	0x1f

