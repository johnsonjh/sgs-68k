/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ms_lp.h: version 24.1 created on 10/28/91 at 17:39:13	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ms_lp.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*
 *	ms_lp.h
 */
/*------------------------------------------------------------------------*/
#define START_OF_MS_LP_BUF_LO	(unchar *)0xf0020000
#define END_OF_MS_LP_BUF_LO	(unchar *)0xf0021fff

#define START_OF_MS_LP_BUF_HI	(unchar *)0xf002e000
#define END_OF_MS_LP_BUF_HI	(unchar *)0xf002ffff
/*------------------------------------------------------------------------*/

#define MS_LP_CONTROL_REG 	(unchar *)0xf0022000
#define MS_LP_CLEAR_DONE 	(unchar *)0xf0022001
#define MS_LP_STATUS_REG 	(unchar *)0xf0024000
#define MS_LP_CLEAR_INT 	(unchar *)0xf0024001
/*------------------------------------------------------------------------*/
#define MS_LP_BUF_SIZE		0x2000
/*------------------------------------------------------------------------*/
/*
 *	MS_LP_CONTROL_REG bits
 */
#define MS_LPC_DMAEN	1	/* enable printer DMA transfer */
#define MS_LPC_RESET	2	/* reset 'interrupt' and 'done' bits */
#define MS_LPC_D_U	4	/* if set, transfer counter decements*/
#define MS_LPC_TEST	8	/* test mode*/
#define MS_LPC_INTEN	0x10	/* enable interrupt to IOPM */
#define MS_LPC_SETDONE	0x20	/* dont hold F/F set - used by diagnostics */
#define MS_LPC_ACKNLG	0x40	/* toggle ack signal - used by diagnostics */
#define MS_LPC_PRIME	0x80	/* clears printer buffer & inits printer */

#define MS_LPC_KEEP_HI	(MS_LPC_RESET|MS_LPC_SETDONE|MS_LPC_ACKNLG)
/*------------------------------------------------------------------------*/
/*
 *	MS_LP_STATUS_REG bits
 */
#define MS_LPS_INT	1	/* transfer completion interrupt */
#define MS_LPS_ACKNG	2	/* printer acknowledge */
#define MS_LPS_BUSY	4	/* printer busy */
#define MS_LPS_SLCT	8	/* printer selected */
#define MS_LPS_PE	0x10	/* paper empty */
#define MS_LPS_FAULT	0x20	/* printer fault */
#define MS_LPS_DONE	0x40	/* transfer complete */
/*			0x80	   not used */
/*------------------------------------------------------------------------*/
