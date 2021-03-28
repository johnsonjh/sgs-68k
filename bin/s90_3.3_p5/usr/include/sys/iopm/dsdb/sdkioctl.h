/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sdkioctl.h: version 24.1 created on 10/28/91 at 17:51:03	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sdkioctl.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* sdkioctl.h -- definition for ioctl function */

#define SDK_FORMAT		0x01
#define	SDK_GETSLICE		0x02
#define	SDK_SETSLICE		0x03
#define SDK_CONFIG		0x04
#define SDK_TRNSLT		0x05
#define SDK_NEXT_CYLN_BLKNO	0x06
#define	GET_ACCOUNTING		0x80
#define GET_QUEUE		0x81


typedef struct fmt_pmtrs {
#ifndef OLD_DISK
	uint	date;
#endif /* OLD_DISK */
 	uint	block_size;
	ushort	alt_secs;
	ushort	alt_trk;
	ushort	interleave;
	ushort	trk_skew;
	ushort	clyn_skew;
	unchar	flags;
}SDKFMTP; 
#define FMTPSIZE sizeof(SDKFMTP)

#define	MAXSLICE	16

#ifdef OLD_DISK
typedef struct s_size {
	uint	open_flag;	/* the logic drive status 	*/
	uint	offset;		/* starting block number	*/
	uint	noblk;		/* size of partition		*/
} SLSIZE;

#else

typedef struct s_size {
	uint	offset;		/* starting block number	*/
	uint	noblk;		/* size of partition		*/
	unchar	type;		/* the logic drive status 	*/
	unchar  spt;
} SLSIZE;

#endif /* OLD_DISK */
