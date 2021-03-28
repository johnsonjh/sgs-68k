#ident	"@(#)getgrent.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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
#ifndef lint
#ident	"@(#)getgrent.c	3.3 LAI System V NFS Release 3.2/V3	source"
#endif
/*	Copyright (c) 1984 Sun Microsystems, Inc.  */

/*    Copyright (c) 1984 AT&T */
/*      All Rights Reserved   */

/*    THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T     */
/*    The copyright notice above does not evidence any        */
/*    actual or intended publication of such source code.     */

/*	@(#)getgrent.c	1.4	*/
/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
#include <stdio.h>
#include <grp.h>


#ifdef NFSMNT
#include <ctype.h>
#include <rpcsvc/ypclnt.h>

#define MAXGROUPLEN 1024
#define MAXINT 0x7fffffff;
#define	MAXGRP	200
#define	CL	':'
#define	CM	','
#define	NL	'\n'

/* 
 * access members of a netgroup
 */

struct grouplist {		/* also used by pwlib */
	char *gl_machine;
	char *gl_name;
	char *gl_domain;
	struct grouplist *gl_nxt;
};

extern char *malloc(), *fgets(), *strcpy(),*strncpy(), *strchr();
extern int atoi();

static char *any();

static struct grouplist *grouplist, *grlist;

static void getgroup();
static void doit();
static char *fill();
static char *match();

static char line[BUFSIZ+1];
static struct group grp;

static char domain[256];
static struct group NULLGP = {NULL, NULL, 0, NULL};
static char GROUP[] = "/etc/group";
static FILE *grf = NULL;	/* pointer into /etc/group */
static char *yp;		/* pointer into yellow pages */
static int yplen;
static char *oldyp = NULL;	
static int oldyplen;

static char *gr_mem[MAXGRP];
static struct list {
    char *name;
    struct list *nxt;
} *minuslist;			/* list of - items */

static struct group *interpret();
static struct group *interpretwithsave();
static struct group *save();
static struct group *getnamefromyellow();
static struct group *getgidfromyellow();

struct group *
getgrgid(gid)
register gid;
{
	struct group *gp;
	char line[BUFSIZ+1];

	setgrent();
	if (!grf)
		return NULL;
	while (fgets(line, BUFSIZ, grf) != NULL) {
		gp = interpret(line, strlen(line));
		if (matchgid(line, &gp, gid)) {
			endgrent();
			return gp;
		}
	}
	endgrent();
	return NULL;
}

struct group *
getgrnam(name)
register char *name;
{
	struct group *gp;
	char line[BUFSIZ+1];

	setgrent();
	if (!grf)
		return NULL;
	while (fgets(line, BUFSIZ, grf) != NULL) {
		gp = interpret(line, strlen(line));
		if (matchname(line, &gp, name)) {
			endgrent();
			return gp;
		}
	}
	endgrent();
	return NULL;
}

setgrent()
{
	if (getdomainname(domain, sizeof(domain)) < 0) {
		fprintf(stderr, 
		    "setgrent: getdomainname system call missing\n");
		exit(1);
	}
	if(!grf)
		grf = fopen(GROUP, "r");
	else
		rewind(grf);
	if (yp)
		free(yp);
	yp = NULL;
	freeminuslist();
}

endgrent()
{
	if(grf) {
		fclose(grf);
		grf = NULL;
	}
	if (yp)
		free(yp);
	yp = NULL;
	freeminuslist();
}

static char *
grskip(p,c)
	register char *p;
	register c;
{
	while(*p && *p != c && *p != '\n') ++p;
	if(*p) *p++ = 0;
	return(p);
}

struct group *
getgrent()
{
	char line1[BUFSIZ+1];
	register char *p, **q;
	static struct group *savegp, *gp;

	if (domain[0] == 0 && getdomainname(domain, sizeof(domain)) < 0) {
		fprintf(stderr, 
		    "getgrent: getdomainname system call missing\n");
		exit(1);
	}
	if(!grf && !(grf = fopen(GROUP, "r")))
		return(NULL);
  again:
	if (yp) {
		gp = interpretwithsave(yp, yplen, savegp);
		free(yp);
		getnextfromyellow();
		if (onminuslist(gp))
			goto again;
		else
			return gp;
	}
	else if (!(p = fgets(line1, BUFSIZ, grf)))
		return(NULL);
	gp = interpret(line1, strlen(line1));
		switch(line1[0]) {
			case '+':
				if (strcmp(gp->gr_name, "+") == 0) {
					getfirstfromyellow();
					savegp = save(gp);
					goto again;
				}
				savegp = save(gp);
				gp = getnamefromyellow(gp->gr_name+1, savegp);
				if (gp == NULL)
					goto again;
				else if (onminuslist(gp))
					goto again;
				else
					return gp;
				break;
			case '-':
				addtominuslist(gp->gr_name+1);
				goto again;
				break;
			default:
				if (onminuslist(gp))
					goto again;
				return gp;
				break;
		}
}

