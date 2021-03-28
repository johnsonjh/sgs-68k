/*	ftw.h	1.1	*/


#ifndef FTW_H
#ident	"@(#)ftw.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define FTW_H

/*
 *	Codes for the third argument to the user-supplied function
 *	which is passed as the second argument to ftw
 */

#define	FTW_F	0	/* file */
#define	FTW_D	1	/* directory */
#define	FTW_DNR	2	/* directory without read permission */
#define	FTW_NS	3	/* unknown type, stat failed */

#endif /* FTW_H */
