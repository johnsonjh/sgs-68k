#ifndef SYS_GCP_GCIOCP_H
#define SYS_GCP_GCIOCP_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gciocp.h: version 2.1 created on 5/22/89 at 19:06:19	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gciocp.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* @(#)head/gcp:gciocp.h	1.11 */
/*******************************************************************
 *
 *			G  C  I  O  C  P
 *
 *	GENERAL COMMUNICATION INPUT/OUTPUT CONTROL PROCESSOR
 *
 *******************************************************************/
/*
 gh0 - 07-12-88 - change time constant from 25 mills to 10 mills so we
		  can interrupt kernel every 10 mills instead of every cmd
*/

#ifdef GC16
#define RTC_16MEG_TC	20000		/* real time clock time constant */
#else
#define RTC_8MEG_TC	20000		/* real time clock time constant */
#define RTC_10MEG_TC	25000		/* real time clock time constant */
#endif
#define RTC_MS		10		/* 10 milliseconds per tick */
#define RTC_SYS_SLICE	5		/* time in ticks a process can run */
#define RTC_TPS		1000/RTC_MS	/* rtc ticks per second */

/*******************************************************************
 *
 *	M E M O R Y    M A P P E D    I / O
 *
 *******************************************************************/

#ifdef GC16
#define SCC0B		(struct scc *)		0x222000
#define SCC0A		(struct scc *)		0x222020
#define SCC1B		(struct scc *)		0x222100
#define SCC1A		(struct scc *)		0x222120
#define SCC2B		(struct scc *)		0x222200
#define SCC2A		(struct scc *)		0x222220
#define SCC3B		(struct scc *)		0x222300
#define SCC3A		(struct scc *)		0x222320
#define SCC4B		(struct scc *)		0x222400
#define SCC4A		(struct scc *)		0x222420
#define SCC5B		(struct scc *)		0x222500
#define SCC5A		(struct scc *)		0x222520
#define SCC6B		(struct scc *)		0x222600
#define SCC6A		(struct scc *)		0x222620
#define SCC7B		(struct scc *)		0x222700
#define SCC7A		(struct scc *)		0x222720

#define CIO		(struct cio *)		0x222800

#else		/* gc8 */
#define SCC0B		(struct scc *)		0x50000
#define SCC0A		(struct scc *)		0x50020
#define SCC1B		(struct scc *)		0x52000
#define SCC1A		(struct scc *)		0x52020
#define SCC2B		(struct scc *)		0x54000
#define SCC2A		(struct scc *)		0x54020
#define SCC3B		(struct scc *)		0x56000
#define SCC3A		(struct scc *)		0x56020
#define CIO		(struct cio *)		0x58000
#endif

#ifdef GC16
/* write board control registers */
#define DTR_CTRL	(unsigned short *)	0x220004 /* new */
#define STOM_INTR	(unsigned short *)	0x22000C /* was ICB_INTERRUPT*/
#define FLOAT_BASE	(unsigned short *)	0x220010 /* was byte */
#define DIAG_CTRL	(unsigned short *)	0x220014 /* new */
#define SCC_SYNC_CTRL	(unsigned short *)	0x220018 /* new */
#define MISC_CTRL	(unsigned short *)	0x22001C /* new */
#define BD_CONTROL	(unsigned short *)	0x225000 /* changed, byte */

/* read board control registers */
#define MTOS_INTR	(unsigned short *)	0x220000 /* was ICB_INTERRUPT*/
#define DIAG_STATUS	(unsigned short *)	0x220008 /* new */
#define DCD_STATUS	(unsigned short *)	0x22000C /* new */
#define BOARD_TYPE	(unsigned short *)	0x223004 /* was ICB_HISTAT */
#define TAS_CLR		(unsigned short *)	0x224004 /* was char */
#define SLOT_ADDR	(unsigned short *)	0x228000 /* was ICB_LOSTAT */

#else		/* gc8 */
/* write registers */
#define TAS_CLR		(unsigned char *)	0x60000  /* byte write only */
#define BD_CONTROL	(unsigned char *)	0x60001  /* byte write only */
#define MEM_FLT_CLR	(unsigned char *)	0x60002  /* byte write only */
#define FLOAT_BASE	(unsigned char *)	0x60003  /* byte write only */
#define GCP_IF_CTRL	(unsigned short *)	0x60004  /* word write only */
#define STOM_INTR	(unsigned short *)	0x60006  /* word write only */