static struct group *
interpret(val, len)
	char *val;
{
	register char *p, **q;
	static struct group gp;
	static char line[BUFSIZ+1];

	strncpy(line, val, len);
	p = line;
	line[len] = '\n';
	line[len+1] = 0;

	gp.gr_name = p;
	gp.gr_passwd = p = grskip(p,':');
	gp.gr_gid = atoi(p = grskip(p,':'));
	gp.gr_mem = gr_mem;
	p = grskip(p,':');
	grskip(p,'\n');
	q = gr_mem;
	while(*p){
		if (q < &gr_mem[MAXGRP-1])
			*q++ = p;
		p = grskip(p,',');
	}
	*q = NULL;
	return(&gp);
}

static
freeminuslist() {
	struct list *ls;
	
	for (ls = minuslist; ls != NULL; ls = ls->nxt) {
		free(ls->name);
		free(ls);
	}
	minuslist = NULL;
}

static struct group *
interpretwithsave(val, len, savegp)
	char *val;
	struct group *savegp;
{
	struct group *gp;
	
	gp = interpret(val, len);
	if (savegp->gr_passwd && *savegp->gr_passwd)
		gp->gr_passwd =  savegp->gr_passwd;
	if (savegp->gr_mem && *savegp->gr_mem)
		gp->gr_mem = savegp->gr_mem;
	return gp;
}

static
onminuslist(gp)
	struct group *gp;
{
	struct list *ls;
	register char *nm;
	
	nm = gp->gr_name;
	for (ls = minuslist; ls != NULL; ls = ls->nxt)
		if (strcmp(ls->name, nm) == 0)
			return 1;
	return 0;
}

static
getnextfromyellow()
{
	int reason;
	char *key;
	int keylen;
	
	if (reason = yp_next(domain, "group.byname",
	    oldyp, oldyplen, &key, &keylen,
	    &yp, &yplen)) {
#ifdef DEBUG
fprintf(stderr, "reason yp_next failed is %d\n", reason);
#endif
		yp = NULL;
	}
	if (oldyp)
		free(oldyp);
	oldyp = key;
	oldyplen = keylen;
}

static
getfirstfromyellow()
{
	int reason;
	char *key;
	int keylen;
	
	if (reason =  yp_first(domain, "group.byname",
	    &key, &keylen, &yp, &yplen)) {
#ifdef DEBUG
fprintf(stderr, "reason yp_first failed is %d\n", reason);
#endif
		yp = NULL;
	}
	if (oldyp)
		free(oldyp);
	oldyp = key;
	oldyplen = keylen;
}

static struct group *
getnamefromyellow(name, savegp)
	char *name;
	struct group *savegp;
{
	struct group *gp;
	int reason;
	char *val;
	int vallen;
	
	if (reason = yp_match(domain, "group.byname",
	    name, strlen(name), &val, &vallen)) {
#ifdef DEBUG
fprintf(stderr, "reason yp_next failed is %d\n", reason);
#endif
		return NULL;
	}
	else {
		gp = interpret(val, vallen);
		free(val);
		if (savegp->gr_passwd && *savegp->gr_passwd)
			gp->gr_passwd =  savegp->gr_passwd;
		if (savegp->gr_mem && *savegp->gr_mem)
			gp->gr_mem = savegp->gr_mem;
		return gp;
	}
}

static
addtominuslist(name)
	char *name;
{
	struct list *ls;
	char *buf;
	
	ls = (struct list *)malloc(sizeof(struct list));
	buf = (char *)malloc(strlen(name) + 1);
	strcpy(buf, name);
	ls->name = buf;
	ls->nxt = minuslist;
	minuslist = ls;
}

