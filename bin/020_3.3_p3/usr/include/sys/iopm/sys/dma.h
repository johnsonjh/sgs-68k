/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) dma.h: version 24.1 created on 10/28/91 at 17:59:52	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)dma.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef IOPM_DMA_H
#define IOPM_DMA_H

#define DMA       0x01	/* "simple" dma allowed, no SG, no other resrictions */
#define SGDMA     0x02	/* scatter/gather dma ok */
#define DMA16     0x04	/* dma limited to 16 byte boundary */
#define LOCALDMA  0x08	/* local dma only */

#endif /* IOPM_DMA_H */
