#ident	"@(#)dofpnum.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."


/*	OLD MOT:dofpnum.c	7.1		*/
/*	OLD MOT:	dofpnum.c	7.1	5/30/85	*/
#include "fpnum.h"		/* constants definations */
#include "fpinit.h"		/* tables initialization */
#include "gendefs.h"

/* bk: definitions for objects declared in fpnum.h */
short	fp_round;	/* default rounding mode .n  */
short	fp_precision;	/* default precision  .x */
short	fp_extension;	/* default extension .x */

unsigned int	exceptions[N_EXCEPT];

char	*fp_man;	/* fp mantissa string	*/
char	*fp_exp;	/* fp exponent string	*/
unsigned short int
		fp_mansign,		/* mantissa sign   */
		fp_expsign,		/* exponent sign   */
		fp_p;		/* number of digits to the right of the dec. pt. */


/*
 *	DO_FP_RNUM
 *		Convert a floating point immediate with an
 *	extension of .S, .D, .X, and .P into its binary
 *	equivalent value. 
 *
 *	Input parameters:
 *		FP_STR	contains the 
 *			mantissa string,
 *			exponent string,
 *			sign of mantissa,
 *			sign of exponent,
 *			and number of digit to the right of the decimal point.		
 *
 *	Output parameters:
 *		fp_num contains the resultant binary value of the immediate
 *		value. fp_num is an array which packed as the following order:
 *
 *	               S             D           X             P
 *
 *	fp_num[5]    MS word      MS word     MS word     SM,SE,Exponent
 *	fp_num[4]    LS word         |           |        Integer Digit
 *	fp_num[3]      0             |           |        MS fraction
 *	fp_num[2]      0          LS word        |             |
 *	fp_num[1]      0             0           |             |
 *	fp_num[0]      0             0        LS word     LS fraction
 *
 *	Other parameters:
 *		. Globals fp_round ( rounding mode ) and fp_precision
 *		  are referenced to find out the approriate information.
 *
 *		. Globals fp_extension which is the size code of the value
 *		  used. It has value .b, .w, .l, .s, .d, .x, and .p.
 *
 *		. Globals exprange contains the maximum and minimun
 *		  allowable exponents for each target size.
 *
 *		. Global  rountab contains the location of the l (ls bit),
 *		  and g (guard bit) for each rounding precision.
 *
 *		. Global pwten contains the power of ten table.
 *
 *		. pwr_rnd_byte is a table containing rounding bytes for
 *		  powers of ten.
 *
 */
do_fp_num(fp_num)
unsigned int	fp_num[6];
{
	int i;
	
	for ( i = 0; i <=5; i++)	/* zero result */
		fp_num[i] = 0;
	for ( i = 0; i < N_EXCEPT; i++)	/* no exceptions to start */
		exceptions[i] = 0;

	switch(fp_extension)
	{
		case FP_P:
			decpack(fp_man,fp_exp,fp_p,fp_mansign,fp_expsign,fp_num);
			break;

		case FP_S:
		case FP_D:
		case FP_X:
			decbin(fp_man,fp_exp,fp_p,fp_mansign,fp_expsign,fp_num);
			break;

		default:
			aerror("floating extension is undefined");
	}	/* end switch */


	if ( exceptions[FP_INX] ) 
		werror("result is inexact");
	if ( exceptions[FP_OVF] ) 
		werror("result is overflow");
	if ( exceptions[FP_UNF] ) 
		werror("result is undeflow");

}	/* end do_fp_num */

/**/
/* 
 *	DECPACK
 *		Convert a decimal string to the packed '.p' format.
 *		
 *		Since the decimal point in the decimal string can be   
 *		anywhere and since the .P format requires that the
 *		decimal point be between the first and second digits.
 *		one of the main tasks of this routines is to adjust
 *		the decimal exponent if necessary.
 *	
 *		entry:
 *			man		mantissa string
 *			exp		exponent string
 *			p		number of digits to the right of dec. pt.
 *			mansign		mantissa sign
 *			expsign		exponent sign
 *			
 *		exit:
 *			fp_num		6 words fp number in the .P format
 *
 *
 *
 *		Packed decimal floating point format:
 *
 *   15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0    temp[n]
 * 
 *  |SM|SE| y  y|exp(2)      |exp(1)     |exp(0)    |   23 -- 20 
 *  |-----------------------------------------------------------
 *  |    zeroes                          |mant(16)  |   19 -- 16
 *  |-----------------------------------------------
 *  |mant(15)   |mant(14)    |mant(13)   |mant(12)  |   15 -- 12
 *  |-----------------------------------------------
 *  |mant(11)   |mant(10)    |mant(9)    |mant(8)   |   11 --  8
 *  |-----------------------------------------------
 *  |mant(7)    |mant(6)     |mant(5)    |mant(4)   |    7 --  4
 *  |-----------------------------------------------
 *  |mant(3)    |mant(2)     |mant(1)    |mant(0)   |    3 --  0
 *  | -----------------------------------------------
 *  
 *  
 *  SM           Sign of mantissa
 *  
 *  SE           Sign of exponent
 *  
 *  exp(n)       'n'th BCD digits of the unbiased decimal exponent
 *  
 *  mant(n)      'n'th BCD digits of the
 *                decimal mantissa 
 *                mant(16) is the integer digit.
 *                mant(15) - mant(0) are the
 *                fraction digits
 *  
 *   
 */
decpack(man,exp,p,mansign,expsign,fp_num)
char	*man,		/* mantissa string */
   	*exp;		/* exponent string */
unsigned short int
		p,		/* number of digits to the right of decimal point */
		mansign,	/* mantissa sign either POS or NEG */
		expsign;	/* exponent sign either POS or NEG */

