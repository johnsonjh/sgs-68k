/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"head:unistd.h	1.4"			*/

#ident	"%Z%uts/head:%M%	%I%"

#define F_ULOCK	0	/* Unlock a previously locked region */
#define F_LOCK	1	/* Lock a region for exclusive use */
#define F_TLOCK	2	/* Test and lock a region for exclusive use */
#define F_TEST	3	/* Test a region for other processes locks */

/* unistd.h
 * local version of the /usr/group standard include file
 */

/* symbolic constants for the access function */

#define	R_OK	4		/* test for read permission */
#define	W_OK	2		/* test for write permission */
#define	X_OK	1		/* test for execute permission */
#define	F_OK	0		/* test for existence of file */


/* symbolic constants for the lseek function */

#define SEEK_SET 0		/* set file pointer to offset */
#define SEEK_CUR 1		/* set file pointer to current plus offset */
#define SEEK_END 2		/* set file pointer to EOF plus offset */

/* path names */

#define GF_PATH	 "/etc/group"	/* pathname of the group file */
#define PF_PATH  "/etc/passwd"	/* pathname of the password file */
#define IN_PATH	 "/etc/include" /* pathname for <...> directory */



