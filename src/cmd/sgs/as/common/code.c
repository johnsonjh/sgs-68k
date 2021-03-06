#ident	"@(#)code.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/


/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include "gendefs.h"
#include "scnhdr.h"
#include "systems.h"
#include "section.h"
#include "temppack.h"

int previous = 0;
int counter = -1;
int seccnt = 0;
short bitpos = 0;
extern symbol symtab[];
extern short Oflag;	/* debugging flag */
extern long newdot;	/* up-to-date value of "." */

extern symbol	*dot;
extern unsigned short line;
extern char *filenames[];
extern upsymins *lookup();

struct scnhdr sectab[NSECTIONS+1];	/* section header table */
struct scninfo secdat[NSECTIONS+1];	/* section info table */


/*
 *	generate creates an intermediate file entry from the info
 *	supplied thru its args. The intermediate file format consists
 *	of fixed sized records of the codebuf structure (see codeout.h).
 *	These entries represent the encoded user program and
 *	will be used as input to the second pass of the assembler.
 *	Different files, with their associated buffers, are maintained
 *	for each section.
 */
generate (nbits,action,value,sym)
BYTE nbits;
unsigned short action;
long value;
symbol *sym;
{
	register long	pckword;
	register struct scninfo *seci;
	unsigned short locindex;
	static int nosecerr = 1;

	locindex = sym==NULL ? 0 : 1+(sym-symtab);
#if DEBUG
	if (Oflag) {
		if (bitpos == 0)
			printf("(%d:%6lx)	",dot->styp,newdot);
		else
			printf("		");
		printf("%hd	%d	%hd	%.13lx	%hd\n",
			line,(short)nbits,action,value,locindex);
	}
#endif
	
	if ((sectab[dot->sectnum].s_flags & STYP_BSS) && nbits != 0)
	{
		yyerror("Attempt to initialize bss");
		return;
	}
	seci = &secdat[dot->sectnum];

	/* the following "if" statment */
	/* was introduced for software workarounds. */
	/* these functions use it to count */
	/* the number bits which are generated. */
	/* in the normal case "counter == -1" end does not */
	/* affect the code generation */ 
	if ((dot->styp & TXT) && (counter >=0))
	{
	   counter += nbits;
	   return;
	}
	/* Check if any code was generated since last NEWSTMT.
	   If not then no need to output this record	*/
		seci->s_any += nbits; /* indicate code generated */
#ifdef ickypoo
	if (action != NEWSTMT)
		seci->s_any += nbits; /* indicate code generated */
	else {	
		if (seci->s_any)
			seci->s_any = 0;  /* reset flag */
		else	return;
	}
#endif
	if ((seccnt <= 0) && nbits) {
		if (nosecerr) {
			nosecerr = 0;
			yyerror("Code generated before naming section");
			}
		return;
		}
	/* put a codebuf structure out to the packed temp file */

	if (seci->s_cnt == TBUFSIZ) {
		fwrite(seci->s_buf, sizeof(long), TBUFSIZ, seci->s_fd);
		seci->s_cnt=0;
	}
	pckword = MKACTNUM(action) | MKNUMBITS(nbits);
	if ((locindex) != 0) {
		pckword |= SYMINDEX | (locindex & LWRBITS);
		if ((value) == 0) {
			seci->s_buf[seci->s_cnt++] = pckword | VAL0;
		} else {
			seci->s_buf[seci->s_cnt++] = pckword;
			if (seci->s_cnt == TBUFSIZ) {
				fwrite(seci->s_buf, sizeof(long), TBUFSIZ, seci->s_fd);
				seci->s_cnt=0;
			}
			seci->s_buf[seci->s_cnt++] = value;
		}
	} else if (((value) & UPRBITS) == 0) {
		seci->s_buf[seci->s_cnt++] = VAL16 | SYMORVAL(value) | pckword;
	} else {
		seci->s_buf[seci->s_cnt++] = pckword;
		if (seci->s_cnt == TBUFSIZ) {
			fwrite(seci->s_buf, sizeof(long), TBUFSIZ, seci->s_fd);
			seci->s_cnt=0;
		}
		seci->s_buf[seci->s_cnt++] = value;
	}
	bitpos += nbits;
	newdot += bitpos/BITSPBY;
	bitpos %= BITSPBY;
}
    

