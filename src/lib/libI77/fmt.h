/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) fmt.h: version 25.1 created on 12/2/91 at 18:42:30	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)fmt.h	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libI77:fmt.h	1.4"	*/
	
#ident	"@(#)libI77:fmt.h	25.1"

/*	3.0 SID #	1.2	*/
struct syl
{	int op,p1,p2,p3;
};
#define RET 1
#define REVERT 2
#define GOTO 3
#define X 4
#define SLASH 5
#define STACK 6
#define I 7
#define ED 8
#define NED 9
#define IM 10
#define APOS 11
#define H 12
#define TL 13
#define TR 14
#define T 15
#define COLON 16
#define S 17
#define SP 18
#define SS 19
#define P 20
#define BN 21
#define BZ 22
#define F 23
#define E 24
#define EE 25
#define D 26
#define G 27
#define GE 28
#define L 29
#define A 30
#define AW 31
#define O 32
#define NONL 33
extern struct syl syl[];
extern int pc,parenlvl,revloc;
extern int (*doed)(),(*doned)();
extern int (*dorevert)(),(*donewrec)(),(*doend)();
extern flag cblank,cplus,workdone, F77nonl;
extern int dummy();
extern char *fmtbuf;
extern int scale;
typedef union
{	float pf;
	double pd;
} ufloat;
typedef union
{	short is;
	char ic;
	long il;
} xint;
#define GET(x) if((x=(*getn)())<0) return(x)
#define VAL(x) (x!='\n'?x:' ')
#define PUT(x) (*putn)(x)
extern int cursor;