unsigned int fp_num[6];			/* 6 words fp number in the .P format */
{
	int	expadj,		/* exponent adjust */
	   	binexp;		/* binary equiv. of decimal exponent */
	short	temp[24];	/* used to collect nubbles of the result */
	short   manlen,explen;	/* mantissa and exponent length */	
	short	i, j;

	bzero(temp,24);		/* initialize temporary space */

	/* set bit 15 for SM and bit 14 for SE 
	 */
	if ( mansign == NEG )		
		temp[23] = 0x8;
	if ( expsign == NEG )		
		temp[23] += 0x4;     
	
	/* move the mantissa string into temp starting at temp[16]. 
	 * This has the effect of putting the decimal pt between
	 * digits 1 and 2
	 */
		manlen = strlen(man);
		for ( i = 16, j = 1; j <= manlen ; i--, j++)
			temp[i] = *man++ - '0';	/* strip off ascii */

	/* convert the decimal exponent to a binary integer 
	 */
		explen = strlen(exp);
		for ( binexp = 0, j = 1; j <= explen; j++ )
		{
			binexp *= 10;
			binexp += *exp++ - '0';
		}

	/* calculate the amount we have to adjust the exponent
	 * and then adjust it.
	 */
		expadj = manlen - p - 1;
		if ( expsign == POS )
			binexp += expadj;
		else
			binexp -= expadj;

		if ( binexp < 1000 )
		{
			/* convert resulting exponent back into packed bcd
			 */
			temp [22] = binexp / 100;	/* ms digit */
			temp [21] = (binexp / 10) % 10;	/* middle digit */
			temp [20] = binexp % 10;		/* ls digit */
		}
		else
		{	/* too big to represent after exponent adjustment
			 * this code is for a specific case when
			 * .xxxxe-999 is entered. The above algorithm
			 * will change this to x.xxxe-1000 which is too 
			 * small. This code undoes the algorithm and leaves
			 * it .xxxxe-999 which fits 
			 */
			if ( expsign == NEG )
			{
				for ( i = 0; i <= 15 ; i++ )
					temp[i] = temp [i+1];
				temp [16] = 0;
				temp [22] = temp [21] = temp [20] = 9;
			}
			else
			{	/* overflow */
				exceptions[FP_OVF] = 1;
				switch ( fp_round )
				{
					case FP_RN :
						decinfinity(temp);
						break;
					case FP_RZ :
						decmaxnbr(temp);
						break;
					case FP_RP :
						if ( mansign == POS )
							decinfinity(temp);
						else
							decmaxnbr(temp);
						break;
					case FP_RM :
						if ( mansign == POS )
							decmaxnbr(temp);
						else
							decinfinity(temp);
						break;
				}	/* end switch */
			}	/* end overflow */
		}	/* end specific case */	
		
		/* move the temp result to fp_num */

		for ( i = 6; i >= 1; i--)
		{
			j = ( 4 * i ) - 1;		/* index into temp */
			if ( temp [j] > 7 ) temp[j] -= 16;
			fp_num[i-1] = (temp[j] * 4096) + (temp[j-1] * 256) +
				(temp[j-2] * 16) + temp[j-3];
		}
}	/* end decpack */

/**/
/*
 *	DECBIN
 *		Given input strings for the mantissa and exponent,
 *		and the number of digits to the right of the decimal
 *		point, return the equivalent fp number.
 *
 *		This is a common routine for extensions .S, .D and .X
 *	
 *		Algorithm :
 *
 *		if the mantissa string is zero then return zero as the result
 *		else
 *
 *	 	Convert the exponent string to a signed integer 'w'.
 *		Move the decimal point to the right of the mantissa
 *		string by subtracting 'p' from 'w'. The result
 *		exponent 'expten' will be used in further calculations.
 *		
 *		(if expten is greater than 27 or less than -27 then the
 *		power of ten will be inexact, unless we can force the
 *		expten back into this range with right or left shifts.
 *		If we can't get back into the accurate range, the smaller
 *		the absolute value of the exponent the more accurate it is.
 *		Then, we will shift whichever way will result in the most
 *		accurate power of ten.)
 *		
 *		Call "strbin" to convert mantissa string to a a fp integer.
 *		Get the power of ten for abs(expten).
 *
 *		if expten >= 0 then
 *		Result = mantissa fp integer * power of ten 
 *		else
 *		Result = mantissa fp integer / power of ten
 *	
 *		entry:
 *			man		mantissa string
 *			exp		exponent string
 *			p		number of digits to the right of dec. pt.
 *			mansign		mantissa sign
 *			expsign		exponent sign
 *			
 *		exit:
 *			fpbin		global struct (fp number in binary format)
 */
decbin(man,exp,p,mansign,expsign,fp_num)
char	*man,		/* mantissa string */
   	*exp;		/* exponent string */
unsigned short int
		p,	/* number of digits to the right of decimal point */
		mansign,/* mantissa sign either POS or NEG */
		expsign;/* exponent sign either POS or NEG */
