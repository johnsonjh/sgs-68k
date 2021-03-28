/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) error.c: version 25.1 created on 12/2/91 at 14:55:33	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)error.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)efl:error.c	25.1"


#include "defs"

char *linerr()
{
static char buff[50];
register int i;

for(i = filedepth; i>0 && filenames[i]==NULL ; --i)
	;
if(i > 0)
	sprintf(buff, "on line %d of file %s", yylineno, filenames[i]);
else
	sprintf(buff, "on line %d", yylineno);
return(buff);
}



laberr(s,t)
char *s;
char *t;
{
errmess("Label error", s, t);
}





exprerr(s,t)
char *s;
ptr t;
{
errmess("Expression error", s, t);
}




execerr(s,t)
char *s, *t;
{
errmess("Error", s, t);
}


errmess(m,s,t)
char *m, *s, *t;
{
fprintf(diagfile, "**%s %s:  ", m, linerr());
if(s)
	fprintf(diagfile, s, t);
fprintf(diagfile, "\n");
++nerrs;
}



dclerr(s, n)
char *s, *n;
{
extern int nerrs;

fprintf(diagfile, "**Error %s: Declaration for %s: %s\n",
		linerr(), n, s);
++nerrs;
}




badtag(routine, tag)
char *routine;
int tag;
{
char buff[100];
sprintf(buff, "impossible tag %d in routine %s", tag, routine);
fatal(buff);
}



fatal1(s,t)
char *s;
int t;
{

sprintf(msg, s, t);
fatal(msg);
}



fatal(s)
char *s;
{
fprintf(diagfile, "\n***Compiler error %s.", linerr());
if(s) fprintf(diagfile, "   %s.", s);
fprintf(diagfile, "\n");
fflush(stdout);

if(dumpcore)
	abort(0);
else	{
	rmiis();
	exit(-1);
	}
}



warn1(s,t)
char *s;
int t;
{
sprintf(msg, s, t);
warn(msg);
}




warn(s)
char *s;
{
++nwarns;
if( !nowarnflag)
	fprintf(diagfile, "*Warning: %s\n", s);
}



yyerror(s)
char *s;
{
errmess(s, CNULL, CNULL);
}
