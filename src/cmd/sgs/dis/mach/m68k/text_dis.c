#ident	"@(#)text_dis.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.2	89/09/26 16:44:50 hossein
	added support for M68040.

    1.1	89/06/08 00:39:18 root
	date and time created 89/06/08 00:39:18 by root

*/
/*MOT : ext_dis.c	7.1		*/

/* MC680X0 Disassembler */
/*	Also works with sdb to provide single line disassembly */

/* MOT : text_dis.c	7.1 	85/09/18" */

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


#ifdef SDB
dis_dot(ldot, idsp, fmt)
long ldot;
int idsp; char fmt;
{
#else
dis_text()
{
	int	idsp=0;
#endif
	short sect_num;
	static void (*code_map[16])() =
	{
		bit_movep_imm,
		move_byte,
		move_long,
		move_word,
		miscell,
		addq_subq_scc_dbcc,
		bcc_bsr_bra,
		moveq,
		or_div_sbcd,
		sub_subx,
		unassigned,
		cmp_eor,
		and_mul_abcd_exg,
		add_addx,
		shft_rot,
		cp_instr_040
	};
#ifdef SDB
					/* note, same code in both cases    */
					/* of this ifdef.  simpler this way */
		object[0] = '\0';
		loc = ldot;
		mneu[0] = '\0';
		oldinstr = curinstr;
		GET2BYTES;
		curinstr = cur2bytes;	/* save bytes in case eff_add changes it									*/
		comp_temp[0] = '\0';
		(*code_map[HI4OF16(cur2bytes)])(idsp);
		strcat(mneu,comp_temp);	/* if there was any pc rel computation 
					   put it at the end of assembly line */
	 return(loc);
#else
	if (Lflag > 0)
		sect_num = sect_name();

	for (loc = scnhdr.s_paddr; setjmp(env),
		((loc-scnhdr.s_paddr) < scnhdr.s_size) && (errlev < MAXERRS); 
		printline())
	{
		if (Lflag > 0)
			looklabel(loc,sect_num);
		line_nums(scnhdr.s_nlnno);
		prt_offset();
		mneu[0] = '\0';
		oldinstr = curinstr;
		GET2BYTES;
		curinstr = cur2bytes;	/* save bytes in case eff_add changes it									*/
		comp_temp[0] = '\0';
		(*code_map[HI4OF16(cur2bytes)])(idsp);
		strcat(mneu,comp_temp);	/* if there was any pc rel computation 
					   put it at the end of assembly line */
	}
#endif
}	/* dis_text() */


void
move_address(idsp)
int	idsp;
{
	strcat(mneu,other_ops[MOVEA]);
	strcat(mneu,opt_size[3-BIT12(curinstr)]);
	PAD(mneu);
	strcat(mneu,std_eff_add( BIT12(curinstr) ? WORD : LONG,SIGNED));
	sprintf(mneu,"%s,%s",mneu,addregs[BITS11_9(curinstr)]);
}

void
bit_movep_imm(idsp)
int	idsp;
{ 
	int	ins_code;
	int	save2bytes;

	if (BIT8(cur2bytes)) {
		if (BITS5_3(cur2bytes) == 1) {	/* movep */
			strcat(mneu,other_ops[MOVEP]);
			strcat(mneu,opt_size[BIT6(cur2bytes)+2]);
			PAD(mneu);
			if (BIT7(cur2bytes)) {
			  strcat(mneu,reg_names[BITS11_9(curinstr)]);
			  strcat(mneu,",");
			  strcat(mneu,eff_add(5,BITS2_0(cur2bytes),NULL,NULL));
			  }
			else {
			  strcat(mneu,eff_add(5,BITS2_0(cur2bytes),NULL,NULL));
			  strcat(mneu,",");
			  strcat(mneu,reg_names[BITS11_9(curinstr)]);
			  }
			}
		else {	/* dynamic bit */
			strcat(mneu,bit_ops[BITS7_6(cur2bytes)]);
			strcat(mneu,reg_names[BITS11_9(curinstr)]);
			strcat(mneu,",");
			strcat(mneu,std_eff_add(NULL,NULL));
			}
		return;
		} /* end if (BIT8(cur2bytes)) */

      if( BITS7_6(curinstr)!=3)
	switch(ins_code=BITS11_9(cur2bytes)) {
	char add_temp[16];
	case 2:
	case 3:
	case 6:
		if (BITS5_0(curinstr) == 074 ) {
			fprintf(stderr,"\n%s%s ERROR : illegal bit setting for immediate instruction %x\n",
				SGS,TOOL,cur2bytes);
			ERROR_ACTION;
			}
	case 0:
	case 1:
	case 5:
		sprintf(mneu,imm_fmt[ins_code],			  /* op  */
		    opt_size[BITS7_6(curinstr)+1],		  /* sz  */
		    eff_add(7,4,BITS7_6(curinstr)+1,UNSIGNED) ); /* IMM */

		if (BITS5_0(curinstr) == 074) {
			if (BITS7_6(curinstr) == 0)
				strcat(mneu,ctl_reg_names[CCR]);
			else if ( BITS7_6(curinstr) == 1 )
				strcat(mneu,ctl_reg_names[SR]);
			else {
				fprintf(stderr,"\n%s%s ERROR : illegal bit setting for immediate instruction %x\n",
					SGS,TOOL,curinstr);
				ERROR_ACTION;
				}
			}
		else
			strcat(mneu,std_eff_add(NULL,NULL));
		return;

	case 4:			/* Static Bit Ops */
		strcat(mneu,bit_ops[BITS7_6(cur2bytes)]);
		strcat(mneu,eff_add(7,4,BYTE,UNSIGNED));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS5_3(curinstr),
				  BITS2_0(curinstr),NULL,NULL));
		return;

	case 7:
		strcat(mneu,other_ops[MOVES]);
		strcat(mneu,opt_size[BITS7_6(cur2bytes)]);
		PAD(mneu);
		GET2BYTES;
		if (BIT11(cur2bytes)) {
			strcat(mneu,reg_names[BITS15_12(cur2bytes)]);
			strcat(mneu,",");
			strcat(mneu,std_eff_add(NULL,NULL));
		   }
		else {
			strcat(mneu,std_eff_add(NULL,NULL));
			strcat(mneu,",");
			strcat(mneu,reg_names[BITS15_12(cur2bytes)]);
		   };
		return;
	}
      else
	switch( ins_code = BITS11_9(curinstr) ) {
	case 0:
	case 1:
	case 2:
				/* CHK2 or CMP2 */
	       
		GET2BYTES;
		save2bytes = cur2bytes;
		sprintf(mneu,chk2_cmp2_fmt[BIT11(save2bytes)],
			     opt_size[BITS10_9(curinstr)+1]);
		if( BIT11(save2bytes) )
		    sprintf(mneu,"%s%s,%s",mneu,
			     std_eff_add(BITS10_9(curinstr)+1,UNSIGNED),
			     reg_names[BITS15_12(save2bytes)]);
		else				/* as's CMPs are reversed */
		    sprintf(mneu,"%s%s,%s",mneu,
			     reg_names[BITS15_12(save2bytes)],
			     std_eff_add(BITS10_9(curinstr)+1,UNSIGNED));
		return;
	case 3:
				/* RTM or CALLM */
		if( BITS5_4(curinstr) == 0 ) {
		    strcat(mneu,other_ops[RTM]);
		    strcat(mneu,std_eff_add(LONG,NULL));
		  }
		else {
		    strcat(mneu,other_ops[CALLM]);
		    strcat(mneu,eff_add(7,4,BYTE,UNSIGNED));
		    strcat(mneu,",");
		    strcat(mneu,std_eff_add(LONG,NULL));
		  };
		return;

	case 4:		/* static bit set case */
		strcat(mneu,bit_ops[BITS7_6(curinstr)]);
		strcat(mneu,eff_add(7,4,BYTE,UNSIGNED));
		strcat(mneu,",");
		strcat(mneu,std_eff_add(NULL,NULL));
		return;

	case 5:
	case 6:
	case 7:		/* CAS or CAS2 */
		GET2BYTES;
		if( BITS14_12(cur2bytes)==0 ) {	/* CAS */
		    sprintf(mneu,cas_cas2_fmt[0],
			opt_size[BITS10_9(curinstr)],
			reg_names[BITS2_0(cur2bytes)],
			reg_names[BITS9_6(cur2bytes)],
			std_eff_add(BITS10_9(curinstr),NULL));
		    }
		else {
		    save2bytes = cur2bytes;
		    GET2BYTES;
		    sprintf(mneu,cas_cas2_fmt[1],
			opt_size[BITS10_9(curinstr)],
			reg_names[BITS2_0(save2bytes)],
			reg_names[BITS2_0(cur2bytes)],
			reg_names[BITS8_6(save2bytes)],
			reg_names[BITS8_6(cur2bytes)],
			reg_names[BITS15_12(save2bytes)],
			reg_names[BITS15_12(cur2bytes)]
			);
		};
		return;
	}
}


