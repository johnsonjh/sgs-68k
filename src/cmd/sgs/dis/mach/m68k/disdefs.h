#ident	"@(#)disdefs.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.2	89/09/26 16:44:28 hossein
	added support for M68040.

    1.1	89/06/08 00:39:15 root
	date and time created 89/06/08 00:39:15 by root

*/

#define		BYTE		1 /* values for size parm to eff_add */
#define		WORD		2
#define		LONG		3
				  /* values for 881's size fields 8-14 */
#define		FLONG		8
#define		FSINGLE		9
#define		FEXTENDED	10
#define		FPACKED		11
#define		FWORD		12
#define		FDOUBLE		13
#define		FBYTE		14

#define		SIGNED		1 /* immediate value signed or unsigned */
#define		UNSIGNED	0

#define		MAXERRS	10	/* Maximum number of errors allowed	*/
				/* before abandoning this disassembly	*/
				/* as a hopeless case.			*/
			
				/* Macro to select the mode bits 5-3	*/
#define MODE(x)	BITS5_3(x)
#ifdef	M68040
#define	CACHE(x)	BITS7_6(x)
#define	SCOPE(x)	BITS4_3(x)
#define	REGISTER(x)	BITS2_0(x)
#define	OP_MODE(x)	SCOPE(x)
#endif

				/* Macro for adding idsp to every	*/
				/* eff_add call.  Necessary for sdb to	*/
				/* permit passing index, idsp is not 	*/
				/* used for dis.			*/
#define eff_add(a,b,c,d)\
		(effect_add((a),(b),(c),(d),idsp))

				/* Macro for generating a standard	*/
				/* effective address.  Uses low 6 bits	*/
				/* of curinstr to get mode and reg.	*/
#define std_eff_add(size,sign)\
		(eff_add(MODE(curinstr),BITS2_0(curinstr),size,sign))

				/* Macro to pad string to next 8 chars	*/
#define PAD(x)	strncat(x,"        ",8-(strlen(x)%8))


extern void	bit_movep_imm();
extern void	move_byte();
extern void	move_long();
extern void	move_word();
extern void	miscell();
extern void	movem();
extern void	mul_div();
extern void	addq_subq_scc_dbcc();
extern void	bcc_bsr_bra();
extern void	moveq();
extern void	or_div_sbcd();
extern void	unassigned();
extern void	cmp_eor();
extern void	and_mul_abcd_exg();
extern void	sub_subx();
extern void	add_addx();
extern void	opa_opx_op();
extern void	shft_rot();
extern void	cp_instr_040();

extern void	move_address();
extern void	recover();
extern char	*effect_add();
extern char	*imm_string();
extern 		fullformat();

extern unsigned short curinstr;	/* for saving first part of instruction
				   when cur2bytes is used to read displ */
extern unsigned short oldinstr;	/* to save previous instruction for
				   testing that swbeg follows jump	*/

extern char	conv_temp[NHEX];	/* Temporary location for ascii	*/
				/* representation of operands.	*/

extern char	comp_temp[NHEX];	/* temp for calling compoff	*/

extern int	errlev;		/* Errors encountered during the	*/
				/* disassembly, probably due to being	*/
				/* out of sync.				*/

#ifdef SDB
					/* data for sdb only */
extern unsigned short	cur2bytes;
extern unsigned long	cur4bytes;
extern int		debugflag;
extern long		loc;		/* byte location being disassembled */
extern char		mneu[NLINE];	/* line to be printed if desired    */
extern char		object[NHEX];	/* reserved for future use	    */
#else
					/* data for dis only */

extern int	 debugflag;
extern jmp_buf	 env;			/* used by set/longjmp when an error is
				   located down in a subroutine these
				   routines allow continuing after resync */


/*	From dis_extn.c.	*/
extern SCNHDR	scnhdr;
extern LDFILE	*t_ptr;
extern int	Lflag;
extern long	loc;
extern char	mneu[];
extern char	*sname;
extern char	*fname;
extern unsigned short	cur2bytes;
extern unsigned long	cur4bytes;

/*	From dis_utls.c.	*/
extern		convert();
extern int	get2bytes();
extern		line_nums();
extern		looklabel();
extern		printline();
extern		prt_offset();
extern		resync();
extern short	sect_name();
#endif
