/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) cdb.h: version 24.1 created on 10/28/91 at 17:52:37	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)cdb.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/************************************************
*						*
*	SCSI cdb definitions for all devices	*
*						*
************************************************/

#define CCS_NO_CMD	0xFF	/* this is not css command */

/*************************************************
	scsi commmand code for all devices
*************************************************/

/* CCS Required Commands	*/
#define	CDB_TEST_UNIT_READY	0x00
#define CDB_REQUEST_SENSE	0x03
#define	CDB_READ		0x08
#define CDB_WRITE		0x0A
#define CDB_INQUIRY		0x12
#define CDB_MODE_SELECT		0x15
#define CDB_RESERVE_UNIT	0x16
#define CDB_RELEASE_UNIT	0x17
#define CDB_COPY		0x18
#define	CDB_MODE_SENSE		0x1A
#define	CDB_RECEIVE_DIAG	0x1C
#define CDB_SEND_DIAG		0x1D
#define CDB_MEDIUM_REMOVAL	0x1E
#define CDB_COMPARE		0x39
#define	CDB_CPVFY		0x3A


/***************************************
	the common cdb structure 
****************************************/

/*
 *	group 0 cdb
*/
struct g0_cdb {
	unchar opcode;  /* Group Code + Command Code 		*/
	unchar lun_flags;     /* first three bits are logical unit no. */
	unchar cdp0;	/* the next 3 fields are command dependent */
	unchar cdp1;
	unchar cdp2;
	unchar ctl;	/* control code such as Link and Flag */
};


/*
 *	group 1 cdb
*/
struct g1_cdb {
	unchar opcode; 	/* Group Code + Commnad Code	*/
	unchar lun_flags; /* lun + flags			*/
	unchar g1cdp0;	  /* command dependent fields		*/
	unchar g1cdp1;	  
	unchar g1cdp2;	 
	unchar g1cdp3;	
	unchar g1_res;	  /* reserved				*/
	unchar g1cdp4;
	unchar g1cdp5;	
	unchar ctl;	
};


/*
 *	group 5 cdb
*/
struct g5_cdb {
	unchar opcode; 	/* Group Code + Commnad Code	*/
	unchar lun_flags; /* lun + flags			*/
	unchar g5cdp0;	  /* command dependent fields		*/
	unchar g5cdp1;	  
	unchar g5cdp2;	 
	unchar g5cdp3;	
	unchar g5_res0;	  /* reserved				*/
	unchar g5_res1;	  /* reserved				*/
	unchar g5_res2;	  /* reserved				*/
	unchar g5cdp4;
	unchar g5cdp5;	
	unchar ctl;	
};

/* the group number of cdb definitions in opcode */
#define GRPMASK			0xE0
#define GRP0			0x00
#define GRP1			0x20
#define GRP5			0xA0


/* size of each group cdb */ 

#define	G0SIZE	sizeof(struct g0_cdb)
#define G1SIZE  sizeof(struct g1_cdb)
#define G5SIZE  sizeof(struct g5_cdb)
	

#define alloc_len	cdp2	/* redefine cdp2 for some basic command */




/* device type definition */
#define	 DA_DEV	0x00	/* direct-access device */
#define	 SA_DEV 0x01	/* sequential-access device */
#define	 PR_DEV 0x02	/* printer device */
#define	 PC_DEV	0x03	/* processor device	*/
#define  WO_DEV 0x04	/* write-once read-multiple device */
#define	 RO_DEV	0x05	/* read-only direct-access device */


/*
	 inquiry_data  
*/

/* using g0_data_cdb for command */

struct inquiry_data {
		unchar	dev_type;
		unchar	dev_qual;
		unchar	version;
		unchar	rep_fmt;
		unchar	len;
		unchar	reserved[3];
		unchar	vdr_id[8];
		unchar	prod_id[16];
		unchar	rev_level[4];
		unchar	blk_size[2];
		unchar	blk_track;
		unchar	blk_side;
} ;
	
typedef	struct inquiry_data	INQURY;	

