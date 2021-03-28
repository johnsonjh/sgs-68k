#ident	"@(#)addr2.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
	1.1	89/06/08 00:40:17 root
    date and time created 89/06/08 00:40:17 by root

*/
/*	OLD MOT:addr2.c	7.1			*/
/*
static	char	sccsid[] = "OLD MOT addr2.c 7.1 5/30/85";
/* static	char	sccsid[] = "OLD MOT addr2.c: 2.8 6/28/83"; */



#include	"gendefs.h"
#include	"reloc.h"
#include	"temppack.h"
#include	"scnhdr.h" /* hanna */


extern unsigned short
		relent;	/* Number of reloc entries written to fdrel.	*/

extern long	newdot;
extern symbol *dot;

extern symbol	symtab[];

extern FILE	*fdrel,	/* File of relocation table information.	*/
		*fdcode;/* File of intermediate object code.		*/

extern BYTE	*longsdi;


extern int picky();
extern	setfile(),
	lineno(),
	linenum(),
	lineval(),
	define(),
	endef(),
	setval(),
	settyp(),
	settypq(),
	setlno(),
	setscl(),
	settag(),
	setsiz(),
	setdim1(),
	setdim2(),
	newstmt(),
	genreloc(),
	brloc(),
	braopt(),
	bccopt(),
	bsropt(),
	absopt(),
	swbeg(),
	move(),
	iopt(),
	insti(),
	absbr(),
	ndxreloc(),
	iabsbr(),
	brbyt();
#ifdef M68881
	extern cpbccopt();
#endif


int	(*(modes[NACTION+3]))() =
	{
	0,			/*  0 */
	setfile,		/*  1 */
	lineno,			/*  2 */
	linenum,		/*  3 */
	lineval,		/*  4 */
	define,			/*  5 */
	endef,			/*  6 */
	setval,			/*  7 */
	settyp,			/*  8 */
	setlno,			/*  9 */
	setscl,			/* 10 */
	settag,			/* 11 */
	setsiz,			/* 12 */
	setdim1,		/* 13 */
	setdim2,		/* 14 */
	newstmt,		/* 15 */
	genreloc,		/* 16 */
	brloc,			/* 17 */
	braopt,			/* 18 */
	bccopt,			/* 19 */
	bsropt,			/* 20 */
	absopt,			/* 21 */
	swbeg,			/* 22 */
	move,			/* 23 */
	iopt,			/* 24 */
	insti,			/* 25 */
	absbr,			/* 26 */
	ndxreloc,		/* 27 */
	iabsbr,			/* 28 */
	brbyt,			/* 29 */
#ifdef M68881
	cpbccopt,
#endif
	settypq,		/* 31 */
	0
	};


genreloc(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	char		*rsym;
	prelent		trelent;
	short		stype;
	long		testval;

	if (sym != NULLSYM)
	{
		code->cvalue += (sym->value &
			(code->cnbits == BITSPBY?0xff:
			(code->cnbits == BITSPOW?0xffff:0xffffffff)));
		if(sym->sectnum > 6)		/* hanna/bk */
			stype=sym->sectnum;		/* hanna */
		else
			stype = sym->styp & TYPE;		/* hanna */
		testval = code->cvalue;
		if ((code->cnbits == BITSPBY && (testval < -128 || testval > 255)) ||
		    (code->cnbits == BITSPOW && (testval < -32768L || testval > 65535L)))
			werror("size of code generated may not hold operand");
	}
	else
		return;

	if(stype > 4 ){		/* hanna */
		sym->styp |= EXTERN;	/* Make sure it gets into the	*/
		rsym = sym->_name.name;	/*	symbol table.		*/
	}else		/* hanna */
	switch (stype)
	{
	case ABS:
		return;
	case TXT:
		rsym = ".text";
		break;
	case DAT:
		rsym = ".data";
		break;
	case BSS:
		rsym = ".bss";
		break;
	case UNDEF:
		sym->styp |= EXTERN;	/* Make sure it gets into the	*/
		rsym = sym->_name.name;	/*	symbol table.		*/
		break;
	default:
		aerror("attempt to relocate weird type");
	}

	trelent.relval = newdot;
	trelent.relname = rsym;
	if (code->cnbits == 8)
		trelent.reltype = R_RELBYTE;
	else if (code->cnbits == 16)
		trelent.reltype = R_RELWORD;
	else
		trelent.reltype = R_RELLONG;
	fwrite((char *)(&trelent),sizeof(prelent),1,fdrel);
	++relent;
}	/* genreloc()	*/



