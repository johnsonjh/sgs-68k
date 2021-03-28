#ifndef SYS_VREG_H
#define SYS_VREG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) vreg.h: version 2.1 created on 5/22/89 at 19:12:48	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)vreg.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys:vreg.h	2.1"

/*
 * the icb master builds a structure in main memory that represents the
 * card cage.  the structure consists of an array of board descriptor
 * pointers and other information.
 */
struct slot_info {
	struct slot_info *next;
	struct icb *slotaddr;
	struct bd_desc *bdptr;
	unsigned short intword;
	unsigned char cmdpndg;
	unsigned char pcmd;
	unsigned char flags;
	unsigned char pad; /* assure even boundary */
};

/* defines for above flag field */
#define ICBCMD		0x01
#define ICBDP		0x02
#define BOARDDOWN	0x40
#define OCCUPIED	0x80

/*
 * when polling the card cage the ICB master writes all iop's shifted slot
 * address into their respective ICB interrupt register.  the iop when ready
 * will respond by reading the number from the register (this information is
 * necessary for the placement of 'floating' dual port ram on boards so
 * equipped). then the master polls the iop TAS register and when free expects
 * the following board descriptor in the iop's dual port memory. the structure
 * is pointed to by dual port location 0xffec (BDVECT).  if at this time the
 * master wishes to update an iop's board descriptor, he will do so and inform
 * the iop with a special single byte icb command (IVRUPDT).
 * the test and set (TAS) bit must be used whenever any processor alters a
 * field in the board descriptor.
 */

struct bd_desc {
	unsigned bd_info; /* detailed below */
	unsigned char bd_opt; /* board option */
	unsigned char bd_memopt; /* # of contiguous 64k blks of ram excluding ICB dual port */
	unsigned char bd_iocard; /* i/o card option */
	unsigned char bd_ifcard; /* interface card option */
	unsigned bd_poff; /* pointer offset, actually iop base address for dp mem */
	unsigned short sysinfsz; /* size of own_sysinfo structure */
	long *sysinfptr; /* pointer to own_sysinfo array */
	unsigned short bd_frev; /* iop's firmware revision number */
	unsigned char bd_stst; /* self test status */
	unsigned char bd_icbint; /* this board's icb interrupt level */
	char *bd_memst; /* start of iop's dual port free memory */
	unsigned bd_szmem; /* size in bytes of free memory */
	unsigned char bd_cpu; /* coded cpu information */
	unsigned char bd_slot; /* card cage slot number (0-0x1f) */
	char bd_dwnld; /* iop software flag */
	unsigned char bd_dtbid; /* dtb interface id number */
	unsigned char bd_xdtbid; /* default external dtb device interface number */
	unsigned char bd_devs; /* number of device types on board */
	struct dev_desc *bd_dvptr; /* pointer to array of device types */
	unsigned char tas1, tas2, tas3, tas4; /* test and set bytes */
	struct devq *freefst; /* linked list of free control blocks */
	struct devq *reqfst; /* list head of request control blocks */
	struct devq *reqlst; /* list tail of request control blocks */
	struct devq *rsfst; /* list head of response control blocks */
	struct devq *rslst; /* list tail of response control blocks */
	struct icbcmdhdr *bd_imcptr; /* pointer to memory resident ICB command */
};

#define BDVECT 0xffec /* offset from slot base address */

/* the following pointer macros are to be used by the ICB master */
#define PUTIOPTR(PTR,BDPTR) (((unsigned)(PTR) & 0xffff) + (BDPTR)->bd_poff)
#define GETIOPTR(PTR,SLOTBASE) (((unsigned)(PTR) & 0xffff) + ((unsigned)(SLOTBASE)))

/* defines for the above bd_info field */
#define BDIICBM   0x80000000 /* icb master */
#define BDIMSTR   0x40000000 /* master cpu board */
#define BDISLV    0x20000000 /* slave cpu board */
#define BDIDMC    0x10000000 /* dma memory controller board */
#define BDIMEM    0x08000000 /* memory board */
#define BDIIOP    0x04000000 /* IOP board */

/* defines for the bd_icbint field */
#define ICBINV    0xff /* invalid icb interrupt number */

/* defines for the self test status (bd_stst) field */
#define STERMSK   0x7f /* self test error mask */
#define STPCERR   0x01 /* prom checksum error */
#define STMERR    0x02 /* memory test failure */
#define STDVERR   0x04 /* device failure */
#define STCMPLT   0x80 /* self test complete */

/* defines for the cpu information field (bd_cpu) */
#define MOT68K    1 /* motorola 68000 */
#define Z80A      2 /* zilog z80a */

/* values for the bd_dwnld field */
#define ICBIDL    -1 /* firmware code running */
#define ICBDLD    0  /* iop is in download mode (value > 0, downloaded code running) */

/* values for bd_dwnld field for HSDT board */
#define HSDT_FORM	1	/* download hsdt sw with format/arch tape */
#define HSDT_FM2TP	2	/* download hsdt sw with format/arch/9track */

/* defines for the bd_ifcard field for HSDT board */
#define IFC_DWN		0	/* down rev hsdt interface card */
#define IFC_ARCH	1	/* arch tape/disk hsdt interface card */
#define IFC_TRK9	2	/* 9 track/disk hsdt interface card */

