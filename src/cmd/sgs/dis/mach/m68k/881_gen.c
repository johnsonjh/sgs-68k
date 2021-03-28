#ident	"@(#)881_gen.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.1	89/06/08 00:39:13 root
	date and time created 89/06/08 00:39:13 by root

*/


/* MC68020 Disassembler - Generalized Coprocessor Support */

/* MOT:881_gen.c	7.2		85/09/18 */

#include	<stdio.h>
#include	"dis.h"
#include	"names.h"
#ifndef SDB
#	include	<setjmp.h>
#	include	<filehdr.h>
#	include	<ldfcn.h>
#	include	<sgs.h>		/* for definition of SGS */
#	include	<scnhdr.h>
#endif

#include	"disdefs.h"
#define		FSIZE_SHIFT	8

char	*float_cc[] =
	{
		"f",
		"eq",
		"ogt",
		"oge",
		"olt",
		"ole",
		"ogl",
		"or",
		"un",
		"ueq",
		"ugt",
		"uge",
		"ult",
		"ule",
		"neq",
		"t",

		"sf",
		"seq",
		"gt",
		"ge",
		"lt",
		"le",
		"gl",
		"gle",
		"ngle",
		"ngl",
		"nle",
		"nlt",
		"nge",
		"ngt",
		"sneq",
		"st"
	};

char	*fp_regs[] =
	{ "%fp0", "%fp1", "%fp2", "%fp3", "%fp4", "%fp5", "%fp6", "%fp7" };

				/* Changes to fp_op should be reflected here */
#define FMOV	0
#define FCMP	0x38
#define FTEST	0x3a
#define FRESERVED 0x3b
#define	FYTOX	0x3c
#define FMOVM	"fmovm   "
#define FMOVCR	"fmovcr"
				/* sincos requires specail decoding */
#define FSINCOS_first	0x30
#define FSINCOS_last	0x37

char	*fp_op[] =
    {
	"fmov",
	"fint",
	"fsinh",
	"fintrz",
	"fsqrt",
	"???",
	"flognp1",
	"???",
	"fetoxm1",
	"ftanh",
	"fatan",
	"???",
	"fasin",
	"fatanh",
	"fsin",
	"ftan",
	"fetox",	/* 0x10 */
	"ftwotox",
	"ftentox",
	"???",
	"flogn",
	"flog10",
	"flog2",
	"???",
	"fabs",
	"fcosh",
	"fneg",
	"???",
	"facos",
	"fcos",
	"fgetexp",
	"fgetman",
	"fdiv",		/* 0x20 */
	"fmod",
	"fadd",
	"fmul",
	"fsgldiv",
	"frem",
	"fscale",
	"fsglmul",
	"fsub",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"???",
	"fsincos",	/* 0x30 */
	"fsincos",
	"fsincos",
	"fsincos",
	"fsincos",
	"fsincos",
	"fsincos",
	"fsincos",
	"fcmp",		/* 0x38 */
	"???",
			/* changes to fp_ops should change defines */
	"ftest",	/* 0x3a */
	"fRESERVED",	/* 0x3b */
	"fytox",	/* 0x3c */
    };

char	*fp_size[] =
    { ".l", ".s", ".x", ".p", ".w", ".d", ".b", ".p" };

char	*fp_ctl_regs[] =
    {
	"RESERVED",
	"%iaddr",
	"%status",
	"%status,%iaddr",
	"%control",
	"%control,%iaddr",
	"%control,%status",
	"%control,%status,%iaddr"
    };

