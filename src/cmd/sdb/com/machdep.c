/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) machdep.c: version 23.2 created on 5/8/91 at 10:37:06	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)machdep.c	23.2	5/8/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/com:machdep.c	23.2"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
	/*	OLD MOT:machdep.c	6.6		*/

/*
 ****		MACHINE and OPERATING SYSTEM DEPENDENT
 ****		Routines which deal with the run-time stack
 */

#include "head.h"
#include "coff.h"

#define SYSTEM "/arix"

#undef n_name

extern BKPTR	bkpthead;
extern MSG		NOPCS;

long rdsym();			/* in symt.c */
extern SYMENT syment;		/* rdsym() stores symbol table entry */
extern AUXENT auxent[];		/* rdsym() stores auxiliary entry */
extern int gflag;		/* initfp() in symt.c sets */
extern FILHDR filhdr;
static int frameflg;		/* set if now past last frame */

#define EVEN	(-2)

/* initialize frame pointers to top of call stack */
/*  MACHINE DEPENDENT */
struct proct *
initframe() {
	register struct proct *procp;
	ADDR stackp;	/* hh1 - need to see if frame pointer is valid */

#if DEBUG > 1
	if(debugflag)
		fprintf(FPRT2, "initfram();\n");
	if(debugflag > 1) {
		fprintf(FPRT2,	"   &SDBREG(AP)=%#x", &SDBREG(AP) );
		fprintf(FPRT2, "   &uu=%#x; AP=%#x;\n", &uu, AP);
	}
#endif
	if ((pid == 0) && (fcor < 0 || fakecor)) {
		frameflg = 1;
		return(badproc);
	}

	frame = argp = SDBREG(FP);
	callpc = SDBREG(PC);
	stackp = SDBREG(SP);

	/* FP in m68k is initialized by kernel to be 0L (it isn't really -hh)*/
	if (frame > USRSTACK || frame < stackp) { /* hh1 - don't check just */
		frameflg = 1;			   /* for zero - check valid */
		return(badproc);
	}

	procp = adrtoprocp(callpc);
#if DEBUG > 1
	if(debugflag > 1)
	    fprintf(FPRT2,
		"	procp->pname=%s; argp=%#x; frame=%#x; callpc=%#x;\n",
			procp->pname, argp, frame, callpc);
#endif
	frameflg = 0;
	return(procp);
}

struct proct *
nextframe() {
	register struct proct *procp;
	union word word;
	static ADDR o_frame;	/* hh1 - old value of frame */

#if DEBUG > 1
	if(debugflag)
		fprintf(FPRT2,
			"nextframe():   prev frame=%#x(%#o);\n", frame, frame);
#endif

	callpc = get(NEXTCALLPC, DSP);
	o_frame = frame;			/* hh1 */
	frame = get(NEXTFRAME, DSP) & EVEN;
	argp = frame;

#if DEBUG > 1
	if(debugflag)
		fprintf(FPRT2, "  :  callpc=%#x; argp=%#x; frame=%#x;\n",
				callpc, argp, frame);
#endif

	if( frame < o_frame || frame > USRSTACK) /* frame pointer not init'd */
	{					 /* by kernel - look for bad */
		frameflg = 1;			 /* value.	*hh1*       */
		return(badproc);
	}

	procp = adrtoprocp(callpc-1);

#if DEBUG > 1
	if(debugflag > 1) {
		fprintf(FPRT2, "	nxtframe:\n	");
		fprintf(FPRT2,
			"procp->pname=%s; argp=0%o; frame=%#x; callpc=%#x;\n",
					procp->pname, argp, frame, callpc);
	}
#endif
	return(procp);
}

/* returns core image address for variable */
/* formaddr() should be in symt.c ?? */
ADDR
/* needs type so stackreg can find float reg var properly*/
formaddr(class, addr, type)
short type;
register char class;
ADDR addr; {
#if DEBUG
	if (debug)
		fprintf(FPRT2, "formaddr(class=%d(%#o), addr=%#x(%d))\n",
					class,class, addr,addr);
#endif
	switch(class) {
	case C_REG:
	case C_REGPARM:
		{
			char *desc;
			desc = typetodesc(type);
			if ( (*desc=='g') || (*desc=='f') )
				return(stackreg(addr, 1));
			else
				return(stackreg(addr, 0));
		}
	case C_EXT:
	case C_STAT:
		return(addr);
	case C_MOS:
	case C_MOU:
	case C_MOE:
	case C_FIELD:			/* ?? */
		return(addr);
		
	case C_ARG:
		return(argp+addr);
		
	case C_AUTO:
		return(frame+addr);	/* addr was negated in outvar() */

	default:
		fprintf(FPRT1,"Bad class in formaddr: %d(%#x)\n", class, class);
		return(0);
	}
}

