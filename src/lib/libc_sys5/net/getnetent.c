#ident	"@(#)getnetent.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
 *	Copyright (c) 1984 Sun Microsystems, Inc.  
 */
/*
 *	Copyright 1986 Lachman Associates Inc. and The Instruction Set Ltd.
 */
#ident	"@(#)getnetent.c	2.4 LAI KNFS for ARETE source"
#ident	"@(#)getnetent.c	2.4 System V NFS source"
/* 
 * Copyright (c) 1984 by Sun Microsystems, Inc.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/fs/nfs/inet_insup.h> 
#ifdef EXOS
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#else
#include <CMC/socket.h>
#include <CMC/in.h>  
#include <CMC/netdb.h>
#endif

#include <sys/fs/nfs/inet.h>  
#include <rpcsvc/ypclnt.h>

/*
 * Internet version.
 */
#define	MAXALIASES	35
#define	MAXADDRSIZE	14

static char domain[256];
static int stayopen;
static char *current = NULL;	/* current entry, analogous to netf */
static int currentlen;
static struct netent *interpret();
struct netent *getnetent();
char *inet_ntoa();
static char *any();
static char NETDB[] = "/etc/networks";
static FILE *netf = NULL;
static int usingyellow;		/* are yellow pages up? */
static char *nettoa();

struct netent *
getnetbyaddr(net, type)
{
	register struct netent *p;
	int reason;
	char *adrstr, *val;
	int vallen;

	setnetent(0);
	if (!usingyellow) {
		while (p = getnetent()) {
			if (p->n_addrtype == type && p->n_net == net)
				break;
		}
	}
	else {
		adrstr = nettoa(net);
		if (reason = yp_match(domain, "networks.byaddr",
		    adrstr, strlen(adrstr), &val, &vallen)) {
#ifdef DEBUG
			fprintf(stderr, "reason yp_first failed is %d\n",
			    reason);
#endif
			p = NULL;
		    }
		else {
			p = interpret(val, vallen);
			free(val);
		}
	}
	endnetent();
	return (p);
}

struct netent *
getnetbyname(name)
	register char *name;
{
	register struct netent *p;
	register char **cp;
	int reason;
	char *val;
	int vallen;

	setnetent(0);
	if (!usingyellow) {
		while (p = getnetent()) {
			if (strcmp(p->n_name, name) == 0)
				break;
			for (cp = p->n_aliases; *cp != 0; cp++)
				if (strcmp(*cp, name) == 0)
					goto found;
		}
	}
	else {
		if (reason = yp_match(domain, "networks.byname",
		    name, strlen(name), &val, &vallen)) {
#ifdef DEBUG
			fprintf(stderr, "reason yp_first failed is %d\n",
			    reason);
#endif
			p = NULL;
		    }
		else {
			p = interpret(val, vallen);
			free(val);
		}
	}
found:
	endnetent();
	return (p);
}

setnetent(f)
	int f;
{
	if (getdomainname(domain, sizeof(domain)) < 0) {
		fprintf(stderr, 
		    "setnetent: getdomainname system call missing\n");
		exit(1);
	}
	if (netf == NULL)
		netf = fopen(NETDB, "r");
	else
		rewind(netf);
	if (current)
		free(current);
	current = NULL;
	stayopen |= f;
	yellowup(1);	/* recompute whether yellow pages are up */
}

endnetent()
{
	if (current && !stayopen) {
		free(current);
		current = NULL;
	}
	if (netf && !stayopen) {
		fclose(netf);
		netf = NULL;
	}
}

