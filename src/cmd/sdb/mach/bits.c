/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) bits.c: version 25.1 created on 12/2/91 at 17:15:15	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)bits.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/mach:bits.c	25.1"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
	/*	OLD MOT:bits.c	7.1		*/
/* MC68000 Disassembler */

/* bits.c:	2.7 83/07/01 */
/* 	static char SCCSID[] = "OLD MOT:bits.c	7.1"; */

#include	<stdio.h>
#include	"dis.h"

#define		FPRT2		stderr
#define		BYTE		1 /* values for size parm to eff_add */
#define		WORD		2
#define		LONG		3

#define		SIGNED		1 /* immediate value signed or unsigned */
#define		UNSIGNED	0

int		errlev = 0;	/* Errors encountered during the	*/
				/* disassembly, probably due to being	*/
				/* out of sync.				*/

unsigned short curinstr;	/* for saving first part of instruction
				   when cur2bytes is used to read displ */
unsigned short oldinstr = 0;	/* to save previous instruction for
				   testing that swbeg follows jump	*/
unsigned short	cur2bytes;
extern	int	debugflag;

long	loc;			/* byte location being disassembled */
char	mneu[NLINE];		/* line to be printed if desired */
char	object[NHEX];		/* reserved for future use	*/


extern		convert();
extern int	get2bytes();
extern		printline();
extern unsigned long chkget();

extern void	bit_movep_imm();
extern void	move_byte();
extern void	move_long();
extern void	move_word();
extern void	miscell();
extern void	addq_subq_scc_dbcc();
extern void	bcc_bsr_bra();
extern void	moveq();
extern void	or_div_sbcd();
extern void	sub_subx();
extern void	unassigned();
extern void	cmp_eor();
extern void	and_mul_abcd_exg();
extern void	add_addx();
extern void	shft_rot();

extern void	move_address();
extern char	*eff_add();



char	conv_temp[NHEX];	/* Temporary location for ascii	*/
				/* representation of operands.	*/

char	comp_temp[NHEX];	/* temp for calling compoff	*/

char	size[] = {'b','w','l'};

char	*cond_codes[] =
	{
		"t      ",	"f      ",	"hi     ",	"ls     ",
		"cc     ",	"cs     ",	"ne     ",	"eq     ",
		"vc     ",	"vs     ",	"pl     ",	"mi     ",
		"ge     ",	"lt     ",	"gt     ",	"le     "
	};

char 	*addregs[] = 
	{ "a0","a1","a2","a3","a4","a5","a6","a7" };

dis_dot(ldot, idsp, fmt)
long ldot;
int idsp;
char fmt;      	/* not used */
{
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
		unassigned
	};
#if DEBUG
		if (debugflag)
			fprintf(FPRT2,"Entering dis_dot; ldot=%d\n",ldot);
#endif

		object[0] = '\0';
		loc = ldot;
		mneu[0] = '\0';
		oldinstr = curinstr;
		get2bytes(idsp);
		curinstr = cur2bytes;	/* save bytes in case eff_add changes it									*/
		comp_temp[0] = '\0';
#if DEBUG
		if (debugflag)
			fprintf(FPRT2,"In disdot: code map index is %d idsp is %d\n", HI4OF16(cur2bytes),idsp);
#endif
		(*code_map[HI4OF16(cur2bytes)])(idsp);
		strcat(mneu,comp_temp);	/* if there was any pc rel computation 
					   put it at the end of assembly line */
#if DEBUG
		if (debugflag)
			fprintf(FPRT2,"About to leave dis_dot; loc=%d idsp is %d\n",loc,idsp);
#endif
	return(loc);
}	/* dis_dot() */


void
move_address(idsp)
int idsp;
{
#if DEBUG
		if (debugflag)
			fprintf(FPRT2,"entering move_address : idsp= %d\n",idsp);
#endif
	strcat(mneu,"movea._ ");
	mneu[6] = BIT12(cur2bytes) ? 'w' : 'l';
	strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
		BIT12(cur2bytes) ? WORD : LONG,SIGNED,idsp));
	sprintf(mneu,"%s,%s",mneu,addregs[BITS11_9(curinstr)]);
}

