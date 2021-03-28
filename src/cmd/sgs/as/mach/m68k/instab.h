#ident	"@(#)instab.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	OLD MOT:instab.h	7.1			*/


/*		Header file for mc68000 assembler.			*/

typedef	enum
	{
		FALSE,
		TRUE
	}	BOOL;


#define	B	0		/* Size attribute .b == BYTE.		*/
#define W	1		/* Size attribute .w == WORD.		*/
#define L	2		/* Size attribute .l == LONG.		*/
#define UNSPEC	3		/* No size attribute specified.		*/
#define S	4		/* Size attribute .s == Single precision	*/
#define X	5		/* Size attribute .x == Extended precision	*/
#define P	6		/* Size attribute .p == Packed BCD	*/
#define D	7		/* Size attribute .d == Double precision	*/
#define	RN	8		/* round type .n == round to nearest		*/
#define	RM	9		/* round type .n == round to minus		*/
#define	RZ	10		/* round type .n == round to zero		*/


/* These must fit within a byte */

#define PSEUDO	0x40
#define DADD	0x41
#define ADD	0x42
#define XADD	0x43
#define LOGAND	0x44
#define ASHFT	0x45
#define BRA	0x46
#define	BR	0x47
#define BCHG	0x48
#define BTST	0x49
#define BSR	0x4a
#define MULT	0x4b
#define OPTSIZE	0x4c
#define CMP	0x4d
#define DBCC	0x4e
#define LOGEOR	0x4f
#define EXG	0x50
#define EXT	0x51
#define LEA	0x52
#define LINK	0x53
#define LSHFT	0x54
#define MOV	0x55
#define MOVM	0x56
#define MOVP	0x57
#define LOGOR	0x58
#define ROT	0x59
#define ROTX	0x5a
#define STOP	0x5b
#define SWAP	0x5c
#define TRAP	0x5d
#define UNLK	0x5e
#define LOGSRC	0x5f
#define LOGDST	0x60
#define ADDQ	0x61
#define MOVMMR	0x62
#define MOVMRM	0x63
#define ADDSRC	0x64
#define ADDDST	0x65
#define RTD	0x66
#define MOVS	0x67
#define BFCHG	0x68
#define BFFFO	0x69
#define BFINS	0x6a
#define BFTST	0x6b
#define BKPT	0x6c
#define CAS 	0x6d
#define CAS2	0x6e
#define CALLM	0x6f
#define CHK	0x70 
#define CHK2	0x71
#define CMP2	0x72
#define DIVS	0x73
#define DIVU	0x74
#define MULS	0x75
#define MULU	0x76
#define PACK	0x77
#define RTM	0x78
#define TDIVS	0x79
#define TDIVU	0x7a
#define TMULS	0x7b
#define TMULU	0x7c
#define TPCC	0x7d
#define TST	0x7e
#define UNPK	0x7f
#define	DEFAULT	0x80			/* default; size (B,W,L) added in */
/* Allow for DEFAULT+[B|W|L|UNSPEC] */
#ifdef  M68881 		/* define co_processor instructions */
#	define	CPBCC	0x84
#	define	CPDBCC	0x85
#	define	CPRESTORE	0x86
#	define	CPSAVE	0x87
#	define	CPGEN	0x88
#	define	CPSCC	0x89
#	define	CPTCC	0x8a
#	define	CPTPCC	0x8b
#	define	FCMP	0x8c
#	define	FMOV	0x8d
#	define	FMOVCR	0x8e
#	define	FMOVM	0x8f
#	define	FNOP	0x90
#	define	FOP	0x91
#	define	FSGLDIV	0x92
#	define	FSGLMUL	0x93
#	define	FSINCOS	0x94
#	define	FTST	0x95
#endif /*M68881*/

#ifdef M68030
#define	PFLUSH	0x96
#define	PLOAD	0x97
#define	PMOVE	0x98
#define	PTEST	0x99
#endif 	

#ifdef M68040
#define	PFLUSH	0x96
#define CINV	0x97
#define CPUSH	0x98
#define	PTEST	0x99
#define	MOVE16	0x9a
#endif