struct netent *
getnetent()
{
	int reason;
	char *key, *val;
	int keylen, vallen;
	static char line1[BUFSIZ+1];
	struct netent *np;

	yellowup(0);
	if (!usingyellow) {
		if (netf == NULL && (netf = fopen(NETDB, "r")) == NULL)
			return (NULL);
	        if (fgets(line1, BUFSIZ, netf) == NULL)
			return (NULL);
		return interpret(line1, strlen(line1));
	}
	if (current == NULL) {
		if (reason =  yp_first(domain, "networks.byaddr",
		    &key, &keylen, &val, &vallen)) {
#ifdef DEBUG
			fprintf(stderr, "reason yp_first failed is %d\n",
			    reason);
#endif
			return NULL;
		    }
	}
	else {
		if (reason = yp_next(domain, "networks.byaddr",
		    current, currentlen, &key, &keylen, &val, &vallen)) {
#ifdef DEBUG
			fprintf(stderr, "reason yp_next failed is %d\n",
			    reason);
#endif
			return NULL;
		}
	}
	if (current)
		free(current);
	current = key;
	currentlen = keylen;
	np = interpret(val, vallen);
	free(val);
	return (np);
}

static struct netent *
interpret(val, len)
{
	static char *net_aliases[MAXALIASES];
	static struct netent net;
	static char line[BUFSIZ+1];
	char *p;
	register char *cp, **q;

	strncpy(line, val, len);
	p = line;
	line[len] = '\n';
	if (*p == '#')
		return (getnetent());
	cp = any(p, "#\n");
	if (cp == NULL)
		return (getnetent());
	*cp = '\0';
	net.n_name = p;
	cp = any(p, " \t");
	if (cp == NULL)
		return (getnetent());
	*cp++ = '\0';
	while (*cp == ' ' || *cp == '\t')
		cp++;
	p = any(cp, " \t");
	if (p != NULL)
		*p++ = '\0';
	net.n_net = inet_network(cp);
	net.n_addrtype = AF_INET;
	q = net.n_aliases = net_aliases;
	if (p != NULL) 
		cp = p;
	while (cp && *cp) {
		if (*cp == ' ' || *cp == '\t') {
			cp++;
			continue;
		}
		if (q < &net_aliases[MAXALIASES - 1])
			*q++ = cp;
		cp = any(cp, " \t");
		if (cp != NULL)
			*cp++ = '\0';
	}
	*q = NULL;
	return (&net);
}

static char *
any(cp, match)
	register char *cp;
	char *match;
{
	register char *mp, c;

	while (c = *cp) {
		for (mp = match; *mp; mp++)
			if (*mp == c)
				return (cp);
		cp++;
	}
	return ((char *)0);
}

/* 
 * check to see if yellow pages are up, and store that fact in usingyellow.
 * The check is performed once at startup and thereafter if flag is set
 */
static
yellowup(flag)
{
	static int firsttime = 1;
	char *key, *val;
	int keylen, vallen;

	if (firsttime || flag) {
		firsttime = 0;
		if (domain[0] == 0) {
			if (getdomainname(domain, sizeof(domain)) < 0) {
				fprintf(stderr, 
			    "getnetent: getdomainname system call missing\n");
				exit(1);
			}
		}
		usingyellow = !yp_bind(domain);
	}	
}

static
char *
nettoa(net)
	unsigned net;
{
	static char buf[10];
	char *p, *strchr(), *strrchr();
	struct in_addr in;
	int addr;

	in = inet_makeaddr(net, INADDR_ANY);
	addr = in.s_addr;
	strcpy(buf, inet_ntoa(in));
#ifdef EXOS
	if (IN_CLASSA & addr) {
#else
	if (IN_CLASSA(htonl(addr))) {
#endif
		p = strchr(buf, '.');
		if (p == NULL)
			return NULL;
		*p = 0;
	}
#ifdef EXOS
	else if (IN_CLASSB & addr) {
#else
	else if (IN_CLASSB(htonl(addr))) {
#endif
		p = strchr(buf, '.');
		if (p == NULL)
			return NULL;
		p = strchr(p+1, '.');
		if (p == NULL)
			return NULL;
		*p = 0;
	}
#ifdef EXOS
#define IN_CLASSC (IN_CLASSA | IN_CLASSB)
	else if (IN_CLASSC & addr) {
#else
	else if (IN_CLASSC(htonl(addr))) {
#endif
		p = strrchr(buf, '.');
		if (p == NULL)
			return NULL;
		*p = 0;
	}
	return buf;
}
