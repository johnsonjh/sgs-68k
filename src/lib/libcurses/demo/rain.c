/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) rain.c: version 25.1 created on 12/2/91 at 19:09:13	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)rain.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"curses:demo/rain.c	1.1"	*/
	
#ident	"@(#)curses:rain.c	25.1"

#define MINICURSES
#include <curses.h>
#include <signal.h>
/* rain 11/3/1980 EPS/CITHEP */

#define cursor(col,row) move(row,col)
main(argc,argv)
int argc;
char *argv[];
{
    char *malloc();
    float ranf();
    int onsig();
    register int x, y, j;
    static int xpos[5], ypos[5];
    setbuf(stdout,malloc(BUFSIZ));

    for (j=SIGHUP;j<=SIGTERM;j++)
	if (signal(j,SIG_IGN)!=SIG_IGN) signal(j,onsig);

    initscr();
    nl();
    noecho();
    for (j=5;--j>=0;) {
	xpos[j]=(int)(76.*ranf())+2;
	ypos[j]=(int)(20.*ranf())+2;
    }
    for (j=0;;) {
	x=(int)(76.*ranf())+2;
	y=(int)(20.*ranf())+2;

	cursor(x,y); addch('.');

	cursor(xpos[j],ypos[j]); addch('o');

	if (j==0) j=4; else --j;
	cursor(xpos[j],ypos[j]); addch('O');

	if (j==0) j=4; else --j;
	cursor(xpos[j],ypos[j]-1);
	addch('-');
	cursor(xpos[j]-1,ypos[j]);
	addstr("|.|");
	cursor(xpos[j],ypos[j]+1);
	addch('-');

	if (j==0) j=4; else --j;
	cursor(xpos[j],ypos[j]-2);
	addch('-');
	cursor(xpos[j]-1,ypos[j]-1);
	addstr("/ \\");
	cursor(xpos[j]-2,ypos[j]);
	addstr("| O |");
	cursor(xpos[j]-1,ypos[j]+1);
	addstr("\\ /");
	cursor(xpos[j],ypos[j]+2);
	addch('-');

	if (j==0) j=4; else --j;
	cursor(xpos[j],ypos[j]-2);
	addch(' ');
	cursor(xpos[j]-1,ypos[j]-1);
	addstr("   ");
	cursor(xpos[j]-2,ypos[j]);
	addstr("     ");
	cursor(xpos[j]-1,ypos[j]+1);
	addstr("   ");
	cursor(xpos[j],ypos[j]+2);
	addch(' ');
	xpos[j]=x; ypos[j]=y;
	refresh();
    }
}

onsig(n)
int n;
{
    endwin();
    exit(0);
}

float
ranf()
{
    float rv;
    long r = rand();

    r &= 077777;
    rv =((float)r/32767.);
    return rv;
}