void
bit_movep_imm(idsp)
int idsp;
{ 
	static char *misc_ops[4] = {
			"btst    ",
			"bchg    ",
			"bclr    ",
			"bset    "
		};
	if (BIT8(cur2bytes)) {
		if (BITS5_3(cur2bytes) == 1) {	/* movep */
			strcat(mneu,"movep._ ");
			mneu[6] = BIT6(cur2bytes) ? 'l' : 'w';
			if (BIT7(cur2bytes))
				sprintf(mneu,"%sd%o,%s",mneu,
					BITS11_9(curinstr),
					eff_add(5,BITS2_0(cur2bytes),
						NULL,NULL,idsp));
			else
				sprintf(mneu,"%s%s,d%o",mneu,
					eff_add(5,BITS2_0(cur2bytes),NULL,
						NULL,idsp), BITS11_9(curinstr));
			}
		else {	/* dynamic bit */
			strcat(mneu,misc_ops[BITS7_6(cur2bytes)]);
			sprintf(mneu,"%sd%o,%s",mneu,BITS11_9(curinstr),
				eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
					NULL,NULL,idsp));
			}
		return;
		} /* end if (BIT8(cur2bytes)) */
	switch(BITS11_9(cur2bytes)) {
	char add_temp[16];
	case 0:
		if (BITS7_6(cur2bytes) == 3) {
			fprintf(stderr,"\n ERROR : illegal bit setting for or immediate instruction %x\n",
				cur2bytes);
			errlev++;
			return;
			}
		strcat(mneu,"ori._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(7,4,BITS7_6(cur2bytes) + 1,UNSIGNED,idsp));
		strcat(mneu,",");
		if (BITS5_0(curinstr) == 074) {
			if (BITS7_6(curinstr) == 0)
				strcat(mneu,"ccr");
			else if BITS7_6(curinstr == 1)
				strcat(mneu,"sr");
			else {
				fprintf(stderr,"\n ERROR : illegal bit setting for or immediate instruction %x\n",
					curinstr);
				errlev++;
				return;
				}
			}
		else
			strcat(mneu,eff_add(BITS5_3(curinstr),
				BITS2_0(curinstr),NULL,NULL,idsp));
		return;
	case 1:
		if (BITS7_6(cur2bytes) == 3) {
			fprintf(stderr,"\n ERROR : illegal bit setting for and immediate instruction %x\n",
				cur2bytes);
			errlev++;
			return;
			}
		strcat(mneu,"andi._   ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(7,4,BITS7_6(cur2bytes) + 1,UNSIGNED,idsp));
		strcat(mneu,",");
		if (BITS5_0(curinstr) == 074) {
			if (BITS7_6(curinstr) == 0)
				strcat(mneu,"ccr");
			else if (BITS7_6(curinstr) == 1)
				strcat(mneu,"sr");
			else {
				fprintf(stderr,"\n ERROR : illegal bit setting for and immediate instruction %x\n",
					curinstr);
				errlev++;
				return;
				}
			}
		else
			strcat(mneu,eff_add(BITS5_3(curinstr),
				BITS2_0(curinstr),NULL,NULL,idsp));
		return;
	case 2:
		if ((BITS7_6(cur2bytes) == 3) || (BITS5_0(cur2bytes) == 074)) {
			fprintf(stderr,"\n ERROR : illegal bit settings for subi instruction %x\n",
				cur2bytes);
			errlev++;
			return;
			}
		strcat(mneu,"subi._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(7,4,BITS7_6(cur2bytes) + 1,UNSIGNED,idsp));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS5_3(curinstr),BITS2_0(curinstr),
					NULL,NULL,idsp));
		return;
	case 3:
		if ((BITS7_6(cur2bytes) == 3) || (BITS5_0(cur2bytes) == 074)) {
			fprintf(stderr,"\n ERROR : illegal bit settings for addi instruction %x\n",
				cur2bytes);
			errlev++;
			return;
			}
		strcat(mneu,"addi._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(7,4,BITS7_6(cur2bytes) + 1,UNSIGNED,idsp));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS5_3(curinstr),BITS2_0(curinstr),
					NULL,NULL,idsp));
		return;
	case 4:
			strcat(mneu,misc_ops[BITS7_6(cur2bytes)]);
			strcat(mneu,eff_add(7,4,(BITS7_6(cur2bytes) ==1) ?
						BYTE : WORD,UNSIGNED,idsp));
			strcat(mneu,",");
			strcat(mneu,eff_add(BITS5_3(curinstr),
					BITS2_0(curinstr),NULL,NULL,idsp));
			return;
	case 5:
		if (BITS7_6(cur2bytes) == 3) {
			fprintf(stderr,"\n ERROR : illegal bit setting for exclusive or immediate instruction %x\n",
				cur2bytes);
                        errlev++;
			return;
			}
		strcat(mneu,"eori._   ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(7,4,BITS7_6(cur2bytes) + 1,UNSIGNED,idsp));
		strcat(mneu,",");
		if (BITS5_0(curinstr) == 074) {
			if (BITS7_6(curinstr) == 0)
				strcat(mneu,"ccr");
			else if BITS7_6(curinstr == 1)
				strcat(mneu,"sr");
			else {
				fprintf(stderr,"\n ERROR : illegal bit setting for exclusive or immediate instruction %x\n",
					curinstr);
                                errlev++;
				return;
				}
			}
		else
			strcat(mneu,eff_add(BITS5_3(curinstr),
				BITS2_0(curinstr),NULL,NULL,idsp));
		return;
	case 6:
		if ((BITS7_6(cur2bytes) == 3) || (BITS5_0(cur2bytes) == 074)) {
			fprintf(stderr,"\n ERROR : illegal bit settings for cmpi instruction %x\n",
				cur2bytes);
                        errlev++;
			return;
			}
		strcat(mneu,"cmpi._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		strcpy(add_temp,eff_add(7,4,BITS7_6(cur2bytes)+1,SIGNED,idsp));
		strcat(mneu,eff_add(BITS5_3(curinstr),BITS2_0(curinstr),
					NULL,NULL,idsp));
		strcat(mneu,",");
		strcat(mneu,add_temp);
		return;
	case 7:
	{
		char add_temp[25];
		add_temp[0] = '\0';
		strcat(mneu,"moves._ ");
		mneu[6] = size[BITS7_6(cur2bytes)];
		strcat(add_temp,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			NULL,NULL,idsp));
		get2bytes(idsp);
		if (BIT11(cur2bytes))
			if (BIT15(cur2bytes))
				sprintf(mneu,"%s%s,%s",mneu,
				addregs[BITS14_12(cur2bytes)],add_temp);
			else
				sprintf(mneu,"%sd%o,%s",mneu,
				BITS14_12(cur2bytes),add_temp);
		else
			if (BIT15(cur2bytes))
				sprintf(mneu,"%s%s,%s",mneu,add_temp,
				addregs[BITS14_12(cur2bytes)]);
			else
				sprintf(mneu,"%s%s,d%o",mneu,add_temp,
				BITS14_12(cur2bytes));
		return;
	}
	}
}


void
move_byte(idsp)
int idsp;
{
	if (BITS8_6(cur2bytes) == 0x1L ) {
		fprintf(stderr,"\n ERROR : impossible bit settings on move instruction %x\n",
			cur2bytes);
		errlev++;
		return;
		}
	else {
		strcat(mneu,"move.b  ");
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),BYTE,
			SIGNED,idsp));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS8_6(curinstr),BITS11_9(curinstr),NULL,
			NULL,idsp));
		}
}	/* move_byte()	*/