/*
 *  stackreg(reg):
 * This routine is intended to be used to find the address of a register
 * variable.  The stack is searched backwards from the current frame
 * until the procedure with the register variable is found.  If the
 * register assigned to the variable appears on the stack, then its
 * address is returned.  Otherwise, the register variable is still in
 * the named register, so the register number is returned.  We distinguish
 * addresses from register numbers by noting that register numbers are less
 * than NUMREGS (16) and that stack addresses are greater.
 *
 *  MACHINE DEPENDENT
 */

/*  68000 version - C stores 'n' registers arbitrarily.
 *	A mask (16 bits) is stored as a  short following a movem 
 *	instruction in the object module. (Unlike the vax, this mask is not 
 *	saved on the stack.) Starting at the entry point of the proc, the
 *	object code is searched for the presence of a movem. The search
 *	currently is set up terminate after 15 instructions or a rts, whichever
 *	comes first. The MOVEM instruction is decoded to fined the mask and EA.
 *      Once the mask is gotten, the stack is then searched.
 *	The 0'th bit of this mask tells whether register 0 has been saved,
 *	the 1'st bit tells wheter register 1 has been saved, etc.
 *	Each saved register in the frame occupies another word of space,
 *	and they are stored in order, from D0 to A7
 *	in ascending locations on the stack.
 *
 *	The first saved register is at:
 *
 * 		EA - WORDSIZE*(number of saved registers)
 *
 *	where (number of saved registers) is computed by counting the
 *  	bits in the MOVEM mask.
 *	
 *	EA is determined by decoding the addressing mode of the MOVEM
 *	instruction.  Currently valid EA's for the MOVEM are:
 *
 *		mode 010  reg 110		(%fp)
 *		mode 101  reg 110		disp(%fp)	(16 bit disp)
 *		mode 101  reg 111		disp(%sp)	(16 bit disp)
 *			(requires link_size, the size of the activation
 *			 record which is decoded from the link.[wl] instruction)
 *
 *	Additions for 881 support:
 *		the argument fpflag=1 denotes type double or float.
 *		If a M68881 floating point co-processor is present,
 *		double or float register variables are stacked in
 *		extended form using the fmovm instruction.  The logic
 *		for finding these stacked register variables is very
 *		similar to that descrived above for "disp(%sp)" case
 *		of the movm.  The mask, however, is only eight bits long
 *		and registers saved are indicated so that bit 0 corresponds
 *		to fp register 7, bit 1 to fp reg 6, etc., therefore
 *		the logic of decoding the mask is a little different.
 *		Also, each register requires 4*WORDSIZE bytes on the
 *		stack.
 */
#define MOVEM	0x48800000	/* movem opcode stored in high word	*/
#define LINKW	0x4e500000	/* link.w opcode stored in high word	*/
#define LINKL	0x48080000	/* link.l opcode stored in high word	*/
#define RTS	0x4e750000	/* rts opcode stored in high word	*/
#define INSLIMIT  15		/* number of assemble language instructions */
#define FMOVM	0xf000e000	/* fline nibble, opclass of fmovm */