void
move_byte(idsp)
int	idsp;
{
	if (BITS8_6(cur2bytes) == 0x1L ) {
		fprintf(stderr,"\n%s%s ERROR : impossible bit settings on move instruction %x\n",
			SGS, TOOL, cur2bytes);
		ERROR_ACTION;
		}
	else {
		strcat(mneu,other_ops[MOVE]);
		strcat(mneu,opt_size[BYTE]);
		PAD(mneu);
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),BYTE,
			SIGNED));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS8_6(curinstr),BITS11_9(curinstr),NULL,
			NULL));
		}
}	/* move_byte()	*/



void
move_long(idsp)
int	idsp;
{
	if (BITS8_6(cur2bytes) == 0x1L )
		move_address();
	else {
		strcat(mneu,other_ops[MOVE]);
		strcat(mneu,opt_size[LONG]);
		PAD(mneu);
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),LONG,
			SIGNED));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS8_6(curinstr),BITS11_9(curinstr),NULL,
			NULL));
	}
}	/* move_long()	*/

void
move_word(idsp)
int	idsp;
{
	if (BITS8_6(cur2bytes) == 0x1L )
		move_address();
	else {
		strcat(mneu,other_ops[MOVE]);
		strcat(mneu,opt_size[WORD]);
		PAD(mneu);
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),WORD,
			SIGNED));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS8_6(curinstr),BITS11_9(curinstr),NULL,
			NULL));
	}
}	/* move_word()	*/