/* read registers */
#define BOARD_TYPE	(unsigned char *)	0x60004  /* byte read only */
#define SLOT_ADDR	(unsigned char *)	0x60005  /* byte read only */
#define MTOS_INTR	(unsigned short *)	0x60006  /* word read only */
#endif

/*******************************************************************
 *
 *	I N T E R R U P T    V E C T O R S
 *
 *******************************************************************/

#define SCC0VEC		0x80
#define SCC1VEC		0x90
#define SCC2VEC		0xA0
#define SCC3VEC		0xB0
#define SCC4VEC		0xC0
#define SCC5VEC		0xD0
#define SCC6VEC		0xE0
#define SCC7VEC		0xF0

#define CTCVEC		0x31
#define PORTAVEC	0x39
#define PORTBVEC	0x3B

/* DIAG_CTRL */
#define WRITE_PARITY		0x0080
#define ICB_PARITY_ENBL		0x0040
#define IOP_PARITY_ENBL		0x0020
#define MISC_TEST		0x0010
#define PARALLEL_TEST		0x0008

/* MISC_CTRL */
#define PARITY_RESET		0x8000
#define TIMEOUT_RESET		0x4000
#define INTR_FIFO_ENBL		0x2000
#define NMI_ENABLE		0x1000
#define RELOCATE_PROM		0x0800
#define DMA_ENABLE		0x0400
#define TCOM_XMIT_ENBL		0x0200
#define SEL_8_CHAN_IF		0x0100

/* BD_CONTROL */
#ifdef GC16
#define BC_MASK_LED		0x3FFF
#define BC_NO_LED		0xC000
#define BC_GREEN_LED		0x8000
#define BC_RED_LED		0x4000
#define BC_YELLOW_LED		0x0000

#define BC_PARITY_ENBL		0x1000
#define BC_FLOAT_MEM_ENBL	0x0800
#define BC_IOP_READY		0x0400
#define BC_MEM_CONFIG		0x0200

#else			/* gc8 */
#define BC_IF_RESET_NOT		0x80
#define BC_MEM_CONFIG		0x40
#define BC_IOP_READY		0x20
#define BC_FLOAT_MEM_ENBL	0x10
#define BC_PARITY_ENBL		0x08
#define BC_REFRESH_ENBL_NOT	0x04
#define BC_MASK_LED		0xfc
#define BC_YELLOW_LED		0x00
#define BC_GREEN_LED		0x01
#define BC_RED_LED		0x02
#define BC_NO_LED		0x03
#endif

/* DIAG_STATUS */
#define CPU_PARITY_ERR		0x8000
#define TIMEOUT_ERR		0x4000
#define ICB_PARITY_ERR		0x2000
#define DMA_PARITY_ERR		0x1000
#define IOP_PARITY_ERR		0x0800
#define BP_COP_INST		0x0400
#define PARALLEL_ERR		0x0200
#define DIAG_BOARD		0x0100

/* BOARD_TYPE */
#ifdef GC16
#define ICB_CMD_RECV		0x8000
#define IOP_INTR_PEND		0x4000
#define IF_CARD_TYPE		0x3800
#define GCP_BD_TYPE		0x0700

#define IF_TYPE_0		0x0000
#define IF_TYPE_1		0x0800
#define IF_TYPE_2		0x1000
#define IF_TYPE_3		0x1800
#define IF_TYPE_5		0x2800

#else			/* gc8 */
#define ICB_CMD_RECV		0x80
#define IOP_INTR_PEND		0x40
#define IF_CARD_TYPE		0x38
#define GCP_BD_TYPE		0x07

#define IF_TYPE_0		0x00
#define IF_TYPE_1		0x08
#define IF_TYPE_2		0x10
#define IF_TYPE_3		0x18
#define IF_TYPE_5		0x28
#endif

/* SLOT_ADDR */
#define TAS_STATUS		0x0080
#define IOP_FAULTED		0x0040
#define MEM_CONFIG		0x0020
#define	SLOT_ADDRESS		0x001f

/* misc */
#define VREGS  (int *)(sys_icb_base + 0xff00)

#endif /* SYS_GCP_GCIOCP_H */
