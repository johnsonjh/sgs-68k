#ident	"@(#)inet_netof.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
 *	Copyright 1986 Lachman Associates Inc. and The Instruction Set Ltd.
 */
#ident	"@(#)inet_netof.c	2.2 LAI KNFS for ARETE source"
#ident	"@(#)inet_netof.c	2.7 System V NFS source"
/*	inet_netof.c	4.3	82/11/14	*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/fs/nfs/inet_insup.h>

#ifdef EXOS
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <CMC/socket.h>
#include <CMC/in.h>
#endif
/*#define DEBUG 	1 */
/*
 * Return the network number from an internet
 * address; handles class a/b/c network #'s.
 */
inet_netof(in)
	struct in_addr in;
{
#ifdef EXOS
	ulong	i;
	if ( (in.s_addr & IN_CLASSA) == 0) {

	  /* return (ntohl(in.s_addr&IN_CLASSA_NET) >> IN_CLASSA_NSHIFT); */

	  /*
	   * The Vax compiler on System file produces garbage when the above
	   * expression is returned, so we do it the long way.
	   */

	  i = in.s_addr&IN_CLASSA_NET;
	  i = ntohl(i);
	  i = i >> IN_CLASSA_NSHIFT;
#ifdef DEBUG
fprintf(stderr,"inet_netof: IN_CLASSA i=%8x\n", i);
#endif
	  return(i);
	}
	if ( (in.s_addr & IN_CLASSB) == 0) {

	  /* return (ntohl(in.s_addr&IN_CLASSB_NET) >> IN_CLASSB_NSHIFT); */

	  i = in.s_addr&IN_CLASSB_NET;
	  i = ntohl(i);
	  i = i >> IN_CLASSB_NSHIFT;
#ifdef DEBUG
fprintf(stderr,"inet_netof: IN_CLASSB i=%8x\n", i);
#endif

	  return(i);
	}

	/* return (ntohl(in.s_addr&IN_CLASSC_NET) >> IN_CLASSC_NSHIFT); */

	i = in.s_addr&IN_CLASSC_NET;
	i = ntohl(i);
	i = i >> IN_CLASSC_NSHIFT;
#ifdef DEBUG
fprintf(stderr,"inet_netof: IN_CLASSC i=%8x\n", i);
#endif
	return(i);
#else
	register ulong i = ntohl(in.s_addr);

	if (IN_CLASSA(i))
{
#ifdef DEBUG
	printf("inet_netof: in.s_addr=%x IN_CLASSA_NET=%d IN_CLASSA_NSHIFT=%d\n",i, IN_CLASSA_NET, IN_CLASSA_NSHIFT);
#endif	
		return (((i)&IN_CLASSA_NET) >> IN_CLASSA_NSHIFT);
}
	else if (IN_CLASSB(i))
{
#ifdef DEBUG
	printf("inet_netof: in.s_addr=%x IN_CLASSA_NET=%d IN_CLASSA_NSHIFT=%d\n",i, IN_CLASSA_NET, IN_CLASSA_NSHIFT);
#endif	
		return (((i)&IN_CLASSB_NET) >> IN_CLASSB_NSHIFT);
}
	else
{
#ifdef DEBUG
	printf("inet_netof: in.s_addr=%x IN_CLASSA_NET=%d IN_CLASSA_NSHIFT=%d\n",i, IN_CLASSA_NET, IN_CLASSA_NSHIFT);
#endif	
		return (((i)&IN_CLASSC_NET) >> IN_CLASSC_NSHIFT);
}
#endif
}