#define	INQUIRY_LEN 	sizeof( INQURY)
#define QUAL_RMB	0x80		/* qualifier drive with removable media */



/* 
 *	mode select/mode sense command
*/

/* using g0_data_cdb */

/* definition of mode sense data */
 
struct mode_head {
	unchar	sd_len;		/* sense data length */
	unchar	med_type;	/* medium type 		*/
	unchar	wp_bm_speed;	/*  write protected,buffer mode and speed */
	unchar	dscptr_len;  	/* block descriptorlength	*/
};

#define BM	0x40		/* buffer mode */
#define WP	0x80		/* write protect */

#define	MODEHEAD_LEN	sizeof(struct mode_head)


/* block descriptor in mode select/mode sense */

struct blk_dscptr {
	unchar	density;	/* density code	*/
	unchar	numblk_msb;	/* number of block	*/
	unchar	numblk;		
	unchar	numblk_lsb;	
	unchar	pad;		/* reserved	*/
	unchar	blk_len_msb;	/* block length	*/
	unchar	blk_len;
	unchar	blk_len_lsb;	
};

#define DSCPTR_LEN	sizeof(struct blk_dscptr)

/*
 * request sense 
*/

/* using g0_data_cdb struct */

/* sense data structure */

typedef struct {
        unchar err_class;
	unchar segment;
	unchar sense_key;
	unchar info_msb;
	unchar info_2;
	unchar info_1;
	unchar info_lsb;
	unchar xsense_len;
	unchar pad[4];
	unchar err_code;
	unchar resv1[1];
	unchar frucode;
	unchar flags;
	unchar field_hi;
	unchar field_lo;
	unchar resv[8];
} EX_SENSE_DATA;

#define SENSE_DATA_SIZE	sizeof(EX_SENSE_DATA)

typedef struct {
	unchar err_class;
	unchar lba_msb;
	unchar lba;
	unchar lba_lsb;
} SENSE_DATA;


/* definition of err_class */
#define INFO_VALID	0x80	/* information bytes is valid */
#define ERR_CLASS_MASK  0x70	/* error class field */
#define ERR_CODE_MASK	0x0F
#define ANSI_CLASS	0x70	/* error class 7 */

/* definition of sense byte */
#define FMD		0x80	/* filemark */
#define EOMD		0x40	/* eom */
#define ILI		0x20	/* incorrect length indicator */
#define SENSE_KEY	0x0F	/* mask for sense key */

/* SCSI sense key definition */
#define	SNSKEY_NO_SENSE		0x00
#define SNSKEY_RECOVERED_ERR 	0x01
#define SNSKEY_NOT_READY	0x02
#define SNSKEY_MEDIUM_ERR	0x03
#define SNSKEY_HARD_ERR		0x04
#define SNSKEY_ILL_REQUEST	0x05
#define SNSKEY_UNIT_ATTN	0x06
#define	SNSKEY_DATA_PROTECT	0x07
#define SNSKEY_BLANK_CHECK	0x08
#define	SNSKEY_UNKNOWN_ERR	0x09 
#define	SNSKEY_COPY_ABORT	0x0A
#define	SNSKEY_ABORTED_CMD	0x0B
#define	SNSKEY_EQUAL		0x0C
#define SNSKEY_VOL_OVERFLOW	0x0D
#define SNSKEY_MISCOMPARE	0x0E

/* definition  of flags in extension of sense data */
#define FPV		0x80	/* Field Pointer valid bit */
#define CDBIT		0x40	/* 1: CDB byte pointer 0: data byte
				   pointer */
#define BPV		0x08	/* bit pointer valid bit */
#define BIT_POINTER_MASK 0x07	/* bit pointer field */

#define DRV_LUN		0	/* target's logical unit number */

#define IBM_FMT_CORRPT	0x31	/* media format corrupted */
/* 
 * test unit ready
 */

/* using g0_cdb struct */
/*
 * prevent/allow media removal
*/

#define prvnt 	cdp2

/* prvnt definition */
#define ALLOW		0x00
#define PREVENT		0x01
