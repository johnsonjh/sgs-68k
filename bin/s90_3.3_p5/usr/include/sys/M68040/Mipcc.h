#ifndef	SYS_M68040_MIPCC_H
#define	SYS_M68040_MIPCC_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Mipcc.h: version 24.1 created on 10/28/91 at 18:30:35	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Mipcc.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * 	ipcc.h -- Interprocessor communication and control
 *
 *	SHIHNAN	- 05/30/89
 *		- 08/03/89	add confident test for DPM040
 *				add individual test for DPM040
 */

/*
================================= IPCC PROTOCOL ===============================

COMMAND WORD:

            |<--- address --->|<----- data ----->|
	    +---+-----+---+---+------------------+
	    |   |     |   |   |  DATA (32 bits)  |
	    +---+-----+---+---+------------------+
	      |    |    |   |          |
	      |    |    |   |          IPC DATA FRAME (32 bits)
	      |    |    |   IPC FRAME ID: 0x80 for cpuA; 0x84 for cpuB (8 bits)
	      |    |    IPC COMMAND (8 bits)
              |    RESERVED (12 bits)
	      IPC SOURCE SLOT (4 bits)


The source slot is the slot that the board that sent the command is in.

The command field and source slot number will be embedded in the address
written to the board in the form:

			0xY000XX00

Where Y is the source slot number and XX is the command field.

===============================================================================
*/



#ifndef	SYS_TYPES_H
#include "sys/types.h"
#endif	/* SYS_TYPES_H */

/* FOR PM020 ONLY */ 
#define IPCC_ADDR_SLOT	(0x80) /* old stuff */
#define IPCC_ADDR_SLOT_A (IPCC_ADDR_SLOT)
#define IPCC_ADDR_SLOT_B (0x84)

/* Comm data structure */
typedef struct ipcc 
{
	unsigned	addr;
	unsigned	data;
	unsigned	count;
	unsigned	MMaddr;
	unsigned char	slot;
	unsigned char	size;
	unsigned char	MMslot;
	unsigned char	last_icmd;
	unsigned char	last_ocmd;
	unsigned char	stdout;
	unsigned char	acck;
	unsigned char	nack;
	unsigned char	error;
	unsigned char	cancel;
	unsigned char	d_handle;
	unsigned char	d_ack;
	unsigned char	d_stats;
} IPCC;

/* IPCC command defines */
#define IPCC_ACK	(0x00)		/* Generic acknowledge         */
#define IPCC_NACK	(0x01)		/* Nope I won't do that        */
#define IPCC_ERROR	(0x02)		/* Report of error             */
#define IPCC_CANCEL	(0x03)		/* Cancel 		       */

#define IPCC_NOP	(0x10)		/* NOP, tests link,        ACK */
#define IPCC_SLOT	(0x11)		/* DATA is slot info,      ACK */
#define IPCC_ADDR	(0x12)		/* DATA is addr info,      ACK */
#define IPCC_DATA	(0x13)		/* DATA is data info,      ACK */
#define IPCC_SIZE	(0x14)		/* DATA is size info,      ACK */
#define IPCC_PACKET	(0x15)		/* DATA is packet info,    ACK */
#define IPCC_STDIO	(0x16)		/* DATA is stdio info,     ACK */
#define IPCC_FDATA	(0x17)		/* DATA is from data,      ACK */
#define IPCC_COUNT	(0x18)		/* DATA is count info,     ACK */

#define IPCC_WRITE	 (0x20)		/* Write data at DATA,     ACK */
#define IPCC_READ        (0x21)		/* Read from DATA to data, ACK */
#define IPCC_XTO_START   (0x22)		/* Start xfer to DATA,     ACK */
#define IPCC_XFROM_START (0x23)		/* Start xfer from DATA,       */
#define IPCC_XEND        (0x24)		/* End xfer,               ACK */
#define IPCC_EXECUTE     (0x25)		/* Execute at DATA (new PC)    */
#define IPCC_NWRITE      (0x26)
#define IPCC_NREAD       (0x27)
#define IPCC_DSEND	 (0x28)		/* Send data back,	   ACK */

