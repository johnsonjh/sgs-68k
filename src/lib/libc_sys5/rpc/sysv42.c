#ident	"@(#)sysv42.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
 *	Copyright 1986 Lachman Associates Inc. and The Instruction Set Ltd.
 */
#ident	"@(#)sysv42.c	2.2 LAI KNFS for ARETE source"
#ident	"@(#)sysv42.c	2.9 System V NFS source"
/* 
 * The routines found in this file have been put together to
 * fudge some of the 4.2BSD routine necessary to make user level
 * rpc function on 5.2
 */
#include <sys/fs/nfs/time.h>
#include <sys/types.h>
#include <ctype.h>
#include <memory.h>


/*
 * gettimeofday 
 */
/* int
gettimeofday(tp, tzp)
struct timeval *tp;
struct timezone *tzp;
{
	static unsigned long mili = 0;

	tp->tv_sec = time((long *) 0);
	tp->tv_usec = (tp->tv_sec * 1000) + (mili++ % 1000);

	 * timezone handling is not performed
	
	return(tp->tv_sec < 0 ? -1 : 0);
}
*/

/*
 * random 
 */
long
random()
{
	return(lrand48());
}

/*
 * getgroups
 */
int 
getgroups(n, gidset)
int n;
int *gidset;
{
	unsigned short getgid();

	*gidset = (int) getgid();
	return(1);
}

char *
bcopy(src, dest, n)
register char *src;
register char *dest;
register int n;
{
	return(memcpy(dest, src, n));
}

char *
bzero(base, length)
char *base;
int length;
{
	return(memset(base, 0, length));
}

int
bcmp(s1, s2, n)
char *s1;
char *s2;
int n;
{
	return(memcmp(s2, s1, n));
}

