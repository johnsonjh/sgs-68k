#ifndef	SYS_TYD_UART_H
#define SYS_TYD_UART_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tyd_uart.h: version 24.1 created on 10/28/91 at 18:40:18	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tyd_uart.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *	uart.h
 *
 *	description of registers and bits for the COM78808 Standard
 *	Microsystems Corp. 8-channel Universal Asynchronous
 *	Receiver/Transmitter Octal UT
 */
/*------------------------------------------------------------------*/
#define UT_RECEIVE	0		/* receive buffer */
#define UT_TRANSMIT	0		/* transmitter holding buffer */
#define UT_STATUS	1		/* status register */
#define UT_MODE		2		/* mode registers 1 and 2 */
#define UT_COMMAND	3		/* command register */

#define UT_OFFSET	8		/* offset to next port on uart */

#define UT_DTR_OFFSET	0x80		/* offset to octart dtr register */
#define UT_LOOP_OFFSET	0x81		/* offset to octart loopback register */
/*------------------------------------------------------------------*/
/*
 *	status register (UT_STATUS) bits
 */
#define UT_STATUS_DSR	0x80		/* data set ready (DSR)*/
#define UT_STATUS_DCD	0x40		/* data carrier detect (DCD) */
#define UT_STATUS_FER	0x20		/* framing error */
#define UT_STATUS_ORR	0x10		/* overrun */
#define UT_STATUS_PER	8		/* parity error */
#define UT_STATUS_TxE	4		/* transmitter empty */
#define UT_STATUS_RR	2		/* receiver ready */
#define UT_STATUS_TR	1		/* transmitter ready */
/*------------------------------------------------------------------*/
/*
 *	mode register 1 (UT_MODE) bits
 */
#define UT_MODE_STOP2	0xc0		/* 2   stop bits */
#define UT_MODE_STOP15	0x80		/* 1.5 stop bit */
#define UT_MODE_STOP1	0x40		/* 1   stop bit */

#define UT_MODE_PAR_E	0x30		/* even parity */
#define UT_MODE_PAR_O	0x10		/* odd  parity */
#define UT_MODE_PAR_N	0x00		/* no   parity */

#define UT_MODE_CS8	0x0c		/* 8-bit byte */
#define UT_MODE_CS7	8		/* 7-bit byte */
#define UT_MODE_CS6	4		/* 6-bit byte */
#define UT_MODE_CS5	0		/* 5-bit byte */

#define UT_MODE_RSRV	2		/* reserved */

#define UT_MODE_MCIE	1		/* modem control interrupt enable 
					   (get interrupt on eia change) */
/*------------------------------------------------------------------*/
/*
 *	mode register 2
 *
 *	this is the baud rate register. the transmitter and the receiver
 *	can have separate speeds; the transmitter is the upper 4 bits
 *	and the receiver is the lower 4 bits
 *
 *	the baud rates this chip can provide are almost the same as those
 *	that can be sent via an ioctl, except that the chip has 2000 baud
 *	but not 200 baud, while 200 baud can be requested via an ioctl, but
 *	not 2000 baud.
 */
/*------------------------------------------------------------------*/
/*
 *	command register (UT_COMMAND) bits
 */
#define UT_CMD_OMRL	0xc0		/* operating mode = remote loopback */
#define UT_CMD_OMLL	0x80		/* operating mode = local loopback */
#define UT_CMD_OMAE	0x40		/* operating mode = automatic echo */
#define UT_CMD_OMNO	0		/* operating mode = normal operation */
#define UT_CMD_RxIE	0x20		/* receiver interrupt enable */
#define UT_CMD_RERR	0x10		/* reset error */
#define UT_CMD_TxBRK	8		/* transmit break */
#define UT_CMD_RxEN	4		/* receiver enable */
#define UT_CMD_TxIE	2		/* transmitter interrupt enable */
#define UT_CMD_TxEN	1		/* transmitter enable */
/*------------------------------------------------------------------*/
#endif	/* SYS_TYD_UART_H */
