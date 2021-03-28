#ident	"@(#)fpinit.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


/*	OLD MOT:fpinit.h	7.1		*/
/*	OLD MOT:	fpinit.h	7.1	5/30/85	*/
/* Include file for floating point constants
 * manipulation routine "dofpnum.c"
 */
/* define minimum and maximum exponent for each precision */
struct	{
	short minexp[2];
	short maxexp[2];
}	exprange [] =
	/*	ls    ms */
	{	129, 255,	/* single FP_S min exp FF81 */
		128,   0,	/*             max exp 0080 */	
		1,   252,	/* double FP_D min exp FC01 */
		0,     4,	/*             max exp 0400 */
		1,   192,	/* extend FP_X min exp C001 */
		0,    64	/*             max exp 4000 */
	};

/* define the exponent biases for each precision */
struct	{
	short e[2];
}	bias[] =
	/*	ls     ms  */
	{	127,   0,	/* single: exponent bias 127 */
	 	255,   3,	/* double: exponent bias 1023 */
		255,   63	/* extend: exponent bias 16383 */
	};


/* initialize the base power of ten table */
FPBIN pwten[] = 
{
	/* mantissa                                    exponent  sm    k */
	 0, 0,   0,   0,   0,   0,   0,   0, 128, 0,     0, 0 ,POS,	/*0*/
	 0, 0,   0,   0,   0,   0,   0,   0, 160, 0,     3, 0 ,POS,	/*1*/
	 0, 0,   0,   0,   0,   0,   0,   0, 200, 0,     6, 0 ,POS,	/*2*/
	 0, 0,   0,   0,   0,   0,   0,   0, 250, 0,     9, 0 ,POS,	/*3*/
	 0, 0,   0,   0,   0,   0,   0,  64, 156, 0,     13, 0 ,POS,	/*4*/
	 0, 0,   0,   0,   0,   0,   0,  80, 195, 0,     16, 0 ,POS,	/*5*/
	 0, 0,   0,   0,   0,   0,   0,  36, 244, 0,     19, 0 ,POS,	/*6*/
	 0, 0,   0,   0,   0,   0, 128, 150, 152, 0,     23, 0 ,POS,	/*7*/
	 0, 0,   0,   0,   0,   0,  32, 188, 190, 0,     26, 0 ,POS,	/*8*/
	 0, 0,   0,   0,   0,   0,  40, 107, 238, 0,     29, 0 ,POS,	/*9*/
	 0, 0,   0,   0,   0,   0, 249,   2, 149, 0,     33, 0 ,POS,	/*10*/
	 0, 0,   0,   0,   0,  64, 183,  67, 186, 0,     36, 0 ,POS,	/*11*/
	 0, 0,   0,   0,   0,  16, 165, 212, 232, 0,     39, 0 ,POS,	/*12*/
	 0, 0,   0,   0,   0,  42, 231, 132, 145, 0,     43, 0 ,POS,	/*13*/
	 0, 0,   0,   0, 128, 244,  32, 230, 181, 0,     46, 0 ,POS,	/*14*/
	 0, 0,   0,   0, 160,  49, 169,  95, 227, 0,     49, 0 ,POS,	/*15*/
	 0, 0,   0,   0,   4, 191, 201,  27, 142, 0,     53, 0 ,POS,	/*16*/
	 0, 0,   0,   0, 197,  46, 188, 162, 177, 0,     56, 0 ,POS,	/*17*/
	 0, 0,   0,  64, 118,  58, 107,  11, 222, 0,     59, 0 ,POS,	/*18*/
	 0, 0,   0, 232, 137,   4,  35, 199, 138, 0,     63, 0 ,POS,	/*19*/
	 0, 0,   0,  98, 172, 197, 235, 120, 173, 0,     66, 0 ,POS,	/*20*/
	 0, 0, 128, 122,  23, 183,  38, 215, 216, 0,     69, 0 ,POS,	/*21*/
	 0, 0, 144, 172, 110,  50, 120, 134, 135, 0,     73, 0 ,POS,	/*22*/
	 0, 0, 180,  87,  10,  63,  22, 104, 169, 0,     76, 0 ,POS,	/*23*/
	 0, 0, 161, 237, 204, 206,  27, 194, 211, 0,     79, 0 ,POS,	/*24*/
	 0, 0, 132,  20,  64,  97,  81,  89, 132, 0,     83, 0 ,POS,	/*25*/
	 0, 0, 165,  25, 144, 185, 165, 111, 165, 0,     86, 0 ,POS,	/*26*/
	 0, 0,  15,  32, 244,  39, 143, 203, 206, 0,     89, 0 ,POS,	/*27*/
	 0, 0,   9, 148, 248, 120,  57,  63, 129, 0,     93, 0 ,POS,	/*28*/
	 0, 0,  11, 185,  54, 215,   7, 143, 161, 0,     96, 0 ,POS,	/*29*/
	 0, 0,  78, 103,   4, 205, 201, 242, 201, 0,     99, 0 ,POS,	/*30*/
	 0, 0,  34, 129,  69, 64, 124, 111, 252, 0,     102, 0 ,POS,	/*31*/
	 0, 0, 181, 112,  43, 168, 173, 197, 157, 0,     106, 0 ,POS,	/*32*/
	 0, 0, 166, 207, 255,  73,  31, 120, 194, 0,     212, 0 ,POS,	/*33*/
	 0, 0, 140, 233, 128, 201,  71, 186, 147, 0,     169, 1 ,POS,	/*34*/
	 0, 0, 222, 249, 157, 251, 235, 126, 170, 0,     82, 3 ,POS,	/*35*/
	 0, 0, 145,  14, 166, 174, 160,  25, 227, 0,     164, 6 ,POS,	/*36*/
	 0, 0,  12, 117, 129, 134, 117, 118, 201, 0,     73, 13 ,POS,	/*37*/
	 0, 0,  93,  61, 197,  93,  59, 139, 158, 0,     147,26 ,POS,	/*38*/
	 0, 0, 151,  32, 138,   2,  82,  96, 196, 0,     38, 53 ,POS	/*39*/
};

#define		OFFSET	25		/* define pwr_rnd_byte table offset */
/*
 * initialize the power rounding bytes for each target rounding mode.
 * Note, that is no rounding takes place for power less than 25.
 * therefore, indexing to this table is
 * pwr_rnd_byte[fp_round][k-OFFSET] 
 * where k must be
 *		k >= 25  and  k <= 39 
 */
int pwr_rnd_byte[4][15] = {
/* FP_RN */
	{160, 200, 58, 132, 229, 223, 150, 158,
	213, 224, 142, 199, 023, 229,155},
/* FP_RP */
	{160, 200, 58, 133, 230, 223, 151, 158,
	214, 224, 142, 199, 024, 229,155},
/* FP_RM */
	{160, 200, 58, 132, 229, 222, 150, 157,
	213, 223, 141, 198, 023, 228, 154},
/* FP_RZ */
	{160, 200, 58, 132, 229, 222, 150, 157,
	213, 223, 141, 198, 023, 228, 154}
};

/* define round table for target precision
 * the most signf. byte is MS.
 * the ls signf. byte is byte LS.
 */
struct 
{	short	lsbyte;		/* loc. of ls bit     */
	short	lsbit;		/* ls bit of fraction */
	short	gdbyte;		/* loc. of guard bit  */
	short	gdbit;		/* guard bit          */
}	rountab[] = 
		{
		  MS-2, BIT0, MS-3, BIT7,	/* single */
		  MS-6, 8, MS-6, 4,	/* double */
		  MS-7, BIT0, MS-8, BIT7 	/* extend */
		};





