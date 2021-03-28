#ifdef NO_SCCS_ID
#ident "@(#)inet.h  (TWG)      1.2     89/06/14 "
#endif /*IN_SCCS_ID*/

/*
 * @(#) Copyright 1986.  The Wollongong Group, Inc.  All Rights Reserved.
 */


/*
 * External definitions for
 * functions in inet(3N)
 */
/*struct	in_addr inet_addr(); */
unsigned long inet_addr();
char	*inet_ntoa();
struct	in_addr inet_makeaddr();
unsigned long inet_network();
