#ident	"@(#)rhost.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
 * Copyright (C) 1986, Excelan, Inc.  All rights reserved.
 *
 * $Header: rhost.c,v 1.1 86/12/29 08:19:46 alexl Exp $
 *
 * 001 10/20/87 JPC: use experror
 */
#ifdef S3000

#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#ifdef EXOS
#include <exos/misc.h>
#include <netinet/in.h>
#else
#include <CMC/in.h>
#endif

#ident	"@(#)rhost.c	1.8 10/14/85"

#ifdef SYSTEM5
#define	index(s,c) strchr(s,c)
#define rindex(s,c) strrchr(s,c)
extern char *strchr(), *strrchr();
#else
extern char	*index(), *rindex();
#endif
extern void	experror();			/* 001 */

/* this mess fixes bug in unisoft V7 callan port - GAP 1-13-84 */
#ifdef cadmus
#undef isxdigit
#define isxdigit(c) (isdigit(c)||((c>='A'&&c<='F')||(c>='a'&&c<='f')))
#else
#ifdef VERSION7
#undef isxdigit
#define isxdigit(c) (isdigit(c)||((c>='A'&&c<='F')||(c>='a'&&c<='f')))
#endif
#endif

#ifndef EXOS
static char 	HOSTSLOCAL[] = "/etc/hosts.local";
#endif
char	*malloc();
long    rnumber();
char	*firstwhite();
char	*lastwhite();
char	*skipwhite();

long
rhost(ahost)
	char **ahost;
{
	FILE *hf;
	char hbuf[BUFSIZ], *cp;
	int first = 1;
	long addr;
	char *endp;
#ifdef EXOS
	static char	hosts[] = HOSTS;
#else
	static char	hosts[] = "/etc/hosts"; 
	static char 	local[] = "/etc/hosts.local";
#endif

	if (isdigit(**ahost) && (addr = rnumber(*ahost)) != -1) {
		return (addr);
	}
	hf = fopen(hosts, "r");
	if (hf == NULL) {
		experror(hosts);		/* 001 */
		return( -1 );
	}
top:
	while (fgets(hbuf, sizeof (hbuf), hf)) {
		if (hbuf[0] == '#')
			continue;
		if( endp = index(hbuf, '#') ) {
			*endp = 0;
		} else {
			*index(hbuf, '\n') = 0;
		}
		for (;;) {
			cp = lastwhite(hbuf, ' ');
			if (cp == NULL)
				break;
			if (!strcmp(cp+1, *ahost)) {
				if ((addr = rnumber(hbuf)) == -1)
					goto bad;
				fclose(hf);
				*ahost = firstwhite(hbuf, ' ') + 1;
				*ahost = skipwhite( *ahost );
				cp = firstwhite(*ahost, ' ');
				if (cp)
					*cp = 0;
				cp = malloc(strlen(*ahost)+1);
				strcpy(cp, *ahost);
				*ahost = cp;
				return (addr);
			}
			*cp = 0;
		}
	}
	if (first == 1) {
		first = 0;
		fclose(hf);
		if (hf = fopen(HOSTSLOCAL, "r"))
			goto top;
		return (-1);
	}
bad:
	fclose(hf);
	return (-1);
}

long
rnumber(cp)
	register char *cp;
{
	register long val;
	register int base;
	register char c;
	long parts = 0;
	char *pplow = (char *)&parts;
	char *pplim = pplow+4;
	char *pp = pplow;
	long net, imp, hoi;

	if (index(cp, '/') == 0)
		goto again;
	hoi = atoi(cp);
	if (index(cp, ',')) {
		imp = atoi(index(cp, '/') + 1);
		net = atoi(index(cp, ',') + 1);
		hoi = ntohs((short)hoi);
		val = (net<<24)|(hoi<<8)|imp;
	} else {
		net = atoi(index(cp, '/') + 1);
		val = (net<<24)|hoi;
	}
	/*
	val = htonl(val);
	*/
	return (val);

again:
	val = 0; base = 10;
	if (*cp == '0')
		base = 8, cp++;
	if (*cp == 'x' || *cp == 'X')
		base = 16, cp++;
	while (c = *cp) {
		if (isdigit(c)) {
			val = (val * base) + (c - '0');
			cp++;
			continue;
		}
		if (base == 16 && isxdigit(c)) {
			val = (val << 4) + (c + 10 - (islower(c) ? 'a' : 'A'));
			cp++;
			continue;
		}
		break;
	}
	if (*cp == '.') {
		/*
		 * Internet format:
		 *	net.host.lh.imp
		 */
		if (pp >= pplim)
			return (-1);
		*pp++ = val, cp++;
		goto again;
	}
	if (*cp) {
		/*
		if (*cp == 'n') return (htonl(val));
		*/
		if (*cp == 'n') return (val);
		if ( *cp != ' '&& *cp != '\t' ) return (-1);
	}
	if (pp >= pplim)
		return (-1);
	*pp++ = val;
	/*
	return htonl(parts);
	*/
	return (parts);
}

char *
skipwhite( cpt )
register char	*cpt;
{
	register int	c;

	if (cpt == NULL)
		return (NULL);
	while ((c = *cpt) == ' ' || c == '\t')
		++cpt;
	return ( cpt );
}


/*
 * find first white space
 */
char *
firstwhite( cpt, ch )
register char	*cpt;
char		ch;
{
	register int	c;

	if (cpt == NULL)
		return (NULL);
	while ((c = *cpt) && c != ' ' && c != '\t' )
		++cpt;
	return ( c ? cpt : NULL );
}

/*
 * find last white space
 */
char *
lastwhite( cpt, ch )
register char	*cpt;
char		ch;
{
	register char	*ocpt = NULL;

	while ( ( cpt = firstwhite( cpt, ' ' ) ) != NULL ) {
		ocpt = cpt;
		cpt++;
	}
	return ( ocpt );
}
#endif	/* S3000 */