/* 
 * save away psswd, gr_mem fields, which are the only
 * ones which can be specified in a local + entry to override the
 * value in the yellow pages
 */
static struct group *
save(gp)
	struct group *gp;
{
	static struct group *sv;
	char *gr_mem[MAXGRP];	
	char **av, **q;
	int lnth;
	
	/* 
	 * free up stuff from last time around
	 */
	if (sv) {
		for (av = sv->gr_mem; *av != NULL; av++) {
			if (q >= &gr_mem[MAXGRP-1])
				break;
			free(*q);
			q++;
		}
		free(sv->gr_passwd);
		free(sv->gr_mem);
		free(sv);
	}
	sv = (struct group *)malloc(sizeof(struct group));
	sv->gr_passwd = (char *)malloc(strlen(gp->gr_passwd) + 1);
	strcpy(sv->gr_passwd, gp->gr_passwd);

	q = gr_mem;
	for (av = gp->gr_mem; *av != NULL; av++) {
		if (q >= &gr_mem[MAXGRP-1])
			break;
		*q = (char *)malloc(strlen(*av) + 1);
		strcpy(*q, *av);
		q++;
	}
	*q = 0;
	lnth = (sizeof (char *)) * (q - gr_mem + 1);
	sv->gr_mem = (char **)malloc(lnth);
	bcopy(gr_mem, sv->gr_mem, lnth);
	return sv;
}

static
matchname(line1, gpp, name)
	char line1[];
	struct group **gpp;
	char *name;
{
	struct group *savegp;
	struct group *gp = *gpp;

	switch(line1[0]) {
		case '+':
			if (strcmp(gp->gr_name, "+") == 0) {
				savegp = save(gp);
				gp = getnamefromyellow(name, savegp);
				if (gp) {
					*gpp = gp;
					return 1;
				}
				else
					return 0;
			}
			if (strcmp(gp->gr_name+1, name) == 0) {
				savegp = save(gp);
				gp = getnamefromyellow(gp->gr_name+1, savegp);
				if (gp) {
					*gpp = gp;
					return 1;
				}
				else
					return 0;
			}
			break;
		case '-':
			if (strcmp(gp->gr_name+1, name) == 0) {
				*gpp = NULL;
				return 1;
			}
			break;
		default:
			if (strcmp(gp->gr_name, name) == 0)
				return 1;
	}
	return 0;
}

static
matchgid(line1, gpp, gid)
	char line1[];
	struct group **gpp;
{
	struct group *savegp;
	struct group *gp = *gpp;

	switch(line1[0]) {
		case '+':
			if (strcmp(gp->gr_name, "+") == 0) {
				savegp = save(gp);
				gp = getgidfromyellow(gid, savegp);
				if (gp) {
					*gpp = gp;
					return 1;
				}
				else
					return 0;
			}
			savegp = save(gp);
			gp = getnamefromyellow(gp->gr_name+1, savegp);
			if (gp && gp->gr_gid == gid) {
				*gpp = gp;
				return 1;
			}
			else
				return 0;
			break;
		case '-':
			if (gid == gidof(gp->gr_name+1)) {
				*gpp = NULL;
				return 1;
			}
			break;
		default:
			if (gp->gr_gid == gid)
				return 1;
	}
	return 0;
}

static
gidof(name)
	char *name;
{
	struct group *gp;
	
	gp = getnamefromyellow(name, &NULLGP);
	if (gp)
		return gp->gr_gid;
	else
		return MAXINT;
}

static struct group *
getgidfromyellow(gid, savegp)
	int gid;
	struct group *savegp;
{
	struct group *gp;
	int reason;
	char *val;
	int vallen;
	char gidstr[20];
	
	sprintf(gidstr, "%d", gid);
	if (reason = yp_match(domain, "group.bygid",
	    gidstr, strlen(gidstr), &val, &vallen)) {
#ifdef DEBUG
fprintf(stderr, "reason yp_next failed is %d\n", reason);
#endif
		return NULL;
	}
	else {
		gp = interpret(val, vallen);
		free(val);
		if (savegp->gr_passwd && *savegp->gr_passwd)
			gp->gr_passwd =  savegp->gr_passwd;
		if (savegp->gr_mem && *savegp->gr_mem)
			gp->gr_mem = savegp->gr_mem;
		return gp;
	}
}




