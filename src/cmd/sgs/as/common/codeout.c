#ident	"@(#)codeout.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

	

#include "gendefs.h"
#include "systems.h"
#include "temppack.h"
#include "section.h"

extern short Kflag;		/* to signify printing of line */

extern FILE	*fdsect;	/* file written to by codgen */

extern symbol *dot,
	symtab[];	/* internal symbol table */

extern struct scninfo secdat[];		/* section info table */

long	newdot;		/* up-to-date value of dot */

FILE	*fdcode;	/* temp file containing intermediate language */

#if DCODGEN /* direct code generation of arbitrary number of bytes */

codgen(nbits, val)
	register short nbits;
	long val;
{
	if (nbits) {
		if	(Kflag){
			if (dot->styp == TXT)
				spitcode(newdot,nbits,val,0);
				};
		if (nbits % BITSPBY)
			aerror("illegal code generation");
		newdot += nbits / BITSPBY;
		val <<= (sizeof(long)-sizeof(int))*BITSPBY;
		fwrite((char *)&val,(nbits/BITSPBY),1,fdsect);
		};
}

#else

static short
	poscnt = 0,	/* current bit position in outword buffer */
			/* spans from [0 - BITSPOW]	*/
	bitpos = 0;	/* bit position within a byte in outword buffer	*/
			/* spans from [0 - BITSPBY]	*/

static OUTWTYPE	outword;	/* output buffer - see gendefs.h */
static int mask[] = { 0, 1, 3, 7,
		017, 037, 077,
		0177, 0377, 0777,
		01777, 03777, 07777,
		017777, 037777, 077777,
		0177777 };
codgen(nbits, val)
	register short nbits;
	long val;
{
	register short size;
	register long value;
	register spitnum = 1;

start:	if (nbits)
	{
		if	(Kflag && spitnum){
			spitnum = 0;
			if (dot->styp == TXT)
				spitcode(newdot,nbits,val,0);
				};
		value = val;
		if ((size = BITSPOW - poscnt) > nbits)
			size = nbits;
		value >>= nbits - size;
		value &= mask[size];
		value <<= BITSPOW - poscnt - size;
		outword |= (OUTWTYPE)value;
		poscnt += size;
		bitpos += size;
		newdot += bitpos / BITSPBY;
		bitpos %= BITSPBY;
		if (poscnt == BITSPOW)
		{
			poscnt = 0;
			OUT(outword,fdsect);
			outword = 0;
			if (nbits > size)
			{
				nbits -= size;
				goto start;
			}
		}
	}
}
#endif	/* DCODGEN */

extern int (*(modes[]))();	/* array of action routine functions */

extern upsymins *lookup();

extern short sttop;

codout(file,start,sect)
char *file;
long start;
int sect;
{
	codebuf code;
	register symbol *sym;
	register long	pckword;
	long 		pckbuf;

	dot = (*lookup(".",INSTALL,USRNAME)).stp;
	dot->value = newdot = start;
	dot->styp = secdat[sect].s_typ;
	if ((fdcode = fopen(file, "r")) == NULL)
		aerror("Unable to open temporary file");

#ifndef DCODGEN
	poscnt = 0;
#endif
	while (1) {
		/* get a codebuf structure from the packed temp file */
		if (fread(&pckbuf, sizeof(long), 1, fdcode) != 1) break;
		pckword = pckbuf;
		code.caction = ACTNUM(pckword);
		code.cnbits = NUMBITS(pckword);
		if (pckword & SYMINDEX)
			code.cindex =