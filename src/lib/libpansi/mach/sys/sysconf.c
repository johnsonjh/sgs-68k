#ident	"@(#)sysconf.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."



#include "limits.h"
#include "unistd.h"
#include <sys/errno.h>

extern errno;

sysconf(name)
int name;
{

	unsigned int retval;

	switch(name){

		case _SC_ARG_MAX:
			retval = _sysconf(_SC_ARG_MAX);
			return(retval);

		case _SC_CHILD_MAX:
			return(CHILD_MAX);

		case _SC_CLK_TCK:
			return(CLK_TCK);

		case _SC_NGROUPS_MAX:
			retval = _sysconf(_SC_NGROUPS_MAX);
			return(retval);

		case _SC_OPEN_MAX:
			retval = _sysconf(_SC_OPEN_MAX);
			return(retval);

		case _SC_JOB_CONTROL:
			return(_POSIX_JOB_CONTROL);

		case _SC_SAVED_IDS:
			return(_POSIX_SAVED_IDS);

		case _SC_EXIT_SIGHUP:
			return(_POSIX_EXIT_SIGHUP);

		case _SC_KILL_PID_NEG1:
			return(_POSIX_KILL_PID_NEG1);

		case _SC_KILL_SAVED:
			return(_POSIX_KILL_SAVED);

		case _SC_PGID_CLEAR:
			return(_POSIX_PGID_CLEAR);

		case _SC_UID_MAX:
			return(UID_MAX);

		case _SC_VERSION:
			return(_POSIX_VERSION);

		default:
			errno = EINVAL;
			return(-1);
	}

}
