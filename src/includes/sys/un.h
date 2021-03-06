#ifdef  NO_SCCS_ID
#ident "@(#)un.h  (TWG)      1.3     89/06/14 "
#endif  /*NO_SCCS_ID*/

/*
 * @(#) Copyright 1988.  The Wollongong Group, Inc.  All Rights Reserved.
 */
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)un.h	7.1 (Berkeley) 6/4/86
 */

/*
 * Definitions for UNIX IPC domain.
 */
struct	sockaddr_un {
	short	sun_family;		/* AF_UNIX */
	char	sun_path[108];		/* path name (gag) */
};
#define AFUADDRLEN sizeof(struct sockaddr_un)
#define SUNPATHLEN 108