brloc(sym,code)
	register symbol	*sym;
	register codebuf	*code;
{
	prelent		trelent;
	register short	stype;
	register long	offset;

	if ( sym == NULLSYM )
		return;
	offset = sym->value - newdot;
	if (code->cnbits == BITSPBY)
		offset -= 1;
	code->cvalue += offset;
	stype = sym->styp & TYPE;

	if (stype == UNDEF)
	{
		sym->styp |= EXTERN;	/* Make sure it gets into the	*/
		trelent.relval = newdot;/*	symbol table.		*/
		trelent.relname = sym->_name.name;
		if (code->cnbits == 8)
			trelent.reltype = R_PCRBYTE;
		else
		if (code->cnbits == 16)
			trelent.reltype = R_PCRWORD;
		else
			trelent.reltype = R_PCRLONG;
		fwrite((char *)(&trelent),sizeof(prelent),1,fdrel);
		++relent;
	}
	else				/* check for valid offset */
	{
	    if (offset < 0)
		offset = ~offset;	/* force sign bits to 0 */
	    if (   (code->cnbits == 8  && (offset & ~0x7fL)   != 0)
		|| (code->cnbits == 16 && (offset & ~0x7fffL) != 0)
		)
		aerror("branch offset is too remote");
	}
}	/* brloc()	*/



braopt(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	if (*++longsdi)
	{	/* Original bra.w is no good, and the second word	*/
		/* is only good for the symbol it contains.  We need	*/
		/* a jmp abs.l to the symbol in the second code chunk.	*/
/*#if !M68020
/*		codgen(BITSPOW,0x4ef9);			/*	jmp	*/
/*		if (fread(code,sizeof(*code),1,fdcode) != 1)
/*			aerror("unexpected EOF on temporary (code) file");
/*		sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
/*		code->cnbits = 2*BITSPOW;
/*		code->cvalue = 0;
/*		genreloc(sym,code);
/*#else
 */
		/* 68020 allows bra.l */
		codgen(BITSPOW,0x60ff);			/*	bra	*/
		/*if (fread(code,sizeof(*code),1,fdcode) != 1)*/
		if (picky(code)!= 1)
			aerror("unexpected EOF on temporary (code) file");
		sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
		code->cnbits = 2*BITSPOW;
		code->cvalue = 0;
		brloc(sym,code);
/*#endif M68020*/
		return;
	}
	/* Otherwise, original bra.w is ok, so codgen gets code as-is.	*/
}	/* braopt()	*/



bccopt(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	if (*++longsdi)
	{	/* Original bcc.w is no good, and the second word	*/
		/* is only good for the symbol it contains.  Negate	*/
		/* the direction of the branch, then use this to branch	*/
		/* around a jmp.  Direction is reversed by toggling	*/
		/* eighth bit.						*/
/*#if !M68020
/*		code->cvalue ^= 0x100;
/*
/*		/* Add a displacement which will get past the jmp.	*/
/*		code->cvalue |= 6;
/*		codgen(code->cnbits,code->cvalue);
/*		codgen(BITSPOW,0x4ef9);			/*	jmp	*/
/*		if (fread(code,sizeof(*code),1,fdcode) != 1)
/*			aerror("unexpected EOF on temporary (code) file");
/*		sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
/*		code->cnbits = 2*BITSPOW;
/*		code->cvalue = 0;
/*		genreloc(sym,code);	
/*#else
 */
		/* 68020 allows bcc.l */
		codgen(BITSPOW,code->cvalue | 0xff);	/*	bcc	*/
		/*
		if (fread(code,sizeof(*code),1,fdcode) != 1)
		*/
		if (picky(code) != 1)
			aerror("unexpected EOF on temporary (code) file");
		sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
		code->cnbits = 2*BITSPOW;
		code->cvalue = 0;
		brloc(sym,code);	
/*#endif M68020 */
		return;
	}
	/* Otherwise, original bcc.w is ok, so codgen gets code as-is.	*/
}	/* bccopt()	*/



