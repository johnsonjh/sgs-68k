/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ioctl.h: version 2.1 created on 5/22/89 at 19:07:57	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ioctl.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef SYS_IOCTL_H
#define SYS_IOCTL_H


/* Special Peripheral Device ioctl */

#define PERIOC  ('P'<<8)
#define PERGET  (PERIOC|0)
#define PERSET  (PERIOC|1)

/* the following two ioctl command macros are for used
 * by sadc (called by sar) to get system activity info
 * from the iop's
 */

#define GC_GETSAR	0		/* get sar info from gc's */
#define DK_GETSAR	1		/* get sar info from hsdt's */


/* the following three ioctl command macros are for used
 * by sadc (called by sar) to get system activity info
 * from the cpu's
 */
#define GET_SYSINFO	2
#define GET_SYSERR	4

/*
 * this is the data structure used for an ioctl on /dev/sar
 * with any of the previous three commands
 */

struct sario {
	int	cpu;		/* cpu id, 0 - 3 */
	char	*addr;		/* pointer to data area to fill */
};


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
#define QIC11       0xd			/* change to QIC11 format (archive)*/
#define QIC24       0xe			/* change to QIC24 format (archive)*/
#define REVSFM      0x12		/* return tape status from last tape
#define LASTSTAT    0x14		/* return tape status from last tape
					   operation */
#define DRIVETYPE   0x15		/* return drive type TRACK9/ARCHIVE */

/*
 * The following command are  used for asking disk drivers
 * configuration questions, and setting configuration.
 */

#define MIRROR_TYPE		0x01
#define SCSI_TYPE		0x02
#define SMD_TYPE		0x03
#define STRIPE_TYPE		0x04

#define SET_VIRT_DISK		0x30
#define CLEAR_VIRT_DISK		0x31
#define GET_VIRT_DISK		0x32
#define SET_MIRROR_DISK		0x33
#define CLEAR_MIRROR_DISK	0x34
#define GET_MIRROR_DISK		0x35

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

#define READ_RESV		0x50
#define WRITE_RESV		0x51

#define	DKWPON			0x52
#define DKWPOFF			0x53

/*	Logical Dev Types			*/

#define		LDT_UNIX	1
#define		LDT_SWAP	2


struct reserve {
	int	blkno;
	int	buf;
	int	cnt;
	};

#define		LDT_CDATA	3
#define		LDT_PFAIL	4

#endif /* SYS_IOCTL_H */