void
move_long(idsp)
int idsp;
{
#if DEBUG
		if (debugflag)
			fprintf(FPRT2,"entering move_long : idsp= %d\n",idsp);
#endif
	if (BITS8_6(cur2bytes) == 0x1L )
		move_address(idsp);
	else {
		strcat(mneu,"move.l  ");
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),LONG,
			SIGNED,idsp));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS8_6(curinstr),BITS11_9(curinstr),NULL,
			NULL,idsp));
	}
}	/* move_long()	*/

void
move_word(idsp)
int idsp;
{
	if (BITS8_6(cur2bytes) == 0x1L )
		move_address(idsp);
	else {
		strcat(mneu,"move.w  ");
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),WORD,
			SIGNED,idsp));
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS8_6(curinstr),BITS11_9(curinstr),NULL,
			NULL,idsp));
	}
}	/* move_word()	*/



void
miscell(idsp)
int idsp;
{
	if (BIT8(cur2bytes))
	{
		if (!BIT7(cur2bytes))
		{
			fprintf(stderr, "\n ERROR: impossible bit setting for instruction - %x\n",
				cur2bytes);
			errlev++;
			return;
		}
		if (BIT6(cur2bytes))
			sprintf(mneu,"lea.l   %s,%s",eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),LONG,UNSIGNED,idsp),
				addregs[BITS11_9(curinstr)]);
		else
			sprintf(mneu,"chk.w   %s,d%o",
				eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
					WORD,UNSIGNED,idsp),BITS11_9(curinstr));
		return;
	}

	switch (BITS11_9(cur2bytes))
	{
	case 0:
		if (BITS7_6(cur2bytes) == 3) {
			strcat(mneu,"move.w  sr,");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),NULL,NULL,idsp));
			}
		else {
			strcat(mneu,"negx._  ");
			mneu[5] = size[BITS7_6(cur2bytes)];
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),NULL,NULL,idsp));
			}
		return;
	case 1:
		if (BITS7_6(cur2bytes) == 3) 
		{
			strcat(mneu,"move.w  ccr,");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),NULL,NULL,idsp));
			return;
		}
		strcat(mneu,"clr._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			NULL,NULL,idsp));
		return;
	case 2:
		if (BITS7_6(cur2bytes) == 3)
		{
			strcat(mneu,"move.w  ");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),WORD,UNSIGNED,idsp));
			strcat(mneu,",ccr");
			return;
		}
		strcat(mneu,"neg._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			NULL,NULL,idsp));
		return;
	case 3:
		if (BITS7_6(cur2bytes) == 3)
		{
			strcat(mneu,"move.w  ");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),WORD,UNSIGNED,idsp));
			strcat(mneu,",sr");
			return;
		}
		strcat(mneu,"not._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			NULL,NULL,idsp));
		return;
	case 5:
		if (BITS7_6(cur2bytes) == 3)
		{
			if (BITS5_0(cur2bytes) == 074) {
				int i;
				if (BITS15_7(oldinstr) == 0235){
					get2bytes(idsp);
					curinstr = cur2bytes;
					convert(cur2bytes,conv_temp,LEAD);
					sprintf(mneu,"swbeg   #%s",conv_temp);
					for(i=1;i<=curinstr;i++) {
						printline();
						printf("\t");
							/* no need to call line_							nums - just need tab */
						mneu[0] = '\0';
						object[0] = '\0';
						strcat(mneu,eff_add(7,4,WORD,
							SIGNED,idsp));
						}
					return;
					}
				else {
					sprintf(mneu,"illegal ");
					return;
					}
				}
			strcat(mneu,"tas.b   ");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),NULL,NULL,idsp));
			return;
		}
		strcat(mneu,"tst._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			NULL,NULL,idsp));
		return;
	case 4:
		switch(BITS7_6(cur2bytes)) {
		case 0:
			strcat(mneu,"nbcd.b  ");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),NULL,NULL,idsp));
			return;
		case 1:
			if (BITS5_3(cur2bytes)) {
				strcat(mneu,"pea.l   ");
				strcat(mneu,eff_add(BITS5_3(cur2bytes),
					BITS2_0(cur2bytes),NULL,NULL,idsp));
				return;
				}
			else {
				sprintf(mneu,"swap.w  d%o",
					BITS2_0(cur2bytes));
				return;
				}
		case 2:
			if (!BITS5_3(cur2bytes)) {
				sprintf(mneu,"ext.w   d%o",
					BITS2_0(cur2bytes));
				return;
				}
			/* movem falls thru here */
		case 3:
			if (!BITS5_3(cur2bytes)) {
				sprintf(mneu,"ext.l   d%o",
					BITS2_0(cur2bytes));
				return;
				}
			/* movem falls thru here */
		} /* end switch BITS7_6 */
	case 6:
		{
		char add_temp[25];
		add_temp[0] = '\0';
		if (!BIT7(cur2bytes)) {
			fprintf(stderr,"\n ERROR : impossible bit settings for movem.S instruction %x \n",
				cur2bytes);
			errlev++;
			return;
			}
		get2bytes(idsp);
		strcpy(mneu,"movem._ ");
		mneu[6] = BIT6(curinstr) ? 'l' : 'w';
		strcat(add_temp,eff_add(BITS5_3(curinstr),BITS2_0(curinstr),
			NULL,NULL,idsp));
		if(BIT10(curinstr))
			sprintf(mneu,"%s%s,",mneu,add_temp);
		strcat(mneu,"#");
		convert(cur2bytes, conv_temp,LEAD);
		strcat(mneu,conv_temp);
		if (!BIT10(curinstr))
			sprintf(mneu,"%s,%s",mneu,add_temp);
		return;
		}
	case 7:
		switch (BITS7_6(cur2bytes))
		{
		case 0:
			fprintf(stderr,"\n ERROR: impossible bit setting for instruction - %x\n",
				cur2bytes);
			errlev++;
			return;
		case 1:
			switch (BITS5_4(cur2bytes))
			{
			case 0:
				strcat(mneu,"trap    #");
				convert(BITS3_0(cur2bytes),conv_temp,LEAD);
				strcat(mneu,conv_temp);
				return;
			case 1:
				if (BIT3(cur2bytes))
					sprintf(mneu,"unlk    %s",
						addregs[BITS2_0(cur2bytes)]);
				else
				{
					sprintf(mneu,"link    %s,#",
						addregs[BITS2_0(cur2bytes)]);
					get2bytes(idsp);
					if (BIT15(cur2bytes))
						strcat(mneu,"-");
					convert(BIT15(cur2bytes) ?
						-(short)cur2bytes : cur2bytes,
						conv_temp,LEAD);
					strcat(mneu,conv_temp);
				}
				return;
			case 2:
				if (BIT3(cur2bytes))
					sprintf(mneu,"move.l  usp,%s",
						addregs[BITS2_0(cur2bytes)]);
				else
					sprintf(mneu,"move.l  %s,usp",
						addregs[BITS2_0(cur2bytes)]);
				return;
			case 3:
			{
				char add_temp[25];
				static char	*misc_ops[8] =
					{
						"reset",
						"nop",
						"stop    #",
						"rte",
						"rtd     #",
						"rts",
						"trapv",
						"rtr"
					};
                                add_temp[0] = '\0';
				if (BIT3(cur2bytes))
				{
			               strcat(mneu,"movec.l ");
				       get2bytes(idsp);
				       if (BITS2_0(cur2bytes) == 0 &&
				               BITS5_3(cur2bytes) == 0 &&
					       BITS8_6(cur2bytes) == 0 &&
					       BITS11_9(cur2bytes) == 0)
					       strcat(add_temp,"sfc");
					else if (BITS2_0(cur2bytes) == 1 &&
					       BITS5_3(cur2bytes) == 0 &&
					       BITS8_6(cur2bytes) == 0 &&
					       BITS11_9(cur2bytes) == 0)
					       strcat(add_temp,"dfc");
					else if (BITS2_0(cur2bytes) == 0 &&
					       BITS5_3(cur2bytes) == 0 &&
				               BITS8_6(cur2bytes) == 0 &&
					       BITS11_9(cur2bytes) == 4)
					       strcat(add_temp,"usp");
					else if (BITS2_0(cur2bytes) == 1 &&
					       BITS5_3(cur2bytes) == 0  &&
					       BITS8_6(cur2bytes) == 0 &&
					       BITS11_9(cur2bytes) == 4)
					       strcat(add_temp,"vbr");
					else
					       {
					       fprintf(stderr,  "\n%s%s ERROR : impossible bit setting for instruction - %x\n",cur2bytes);
					       errlev++;
					       return;
					       }
                        		if (BIT0(curinstr))
                        			if (BIT15(cur2bytes))
                        				sprintf(mneu,"%s%s,%s",
							mneu,addregs[BITS14_12(cur2bytes)],
							add_temp);
                        			else
                        				sprintf(mneu,"%sd%o,%s",
							mneu,BITS14_12(cur2bytes),
							add_temp);
                        		else
                        			if (BIT15(cur2bytes))
                        				sprintf(mneu,"%s%s,%s",
							mneu,add_temp,
                        				addregs[BITS14_12(cur2bytes)]);
                        			else
                        				sprintf(mneu,"%s%s,d%o",
							mneu,add_temp,
                        				BITS14_12(cur2bytes));
					return;
				}
				strcat(mneu,misc_ops[BITS2_0(cur2bytes)]);
				if (BITS2_0(cur2bytes) == 2)	/* stop	*/
				{
					get2bytes(idsp);
					convert(cur2bytes,conv_temp,LEAD);
					strcat(mneu,conv_temp);
				}
				if (BITS2_0(cur2bytes) == 4)   /* rtd */
				{
					get2bytes(idsp);
					if (BIT15(cur2bytes))
						strcat(mneu,"-");
					convert(BIT15(cur2bytes) ?
						-(short)cur2bytes : cur2bytes,
						conv_temp,LEAD);
					strcat(mneu,conv_temp);
				 }
				return;
			}
			}	/* switch (BITS5_4(cur2bytes))	*/
		case 2:
			strcat(mneu,"jsr     ");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),NULL,NULL,idsp));
			return;
		case 3:
			strcat(mneu,"jmp     ");
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),NULL,NULL,idsp));
			return;
		}	/* switch (BITS7_6(cur2bytes))	*/
	}	/* switch (BITS11_9(cur2bytes))	*/
}	/* miscell() */