void
miscell(idsp)
int	idsp;
{
	if (BIT8(cur2bytes)==1)
	{
		static int miscell_8_size[] = { LONG, NULL, WORD, LONG };

		if( BITS7_3(curinstr) != 0x18 )
		    sprintf(mneu,miscell_8_fmt[BITS7_6(curinstr)],
			std_eff_add(miscell_8_size[BITS7_6(curinstr)],UNSIGNED),
			BITS11_9(curinstr));
		else		/* EXTB.L */
		    sprintf(mneu,miscell_8_fmt[4],reg_names[BITS3_0(curinstr)]);
		return;
	}

	switch (BITS11_9(cur2bytes))
	{
	case 0:
		if( BITS7_6(curinstr)==3 ) {	
			    strcat(mneu,other_ops[MOVE]);
			    strcat(mneu,opt_size[WORD]);
			    PAD(mneu);
			    strcat(mneu,ctl_reg_names[SR]);
			    strcat(mneu,",");
			    strcat(mneu,std_eff_add(WORD,UNSIGNED));
			    return;
		};	
				/* else fall through */
	case 3:
		if( BITS7_6(curinstr)==3 ) {	
			    strcat(mneu,other_ops[MOVE]);
			    strcat(mneu,opt_size[WORD]);
			    PAD(mneu);
			    strcat(mneu,std_eff_add(WORD,UNSIGNED));
			    strcat(mneu,",");
			    strcat(mneu,ctl_reg_names[SR]);
			    return;
		};	
				/* else fall through */
	case 1:
		if( BITS7_6(curinstr)==3 ) {	
			    strcat(mneu,other_ops[MOVE]);
			    strcat(mneu,opt_size[WORD]);
			    PAD(mneu);
			    strcat(mneu,ctl_reg_names[CCR]);
			    strcat(mneu,",");
			    strcat(mneu,std_eff_add(WORD,UNSIGNED));
			    return;
		};	
				/* else fall through */
	case 2:
		if( BITS7_6(curinstr)==3 ) {	
			    strcat(mneu,other_ops[MOVE]);
			    strcat(mneu,opt_size[WORD]);
			    PAD(mneu);
			    strcat(mneu,std_eff_add(WORD,UNSIGNED));
			    strcat(mneu,",");
			    strcat(mneu,ctl_reg_names[CCR]);
			    return;
		};	
				/* else fall through */
	case 5:
		if( BITS7_6(curinstr)!=3 ) {
			sprintf(mneu,boolean_fmt[BITS11_9(curinstr)],
				opt_size[BITS7_6(curinstr)+1],
				std_eff_add(NULL,NULL));
			return;
		};
		if( BITS5_0(curinstr) == 074 ) {
		    int i;
		    if(BITS15_7(oldinstr) == 0235 ) {
			GET2BYTES;
			curinstr = cur2bytes;
			sprintf(mneu,other_ops[SWBET],imm_string(cur2bytes,UNSIGNED));
			for(i=1;i<=curinstr;i++){
				printline();
				printf("\t");
					/* No need to call line_print */
				mneu[0] = '\0';
#ifdef SDB
				object[0] = '\0';
#else
				prt_offset();
#endif
				strcat(mneu,eff_add(7,4,WORD,SIGNED));
				};
			return;
			}
		    else {
			sprintf(mneu,other_ops[ILLEGAL]);
			return;
			}
	       }		/* BITS5_0 == 074 */
	       strcpy(mneu,other_ops[TAS]);
	       strcat(mneu,std_eff_add(BYTE,UNSIGNED));
	       return;
	case 4:
		switch(BITS7_6(cur2bytes)) {
		case 0:
			if( MODE(curinstr) == 1 ) {  /* link.l */

			    sprintf(mneu,other_ops[LINK_L],
				    addregs[BITS2_0(cur2bytes)]);
			    GET4BYTES;
			    strcat(mneu,imm_string( cur4bytes, SIGNED) );
			    return;
			};
			strcat(mneu,bcd_ops[NBCD]);
			strcat(mneu,std_eff_add(NULL,NULL));
			return;
		case 1:
			switch(BITS5_3(cur2bytes)) {
			case 0:
				sprintf(mneu,other_ops[SWAP_L],
					reg_names[BITS2_0(cur2bytes)]);
				return;

			case 1:
				sprintf(mneu,other_ops[BKPT],
				     imm_string(BITS2_0(cur2bytes),UNSIGNED)
				     );
				return;
			default:
				strcat(mneu,other_ops[PEA]);
				strcat(mneu,std_eff_add(NULL,NULL));
				return;
				
			};
		case 2:
		case 3:
		    if (!BITS5_3(cur2bytes)) {
			     sprintf(mneu,other_ops[EXT],
					opt_size[BITS7_6(cur2bytes)]);
			     strcat(mneu,reg_names[BITS2_0(cur2bytes)]);
			     return;
			     }
		    movem();
		    return;
		} /* end switch BITS7_6 */

	case 6:
		if( BIT7(curinstr)==1 )
		    {
			movem();
			return;
		    };
		GET2BYTES;
		mul_div(BIT6(curinstr),BIT11(cur2bytes),LONG,idsp);
		return;

	case 7:
		switch (BITS7_6(cur2bytes))
		{
		case 0:
			fprintf(stderr,"\n%s%s ERROR: impossible bit setting for instruction - %x\n",
				SGS, TOOL, cur2bytes);
			ERROR_ACTION;
		case 1:
			switch (BITS5_4(cur2bytes))
			{
			case 0:
				strcat(mneu,other_ops[TRAP]);
				convert(BITS3_0(cur2bytes),conv_temp,LEAD);
				strcat(mneu,conv_temp);
				return;
			case 1:
				if (BIT3(cur2bytes))
					sprintf(mneu,other_ops[UNLK],
						addregs[BITS2_0(cur2bytes)]);
				else
				{
					sprintf(mneu,other_ops[LINK],
						addregs[BITS2_0(cur2bytes)]);
					GET2BYTES;
					strcat(mneu,
					       imm_string( (short)cur2bytes,
					   	           SIGNED));
				}
				return;
			case 2:
				strcat(mneu,other_ops[MOVE]);
				strcat(mneu,opt_size[LONG]);
				PAD(mneu);
				if (BIT3(cur2bytes))
					sprintf(mneu,"%s%s,%s",mneu,
						ctl_reg_names[USP],
						addregs[BITS2_0(cur2bytes)]);
				else
					sprintf(mneu,"%s%s,%s",mneu,
						addregs[BITS2_0(cur2bytes)],
						ctl_reg_names[USP]);
				return;
			case 3:
			{
				char add_temp[25];
                                add_temp[0] = '\0';
				if (BIT3(cur2bytes))
				{
				     int ctl_index;

				     strcat(mneu,other_ops[MOVEC]);
				     strcat(mneu,opt_size[LONG]);
				     PAD(mneu);
				     GET2BYTES;
				     ctl_index = BITS11_0(cur2bytes);
				     if(ctl_index>=0x800)
				     {
					if (ctl_index>=0x803) --ctl_index;
				     	ctl_index-=(0x800-9);
				     }
				     if( ctl_index > 16 )
					       {
					       fprintf(stderr,  "\n%s%s ERROR : impossible bit setting for instruction - %x\n",SGS, TOOL, cur2bytes);
					       ERROR_ACTION;
					       }
				     if (BIT0(curinstr)) {
					strcat(mneu,reg_names[BITS15_12(cur2bytes)]);
					strcat(mneu,",");
					strcat(mneu,ctl_reg_names[ctl_index]);
					 }
				     else {
					strcat(mneu,ctl_reg_names[ctl_index]);
					strcat(mneu,",");
					strcat(mneu,reg_names[BITS15_12(cur2bytes)]);
				      	};
				     return;
				}
				strcat(mneu,misc_ops[BITS2_0(cur2bytes)]);
			    	if (BITS2_0(cur2bytes) == 2)    /* stop */
				{
				        GET2BYTES;
				        convert(cur2bytes,conv_temp,LEAD);
				        strcat(mneu,conv_temp);
				}
				if (BITS2_0(cur2bytes) == 4)   /* rtd */
				{
					GET2BYTES;
					strcat(mneu,
					   imm_string((short)cur2bytes,SIGNED));
					strcat(mneu,conv_temp);
				 }
				return;
			}
			}	/* switch (BITS5_4(cur2bytes))	*/
		case 2:
			strcat(mneu,other_ops[JSR]);
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),NULL,NULL));
			return;
		case 3:
			strcat(mneu,other_ops[JMP]);
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),NULL,NULL));
			return;
		}	/* switch (BITS7_6(cur2bytes))	*/
	}	/* switch (BITS11_9(cur2bytes))	*/
}	/* miscell() */

