/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) open.c: version 25.1 created on 12/2/91 at 18:43:09	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)open.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libI77:open.c	1.8"	*/
	
#ident	"@(#)libI77:open.c	25.1"


#include	"sys/types.h"
#include	"sys/stat.h"
#include "fio.h"
extern char *mktemp(), *malloc(), *strcpy();
f_open(a) olist *a;
{	unit *b;
	int n;
	char buf[256];
	cllist x;
	if(a->ounit>=MXUNIT || a->ounit<0)
		err(a->oerr,101,"open")
	b= &units[a->ounit];
	if(b->ufd!=0) goto connected;
unconnected:
	b->url=a->orl;
	if(a->oblnk && (*a->oblnk=='z' || *a->oblnk == 'Z')) b->ublnk=1;
	else b->ublnk=0;
	if(a->ofm==0)
	{	if(b->url>0) b->ufmt=0;
		else b->ufmt=1;
	}
	else if(*a->ofm=='f' || *a->ofm == 'F') b->ufmt=1;
	else b->ufmt=0;
	if(a->osta==0) {
		if(a->ofnm)
			goto unknown;
		else
			goto scratch;
	}
	switch(*a->osta)
	{
	unknown:
	default:
	case 'o':
	case 'O':
		if(a->ofnm==0) err(a->oerr,107,"open")
		g_char(a->ofnm,a->ofnmlen,buf);
		b->uscrtch=0;
		if(a->osta == 0)
			goto done;
		if((*a->osta=='o' || *a->osta == 'O') && access(buf,0))
			err(a->oerr,errno,"open")
	done:
		b->ufnm=(char *) malloc((unsigned int)(strlen(buf)+1));
		if(b->ufnm==NULL) err(a->oerr,113,"no space");
		(void) strcpy(b->ufnm,buf);
		b->uend=0;
		if(isdev(buf))
		{	b->ufd = fopen(buf,"r");
			if(b->ufd==NULL) err(a->oerr,errno,buf)
			else	b->uwrt = 0;
		}
		else
		{	if ((b->ufd = fopen(buf, "r+")) || (b->ufd = fopen(buf, "w+")))
				if (fseek(b->ufd, 0L, 2))
					err(a->oerr,errno,"open")
				else
					b->uwrt = 1;
			else if((b->ufd = fopen(buf, "r")) != NULL)
			{	(void) fseek(b->ufd, 0L, 2);
				b->uwrt = 0;
			}
			else	err(a->oerr, errno, buf)
			if(b->url > 0)	/* one can more easily find the end */
				(void) fseek(b->ufd, 0L, 0);
		}
		b->useek=canseek(b->ufd);
		if((b->uinode=inode(buf))==-1)
			err(a->oerr,108,"open")
		if(a->orl && b->useek) rewind(b->ufd);
		return(0);
	 case 's':
	 case 'S':
	 scratch:
		b->uscrtch=1;
		(void) strcpy(buf,"tmp.FXXXXXX");
		(void) mktemp(buf);
		(void) close(creat(buf, 0666));
		goto done;
	case 'n':
	case 'N':
		b->uscrtch=0;
		if(a->ofnm==0) err(a->oerr,107,"open")
		g_char(a->ofnm,a->ofnmlen,buf);
		(void) close(creat(buf, 0666));
		goto done;
	}
connected:
	if(a->ofnm==0)
	{
	same:	if(a->oblnk!= 0) b->ublnk= (*a->oblnk== 'z' || *a->oblnk== 'Z')?1:0;		/* emf 4/26/84 */
		return(0);
	}
	g_char(a->ofnm,a->ofnmlen,buf);
	if(inode(buf)==b->uinode) goto same;
	x.cunit=a->ounit;
	x.csta=0;
	x.cerr=a->oerr;
	if((n=f_clos(&x))!=0) return(n);
	goto unconnected;
}
fk_open(seq,fmt,n) ftnint n;
{	char nbuf[10];
	olist a;
	(void) sprintf(nbuf,"fort.%ld",n);
	a.oerr=1;
	a.ounit=n;
	a.ofnm=nbuf;
	a.ofnmlen=strlen(nbuf);
	a.osta=NULL;
	a.oacc= seq==SEQ?"s":"d";
	a.ofm = fmt==FMT?"f":"u";
	a.orl = seq==DIR?1:0;
	a.oblnk=NULL;
	return(f_open(&a));
}
isdev(s) char *s;
{	struct stat x;
	int j;
	if(stat(s, &x) == -1) return(0);
	if((j = (x.st_mode&S_IFMT)) == S_IFREG || j == S_IFDIR) return(0);
	else	return(1);
}
