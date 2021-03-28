#ifndef SYS_RTB_H
#define SYS_RTB_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) rtb.h: version 2.1 created on 5/22/89 at 19:10:11	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)rtb.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	rtb.h	3.13	*/
/*   Arete/5.0   6/30/84   /usr/src/head/sys/rtb.h   */

#ident	"@(#)uts/head/sys:rtb.h	2.1"

/*
 *	R T B    S T A T U S    R E G
 */

#define RTB(n)		(0x60fffc + (n << 16))
#define RTB_I(n)	(0x60fffe + (n << 16))
#define RTB_D(n)	(0x60fffa + (n << 16))
#define RTB_IBUSY(n)	((n >> 15) & 0x1)
#define RTB_DBUSY(n)	((n >> 13) & 0x1)

#define RTB_VR0(n)	(unsigned short *)(0x60fff0 + (n << 16))
#define RTB_VR1(n)	(unsigned short *)(0x60fff2 + (n << 16))
#define RTB_VR2(n)	(unsigned short *)(0x60fff4 + (n << 16))
#define RTB_VR3(n)	(unsigned short *)(0x60fff6 + (n << 16))

#define RTBCHEAD(n)	(0x600200 + (n << 16))
#define RTBTTYS(n)	(0x60020c + (n << 16))
#define RTBTTY(n,m)	(0x60020c + (n << 16) + (m << 2))

#define RTB_RCVD 0x8000
#define RTB_PNDG 0x4000
#define RTB_TYPE(n) ((n >> 8) & 0x1f)
#define RTB_TAS 0x0080
#define RTB_PERR 0x0040
#define RTB_PROM 0x0020
#define RTB_SLOT(n) (n & 0x1f)
#define SLOTS 0x20

/*
 *	R T B    I N T E R R U P T    R E G
 */

#define RTB_RST 0x8000
#define RTB_NMI	0x4000
#define RTB_VI  0x2000
#define RTB_CHAN(n) ((n >> 8) & 0x7)
#define RTB_COMM(n) (n & 0xff)

/*
 * various settable icb board interrupt priorities
 * (requires vreg protocol)
 */
#define	HSDT_PRI	1	/* HSDT board */
#define	GCP_PRI		2	/* GCP board */

extern ushort rtb_dtack;

union rtb {
	struct {
		ushort r_stat;
		ushort r_int;
	} rtbw;
	struct {
		char r_stathi;
		char r_statlo;
		char r_inthi;
		char r_intlo;
	} rtbb;
};

#endif /* SYS_RTB_H */