#define IFC_TRK9N	3	/* 9 track/disk hsdt interface card with */
				/* EC on HSDT card */

/* defines for the bd_dtbid field */
#define DTBINV   0xff /* no id number yet assigned to dtb interface */
#define NODTB    0xfe /* no dtb interface on card */
#define MAXDTB   16   /* maximum number of dtb id's */

/* defines for the bd_xdtbid field */
/*
	X X X X   X X X X b
	| | | |   | | | |
	| | | |   external dtb device id
	external dtb device type
 */
#define XDTYPMSK    0xf0  /* external dtb device type mask */
#define XDIDMSK     0x0f  /* external dtb device id mask */
#define XDTYPDMA    0x10  /* external dtb device is a dma channel */
#define XDTYPMR     0x20  /* external device is a dtb master */
#define XDTYPSL     0x40  /* external device is a dtb slave */
#define NOXDTB      0x80  /* no external device (local memory buffer) */


/* defines for the tas1 field for HSDT board */
/*
	X X X X   X X X X b
	              | |____ 1 signal to HSDT to stop all diskio
	              |______ 1 signal to HSDT to stop all tapeio
 */
#define STOPDKIO	0x01
#define STOPTPIO	0x02

		

struct dev_desc {
	unsigned short dd_type;
	unsigned short dd_count; /* number of devices of this type */
	union {
		struct virt_uart *dd_uart; /* virtual uart control structure */
		struct pd *dd_pdisk; /* physical disk structure */ 
		struct tp *dd_tape; /* tape drive structure */ 
		struct mirrdk *dd_mrdk; /* mirror disk structure */
	} dd_un;
};

/* defines for dd_type field above */
#define DTMSK    0x00ff /* device mask (256 possible devices) */
#define DTDISK   1      /* disk device */
#define DTTAPE   2      /* tape device */
#define DTMEM    3      /* memory device */
#define DTMRDK   4      /* mirror disk device */
#define DTASIO   5      /* async serial i/o device */
#define DTSSIO   6      /* synchronous serial i/o device */
#define DTCON    7      /* console device */
#define DTPIO8   8      /* 8 bit parallel i/o device */
#define DTNETW   9      /* network device */
#define DTISK    0x8000 /* device is seekable */
#define DTISER   0x4000 /* device is serial */


struct free_mem{
	struct free_mem *fm_next;
	int *fm;
};

/*
 * the following is a control structure for a block device with a
 * dtb interface
 */
struct devq {
	struct devq *q_next;
	struct devq *q_prev; /* for internal driver use only */
	unsigned char q_devtype; /* device type, defines parameters of req */
	unsigned char q_devnum; /* device number */
	unsigned char q_cmd;    /*high order bit set indicates a raw request */
	unsigned char q_priority; /* disk read/write priority requests (0=lowest) */
	union {	/* depends on the q_devtype & q_cmd */
		unsigned block;	/* device block number */
		unsigned lbuf;	/* local buffer address */
		struct {	/* disk location */
			unsigned short cyl;
			unsigned char head;
			unsigned char sector;
		} pdisk;	
		struct {	/* tape location */
			unsigned int file:8;
			unsigned int block:24;
		} ptape;
	} q_devun;

	char * q_mem;		/* main mem (or other) address 
				   if the high order bit is set in q_cmd,
				   that indicates that we will be transferring
				   to non-contiguous memory and this will be
				   the address of the mmutable structure in
				   the u_page.
				*/
	unsigned q_count;	/* number of bytes to transfer */
	unsigned q_key;	/* requestors id field, e.g. bp with unix */
	caddr_t  q_mmu;
	unsigned short q_flag; 		/* 0x01  disable retries on disk r/w 
					0x02  special open request to the disk
					0x04  don't wait for completion on
					tape rewind/tension/erase
					0x08  chain dma requests
					0x100 free request; not to be put on 
					  the response queue
					0x200 presently in use
					0x800  local memory to disk
					0x1000 disk to main memory
					0x2000 disk to local memory
					0x4000 local memory to main memory
					0x8000 main memory to local memory
					 */

	unsigned char rc1; /* return code */
	unsigned char rc2;
	unsigned char q_extdtb;
	unsigned char	opr;

	unsigned char	scnt;			/* sector count */
	unsigned char	retry;			/* retry for disk */
	struct free_mem	*local_mem;		/* local memory for data xfer*/
	unsigned char	totsleft;		/* total sector left */
	unsigned char	badsecn;
	unsigned char	sectleft;		/* sector left done */
	unsigned char	totsdone;		/* total sector done */
};
#define MAXPGNUM 0x40
struct mmu {
	unsigned int dummy;
	unsigned short mmu_off; /* offset into page */
	unsigned short mmu_page[MAXPGNUM];
	unsigned char index;
	unsigned char reserved;
};

/* device type (devq.q_devtype) use defines for dd_type */
#define DTLBUF  4 /* the device is a local memory buffer */

/* q_extdtb use defines for bd_xdtbid */
#define XDDFLT  0xff  /* use the default external dtb in bd_desc (bd_xdtbid) */

#endif /* SYS_VREG_H */
