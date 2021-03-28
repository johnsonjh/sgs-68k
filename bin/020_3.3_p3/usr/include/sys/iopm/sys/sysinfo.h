/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sysinfo.h: version 24.1 created on 10/28/91 at 18:00:20	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sysinfo.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef	IOPM_SYSINFO_H
#define	IOPM_SYSINFO_H

/*
 *	System Information.
 */

struct i_sysinfo {
	long	rcvint;
	long	xmtint;
	long	mdmint;
	long	rawch;
	long	canch;
	long	outch;
};

#ifndef	IOPM_OS				/* trick drivers */
extern  struct i_sysinfo  *get_sysinfo();
#define	sysinfo	(*get_sysinfo())
#endif

#endif	/* IOPM_SYSINFO_H */
