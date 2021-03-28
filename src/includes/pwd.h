/*	pwd.h	1.1	*/
/*	3.0 SID #	1.2	*/


#ifndef	PWD_H
#ident	"@(#)pwd.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define	PWD_H

#include <sys/types.h>

#ifdef	_POSIX_SOURCE

/* uid_t & gid_t added for POSIX 1003.1 compliance */

struct passwd {
	char	*pw_name;
	char	*pw_passwd;
	ushort	:16;			/* Used to align passwd struct */
	uid_t	pw_uid;
	ushort	:16;			/* Used to align passwd struct */
	gid_t	pw_gid;
	char	*pw_age;
	char	*pw_comment;
	char	*pw_gecos;
	char	*pw_dir;
	char	*pw_shell;
};

#else	/* _POSIX_SOURCE */

struct passwd {
	char	*pw_name;
	char	*pw_passwd;
	int	pw_uid;
	int	pw_gid;
	char	*pw_age;
	char	*pw_comment;
	char	*pw_gecos;
	char	*pw_dir;
	char	*pw_shell;
};

#endif /* _POSIX_SOURCE */

struct comment {
	char	*c_dept;
	char	*c_name;
	char	*c_acct;
	char	*c_bin;
};

/* Determines whether a user wishes processes to be SVID or POSIX while
   executing.  Located in the pw_gecos field. */
#define	SVID_FIELD	"SVID"
#define NOJOBS_FIELD	"NOJOBS"

/* SVID doc requires passwd functions to be declared here */
extern	struct	passwd	*getpwent ();
extern	struct	passwd	*getpwnam ();
extern	struct	passwd	*getpwuid ();
extern	struct	passwd	*fgetpwent ();

#endif	/* PWD_H */
