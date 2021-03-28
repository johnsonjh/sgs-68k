/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) cdr_ds.h: version 24.1 created on 10/28/91 at 17:49:46	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)cdr_ds.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include <sys/types.h>
#include <sys/param.h>

#define	CDDRIVER	0
#define	MAXDRVS		16
#define	SUCCESS		1
#define FAILURE		0
#define ISINIT		1
#define NOTINIT		0
#define NULL		0
#define CD_SONY		1
#define CD_LMS		2

#define NBPSCTR		1024

/* cd_flags in state table */
#define CD_OPEN		0x0001
#define CD_ONCE		0x0002
#define CD_OPNIN_CLSIN	0x0004
#define CD_BUSY		0x0008
#define CD_WANTED	0x0010
#define CD_MEDIA_CH	0x0020
#define CD_BAILING_OUT  0x0040
#define CD_RESET_ERROR	0x0080
#define CD_IGNORE_ERR	0x0100		/* ignore error message from llvl */

/* cd_openmap in state table */
#define CDR_ANYOPEN 	0x0007
#define CDR_BLK_OTYP	0x0001
#define CDR_CHR_OTYP	0x0002
#define CDR_MNT_OTYP	0x0004

/* reset status states */
#define CD_TIMEOUT	1
#define CD_A_RESET	2
#define CD_RESET_DONE	8

#define RDCP_TIMER	( 60 * HZ)
#define DEF_TIMER	( 50 * HZ)

#define SHFT24		24
#define SHFT16		16
#define SHFT8		8
#define SHFT4		4

#define GETSTBL(x)	&cdr_state[x]
#define UNIT(x)		((x)>>4)&0x0F

#define MAXRETRY	15
#define BUSY		1
#define NOTBUSY		0

#define CMD_REGULAR	0 	/* used to set command type when building
				 * command mailbox for llvl driver
				 */

#define b_cmd 		b_blkno
#define b_sblock	av_back

/*
 * data structures for scsi cd-rom driver
 */

struct statetbl {
	struct buf 	*cd_head ;
	struct buf  	*cd_tail ;
	uint		cd_flags ;
	int		cd_busy ;
	struct cmbox 	*cd_cmbox ;
	/* information about the current cd in the drive 		*/
	int cd_max_blks ;	/* maximum number of blocks on this cd  */
				/* as absolute blkno = first bad	*/
	int cd_log_blk_size ;	/* block size this cd: 512, 1024, 2048
				   2340, 2336  				*/ 
	uint cd_start_bit ;	/* 1 = start; 0 = stop; used to spin 
					up/down drive                   */
	uint cd_openmap ; 	/* bit map of open types		*/
	int cd_vendor ;
	int cd_reset_state ;	/* used with reset; indicates state to be
				   in upon entering cdr_reset routine 	*/
	int cd_retry_cnt ;	/* retry count for command associated with
				   cd_head 				*/
	int cd_rst_retry ;	/* retry count for attempts to recover from
				   bus reset 				*/

	unchar 	cd_dev ;
} ;

typedef struct statetbl STTBL ;

/* data structure to define return values for read capacity command */

struct capacity {
	unchar	lba_msb ;
	unchar	lba_mid0 ;
	unchar  lba_mid1 ;
	unchar  lba_lsb ;
	unchar  bl_msb ;
	unchar  bl_mid0 ;
	unchar  bl_mid1 ;
	unchar  bl_lsb ;
} ;

/* data structure to define return values for inquiry command */

struct inquiry {
	unchar i_dev_type ;
	unchar i_qualifier ;
	unchar i_version ;
	unchar i_resp_format ;
	unchar i_len ;
	unchar i_res[3] ;
	unchar i_vendor[8] ;
	unchar i_product[16] ;
	unchar i_revision[4] ;
} ;

#define DEV_CDROM	0x05
	
/* from open.h which doesn't compile */
#define OTYPCNT		5
#define	OTYP_BLK	0
#define	OTYP_MNT	1
#define	OTYP_CHR	2
#define	OTYP_SWP	3
#define	OTYP_LYR	4