void
addq_subq_scc_dbcc(idsp)
int idsp;
{
	if (BITS7_6(cur2bytes) == 3)
		if (BITS5_3(cur2bytes) == 1)
		{
			strcat(mneu,"db");
			strcat(mneu,cond_codes[BITS11_8(cur2bytes)]);
			/* dbCC is one character longer than other CCs,	*/
			/* so null out the ninth byte to keep aligned.	*/
			mneu[8] = '\0';
			sprintf(conv_temp,"d%o,",BITS2_0(cur2bytes));
			strcat(mneu,conv_temp);
			get2bytes(idsp);
			strcat(mneu, "#");
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
		{
			strcat(mneu,"s");
			strcat(mneu,cond_codes[BITS11_8(cur2bytes)]);
			if (BITS11_8(cur2bytes) < 2)
			{
				/* cc is only one character */
				mneu[2] = '.';
				mneu[3] = 'b';
			}
			else
			{
				/* cc is two characters */
				mneu[3] = '.';
				mneu[4] = 'b';
			}
			strcat(mneu,eff_add(BITS5_3(cur2bytes),
				BITS2_0(cur2bytes),NULL,NULL,idsp));
		}
	else
	{
		strcpy(mneu,BIT8(cur2bytes) ? "subq._  #" : "addq._  #");
		mneu[5] = size[BITS7_6(cur2bytes)];
		convert(BITS11_9(cur2bytes) ? BITS11_9(cur2bytes) : 8,
			conv_temp,LEAD);
		strcat(mneu,conv_temp);
		strcat(mneu,",");
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			NULL,NULL,idsp));
	}
}	/* addq_subq_scc_dbcc()	*/