static char *
fgrskip(p, c)
register char *p;
register int c;
{
	while(*p != '\0' && *p != c)
		++p;
	if(*p != '\0')
	 	*p++ = '\0';
	return(p);
}

struct group *
fgetgrent(f)
FILE *f;
{
	register char *p, **q;

	if((p = fgets(line, BUFSIZ, f)) == NULL)
		return(NULL);
	grp.gr_name = p;
	grp.gr_passwd = p = fgrskip(p, CL);
	grp.gr_gid = atoi(p = fgrskip(p, CL));
	grp.gr_mem = gr_mem;
	p = fgrskip(p, CL);
	(void) fgrskip(p, NL);
	q = gr_mem;
	while(*p != '\0') {
		*q++ = p;
		p = fgrskip(p, CM);
	}
	*q = NULL;
	return(&grp);
}

setnetgrent(grp)
	char *grp;
{
	static char oldgrp[256];
	
	if (strcmp(oldgrp, grp) == 0)
		grlist = grouplist;
	else {
		if (grouplist != NULL)
			endnetgrent();
		getgroup(grp);
		grlist = grouplist;
		(void) strcpy(oldgrp, grp);
	}
}

endnetgrent() {
	struct grouplist *gl;
	
	for (gl = grouplist; gl != NULL; gl = gl->gl_nxt) {
		if (gl->gl_name)
			free(gl->gl_name);
		if (gl->gl_domain)
			free(gl->gl_domain);
		if (gl->gl_machine)
			free(gl->gl_machine);
		free((char *) gl);
	}
	grouplist = NULL;
	grlist = NULL;
}

getnetgrent(machinep, namep, domainp)
	char **machinep, **namep, **domainp;
{
	if (grlist) {
		*machinep = grlist->gl_machine;
		*namep = grlist->gl_name;
		*domainp = grlist->gl_domain;
		grlist = grlist->gl_nxt;
		return (1);
	}
	else
		return (0);
}



static void
getgroup(grp)
	char *grp;
{
	if (getdomainname(domain, sizeof(domain)) < 0) {
		(void) fprintf(stderr, 
		    "getnetgrent: getdomainname system call missing\n");
	    exit(1);
	}
	doit(grp,(struct list *) NULL);
}
		

/*
 * recursive function to find the members of netgroup "group". "list" is
 * the path followed through the netgroups so far, to check for cycles.
 */
static void
doit(group,list)
    char *group;
    struct list *list;
{
    register char *p, *q;
    register struct list *ls;
    char *val;
    struct grouplist *gpls;
 
 
    /*
     * check for non-existing groups
     */
    if ((val = match(group)) == NULL) {
        return;
    }
 
 
    /*
     * check for cycles
     */
    for (ls = list; ls != NULL; ls = ls->nxt) {
        if (strcmp(ls->name, group) == 0) {
            (void) fprintf(stderr,
					"Cycle detected in /etc/netgroup: %s.\n",group);
            return;
        }
    }
 
 
    ls = (struct list *) malloc(sizeof(struct list));
    ls->name = group;
    ls->nxt = list;
    list = ls;
    
    p = val;
    while (p != NULL) {
        while (*p == ' ' || *p == '\t')
            p++;
        if (*p == 0 || *p =='#')
            break;
        if (*p == '(') {
            gpls = (struct grouplist *) malloc(sizeof(struct grouplist));
            p++;
 
            if (!(p = fill(p,&gpls->gl_machine,',')))  {
                goto syntax_error;
            }
            if (!(p = fill(p,&gpls->gl_name,','))) {
                goto syntax_error;
            }
            if (!(p = fill(p,&gpls->gl_domain,')'))) {
                goto syntax_error;
            }
            gpls->gl_nxt = grouplist;
            grouplist = gpls;
        } else {
            q = any(p, " \t\n#");
            if (q && *q == '#')
                break;
            *q = 0;
            doit(p,list);
            *q = ' ';
        }
        p = any(p, " \t");
    }
    free(ls);
    return;
 
syntax_error:
    (void) fprintf(stderr,"syntax error in /etc/netgroup\n");
    (void) fprintf(stderr,"--- %s\n",val);
    free(ls);
    return;
}



/*
 * Fill a buffer "target" selectively from buffer "start".
 * "termchar" terminates the information in start, and preceding
 * or trailing white space is ignored. The location just after the
 * terminating character is returned.  
 */
