#ident	"@(#)innetgr.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
 *	System V NFS - Release 3.2/V3
 *
 *	Copyright 1986, 1987, 1988 Lachman Associates, Incorporated (LAI)
 *
 *	All Rights Reserved.
 *
 *	The copyright above and this notice must be preserved in all
 *	copies of this source code.  The copyright above does not
 *	evidence any actual or intended publication of this source
 *	code.
 *
 *	This is unpublished proprietary trade secret source code of
 *	Lachman Associates.  This source code may not be copied,
 *	disclosed, distributed, demonstrated or licensed except as
 *	expressly authorized by Lachman Associates.
 */
#ident	"@(#)innetgr.c	3.2 LAI System V NFS Release 3.2/V3	source"
/* 
 * Copyright (c) 1985 by Sun Microsystems, Inc.
 */

#include <stdio.h>
#include <ctype.h>
#include <rpcsvc/ypclnt.h>

/* 
 * innetgr: test whether I'm in /etc/netgroup
 * 
 */

extern char *malloc();

static char *any(), *strchr(), *strcpy();
static struct list {			/* list of names to check for loops */
	char *name;
	struct list *nxt;
} *list;
static char *name, *machine, *domain;
static char thisdomain[256];

innetgr(grp, mach, nm, dom)
	char *grp;
	char *mach;
	char *nm;
	char *dom;
{
	register struct list *ls;
	int res;

	if (getdomainname(thisdomain, sizeof(thisdomain)) < 0) {
		(void) fprintf(stderr, 
		    "innetgr: getdomainname system call missing\n");
	    exit(1);
	}
	if (list) {
		for (ls = list; ls != NULL; ls = ls->nxt) {
			free(ls->name);
			free((char *) ls);
		}
	}
	list = NULL;
	machine = mach;
	name = nm;
	domain = dom;
	if (domain) {
		if (name && !machine) {
			if (lookup("netgroup.byuser",grp,name,domain,&res)) {
				return(res);
			}
		} else if (machine && !name) {
			if (lookup("netgroup.byhost",grp,machine,domain,&res)) {
				return(res);
			}
		}
	}
	return doit(grp);
}
	

	

/* 
 * calls itself recursively
 */
static
doit(group)
	char *group;
{
	char *key, *val;
	int vallen,keylen;
	char *r;
	int match;
	register char *p, *q;
	register struct list *ls;
	int err;
	
	key = group;
	keylen = strlen(group);
	err = yp_match(thisdomain, "netgroup", key, keylen, &val, &vallen);
	if (err) {
#ifdef DEBUG
		if (err == YPERR_KEY)
			(void) fprintf(stderr, "no such netgroup as %s\n", group);
		else
			(void) fprintf(stderr, "error: yp_match, %s\n",yperr_string(err));
#endif
		return(0);
	}
	/* 
	 * check for recursive loops
	 */
	for (ls = list; ls != NULL; ls = ls->nxt)
		if (strcmp(ls->name, group) == 0) {
			(void) fprintf(stderr, "%s called recursively\n", group);
			return(0);
		}
	ls = (struct list *) malloc(sizeof(struct list));
	ls->name = malloc((unsigned)keylen+1);
	(void) strcpy(ls->name, group);
	ls->nxt = list;
	list = ls;
	
	p = val;
	p[vallen] = 0;
	while (p != NULL) {
		match = 0;
		while (*p == ' ' || *p == '\t')
			p++;
		if (*p == 0 || *p == '#')
			break;
		if (*p == '(') {
			p++;
			while (*p == ' ' || *p == '\t')
				p++;
			r = q = strchr(p, ',');
			if (q == NULL) {
				(void) fprintf(stderr,
				    "syntax error in /etc/netgroup\n");
				return(0);
			}
			if (p == q || machine == NULL)
				match++;
			else {
				while (*(q-1) == ' ' || *(q-1) == '\t')
					q--;
				if (strncmp(machine, p, q-p) == 0)
					match++;
			}
			p = r+1;

			while (*p == ' ' || *p == '\t')
				p++;
			r = q = strchr(p, ',');
			if (q == NULL) {
				(void) fprintf(stderr,
				    "syntax error in /etc/netgroup\n");
				return(0);
			}
			if (p == q || name == NULL)
				match++;
			else {
				while (*(q-1) == ' ' || *(q-1) == '\t')
					q--;
				if (strncmp(name, p, q-p) == 0)
					match++;
			}
			p = r+1;

			while (*p == ' ' || *p == '\t')
				p++;
			r = q = strchr(p, ')');
			if (q == NULL) {
				(void) fprintf(stderr,
				    "syntax error in /etc/netgroup\n");
				return(0);
			}
			if (p == q || domain == NULL)
				match++;
			else {
				while (*(q-1) == ' ' || *(q-1) == '\t')
					q--;
				if (strncmp(domain, p, q-p) == 0)
					match++;
			}
			p = r+1;
			if (match == 3) {
				free(val);
				return 1;
			}
		}
		else {
			q = any(p, " \t\n#");
			if (q && *q == '#')
				break;
			if (q)
				*q = 0;
			if (doit(p)) {
				free(val);
				return 1;
			}
			if (q)
				*q = ' ';
		}
		p = any(p, " \t");
	}
	free(val);
	return 0;
}

/* 
 * scans cp, looking for a match with any character
 * in match.  Returns pointer to place in cp that matched
 * (or NULL if no match)
 */
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
 * return 1 if "what" is in the comma-separated, newline-terminated "list"
 */
static
inlist(what,list)
	char *what;
	char *list;
{
#	define TERMINATOR(c)    (c == ',' || c == '\n')

	register char *p;
	int len;
         
	len = strlen(what);     
	p = list;
	do {             
		if (strncmp(what,p,len) == 0 && TERMINATOR(p[len])) {
			return(1);
		}
		while (!TERMINATOR(*p)) {
			p++;
		}
		p++;
	} while (*p);
	return(0);
}




/*
 * Lookup a host or user name in a yp map.  Set result to 1 if group in the 
 * lookup-up list of groups. Return 1 if the map was found.
 */
static
lookup(map,group,name,domain,res)
	char *map;
	char *group;
	char *name;
	char *domain;
	int *res;
{
	int err;
	char *val;
	int vallen;
	char key[256];
	char *wild = "*";
	int i;

	for (i = 0; i < 4; i++) {
		switch (i) {
		case 0: makekey(key,name,domain); break;
		case 1: makekey(key,wild,domain); break;	
		case 2: makekey(key,name,wild); break;
		case 3: makekey(key,wild,wild); break;	
		}
		err  = yp_match(thisdomain,map,key,strlen(key),&val,&vallen); 
		if (!err) {
			*res = inlist(group,val);
			free(val);
			if (*res) {
				return(1);
			}
		} else {
#ifdef DEBUG
			(void) fprintf(stderr,
				"yp_match(%s,%s) failed: %s.\n",map,key,yperr_string(err));
#endif
			if (err != YPERR_KEY)  {
				return(0);
			}
		}
	}
	*res = 0;
	return(1);
}



/*
 * Generate a key for a netgroup.byXXXX yp map
 */
static
makekey(key,name,domain)
	register char *key;
	register char *name;
	register char *domain;
{
	while (*key++ = *name++)
		;
	*(key-1) = '.';
	while (*key++ = *domain++)
		;
}	
