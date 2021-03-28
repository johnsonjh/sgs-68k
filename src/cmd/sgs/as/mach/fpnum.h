#ident	"@(#)fpnum.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	OLD MOT:fpnum.h	7.1		*/
/*	OLD MOT:	fpnum.h	7.1	5/30/85	*/
/* Include file for floating point constants
 * manipulation routine "dofpnum.c"
 */
#define		POS	0	/* mantissa and exponent sign */
#define		NEG	1

/* define mostly use contant value for each bit */
#define		BIT0	1
#define		BIT1	2
#define		BIT2	4
#define		BIT3	8
#define		BIT4	16
#define		BIT5	32
#define		BIT6	64
#define		BIT7	128

/* define max. size of mantissa and exponent used in binary format */
#define		SIG	9	/* 9 bytes of mantissa */
#define		EXP	2	/* 2 bytes of exponent */

#define		MS	SIG-1	/* most significant byte of mantissa  */
#define		LS	0	/* least significant byte of mantissa */

#define		msb_e	EXP-1	/* most significant byte of exponent */
#define		lsb_e	0	/* least significant byte of exponent */

/* define maximum length of mantissa and exponent */
#define		MANMAX	17	/* mantissa length up to 17 digits */
#define		EXPMAX	3	/* exponent length up to 3 digits */


/* define floating point rounding mode */
#define 	FP_RN	0	/* round to the Nearest */
#define		FP_RP	1	/* round toward Plus Infinity */
#define		FP_RM	2	/* round toward Minus Infinity */
#define		FP_RZ	3	/* round toward Zero */

/* define floating point precision */
#define		FP_S	0	/* single precision */
#define		FP_D	1	/* double precision */
#define		FP_X	2	/* extended precision */
#define		FP_P 	3	/* packed bcd */


extern short	fp_round;	/* default rounding mode .n  */
extern short	fp_precision;	/* default precision  .x */
extern short	fp_extension;	/* default extension .x */

/* define exceptions */
#define 	FP_INX	0	/* fp exception inexact */
#define 	FP_OVF	1	/* fp exception overflow */
#define 	FP_UNF	2	/* fp exception underflow */
#define		N_EXCEPT	3	/* number of fp exceptions */

extern unsigned int	exceptions[N_EXCEPT];

typedef	struct fpbin
{	short	m[10],	/* binary mantissa */
		e[2];	/* binary exponent */
	unsigned short	sm;	/* sign of mantissa */
}	FPBIN ;


extern char	*fp_man;	/* fp mantissa string	*/
extern char	*fp_exp;	/* fp exponent string	*/
extern unsigned short int
		fp_mansign,		/* mantissa sign   */
		fp_expsign,		/* exponent sign   */
		fp_p;		/* number of digits to the right of the dec. pt. */