unsigned int fp_num[6];
{
	FPBIN	fpmult(),fpdiv(),pwrten(),strbin();
	int	expten,		/* power of ten */
		mulct,		/* multiply count */
		w;		/* original exponent */
	short int i;		/* temp index */
	short int manlen;	/* mantissa length */
	FPBIN	f,g;

	manlen = strlen(man);
	if (stzero(man))		/* if mantissa = zeroes */
	{	/* return correctly signed zero */
		bzero(f.m,10);
		bzero(f.e,2);
		f.e[msb_e] = BIT7 + BIT6;
		f.sm = mansign;
		/* packed fpbin in the result array fp_num and return */
		fppack (f,fp_num);
		return;
	}
	
	/* get the exponent value into w */
	for ( w = 0; *exp != '\0'; exp++)
		w = (w*10) +  (*exp - '0');
	if ( expsign == NEG ) w = -w;

	/* moves the decimal point to the right */
	expten = w - p;

#if	FPDEBUG
	printf("DECBIN: exponent value after moving the decimal point %d\n",expten);
#endif

	mulct = 0;
	if ( expten > 27 || expten < -27 )
	{	/* power of ten inexact 
		 * then strip off trailing 0 and increment exponent 
		 * by number of zeroes being stripped 
		 */
		expten += stripzr ( man );
		
		if ( expten > 27 )
		{ 	/* if the exponent becomes too big 
		 	 * then left shift mantissa and decrement exponent
		 	 */
			mulct = MANMAX - manlen;
			mulct += leadzr(man);
			expten -= mulct;
		}
	}	/* end power of ten inexact */

	/* convert mantissa string to fp integer
	 * and left shift if necessary
	 */
	 f = strbin(man,mansign,mulct);

#if FPDEBUG
	 printf("DECBIN: mantissa after converting to integer value\n");
	 prfpbin("DECBIN",&f);
#endif


	 /* Multiply or divide the fp integer f
	  * by power of ten
	  */
	  if ( expten != 0 )
	  {
		g = pwrten (expten,f.sm);	/* create power of ten */
		if ( expten > 0 )
			f = fpmult(f,g);
		else
			f = fpdiv(f,g);
	  }

#if FPDEBUG
	 printf("\tBefore calling fppack\n");
	 prfpbin("DECBIN",&f);
#endif
	/* packed fpbin in the result array fp_num and return */
	fppack (f,fp_num);
	return;

}	/* end decbin */

/**/
/*
 *	FPPACK
 *		Takes a floating value fpbin and packs the number so
 *	that it is represented as the IEEE standard draft 10.0.
 *
 *  Single precision:
 *
 *    31   30                 23 22            0
 *  --------------------------------------------
 *  |1-bit|     8-bit          |       23 bit  |
 *  |sign |     biased exponent|       fraction|
 *  --------------------------------------------
 *
 *  Double precision:
 *
 *  63   62                 52  51          0
 *  -------------------------------------------
 *  |1-bit|    11-bit          |       52 bit |
 *  |sign |    biased exponent |      fraction|
 *  -------------------------------------------
 *
 * Extended precision:
 *
 *  96    95              81  80     64  63    0
 *  ----------------------------------------------
 *  |1-bit|   15-bit        | undefined |  64 bit|
 *  |sign |  biased exponent|           |fraction|
 *  ----------------------------------------------
 *
 *	Significands in single and double have an implied 1.0 hidden.
 *	In extended the 1.0 is explicit.
 *
 *	Since the number fpbin stored internally may not fit in the
 *	target format, this rountine has to check overflow and under-
 *	flow. It also round inexact results.
 *
 *	Global variable used:
 *		fp_extension	extension of the instruction
 */
fppack(f,num)
FPBIN	f;
int	num[];
{
	short	temp[12];	/* temporary storage */
	int 	carry, i,j;
	short	top = 11;	/* most significant byte */
#if FPDEBUG
	 printf("\tmantissa before packing to integer value\n");
	 prfpbin("FPPACK",&f);
#endif
	bzero(temp,12);	/* zero out temporary space */

	/* pack based on the extension of the instruction */
	switch(fp_extension)
	{
		case FP_S:
			/* use the top 32 bits of temp */
			validout(&f,FP_S);
			if ( f.sm == NEG )
				temp[top] += BIT7;	/* insert sign */
			binadd(f.e,bias[FP_S].e,f.e,EXP);

			/* insert exponent, only 8 bits are allowed */
			carry = rshift (f.e,EXP);
			temp[top]  += (f.e[lsb_e] % BIT7);
			temp[top-1] += ( carry * BIT7 );

			/* insert significant */
			temp[top-1] += ( f.m[MS] % BIT7 );
			temp[top-2] = f.m[MS-1];
			temp[top-3] = f.m[MS-2];
			break;
		case FP_D:
			/* use the top 64 bits of temp */
			validout(&f,FP_D);
			if ( f.sm == NEG )
				temp[top] += BIT7;
			binadd(f.e,bias[FP_D].e,f.e,EXP);

			/* insert exponent , only 11 bits are allowed*/
			for ( i = 0; i <= 3; i++ )
				lshift ( f.e,EXP);
			temp[top] += (f.e[msb_e] % BIT7);
			temp[top-1] += f.e[lsb_e] ;

			/* insert significant, start after bit 4 of top-1 */
			f.m[MS] %= BIT7;
			for ( i = 0; i <= 4; i++ )
				lshift(f.m,SIG+1);
			temp[top-1] += (f.m[MS+1] % BIT4 );
			for ( i = 0; i <= 5; i++)
				temp[top-2-i] = f.m[MS-i];
			break;

		case FP_X:
			validout(&f,FP_X);
			if ( f.sm == NEG )
				temp[top] += BIT7;
			binadd(f.e,bias[FP_X].e,f.e,EXP);
			/* insert exponent, into upper 16 bits */
			f.e[msb_e] %= BIT7;
			temp[top] += f.e[msb_e];
			temp[top-1] += f.e[lsb_e];
			/* insert significant starting at bit 63 */
			for ( i = 0; i <= 7; i++ )
				temp[top-4-i] = f.m[MS-i];
			break;
	}	/* end switch */

	/* convert temp into num */
	for ( i = 5; i >= 0; i-- )
	{	j = i*2 + 1;
		if ( temp[j] > 127 ) temp[j] -= 256;
		num[i] = temp[j] * 256 + temp[j-1];
	}
}	/* end fppack */

