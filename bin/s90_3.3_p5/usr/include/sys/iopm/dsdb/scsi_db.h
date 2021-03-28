/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) scsi_db.h: version 24.1 created on 10/28/91 at 17:51:25	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)scsi_db.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/* dual scsi device board extention */
 
typedef struct {
	union {
		struct {
			unsigned int	bcr[2]; /* board control regester, channel A,B */
			unsigned int	pcrh;   /* pointer counter for bit 31 - 10   */
			unsigned int	res1;	/* not used. */
			struct {
				unsigned int	pcr; /* ptr counter bit 9-2,channel A,B */
				unsigned int	res2;  /* another spare.. */
			} wg[2];  /* two of these now as well. */
		} w;			/* write only registers set */
		struct {
			unsigned int	bsr;    /* board status register  */
			unsigned int	res[3];	/* fill in reserved words. */
			struct {
				unsigned int pcr; /* pointer counter bit 9-2,channel A,B */
				unsigned int mmr; /* memory address register, chan A,B */
			} rg[2];
		} r; 			/* read only registers set */
	} un;
} SCSI_DB_REGS;


/******************************************************************************
*   DMA control registers:
*   Long word access only.  Write only.  Same register is for either channel.
*
*   ps: the valid data loactions --
*	channel A -- bit 15 - 8.
*	channel B -- bit 23 - 16.
*
*   31 ----- 8   7    6   5     4     3     2     1   0
*   +----------+---+----+----+------+-----+----+----+---+
*   | not used |burst|scsi|test|cmd sl|wr/rd|enab|unld|res|
*   +----------+-----+----+----+------+-----+----+----+---+
*        |       |     |    |     |      |     |   |    |
*        |       |     |    |     |      |     |   |    +--Reset this channel.
*        |       |     |    |     |      |     |   |      Hold 1 for min of 2us.
*        |       |     |    |     |      |     |   +---- Unload this fifo.
*        |       |     |    |     |      |     |         disable DMA prior to
*        |       |     |    |     |      |     |         setting this bit.   
*        |       |     |    |     |      |     +---- Enable this dma chan.
*        |       |     |    |     |      +----- write/read. A 1 will xfer
*        |       |     |    |     |             from memory to scsi.
*        |       |     |    |     |             (write operation)
*        |       |     |    |     +----- 0 = single ended outputs
*        |       |     |    |            1 = differential outputs
*        |       |     |    +----- Cmd test mode. 1 = test.
*        |       |     +----- scsi reset chip
*        |       +---------------------------------- Burst bit: 0 is set,
*        |      						1 is normal.
*        +------------------------------------------ Not used by hardware.
*
*
******************************************************************************/


/* definitions of board control register - bcra,bcrb */
#ifdef	WIREWRAP
#define BCR_RESET	0x01	/* reset device board */
#else
#define	BCR_UNRESET	0x01	/* 1 - unreset dma channel , 0 - reset */
#endif
#define BCR_UNLOAD	0x02	/* unload pipe line  */
#define	BCR_ENDMA	0x04	/* enable DMA	     */
#define BCR_DMAWRITE	0x08	/* dma direction: 1: mem->scsi, 0: scsi->mem */
#define BCR_DIFF	0x10	/* set connection */
#define BCR_SINGLE      0x00	/* single ended
#define BCR_TESTMODE	0x20	/* set to test mode	*/
#ifdef	WIREWRAP
#define BCR_SCSIRES	0x40	/*   scsi chip unreset */
#else
#define BCR_UNSCSIRES	0x40	/*  1 - scsi chip unreset, 0 - chip reset */
#endif
#define BCR_BURST	0x80	/* Burst bit: 0 is set, 1 is normal. */

#ifndef WIREWRAP
#define BCR_SHIFT_A	8	/* shift 8 bit after setting channel a command */
#define BCR_SHIFT_B	16	/* shift 16 bit after setting channel a command */
#endif
/******************************************************************************
*   DMA status register:
*   Long word access only.  Read only.
*
*   31 - 8    7        6        5       4       3        2    1        0
*   +-----+--------+--------+-------+-------+-------+-------+-------+-------+
*   | n/a | unld_A | unld_B | err_a | err_b | int_A | int_B | ftstB | ftstA | 
*   +-----+--------+--------+-------+-------+-------+-------+-------+-------+
*      |      |       |       |       |       |        |         |     | 
*      |      |       |       |       |       |        |         +--fuse test 
*      |      |       |       |       |       |        |
*      |      |       |       |       |       |        +-- SCSI int, chan B.
*      |      |       |       |       |       +----------- SCSI int, chan A.
*      |      |       |       |       |                   
*      |      |       |       |       +------------------- SCSI bus error, B.
*      |      |       |       |                         
*      |      |       |       +--------------------------- SCSI bus error, A.
*      |      |       |                               
*      |      |       +----------------------------------- Unload done, chan B
*      |      |                                    
*      |      +------------------------------------------- Unload done, chan A
*      |                                              
*      +-------------------------------------------------- Not used by hardware.
*
******************************************************************************/

/* definitions of status register - bsr */
#ifndef WIREWRAP
#define BSR_FUSEB	0x01	/* fuse test channel B		 */
#define BSR_FUSEA	0x02	/* fuse test channel A		 */
#endif
#define BSR_SCSIB	0x04	/* scsi interrupt from channel B */
#define BSR_SCSIA	0x08    /* scsi interrupt from channel A */
#define BSR_BUSERRB	0x10	/* bus error from channel B 	 */
#define BSR_BUSERRA	0x20	/* bus error from channel A 	 */
#define BSR_UNLDB	0x40	/* unload done on channel B	 */	
#define BSR_UNLDA	0x80	/* unload done on channel A    	 */

/* memory address map on scsi device board	*/
#define	E_PROM		0xF0000000	/* eeprom: f0000000 - f003ffff */
#define SCSI_CHIPA	0xF0080000	/* ncr chip a	*/
#define SCSI_CHIPB	0xF0090000	/* ncr chip b	*/
#define DB_CONTROL	0xF00A0000	/* External register */