void
bcc_bsr_bra(idsp)
int idsp;
{
	strcpy(mneu,"b");
	if (BITS11_8(cur2bytes) == 1)
		strcat(mneu,"sr     ");
	else if (BITS11_8(cur2bytes) == 0)
		strcat(mneu,"ra     ");
	else
		strcat(mneu,cond_codes[BITS11_8(cur2bytes)]);
	if (LOW8(cur2bytes))
	{
		mneu[3] = '.';
		mneu[4] = 'b';
		convert(BIT7(cur2bytes) ?
			-((short) ( LOW8(cur2bytes) | 0xff00)) :
			LOW8(cur2bytes),conv_temp,LEAD);
		compoff(BIT7(cur2bytes) ? 
			((long)(short) (LOW8(cur2bytes) | 0xff00)) : 
			(long) LOW8(cur2bytes), comp_temp);
		strcat(mneu, "#");
		if (BIT7(cur2bytes))
			strcat(mneu,"-");
	}
	else
	{
		get2bytes(idsp);
		convert(BIT15(cur2bytes) ? -((short) cur2bytes) :
			cur2bytes,conv_temp,LEAD);
		compoff((BIT15(cur2bytes) ? ((long) (short) cur2bytes) :
			(long) cur2bytes) -2, comp_temp);
				/* the -2 above is needed because loc has been 
				   updated when getting the displacement, but 
				   for Motorola the pc is the address of the 
				   extension word */
		strcat(mneu, "#");
		if (BIT15(cur2bytes))
			strcat(mneu,"-");
	}
	strcat(mneu,conv_temp);
}	/* bcc_bsr_bra() */



void
moveq(idsp)
int idsp;
{
/*
	if (BIT8(cur2bytes)) {
		fprintf(stderr,"\n ERROR : Bit 8 is on for move quick instruction %x\n",
			cur2bytes);
		errlev++;
		return;
		}
*/
	strcpy(mneu,"moveq.l #");
	if (BIT7(cur2bytes))
		strcat(mneu,"-");
	convert(BIT7(cur2bytes) ? 
		-(short) (LOW8(cur2bytes) | 0xff00) : 
		LOW8(cur2bytes), conv_temp,LEAD);
	strcat(mneu,conv_temp);
	sprintf(conv_temp,",d%o",BITS11_9(cur2bytes));
	strcat(mneu,conv_temp);
}	/* moveq() */

void
or_div_sbcd(idsp)
int idsp;
{
	if (BITS7_6(cur2bytes) == 3) {	/* divide */
		strcat(mneu,"div_.w  ");
		if (BIT8(cur2bytes))
			mneu[3] = 's';
		else
			mneu[3] = 'u';
		sprintf(mneu,"%s%s,d%o",mneu,
			eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),WORD,
				BIT8(cur2bytes) ? SIGNED : UNSIGNED,idsp),
			BITS11_9(curinstr));
		}
	else if (BITS8_6(cur2bytes) == 4 && BITS5_4(cur2bytes) == 0) {
					/* sbcd */
		if (BIT3(cur2bytes))
			sprintf(mneu,"sbcd.b  -(%s),-(%s)",
				addregs[BITS2_0(cur2bytes)],
				addregs[BITS11_9(cur2bytes)]);
		else
			sprintf(mneu,"sbcd.b  d%o,d%o",
				BITS2_0(cur2bytes),BITS11_9(cur2bytes));
		}
	else {				/* or */
		strcat(mneu,"or._    ");
		mneu[3] = size[BITS7_6(cur2bytes)];
		conv_temp[0] = '\0';
		strcat(conv_temp, eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
				BITS7_6(cur2bytes)+1,UNSIGNED,idsp));
		if (!BIT8(cur2bytes))
			sprintf(mneu, "%s%s,",mneu,conv_temp);
		sprintf(mneu,"%sd%o",mneu,BITS11_9(cur2bytes));
		if (BIT8(cur2bytes))
			sprintf(mneu, "%s,%s",mneu,conv_temp);
		}
}

void
sub_subx(idsp)
int idsp;
{
	if (BITS7_6(cur2bytes) == 3) {	/* suba */
		strcat(mneu,"suba._  ");
		mneu[5] = BIT8(cur2bytes) ? 'l' : 'w';
		sprintf(mneu,"%s%s,%s",mneu,
			eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
				BIT8(cur2bytes) ? LONG : WORD,SIGNED,idsp),
			addregs[BITS11_9(curinstr)]);
		}
	else if (BIT8(cur2bytes) && !BITS5_4(cur2bytes)) { /* subx */
		strcat(mneu,"subx._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		if (BIT3(cur2bytes))
			sprintf (mneu,"%s-(%s),-(%s)",mneu,
				addregs[BITS2_0(cur2bytes)],
				addregs[BITS11_9(cur2bytes)]);
		else 
			sprintf(mneu,"%sd%o,d%o",mneu,BITS2_0(cur2bytes),
				BITS11_9(cur2bytes));
		}
	else {				/* sub */
		strcat(mneu,"sub._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		if (BIT8(cur2bytes))
			sprintf(mneu,"%sd%o,%s",mneu,BITS11_9(curinstr),
				eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
					NULL,NULL,idsp));
		else
			sprintf(mneu,"%s%s,d%o",mneu,
				eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
					BITS7_6(cur2bytes) + 1,UNSIGNED,idsp),
				BITS11_9(curinstr));
		}
}