/**/
/* 
 *	STRBIN
 *		Takes the mantissa string and returns the integer 
 *	value f.
 *
 *	Entry :
 *		man		mantissa string
 *		mansign		mantissa sign
 *		mulct		number of times the number should
 *				be multiplied by 10
 *		
 *	Return:
 *		f		(equivalent fp number) of type FPBIN
 */
FPBIN strbin(man,mansign,mulct)
char	*man;
short	mansign,
	mulct;
{
	FPBIN	f;
	int	carry;
	short	i;
	bzero(f.m,SIG);
	f.sm = mansign;
	/* convert mantissa string to binary */
	for ( ; *man != '\0'; man++)
	{
		carry = *man - '0';
		mul10(f.m,carry,SIG);
	}

	/* do any additional mul10's */
	for ( carry = 0, i = 0; i  < mulct ; i++ )
		mul10(f.m,carry,SIG);
	
	/* initialize exponent to 71 */
	f.e[lsb_e] = (SIG * 8) -1;
	f.e[msb_e] = 0;

#if	FPDEBUG
	printf("\tBefore calling quicknorm\n");
	prfpbin("STRBIN",&f);
#endif

	quicknorm(&f);

#if	FPDEBUG
	printf("\tfpbin after quicknormalizing\n");
	prfpbin("STRBIN",&f);
#endif

	return(f);
}	/* end strbin */

/**/
/*
 *	VALIDOUT
 *		Performs a simple output validation on the input variable
 *	f of type FPBIN, to get its exponent into the range for precision
 *	prec.
 *
 *	the algorithm is:
 *		Get the minimun exponent value allowed for the target prec.
 *		check for underflow
 *		if underflow then
 *			if zero return zero
 *			else denormalize the result, and ajust the bias
 *		round
 *		check for overflow
 *		if overflow then case rounding mode of
 *			rn: return signed infinity
 *			rz: return signed max number
 *			rp: if signif. is pos then return infinity
 *			    else return max number
 *			rm: if signif. is pos then return max number
 *			    else return infinity.
 */
validout(f,prec)
FPBIN	*f;
short	prec;
{
	short inv[EXP],thrshold[EXP];
	
	binassgn ( thrshold,exprange[prec].minexp,EXP);
	bincr ( thrshold,EXP );

	/* check for underflow */
	/* handle underflow by denormalization */
	if ( binscmpr(f->e,thrshold,EXP) == -1 )
	{	/* exponent is below denorm threhold */
		if ( bifzero(f->m,SIG) )
			binassgn(f->e,exprange[prec].minexp,EXP);
		else
		{	exceptions[FP_UNF] = 1;
			binassgn(inv,f->e,EXP);
			negate(inv,EXP);
			/* find the difference */
			binadd(exprange[prec].minexp,inv,inv,EXP);
			if ( inv[LS+1] > 0 || inv[LS] > 65 )
			{
				bzero(f->m,SIG);
				binassgn(f->e,exprange[prec].minexp,EXP);
			}
			else
			{	/* denormalize it till exponent is at threshold */
				while ( binscmpr(f->e,thrshold,EXP) < 0 )
				{
					rshift(f->m,SIG);
					bincr(f->e,EXP);
				}
				/* subtract denorm bias */
				if ( !(f->m[MS] & BIT7) ) 
					bindec(f->e,EXP);
			}
		}	/* end not zero */
	}	/* end below threshold exp. */


	round(f,0,prec);


	/* check for overflow */
	if ( binscmpr(f->e,exprange[prec].maxexp,EXP) >= 0 )
	{
		exceptions[FP_OVF] = 1;
		switch(fp_round)
		{
			case FP_RN:
				infinity(f,prec);
				break;
			case FP_RZ:
				maxnbr(f,prec);
				break;
			case FP_RP:
				if(f->sm == POS )
					infinity(f,prec);
				else
					maxnbr(f,prec);
				break;
			case FP_RM:
				if( f->sm == POS )
					maxnbr(f,prec);
				else
					infinity(f,prec);
				break;
			default:
				aerror("Invalid floating point rounding mode");
		}	/* end switch */
	}	/* end if overflow */
}	/* end validout */

/**/
/*
 *	DECINFINITY
 *		Return a .P infinity
 */
 decinfinity(temp)
 short	temp[24];
 {
	int i;
	/* clear bit 14, set bit 13 and 12. 
	 * leave mantissa sign unaltered
	 */
	temp[23] = (temp[23]/0x8) * 0x8 + 3;
	for ( i = 22 ; i >= 20 ; i--)
		temp[i] = 0xf;
	for ( i = 19; i >= 0; i--)
		temp[i] = 0;

}	/* end decinfinity */

/*
 *	DECMAXNBR
 *		Return a .P max number
 */
decmaxnbr(temp)
short	temp[24];
{
	int i;
	/* set 3-digit exp to all 9's,
	 * set mantissa to all 9's,
	 * and leave the rest unaltered
	 */
	for ( i = 22 ; i >= 20 ; i--)
		temp[i] = 9;   
	for ( i = 16; i >= 0; i--)
		temp[i] = 9;

}	/* end decmaxnbr */

/*
 *	MAXNBR 
 *		Returns max value mantissa for precision .S, .D, and .X..
 *	Sign remains unchanged. Exponent becomes maximun according to
 *	precision. Mantissa becomes all F's.
 *
 *		Parameters: Global variable f of type FPBIN
 */
maxnbr(f,prec)
FPBIN	*f;
short	prec;
{
	short i;
	binassgn(f->e,exprange[prec].maxexp,EXP);
	for ( i = LS ; i <= MS ; i++ )
		f->m[i] = 255;
}