#define IPCC_STDOUT_ENA	 (0x30)
#define IPCC_STDOUT_DIS	 (0x31)
#define IPCC_DISP_ON	 (0x32)
#define IPCC_DISP_OFF	 (0x33)
#define IPCC_DISP_ACK	 (0x34)
#define IPCC_DISP_NOACK	 (0x35)
#define IPCC_MM_SLOT	 (0x36)
#define IPCC_MM_OFFSET	 (0x37)
#define IPCC_SM_SLOT	 (0x38)
#define IPCC_ID_ACCT_ON	 (0x39)
#define IPCC_ID_ACCT_OFF (0x3A)

#define IPCC_MM_FILL	(0x40)
#define IPCC_MM_VERIFY	(0x41)
#define IPCC_MM_TEST0	(0x42)		/* simple power-up system memory test */
#define IPCC_MM_TEST1	(0x43)		/* medium system memory test request */
#define IPCC_MM_TEST2	(0x44)		/* complex system memory test request */
#define IPCC_MM_ERRA	(0x45)		/* memory test error address */
#define IPCC_MM_ERRD	(0x46)		/* memory test error read data */
#define IPCC_MM_ERRXD	(0x47)		/* memory test error expected data */
#define IPCC_MM_ERRS	(0x48)		/* memory test error memory slot */
#define IPCC_MM_ERRC	(0x49)		/* memory test error code */
#define IPCC_MM_LPC	(0x4a)		/* memory test loop count */

#define	IPCC_BUS_LOCK	(0x50)		/* S90 bus interlock test */

#define IPCC_PM_KRP	(0x80)		/* with sending int ack */
#define IPCC_PM_NKRP	(0x81)		/* without sending int ack */
/*
			   data = ROOT POINTER
*/
#define	IPCC_PM_PUTR	(0x82)		/* PM Power-Up Test Request */
/* 
			   data = 1 --> stage-I
			   data = 2 --> stage-II
			   data = 3 --> stage-III
			   data = -1 --> skip rest of test, jump to idle state 
*/
#define IPCC_IND_CONF	(0x83)		/* PM's confidence test */
/*
			   data =  0 --> cache off 
			   data >= 1 --> cache on	
*/ 
#define	IPCC_IND_TEST	(0x84)		/* PM's individual test */
/* 
			   data = 2 --> CTAG memory test 
			   data = 3 --> BTAG memory test
			   data = 4 --> TLB memory test
			   data = 5 --> MTAG memory test 
			   data = 6 --> MMU flush test 
			   data = 7 --> root pointers test
			   data = 8 --> interrupt test
			   data = 9 --> parity generator test
			   data = 10 -> receive funnel test 
			   data = 11 -> WAB movml test
			   data = 12 -> read permission test
			   data = 13 -> write permission test
			   data = 14 -> execute permission test
			   data = 15 -> page fault permission test
			   data = 16 -> nak except test
			   data = 17 -> timeout except test
			   data = 18 -> bad I/O except test
			   data = 19 -> table walk except test
			   data = 20 -> read hit test
			   data = 21 -> read miss test
			   data = 22 -> write through test
			   data = 23 -> write miss test
			   data = 26 -> spy test	
*/
#define	IPCC_CNTXT	(0x85)		/* data = Context Number */
#define IPCC_MAP	(0x86)		/* build mem map tables */
					/* data = size of mem map tbls */

/* 
	shihnan - 08/03/89
	commands 0x83 and 0x84 are used for PM020 only 
	these commands will be replaced with 0x87 and 0x88 for the DPM040
*/
		
#define IPCC_DPM_CONF	(0x87)		/* DPM040's confidence test */
/*
			   data = to be defined
*/ 
#define	IPCC_DPM_IND	(0x88)		/* DPM040's individual test */
/* 
			   data = to be defined
*/

#define IPCC_SM_CLOCK	(0xA0)
#define IPCC_SM_CONFIG	(0xA1)
#define IPCC_SM_FLOPPY	(0xA2)
#define IPCC_SM_FACK	(0xA3)
#define IPCC_SM_PRINTF	(0xA4)
#define	IPCC_MS_ON	(0xA5)		/* Turn on SPM Memory simulation mode */
/* 
		  	   data = 00 (NULL)
			   data = 01 (BADD)
			   data = 02 (BDEST1)	
			   data = 03 (BDEST0)
			   data = 04 (BSRC1)
			   data = 05 (BSRC0)	
*/
#define IPCC_MS_OFF	(0xA6)		/* Turn off SPM MS mode */

#endif	/* SYS_M68040_MIPCC_H */