#define	MOVEQ	0x9b	/* hoss#9 */

/*	define register type */
#	define NULAREG	001
#	define NULDREG	002
#	define NULPCREG 003

#ifdef M68881
#	define	FPREG		4	/* floating point register	*/
#	define	FPCONTROL	5	/* floating point control register	*/
#	define	FPIADDR		6	/* floating point iaddr register	*/
#	define	FPSTATUS	7	/* floating point status register	*/
#endif /*M68881*/

#define ADREG	000
#define AAREG	010
#define AIREG   020
#define AINC    030
#define ADEC    040
#define AOFF	050
#define ANDX	060
#define ABSW	070
#define ABSL	071
#define APCOFF	072
#define APCNDX	073
#define AIMM	074


#define PCREG   0x70L
#define CCREG   0x72L
#define SRREG   0x73L
#define CTLREG	0x74L
#define USPREG	0x800L	/* this is an opcode, not a type! (i.e. > byte) */

#if defined( M68030 ) || defined( M68040 )
#define	CRPREG	0x75L
#define	SRPREG	0x76L
#define	TCREG	0x77L
#define	TTREG	0x78L
#define	MMUREG	0x79L
#define	ITREG	0x8aL
#define	DTREG	0x8bL
#endif	/** M68030 || M68040	**/

#ifdef	M68040
#define	CACHE	0xa0L	/* defined for CINV instructions	*/
#define	DTCACHE	0x1L
#define INCACHE	0x2L
#define	DICACHE	0x3L
#endif


/* define addressing mode */
#define CONTROL	0x27e	/* control addressing mode only */
#define CONPRE	0x2f8	/* control alterable or predecrement */
#define CONPOST	0x37e	/* control or postincrement */
#define ALTMEM	0x3f8	/* alterable memory addressing */
#define DATALT	0xbf8	/* data alterable addressing */
#define DATNIM	0xbfe	/* data but not immediate */
#define DATAA	0xbff	/* data addressing */
#define ALT	0xff8	/* alterable addressing */
#define ALL	0xfff	/* all addresssing mode */
#define DnALTCON 0xa78	/* data register direct or alterable control */
#define DnCONTROL 0xa7e	/* data register direct or control */
#define	MEMORY	0x3ff	/* memory addressing mode	*/

#	define X1	0	/* define scales for indexed addressing */
#	define X2	1
#	define X4	2
#	define X8	3

#	define INDIRECT  0	/* no memory indirection */
#	define POSTNDXMI 1	/* post indexed memory indirect */
#	define PRENDXMI  2	/* pre indexed memory indirect */
#	define BFINDIRECT  3	/* no memory indirection,brief format */

#	define SET	1	/* as in "ss" in sized registers */
#	define NOTSET	0


#	define FP_INT	1	/* integer value, don't need any conversion	*/
#	define FP_BIN	2	/* floating point constant in binary format	*/
#	define FP_DEC	3	/* floating point constant in dec. ASCII	*/

struct	arg
	{
 		BYTE		atype;
		instr		*areg1;
		instr		*areg2;
		BYTE		asize;		/* associated with index reg */
		BYTE		ascale;		/* associated with index reg */
		short		aflags;
		struct exp	*xp1;
		struct exp	*xp2;
#define xp	xp1
	};

struct	exp
	{
		short	xtype;
		symbol	*xsymptr;
		long	xvalue;
		BYTE	xsize;			/* sized displacements	*/
#ifdef M68881
		short	xvaltype;		/* value type, FP_BIN,FP_DEC	*/
		long	xvalue2;		/* double, extended, and	*/
		long	xvalue3;		/* packed BCD extension	*/
#endif /* M68881 */
	};

typedef union
	{
		long		ylong;
 		int		yint;
		symbol		*yname;
		instr		*yinst;
		struct exp	*yexp;
		struct arg	*yarg;
		char		*ystr;
	} YYSTYPE;

extern  YYSTYPE		yylval;
extern struct arg	*ap;
extern struct exp	*xp;