static char *
fill(start,target,termchar)
    char *start;
    char **target;
    char termchar;
{
    register char *p;
    register char *q;
    char *r;
	unsigned size;
	
 
    for (p = start; *p == ' ' || *p == '\t'; p++)
        ;
    r = strchr(p, termchar);
    if (r == NULL) {
        return(NULL);
    }
    if (p == r) {
		*target = NULL;	
    } else {
        for (q = r-1; *q == ' ' || *q == '\t'; q--)
            ;
		size = q - p + 1;
		*target = malloc(size+1);
		(void) strncpy(*target,p,(int) size);
		(*target)[size] = 0;
	}
    return(r+1);
}



static char *
match(group)
	char *group;
{
	char *val;
	int vallen;
	int err;

	err = yp_match(domain,"netgroup",group,strlen(group),&val,&vallen);
	if (err) {
#ifdef DEBUG
		(void) fprintf(stderr,"yp_match(netgroup,%s) failed: %s\n",group
				,yperr_string(err));
#endif
		return(NULL);
	}
	return(val);
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
#else


#define	CL	':'
#define	CM	','
#define	NL	'\n'
#define TRUE	1
#define FALSE	0

extern int atoi(), fclose();
extern char *fgets();
extern FILE *fopen();
extern void rewind();

static char GROUP[] = "/etc/group";
static FILE *grf = NULL;
static char *line, *gr_mem;
static struct group grp;
static int size, gr_size;

void
setgrent()
{
	if (grf == NULL)
		grf = fopen(GROUP, "r");
	else
		rewind(grf);
}

void
endgrent()
{
	if (grf != NULL)
		{
		(void) fclose(grf);
		grf = NULL;
		}
}

static void
cleanup()
{
	if (line != NULL)
		{
		free(line);
		line = NULL;
		}
	if (gr_mem != NULL)
		{
		free (gr_mem);
		gr_mem = NULL;
		}
	(void) endgrent();
}

static char *
grskip(p, c)
char *p;
int c;
{
	while (*p != '\0' && *p != c)
		++p;
	if (*p != '\0')
	 	*p++ = '\0';
	return(p);
}

struct group *
getgrent()
{
	extern struct group *fgetgrent();

	if (grf == NULL && (grf = fopen(GROUP, "r")) == NULL)
		return(NULL);
	return (fgetgrent(grf));
}

struct group *
fgetgrent(f)
FILE *f;
{
	char *p, **q;
	int len, count;
	long offset, ftell(), lseek();
	char done, *calloc(), *realloc();

	count = 1;
	if (line == NULL)
		{
		size = BUFSIZ+1;
		if ((line = calloc((unsigned)size, sizeof(char))) == NULL)
			{
			(void) cleanup();
			return(NULL);
			}
		}
	done = FALSE;
	while (!done)
		{
		offset = ftell(f);
		if ((p = fgets(line, size, f)) == NULL)
			return(NULL);
		len = strlen(p);
		if ((len <= size) && (p[len-1] == NL))
			done = TRUE;
		else
			{
			size *= 32;
			if ((line = realloc(line, (unsigned)size * sizeof(char))) == NULL)
				{
				(void) cleanup();
				return(NULL);
				}
			fseek(f, offset, 0);
			}
		}
	grp.gr_name = p;
	grp.gr_passwd = p = grskip(p, CL);
	grp.gr_gid = atoi(p = grskip(p, CL));
	p = grskip(p, CL);
	(void) grskip(p, NL);
	if (gr_mem == NULL)
		{
		gr_size = 2;
		if ((gr_mem = calloc((unsigned)(gr_size), sizeof(char *))) == NULL)
			{
			(void) cleanup();
			return(NULL);
			}
		}
		grp.gr_mem = (char **)gr_mem;
		q = grp.gr_mem;
		while (*p != '\0')
			{
			if (count >= gr_size - 1)
				{
				*q = NULL;
				gr_size *= 2;
				if ((gr_mem = realloc(gr_mem, (unsigned)(gr_size) * sizeof(char *))) == NULL)
					{
					(void) cleanup();
					return(NULL);
					}
				grp.gr_mem = (char **)gr_mem;
				q = grp.gr_mem;
				while (*q != NULL)
					q++;
				}
			count++;
			*q++ = p;
			p = grskip(p, CM);
			}
		*q = NULL;
		return(&grp);
}
#endif
