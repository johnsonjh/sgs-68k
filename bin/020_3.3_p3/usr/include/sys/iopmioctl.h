#ifndef SYS_IOPMIOCTL_H
#define SYS_IOPMIOCTL_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iopmioctl.h: version 24.1 created on 10/28/91 at 18:34:19	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iopmioctl.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#define NUM_CONFIG   16		/* max drivers in one file */
				/* max -s and -b strings for one ldiopm cmd */

#define SPM_CMD      0x80000000	/* the spm sent the cmd to the IOPM */
#define CMD_MASK     0x0fffffff

#define LOAD_WHERE   0x00000001	/* ask the IOPM where to load a module */
#define START_MOD    0x00000008	/* start the module executing */
#define BUF_DEVSW    0x00000010	/* enter info in IOPM bdevsw */
#define STR_DEVSW    0x00000013	/* enter str info in IOPM cdevsw */
#define MOD_SW       0x00000014	/* enter mod into fmodsw */
#define SET_QUEUES   0x00000023	/* set the number of queues on an IOPM */
#define SET_BP       0x00000033	/* set the number of bp (w/o buf) on an IOPM */
#define SET_BUF      0x00000034	/* set the number of bp (w buf) on an IOPM */
#define	SET_NUMTCB   0x00000036	/* set the number of tasks */
#define SET_NMSG     0x00000049

#define RESETIOPM    0x00000050
#define UNRESETIOPM  0x00000051

#define VTOP         0x00000060

struct iopmioctlw {		/* LOAD_WHERE ioctl */
	uint  taddr;		/* text address (virtual in, physical out) */
	uint  tlength;		/* text lenght */
	uint  daddr;		/* data address (virtual in, physical out) */
	uint  dlength;		/* data lenght */
};

struct iopmioctld {		/* BUF_DEVSW & STR_DEVSW ioctl */
	char    prefix[16];	/* name of module/driver */
	dev_t   kmin;		/* starting kernel minor number */
	ushort  numdev;		/* number of minor numbers for this group */
	dev_t   clonemin;	/* clone minor number */
	dev_t   imin;		/* IOPM starting minor number */
	int     port;		/* hardware port for this group */
};

struct iopmioctlb {		/* SET_PBUF ioctl */
	uint  num;
	uint  size;
};

#endif /* SYS_IOPMIOCTL_H */
