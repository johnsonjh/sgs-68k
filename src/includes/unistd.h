/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"head:unistd.h	1.4"			*/


#ifndef	UNISTD_H
#ident	"@(#)unistd.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define	UNISTD_H

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



#ifdef	_POSIX_SOURCE

/* Added for POSIX 1003.1 compliance */

#define STDIN_FILENO	0
#define STDOUT_FILENO	1
#define STDERR_FILENO	2
#define NULL 0

#define _POSIX_JOB_CONTROL 	1
#define _POSIX_SAVED_IDS 	1
#define _POSIX_VERSION		198808L
#define _POSIX_EXIT_SIGHUP	1
#define _POSIX_KILL_PID_NEG1	1
#define _POSIX_KILL_SAVED	1
#define _POSIX_PGID_CLEAR	1
#define _POSIX_SAVED_IDS	1
#define _POSIX_CHOWN_RESTRICTED 1	/* The use of chown is restricted to 
					   a process with the apporpriate
					   privileges, and to changing the 
					   group ID of a file only to the
					   effective group ID of the process or
					   to one of its supplementary group IDs
					*/
#define _POSIX_CHOWN_SUP_GRP    1
#define _POSIX_DIR_DOTS		1
#define _POSIX_GROUP_PARENT	1
#define _POSIX_NO_TRUNC  	1	/* Pathname components longer than
					   NAME_MAX generate an error
					*/
#define _POSIX_UTIME_OWNER	1
#define _POSIX_V_DISABLE	0x1f
#define _POSIX_VDISABLE  	0x1f	/* Terminal special characters defined
					   in <termios.h> can be disabled
					   using this character value, if it
					   is defined.
					*/


/* Used by the sysconf function */

#define _SC_ARG_MAX		1
#define _SC_CHILD_MAX		2
#define _SC_CLK_TCK		3
#define _SC_NGROUPS_MAX		4
#define _SC_OPEN_MAX		5
#define _SC_PASS_MAX		6
#define _SC_PID_MAX		7
#define _SC_UID_MAX		8
#define _SC_EXIT_SIGHUP		9
#define _SC_KILL_PID_NEG1	10
#define _SC_KILL_SAVED		11
#define _SC_PGID_CLEAR		12
#define _SC_JOB_CONTROL		13
#define _SC_SAVED_IDS		14
#define _SC_VERSION		15



/* Used by the pathconf function */

#define _PC_FCHR_MAX		1
#define _PC_LINK_MAX		2
#define _PC_MAX_CANNON		3
#define _PC_MAX_CANON		3	/*POSIX as opposed to FIPS */
#define _PC_MAX_INPUT		4
#define _PC_NAME_MAX		5
#define _PC_PATH_MAX		6
#define _PC_PIPE_BUF		7
#define _PC_CHOWN_RESTRICTED	8
#define _PC_CHOWN_SUP_GRP	9
#define _PC_DIR_DOTS		10
#define _PC_GROUP_PARENT	11
#define _PC_LINK_DIR		12
#define _PC_NO_TRUNC		13
#define _PC_UTIME_OWNER		14
#define _PC_V_DISABLE		15
#define _PC_VDISABLE		15	/*This is for POSIX, the above is FIPS*/

/* define for Posix 1003.1a */
typedef short	ssize_t;

#endif	/* _POSIX_SOURCE */

#endif	/* UNISTD_H */