void
cmp_eor(idsp)
int idsp;
{
	if (BITS7_6(cur2bytes) == 3) {	/* cmpa */
		strcat(mneu,"cmpa._  ");
		mneu[5] = BIT8(cur2bytes) ? 'l' : 'w';
		sprintf(mneu,"%s%s,%s",mneu,addregs[BITS11_9(curinstr)],
			eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
				BIT8(cur2bytes) ? LONG : WORD,SIGNED,idsp));
		}
	else if (BIT8(cur2bytes) && (BITS5_3(cur2bytes) == 1)) { /* cmpm */
		strcat(mneu,"cmpm._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		sprintf(mneu,"%s(%s)+,(%s)+",mneu,
			addregs[BITS11_9(cur2bytes)],
			addregs[BITS2_0(cur2bytes)]);
		}
	else {				/* cmp or eor */
		if (BIT8(cur2bytes))
			strcat(mneu,"eor._   ");
		else
			strcat(mneu,"cmp._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		sprintf(mneu,"%sd%o,%s",mneu,BITS11_9(curinstr),
			eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
				BITS7_6(cur2bytes) +1,BIT8(cur2bytes) ?
				UNSIGNED : SIGNED,idsp));
		}
}

void
and_mul_abcd_exg(idsp)
int idsp;
{
	if(BITS7_6(cur2bytes) == 3) {	/* mul */
		if (BIT8(cur2bytes))
			strcat(mneu,"muls.w  ");
		else
			strcat(mneu,"mulu.w  ");
		sprintf(mneu,"%s%s,d%o",mneu,eff_add(BITS5_3(cur2bytes),
			BITS2_0(cur2bytes),WORD,
			BIT8(cur2bytes) ? SIGNED : UNSIGNED,idsp),
			BITS11_9(curinstr));
		}
	else if(BIT8(cur2bytes) && !BITS5_4(cur2bytes)) { /* abcd or exg */
		if (BITS8_6(cur2bytes) == 4) {	/* abcd */
			strcat(mneu,"abcd.b  ");
			if (BIT3(cur2bytes))
				sprintf(mneu,"%s-(%s),-(%s)",mneu,
					addregs[BITS2_0(cur2bytes)],
					addregs[BITS11_9(cur2bytes)]);
			else
				sprintf(mneu,"%sd%o,d%o",mneu,
					BITS2_0(cur2bytes),BITS11_9(cur2bytes));
			}
		else if (BITS8_3(cur2bytes) == 050)
			sprintf(mneu,"exg     d%o,d%o",BITS11_9(cur2bytes),
				BITS2_0(cur2bytes));
		else if (BITS8_3(cur2bytes) == 051)
			sprintf(mneu,"exg     %s,%s",
				addregs[BITS11_9(cur2bytes)],
				addregs[BITS2_0(cur2bytes)]);
		else if (BITS8_3(cur2bytes) == 061)
			sprintf(mneu,"exg     d%o,%s",BITS11_9(cur2bytes),
				addregs[BITS2_0(cur2bytes)]);
		else {
			fprintf(stderr,"\n ERROR : impossible bit settings for instruction %x\n",
				cur2bytes);
			errlev++;
			return;
			}
		}
	else {	/* and */
		strcat(mneu,"and._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		if (BIT8(cur2bytes))
			sprintf(mneu,"%sd%o,%s",mneu,BITS11_9(curinstr),
				eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
					NULL,NULL,idsp));
		else
			sprintf(mneu,"%s%s,d%o",mneu,
				eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
					BITS7_6(cur2bytes) + 1,UNSIGNED,idsp),
				BITS11_9(curinstr));
		}
}

void
add_addx(idsp)
int idsp;
{
	if (BITS7_6(cur2bytes) == 3) {	/* adda */
		strcat(mneu,"adda._  ");
		mneu[5] = BIT8(cur2bytes) ? 'l' : 'w';
		sprintf(mneu,"%s%s,%s",mneu,
			eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
				BIT8(cur2bytes) ? LONG : WORD,UNSIGNED,idsp),
			addregs[BITS11_9(curinstr)]);
		}
	else if (BIT8(cur2bytes) && !BITS5_4(cur2bytes)) { /* addx */
		strcat(mneu,"addx._  ");
		mneu[5] = size[BITS7_6(cur2bytes)];
		if (BIT3(cur2bytes))
			sprintf (mneu,"%s-(%s),-(%s)",mneu,
				addregs[BITS2_0(cur2bytes)],
				addregs[BITS11_9(cur2bytes)]);
		else 
			sprintf(mneu,"%sd%o,d%o",mneu,BITS2_0(cur2bytes),
				BITS11_9(cur2bytes));
		}
	else {				/* add */
		strcat(mneu,"add._   ");
		mneu[4] = size[BITS7_6(cur2bytes)];
		if (BIT8(cur2bytes))
			sprintf(mneu,"%sd%o,%s",mneu,BITS11_9(curinstr),
				eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
					NULL,NULL,idsp));
		else
			sprintf(mneu,"%s%s,d%o",mneu,
				eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
					BITS7_6(cur2bytes) + 1,UNSIGNED,idsp),
				BITS11_9(curinstr));
		}
}
	