void
cp_881(idsp)
int	idsp;
{
    int		    cp_id, cp_cmd;
    unsigned long   val;
    char	    *conv_temp[100];
    int		fp_index, rx, ry, cp_command;


    switch( cp_cmd = BITS8_6(curinstr) ) {

    case 0:			/* cpGEN */
	GET2BYTES;

	cp_command = cur2bytes;
	fp_index = BITS6_0(cur2bytes);
	rx = BITS12_10(cur2bytes);
	ry = BITS9_7(cur2bytes);

	switch( BITS15_13(cur2bytes) )  {
	case 0:				/* FP to FP op */
		if( fp_index == 0x7e )
			fp_index = FYTOX;
		else if( fp_index > FRESERVED ) fp_index = FRESERVED;
		strcat(mneu, fp_op[fp_index] );
		strcat(mneu, fp_size[2] );
		PAD(mneu);

		if( fp_index == FCMP ) {		/* swap order of cmp */
			sprintf(mneu,"%s%s,%s",
				mneu,
				fp_regs[ry],
				fp_regs[rx]);
			break;
		};

		strcat(mneu, fp_regs[rx] );

		if( fp_index == FTEST ) break;	/* no dest op for FTEST */

					/* special processing for sincos */
		if( (fp_index >= FSINCOS_first) && (fp_index <= FSINCOS_last) )
		    {
			sprintf(mneu,"%s,%s:%s",
				     mneu,
				     fp_regs[BITS2_0(cp_command)],
				     fp_regs[ry]
			       );
			break;
		    }
		if( rx != ry )
		    sprintf(mneu, "%s,%s", mneu, fp_regs[ry] );
		break;

	case 2:				/* External to FP op  AND */
					/* Move const to FP reg */

		if( rx == 7 ){		/* Move const to FP reg */
		    strcpy(mneu, FMOVCR );
		    PAD(mneu);
		    sprintf(mneu,"%s%s%d,%s", 
			mneu, imm_char, fp_index, fp_regs[ry] );
		    break;
		  };
					/* External to FP op */
		if( fp_index == 0x7e )
			fp_index = FYTOX;
		else if( fp_index > FRESERVED ) fp_index = FRESERVED;

		strcat(mneu, fp_op[fp_index] );
		strcat(mneu, fp_size[rx] );
		PAD(mneu);
		if( fp_index == FCMP ) {		/* swap order of cmp */
			sprintf(mneu,"%s%s,%s",
				mneu,
				fp_regs[ry],
				std_eff_add(rx+FSIZE_SHIFT,UNSIGNED));
			break;
		};

		strcat(mneu,std_eff_add(rx+FSIZE_SHIFT,UNSIGNED));

		if( fp_index == FTEST ) break;	/* no dest op for FTEST */

					/* special processing for sincos */
		if( (fp_index >= FSINCOS_first) && (fp_index <= FSINCOS_last) )
		    {
			sprintf(mneu,"%s,%s:%s",
				     mneu,
				     fp_regs[BITS2_0(cp_command)],
				     fp_regs[ry]
			       );
			break;
		    }

		sprintf(mneu,"%s,%s",mneu, fp_regs[ry]);
		break;
	case 3:				/* Move FP to Dest */
		strcat(mneu, fp_op[FMOV] );
		strcat(mneu, fp_size[rx] );
		PAD(mneu);
		sprintf( mneu, "%s%s,%s", mneu,
			fp_regs[ry],
			std_eff_add( rx+FSIZE_SHIFT , UNSIGNED )
		     );
		if( rx==3 ) 		/* static packed format */
			sprintf(mneu,"%s{%s}",mneu,
				imm_string(BITS6_0(cp_command),UNSIGNED));
		if( rx==7 ) 		/* dynamic packed format */
			sprintf(mneu,"%s{%s}",mneu,
				reg_names[BITS6_4(cp_command)] );
		break;
	case 4:				/* Move/MoveM to system register */
		strcpy(mneu, FMOVM );
		PAD(mneu);
		strcat(mneu, std_eff_add(LONG,NULL) );
		strcat(mneu, "," );
		strcat(mneu, fp_ctl_regs[rx] );	/* possibly multiple resg */
		break;
	case 5:				/* Move/MoveM from system register */
		strcpy(mneu, FMOVM );
		PAD(mneu);
		strcat(mneu, fp_ctl_regs[rx] );	/* possibly multiple resg */
		strcat(mneu, "," );
		strcat(mneu, std_eff_add(LONG,NULL) );
		break;
	case 6:				/* MoveM to FP regs */
		strcpy(mneu, FMOVM );
		PAD(mneu);
		strcat(mneu, std_eff_add(rx+FSIZE_SHIFT,UNSIGNED) );
		strcat(mneu, "," );
		if( BIT11(cp_command) == 0 )
		   strcat(mneu, imm_string(BITS7_0(cp_command),UNSIGNED) );
		else		/* register holds mask */
		   strcat(mneu, eff_add(0,BITS6_4(cp_command),NULL,NULL) );
		break;
	case 7:				/* MoveM from FP regs */
		strcpy(mneu, FMOVM );
		PAD(mneu);
		if( BIT11(cp_command) == 0 )
		   strcat(mneu, imm_string(BITS7_0(cp_command),UNSIGNED) );
		else		/* register holds mask */
		   strcat(mneu, eff_add(0,BITS6_4(cp_command),NULL,NULL));
		strcat(mneu, "," );
		strcat(mneu, std_eff_add(rx+FSIZE_SHIFT,UNSIGNED) );
		break;
	};
	return;

    case 1:
	GET2BYTES;
	if( BITS5_3(curinstr)==1) {
	    sprintf(mneu,"fdb%s", float_cc[BITS5_0(cur2bytes)] );
	    PAD(mneu);
	    sprintf(mneu,"%s%s,", mneu, reg_names[BITS2_0(curinstr)] );
	    GET2BYTES;
	    if (BIT15(cur2bytes)) strcat(mneu,"-");
	    convert(BIT15(cur2bytes) ? -(short) cur2bytes : 
		    cur2bytes,conv_temp,LEAD);
	    strcat(mneu,conv_temp);
	    compoff((BIT15(cur2bytes) ? ((long) (short) cur2bytes) :
		    (long) cur2bytes) -2, conv_temp);
		    /* the -2 above is needed because loc has been 
		       updated when getting the displacement, but 
		       for Motorola the pc is the address of the 
		       extension word */
	    return;
	};

	if( BITS5_3(curinstr)==7 ) {				/* TRAPcc */
	    sprintf(mneu,"ft%s", float_cc[BITS5_0(cur2bytes)] );
	    PAD(mneu);

	    switch( BITS2_0(curinstr) ) {
	    case 0:
	    case 1:
		sprintf(mneu,"ERROR in cpTRAP instruction");
	    case 4:
		return;
	    case 2:
		GET2BYTES;
		val = cur2bytes;
		break;
	    case 3:
		GET4BYTES;
		val = cur4bytes;
		break;
	    };

	    strcat(mneu, imm_string(val,UNSIGNED) );
	    return;

	};

	sprintf(mneu,"fs%s", float_cc[BITS5_0(cur2bytes)] );
	PAD(mneu);
	strcat(mneu,std_eff_add(BYTE,UNSIGNED) );
	return;

    case 2:
    case 3:				/* cpBcc.w or cpBcc.l */
	sprintf(mneu,"fb%s", float_cc[BITS5_0(cur2bytes)] );
   
	if( cp_cmd == 2 )	/* word disp */
	{
		strcat(mneu,opt_size[WORD]);
		PAD(mneu);
		GET2BYTES;
		convert(BIT15(cur2bytes) ? -((short) cur2bytes) :
			cur2bytes,conv_temp,LEAD);
		compoff((BIT15(cur2bytes) ? ((long) (short) cur2bytes) :
			(long) cur2bytes) -2, conv_temp);
				/* the -2 above is needed because loc has been 
				   updated when getting the displacement, but 
				   for Motorola the pc is the address of the 
				   extension word */
		if (BIT15(cur2bytes))
			strcat(mneu,"-");
	}
	else		/* long disp */
	{
		long	fourbytes;

		strcat(mneu,opt_size[LONG]);
		PAD(mneu);

		GET4BYTES;

		convert(  (HIOF32(cur4bytes) ? -cur4bytes : cur4bytes ),
				conv_temp,LEAD);
		compoff( (HIOF32(cur4bytes)?-cur4bytes:cur4bytes)-2, conv_temp);
				/* the -2 above is needed because loc has been 
				   updated when getting the displacement, but 
				   for Motorola the pc is the address of the 
				   extension word */
		if (HIOF32(cur4bytes))
			strcat(mneu,"-");
	}
	strcat(mneu,conv_temp);
	return;

    case 4:
	strcat(mneu,"fsave   ");
	strcat(mneu,std_eff_add(NULL,NULL));
	return;
    
    case 5:
	strcat(mneu,"frestore ");
	strcat(mneu,std_eff_add(NULL,NULL));
	return;

    };
};
