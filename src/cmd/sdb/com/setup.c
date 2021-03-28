/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) setup.c: version 23.1 created on 11/15/90 at 13:19:13	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)setup.c	23.1	11/15/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/com:setup.c	23.1"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
	/*	OLD MOT:setup.c	6.2		*/

/*
 *	UNIX debugger
 *
 * 		Everything here is MACHINE DEPENDENT
 *			and operating system dependent
 */

#include "head.h"
#include "coff.h"

#define SYSTEM "/arix"

extern MSG		BADMAGN;

INT	wtflag = 0;	/* set to allow writing to a.out, core, ISP */

static L_INT		txtsiz;
static L_INT		datsiz;
static L_INT		datbas;
static L_INT		stksiz;
static L_INT		symbas;		/*  not using ?? */
       L_INT		entrypt;

extern INT		argcount;
extern INT		signo;
extern INT		magic;
extern int		errno;

extern STRING		symfil;
extern STRING		corfil;

#define AOUTHDRSIZ	(filhdr.f_opthdr)

#define	maxfile		(1L<<24)

FILHDR	filhdr;		/* a.out file header */
AOUTHDR	aouthdr;	/* a.out Unix (optional) header */
SCNHDR	*scnhdrp;	/* pointer to first section header (space by sbrk) */

setsym()
{
#if DEBUG > 1
	if (debugflag)
		printf("setsym()\n");
#endif
	fsym = getfile(symfil,1);
	txtmap.ufd = fsym;
	errno = 0;
	if (read(fsym,&filhdr,FILHSZ) == FILHSZ	&&
	   ISMAGIC(filhdr.f_magic)		&&
	   AOUTHDRSIZ == sizeof aouthdr		&&
	   read(fsym, &aouthdr, AOUTHDRSIZ)==AOUTHDRSIZ)
	{
		magic=aouthdr.magic;
		if (magic==OMAGIC   || 	/* Writable text: private */
		    magic==PMAGIC   ||	/* Paging text: private.  */
		    magic==NMAGIC)	/* Readonly text: private */
		{
			txtsiz=aouthdr.tsize;
			datsiz=aouthdr.dsize;
			symbas=txtsiz+datsiz;
			rdschdrs();

			switch (magic) {
			/*  use to have many more "magic" cases here */
			/*	assuming text is first section */

			case OMAGIC:	/* 0407 */
				txtmap.b1=0;
				txtmap.e1=txtmap.b1 + symbas;
				txtmap.f1=scnhdrp[0].s_scnptr;
				txtmap.b2=datbas=scnhdrp[0].s_paddr;
				txtmap.e2=symbas;
				txtmap.f2=txtmap.f1;
				break;

			case PMAGIC:	/* 0413 */
			case NMAGIC:	/* 0410 */
				txtmap.b1=scnhdrp[0].s_vaddr;
				txtmap.e1=txtmap.b1 + txtsiz;
				txtmap.f1=scnhdrp[0].s_scnptr;
				txtmap.b2=datbas=scnhdrp[1].s_vaddr;
				txtmap.e2=datbas+datsiz;
				txtmap.f2=scnhdrp[1].s_scnptr;
				break;

			}
			entrypt = aouthdr.entry;
			ststart = filhdr.f_symptr;
		}
		else {
			magic = 0;
			fprintf(FPRT1, "Warn: No magic for %s;\n", symfil);
		}
	}
	else {		/*  may be a ".o" file */
		if (ISMAGIC(filhdr.f_magic))
		{
			magic = filhdr.f_magic;
			rdschdrs();
			/* assuming 3 sections; text, data, and bss */
			txtsiz = scnhdrp[0].s_size;
			datsiz = scnhdrp[1].s_size;
			symbas = txtsiz+datsiz;
			txtmap.b1 = 0;
			txtmap.e1 = txtsiz;
			txtmap.f1 = scnhdrp[0].s_scnptr;
			txtmap.b2 = datbas = scnhdrp[0].s_paddr;
			txtmap.e2 = txtsiz+datsiz;
			txtmap.f2 = scnhdrp[1].s_scnptr;
			entrypt = 0;
			ststart = filhdr.f_symptr;
		}
	}
#if DEBUG > 1
	if(debugflag)
		fprintf(FPRT2, "magic=%#o;\n", magic);
#endif
	if (magic == 0)
	{
		txtmap.e1 = maxfile;
	}
}