/* INFINITY
 *		Returns an infinity in f for precision .S, .D, and .X.
 *	Sign remains unchanged. Exponent becomes maximun according to
 *  precision. Mantissa becomes all zeroes.
 *
 *		Parameters: Global variable f of type FPBIN
 */
infinity(f,prec)
FPBIN	*f;
short	prec;
{
	binassgn(f->e,exprange[prec].maxexp,EXP);
	bzero(f->m,SIG);
}




/**/
/*
 *	FPMULT
 *		Returns properly rounded product of f*g in h.
 *	To meet the IEEE standard, two times the number
 *	of bits in a FPBIN must be calcuated to accurately
 *	round the product.
 */
FPBIN fpmult(f,g)
FPBIN	f,g;
{
	FPBIN	h;	/* product of f and g */
	short	i,		/* index */
		v,		/* overflow bit */
		stik,	/* sticky bit used in rounding */
		bits;	/* max. mantissa bits */
	short	rlo[10];
	/* sign of result is exculsive-or of operand signs */
	if ( f.sm == g.sm )
		h.sm = POS;
	else
		h.sm = NEG;

	/* set result exponent to sum of operand exponents */
	binadd ( f.e,g.e,h.e,EXP);
	
	/* multiply two mantissas to get a double-precision
	 * result.
	 */
	bzero(h.m,SIG);
	bzero(rlo,SIG);
	bits = SIG*8;
	for ( v = 0, i = 1; i <= bits && !( bifzero(f.m,SIG)); i++ )
	{
		/* double shift h.m:rlo */
		lshift(h.m,SIG);
		h.m[LS] += lshift(rlo,SIG);
		if ( lshift(f.m,SIG) )
			if ( binadd(rlo,g.m,rlo,SIG) )
				v = bincr(h.m,SIG);
	}	/* end mantissa multiplcation loop */
	/* if loop terminated early go finish shifts */
	if ( i < bits )
		fastleft(bits-(i-1),h.m,rlo);

	/* normalize 1 bit if needed */
	if ( !(h.m[MS] & BIT7) )
		lshift(h.m,SIG);
	else
		bincr(h.e,EXP);

	/* round the result */
	if ( bifzero(rlo,SIG) )
		stik = 0;
	else
		stik = 1;


	round(&h,stik,fp_precision);

#if FPDEBUG
	printf("\tvalue of h after round (f*g)\n");
	prfpbin("FPMULT",&h);
#endif
	return (h);

}	/* end fpmult */

/*
 *	FPDIV
 *		Returns quotient of f/g in h.		
 */
FPBIN fpdiv(f,g)
FPBIN	f,g;
{
	FPBIN	h;	/* quotient of f and g */
	short 	ng[10];	/* negation of g.m */
	short	i,v;
	short	bits,
		quobit,	/* quotation bit */
		stik;	/* sticky bit, use for rounding */
	if ( f.sm == g.sm ) h.sm = POS;
	else
		h.sm = NEG;

	/* calculate exponent result
	 * as the exponent of f minus the exponent of g.
	 * A 2's complement addition is performed 
	 */
	negate(g.e,EXP);


	binadd (f.e,g.e,h.e,EXP);
	/* calculate two's comp. of g for subtraction */
	binassgn(ng,g.m,SIG);
	negate(ng,SIG);
	bzero(h.m,SIG);

	bits = SIG*8;
	/* Divide loop. Do sig*8 times or until remainder becomes zero */
	for ( v=0, i=1 ;
		i <= bits && !( ( v == 0) && ( bifzero(f.m,SIG)))
	 	; i++)
	{	
		if ( v || (bincmpr( f.m,g.m,SIG) >= 0) ) 
		{
			quobit = 1;
			binadd(f.m,ng,f.m,SIG);
		}
		else
			quobit = 0;

		v = lshift ( f.m,SIG);
		lshift (h.m,SIG);
		h.m[LS] += quobit;
	}	/* end calculate quotient bits */
		
	if ( i <= bits ) 
		fastleft(bits-(i-1),h.m,f.m);
	/* normalize quotient one bit if needed */
	if ( !(h.m[MS] & BIT7) )
	{
		lshift(h.m,SIG);
		bindec(h.e,EXP);
	}

	/* round quotient */
	if (  v == 0 && bifzero(f.m,SIG) )
		stik = 0;
	else
		stik = 1;
	round(&h,stik,fp_precision);
#if FPDEBUG
	printf("\tvalue of h after round f/g\n");
	prfpbin("FPDIV",&h);
#endif
	return (h);
}	/* end fpdiv */

/**/
/*
 *	FASTLEFT
 *		Left shifts a double precision binary integer
 *	(hi:lo) left by ct bits. Both hi and lo must be
 *	size "SIG" 
 */
fastleft(ct,hi,lo)
int	ct;
short	hi[],lo[];
{
	int	bytes,
		bits;
	int	i,		/* leftmost pointer */
		j;		/* rightmost pointer in double prec. value */
	bytes = ct / 8;		/* nbr of bytes to shift left */
	bits = ct % 8;		/* nbr of bits to shift left  */
	if(bytes)
	{
		if ( bytes > SIG )
		{	/* shift out whole lo binary integer */
			/* start from most to least significant byte */
			for ( i = MS; i >= LS ; i-- )
			{
				hi[i] = lo[i];
				lo[i] = 0;
			}
			bytes -= SIG;
		}	

		/* shift remainding bytes */
		i = MS ;
		j = MS  - bytes ;
		for (; j >= LS  ; i-- ,j --)
			/* move up bytes in hi */
			hi[i] = hi[j];

		if ( bifzero(lo,SIG) )
			/* if lo is zero, there is nothing more to move up */
			for ( ; i >= LS ; i-- )
				hi[i] = 0;
		else
		{
			/* move j to top of lo */
			j = MS ;
			for ( ; i >= LS ; i-- ,j--)
				hi[i] = lo[j];
				
			/* move i into lo too */
			i = MS;
			for ( ; j >= LS; j-- ,i--)
				lo[i] = lo[j];

			/* fill lo bits with zero */
			for (; i >= LS ; i--)
				lo[i] = 0;
		}
	}	

	/* shift any remainding bits */
	if ( bifzero(lo,SIG) )
		while(bits>0)
		{
			lshift(hi,SIG);
			bits -= 1;
		}
	else
		while (bits>0)
		{
			lshift(hi,SIG);
			hi[LS] += lshift(lo,SIG);
			bits -= 1;
		}

}	/* end fastleft */

