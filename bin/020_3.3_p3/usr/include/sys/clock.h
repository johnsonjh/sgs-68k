#ifndef SYS_CLOCK_H
#define SYS_CLOCK_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) clock.h: version 24.1 created on 10/28/91 at 18:31:36	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)clock.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *	x1000 PTM registers
 */

#define PTM_REG1	0x01	/* select register 1, channel 2 only */
#define PTM_REG3	0x00	/* select register 3, channel 2 only */
#define PTM_RESET	0x01	/* internal reset, channel 1 only */
#define PTM_DIV8	0x01	/* divide by 8 prescale, channel 3 only */
#define PTM_ECLOCK	0x02	/* E clock pin 17 */
#define PTM_DUAL8	0x04	/* dual 8 bit counters, else single 16 */

#define PTM_IE		0x40	/* interrupt enable */
#define PTM_OE		0x80	/* output enable */

 
#define	SECHR	(60*60)		/* seconds/hr */
#define	SECDAY	(24*SECHR)	/* seconds/day */
#define	SECYR	(365*SECDAY)	/* seconds/common year */

#define TICKHZ	25000		/* PTM ticks per hz */

#endif /* SYS_CLOCK_H */
