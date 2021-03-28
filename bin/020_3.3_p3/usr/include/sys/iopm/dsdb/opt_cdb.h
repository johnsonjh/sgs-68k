/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) opt_cdb.h: version 24.1 created on 10/28/91 at 17:53:14	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)opt_cdb.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/************************************************
*						*
*	cdb definition for direct device 	*
*						*
************************************************/

#include "cdb.h"

/************************************************
	cdb command code definitons
************************************************/


#define CDB_REZERO		0x01
#define CDB_FORMAT_UNIT		0x04
#define	CDB_REASSIGN		0x07
#define CDB_SEEK_G0		0x0B
#define	CDB_START_STOP		0x1B
#define CDB_READ_CAPACITY	0x25
#define CDB_READ_EXTENDED	0x28
#define	CDB_SEEK_G1		0x2B
#define CDB_WRITE_EXTENDED  	0x2A
#define	CDB_WRVFY		0x2E
#define	CDB_VERIFY		0x2F
#define	CDB_SEARCH_HI		0x30
#define	CDB_SEARCH_EQ	 	0x31
#define	CDB_SEARCH_LO		0x32
#define	CDB_SET_LIMITS		0x33
#define	CDB_READ_DEFECT		0x37
#define	CDB_WRITE_BUFFER	0x3B
#define	CDB_READ_BUFFER		0x3C


/***************************************
	cdb structure definitions
****************************************/

/*
 *	format unit
*/
#define	interleave_msb cdp1
#define interleave_lsb cdp2

/* flags definitions */

#define	FORMAT_DATA	0x10
#define CMP_LIST	0x08
#define DEFECT_FMT	0x07


/*
 *	read/write
*/
#define lbaddr_msb	lun_flags	
#define lbaddr		cdp0		
#define lbaddr_lsb	cdp1		
#define xfer_len	cdp2

/*
 * 	mode sense
*/

#define	M_PF		0x10 	/* follow page format */

struct disk_mode_data {
	struct mode_head  mh;
	struct blk_dscptr bdp; 
};
#define	page_flags	cdp0

/* definition of page code	*/
#define	PAGE01	0x01	/* error recovery parameters */
#define PAGE02	0x02	/* Disconnect/Reconnect Control paramters */
#define PAGE03	0x03	/* Format parameters	*/
#define PAGE04	0x04	/* Rigid Disk Drive Geometry parameters	*/
#define	PAGEALL	0x3F	/* return all supported paged	*/

#define	PG_PF	0x80	/* standard page format		*/
#define PG_SP	0x01	/* save current page value except pg 3,4,5 */

/* definition of Page Control Field	*/
#define	CURRENT	0x00	/* return current values	*/
#define CHANGE	0x40	/* return changeable values	*/
#define DEFAULT 0x80	/* return default values	*/
#define SAVED	0xC0	/* return saved values		*/

struct page_hd {
	unchar	code;	/* page code */
	unchar	len;	/* size of page	*/
};
#define	PGHD_LEN	sizeof(struct page_hd)


/* page 1 -- error recovery parameters lists	*/
struct page_01 {
	unchar	err_flags;
	unchar	retry_cnt;
	unchar	correction;
	unchar	hd_offset;
	unchar	strobe;
	unchar	time_limit;
	unchar  rsv1;
	unchar	rsv2;
	unchar  rsv3;
	unchar  rsv4;
};
#define PAGE01_LEN 	sizeof(struct page_01)
/* definition of err_flags */
#define	AWRE	0x80	/* automatic write reallocation	*/
#define	ARRE	0x40	/* automatic read reallocation	*/
#define	TB	0x20	/* the failing data block transfer */
#define RC	0x10	/* read continuous	*/
#define EEC	0x08	/* enable early correction */
#define	PER	0x04	/* post error		*/
#define	DTE	0x02	/* disable transfer on error	*/
#define	DCR	0x01    /* disable correction		*/

struct	page_03 {
	unchar	tpz_msb;	/* track per zone */
	unchar	tpz_lsb;	
	unchar	aspz_msb;	/* alternate sectors per zone */
	unchar	aspz_lsb;
	unchar	atpz_msb;	/* alternate track per zone	*/
	unchar	atpz_lsb;
	unchar	alttk_msb;	/* alternate tracks	*/
	unchar	alttk_lsb;	
	unchar	spt_msb;	/* sector per track	*/
	unchar	spt_lsb;	
	unchar	bps_msb;	/* byte per sector	*/
	unchar	bps_lsb;	
	unchar	intrlv_msb;	/* interleave	*/
	unchar	intrlv_lsb;	
	unchar	tksf_msb;	/* track skew factor	*/
	unchar	tksf_lsb;		
	unchar	cylnsf_msb;	/* cylinder skew factor		*/
	unchar	cylnsf_lsb;
	unchar	flags;		/* characteristic flag 	*/
	unchar  pad[3];
};
#define PAGE03_LEN 	sizeof(struct page_03)

/* definition of flags in page 3	*/
#define	SSEC	0x80	/* soft  sector formatting	*/
#define	HSEC	0x40	/* hard sector formatting	*/
#define	RMB	0x20	/* removable media	*/
#define SURF	0x10	/* allocating surface first */
#define INS	0x08	/* inhibit save		*/

struct	page_04 {
	unchar	cylns_msb;	/* number of cylinders	*/
	unchar	cylns;		
	unchar	cylns_lsb;	
	unchar	heads;		/* number of heads */
	unchar  reserved[6];
	unchar	sprate_msb;	/* drive step rate	*/
	unchar	sprate_lsb;
	unchar	pad[6];		
};
#define PAGE04_LEN	sizeof( struct page_04)


