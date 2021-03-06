/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) open.c: version 25.1 created on 12/2/91 at 20:12:43	*/
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

/*	ATT/t450:#ident	"libplot:t450/open.c	1.2"	*/
	
#ident	"@(#)libplot/t450:open.c	25.1"

#include <termio.h>
#include <signal.h>
/* gsi plotting output routines */
# define DOWN 012
# define UP 013
# define LEFT 010
# define RIGHT 040
# define BEL 007
# define ACK 006
#define PLOTIN 063
#define PLOTOUT 064
# define CR 015
# define FF 014
# define VERTRESP 48
# define HORZRESP 60.
# define HORZRES 6.
# define VERTRES 8.
/* vert and horiz spacing */
#define ESC 033
#define RS 036
#define US 037
#define HT 011
/* down is line feed, up is reverse line feed,
   left is backspace, right is space.  48 points per inch
   vertically, 60 horizontally */

int xnow, ynow;
struct termio ITTY,PTTY;
int OUTF;
float HEIGHT = 6.0, WIDTH = 6.0, OFFSET = 0.0;
int xscale, xoffset, yscale;
float botx = 0., boty = 0., obotx = 0., oboty = 0.;
float scalex = 1., scaley = 1.;

openpl ()
{
	int reset();
		xnow = ynow = 0;
		OUTF = 1;
		printf("\r");
		ioctl(OUTF, TCGETA, &ITTY);
		signal (SIGINT, reset);
		PTTY = ITTY;
		PTTY.c_oflag &= ~(ONLCR|OCRNL|ONOCR|ONLRET);
		PTTY.c_cflag |= CSTOPB;
		ioctl(OUTF, TCSETAW,&PTTY);
		/* set vert and horiz spacing to 6 and 10 */
		putchar(ESC);	/* set vert spacing to 6 lpi */
		putchar(RS);
		putchar(HT);
		putchar(ESC);	/* set horiz spacing to 10 cpi */
		putchar(US);
		putchar(CR);
		/* initialize constants */
		xscale  = 4096./(HORZRESP * WIDTH);
		yscale = 4096 /(VERTRESP * HEIGHT);
		xoffset = OFFSET * HORZRESP;
		return;
}

openvt(){
openpl();
}
