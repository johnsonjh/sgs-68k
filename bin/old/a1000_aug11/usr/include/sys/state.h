#ifndef SYS_STATE_H
#define SYS_STATE_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) state.h: version 2.1 created on 5/22/89 at 19:11:02	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)state.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys:state.h	2.1"

#define	NUMGREG	16
#define	SSW_RW	0x40
#define	SSW_DF	0x0100
#define	DF(ssw)	((ssw)&SSW_DF)
#define	SSW_FC	0x8000
#define	SSW_FB	0x4000
#define	SHORTF  0xA008
#define	LONGF	0xB008
#define	SSW_FCMSK 0x7
#define	SSW_UD	0x01
#define	SSW_UC	0x02
#define	SSW_RMC	0x0080
#define	VECTOR(vecoff)	(((vecoff)&0xFFF)>>2)
#define	stageB(ssw)	((ssw)&SSW_FB)
#define	stageC(ssw)	((ssw)&SSW_FC)
#define	USERACC(ssw,ps)	(USRMODE(ps) || \
			((ssw)&SSW_FCMSK) == SSW_UD \
			|| ((ssw)&SSW_FCMSK) == SSW_UC)

/* 
 * stack frame for bus error handling routine
 */



struct	state	{
	caddr_t	s_sp;		/* usp */
	ulong	s_reg[NUMGREG];
	ushort	s_pad[11];
	ushort	s_ps;		/* psw */
	caddr_t	s_pc;		/* instruction counter */
	ushort	s_vecoff;	/* stack format and vector offset */
	ushort	s_inter1;	/* internal register */
	ushort	s_ssw;		/* special status word for bus error */
	ushort	s_ipstC;	/* instruction pipe stage C */
	ushort	s_ipstB;	/* instruction pipe stage B */
	ulong	s_dcfa;		/* data cycle fault address */
	ulong	s_inter2;	/* internal register */
	ulong	s_dob;		/* data output buffer */
	ushort	s_inter3;	/* internal register */
	ushort	s_inter6;	/* internal register */
	ulong	s_inter4;	/* internal register */
	ulong	s_stBaddr;	/* stage B address */
	ulong	s_intern5;	/* internal register */
	ulong	s_dib;		/* data input buffer */
	ulong	s_inter6a;
	ushort  s_inter6b;
	ushort	s_version;	/* bits 12-15 contain version # */
	ulong	s_inter7[9];
};

#endif /* SYS_STATE_H */
