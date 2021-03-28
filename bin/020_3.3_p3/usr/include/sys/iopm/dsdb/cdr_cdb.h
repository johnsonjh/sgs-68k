/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) cdr_cdb.h: version 24.1 created on 10/28/91 at 17:49:42	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)cdr_cdb.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/************************************************
*						*
*	cdb definition for direct device 	*
*						*
************************************************/


/************************************************
	cdb command code definitons
************************************************/


#define CDB_REZERO		0x01
#define CDB_FORMAT_UNIT		0x04
#define	CDB_REASSIGN		0x07
#define CDB_SEEK_G0		0x0B
#define CDB_INQUIRY		0x12
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
};


/* definition of flags in page 3	*/
#define	SSEC	0x80	/* soft  sector formatting	*/
#define	HSEC	0x40	/* hard sector formatting	*/
#define	RMB	0x20	/* removable media	*/
#define SURF	0x10	/* allocating surface first */
#define INS	0x08	/* inhibit save		*/


#define	RCD	0x01


/* 
 *	read extended 
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

/*
 * 	read capacity
*/

#define calba_msb	g1cdp0
#define calba_2		g1cdp1
#define calba_1		g1cdp2
#define calba_lsb	g1cdp3
#define ca_pmi		g1cdp5

