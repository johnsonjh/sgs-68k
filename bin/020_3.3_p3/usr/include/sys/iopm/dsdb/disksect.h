/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) disksect.h: version 24.1 created on 10/28/91 at 17:50:23	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)disksect.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* ****************** */
/* layout of sector 0 */
/* ****************** */

#define FORMAT_LEVEL id[7]
#define DSETUP_LEVEL id[6]

#define LOGDR		16		/* # of logical dr within phy drive */
struct edt_sector0
{
	char	id[8];			/* INIT name */
					/* id[7] = 1 indicates skew format */
					/* id[6] is dsetup compatible check */
	char	date[8];		/* format date */
					/* date[0],date[1] = version num */
					/* date[2]-date[7] = ascii date code */

	unsigned short	dummy[2];

	unsigned short	trkbpsg;	/* #of bits/sec+gap length */
	unsigned short	ltrkbpsg;	/* #of bits in last sec+gap length */


	unsigned short	cyldisk;	/* # of cyls on the disk */
	unsigned char	headcyl;	/* # of heads per cyl */
	unsigned char	sechead;	/* # of sectors per head */
	unsigned short	seccyl;		/* # of sectors per cyl */
	unsigned short	bytesec;	/* # of bytes per sector */

	unsigned char	gap[32];	/* timing data */

	unsigned char 	pd_ldmaxnum;	/* max num of log drive */
	unsigned char	pd_ldnum;	/* current num of log drive */
	unsigned char   rec_interlace;  /* recommended disk interlace for mkfs*/
	unsigned char	isscsi;		/* is this SCSI device (880708 cjk) */
	unsigned	secdisk;	/* # of sectors per disk (880708 cjk) */
	unsigned char   synchronous_drive; /*does this drive support synchronous
					   data transfers */
	unsigned char	dum[7];

	unsigned 	nextsec;	/* next log sector num available 
					for disk spare tbl, media list */

	long	unused1[107];
	unsigned 	rv_strt;	/* starting sect # of reserved drive */
	unsigned 	rv_size;	/* length of reserved drive */
	short 		dummy2[3];
	unsigned short	rv_skip;	/* temp, for format '0' only */
	struct	logtype {
		unsigned 	ld_strt;/* starting sector # of log drive */
		unsigned 	ld_size;/* size of log drive */
		char	ld_type;	/* 1=unix file system */
					/* 2=swap area */
					/* 3=customer data file system */
					/* 4=power fail,memory dump area */
		unsigned char	ld_mirror;/* mirror logical disk num */

		unsigned char	mirtype;/* 1=master (has mirror drive) */
					/* 2=slave (mirror of other drive) */
		char dum[1];
		unsigned short	ld_clean;/* size of inode area */
		unsigned short	ld_skip; /* temp, for format '0' only */
	} logdrive[LOGDR];
	long	unused2[60];
};

/* *************************** */
/* logdrive.ld_type definition */
/* *************************** */
#define LD_UNIXFS	1
#define LD_SWAP		2
#define LD_PWF		3
#define LD_USERHOLE	4
#define LD_USERSPARE	5
#define LD_USERSKIP	6
#define LD_USERSPLIT	7

/* *********************************************** */
/* layout of spare sector table entry  in sector 2 */
/* *********************************************** */
#define SPARELSTSIZ	126
struct spare_list {
	unsigned totspare;	/* total sectors available for sparing 
					   in this sector */
	unsigned usedspare;	/* sectors already used for sparing in
						this sector */
	struct sprs{
		unsigned short	cyl;	/* bad c,h,s or -1 indicates the alt */
		unsigned char	head;	/*  sector is aviable for used */
		unsigned char	sector;
		unsigned short	altcyl;	/* alternate c,h,s */
		unsigned char	althead;
		unsigned char	altsector;
	} sparesect[SPARELSTSIZ];

	unsigned short	dummy;
	unsigned char	idfield;
	unsigned char	seqnum;		/* start with 0 */
	int	nextsec;		/* sector num for holding more entry */
};


/* *************************** */
/* skip track list in sector 4 */
/* *************************** */

/* each logical drive has their own array of skip track list with zero
   cylinder, zero head in the last element to indicate the end of the array */

/* example: if none of the logical drives has skip track list, there will
	    be 16 long words with zero in sector 4 */
/* 
	the following example shows
	log 0 has 2 skip tracks, log 2 has 1 skip track,
	log 3 has 3 skip tracks

	c,h c,h 0,0	0,0	c,h 0,0		c,h c,h c,h 0,0
	0,0 		0,0	0,0		0,0
	0,0 		0,0	0,0		0,0
	0,0 		0,0	0,0		0,0
*/

struct badtrck {
	unsigned short cyl;	/* cyl,head num to skip track */
	unsigned short head;
};

/* *****only can hold 254 skip track list */
#define SKTRKSIZ	254			/* max num in single sector */

struct sktrk_list {
	struct badtrck badtrack[SKTRKSIZ];
	unsigned short dummy;
	unsigned char idfield;
	unsigned char seqnum;		/* start with 0 */
	unsigned  nextsec;		/* next log sector num that
						holds more skip track list */
};

/* last 2 long words of this sector is defined as follows:  */
/*	unsigned short	dummy; */
/*	unsigned char	idfield; */
/*	unsigned char	seqnum; */		/* start with 0 */
/*	unsigned	nextsec; */		/* next log sector num that
						holds more skip track list */


/* ***************************** */
/* media defect list in sector 6 */
/* ***************************** */
#define MEDLSTSIZ	50		/* total num can hold in single sect */
struct md_list {
	unsigned nummedlist;		/* num of med defect list in this
						sector */
	struct medf {
		unsigned short	cyl;
		unsigned char	head;
		unsigned char	dummy1;
		struct defpos {
			unsigned short	pos;	/* position of defect is in
						bytes after Index +- 1 byte */
			unsigned short	len;	/* length of defect is in 
						bits +- 1 bit */
		} defpos[4];
	} medflist[MEDLSTSIZ];
	unsigned short dummy2[7];
	unsigned char	idfield;
	unsigned char	seqnum;		/* start with 0 */
	unsigned nextsec;		/* next sector num for more media
						defect list */
};

/* ********************************* */
/* boot information list in sector 8 */
/* ********************************* */
struct boot_list {
	unsigned boot_strt;		/* starting sector # in reserved area */
	unsigned boot_size;
	
	unsigned dummy[254];
};

/* ********************************* */
/* bad sector list in sector 0x10    */
/* ********************************* */
#define ASCIBAD		0x42414400	/* id field 'BAD'0 */

#define BADLSTSIZ	252
struct badlist {
	char id[4];
	unsigned numofentries;		/* total num of bad sectors recorded 
					in this sector */
	struct sectid {
		unsigned short cyl;
		unsigned char	head;
		unsigned char	sector;
	} badsctrinfo[BADLSTSIZ];		/* this is a random list */
	unsigned short	dummy;
	unsigned char	idfield;
	unsigned char	seqnum;		/* start with 0 */
	unsigned 	nextsec;
};

/* value for idfield for all system sectors */
#define SPARSECID	0x32
#define SKIPSECID	0x34
#define MEDSECID	0x36
#define BADSECID	0x61