bsropt(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	if (*++longsdi)
	{	/* Original bsr.w is no good, and the second word	*/
		/* is only good for the symbol it contains.  We need	*/
		/* a jsr abs.l to the symbol in the second code chunk.	*/
/*#if !M68020
/*		codgen(BITSPOW,0x4eb9);			/*	jsr	*/
/*		if (fread(code,sizeof(*code),1,fdcode) != 1)
/*			aerror("unexpected EOF on temporary (code) file");
/*		sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
/*		code->cnbits = 2*BITSPOW;
/*		code->cvalue = 0;
/*		genreloc(sym,code);
/*#else */
		/* 68020 allows bsr.l */
		codgen(BITSPOW,0x61ff);			/*	bsr	*/
		/*
		if (fread(code,sizeof(*code),1,fdcode) != 1)
		*/
		if (picky(code) != 1)
			aerror("unexpected EOF on temporary (code) file");
		sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
		code->cnbits = 2*BITSPOW;
		code->cvalue = 0;
		brloc(sym,code);
/*#endif M68020*/
		return;
	}
	/* Otherwise, original bsr.w is ok, so codgen gets code as-is.	*/
}	/* bsropt()	*/



absopt(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	if (*++longsdi)
	{	/* original abs.w is no good.  The mode field needs to	*/
		/* be changed to abs.l, and the next word has to be	*/
		/* expanded to two words.				*/
		code->cvalue |= 0x1;
	}
	codgen(code->cnbits,code->cvalue);
	/*
	if (fread(code,sizeof(*code),1,fdcode) != 1)
	*/
	if (picky(code) != 1)
		aerror("unexpected EOF on temporary (code) file");
	if (*longsdi)
		code->cnbits = 2*BITSPOW;
	sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
	genreloc(sym,code);
}	/* absopt()	*/



swbeg(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	int		swbegct;
	int		i;
	long		swtabbase;
	codgen(code->cnbits,code->cvalue);
	/*
	if (fread(code,sizeof(*code),1,fdcode) != 1)
	*/
	if (picky(code) != 1)
		aerror("unexpected EOF on temporary (code) file");
	swbegct = code->cvalue;
	codgen(code->cnbits,code->cvalue);
	/* Swbeg line ends in newline, followed by a line with just	*/
	/* the switch table beginning, then a newline, then "arg"	*/
	/* switch table elements, each accompanied by a newline.	*/
	/*
	if (fread(code,sizeof(*code),1,fdcode) != 1)
	*/
	if (picky(code) != 1)
		aerror("unexpected EOF on temporary (code) file");
	sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
	newstmt(sym,code);
	/*
	if (fread(code,sizeof(*code),1,fdcode) != 1)
	*/
	/* New generate does not generate new statement for label? */
	if (picky(code) != 1)
		aerror("unexpected EOF on temporary (code) file");
	sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
	newstmt(sym,code);
	
	swtabbase = newdot;	/* The value of the location counter	*/
				/*	for the right hand side of the	*/
				/*	label subtraction expression.	*/

	for (i = 1; i < swbegct; i++)
	{
		/*
		if (fread(code,sizeof(*code),1,fdcode) != 1)
		*/
		if (picky(code) != 1)
			aerror("unexpected EOF on temporary (code) file");
		sym = symtab + code->cindex - 1;
		codgen(code->cnbits,sym->value - swtabbase);
		/*
		if (fread(code,sizeof(*code),1,fdcode) != 1)
		*/
		if (picky(code) != 1)
			aerror("unexpected EOF on temporary (code) file");
		sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
		newstmt(sym,code);
	}

	/* We've completed all but one of the elements of the switch	*/
	/* table.  Codout (the routine which invoked this routine)	*/
	/* will call codgen with what we leave in code.  Therefore, the	*/
	/* last element of the switch table is left in code for codout.	*/

	/*
	if (fread(code,sizeof(*code),1,fdcode) != 1)
	*/
	if (picky(code) != 1)
		aerror("unexpected EOF on temporary (code) file");
	sym = symtab + code->cindex - 1;
	code->cvalue = sym->value - swtabbase;
}	/* swbeg()	*/



