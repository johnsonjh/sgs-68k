#ifndef SYS_PHINF_H
#define SYS_PHINF_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) phinf.h: version 2.1 created on 5/22/89 at 19:09:28	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)phinf.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#define MAXDRIVE	4
#define LOGDRIVE	0x10	/* # of logical drive per physical drive */
#define SECSIZE		0x400	/* # of bytes in a sector */

#define PHYS	2		/* disk r/w command with c,h,s */

/* bit define in dkflag */
#define RAW	0x1
#define OWN	0x2

/* structure of spare table */
struct cylhdsec
{	unsigned short	cyl;	
	unsigned char head;
	unsigned char sect;
	unsigned short	mcyl;	
	unsigned char mhead;
	unsigned char msect;
};

struct logsize
{	unsigned long logsoff;
	unsigned long nblocks;
};

/* struct of logical drive table */
struct logdr
{
	struct logsize dksize[2];
};

struct	logtype {
	uint	ld_strt;/* starting sector # of log drive */
	uint	ld_size;/* size of log drive */
	char	ld_type;/* 1=unix file system */
			/* 2=swap area */
			/* 3=customer data file system */
			/* 4=power fail,memory dump area */
	unsigned char	ld_mirror; /* mirror logical disk num */

	unsigned char	mirtype;	/* 1=master (has mirror drive) */
				/* 2=slave (mirror of other drive) */
	char dum[5];
};

struct psd_info {
	unsigned	char	flag;		/* open/close flag */
	unsigned	char	rec_interlace;
	unsigned	short	cyldisk;
	unsigned	char	headcyl;
	unsigned	char	sechead;
	unsigned	short	seccyl;
	struct		logtype	part[LOGDRIVE];
	int			opencnt;
	};
	
/* ****************** */
/* layout of sector 0 */
/* ****************** */

struct sector0
{
	char	id[8];		/* INIT name */
	char	date[8];	/* format date */
	ushort	dummy[4];
	ushort	cyldisk;	/* # of cyls on the disk */
	unsigned char	headcyl;	/* # of heads per cyl */
	unsigned char	sechead;	/* # of sectors per head */
	ushort	seccyl;		/* # of sectors per cyl */
	ushort	bytesec;	/* # of bytes per sector */

	unsigned char	gap[32];	/* timing data */

	unsigned char 	pd_ldmaxnum;	/* max num of log drive */
	unsigned char	pd_ldnum;	/* current num of log drive */
	unsigned char	rec_interlace;	/* recommended interlace used by mkfs */
	unsigned char	dum[13];

	uint	nextsec;	/* next log sector num available 
					for disk spare tbl, media list */

	long	unused1[107];
	uint	rv_strt;	/* starting sect # of resversed drive */
	uint	rv_size;	/* length of resvered drive */
	uint	dummy2[2];
	struct	logtype logdrive[LOGDRIVE];
	long	unused2[60];
};

struct cyhd {
	short	cyl;
	char	head;
	char	sect;
};

struct rawq {
	int	blkno;
	struct	cyhd cylhs[4];
	short	pageoff[4];
	int	bcnt;
};

/* byte offset into sector 2 for spare information */
#define spr_numoff	4
#define spr_altoff	6
#define spr_addoff	0x20		/* physical sect num */

struct rioptrs{
	char * own_ptr;
	struct rawq *rtab_ptr;
	struct rq *disk_ptr;
	int dkflag;
	int tape_cnt;
	int t_remain;
	int disk_cnt;
	int d_totleft;
	int d_remain;
	short tmmu_slot;
	caddr_t t_mu;
	short dmmu_slot;
	caddr_t d_mu;
	short roffset1;
};

struct map_ptr{
	short index;
	short offset;
	int length;
};

#endif /* SYS_PHINF_H */
