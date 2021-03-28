#ident	"@(#)qsort.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libc-port:gen/qsort.c	1.6"	*/
	

/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/

#include <stdlib.h>
#include <string.h>

#ifndef pdp11
#if ( vax || 1 )   /* hanna arete 68k family should probably use this too */
		   /* number is determined experimentally on vax-11/780 */
#define _MINCPY	24 /* minimum number of characters worth using memcpy for */
#else              /* number is determined experimentally on 3b20s */
#define _MINCPY	8  /* minimum number of characters worth using memcpy for */
#endif
#define _CPY(i, j) ((void) memcpy(i, j, n))

#include <sys/types.h>

static char *qsbuf = NULL;
#endif

static size_t qses;
static int (*qscmp)(const void *, const void *);

static void qs1 ( char *a, char *l);
static void qsexc ( register char *ri, register char *rj );
static void qstexc ( register char *ri, register char *rj, register char *rk );

void qsort(void *a, size_t n, size_t es, int(*fc)(const void *,const void *))
{

#ifndef pdp11
	{
		static unsigned qsbufsize;

		if (es >= _MINCPY)
			if (qsbuf == NULL)
				qsbuf = (char *)malloc(qsbufsize = es);
			else if (qsbufsize < es)
				qsbuf = (char *)realloc(qsbuf, qsbufsize = es);
	}
#endif
	qscmp = fc;
	qses = es;
	qs1((char *)a, (char *)((char *)a + n*es));
}

static void qs1 ( char *a, char *l)
{
	register char *i, *j;
	register size_t es;
	char	*lp, *hp;
	int	c;
	unsigned n;

	es = qses;
start:
	if((n=l-a) <= es)
		return;
	n = es * (n / (2*es));
	hp = lp = a+n;
	i = a;
	j = l-es;
	while(1) {
		if(i < lp) {
			if((c = (*qscmp)(i, lp)) == 0) {
				qsexc(i, lp -= es);
				continue;
			}
			if(c < 0) {
				i += es;
				continue;
			}
		}

loop:
		if(j > hp) {
			if((c = (*qscmp)(hp, j)) == 0) {
				qsexc(hp += es, j);
				goto loop;
			}
			if(c > 0) {
				if(i == lp) {
					qstexc(i, hp += es, j);
					i = lp += es;
					goto loop;
				}
				qsexc(i, j);
				j -= es;
				i += es;
				continue;
			}
			j -= es;
			goto loop;
		}

		if(i == lp) {
			if(lp-a >= l-hp) {
				qs1(hp+es, l);
				l = lp;
			} else {
				qs1(a, lp);
				a = hp+es;
			}
			goto start;
		}

		qstexc(j, lp -= es, i);
		j = hp -= es;
	}
}

static void qsexc ( register char *ri, register char *rj )
{
	register size_t n = qses;

#ifndef pdp11
	if (n >= _MINCPY && qsbuf != NULL) {
		_CPY(qsbuf, ri);
		_CPY(ri, rj);
		_CPY(rj, qsbuf);
		return;
	}
#endif
	do {
		register char c = *ri;
		*ri++ = *rj;
		*rj++ = c;
	} while(--n);
}

static void qstexc ( register char *ri, register char *rj, register char *rk )
{
	register size_t n = qses;

#ifndef pdp11
	if (n >= _MINCPY && qsbuf != NULL) {
		_CPY(qsbuf, ri);
		_CPY(ri, rk);
		_CPY(rk, rj);
		_CPY(rj, qsbuf);
		return;
	}
#endif
	do {
		register char c = *ri;
		*ri++ = *rk;
		*rk++ = *rj;
		*rj++ = c;
	} while(--n);
}