ADDR
stackreg(reg, fpflag) {
	register int  regfl, mask, i, ctr;
	register struct proct *procp;
	ADDR regaddr, insaddr, ins;
	long	ea,rsa_disp=0,link_size;
	long	save_frame, fpregfl=0;
	long fprsa_disp;	/* fp reg save area displacement */
	register int fpmask;	/* fp reg save mask */

#if DEBUG > 1
	if(debugflag)
		fprintf(FPRT2, "stackreg(reg=%d);\n", reg);
#endif
	if (proc[0] == '\0')
		return(reg);
	regaddr = reg;
	regfl = 0x10000 << reg;
	if(fpflag)
		fpregfl = 0x1 << (7-reg);
	save_frame = frame;		/* search down stack to this frame */
	for (procp=initframe(); procp!=badproc; procp=nextframe()) {
#if DEBUG > 1
		if (debugflag)
		    fprintf(FPRT2,
			"procp->pname=%s, sl_procp->pname=%s, proc=%s\n",
			procp->pname, sl_procp->pname, proc);
#endif
		/*if (sl_procp == procp)*/
		if (save_frame == frame)
			break;
/* search the object to see if mask is present, giveup after ctr instructions */

		insaddr = procp->paddress;	/* offset in a.out */
		ctr = INSLIMIT;			/* stop after INSLIMIT instrs */
		mask = 0;			/* initialize */
		fpmask = 0;			/* initialize */
		link_size = 0;
		while(ctr--) {
			ins = get(insaddr,ISP);
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2,
				"SEARCH FOR movem instr, ins=%x \n", ins );
#endif
			if((ins&0xff800000) == MOVEM) {
				mask = (ins << 16);
				ea   = ( (ins>>16) & 077 );
				if (ea == 026 ) {	/* (%fp) */
#if DEBUG > 1
		if(debugflag) fprintf(FPRT2, "Mode is 026 (%%fp) \n");
#endif
					rsa_disp = 4;
				   };
				if ( ea == 056 ) {	/* d(%fp) */
				     rsa_disp=
					(short)(get(insaddr+WORDSIZE,ISP)>>16)+4;
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2,
				"Mode is 056 %d(%%fp)\n", rsa_disp );
#endif
				    };
				if ( ea == 057 ) {	/* d(%sp) */
				     rsa_disp=
					(short)(get(insaddr+WORDSIZE,ISP)>>16);
				     rsa_disp = link_size + rsa_disp;
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2,
				"Mode is 057 %d(%%sp)\n", rsa_disp );
#endif
				    };
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2,
				"	ea=%o rsa_disp=%x frame=%x\n",
				ea, rsa_disp, frame);
#endif
				/* continue to look for fmovm */
			}
			if((ins&0xf000e000) == FMOVM) {
				fpmask = (ins & 0xff) ;
				     fprsa_disp=
					(short)(get(insaddr+2*WORDSIZE,ISP)>>16);
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2, "fprsa_disp=0x%08x (%d)\n", fprsa_disp, fprsa_disp);
#endif
				     fprsa_disp = link_size + fprsa_disp;
				break;
			}

			if( (ins&0xfff80000) == LINKW ) {
				link_size = (short)(ins&0x0000ffff);
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2,
				"	LINK.W instruction, size is %d\n",
				link_size);
#endif
			    };

			if( (ins&0xfff80000) == LINKL ) { /* 32 bit link size */
				link_size = (short)(get(insaddr+WORDSIZE,ISP)>>16);
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2,
				"	LINK.L instruction, size is %d\n",
				link_size);
#endif
			    };

			if( (ins&0xffff0000) == RTS) 
				break;
			insaddr = dis_dot(insaddr,ISP, '\0');
		} /* end while loop */
		if ((mask & regfl) && !fpflag) {
			regaddr = frame + rsa_disp;
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2,
				"After finding MOVEM, regaddr=0x%x, frame=0x%x, rsa_disp=%d\n",
				regaddr, frame, rsa_disp );


#endif
			/* find offset of desired register */
			for (i=0; i < reg; i++) {
				if (mask & 0x10000) regaddr += WORDSIZE;
				mask >>= 1;
			}
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2," After modifying regaddr second time,\
			  mask(%x), reg(%d), regaddr=%x\n", mask, reg, regaddr);
#endif
		}
		if ((fpmask & fpregfl) && fpflag) {
			regaddr = frame + fprsa_disp;
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2,
				"After finding FMOVM, regaddr=0x%x, frame=0x%x, fprsa_disp=%d\n",
				regaddr, frame, fprsa_disp );


#endif
			/* find offset of desired register */
			for (i=0; i < reg; i++) {
				if (fpmask & 0x80) regaddr += 3*WORDSIZE;
				fpmask <<= 1;
			}
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2," After modifying regaddr second time,\
			  fpmask(%x), reg(%d), regaddr=%x\n", fpmask, reg, regaddr);
#endif
		}
#if DEBUG > 1
		if(debugflag > 1)
			fprintf(FPRT2,
				"	pname=%s; regaddr=%#x;\n",
					    procp->pname, regaddr);
#endif
	}
	if (procp == badproc) {
		fprintf(FPRT1, "Stackreg() error: frame=%#x\n", frame);
		regaddr = ERROR;
	}
	return(regaddr);
}

/* Print the stack trace: 't' and 'T' commands
 * set top (i.e. top != 0) to print just the top procedure
 * modified to print something even if a.out stripped
 */

/* if stripped or -g not used, default to print NARGDEFAULT words from stack. No other way to tell how many args (no arg count on 68k) */
#define NARGSTACK    (argp += WORDSIZE*2, (narg = (procp->lineno) ? 0 : NARGDEFAULT))

