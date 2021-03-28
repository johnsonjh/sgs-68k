#ifndef SYS_UTSNAME_H
#define SYS_UTSNAME_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) utsname.h: version 2.1 created on 5/22/89 at 19:12:39	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)utsname.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/utsname.h	10.2"	*/

#ident	"@(#)uts/head/sys:utsname.h	2.1"


struct utsname {
	char	sysname[9];
	char	nodename[9];
	char	release[9];
	char	version[9];
	char	machine[9];
};
extern struct utsname utsname;

#endif /* SYS_UTSNAME_H */