void
shft_rot(idsp)
int idsp;
{
	char	direction;		/* l -- left, r -- right	*/

	direction = BIT8(cur2bytes) ? 'l' : 'r';

	if (BITS7_6(cur2bytes) == 3)	/* Memory rotate.		*/
	{
		if (BIT11(cur2bytes) != 0)
		{
			fprintf(stderr, "\n ERROR: bit 11 of memory shift/rotate instruction must be zero %x\n", 
				cur2bytes);
			errlev++;
			return;
		}

		switch (BITS10_9(cur2bytes))
		{
		case 0:	strcat(mneu,"as_.w   #1,");
			mneu[2] = direction;
			break;
		case 1: strcat(mneu,"ls_.w   #1,");
			mneu[2] = direction;
			break;
		case 2: strcat(mneu,"rox_.w  #1,");
			mneu[3] = direction;
			break;
		case 3: strcat(mneu,"ro_.w   #1,");
			mneu[2] = direction;
			break;
		}
		strcat(mneu,eff_add(BITS5_3(cur2bytes),BITS2_0(cur2bytes),
			NULL,NULL,idsp));
	}
	else				/* Register rotate.		*/
	{

		switch (BITS4_3(cur2bytes))
		{
		case 0:	strcat(mneu,"as_._   ");
			mneu[2] = direction;
			mneu[4] = size[BITS7_6(cur2bytes)];
			break;
		case 1: strcat(mneu,"ls_._   ");
			mneu[2] = direction;
			mneu[4] = size[BITS7_6(cur2bytes)];
			break;
		case 2: strcat(mneu,"rox_._  ");
			mneu[3] = direction;
			mneu[5] = size[BITS7_6(cur2bytes)];
			break;
		case 3: strcat(mneu,"ro_._   ");
			mneu[2] = direction;
			mneu[4] = size[BITS7_6(cur2bytes)];
			break;
		}

		if (BIT5(cur2bytes))
		{
			sprintf(conv_temp,"d%o,d%o",
				BITS11_9(cur2bytes),BITS2_0(cur2bytes));
			strcat(mneu,conv_temp);
		}
		else
		{
			strcat(mneu,"#");
			convert(BITS11_9(cur2bytes) ? BITS11_9(cur2bytes) : 8, 
				conv_temp, LEAD);
			sprintf(conv_temp,"%s,d%o", conv_temp,
				BITS2_0(cur2bytes));
			strcat(mneu,conv_temp);
		}
	}
}	/* shft_rot()	*/



void
unassigned(idsp)
int idsp;
{
	strcpy(mneu,"***ERROR--unassigned op code***");
	errlev++;
	return;
}	/* unassigned() */

char *
eff_add(mode,reg,size,sign,idsp)
int mode,reg,size,idsp;
	/* size will be NULL, BYTE, WORD, or LONG */