/**/
/*
 *	GETPWR
 *		Get a correctly rounded power from the table.
 *	
 */
FPBIN getpwr(k)
{
	FPBIN f;
	f = pwten[k];
	if ( k > 27 )		/* inexact */
		exceptions[FP_INX] = 1;
	if ( k > 24 )		/* get correct round last byte */
		f.m[1] = pwr_rnd_byte[fp_round][k-OFFSET];

	return(f);
}

/*
 *	PWRTEN
 *		Find the rounded power of ten.
 *		entry:
 *		'k' is the signed power of ten desired.
 *		sm  sign, either POS or NEG
 *
 *		return:
 *		'f' is the resulting power of ten correctly rounded.
 */
FPBIN pwrten(k,sm)
int	k;     
short	sm;
{
	FPBIN	f,g;
	int	absk;		/* absolute of k */
	int	i;
	short	rmsave;

	/* save caller's rounding mode */
	rmsave = fp_round;

	/* using the caller's rounding mode, the sign of the 
	 * desired power, and the sign of number the power will
	 * be multiplied or divided by, we can calculate the
	 * correct 'local rounding mode' using in this procedure
	 *
	 * sign of power   sign of nbr    caller's rnd mode  local rnd mode
	 *     
	 *	    +              +               RN                RN
	 *	    +              +               RZ                RM
	 *	    +              +               RP                RP
	 *	    +              +               RM                RM
	 *
	 *	    +              -               RN                RN
	 *	    +              -               RZ                RM
	 *	    +              -               RP                RM
	 *	    +              -               RM                RP
	 *
	 *	    -              +               RN                RN
	 *	    -              +               RZ                RP
	 *	    -              +               RP                RM
	 *	    -              +               RM                RP
	 *
	 *	    -              -               RN                RN
	 *	    -              -               RZ                RP
	 *	    -              -               RP                RP
	 *	    -              -               RM                RM
	 */

	 if ( k > 0 )     
	 {	/* sign of power is pos */
		if ( sm == POS ) 
		{
			if ( fp_round == FP_RZ ) fp_round = FP_RM;
		}
		else
		{	switch(fp_round)
			{
				case FP_RZ : fp_round = FP_RM; break;
				case FP_RP : fp_round = FP_RM; break;
				case FP_RM : fp_round = FP_RP; break;
				case FP_RN : ;
			}	/* end switch */
		}
	}
	else
	{	/* sign of power is neg */
		if ( sm == POS )
		{	switch(fp_round)
			{
				case FP_RZ : fp_round = FP_RP; break;
				case FP_RP : fp_round = FP_RM; break;
				case FP_RM : fp_round = FP_RP; break;
				case FP_RN : ;
			}
		
		}
		else
			if ( fp_round == FP_RZ ) fp_round = FP_RP;
	}

	/* The power of ten table contains a canned value
	 * for powers up to 32. After 32 only the 2**n powers
	 * are included. Any power can thus be obtained by
	 * multiplying lesser powers.
	 * examples:  10^130 = 10^2 * 10^32 * 10^32 * 10^32 * 10^32
	 */
	absk = abs(k);
	i = absk % 32;		
	if ( i > 0 )
		f = getpwr(i);
	else
	{	/* return 1.0 */
		f.sm = POS;
		bzero(f.e,EXP);
		bzero(f.m,SIG);
		f.m[MS] = BIT7;
	}
	absk /= 32;		/* get powers greater than 32 */
	i = 32;

	/* loop to mult. by next power of ten for each bit
	 * set in k       
	 */

	 while ( absk != 0 )
	 {
		if ( absk % 2 )
		{
			g = getpwr (i);		/* get 2^th power of ten */
			f = fpmult(g,f);		/* mult by previous result */
		}
		absk /= 2;
		i++;
	}
	fp_round = rmsave;		/* restore rounding mode */

#if FPDEBUG
	prfpbin("PWRTEN",&f);
#endif

	return(f);
}	/* end pwrten */

/**/
/*
 *	ROUND
 *		Round the fp number f based on the target precision 'prec'
 *	This rounding is performed as required by the IEEE standard
 *	draft 10.0
 *
 *	Rounding is accomplished using the v, lsbit(l), guard(g) , 
 *  and sticky(s) bits.
 *
 *	The algorithm is:
 *
 *		gather up all the sticky bits in s
 *		extract g and l bits
 *		if g=s=0 then result is exact
 *		else
 *		   set inexact result flag
 *		   case rounding mode of
 *		       M : if neg then add 1 to lsbit 
 *		       P : if pos then add 1 to lsbit
 *		       N : if g=1 
 *		              if ( s = 1 or l = 1 )
 *		              round to nearest even ( lsbit = 0 )
 *		       Z : do nothing
 *		endif
 *		if overflow 
 *		   shift right 
 *		   increment exponent
 *		endif
 *		clear g and s
 *
 */
