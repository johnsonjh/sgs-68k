#ifndef SYS_IOCTL_H
#define SYS_IOCTL_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ioctl.h: version 24.1 created on 10/28/91 at 18:33:57	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ioctl.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/* Special Peripheral Device ioctl */

#define PERIOC  ('P'<<8)
#define PERGET  (PERIOC|0)
#define PERSET  (PERIOC|1)

/* the following three ioctl command macros are for used
 * by sadc (called by sar) to get system activity info
 * from the cpu's
 */
#define GET_SYSINFO	2
#define GET_SYSERR	4

/*
 * The following ioctl commands are for use with archive and
 * nine track devices.  They are all used as a value pointed
 * to in arg, with a PERSET command.
 *
 * ioctl(fd, PERSET, arg)
 * int fd, cmd, *arg;
 */

#define REWIND 	    3			/* rewind the tape */
#define WEOF 	    4			/* write end of file (9 track) */
#define REOF 	    5			/* read end of file (9 track) */
#define TAPESTAT    6			/* get tape status */
#define TENSION     7			/* archive only */
#define ERASE       8			/* erase tape from current location*/ 
#define RECBACK     0xb			/* back one record (9track)*/
#define REVSFM      0x12		/* return tape status from last tape */
#define LASTSTAT    0x14		/* return tape status from last tape
					   operation */
#define DRIVETYPE   0x15		/* return drive type TRACK9/ARCHIVE */

/* Tape control commands 						    *
 * These ioctl commands were originally found in sys/mt.h this has been     *
 * removed with the purge of EDT system files since this is now unsupported *
 * A lot of these commands had been moved into the iopm tape drivers local  *
 * header files but these were not public so they have been duplicated here */

#define ARCHIVE	2	/* Archive tape drive on line */
#define TRACK9	4	/* 9 track tape drive on line */

/* ioctl command code -- capatible to A1000's EDT tape driver */

#define TAPE_MAP	0x02    /* for ramtape used only */
/* FIX temp define since ioctl.h has bugs in pm */
#define LASTSTAT        0x14
#define TAPE_TYPE	0x15	/* return drive type */

/* The status return to user(the code is compatible with EDT) */

#define TP_NOTAPE	0x30
#define TP_NRDY		0x31
#define TP_EOM		0x33

#define T_HAS_CACHE	0x23		/* tape drive has a cache	*/
#define T_NO_CACHE	0x24		/* tape drive has no cache	*/
#define T_HIGH_DENSITY	0x25		/* use high density mode	*/
#define T_MEDIUM_DENSITY 0x2e		/* use medium density mode 	*/
#define T_LOW_DENSITY	0x26		/* use low density mode		*/
#define T_VLOW_DENSITY	0x2d		/* use very low density mode	*/
#define T_HIGH_SPEED	0x27		/* use high speed mode		*/
#define T_LOW_SPEED	0x28		/* use low speed mode		*/
#define T_GET_TYPE	0x29		/* returns drive type: TY_CYPHER*/
#define T_GET_CACHE	0x2a		/* returns cache bit: 0=no,1=yes*/
#define T_GET_DENSITY	0x2b		/* returns density:0=default 	*/
					/* 1=800,2=1600,3=6250,6=1600   */
#define T_GET_SPEED	0x2c		/* returns speed: 0=low,1=hi	*/

#define T_6250_DENSITY	T_HIGH_DENSITY
#define T_1600_DENSITY  T_LOW_DENSITY
#define T_800_DENSITY	T_VLOW_DENSITY
#define T_3200_DENSITY	T_MEDIUM_DENSITY
#define T_DEFT_DENSITY	0x2f		/* device's default or only density */

/*
 * The following defines are commands for use in asking the kernel for the
 * address of certain symbols via an ioctl on /dev/mem or /dev/kmem.
 */

#define KV_ADDR_PROC	0
#define KV_ADDR_SWPLO	1
#define KV_ADDR_V	2

/*
 * The following command are  used for asking disk drivers
 * configuration questions, and setting configuration.
 */

#define SET_VIRT_DISK		0x30
#define CLEAR_VIRT_DISK		0x31
#define GET_VIRT_DISK		0x32

#define FIRST_MR_IOCTL		CREATE_MIRROR_DISK
#define CREATE_MIRROR_DISK	0x33	/* create a mirror		*/
#define DELETE_MIRROR_DISK	0x34	/* delete a mirror		*/
#define GET_MIRROR_DISK		0x35	/* get the mirror structure	*/
#define LOCK_MIRROR_DISK	0x36	/* lock/unlock a mirror		*/
#define RE_MIRROR_DISK		0x37	/* copy a mirror		*/
#define INIT_MIRROR_DISK	0x38	/* initialize a mirror struct	*/
#define WAIT_MIRROR_ERROR	0x39	/* wait for an error to happen	*/
#define CAUSE_MIRROR_ERROR	0x3a	/* simulate an error		*/
#define READ_MIRROR_DISK	0x3b	/* read from a given side	*/
#define LAST_MR_IOCTL		READ_MIRROR_DISK

#define GET_DISK_SIZE		0x40
#define GET_CYLINDER_SIZE	0x41
#define GET_INTERLACE		0x42
#define GET_TRACK_SIZE		0x43
#define GET_NUM_TRACKS		0x44
#define GET_NUM_CYLINDERS	0x45
#define GET_TOTAL_TRACKS	0x46
#define GET_TOTAL_CYLINDERS	0x47
#define GET_DEV_TYPE		0x48
#define GET_LOG_TYPE		0x49
#define GET_OPEN_TYPES		0x4a	/* return an opentyp_t */

#define READ_RESV		0x50
#define WRITE_RESV		0x51

#define SDK_READ_VTOC		0x60

/* structure for doing reserve area ioctls */
struct reserve {	
	int	blkno;
	int	buf;
	int	cnt;
};

/*	Legal Dev Types				*/

#define MIRROR_TYPE		0x01
#define SCSI_TYPE		0x02
#define SMD_TYPE		0x03
#define STRIPE_TYPE		0x04

/*	Logical Dev Types			*/

#define		LDT_UNIX	1
#define		LDT_SWAP	2
#define		LDT_CDATA	3
#define		LDT_PFAIL	4

/* definitions for optical driver */

#define W_GET_DISK_SIZE	0x70	/* worm disk size */
#define W_GET_BLK_SIZE	0x71	/* worm block size */
#define W_BYTESWAPON	0x72	/* swap bytes */
#define W_BYTESWAPOFF	0x73 /* no swap */
#define W_EJECTONCLOSE	0x74 /* eject medium on close */
#define W_NOEJECTONCLOSE	0x75 /* don't eject medium on close */
#define W_SONY_FORMAT	0x76	/* format a sony medium */

#endif /* SYS_IOCTL_H */
