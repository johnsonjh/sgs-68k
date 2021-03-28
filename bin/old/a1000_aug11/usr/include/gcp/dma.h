#ifndef SYS_GCP_DMA_H
#define SYS_GCP_DMA_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) dma.h: version 2.1 created on 5/22/89 at 19:06:13	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)dma.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)head/gcp:dma.h	1.1"

struct  dmactl
{
	short  loaddr;
	short  hiaddr;
	short  count;
	short  notused;
};

struct  dmaen
{
	short  control;
	short  notused1;
	short  notused2;
	short  notused3;
};

extern struct dmactl  txdmactl[];
extern struct dmactl  rxdmactl[];
extern struct dmactl  ppdmactl;

extern struct dmaen   txdmaen[];
extern struct dmaen   rxdmaen[];
extern struct dmaen   ppdmaen;

#define  DMAON  0x8000
#define  DMAOFF 0x0000

#endif /* SYS_GCP_DMA_H */
