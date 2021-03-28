#ifndef SYS_STATE_H
#define SYS_STATE_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) state.h: version 24.1 created on 10/28/91 at 18:38:27	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)state.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

	/* flags and macros for special status word */


#ifdef M68020
#define	SSW_FC		0x8000
#define	SSW_FB		0x4000
#define	SSW_RC		0x2000
#define	SSW_RB		0x1000
#define	SSW_DF		0x0100
#define SSW_RMW		0x0080
#define SSW_READ	0x0040		/* this bit off means a write */
#define SSW_FC2		0x0004		/* function code 2 kernel/user */

#define	DF(ssw)		((ssw) & SSW_DF)
#define	stageB(ssw)	((ssw) & SSW_FB)
#define	stageC(ssw)	((ssw) & SSW_FC)
#define	kernel_ref(ssw)	((ssw) & SSW_FC2)

/* special status word for A and B stack frame formats (same offset in both) */
#define s_ABssw		s_proc.formatA.fA_ssw
#endif /* M68020 */

#ifdef	M68040
/* Defines for format 7 special status word and writeback status words        */
#define	SSW_CP	0x8000		/* pending fp post exception w/trace  */
#define	SSW_CU	0x4000		/* pending fp unimpl. instr. w/trace  */
#define	SSW_CT	0x2000		/* pending trace                      */
#define	SSW_CM	0x1000		/* pending movm                       */
#define	SSW_MA	0x0800		/* misaligned; fault really on nxt pg */
#define	SSW_ATC	0x0400		/* atc fault eg: page not pres. w-prot*/
#define	SSW_LK	0x0200		/* locked op, like cas, tas           */
#define	SSW_RW	0x0100		/* fault on read or write?            */
#define 	RW_READ		0x0100
#define 	RW_WRITE	0x0000
#define	SSW_X		0x0080		/* unused, reserved                   */
#define	SSW_SZ	0x0060		/* size of original operation         */
					/* sizes also for writeback status    */
#define		SZ_BYTE		0x0020	/* single byte			      */
#define		SZ_WORD		0x0040  /* two bytes                          */
#define		SZ_LONG		0x0000  /* four bytes                         */
#define		SZ_LINE		0x0060  /* sixteen bytes                      */
#define	SSW_TT	0x0018		/* transfer type                      */
					/* transfer types also for writebacks */
#define		TT_NORM		0x0000  /* normal access failure              */
#define		TT_MOV16	0x0008  /* mov16 failure                      */
#define		TT_ALT		0x0010  /* alt logical func code access       */
#define		TT_ACK		0x0018  /* acknowledge access failure         */
#define	SSW_TM	0x0007		/* transfer modifier                  */
					/* transfer types also for writebacks */
#define		TM_PUSH		0x0000  /* data cache push failure            */
#define		TM_FC0		0x0000  /* or fc0, if TT_ALT                  */
#define		TM_UDATA	0x0001  /* user data access                   */
#define		TM_UMOV16	0x0001  /* or user mov16 access if TT_MOV16   */
#define		TM_UCODE	0x0002  /* user code access                   */
#define		TM_MMUDATA	0x0003  /* data mmu access                    */
#define		TM_FC3		0x0003  /* or fc3 if TT_ALT                   */
#define		TM_MMUCODE	0x0004  /* code mmu access                    */
#define		TM_FC4		0x0004  /* or fc4 if TT_ALT                   */
#define		TM_SDATA	0x0005  /* supervisor data access             */
#define		TM_SMOV16	0x0005  /* or supervisor mov16 if TT_MOV16    */
#define		TM_SCODE	0x0006  /* supervisor code access             */
#define		TM_INVALID	0x0007  /* invalid/reserved                   */
#define		TM_FC7		0x0007  /* or fc7 if TT_ALT                   */
/* Defines for the writeback status words                                     */
/* values for SZ, TT, TM are defined above in SSW                             */
#define	WBS_VALID	0x0080		/* writeback pending if set           */
#define	WBS_SZ		0x0060		/* original size of transfer          */
#define	WBS_TT		0x0018		/* transfer type                      */
#define	WBS_TM		0x0007		/* transfer modifier                  */
#endif	/* M68040 */