round(f,s,prec)
FPBIN *f;
short	s,	/* sticky bit */
	prec;	/* precision */
{
	short int	v,	/* overflow bit */
		g,	/* guard bit */
		l;	/* least-signf-> bit */
	short int	gbyte,
		gbit,
		lbyte,
		lbit;
	short i;
	lbyte = rountab[prec].lsbyte;
	lbit  = rountab[prec].lsbit;
	gbyte = rountab[prec].gdbyte;
	gbit  = rountab[prec].gdbit;
	/* Calculate the guard and least significant bits */
	g = ( f->m[gbyte] / gbit ) % 2;
	l = ( f->m[lbyte] / lbit ) % 2;
	
	/* Gather up all the bits that make up the sticky bit */
	for ( i = gbyte - 1; i >= LS  ; i-- )
	{
		if ( f->m[i] != 0 )
			s = 1;
		f->m[i] = 0;
	}
	/* Inculde lower portion of quard byte in sticky bit also */
	if ( (f->m[gbyte] % gbit) != 0 )
		s = 1;

	/* Zero aout below the rounding precision */
	if ( prec == FP_D )
		f->m[gbyte] = ( f->m[gbyte] / (gbit * 2) ) * (gbit * 2);
	else
		f->m[gbyte] = 0;

	v = 0;

	if ( g != 0 || s != 0 )
	{	/* we have inexact result */
		exceptions[FP_INX] = 1;
		switch ( fp_round )
		{
			case FP_RM :
				if (f->sm == NEG)
				{	f->m[lbyte] += lbit;
					v = binadjst (f->m,SIG);
				}
				break;
			case FP_RP :
				if (f->sm == POS)
				{	f->m[lbyte] += lbit;
					v = binadjst (f->m,SIG);
				}
				break;
			case FP_RN :
				if ( g )
					if ( s || l )
					{	f->m[lbyte] += lbit;
						v = binadjst(f->m,SIG);
					}
				break;
			case FP_RZ :
				break;
		}	/* end switch */
		/* Normalize mantissa if there is an overflow */
		if ( v )
		{	rshift(f->m,SIG);
			f->m[SIG] += BIT7;
			bincr(f->e,EXP);
		}
	}	/* end result was inexact */
}	/* end round */

/**/
/*
 *	MUL10
 *		Multiply x, where x is an array of short integers, by 10.
 *	Carry is a BCD carry in and must be < 10.
 */
mul10(x,carry,size)
short	x[];
int	carry;
unsigned short	size;
{
	int	nxtcarry;
	short i;
	for ( i = 0; i < size ; i++ )
	{
		if (x[i])
		{
			x[i] = ( x[i] * 10 ) + carry;
			if ( x[i] > 255 )
			{	/* there is a carry out */
				nxtcarry = x[i] / 256;
				x[i] %= 256;
			}
			else
				nxtcarry = 0;
		}
		else
		{
			nxtcarry = 0;
			x[i] = carry;
		}

		carry = nxtcarry;
	}	/* end for */
}	/* end mul10 */

/**/
/*
 *	QUICKNORM
 *		This procedure will normalize a fpbin. 
 *	Unlike "normalize",which doesn't allow any ls bits to
 *	be shifted in from another byte. 
 *
 *		This procedure tries to do a coarse left shift
 *	of the mantissa by looking for a whole byte full of 
 *	zeroes. It shifts these bytes out and then does a
 *	normalization that is guaranteed to do 8 or less shifts.
 */
quicknorm(f)
FPBIN *f;
{
	short	i,j,
		fst;		/* first non zero byte */
	/* counting leading zero bytes */
	for ( fst = MS ; f->m[fst] == 0 ; fst --)
		;
	/* shift to the left */
	for ( j = MS, i = fst; i >= LS ; i--,j--)
		f->m[j] = f->m[i];

	/* clear out ls bytes if any */
	for ( ; j >= LS ; j--)
		f->m[j] = 0;

	/* adjust exponent */
		f->e[lsb_e] -= (MS  - fst)*8;

	/* start fine normalization */
	while ( !(f->m[MS] &  BIT7) )	/* while bit7 is not set */
	{
		lshift( f->m,SIG);
		f->e[lsb_e] --;
	};

#if	FPDEBUG
	prfpbin("QUICKNORM",f);
#endif

}	/* end quicknorm */

/**/
/*
 *	LSHIFT
 *		Simulates a logical left shift of a short integer.
 *	and returns the bit shifted out of the ms bit. A zero
 *	is shifted into the ls bit.
 */
lshift(x,size)
short x[];
short size;
{
	int i,cary;
	cary = 0;
	for ( i = 0; i < size; i++)
	{
		x[i] = ( x[i] + x[i] ) + cary;	/* multipy by 2 ,plus carry*/
		if ( x[i] >= 256 )
		{
			x[i] -= 256;
			cary = 1;
		}
		else
			cary = 0;
	}
	return(cary);
}
	
/*
 *	RSHIFT
 *		Simulates a logical right shift of binary integer.
 *	and returns the bit shifted out of the low-order bit.
 *	A zero is shifted into the ms bit.
 */
rshift(x,size)
short x[];
short size;
{
	int	i,  caryin, caryout;
	caryin = 0;
	for ( i = size - 1; i >= 0; i--)
	{
		caryout = x[i] % 2;
		x[i] = (x[i] / 2) + (caryin * BIT7);
		caryin = caryout;
	}
	return(caryout);
}	/* end rshift */

/**/
/*
 *	BINADD
 *		Performs a binary 2's complement add of 'x' to 'y'
 *	leaving the result in 'z'.
 *		Returns the carry out of the ms bit of the result.
 *
 */
binadd(x,y,z,size)
short	x[], y[], z[];
unsigned short size;
{
	short i,carry;
	for  ( i = 0; i < size; i++)
		z[i] = x[i] + y[i];
	carry = binadjst(z,size);
	return(carry);
}	/* end binadd */

