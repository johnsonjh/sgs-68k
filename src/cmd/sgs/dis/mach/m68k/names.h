#ident	"@(#)names.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.2	89/09/26 16:45:14 hossein
	added support for M68040.

    1.1	89/06/08 00:39:16 root
	date and time created 89/06/08 00:39:16 by root

*/
/*
 *MOT : names.h	7.1	
 *		External definitions and defines for table driven 
 *		opcodes and register names for the MC68020
 *
 */


/*		from names.c		*/
extern char	*imm_char;
extern char	size[];
extern char	*opt_size[];
extern char	*cond_codes[];
extern char	*addregs[];
extern char	*reg_names[];
extern char	*suppressed_reg_names[];
extern char	*ctl_reg_names[];
extern char	*scale_string[];
extern char	*bit_ops[];
extern char	*bit_field_ops[];
extern char	*imm_fmt[];
extern char	*chk2_cmp2_fmt[];
extern char	*cas_cas2_fmt[];
extern char	*boolean_fmt[];
extern char	*miscell_8_fmt[];
extern char	*mul_div_ops[];
extern char	*shift_rot_fmt[];
extern char	*bcd_ops[];
extern char	*misc_ops[];
extern char	*other_ops[];

			/* Define index int ctl_reg_names	*/
#define	SFC	0			/* changing text_dis.c		      */
#define	DFC	1
#define	CACR	2
#ifdef	M68040
#define	TC	3
#define	ITT0	4
#define	ITT1	5
#define	DTT0	6
#define	DTT1	7
#define	SR	8
#define	USP	9	
#define	VBR	10
#define	MSP	11
#define	ISP	12
#define	MMUSR	13
#define	URP	14
#define	SRP	15
#define	CCR	16
#else
#define	SR	3
#define	USP	4
#define	VBR	5
#define	CAAR	6
#define	MSP	7
#define	ISP	8
#define	CCR	9
#endif

			/* Define index into bcd_ops    	*/
#define	NBCD	3
			/* Define indicies into other_ops	*/
#define BADOP	0
#define MOVEA	1
#define	MOVEP	2
#define	MOVES	3
#define	MOVEQ	4
#define	MOVEI	5
#define	MOVE	6
#define	RTM	7    
#define	CALLM	8  
#define	SWBET	9
#define	ILLEGAL	10
#define	LINK_L	11
#define	SWAP_W	12
#define	BKPT	13
#define	PEA	14
#define	EXT	15
#define	TRAP	16
#define	UNLK	17
#define	LINK	18
#define	MOVEC	19
#define	JSR	20
#define	JMP	21
#define	MOVEM	22
#define	DBcc	23
#define	Scc	24
#define	Tcc	25
#define	Bcc	26
#define	BSR	27
#define	BRA	28
#define	SUBQ	29
#define	ADDQ	30
#define	OR	31
#define	CMPA	32
#define	CMPM	33
#define	EOR	34
#define	CMP	35
#define	ABCD	36
#define	EXG	37
#define	AND	38
#define	SUB	39
#define	ADD	40
#define SWAP_L	41
#define TAS	42
			/* Define modifier indicies */
#define	AMOD	43 	/* AMOD - modifier for adda suba */
#define	XMOD	44 	/* XMOD - modifier for adds/subx */
#define CINVL	45
#define CINVP 	46
#define CINVA	47
#define CPUSHL	48
#define CPUSHP	49
#define CPUSHA	50
#define MOVE16	51
#define PFLUSH	52
#define	PFLUSHN 53
#define	PFLUSHA	54
#define	PFLUSHAN	54
#define	PTESTR	55
#define PTESTW	56
