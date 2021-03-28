#ifndef SYS_GCP_SCC_H
#define SYS_GCP_SCC_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) scc.h: version 2.1 created on 5/22/89 at 19:06:47	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)scc.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* @(#)head/gcp:scc.h	1.5 */
/*******************************************************************
 *
 *			S   C   C
 *
 *******************************************************************/

struct scc_reg {
#ifdef GC16
	unsigned char reg;
	unsigned char filler;
#else
	unsigned char filler;
	unsigned char reg;
#endif
};

struct scc {
	struct scc_reg reg[16];		/* 16 read/write registers */
};

/*******************************************************************
 *
 *	R E A D    R E G I S T E R S
 *
 *******************************************************************/

#define RR0_RX		0x01	/* rx ready */
#define RR0_ZERO	0x02	/* zero count */
#define RR0_TX		0x04	/* tx ready */
#define RR0_DCD		0x08	/* data carrier detect */
#define RR0_SYNC	0x10	/* sync */
#define RR0_HUNT	0x10	/* hunt */
#define RR0_RING	0x10	/* ring */
#define RR0_CTS		0x20	/* clear to send */
#define RR0_TUR		0x40	/* tx underrun */
#define RR0_EOM		0x40	/* end of message */
#define RR0_BREAK	0x80	/* break */
#define RR0_ABORT	0x80	/* abort */
#define RR0_CARR_ON(x)	((x&(RR0_DCD|RR0_RING)) == (RR0_DCD|RR0_RING))

#define RR1_ALL_SENT	0x01	/* all sent */
#define RR1_PAR		0x10	/* parity error */
#define RR1_ROR		0x20	/* rx overrun error */
#define RR1_FER		0x40	/* framming error */
#define RR1_CRC		0x40	/* crc error */
#define RR1_EOF		0x80	/* end of frame */

/*******************************************************************
 *
 *	W R I T E    R E G I S T E R S
 *
 *******************************************************************/

#define WR0_EXINT	0x10	/* reset external status interrupts */
#define WR0_ABORT	0x18	/* send abort */
#define WR0_TXINT	0x28	/* reset tx int pending */
#define WR0_ERROR	0x30	/* reset error */
#define WR0_HIUS	0x38	/* reset highest ius */
#define WR0_RXCRC	0x40	/* reset rx crc checker */
#define WR0_TXCRC	0x80	/* reset tx crc generator */
#define WR0_TXEOM	0xc0	/* reset tx underrun/eom */

#define WR1_EXTEN	0x01	/* external ints enable */
#define WR1_TXEN	0x02	/* transmitter ints enable */
#define WR1_PARSP	0x04	/* parity is special condition */
#define WR1_RX1ST	0x08	/* rx int on first or special */
#define WR1_RXALL	0x10	/* rx int on all or special */
#define WR1_RXSP	0x18	/* rx int on special only */
#define WR1_DMA_RCV	0x20	/* WAIT/REQUEST follows rx buffer */
#define WR1_DMA_REQ	0x40	/* WAIT/REQUEST function */
#define WR1_DMA_EN	0x80	/* enable WAIT/REQUEST for dma */

#define WR3_RXEN	0x01	/* receiver enable */
#define WR3_SYNC	0x02	/* sync char load inhibit */
#define WR3_ADDR	0x04	/* address search mode */
#define WR3_CRC		0x08	/* receiver crc enable */
#define WR3_HUNT	0x10	/* enter hunt mode */
#define WR3_AUTO	0x20	/* auto enables */
#define WR3_RX5		0x00	/* 5 or less rx bits */
#define WR3_RX7		0x40	/* 7 rx bits */
#define WR3_RX6		0x80	/* 6 rx bits */
#define WR3_RX8		0xc0	/* 8 rx bits */

#define WR4_PARITY	0x01	/* enable parity generation and checking */
#define WR4_ODD		0x00	/* use odd parity */
#define WR4_EVEN	0x02	/* use even parity */
#define WR4_SYNC	0x00	/* sync modes enable */
#define WR4_1STOP	0x04	/* 1 stop bit */
#define WR4_1_5STOP	0x08	/* 1.5 stop bits */
#define WR4_2STOP	0x0c	/* 2 stop bits */
#define WR4_8SYNC	0x00	/* 8 bit sync char */
#define WR4_16SYNC	0x10	/* 16 bit sync char */
#define WR4_SDLC	0x20	/* SDLC mode */
#define WR4_EXSYNC	0x30	/* use external sync */
#define WR4_1CLOCK	0x00	/* x1 clock */
#define WR4_16CLOCK	0x40	/* x16 clock */
#define WR4_32CLOCK	0x80	/* x32 clock */
#define WR4_64CLOCK	0xc0	/* x64 clock */

