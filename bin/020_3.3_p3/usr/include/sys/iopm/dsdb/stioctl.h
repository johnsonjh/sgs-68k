/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) stioctl.h: version 24.1 created on 10/28/91 at 17:54:30	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)stioctl.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* smtioctl.h */

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
#define T_LOW_DENSITY	0x26		/* use low density mode		*/
#define T_HIGH_SPEED	0x27		/* use high speed mode		*/
#define T_LOW_SPEED	0x28		/* use low speed mode		*/
#define T_GET_TYPE	0x29		/* returns drive type: TY_CYPHER*/
#define T_GET_CACHE	0x2a		/* returns cache bit: 0=no,1=yes*/
#define T_GET_DENSITY	0x2b		/* returns density:0=default 	*/
					/* 1=800,2=1600,3=6250,6=1600   */
#define T_GET_SPEED	0x2c		/* returns speed: 0=low,1=hi	*/

#define T_6250_DENSITY	T_HIGH_DENSITY
#define T_1600_DENSITY  T_LOW_DENSITY
#define T_800_DENSITY	0x2d
#define T_3200_DENSITY	0x2e
#define T_DEFT_DENSITY	0x2f		/* device's default or only density */
