#ifndef SYS_ICB_H
#define SYS_ICB_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) icb.h: version 2.1 created on 5/22/89 at 19:07:35	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)icb.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	icb.h	1.1	*/
/*   	Arete/5.0   6/30/84   /usr/src/head/sys/icb.h   */

#ident	"@(#)uts/head/sys:icb.h	2.1"

#define MAXSLOTS 19
#define MAXIOPS 12

/*
 *	board types
 */

#define	MAXVAC		16
#define MAXEDT		4
#define MAXLEVEL	4

#define CPU 		0
#define MASTERCPU 	0
#define SLAVECPU  	1
#define DMC 		2
#define DTC 		3
#define ASP 		4
#define HSDT 		5
#define MEM 		6
#define GCP 		7
#define	MAC 		8
#define DMC4 		9
#define MASTERCPU20 	10
#define SLAVECPU20  	11
#define EDT  		12
#define DMC4_2		13
#define EGCP 		14
#define MEM32 		15
#define SCSIEDT		16
#define VAC		17
#define MAXTYPES	18

/*
 *	board vectors
 */

#define VECDTC 0
#define VECHSDT 0
#define VECASP 1
#define VECGCP 1
#define VECMAC 1
#define VECVAC 1
#define VECDMC 2
#define VECCPU 3

#define	ICBINVAL	0x00
#define	ICBVALID	0x01		/* iocp is valid icb slot */
#define	ICBACTIV	0x02		/* iocp is executing out of ram */
#define	ICBPRFEN	0x04		/* iocp code is profilable */
#define	ICBREAD		0x08		/* iocp memory is readable */
#define	ICBWRITE	0x10		/* iocp memory is writable */
#define	ICBEXEC		0x20		/* iocp has a processor */
#define	ICBVREG		0x40		/* iocp has vreg protocol */
#define	ICBDPMEM	0x80		/* iocp has dual port memory */
#define	ICBBSY		0x100		/* iocp interrupt sync bit */
#define	ICBWAIT		0x200		/* iocp is busy user waiting */
#define	ICBERROR	0x400		/* iocp error in communication */
#define	ICBDWNLD	0x800		/* iocp error in communication */
#define ICBTIMEO	0x1000		/* iocp timeout still in effect */

struct icb {
	int status;			/* status of icb slot */
	struct icb *nextvec;
	int (* service)();		/* interrupt service routine */
	int (* proc)();
	union rtb *rtb;
	unsigned pc;			/* pc on every clock tick */
	unsigned ps;			/* ps on every clock tick */
	struct icbcmdhdr *ichdr;	/* icb command struct ptr */
	struct bd_desc *bdptr;		/* board descriptor pointer */
	unsigned long ramsiz;		/* contiguous ram (non dual port) */
	unsigned long dpmem;		/* iocp address for dual port ram */
	char slot;			/* icb slot number */
	char type;			/* type of board */
	char board;			/* nomadic number of type */
	char serno;			/* software serial no. */
};

/* defines for ip.serno */
#define	GCPSERIAL	0xb		/*    "        "     "   "  gcp */

#define ICB_BASE(n) (0x600000 + (n << 16))
#define	ICBDPMEMSZ	0x10000		/* size of dual port memory */


/******************************************************************************
 *
 * Below are the defines for the ICB command mechanism
 * Single byte commands are passed in the ICB interrupt register.
 * ICB command packets may be passed byte at a time over the ICB int reg or
 * laid down in ICB dual port memory.
 *
 ******************************************************************************/

/* ICB command header structure */
struct icbcmdhdr {
	unsigned char command;
	unsigned char status;
	unsigned char destination;
	unsigned char source;
	unsigned information;
	unsigned short count;
};

/* packet ICB commands */
#define ISTSCMD  0x01 /* self test status command */
#define IPUTCMD  0x02 /* put character command */
#define IGETCMD  0x03 /* get character command */
#define ILEDCMD  0x04 /* write LED command */
#define ICONFCMD 0x05 /* read configuration command */
#define IWMEMCMD 0x06 /* write memory command */
#define IRMEMCMD 0x07 /* read memory command */
#define IJMPCMD  0x08 /* jump command */
#define IGTASCMD 0x09 /* get TAS command */
#define ICTASCMD 0x0a /* clear TAS command */
#define ICONCMD  0x0b /* console command */
#define IWJCMD   0x0c /* write memory and jump command */

/* ICB driver direction commands */
#define	IWDATA	0x0f	/* write the data to the iocp */

/* packet ICB status */
#define ICMPLT   0x01 /* command complete */
#define IBDEST   0x02 /* bad destination slot address */
#define IILLEG   0x03 /* command not supported */
#define IPARAM   0x04 /* parameter out of range */
#define IABRTD   0x05 /* command aborted */
#define ICBBUSY  0x06 /* ICB busy */

/* single byte ICB commands */
#define ICBSBCMD 0x80 /* icb single byte command bit */
#define IMEMCMD  0x81 /* process a memory resident command packet */
#define IDWNLD   0x82 /* go into download mode */
#define IABORT   0x83 /* abort current icb command */
#define IVRUPDT  0x84 /* the virtual registers have been updated */
#define IDEVREQ  0x85 /* process a devq request */

/* kernel ICB commands */
#define ICBGET	 	0x86 /* read kernel icb struct */
#define	ICBDWLD	 	0x87 /* put iocp into download mode */
#define ICBPROC	 	0x88 /* start up iocp specific driver */
#define	ICBRESET 	0x89 /* reset the board and kernel stuff
				to something sane */
#define ICBSTOP_TAPE 	0x8a /* stop processing tape requests */
#define ICBSTOP_IO 	0x8b /* stop processing disk and tape requests */
#define ICBCPUSPD	0x8c /* get the CPU speed table */

/* miscellaneous */
#define IPAKSIZ   256 /* max data pack size for ICB register commands */
#define ICBMSTR  0x20 /* use when ICB master slot is unknown */
#define MAXSLOT  0x20 /* maximum number of slots possible */
#define HDRSIZ   sizeof(struct icbcmdhdr)
#define TOTPAK   HDRSIZ+IPAKSIZ
#define FILLHDR(P,C,ST,D,S,I,CT) {P->command = C; P->status = ST; \
P->destination = D; P->source = S; P->information = I; P->count = C;}


#endif /* SYS_ICB_H */