move(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	int		mode1;
	int		mode2;

	codgen(code->cnbits,code->cvalue);
	mode1 = code->cvalue & 070;
	mode2 = code->cvalue & 0700;
	if (mode1 > 040)	/* Source has more operand data.	*/
	{
		/*
		if (fread(code,sizeof(*code),1,fdcode) != 1)
		*/
		if (picky(code) != 1)
			aerror("unexpected EOF on temporary (code) file");
		if (code->caction == GENRELOC)
		{
			sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
			genreloc(sym,code);
		}
		codgen(code->cnbits,code->cvalue);
	}
	if (mode2 > 0400)	/* Destination has more operand data.	*/
	{
		/*
		if (fread(code,sizeof(*code),1,fdcode) != 1)
		*/
		if (picky(code) != 1)
			aerror("unexpected EOF on temporary (code) file");
		if (code->caction == GENRELOC)
		{
			sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
			genreloc(sym,code);
		}
		codgen(code->cnbits,code->cvalue);
	}
	code->cnbits = 0;
}	/* move()	*/



iopt(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	if (*++longsdi)
		code->cvalue |= 0x1;
	codgen(code->cnbits,code->cvalue);
	/*
	if (fread(code,sizeof(*code),1,fdcode) != 1)
	*/
	if (picky(code) != 1)
		aerror("unexpected EOF on temporary (code) file");
	if (code->caction == GENRELOC)
	{
		sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
		genreloc(sym,code);
	}
	codgen(code->cnbits,code->cvalue);
	/*
	if (fread(code,sizeof(*code),1,fdcode) != 1)
	*/
	if (picky(code) != 1)
		aerror("unexpected EOF on temporary (code) file");
	if (*longsdi)
		code->cnbits = 2*BITSPOW;
	sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
	genreloc(sym,code);
}	/* iopt()	*/



insti(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	int		mode;

	mode = code->cvalue & 070;
	codgen(code->cnbits,code->cvalue);
	/*
	if (fread(code,sizeof(*code),1,fdcode) != 1)
	*/
	if (picky(code) != 1)
		aerror("unexpected EOF on temporary (code) file");
	if (code->caction == GENRELOC)
	{
		sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
		genreloc(sym,code);
	}
	if (mode > 040)
	{
		codgen(code->cnbits,code->cvalue);
		/*
		if (fread(code,sizeof(*code),1,fdcode) != 1)
		*/
		if (picky(code) != 1)
			aerror("unexpected EOF on temporary (code) file");
		if (code->caction == GENRELOC)
		{
			sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
			genreloc(sym,code);
		}
	}
}	/* insti()	*/



/* 
 *	absbr(sym,code)
 *	This module is good to use iff the instruction has one
 *	instruction word before any extension words.	Nov,12,1984 SFL
 *
 */
absbr(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	if (*++longsdi)
	{	/* Original pc-relative instruction is no good, so we	*/
		/* convert it to the same instruction using the abs.l	*/
		/* mode.						*/
		code->cvalue ^= 03;
		codgen(code->cnbits,code->cvalue);
		/*
		if (fread(code,sizeof(*code),1,fdcode) != 1)
		*/
		if (picky(code) != 1)
			aerror("unexpected EOF on temporary (code) file");
		code->cnbits = 2*BITSPOW;
		sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
		genreloc(sym,code);
		return;
	}
	codgen(code->cnbits,code->cvalue);
	/*
	if (fread(code,sizeof(*code),1,fdcode) != 1)
	*/
	if (picky(code) != 1)
		aerror("unexpected EOF on temporary (code) file");
	if (code->cindex)
	{
		sym = symtab + code->cindex - 1;
		brloc(sym,code);
	}
}	/* absbr()	*/



/* 
 *	iabsbr(sym,code)
 *	
 *	This module is simular to absbr. However, it is used by
 *  instruction which may have 2 or more instruction words
 *	before extention words. Therefore, if original pc-relative
 *	instruction does't work, convert it to the same instruction
 *	using the abs.l mode and the next word after all the instruction
 *	words has to be expanded to two words. 	nov,12,1984 SFL
 *		
 */