void
mul_div(mul_div,su_type,size,idsp)
int mul_div,su_type,size;
int	idsp;
{
	int	ext_word;
	int	index;

	ext_word = cur2bytes;
	index = (  (BIT10(cur2bytes)<<3) | (mul_div<<2) | (su_type<<1) |
		   ( (size==WORD)?0:1 ));

	strcat(mneu,mul_div_ops[index]);
	strcat(mneu,std_eff_add(size,su_type?SIGNED:UNSIGNED));
	strcat(mneu,",");
	if( (size==LONG) && (BIT10(ext_word)==1) ) {
		strcat(mneu,reg_names[BITS2_0(ext_word)]);
		strcat(mneu,":");
	   };
	if( size==LONG )
	    strcat(mneu,reg_names[BITS14_12(ext_word)]);
	else
	    strcat(mneu,reg_names[BITS11_9(curinstr)]);
	return;
}	/* mul_div */

void
movem(idsp)
int	idsp;
{
       unsigned short mask;		/* gets movem.[wl] mask */
       char add_temp[25];
       add_temp[0] = '\0';
       if (!BIT7(cur2bytes)) {
	       fprintf(stderr,"\n%s%s ERROR : impossible bit settings for movem.S instruction %x \n",
		       SGS, TOOL, cur2bytes);
	       ERROR_ACTION;
	       }
       GET2BYTES;		/* read mask and save */
       mask = cur2bytes;
       strcpy(mneu,other_ops[MOVEM]);
       strcat(mneu,opt_size[BIT6(curinstr)+2]);
       PAD(mneu);
       strcat(add_temp,std_eff_add(NULL,NULL));
       if(BIT10(curinstr))
	       sprintf(mneu,"%s%s,",mneu,add_temp);
       strcat(mneu,imm_char);
       convert(mask, conv_temp,LEAD);
       strcat(mneu,conv_temp);
       if (!BIT10(curinstr))
	       sprintf(mneu,"%s,%s",mneu,add_temp);
       return;
}	/* movem() */

void
addq_subq_scc_dbcc(idsp)
int	idsp;
{
	if (BITS7_6(cur2bytes) == 3)
		if (MODE(cur2bytes) == 1)
		{
			strcat(mneu,other_ops[DBcc]);
			strcat(mneu,cond_codes[BITS11_8(cur2bytes)]);
			PAD(mneu);
			/* dbCC is one character longer than other CCs,	*/
			/* so null out the ninth byte to keep aligned.	*/
			mneu[8] = '\0';
			strcat(mneu,reg_names[BITS2_0(cur2bytes)]);
			strcat(mneu,",");
			GET2BYTES;
			if (BIT15(cur2bytes))
				strcat(mneu,"-");
			convert(BIT15(cur2bytes) ? -(short) cur2bytes : 
				cur2bytes,conv_temp,LEAD);
			strcat(mneu,conv_temp);
			compoff((BIT15(cur2bytes) ? ((long) (short) cur2bytes) :
				(long) cur2bytes) -2, comp_temp);
				/* the -2 above is needed because loc has been 
				   updated when getting the displacement, but 
				   for Motorola the pc is the address of the 
				   extension word */
		}
		else
		if ( MODE(curinstr)==7 && (BITS2_0(curinstr) > 1) ) {
			int sz;
			strcat(mneu,other_ops[Tcc]);
			strcat(mneu,cond_codes[BITS11_8(curinstr)]);
			PAD(mneu);
			sz = BITS2_0(curinstr);
			if(sz==4) return;
			if(sz==3) {
				GET4BYTES;
				strcat(mneu,imm_string(cur4bytes));
				return;
			}
			else
			if(sz==2) {
				GET2BYTES;
				strcat(mneu,imm_string(cur2bytes));
				return;
			}
	       		fprintf(stderr,
			  "\n%s%s ERROR : Bad size of data for TRAPcc %d\n",
			  SGS, TOOL, sz);
			ERROR_ACTION;
		}
		else
		{
			strcat(mneu,other_ops[Scc]);
			strcat(mneu,cond_codes[BITS11_8(cur2bytes)]);
			strcat(mneu,opt_size[BYTE]);
			PAD(mneu);
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),NULL,NULL));
		}
	else
	{
		strcpy(mneu,BIT8(cur2bytes) ? other_ops[SUBQ]:other_ops[ADDQ]);
		strcat(mneu,opt_size[BITS7_6(curinstr)+1]);
		PAD(mneu);
		strcat(mneu,
		      imm_string(BITS11_9(cur2bytes)?BITS11_9(cur2bytes): 8,
				 UNSIGNED));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			NULL,NULL));
	}
}	/* addq_subq_scc_dbcc()	*/



