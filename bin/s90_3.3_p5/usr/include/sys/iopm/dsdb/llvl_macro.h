/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) llvl_macro.h: version 2.1 created on 4/17/90 at 14:01:24	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)llvl_macro.h	2.1	4/17/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* macro definitions	*/
#define MAXNODRVS	16

#define GET_BUS_ID(unit)	((unit) & 0x07)
#define INVALID_UNIT_ID(x) ((x)&(~(MAXNODRVS-1)))
#define IS_INITIATOR_ID(x) (GET_BUS_ID(x) == INITIATOR_ID)
#define	ENCODE_BUS_ID(unit)	(1 << ((unit) & 0x07))
#define GETSCSI(x)	&chan[((x&8)>>3)]	/* get scsi bus control block */
#define ESPRREG(x)	scb->ch_r.sr->un.r.x /* chip read only register */
#define ESPWREG(x)	scb->ch_r.sr->un.w.x /* chip write only register */
/* #define dma_xtrabyte(x) (4 - (uint)x&3)&3 */ /* return number of byte ahead
					 to the next long word address */

#define dma_xtrabyte(x) 0 	/* return number of byte ahead */

/* set first dma page address */
#define SCSI_DMA_FIRST_PAGE(x,y)   (unchar **)((int)x + ((int)y >> 10))

/* get page offset */
#define DMA_PAGE_OFFSET(x)  ((int)x&(DMA_PAGE-1))

#define GETDCB(x)	&scsidev[x&15]  /* get device table */
#define DISC_PHASE	0xFF		/* disconnect phase */