iabsbr(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	if (*++longsdi)
	{	/* Original pc-relative instruction is no good, so we	*/
		/* convert it to the same instruction using the abs.l	*/
		/* mode.						*/
		code->cvalue ^= 03;
		codgen(code->cnbits,code->cvalue);
		/*
		if (fread(code,sizeof(*code),1,fdcode) != 1)
		*/
		if (picky(code) != 1)
			aerror("unexpected EOF on temporary (code) file");
		if (code->caction == GENRELOC)
		{
			sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
			genreloc(sym,code);
		}
		codgen(code->cnbits,code->cvalue);
		/*
		if (fread(code,sizeof(*code),1,fdcode) != 1)
		*/
		if (picky(code) != 1)
			aerror("unexpected EOF on temporary (code) file");
		code->cnbits = 2*BITSPOW;
		sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
		genreloc(sym,code);
		return;
	}
	codgen(code->cnbits,code->cvalue);
	/*
	if (fread(code,sizeof(*code),1,fdcode) != 1)
	*/
	if (picky(code) != 1)
		aerror("unexpected EOF on temporary (code) file");
	sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
	brloc(sym,code);
}	/* iabsbr()	*/



ndxreloc(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	prelent		trelent;

	if (sym == NULLSYM)
		return;
	codgen(BITSPBY,code->cvalue >> BITSPBY);
	code->cvalue &= 0xff;
	code->cnbits = BITSPBY;
	genreloc(sym,code);
}	/* ndxreloc()	*/

#ifdef	M68881
cpbccopt(sym,code)
	register symbol	*sym;
	codebuf		*code;
{
	if (*++longsdi)
	{	/* Original cpbcc.w is no good, then generate a .l */
		/* turn on bit 6 for L */
		codgen(BITSPOW,code->cvalue | 0x40);	/*	cpbcc L	*/
		/*
		if (fread(code,sizeof(*code),1,fdcode) != 1)
		*/
		if (picky(code) != 1)
			aerror("unexpected EOF on temporary (code) file");
		sym = code->cindex ? (symtab + code->cindex - 1) : NULLSYM;
		code->cnbits = 2*BITSPOW;
		code->cvalue = 0;
		brloc(sym,code);	
		return;
	}
	/* Otherwise, original bcc.w is ok, so codgen gets code as-is.	*/
}	/* cpbccopt()	*/

#endif	/*M68881*/
extern short Kflag;
brbyt(osym,ocode)
	register symbol	*osym;
	codebuf		*ocode;
{
	register symbol *sym;
	codebuf		code;
	register long	pckword;
	long pckbuf;
	/*
	if (fread(&pckbuf, sizeof(long), 1, fdcode) != 1)
	*/
	if (picky(&code) != 1)
		aerror("Byte Branch cannot read offset");
	sym = code.cindex ? (symtab + (code.cindex-1))
			: (symbol *) NULL;
	if (code.caction > NACTION || code.caction != BRLOC)
		aerror("Invalid action routine for byte offset");
	code.cvalue--;
	(*(modes[code.caction]))(sym,&code);
	if(code.cvalue){
		codgen((short)(ocode->cnbits),ocode->cvalue);
		ocode->cnbits=8;ocode->cvalue=code.cvalue;
	}else{
		codgen((short)8,0x4e);
		ocode->cnbits=8;ocode->cvalue=0x71;
	}
	if (Kflag){
		if((code.caction == NEWSTMT) && (dot->styp == TXT))
			spitline(dot->value,code.cvalue);
	}
}

int picky(code)
codebuf *code;
{
	register long	pckword;
	long 		pckbuf;
	if (fread(&pckbuf, sizeof(long), 1, fdcode) != 1) return 0;
	pckword = pckbuf;
	code->caction = ACTNUM(pckword);
	code->cnbits = NUMBITS(pckword);
	if (pckword & SYMINDEX)
		code->cindex = SYMORVAL(pckword);
	else
		code->cindex = 0;
	if (pckword & VAL0)
		code->cvalue = 0;
	else if (pckword & VAL16)
		code->cvalue = SYMORVAL(pckword);
	else if (fread(&(code->cvalue), sizeof(long), 1, fdcode) != 1){
		aerror("Bad temporary file format");
		return 0;
	}
	return 1;
}