void
bcc_bsr_bra(idsp)
int	idsp;
{
	strcpy(mneu,other_ops[Bcc]);
	if (BITS11_8(cur2bytes) == 1)
		strcpy(mneu,other_ops[BSR]);
	else if (BITS11_8(cur2bytes) == 0)
		strcpy(mneu,other_ops[BRA]);
	else
		strcat(mneu,cond_codes[BITS11_8(cur2bytes)]);
	if ( (LOW8(cur2bytes)!=0) && (LOW8(cur2bytes)!=0xff) )
	{
		strcat(mneu,opt_size[BYTE]);
		PAD(mneu);
		convert(BIT7(cur2bytes) ?
			-((short) ( LOW8(cur2bytes) | 0xff00)) :
			LOW8(cur2bytes),conv_temp,LEAD);
		compoff(BIT7(cur2bytes) ? 
			((long)(short) (LOW8(cur2bytes) | 0xff00)) : 
			(long) LOW8(cur2bytes), comp_temp);
		if (BIT7(cur2bytes))
			strcat(mneu,"-");
	}
	else
	if( LOW8(cur2bytes)==0 )	/* word disp */
	{
		strcat(mneu,opt_size[WORD]);
		PAD(mneu);
		GET2BYTES;
		convert(BIT15(cur2bytes) ? -((short) cur2bytes) :
			cur2bytes,conv_temp,LEAD);
		compoff((BIT15(cur2bytes) ? ((long) (short) cur2bytes) :
			(long) cur2bytes) -2, comp_temp);
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
		compoff( (HIOF32(cur4bytes)?-cur4bytes:cur4bytes)-2, comp_temp);
				/* the -2 above is needed because loc has been 
				   updated when getting the displacement, but 
				   for Motorola the pc is the address of the 
				   extension word */
		if (HIOF32(cur4bytes))
			strcat(mneu,"-");
	}
	strcat(mneu,conv_temp);
}	/* bcc_bsr_bra() */



void
moveq(idsp)
int	idsp;
{
	if (BIT8(cur2bytes)) {
		fprintf(stderr,"\n%s%s ERROR : Bit 8 is on for move quick instruction %x\n",
			SGS,TOOL,cur2bytes);
		ERROR_ACTION;
		};
	strcpy(mneu,other_ops[MOVEQ]);
	strcat(mneu,imm_string( 
		(BIT7(cur2bytes) ? 
		      (short) (LOW8(cur2bytes) | 0xff00) : LOW8(cur2bytes)),
		SIGNED));
	strcat(mneu,",");
	strcat(mneu,reg_names[BITS11_9(cur2bytes)]);
}	/* moveq() */

void
or_div_sbcd(idsp)
int	idsp;
{
	if (BITS7_6(cur2bytes) == 3) {	/* divide */
		mul_div(1,BIT8(cur2bytes),WORD,idsp);
		return;
		}
	if( BITS5_4(curinstr) == 0 && BIT8(curinstr)==1 ) {
							/* not an or */
		strcat(mneu,bcd_ops[BITS7_6(curinstr)]);
		if (BIT3(cur2bytes))
		     sprintf(mneu,"%s-(%s),-(%s)",
			     mneu,
			     addregs[BITS2_0(cur2bytes)],
			     addregs[BITS11_9(cur2bytes)]);
		else
		     sprintf(mneu,"%s%s,%s",
			     mneu,
			     reg_names[BITS2_0(cur2bytes)],
			     reg_names[BITS11_9(cur2bytes)]);
		if( BITS7_6(curinstr) == 0  ) return;
		strcat(mneu,",");
		GET2BYTES;
		strcat(mneu,imm_string(cur2bytes,UNSIGNED));
	}
	else {				/* or */
		unsigned short inst = cur2bytes;	/* remember inst. */
		char operand[32];

		strcat(mneu,other_ops[OR]);
		strcat(mneu,opt_size[BITS7_6(inst)+1]);
		PAD(mneu);
		operand[0] = '\0';
		strcat(operand, eff_add(BITS5_3(inst),BITS2_0(inst),
				BITS7_6(inst)+1,UNSIGNED));
		if (!BIT8(inst))
			sprintf(mneu, "%s%s,",mneu,operand);
		sprintf(mneu,"%s%s",mneu,reg_names[BITS11_9(inst)]);
		if (BIT8(inst))
			sprintf(mneu, "%s,%s",mneu,operand);
		}
}