setcor()
{
	int i;

	datmap.ufd = fcor = getfile(corfil,2);
	if(fcor < 0)
	{
		return;
	}
	   /*  sure a core file */
	if (read(fcor, &uu, XTOB(USIZE))==XTOB(USIZE) && 
	    magic == (uu.u_exdata.ux_mag))
	{

		uu.u_ar0 = (int *)((uint)uu.u_ar0 + (uint)&uu); 

		signo = uu.u_arg[0]&037;

		txtsiz = XTOB(uu.u_tsize);
		datsiz = XTOB(uu.u_dsize);
		stksiz = XTOB(uu.u_ssize);
#if DEBUG > 1
		if(debugflag > 1) {
			fprintf(FPRT2,
			    "uu.u_tsize=%#x; uu.u_dsize=%#x; uu.u_ssize=%#x;\n",
				uu.u_tsize, uu.u_dsize, uu.u_ssize);
			fprintf(FPRT2, "txtsiz=%#x; datsiz=%#x; stksiz=%#x;\n",
				txtsiz, datsiz, stksiz);
		}
#endif
		if( magic == NMAGIC || magic == PMAGIC )
		{
			datmap.b1=datbas=scnhdrp[1].s_paddr; /* hh1 0 <- 1 */
		}
		else
		{
			datmap.b1=datbas=0;
		}
		datmap.e1=(magic==OMAGIC?txtsiz:datmap.b1)+datsiz;
		datmap.f1 = XTOB(USIZE);
		/* MC680X0 stack also grows down from USRSTACK */
		/* but stksiz doesn't include sizeof user area  hh1 */
		datmap.b2 = USRSTACK - stksiz;
		datmap.e2 = USRSTACK; /* vax || 680X0 */
		if( magic == NMAGIC || magic == PMAGIC )
		{
			datmap.f2 = XTOB( USIZE ) + datsiz;
		}
		else
		{
			datmap.f2 = XTOB( USIZE ) + datmap.e1;
		}
			/* hh5 (or so) copy register values from uu into */
			/* regs[] so that the new SDBREG works correctly */
			/* SDBREG was modified to use regs for S90/a1000 */
		for (i=0; i <= NUMREGS; i++)
			regs[i]	= (ADDR)(uu.u_ar0[i]);

#if DEBUG > 1
		if(debugflag > 0)
			fprintf(FPRT2, "uu.u_arg[0]=%#x; signo=%#x;\n",
					uu.u_arg[0], signo);
#endif
	}
	else		/*  e.g. /dev/kmem or any ordinary file */
	{
		datmap.e1 = datmap.e2 = maxfile;
		datmap.b1 = datmap.f1 = 0;
		datmap.b2 = datmap.f2 = 0;
		fprintf(FPRT1, "Warning: `%s' not a core file.\n", corfil);
	        fprintf(FPRT1, "       : magic(0%o), found(0%o)\n",
	      		magic, uu.u_exdata.ux_mag);
		fakecor = 1;
	}
}


create(f)
STRING	f;
{	int fd;
	if ((fd=creat(f,0644)) >= 0) {
		close(fd);
		return(open(f,wtflag));
	}
	else return(-1);
}

getfile(filnam,cnt)
STRING	filnam;
{
	REG INT		fd;

	if (!eqstr("-",filnam))
	{
		fd=open(filnam,wtflag);
		if (fd<0 && argcount>cnt)
		{
			if (fd < 0)
			{
				fprintf(FPRT1, "cannot open `%s'\n", filnam);
			}
		}
	}
	else
	{
		fd = -1;
	}
	return(fd);
}

/* rdschdrs(): read section headers */
rdschdrs()
{
	register unsigned nb;
	extern FILHDR filhdr;		/* a.out file header */

	nb = filhdr.f_nscns * SCNHSZ;
	scnhdrp = (SCNHDR *) sbrk(nb);
	if (read(fsym, scnhdrp, nb) != nb)
		fprintf(FPRT1, "Warn: section header read error\n");
	/* chkerr();	 does longjmp and haven't done setjmp yet */
	return(filhdr.f_nscns);
}