prfrx(top)  /* dd0 */
{	int narg;		/* number of words that arguments occupy */
	long offs;		/* offset into symbol table */
	register char class;	/*storage class of symbol */
	register int endflg;	/* 1 iff cannot find symbolic names of
					more arguments */
	int subsproc = 0;	/* 1 iff not top function on stack */
	register char *p;	/* pointer to procedure (function) name */
	int line_num;		/* line number in calling function */
	register struct proct *procp;
	SYMENT *syp = &syment;
	AUXENT *axp = auxent;
	
	procp = initframe();
	if (frameflg) {		/*  no initial frame */
		if (pid == 0 && (fcor < 0 || fakecor))	{  /* usual error */
			errflg = "No process and no core file.";
			chkerr();
		}
		else {				/* unusual initial frame */
			return;
		}
	}
	do {
		/*  VAX crt0 (start) gets a current fp of zero */
		if (frame == 0) return;
		p = procp->pname;
		if (eqstr("_dbsubc", p))
			return;
		if (procp == badproc) {		/*  if stripped a.out */
			printf("pc: 0x%lx;	args: (", callpc);
			endflg = 1;
		}
		else {
			printf("%s(", p);
			endflg = 0;
		}
		if (endflg == 0) {
			offs = procp->st_offset;
			do {		/*  in COFF, always just 1 ? */
				if( (offs = rdsym(offs)) == ERROR) {
					endflg++;
					break;
				}
			} while (ISFCN(syp->n_type));
			class = syp->n_sclass;
			while (! ISARGV(class)) {
				if ((offs = rdsym(offs)) == ERROR) {
					endflg++;
					break;
				}
				class = syp->n_sclass;
				if (ISFCN(syp->n_type)) {
					endflg++;
					break;
				}
			}
		}

		narg = NARGSTACK;
#if DEBUG > 1
		if(debugflag > 1)
			fprintf(FPRT2, "prfrx(): pname=%s; narg=%d;\n",
						procp->pname, narg);
#endif
		if (eqstr("main", p))	/* always print 3 arguments for main*/
			narg = 3;
		while(ISARGV(class) || (narg > 0)) {
			if (endflg) {
				printf("%d", get(argp, DSP));
				argp += WORDSIZE;
			} else {
				int length;
				if ((syp->n_type == T_STRUCT) ||
				    (syp->n_type == T_UNION))   {
				    /* print address of structure
				     * (so that structures of, e.g.
				     * 100 element arrays, are not dumped)
				     */
				    printf( "&%s=", syp->n_nptr );
				    dispx(argp, "x", C_EXT, 
						(short) (-1), 0, DSP);
				    length = axp->x_sym.x_misc.x_lnsz.x_size;
				}
				else {
				    if (syp->n_type == T_ENUM)
					length =
					    axp->x_sym.x_misc.x_lnsz.x_size;
				    else
					length = typetosize(syp->n_type, 0);
				    printf("%s=", syp->n_nptr);
				    dispx(argp, "", C_EXT, 
					    (short) syp->n_type, 0, DSP);
				}
				if (length > WORDSIZE) {
					argp += length;
				}
				/* bytes, shorts, longs pushed as ints */
				else {
					argp += WORDSIZE;
				}
			}
			do {
				if (endflg) break;
				if ((offs = rdsym(offs)) == ERROR) {
					endflg++;
					break;
				}
				class = syp->n_sclass;
				if (ISFCN(syp->n_type)) {
					endflg++;
					break;
				}
			} while (! ISARGV(class));
			if ((--narg > 0) || ISARGV(class)) printf(",");
		}
		printf(")");
#if DEBUG
		if (debug) fprintf(FPRT2, "  @ 0x%lx ", callpc);
#endif
		if (procp->sfptr != badfile)
			printf("   [%s",
				(procp->sfptr)->sfilename);
		if(gflag) {
			if ((line_num = adrtolineno(callpc-subsproc,procp)) > 0)
				printf(":%d", line_num);
		}
		if(procp->sfptr != badfile)
			printf("]");
		printf("\n");
		subsproc = 1;
		procp = nextframe();
	} while (!top && !frameflg);	/*  only one frame desired, or
						no frames left in backtrace */
/* Vax:	} while (((procp = nextframe()) != badproc) && !top);*/
}


/* machine dependent initialization */
sdbenter(xflag) {
}

/* machine dependent exit code */
sdbexit() {
	exit(0);
}