flushbuf()
{
	register int i;
	register struct scninfo *p;

	for(i = 1, p= &secdat[1]; i <= seccnt; i++, p++)
	{
		fwrite((char *)p->s_buf, sizeof(long), p->s_cnt, p->s_fd);
		p->s_cnt = 0;
		fflush(p->s_fd);
		if (ferror(p->s_fd))
			aerror("trouble writing; probably out of temp-file space");
		fclose(p->s_fd);
	}
}

/*
 *	cgsect changes the section into which the assembler is 
 *	generating info
 */
cgsect(newsec, secnum)
	register int newsec;
{
	/*
	 * save current section "."
	 */

	sectab[dot->sectnum].s_size = newdot;

	/*
	 * save current section number
	 */

	previous = dot->sectnum;

	/*
	 * change to new section
	 */

	dot->sectnum = newsec;
	dot->styp = secdat[newsec].s_typ;
	dot->value = newdot = sectab[newsec].s_size;
}

/*
 * search for (and possibly define) a section by name
 */

short
mksect(sym, att)
register symbol	*sym;
register int	att;
{
	register struct scnhdr	*sect;
	register struct scninfo	*seci;
	FILE			*fd;

	if (!(sym->styp & SECTION))	/* is symbol a previously
					defined section? */
	{
		if (!att)
			yyerror("Section definition must have attributes");
		else if ((sym->styp & TYPE) != UNDEF)
			yyerror("Section name already defined");
		else if (seccnt >= NSECTIONS)
			yyerror("Too many sections defined");
		else if (strlen(sym->_name.name) > 8)
			yyerror("Section name too long");
		else
		{
			seccnt++;
			if ((fd = fopen(filenames[seccnt+5], "w")) == NULL)
				aerror("Cannot create (section) temp file");
			seci = &secdat[seccnt];
			sect = &sectab[seccnt];
			seci->s_fd = fd;
			if (att & STYP_TEXT) seci->s_typ = TXT;
			else if (att & STYP_DATA) seci->s_typ = DAT;
			else if (att & STYP_BSS) seci->s_typ = BSS;
			else seci->s_typ = ABS;
			sect->s_flags = att;
			strncpy(sect->s_name, sym->_name.name, 8);
			sym->styp = SECTION;
			sym->sectnum = seccnt;
			sym->value = att;
		}
	}
	else if (att && (sym->value != att))
		yyerror("Section attributes do not match");
	return(sym->sectnum);
}

void
comment(string)
char *string;
{
	int i, j, k, prevsec, size;
	union {
		long lng;
		char chr[sizeof(long)/sizeof(char)];
		} value;
	static int comsec = -1;
	if (comsec < 0) comsec = mksect(
		lookup(".comment", INSTALL, USRNAME)->stp, STYP_INFO);
	prevsec = previous;
	cgsect(comsec,0);
	size = strlen(string) + 1;
	for (i=0; i < size/4; i++) {
		for (j = 0; j < sizeof(long)/sizeof(char); j++)
			value.chr[j] = *(string + j + i * 4);
		generate(BITSPBY*4, NOACTION, value.lng, NULLSYM);
		}
	if (size % 4) {
		for (k = 0,j = sizeof(long)/sizeof(char) - size % 4; j < sizeof(long)/sizeof(char); j++,k++)
			value.chr[j] = *(string + k + i * 4);
		generate((size % 4) * BITSPBY, NOACTION, value.lng, NULLSYM);
		}
	cgsect(previous,0);
	previous = prevsec;
}