void
cmp_eor(idsp)
int	idsp;
{
	if (BITS7_6(cur2bytes) == 3) {	/* cmpa */
		strcat(mneu,other_ops[CMPA]);
		strcat(mneu,opt_size[BIT8(curinstr)+2]);
		PAD(mneu);
		sprintf(mneu,"%s%s,%s",mneu,addregs[BITS11_9(curinstr)],
			eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
				BIT8(cur2bytes) ? LONG : WORD,SIGNED));
		}
	else if (BIT8(cur2bytes) && (BITS5_3(cur2bytes) == 1)) { /* cmpm */
		strcat(mneu,other_ops[CMPM]);
		strcat(mneu,opt_size[BITS7_6(curinstr)]);
		PAD(mneu);
		sprintf(mneu,"%s(%s)+,(%s)+",mneu,
			addregs[BITS11_9(cur2bytes)],
			addregs[BITS2_0(cur2bytes)]);
		}
	else {				/* cmp or eor */
		if (BIT8(cur2bytes))
			strcat(mneu,other_ops[EOR]);
		else
			strcat(mneu,other_ops[CMP]);
		strcat(mneu,opt_size[BITS7_6(cur2bytes)+1]);
		PAD(mneu);
		sprintf(mneu,"%s%s,%s",mneu,
			reg_names[BITS11_9(curinstr)],
			eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
				BITS7_6(cur2bytes) +1,BIT8(cur2bytes) ?
				UNSIGNED : SIGNED));
		}
}

void
and_mul_abcd_exg(idsp)
int	idsp;
{
	if(BITS7_6(cur2bytes) == 3) {	/* mul */
		mul_div(0,BIT8(cur2bytes),WORD,idsp);
		return;
		}
	else if(BIT8(cur2bytes) && !BITS5_4(cur2bytes)) { /* abcd or exg */
		if (BITS8_6(cur2bytes) == 4) {	/* abcd */
			strcat(mneu,other_ops[ABCD]);
			if (BIT3(cur2bytes))
				sprintf(mneu,"%s-(%s),-(%s)",mneu,
					addregs[BITS2_0(cur2bytes)],
					addregs[BITS11_9(cur2bytes)]);
			else
				sprintf(mneu,"%s%s,%s",mneu,
					reg_names[BITS2_0(cur2bytes)],
					reg_names[BITS11_9(cur2bytes)]);
			}
		else if (BITS8_3(cur2bytes) == 050)
			sprintf(mneu,other_ops[EXG],
				reg_names[BITS11_9(cur2bytes)],
				reg_names[BITS2_0(cur2bytes)]);
		else if (BITS8_3(cur2bytes) == 051)
			sprintf(mneu,other_ops[EXG],
				addregs[BITS11_9(cur2bytes)],
				addregs[BITS2_0(cur2bytes)]);
		else if (BITS8_3(cur2bytes) == 061)
			sprintf(mneu,other_ops[EXG],
				reg_names[BITS11_9(cur2bytes)],
				addregs[BITS2_0(cur2bytes)]);
		else {
			fprintf(stderr,"\n%s%s ERROR : impossible bit settings for instruction %x\n",
				SGS, TOOL, cur2bytes);
			ERROR_ACTION;
			}
		}
	else {	/* and */
		strcat(mneu,other_ops[AND]);
		strcat(mneu,opt_size[BITS7_6(curinstr)+1]);
		PAD(mneu);
		if (BIT8(cur2bytes))
			sprintf(mneu,"%s%s,%s",mneu,
				reg_names[BITS11_9(curinstr)],
				eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
					NULL,NULL));
		else
			sprintf(mneu,"%s%s,%s",mneu,
				eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
					BITS7_6(cur2bytes) + 1,UNSIGNED),
				reg_names[BITS11_9(curinstr)]);
		}
}

void
sub_subx(idsp)
int	idsp;
{
	opa_opx_op(other_ops[SUB],idsp);
}

void
add_addx(idsp)
int	idsp;
{
	opa_opx_op(other_ops[ADD],idsp);
}

void
opa_opx_op(str,idsp)
char	*str;
int	idsp;
{
	strcpy(mneu,str);	/* "add" or "sub" */
	if (BITS7_6(cur2bytes) == 3) {	/* adda */
		strcat(mneu,other_ops[AMOD]);
		strcat(mneu,opt_size[BIT8(cur2bytes)+2]);	/* .w or .l */
		PAD(mneu);
		sprintf(mneu,"%s%s,%s",mneu,
			eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
				BIT8(cur2bytes) ? LONG : WORD,UNSIGNED),
			addregs[BITS11_9(curinstr)]);
		}
	else if (BIT8(cur2bytes) && !BITS5_4(cur2bytes)) { /* addx */
		strcat(mneu,other_ops[XMOD]);
		strcat(mneu,opt_size[BITS7_6(cur2bytes)+1]);
		PAD(mneu);
		if (BIT3(cur2bytes))
			sprintf (mneu,"%s-(%s),-(%s)",mneu,
				addregs[BITS2_0(cur2bytes)],
				addregs[BITS11_9(cur2bytes)]);
		else 
			sprintf(mneu,"%s%s,%s",mneu,
				reg_names[BITS2_0(cur2bytes)],
				reg_names[BITS11_9(cur2bytes)]);
		}
	else {				/* add */
		strcat(mneu,opt_size[BITS7_6(cur2bytes)+1]);
		PAD(mneu);
		if (BIT8(cur2bytes))
			sprintf(mneu,"%s%s,%s",mneu,
				reg_names[BITS11_9(curinstr)],
				eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
					NULL,NULL));
		else
			sprintf(mneu,"%s%s,%s",mneu,
				eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
					BITS7_6(cur2bytes) + 1,UNSIGNED),
				reg_names[BITS11_9(curinstr)]);
		}
}
	
