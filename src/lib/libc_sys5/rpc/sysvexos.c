#ident	"@(#)sysvexos.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
 *	Copyright 1986 Lachman Associates Inc. and The Instruction Set Ltd.
 */
#ident	"@(#)sysvexos.c	2.2 LAI KNFS for ARETE source"
#ident	"@(#)sysvexos.c	2.8 System V NFS source"
/* 
 * The routines found in this file have been put together to
 * fudge some of the 4.2BSD routine necessary to make user level
 * rpc function on 5.2
 */
#ifndef S3000

#include <sys/time.h>
#include <sys/types.h>
#include <ctype.h>
#ifdef EXOS
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include <sys/soioctl.h>
#include <netdb.h>
#ifdef DEBUG
#include <stdio.h>
extern int errno;
#endif

/* shutdown */
shutdown(fd, arg)
int fd, arg; 
{
	ioctl(fd,SIOCDONE, &arg);
}



/*
 * the following set of f_ routines are used to map 4.2BSD socket
 * calls into the Excelan suppied 4.1BSD socket calls. the mapping
 * nowhere near complete. enough functionality has been inbyued in these
 * routine to make user level rpc function with a modecom of transparency
 */

/*
 * f_accept maps 4.2 accept => 4.1 accept.
 *
 * caveats:
 *      i) addrlen is not used by 4.1 accept
 *     ii) we assume that the sock is a SOCK_STREAM type socket
 *    iii) the way we return a new socket is a real kludge but neccesary.
 *		unfortunately it is even more complicated by the fact
 *		that the rpc utility that is doing an accept on the socket
 *		has registered the socket as part of xprt registry. 
 */
int 
f_accept(s, addr, addrlen)
int s;
struct sockaddr *addr;
int *addrlen;
{
	int sock;
	struct sockaddr_in holdaddr;

	if (accept(s, addr) == -1)
		return(-1);

	if (socketaddr(s, &holdaddr) == -1)
		return(-1);
	holdaddr.sin_addr.s_addr = 0;
	if((sock = socket(SOCK_STREAM, (struct sockproto *)0, &holdaddr,
		SO_ACCEPTCONN)) == -1)
		return(-1);

	xprt_reregister(s, sock);
	return(s);
}

/*
 * f_connect maps 4.2 connect => 4.1 connect
 *
 * caveats:
 *	i) namelen is not used by 4.1 connect
 */
int 
f_connect(s, name, namelen)
int s;
struct sockaddr *name;
int namelen;
{
	return(connect(s, name));
}

/*
 * f_recvfrom maps 4.2 recvfrom => 4.1 receive
 *
 * caveats:
 *	i) fromlen is not used in 4.1 receive
 *     ii) flags are not used by 4.1 receive and therefore MSG_PEEK
 *		and MSG_OOB functionality is lost(i think)
 *    iii) it appears that the from sockaddr_in address does not
 *		contain the family of the sending socket after
 *		receive completes, so it is kludged to AF_INET
 */
int
f_recvfrom(s, buf, len, flags, from, fromlen)
int s;
char *buf;
int len;
int flags;
struct sockaddr *from;
int *fromlen;
{
	int rval;

	rval = receive(s, from, buf, len);
	if (rval != -1 && ((struct sockaddr_in *)from)->sin_family == 0)
		((struct sockaddr_in *)from)->sin_family = AF_INET;
	return(rval);

}



/* 
 * f_select maps 4.2 select => 4.1 select
 *
 * caveats:
 *	i) 4.1 select does not support an exceptional conditions file desc mask
 * notes:
 *	i) timeouts are specified differently. i believe there is no loss
 *		of functionality between the 2 version in the mapping
 */
int
f_select(width, readfds, writefds, execptfds, timeout)
int width;
int *readfds;
int *writefds;
int *execptfds;
struct timeval *timeout;
{
	return(select(width, readfds, writefds, 
		(long)(timeout ? timeout->tv_sec * 1000 : 0xffffff)));
}

#ifdef DEBUG
int
fudgeerr(a)
char *a;
{
	write(fileno(stderr), a, strlen(a));
	return(1);
}

int
fudgedump(buf, len, header)
int *buf;
int len;
char *header;
{
	int i, j = 0;
	char msg[1000];

	for (i = 0; i < (len + 3)/4; i++) {
		j += sprintf(msg + j, "%s%.8x", (i % 5) ? "  " : header,  
			*(buf + i));
	}
	sprintf(msg + j, "\n");
	fudgeerr(msg);
}
#endif
#endif
