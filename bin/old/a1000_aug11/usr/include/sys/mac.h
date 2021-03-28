#ifndef SYS_MAC_H
#define SYS_MAC_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mac.h: version 2.1 created on 5/22/89 at 19:08:28	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mac.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*
 * mac.h
 *
 *		M A C - Multibus Adapter Card (aka, MBA)
 *
 * description of MAC board and definitions to that affect
 */

#ident	"@(#)uts/head/sys:mac.h	2.1"

struct mac_int {
	short type;	/* interrupt type */
	short value;	/* interrupt value */
};

/*  structure to encompass entire 64 KB "realm" of MAC
 *  NOTE: put user-specific mbios I/O space definitions in separate header file
 */
struct mac {
	char	shram[0x7ffc];	/* shared ram (static)                      */
	struct	mac_int intrpt;	/* interrupt synchronization                */
	char	mbram[0x4000],	/* multibus external ram (may not be there) */
		mbios[0x3ff8];	/* multibus I/O space (may not be there)    */
	unsigned
	short	intreg,		/* interrupt control register (WO)          */
		basreg,		/* ram base address registers (WO)          */
		stsreg,		/* status register (RO)                     */
		rstreg;		/* reset register (WO)                      */
};

/*
 * MAC ioctl call definitions
 */
#define	MAC_INTREG	0	/* ioctl( fd, MAC_INTREG, mask )         */
#define	MAC_BASREG	1	/* ioctl( fd, MAC_BASREG, &short_int )   */
#define	MAC_STSREG	2	/* ioctl( fd, MAC_STSREG, &short_int )   */
#define	MAC_RESET	3	/* ioctl( fd, MAC_RESET )                */
#define	MAC_WAIT	4	/* ioctl( fd, MAC_WAIT )                 */
#define	MAC_CLRINT	5	/* ioctl( fd, MAC_CLRINT )               */
#define	MAC_ATTACH	6	/* ioctl( fd, MAC_ATTACH, mac_instance ) */
#define	MAC_DETACH	7	/* ioctl( fd, MAC_DETACH )               */
#define	MAC_RW		8	/* ioctl( fd, MAC_RW, &var )       */

/* MAC_RW info struct */
struct mac_rw_info {
	char *vaddr;
	unsigned short mtflags;
	unsigned short mtaddr;
};

/* MAC_RW defines */
#define MAC_RW_READ	0x0
#define MAC_RW_WRITE	0x1
#define MAC_RW_BYTE	0x0
#define MAC_RW_SHORT	0x2
#define MAC_RW_TYPE	0xC
#define MAC_RW_RAM	0x0
#define MAC_RW_IO	0x4
#define MAC_RW_SHRAM	0x8

/*******************************************************************************
 *
 *  intreg bit definitions (WO):
 *
 *	+-------------------------------+-------------------------------+
 *	|   | 4 | 2 | 1 |   |   |   |   |          unused byte          |
 *	+-^---^---^---^---^---^---^---^-+-------------------------------+
 *	  |   |   |   |   |   |   |   |
 *	  |   +---+---+   |   |   |   Motorola(0)/Intel(1)
 *	  |       |       |   |   LED 0
 *	  |       |       |   LED 1
 *	  |       |       (unused)
 *	  |       interrupt level
 *	  interrupt enable
 */
#define	MACINTENB	0x8000
#define	MACINTPRI	(((VECMAC+1) & 7) << 12)
#define	MACGRNLED	0x0400		/* turn on Green LED */
#define	MACREDLED	0x0200		/* turn on Red LED */
#define	MACYLWLED	0x0000		/* turn on Yellow LED */
#define	MACINTEL	0x0100		/* swap bytes */
#define	MACMTRLA	0x0000		/* don't swap bytes */

/*******************************************************************************
 *
 *  basreg bit definitions (WO):
 *
 *	+-----------------------+-----------------------+
 *	|21|20|19|18|17|16|15|  |21|20|19|18|17|16|15|14|
 *	+-----------------------+-----------------------+
 *	 shared RAM base addr    IEEE-796 mem base addr
 *
 * ICB access to shared RAM (IEEE-796 address):
 *  +-----------------------------------------------------------------------+
 *  |23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 *  |0 |0 |shared RAM base addr|               ICB address               |  |
 *  +-----------------------------------------------------------------------+
 *
 * ICB access to IEEE-796 RAM (IEEE-796 address):
 *  +-----------------------------------------------------------------------+
 *  |23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 *  |0 |0 |   IEEE-796 base addr  |            ICB address               |  |
 *  +-----------------------------------------------------------------------+
 *
 * ICB access to IEEE-796 I/O (IEEE-796 address):
 *  +-----------------------------------------------------------------------+
 *  |23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 *  |0 |0 |0 |0 |0 |0 |0 |0 |0 |0 |            ICB address               |  |
 *  +-----------------------------------------------------------------------+
 *
 * IEEE-796 access to shared RAM (IEEE-796 address):
 *  +-----------------------------------------------------------------------+
 *  |23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
 *  |0 |0 |shared RAM base addr|            IEEE-796 address             |  |
 *  +-----------------------------------------------------------------------+
 */

/*******************************************************************************
 *
 *  stsreg bit definitions (RO):
 *
 *	+----------------------+-----------------------+
 *	|0 |  |0 |  0 1 0 0 0  |      unused byte      |
 *	+---^-----^------------+-----------------------+
 *	    |     |
 *	    |     board type (identifies the board as a MAC)
 *	    interrupt received/pending (to CPU)
 */
#define	MACGETBRD(x)	(((x) & 0x1f00) >> 8)
#define	MACINTPDG(x)	((x) & 0x4000)

/*******************************************************************************
 *
 *  rstreg bit definitions (WO):
 *
 *	+-----------------------+-----------------------+
 *	|  |             unused (15 bits)               |
 *	+^----------------------+-----------------------+
 *	 |
 *	 reset board (must be asserted for >= 5ms)
 */
#define	MACRESET	0x8000

#endif /* SYS_MAC_H */
