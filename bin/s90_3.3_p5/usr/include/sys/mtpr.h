#ifndef SYS_MTPR_H
#define SYS_MTPR_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mtpr.h: version 24.1 created on 10/28/91 at 18:35:53	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mtpr.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	mtpr.h	3.13	*/
/*   Arete/5.0   6/30/84   /usr/src/head/sys/mtpr.h   */


#define MTPR(port, val) *port = val	/* move to processor register */
#define MFPR(port) *port		/* move from processor register */

#define RXCS	(char *) 0x420011	/* console receiver control and status */
#define RXDB	(char *) 0x420013	/* console receiver data buffer */
#define TXCS	(char *) 0x420011	/* console transmitter control and status */
#define TXDB	(char *) 0x420013	/* console transmitter data buffer */

#define PTMC13	(char *) 0x420001	/* ptm control channels 1 and 3 */
#define PTMC2	(char *) 0x420003	/* ptm control channel 2 */
#define PTMSTAT	(char *) 0x420003	/* ptm status */
#define PTM1MSB	(char *) 0x420005	/* ptm msb channel 1 */
#define PTM1LSB	(char *) 0x420007	/* ptm lsb channel 1 */
#define PTM2MSB	(char *) 0x420009	/* ptm msb channel 2 */
#define PTM2LSB	(char *) 0x42000b	/* ptm lsb channel 2 */
#define PTM3MSB	(char *) 0x42000d	/* ptm msb channel 3 */
#define PTM3LSB	(char *) 0x42000f	/* ptm lsb channel 3 */
#define PTM1CNT	(char *) 0x420005	/* count channel 1 */
#define PTM2CNT	(char *) 0x420009	/* count channel 2 */
#define PTM3CNT	(char *) 0x42000d	/* count channel 3 */

#define LEDS	(char *) 0x4e0003	/* led port */
#define CONFIG	(char *) 0x4e0003	/* configuration switches */

#endif /* SYS_MTPR_H */