/* 
 *	extended read / extended write 
*/
#define lbadr_msb	g1cdp0
#define lbadr_2		g1cdp1
#define lbadr_1		g1cdp2
#define lbadr_lsb	g1cdp3
#define xferlen_msb	g1cdp4
#define xferlen_lsb	g1cdp5

/*
 * 	start stop unit
*/
#define	start_flags	cdp2

#define SPIN_DOWN	0x00
#define SPIN_UP		0x01

#define LOAD_EJECT	0x02
#define NO_LOAD_EJECT	0x00

/*
 * 	read capacity
*/

#define calba_msb	g1cdp0
#define calba_2		g1cdp1
#define calba_1		g1cdp2
#define calba_lsb	g1cdp3
#define ca_pmi		g1cdp5

#define PMI	0x01	/* partial media indicator */

struct capacity {
	unchar	lba_msb;
	unchar	lba_2;
	unchar	lba_1;
	unchar	lba_lsb;
	unchar	bl_msb;
	unchar  bl_2;
	unchar  bl_1;
	unchar  bl_lsb;
};
#define CA_DATA_SIZE sizeof(struct capacity)

/* additional sense code for direct device */
#define ASNS_NOASENSE	0x00	/* No additional sense information */
#define	ASNS_NOSIGNAL	0x01	/* No index/sector signal	*/
#define	ASNS_NOSEEK	0x02	/* No seek Complete		*/
#define	ASNS_WRITE	0x03	/* Write Fault			*/	
#define	ASNS_NOREADY	0x04	/* Drive not ready		*/	
#define	ASNS_NOSELCT	0x05	/* Drive not selected		*/
#define	ASNS_NOZERO	0x06	/* No track zero found		*/
#define	ASNS_MDRVS	0x07	/* Multiple drives selected	*/
#define	ASNS_LUNCOMM	0x08	/* Logical Unit Communication Failure */
#define	ASNS_TRACK	0x09	/* Track Following error	*/
/* 0x0A - 0x0F Reserved */
#define	ASNS_IDCRC	0x10	/* ID CRC or ECC error		*/
#define	ASNS_READ	0x11	/* Unrecovered Read error of data blocks */
#define ASNS_NOMRKID	0x12	/* No address mark found in ID field */
#define ASNS_NOMRKDA	0x13	/* No address mark found in data field */
#define ASNS_NORCD	0x14	/* No record found		*/
#define ASNS_SEEKP	0x15	/* Seek Positioning error	*/
#define ASNS_SYNCMRK	0x16    /* Data Sync Mark error		*/
#define ASNS_RDRETRY	0x17    /* Recovered Read data with target's Read retries(not with ECC) */
#define ASNS_CORRECT	0x18    /* Recovered Read data with target's ECC correction (not with retries) */
#define ASNS_DFCTERR	0x19    /* Defect list error		*/
#define ASNS_PAROVRN	0x1A    /* Parameter overrun		*/
#define ASNS_SYNCXFR	0x1B    /* Synchronous transfer error	*/
#define ASNS_NOPDFCT	0x1C    /* Primary defect list not found */
#define ASNS_CMPERR	0x1D    /* Compare error		*/
#define ASNS_IDECC	0x1E    /* Medium error or Recovered error */
/* 0x1F reserved	*/
#define ASNS_INVALCOC	0x20    /* Invalid Command Operation Code */
#define ASNS_ILLLBA	0x21    /* Illegal logical block address */
#define ASNS_ILLFUNC	0x22    /* Illegal function for device type */
/* 0x23 reserved	*/
#define ASNS_ILLFLD	0x24	/* Illegal field CDB		*/
#define ASNS_INVALLUN	0x25	/* Invalid LUN			*/
#define ASNS_INVALFLD	0x26    /* Invalid field in Parameter list */
#define ASNS_WRTPTCT	0x27    /* Write proctected		*/
#define ASNS_MDMCHNG	0x28	/* Medium Changed		*/
#define ASNS_BUSRESET	0x29	/* Power On or Reset or Bus device Reset */
#define ASNS_MDCHNG	0x2A	/* Mode Select Parameters changed	*/
/* 0x2B - 0x2F Reserved	*/
#define ASNS_INCCRTDGE  0x30	/* Incompatible Cartridge	*/
#define ASNS_MDMFMT	0x31	/* Medium format corrupted	*/
#define ASNS_NOALT	0x32	/* No defect spare location available */
/* 0x33 - 0x3F Reserved		*/
#define ASNS_RAMERR	0x40	/* RAM failure			*/
#define ASNS_DPTHDIAG	0x41    /* Data path diagnostic failure */
#define ASNS_PWRDIAG	0x42	/* power on diagnostic failure	*/
#define ASNS_MSGRJCT	0x43	/* Message Reject error		*/
#define ASNS_CTLRERR	0x44	/* Internal Controller error	*/
#define	ASNS_SLCTFAIL	0x45	/* Select/reselect failed	*/
#define	ASNS_SRESETERR	0x46    /* Unsuccessful Soft Reset	*/
#define	ASNS_SCSIPARITY 0x47	/* SCSI interface parity error 	*/
#define ASNS_INITDTCERR 0x48	/* Initiator detected error	*/
#define ASNS_ILLMSG 	0x49	/* Inappropriate/illegal Message */
/* 0x4A - 0x7F Reserved */
/* 0x80 - 0xFF Vendor Unique */