#define WR5_TXCRCEN	0x01	/* transmitter crc enable */
#define WR5_RTS		0x02	/* request to send */
#define WR5_CRC16	0x04	/* crc-16 */
#define WR5_TXEN	0x08	/* transmitter enable */
#define WR5_BREAK	0x10	/* send break */
#define WR5_TX5		0x00	/* 5 or less tx bits */
#define WR5_TX7		0x20	/* 7 tx bits */
#define WR5_TX6		0x40	/* 6 tx bits */
#define WR5_TX8		0x60	/* 8 tx bits */
#define WR5_DTR		0x80	/* data terminal ready */

#define WR9_VIS		0x01	/* vector include status */
#define WR9_NV		0x02	/* no vector */
#define WR9_DLC		0x04	/* disable lower chain */
#define WR9_MIE		0x08	/* master interrupt enable */
#define WR9_STHI	0x10	/* status modifies bits 4,5&6 */
#define WR9_BRST	0x40	/* channel B reset */
#define WR9_ARST	0x80	/* channel A reset */
#define WR9_HRST	0xc0	/* hardware reset */

#define WR10_6BITSYNC	0x01	/* use 6 bit sync char */
#define WR10_LOOP	0x02	/* connect TxD to RxD */
#define	WR10_ABTTXU	0x04	/* Abort on Tx Underrun */
#define WR10_MARKIDLE	0x08	/* SDLC mark idle vs flag idle */
#define WR10_ACTPOL	0x10	/* go active on poll */
#define WR10_NRZ	0x00	/* NRZ encoding */
#define WR10_NRZI	0x20	/* NRZI encoding */
#define WR10_FM1	0x40	/* FM1 encoding (transition = 1) */
#define WR10_FM0	0x60	/* FM0 encoding (transition = 0) */
#define	WR10_PRE1	0x80	/* Preset 1/0 (crc) */

#define WR11_OXTAL	0x00	/* TRxC out is xtal output */
#define WR11_OTCLK	0x01	/* TRxC out is transmit clock */
#define WR11_OBRG	0x02	/* TRxC out is br generator */
#define WR11_ODPLL	0x03	/* TRxC out is dpll output */
#define WR11_TRIO	0x04	/* TRxC is an output */
#define WR11_TRTxC	0x00	/* transmit clock from rtxc */
#define WR11_TTRxC	0x08	/* transmit clock from trxc */
#define WR11_TBRG	0x10	/* transmit clock from brg */
#define WR11_TDPLL	0x18	/* transmit clock from dpll */
#define WR11_RRTxC	0x00	/* receive clock from rtxc */
#define WR11_RTRxC	0x20	/* receive clock from trxc */
#define WR11_RBRG	0x40	/* receive clock from brg */
#define WR11_RDPLL	0x60	/* receive clock from dpll */
#define WR11_XTAL	0x80	/* xtal on RTxC pin */

#define WR14_BRGEN	0x01	/* enable baud rate generator */
#define WR14_PCLK	0x02	/* brg source is scc's pclk */
#define WR14_DTR_REQ	0x04	/* dtr vs request function */
#define WR14_AUTOECHO	0x08	/* TxD connected to RxD */
#define WR14_LCL_LOOP	0x10	/* local loopback mode */
#define WR14_SRCH	0x20	/* enter search mode */
#define WR14_RST_CLK	0x40	/* reset missing clock (FM mode) */
#define WR14_DIS_DPLL	0x60	/* disable DPLL */
#define	WR14_SRC_BRG	0x80	/* set DPLL clk src as BRG */
#define WR14_SRC_RTC	0xa0	/* set source = RTxC */
#define WR14_FM		0xc0	/* set FM mode */
#define WR14_NRZI	0xe0	/* set NRZI mode */

#define WR15_ZERO_IE	0x02	/* en intr for zero BRG count */
#define WR15_DCD_IE	0x08	/* enable carrier ints */
#define WR15_RI_IE	0x10	/* ring indicator ints */
#define WR15_SYNC_IE	0x10	/* sync ints */
#define WR15_HUNT_IE	0x10	/* hunt ints */
#define WR15_CTS_IE	0x20	/* enable cts ints */
#define WR15_EOM_IE	0x40	/* enable tx underrun ints */
#define WR15_ABORT_IE	0x80	/* enable abort ints */
#define WR15_BREAK_IE	0x80	/* enable break ints */

#endif /* SYS_GCP_SCC_H */