short sign;
{
unsigned long fourbytes;
static char address_fld[32];
int indval;
#if DEBUG
		if (debugflag)
			fprintf(FPRT2,"In eff_add : idsp= %d\n",idsp);
#endif
	address_fld[0] = '\0';
	switch(mode)
	{
	case 0:			/* data register direct */
		sprintf(address_fld, "d%o", reg);
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
		get2bytes(idsp);
		if (BIT15(cur2bytes))
			strcat(address_fld, "-");
		indval = ( BIT15(cur2bytes) ? -(short) cur2bytes : cur2bytes );
		sprintf(address_fld,"%s%d(%s)", address_fld, indval, 
			addregs[reg]);
		break;
	case 6:			/* address register indirect with index */
		get2bytes(idsp);
		if (BITS10_8(cur2bytes)) {
			/* these bits must be zero for this address mode */
			fprintf(stderr, "\n ERROR: bits 10-8 of extension word are non-zero %x\n", 
				cur2bytes);
			errlev++;
			break;
			}
		if (BIT7(cur2bytes))
			strcat(address_fld, "-");
		indval = ( BIT7(cur2bytes) ?
			-(short) (LOW8(cur2bytes) | 0xff00) : LOW8(cur2bytes) );
		sprintf(address_fld,"%s%d(%s,%c%o.%c)", 
			address_fld, indval, addregs[reg],
			BIT15(cur2bytes) ? 'a' : 'd' , BITS14_12(cur2bytes),
			BIT11(cur2bytes) ? 'l' : 'w' );
		break;
	case 7:			/* absolute, pc, and immediate */
		switch(reg)		/* reg is not really a register here */
		{
		case 0:		/* absolute short */
			address_fld[0] = '#';
			get2bytes(idsp);
			convert((short) cur2bytes,address_fld+1,LEAD);
			strcat(address_fld,".W");
			break;	
		case 1:		/* absolute long */
			address_fld[0] = '#';
			get2bytes(idsp);
			fourbytes = cur2bytes << 16;
			get2bytes(idsp);
			fourbytes |= cur2bytes;
			convert(fourbytes,address_fld+1,LEAD);
			strcat(address_fld,".L");
			break;
		case 2:		/* pc with displ */
			get2bytes(idsp);
			if (BIT15(cur2bytes))
				strcat(address_fld, "-");
			convert(BIT15(cur2bytes) ?
				-(short) cur2bytes : cur2bytes,
				conv_temp, LEAD);
			sprintf(address_fld, "%s%s(pc)",
				address_fld, conv_temp);
			compoff((BIT15(cur2bytes) ? ((long) (short) cur2bytes) :
				(long) cur2bytes) -2, comp_temp);
				/* the -2 above is needed because loc has been 
				   updated when getting the displacement, but 
				   for Motorola the pc is the address of the 
				   extension word */
			break;
		case 3:		/* pc with index */
			get2bytes(idsp);
			if (BITS10_8(cur2bytes)) {
				/* these bits must be zero for this address mode */
				fprintf(stderr, "\n ERROR: bits 10-8 of extension word are non-zero %x\n", 
					cur2bytes);
				errlev++;
				break;
			}
			if (BIT7(cur2bytes))
				strcat(address_fld, "-");
			convert(BIT7(cur2bytes) ?
				-(short) (LOW8(cur2bytes) | 0xff00) :
				LOW8(cur2bytes),
				conv_temp,LEAD);
			sprintf(address_fld,"%s%s(pc,%c%o.%c)", 
				address_fld, conv_temp, 
				BIT15(cur2bytes) ? 'a' : 'd' ,
				BITS14_12(cur2bytes),
				BIT11(cur2bytes) ? 'l' : 'w' );
			compoff((BIT7(cur2bytes) ? 
				((long)(short) (LOW8(cur2bytes) | 0xff00)) : 
				(long) LOW8(cur2bytes)) - 2, comp_temp);
				/* the -2 above is needed because loc has been 
				   updated when getting the index word, but 
				   for Motorola the pc is the address of
				   this word */
			sprintf(comp_temp,"%s+%c%o",comp_temp,
				BIT15(cur2bytes) ? 'a' : 'd' ,
				BITS14_12(cur2bytes));
			break;
		case 4:		/* immediate */
			switch(size)
			{
			case NULL:
				fprintf(stderr,"\n ERROR: Null size for immediate data %x\n",
					cur2bytes);
				errlev++;
				break;
			case BYTE:
				get2bytes(idsp);
				if (BITS15_8(cur2bytes))
					fprintf(stderr,"\n ERROR: first byte of extension word not zero for byte immediate data address mode %x\n",
						cur2bytes);
				strcat(address_fld, "#");
				if ((sign == SIGNED) && BIT7(cur2bytes))
					strcat(address_fld, "-");
				convert(((sign == SIGNED) && BIT7(cur2bytes)) ?
					-(short) (LOW8(cur2bytes) | 0xff00) :
					LOW8(cur2bytes),
					conv_temp,LEAD);
				strcat(address_fld, conv_temp);
				break;
			case WORD:
				get2bytes(idsp);
				strcat(address_fld, "#");
				if ((sign == SIGNED) && BIT15(cur2bytes))
					strcat(address_fld, "-");
				convert(((sign == SIGNED) && BIT15(cur2bytes)) ?
					-(short) cur2bytes : cur2bytes ,
					conv_temp,LEAD);
				strcat(address_fld, conv_temp);
				break;
			case LONG:
				get2bytes(idsp);
				strcat(address_fld, "#");
				fourbytes = cur2bytes << 16;
				get2bytes(idsp);
				fourbytes |= cur2bytes;
				if ((sign == SIGNED) && HIOF32(fourbytes))
					strcat(address_fld, "-");
				convert(((sign == SIGNED) && HIOF32(fourbytes))?
					-(long) fourbytes : fourbytes,
					conv_temp, LEAD);
				strcat(address_fld, conv_temp);
				break;
			} /* end of size switch */
			break;
		default:
			fprintf(stderr,"\n  ERROR: invalid register number for mode 7\n");
			errlev++;
			break;
		} /* end reg switch */
		break;
	} /* end mode switch */
return(address_fld);
}
/*
 *	compoff (lng, temp)
 *
 *	This routine will compute the location to which control is to be
 *	transferred.  'lng' is the number indicating the jump amount
 *	(already in proper form, meaning masked and negated if necessary)
 *	and 'temp' is a character array which already has the actual
 *	jump amount.  The result computed here will go at the end of 'temp'.
 *	(This is a great routine for people that don't like to compute in
 *	hex arithmetic.)
 */

compoff(lng, temp)
long	lng;
char	*temp;
{
	extern	long	loc;	/* from _extn.c */

	lng += loc;
	sprintf(temp,"%s <$%lx>",temp,lng);
}
/*
 *	convert (num, temp, flag)
 *
 *	Convert "num" to hex leaving the result in the supplied string array.
 *	If  LEAD  is specified, preceed the number with '$' to
 *	indicate the base (used for information going to the mnemonic
 *	printout).  NOLEAD  will be used for all other printing (for
 *	printing the offset, object code, and the second byte in two
 *	byte immediates, displacements, etc.) and will assure that
 *	there are leading zeros.
 */

convert(num,temp,flag)
unsigned	num;
char	temp[];
int	flag;

{
	if (flag == NOLEAD)
		sprintf(temp,"%.4x",num);
	if (flag == LEAD)
		sprintf(temp,"$%x",num);
}
/*
 *	get2bytes(idsp)
 *
 *	This routine will get 2 bytes, print them in the object file
 *	and place the result in 'cur2bytes'.
 */

int
get2bytes(idsp)
int idsp;
{
	char	temp[NCPS+1];
#if DEBUG
	if (debugflag)
		fprintf(FPRT2,"in get2bytes about to call chkget;loc=%d, idsp=%d\n",loc,idsp);
#endif
	cur2bytes =(chkget(loc,idsp) >> 16) ; /* xyzzy possible hibyte/lobyte prob */

	loc += 2;
	convert( (cur2bytes & 0xffff), temp, NOLEAD);
	sprintf(object,"%s%s ",object, temp);
#if DEBUG
	if (debugflag)
		fprintf(FPRT2,"in get2bytes object<%s>\n",object);
#endif
	return;
}
/*
 *	printline ()
 *
 *	Print the disassembled line, consisting of the object code
 *	and the mnemonics.  The breakpointable line number, if any,
 *	has already been printed, and 'object' contains the offset
 *	within the section for the instruction.
 */

printline()

{

	printf( "%-23s%s", object, mneu );	/* to print hex */
}
