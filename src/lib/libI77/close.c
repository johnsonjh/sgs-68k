/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) close.c: version 25.1 created on 12/2/91 at 18:41:57	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)close.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libI77:close.c	1.5"	*/
	
#ident	"@(#)libI77:close.c	25.1"

/*	3.0 SID #	1.2	*/
#include "fio.h"
f_clos(a) cllist *a;
{	unit *b;
	if(a->cunit >= MXUNIT) return(0);
	b= &units[a->cunit];
	if(b->ufd==NULL) return(0);
	b->uend=0;
	if(a->csta!=0)
		switch(*a->csta)
		{
		default:
		keep:
		case 'k':
		case 'K':		/* emf 4/26/84 */
			if (b->uscrtch!=0) {fprintf(stderr,"Cannot keep scratch file associated with unit %d\n",a->cunit); return(105);}   /* emf 4/26/84 */
			if(b->uwrt) (void) t_runc(b);
			(void) fclose(b->ufd);	/* sys 5 has strange beliefs */
			if(b->ufnm!=0) free(b->ufnm);
			b->ufnm=NULL;
			b->ufd=NULL;
			return(0);
		case 'd':
		case 'D':		/* emf 4/26/84 */
		delete:
			(void) fclose(b->ufd);
			if(b->ufnm!=0)
			{	(void) unlink(b->ufnm); /*SYSDEP*/
				free(b->ufnm);
			}
			b->ufnm=NULL;
			b->ufd=NULL;
			return(0);
		}
	else if(b->uscrtch==1) goto delete;
	else goto keep;
}
f_exit()
{	int i;
	cllist xx;
	xx.cerr=1;
	xx.csta=NULL;
	for(i=0;i<MXUNIT;i++)
	{
		xx.cunit=i;
		(void) f_clos(&xx);
	}
}
flush_()
{	int i;
	for(i=0;i<MXUNIT;i++)
		if(units[i].ufd != NULL) (void) fflush(units[i].ufd);
}
