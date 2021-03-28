#ifdef NO_SCCS_ID
#ident "@(#)lastlog.h  (TWG)      1.2     89/06/14 "
#endif  /*NO_SCCS_ID*/

/*
 *  @(#) Copyright (c) 1985  The Wollongong Group, Inc.  All Rights Reserved
 */

struct lastlog {
	time_t	ll_time;
	char	ll_line[8];
	char	ll_host[16];		/* same as in utmp */
};
