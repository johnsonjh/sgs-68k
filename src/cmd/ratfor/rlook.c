/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) rlook.c: version 25.1 created on 12/2/91 at 16:51:46	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)rlook.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"ratfor:rlook.c	1.3"	*/
	
#ident	"@(#)ratfor:rlook.c	25.1"

#define NULL 0
#define EOS 0
#define	HSHSIZ	101
struct	nlist {
	char	*name;
	char	*def;
	int	ydef;
	struct	nlist *next;
};

struct	nlist	*hshtab[HSHSIZ];
struct nlist	*lookup();
char	*install();
char	*malloc();
char	*copy();
int	hshval;

struct nlist *lookup(str)
char *str;
{
	register char *s1, *s2;
	register struct nlist *np;
	static struct nlist nodef;

	s1 = str;
	for (hshval = 0; *s1; )
		hshval += *s1++;
	hshval %= HSHSIZ;
	for (np = hshtab[hshval]; np!=NULL; np = np->next) {
		s1 = str;
		s2 = np->name;
		while (*s1++ == *s2)
			if (*s2++ == EOS)
				return(np);
	}
	return(&nodef);
}

char *install(nam, val, tran)
char *nam, *val;
int tran;
{
	register struct nlist *np;

	if ((np = lookup(nam))->name == NULL) {
		np = (struct nlist *)malloc(sizeof(*np));
		np->name = copy(nam);
		np->def = copy(val);
		np->ydef = tran;
		np->next = hshtab[hshval];
		hshtab[hshval] = np;
		return(np->def);
	}
	free(np->def);
	np->def = copy(val);
	return(np->def);
}

char *copy(s)
register char *s;
{
	register char *p, *s1;

	p = s1 = (char *) malloc((unsigned)strlen(s)+1);
	while (*s1++ = *s++);
	return(p);
}