/* 
 * stack frame for all types of exceptions
 */


typedef struct	state	{
	ulong	s_sp;		/* usp */
	ulong	s_d0, s_d1, s_d2, s_d3, s_d4, s_d5, s_d6, s_d7;
	ulong	s_a0, s_a1, s_a2, s_a3, s_a4, s_a5, s_a6;
	ushort	s_ps;		/* psw */
	ulong	s_pc;		/* instruction counter */

	uint	s_format : 4;	/* describes format of any additional info */
	uint	s_crunch : 1;	/* reduce this stack frame to format 0 */
	uint	s_expand : 1;	/* expand this stack frame */
	uint	s_vector : 8;	/* vector number */
	uint	s_unused : 2;	/* unused bits */

	/* additional processor state information (see s_format above) */
	union	{
		/* normal stack frame */
		uint	format0;

		/* throwaway four word stack frame */
		struct {
			ushort	f1_ps;
			ulong	f1_pc;
			ushort	f1_vecoff;
		} format1;

		/* six word stack frame */
		struct {
			ulong	f2_instaddr;
		} format2;

		/* M68040 general bus error stack frame */
		struct {
		    ulong *	f7_ea;		/* effective addr for trace */
		    ushort	f7_ssw;		/* don't confuse with 020 type*/
		    /* write back stata (or statuses) */
		    ushort	f7_wb3s;	
		    ushort	f7_wb2s;	
		    ushort	f7_wb1s;	
		    ulong *	f7_fa;		/* virt fault addr */
						/* phys fault addr for push */
		    ulong *	f7_wb3a;	/* writeback 3 address */
		    ulong	f7_wb3d;	/* writeback 3 data */
		    ulong *	f7_wb2a;	/* writeback 2 address */
		    ulong	f7_wb2d;	/* writeback 2 data */
		    ulong *	f7_wb1a;	/* writeback 1 address */
		    ulong	f7_wb1d;	/* writeback 1 data */
#define f7_pd0	f7_wb1d				/* and pushback data 0 */
		    ulong	f7_pd1;		/* pushback data 1 */
		    ulong	f7_pd2;		/* pushback data 2 */
		    ulong	f7_pd3;		/* pushback data 3 */
		} format7;

		/* coprocessor mid-instruction stack frame */
		struct {
			ulong	f9_instaddr;
			ushort	f9_INTERNAL[4];
		} format9;

		/* short bus cycle fault stack frame */
		struct {
			ushort	fA_INTER0[1];	/* internal register(s) */
			ushort	fA_ssw;		/* special status word */
			ushort	fA_pipeC;	/* instruction pipe stage C */
			ushort	fA_pipeB;	/* instruction pipe stage B */
			ulong	fA_faultaddr;	/* data cycle fault address */
			ushort	fA_INTER1[2];	/* internal register(s) */
			ulong	fA_outbuff;	/* data output buffer */
			ushort	fA_INTER2[2];	/* internal register(s) */
		} formatA;

		/* long bus cycle fault stack frame */	
		struct {
			ushort	fB_INTER0[1];	/* internal register(s) */
			ushort	fB_ssw;		/* special status word */
			ushort	fB_pipeC;	/* instruction pipe stage C */
			ushort	fB_pipeB;	/* instruction pipe stage B */
			ulong	fB_faultaddr;	/* data cycle fault address */
			ushort	fB_INTER1[2];	/* internal register(s) */
			ulong	fB_outbuff;	/* data output buffer */
			ushort	fB_INTER2[4];	/* internal register(s) */
			ulong	fB_stBaddr;	/* stage B address */
			ushort	fB_INTER3[2];	/* internal register(s) */
			ulong	fB_inbuff;	/* data input buffer */
			ushort	fB_INTER4[3];	/* internal register(s) */
			ushort	fB_cpu_type : 4;/* mask version */
			uint	fB_INTER5 : 12;	/* internal register(s) */
			ushort	fB_INTER6[18];	/* internal register(s) */
		} formatB;
	} s_proc;
} state_t;

#endif /* SYS_STATE_H */
