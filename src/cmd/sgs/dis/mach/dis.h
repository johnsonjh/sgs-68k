#ident	"@(#)dis.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.1	89/06/08 00:39:11 root
	date and time created 89/06/08 00:39:11 by root

*/
/*	MOT-dis.h	7.1		*/
/* dis.h */
/* NOTE: these files are used for dis AND for sdb.	*/
/*	5.2-dis.h	7.1		16:22:00";	*/


#ifdef SDB
#	define	TOOL		"sdb"
#	define	GET2BYTES	get2bytes(idsp)
#	define	GET4BYTES	get4bytes(idsp)
#	define	ERROR_ACTION	errlev++;return
#	define	FPRT2		stderr
#	define	SGS		""
#else
#	define	TOOL		"dis"
#	define	GET2BYTES	get2bytes(t_ptr)
#	define	GET4BYTES	get4bytes(t_ptr)
#	define	ERROR_ACTION	recover()
				/* Maximum number of errors allowed	*/
				/* before abandoning this disassembly	*/
				/* as a hopeless case.			*/
#	define	MAXERRS		10
#endif


#define		NCPS	8	/* Number of chars per symbol.		*/
#define		NHEX	50	/* Maximum # chars in object per line.	*/
#define		NLINE	33	/* Maximum # chars in mnemonic per line.*/
#define		FAIL	0
#define		TRUE	1
#define		FALSE	0
#define		LEAD	1
#define		NOLEAD	0
#define		TERM	0

#define LNNOBLKSZ	1024	/* size of blocks of line numbers		*/
#define SYMBLKSZ	1024	/* size of blocks of symbol table entries	*/
#define	STRNGEQ		0	/* used in string compare operation */

#define BIT0(x) ( (x) & 0x1L)           /* -------x bit 0               */
#define	BIT1(x)  ( (x >> 1) & 0x1L)
#define	BIT2(x)  ( (x >> 2) & 0x1L)
#define	BIT3(x)  ( (x >> 3) & 0x1L)	/* ----x--- bit 3		*/
#define	BIT5(x)	 ( (x >> 5) & 0x1L)	/* --x----- bit 5		*/
#define	BIT6(x)	 ( (x >> 6) & 0x1L)	/* -x------ bit 6		*/
#define	BIT7(x)	 ( (x >> 7) & 0x1L)	/* x------- bit 7		*/
#define BIT8(x)	( (x >> 8) & 0x1L)
#define BIT10(x) ( (x >> 10) & 0x1L)
#define BIT11(x) ( (x >> 11) & 0x1L)
#define BIT12(x) ( (x >> 12) & 0x1L)
#define BIT15(x) ( (x >> 15) & 0x1L)
#define BITS15_13(x) ( ( x>>13 ) & 0x7L)
#define BITS15_12(x) ( ( x>>12 ) & 0xfL)
#define BITS15_11(x) ( ( x>>11 ) & 0x1fL)
#define	BITS15_8(x) ( (x >> 8) & 0xffL)	
#define BITS15_7(x) ( (x >> 7) & 0x1ffL)
#define BITS14_12(x) ( (x >> 12) & 0x7L)
#define BITS12_10(x) ( (x >> 10) & 0x7L)
#define	BITS11_9(x) ( (x >> 9) & 0x7L)	/* bits 11 through 9		*/
#define	BITS11_8(x) ( (x >> 8) & 0xfL)	/* bits 11 through 8		*/
#define BITS11_0(x) (x & 0xfffL )
#define	BITS10_9(x) ( (x >> 9) & 0x3L)	/* bits 10 through 9		*/
#define	BITS10_8(x) ( (x >> 8) & 0x7L)	/* bits 10 through 8		*/
#define BITS10_6(x) ( (x >> 6) & 0x1fL)
#define BITS9_7(x) ( (x >> 7) & 0x7L)
#define BITS9_6(x) ( (x >> 6) & 0xfL)
#define	BITS8_7(x) ( (x >> 7) & 0x3L)	/* bits 8 through 7		*/
#define	BITS8_6(x) ( (x >> 6) & 0x7L)	/* bits 8 through 6		*/
#define BITS8_3(x) ( (x >> 3) & 0x3fL)	/* bits 8 through 3		*/
#define	BITS7_6(x) ( (x >> 6) & 0x3L)	/* bits 7 through 6		*/
#define BITS7_3(x) ( (x >> 3) & 0x1fL)	
#define BITS7_0(x) ( x & 0xffL )
#define BITS6_4(x) ( (x >> 4) & 0x7L)
#define	BITS6_0(x) ( x & 0x7fL)
#define	BITS5_4(x) ( (x >> 4) & 0x3L)	/* bits 5 through 4		*/
#define	BITS5_3(x) ( (x >> 3) & 0x7L)	/* bits 5 through 3		*/
#define	BITS5_0(x) ( x & 0x3fL)		/* bits 5 through 0		*/
#define	BITS4_3(x) ( (x >> 3) & 0x3L)	/* bits 4 through 3		*/
#define	BITS4_0(x) ( x & 0x1fL)
#define	BITS3_0(x) ( x & 0xfL)		/* bits 3 through 0		*/
#define	BITS2_0(x) ( x & 0x7L)		/* bits 2 through 0		*/
#define BITS1_0(x) ( x & 0x3L)		/* bits 1 through 0		*/
#define	LOW8(x) ( x & 0xffL)		/* low 8 bits of quantity	*/
#define HIOF32(x) ( (x >> 31) & 0x1L)	/* sign bit of 32 bit quantity	*/
#define	HI4OF16(x) ((x >> 12) & 0xfL)
