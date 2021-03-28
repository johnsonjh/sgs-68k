#ident	"@(#)exec_state.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

/* 
   get_exec_status( pswd ) - 
		Examines password file entry to determine whether that entry
		wants their binaries executed as System V or POSIX, or as
		POSIX without job control, and returns a 0 if the uid is SVID,
		1 if POSIX, 2 if jobcontrol not allowed (0 always implies 2)

   set_exec_status( pswd )
		Examines the password file entry to determine how the entry 
		wants it's binaries run, and then changes the current process
		to be of that type

   current_exec_status()
		Determines whether currently running process is 
		SVID (returns 0), POSIX or undetermined (returns 1), or
		POSIX with no job control (returns 2).  Failure (returns -1).
*/

#include <sys/sysarix.h>
#include <sys/types.h>
#include <sys/proc.h>
#include <pwd.h>

struct	passwd	*getpwuid ();

int
current_exec_status ()
{
	unsigned int	exec_flag;

	if ( sysarix( ARIXPOSIX, ARIXGETPOSFLAGS, 0, &exec_flag ) < 0 )
		return (-1);

	/* SVID? */
	if ( !( exec_flag & F_POSIX_BINARY ) )
		return( 0 );


	/* POSIX but no job control? */
	if ( !( exec_flag & JOB_CONTROL_PROC ) &&
	     !( exec_flag & JOB_SESSION_PROC ) )
			return ( 2 );

	/* POSIX with some/all job control */
	return ( 1 );
}

/* 
 * if gecos field is SVID,			return 0
 * if field is empty or no pattern match	return 1
 * if NOJOBS,					return 2
*/
int
get_exec_status( pswd )
	struct passwd *pswd;
{
	if ( ! *( pswd->pw_gecos ) )
		return (1);

	/*
	 * Always check for SVID first!
	 */
	if ( match_state ( pswd->pw_gecos, SVID_FIELD ) )
		return (0);

	if ( match_state ( pswd->pw_gecos, NOJOBS_FIELD ) )
		return (2);

	return (1);
}

int
set_exec_status( pswd )
	struct passwd *pswd;
{
	register unsigned int status;
	unsigned int exec_flag;

	status = get_exec_status( pswd );

	if ( sysarix( ARIXPOSIX, ARIXGETPOSFLAGS, 0, &exec_flag) < 0 )
		return ( -1 );

	switch( status )
	{
	case 0:	/* SVID */
		exec_flag = 
		(exec_flag & ~(JOB_CONTROL_PROC | JOB_SESSION_PROC | 
			F_POSIX_BINARY)) | SVID_COMPAT_PROC;
		break;

	case 1:	/* POSIX & Job Control */
		exec_flag |= JOB_CONTROL_PROC | JOB_SESSION_PROC | 
			F_POSIX_BINARY;
		break;

	case 2:	/* POSIX & !job Control */
		exec_flag = 
		(exec_flag & ~(JOB_CONTROL_PROC | JOB_SESSION_PROC)) 
		| F_POSIX_BINARY;
		break;
	}

	if ( sysarix (ARIXPOSIX, ARIXSETPOSFLAGS, 0, &exec_flag) < 0)
		return (-1);
	else
		return (status);
}

int
match_state( src, pat )
	register char	*src, *pat;
{
	register int	s, p;

	/* Insure args legal */
	if ( !src || !*src || !pat || !*pat )
		return (0);

	s = strlen (src);
	p = strlen (pat);

	if (s < p)
		return (0);

	while (*src) {
		if ( *src == *pat )
			if ( strncmp( src, pat, p ) == 0)
				return (1);
		src++;
		s--;
		if (s < p)
			return (0);
	}
	return (0);
}