void
shft_rot(idsp)
int	idsp;
{
	int	op_size, op_index;
	int	ext_word;
	char	*ea,*reg;
	if (BITS7_6(cur2bytes) == 3 && BIT11(cur2bytes)==1 )
	    {					/* Bit Field Op */
		GET2BYTES;
		ext_word = cur2bytes;
		op_index = BITS10_8(curinstr);

		strcat(mneu,bit_field_ops[op_index]);

		if(op_index==7) {
		    strcat(mneu,reg_names[BITS15_12(ext_word)]);
		    strcat(mneu,",");
		};

		strcat(mneu,std_eff_add(NULL,NULL));
		strcat(mneu,"{");
		if( BIT11(ext_word)==0)
			strcat(mneu, imm_string(BITS10_6(ext_word),UNSIGNED));
		   else
			strcat(mneu,reg_names[BITS8_6(ext_word)]);
		strcat(mneu,":");
		if( BIT5(ext_word)==0)
			strcat(mneu, imm_string(BITS5_0(ext_word)==0 ?
					   32 : BITS5_0(ext_word) , UNSIGNED));
		   else
			strcat(mneu,reg_names[BITS2_0(ext_word)]);
		strcat(mneu,"}");

		if( op_index==1 || op_index==3 || op_index==5 ) {
			strcat(mneu,",");
			strcat(mneu,reg_names[BITS15_12(ext_word)]);;
		};
		
		return;
        }

	op_size = BITS7_6(curinstr);
	if( op_size == 3 ) {			/* shift/rotate memory */
		op_size = WORD;
		op_index = BITS10_8(curinstr);
		ea = std_eff_add(WORD,SIGNED);
		reg = imm_string(1,SIGNED);
	    }
	else {					/* shift/rotate register */
		op_size += 1;	/* Adjust to BYTE==1, WORD==2, LONG==3 */
		op_index = (BITS4_3(curinstr)<<1) | BIT8(curinstr);
		ea = reg_names[BITS2_0(curinstr)];
		if( BIT5(curinstr) )
			reg = reg_names[BITS11_9(curinstr)];
		   else
			reg = imm_string(
				   BITS11_9(curinstr) ?
					 BITS11_9(curinstr):8,
				   SIGNED);
	    }
	
	sprintf(mneu,shift_rot_fmt[op_index],opt_size[op_size],reg,ea);
	return;

}	/* shft_rot()	*/

void
cp_instr_040(idsp)
int	idsp;
{
    switch( BITS11_9(curinstr) ) {
    case 1 :
	cp_881(idsp);
	return;
    default:
	cp_gen(idsp);
    };

};



void
unassigned(idsp)
int	idsp;
{
	strcpy(mneu,"***ERROR--unassigned op code***");
	ERROR_ACTION;
}	/* unassigned() */

/*
 *
 *	imm_string(val)
 *		val	value to be encoded
 *			must be sign extended on call if necessary.
 *
 *	returns
 *		string encoded as immediate operand.
 */

char *
imm_string(val,sign)
long val;
int  sign;
{
	static char imm_field[20];

	imm_field[0] = '\0';

	strcat(imm_field,imm_char);
	if( val<0 && (sign ==SIGNED) ) {
		strcat(imm_field,"-");
		val = -val;
	    };
/*	convert(val,conv_temp,LEAD);		*/
/*	strcat(imm_field,conv_temp);		*/
	sprintf(imm_field,"%s%d",imm_field,val);

	return(imm_field);
};

