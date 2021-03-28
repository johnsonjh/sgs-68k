/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) machdep.h: version 23.2 created on 5/8/91 at 10:37:13	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)machdep.h	23.2	5/8/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/com:machdep.h	23.2"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
	/*	OLD MOT:machdep.h	6.3		*/

#include	<sys/param.h>
#include	<sys/types.h>
#include	<signal.h>

#include	<sys/sysmacros.h>

#define USRSTACK 0x80000000

#include	<sys/immu.h>
#include	<sys/region.h>
#include	<sys/user.h>
#include	<sys/errno.h>
#include	<a.out.h>
#include	"sdbreg.h" 	/* hh (part of mach/sys/reg.h) */
#include	<sys/reg.h>
#include	<sys/stat.h>

#define NARGDEFAULT 3
#define XTOB(x) (ctob(x))
#define ISJSR		((get(dot,ISP) & 0xffc00000) == 0x4e800000)  /* jsr call */
#define ISBSR		((get(dot,ISP) & 0xff000000) == 0x61000000)  /* bsr call */
#define SUBCALL		(ISJSR || ISBSR)
#define SUBRET		get(SDBREG(SP), DSP)	/* subroutine ret instruction */
#define RETREG		0			/* register with ret value (d0)  */

/* Given the current fp (frame), these are the next frame pc, ap, fp */
#define NEXTCALLPC (frame + WORDSIZE)
#define NEXTFRAME  (frame)
#define NEXTARGP   NEXTFRAME   /* no arg ptr for 68k */

#define TXTRNDSIZ 512L	/* not used after R1 V2.8; use optional header */

#define	PROCOFFSET	0	/* offset from label to first instruction */
#define BKOFFSET    0	/* offset from beginning of proc to bkpt */
#define ALTBKOFFSET 0	/* offset from beginning of proc to bkpt */

#define FPNO	14	/* frame pointer register number */
#define APNO	FPNO	/* 68000 does not have arg ptr, define in terms of fp */
#define AP	FP	/* FP defined in /usr/include/sys/reg.h */
#define NUMREGLS 18	/* number of "registers" in reglist[] */

#define ISDATAREG(regno) ((2 <= (regno) && (regno) < 8)) 
#define ISADDRREG(regno) ((10 <= (regno) && (regno) < 14)) 
#define ISREGVAR(regno) (ISDATAREG(regno) || ISADDRREG(regno))


extern ADDR	callpc, frame, argp;	/* current stack frame */

#define WORDSIZE (sizeof(int))	/* wordsize in bytes on this machine */
#define REGSIZE WORDSIZE	/* register size in bytes on this machine */

#define NOBACKUP 0		/* set to 1 if machine does not back up */
			        /* to previous instruction at exception */
#define ADDRTYPE	"d"	/* type of address for getval */

#define MAXPOS 	0x7fffffff 	/* max int instead of max addr for 68020 */

/*  two extra numbers to be printed with regs; in optab.c */
/*  removed because these are not offsets from R0; can't use SDBREG */
/*
#define VAL1	((unsigned)&(((struct user *) 0)->u_rval1)
#define VAL2	((unsigned)&(((struct user *) 0)->u_rval2)
*/

/* ptracew modes */
#define	SETTRC	0
#define	RDUSER	2
#define	RIUSER	1
#define	WDUSER	5
#define WIUSER	4
#define	RUREGS	3
#define	WUREGS	6
#define	CONTIN	7
#define	EXIT	8
#define SINGLE	9
#define RFPUREGS 10
#define WFPUREGS 11

extern REGLIST reglist [];

#define SDBREG(A) (regs[(A)])
#define SYSREG(A) (&uu.u_ar0[(A)])	/* U_AR0 + offset of reg in state_t */

#define NUMREGS	18	/* number of registers */

#define ISREGN(reg)	(0 <= (reg) && (reg) < NUMREGS && (reg) != 17 && (reg) != 18)

#define USERPC  SDBREG(PC)

union word {
	char c[WORDSIZE]; 
	unsigned char cu[WORDSIZE];
	short s[WORDSIZE/2];
	unsigned short su[WORDSIZE/2];
	int w;
	unsigned int iu;
	long l;
	float f;
};
union dbl {
	struct {
		int w1, w2;
	} ww;
	double dd;
	int i;
	float f;
	char cc[WORDSIZE*2];
};

fregs proc_fpp;

typedef union {
	user_t	u_struct;
	uchar	u_page[XTOB(USIZE)];
} user_page_t;

extern user_page_t	user_page;

#define uu		user_page.u_struct