/*
 *	BINADJST
 *		Adjusts for carries between bytes of 'x' caused
 *	by an addition or a multiplication by two.
 */
binadjst(x,size)
short x[];
unsigned short size;
{
	short i,carry;
	for ( carry=0,i=0; i < size; i++)
	{
		x[i] += carry;
		if ( x[i] >= 256 )
		{
			x[i] -= 256;
			carry = 1;
		}
		else
			carry = 0;
	}
	return(carry);
}	/* binadjst */


/*
 *	NEGATE
 *		changes x into the 2's complent of its value.
 */
negate(x,size)
short	x[];
short	size;
{
	short i;
	for ( i = 0; i < size; i++ )
		x[i] = 256 - x[i] - 1;	/* take 1's comp */
	bincr(x,size);	/* and make it 2's */
}	/* end negate */

/*
 *	BZERO
 *		Initializes x,which is a short array of integer,
 *		to zeroes.
 */
 bzero(x,size)
 short x[];
 short size;
 {
	short i; 
	for ( i = 0; i < size; i++)
		x[i] = 0;
}	/* end bzero */

/*
 *	STZERO
 *		Check if all the input characters are zeroes.
 *	If so, then return 1 else 0.
 */	
stzero(s)
char *s;
{
	short int flag;
	for ( flag = 1; flag && *s != '\0'; s++)
	{	if (*s != '0') 
			flag = 0;
	}
	return(flag);
}	/* end stzero */


/*
 *	STRIPZR
 *		Strips off trailing zero character from a given string.
 *	Returns number of trailing zeroes being stripped off.
 */
 stripzr(s)
 char s[];
 {
	short i,len;
	len = strlen(s) - 1;
	for ( i = len ; s[i] == '0' ; i-- )
		s[i] = '\0';
	return(len-i);
}

/*
 *	LEADZR
 *		Counts number of leading zero character from a given string
 */
 leadzr(s)
 char s[];
 {
	short i = 0;
	while ( *s == '0' )
	{
		i ++;
		s ++ ;
	}
	return(i);
}	/* end leadzr */

/*
 *	BINCR
 *		Increment binary integer x and returns the carry
 *	out of the msbit of x.
 */
bincr(x,size)
short	x[];
unsigned int	size;
{
	short	i,
		cary;
	for ( i = 0, cary = 1; i < size && cary ; i++ )
	{
		x[i] += cary;
		if ( x[i] >= 256 )
		{
			cary = 1;
			x[i] -= 256;
		}
		else
			cary = 0;
	}
	return (cary);
}	/* end bincr */
	
/*
 *	BINDEC
 *		Decrements the binary integer x and returns the carry
 *	out of the ms bit of x as the function value.
 */
bindec(x,size)
short	x[];
unsigned short	size;
{
	short	i,
		borrow;
	for ( i=0, borrow = 1; i < size && borrow ; i++ )
	{
		x[i] -= borrow;
		if ( x[i] < 0 )
		{
			x[i] += 256;
			borrow = 1;
		}
		else
			borrow = 0;
	}
	return(borrow);

}	/* end bindec */

/*
 *	BINASSGN
 *		Assign binary integer form y to x.
 */
binassgn(x,y,size)
short	x[],y[];
unsigned short	size;
{
	short i;
	for ( i = 0; i< size; i++, x++,y++)
		*x = *y;
}	/* end binassgn */

/*
 *	BIFZERO
 *		Returns 1 if the binary integer x is zero, else
 *	returns 0;
 */
bifzero(x,size)
short x[];
unsigned short size;
{
	short i,zr;
	for ( i= 0, zr = 1; (i < size) && zr ; i++ )
		if ( x[i] != 0 )
			zr = 0;
	return(zr);
}	/* end bifzero */

/*
 *	BINCMPR
 *		Performs unsigned comparison on a and b.
 *	Returns -1 iff a < b
 *	        0  iff a = b
 *	        +1 iff a > b
 */
bincmpr(a,b,size)
short	a[],b[];
unsigned short	size;
{
	int cmp;
	short	i,done;
	done = 0;
	cmp = 0;
	for ( i = size -1; i >= 0 && !(done); i--)
	{
		if ( a[i] > b[i] )
		{	
			done = 1;
			cmp = 1;
		}
		else
		if ( a[i] < b[i] )
		{
			done = 1;
			cmp = -1;
		}
	}	/* end for */
	return (cmp);
}	/* end bincmpr */

/*
 *	BINSCMPR
 *		Performs signed two's comp. comparison on a and b.
 *	Returns -1 iff a < b
 *	        0  iff a = b
 *	        +1 iff a > b
 */
binscmpr(a,b,size)
short	a[],b[];
unsigned short	size;
{
	int cmp;
	short x[10],y[10];
	unsigned short sz;

	/* change to excess-BIT7 notation and do unsigned
	 * comparison
	 */
	binassgn(x,a,size);
	binassgn(y,b,size);

	sz = size -1;

	if ( x[sz] >= BIT7 )
		x[sz] -= BIT7;
	else
		x[sz] += BIT7;

	if ( y[sz] >= BIT7 )
		y[sz] -= BIT7;
	else
		y[sz] += BIT7;
	cmp = bincmpr( x,y,size);
	return(cmp);
}	/* end binscmpr */


/**/

#if FPDEBUG
prfpbin(module,f)
char *module;
FPBIN *f;
{
	int i;
	printf("\n%s:",module);
	printf("\tmantissa ");
	for ( i = MS; i >= LS; i-- )
		printf("%x ",f->m[i]);
	printf("\texponent %x %x\n",f->e[1],f->e[0]);
}
#endif	/*FPDEBUG*/