char *
effect_add(mode,reg,size,sign,idsp)
int	mode,reg,size;		 /* size will be NULL, BYTE, WORD, or LONG */
short	sign;
int	idsp;
{
unsigned long fourbytes;
static char address_fld[32];
	address_fld[0] = '\0';
	switch(mode)
	{
	case 0:			/* data register direct */
		sprintf(address_fld, "%s", reg_names[reg]);
		break;
	case 1:			/* address register direct */
		sprintf(address_fld, "%s", addregs[reg]);
		break;
	case 2:			/* address register indirect */
		sprintf(address_fld, "(%s)", addregs[reg]);
		break;
	case 3:			/* address register indirect with post incr */
		sprintf(address_fld, "(%s)+", addregs[reg]);
		break;
	case 4:			/* address register indirect with pre decr */
		sprintf(address_fld, "-(%s)", addregs[reg]);
		break;
	case 5:			/* address register indirect with displ */
		GET2BYTES;
		if (BIT15(cur2bytes))
			strcat(address_fld, "-");
		convert(BIT15(cur2bytes) ? -(short) cur2bytes : cur2bytes,
			conv_temp,LEAD);
		sprintf(address_fld,"%s%s(%s)", address_fld, conv_temp, 
			addregs[reg]);
		break;
	case 6:			/* address register indirect with index */
		GET2BYTES;
#if !m68k
		if (BITS10_8(cur2bytes)) {
			/* these bits must be zero for this address mode */
			fprintf(stderr, "\n%s%s ERROR: bits 10-8 of extension word are non-zero %x\n", 
				SGS, TOOL, cur2bytes);
			ERROR_ACTION;
			}
#else
		if (BIT8(cur2bytes)==1) {
			fullformat(reg,cur2bytes,address_fld);
			break;
			};
#endif
		if (BIT7(cur2bytes))
			strcat(address_fld, "-");
		convert(BIT7(cur2bytes) ?
			-(short) (LOW8(cur2bytes) | 0xff00) : LOW8(cur2bytes),
			conv_temp,LEAD);
		sprintf(address_fld,"%s%s(%s,%s%s)", 
			address_fld, conv_temp, addregs[reg],
			reg_names[ BITS15_12(cur2bytes)],
			scale_string[BITS11_9(cur2bytes)] );
		break;
	case 7:			/* absolute, pc, and immediate */
		switch(reg)		/* reg is not really a register here */
		{
		case 0:		/* absolute short */
			GET2BYTES;
			convert((short) cur2bytes,address_fld,LEAD);
			strcat(address_fld,opt_size[WORD]);
			break;	
		case 1:		/* absolute long */
			GET4BYTES;
			convert(cur4bytes,address_fld,LEAD);
			strcat(address_fld,opt_size[LONG]);
			break;
		case 2:		/* pc with displ */
			GET2BYTES;
			if (BIT15(cur2bytes))
				strcat(address_fld, "-");
			convert(BIT15(cur2bytes) ?
				-(short) cur2bytes : cur2bytes,
				conv_temp, LEAD);
			sprintf(address_fld, "%s%s(%s)",
				address_fld, conv_temp, reg_names[16/*pc*/]);
			compoff((BIT15(cur2bytes) ? ((long) (short) cur2bytes) :
				(long) cur2bytes) -2, comp_temp);
				/* the -2 above is needed because loc has been 
				   updated when getting the displacement, but 
				   for Motorola the pc is the address of the 
				   extension word */
			break;
		case 3:		/* pc with index */
			GET2BYTES;
#if !m68k
			if (BITS10_8(cur2bytes)) {
				/* these bits must be zero for this address mode */
				fprintf(stderr, "\n%s%s ERROR: bits 10-8 of extension word are non-zero %x\n", 
					SGS, TOOL, cur2bytes);
				ERROR_ACTION;
			}
#else
		   	if (BIT8(cur2bytes)==1) {
				fullformat(8/*pc*/,cur2bytes,address_fld,idsp);
				break;
				};
#endif
			if (BIT7(cur2bytes))
				strcat(address_fld, "-");
			convert(BIT7(cur2bytes) ?
				-(short) (LOW8(cur2bytes) | 0xff00) :
				LOW8(cur2bytes),
				conv_temp,LEAD);
			sprintf(address_fld,"%s%s(%s,%s%s)", 
				address_fld, conv_temp, 
				reg_names[16/*pc*/],
				reg_names[BITS15_12(cur2bytes)],
				scale_string[BITS11_9(cur2bytes)] );
			compoff((BIT7(cur2bytes) ? 
				((long)(short) (LOW8(cur2bytes) | 0xff00)) : 
				(long) LOW8(cur2bytes)) - 2, comp_temp);
				/* the -2 above is needed because loc has been 
				   updated when getting the index word, but 
				   for Motorola the pc is the address of
				   this word */
			sprintf(comp_temp,"%s+%s",comp_temp,
				reg_names[BITS15_12(cur2bytes)]);
			break;
		case 4:		/* immediate */
			switch(size)
			{
			case NULL:
				fprintf(stderr,"\n%s%s ERROR: Null size for immediate data %x\n",
					SGS,TOOL, cur2bytes);
				ERROR_ACTION;
				break;
			case FBYTE:
			case BYTE:
				GET2BYTES;
				if (BITS15_8(cur2bytes))
					fprintf(stderr,"\n%s%s ERROR: first byte of extension word not zero for byte immediate data address mode %x\n",
						SGS,TOOL, cur2bytes);
				strcat(address_fld,
				    imm_string(
					((sign == SIGNED) && BIT7(cur2bytes)) ?
						(short) (LOW8(cur2bytes) | 0xff00) : LOW8(cur2bytes),
				        sign) );
				break;
			case FWORD:
			case WORD:
				GET2BYTES;
				strcat(address_fld,
					imm_string((short)cur2bytes,sign));
				break;
			case FLONG:
			case LONG:
				GET4BYTES;
				strcat(address_fld,imm_string(cur4bytes,sign));
				break;
			case FSINGLE:
				GET4BYTES;
				sprintf(address_fld,"%s%s:%.8x",
					address_fld,imm_char,cur4bytes);
				break;
			case FDOUBLE:
				GET4BYTES;
				sprintf(address_fld,"%s%s:%.8x",
					address_fld,imm_char,cur4bytes);
				GET4BYTES;
				sprintf(address_fld,"%s%.8x",
					address_fld,cur4bytes);
				break;
			case FEXTENDED:
			case FPACKED:
				GET4BYTES;
				sprintf(address_fld,"%s%s:%.8x",
					address_fld,imm_char,cur4bytes);
				GET4BYTES;
				sprintf(address_fld,"%s%.8x",
					address_fld,cur4bytes);
				GET4BYTES;
				sprintf(address_fld,"%s%.8x",
					address_fld,cur4bytes);
				break;
			default:
				fprintf(stderr,"\n%s%s  ERROR: unexpected size for immediate value (%d)\n",SGS,TOOL,size);
			} /* end of size switch */
			break;
		default:
			fprintf(stderr,"\n%s%s  ERROR: invalid register number for mode 7\n",SGS,TOOL);
			ERROR_ACTION;
		} /* end reg switch */
		break;
	} /* end mode switch */
return(address_fld);
}

#ifndef SDB
void
recover()
{
	printline();
	if (resync() == FAILURE)
		if (++errlev > MAXERRS)
		{
			fprintf(stderr,"\n%ss: %s: %s:",SGS,TOOL,fname,sname);
			fprintf(stderr," too many errors\n");
			fprintf(stderr,"\tdisassembly terminated\n");
			exit(127);
		}
	longjmp(env, 1); /* the value 1 is not actually used in this program */
}
#endif
