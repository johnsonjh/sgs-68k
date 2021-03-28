#ident	"@(#)time.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.1	89/06/08 00:39:19 root
	date and time created 89/06/08 00:39:19 by root

*/
/* MOT:time.c	6.1		*/


/* This file contains the routine(s) which computes the time for the
   specified operation. This computation is dependent upon the 
   source and destination addressing mode, and in some cases the size
   of the operand.
*/

char *time(instruction, src_op, dst_op, size)

int	instruction;
int	src_op;
int	dst_op;
int	size;
{
   char* result;

   switch(instruction)
      {
   case	T_ABCD:
   case	T_SBCD:
      if (src_op == Dn)
	 result = "4(0/0/0)	4(0/0/0)	5(0/1/0)";
      else if (src_op == AIdecr)
	 result = "14(2/0/1)	16(2/0/1)	17(2/1/1)";
      else
	 {
	 result = "*** Illegal Addressing Mode ***";
	 time_error(result);
	 }
      break;

   case	T_ADD:
   case	T_ADDA:
   case T_SUB:
      if (dst_op == Dn || dst_op == An)
	 {
	 result = "0(0/0/0)	2(0/0/0)	3(0/1/0)";
	 return(sumtime(result, fetch_EA[src_op]));
	 }
      else if (src_op == Dn)
	 {
	 result = "3(0/0/1)	4(0/0/1)	6(0/1/1)";
	 return(sumtime(result, fetch_EA[dst_op]));
	 }
      else
	 {
	 result = "*** Illegal Addressing Mode ***";
	 time_error(result);
	 }
      break;

   case	T_ADDI:
      if (dst_op == Dn)
	 {
	 result = "0(0/0/1)	2(0/0/0)	3(0/1/0)";
	 return(sumtime(result, fetch_imm_EA[src_op]));
	 }
      result = "3(0/0/1)	4(0/0/1)	6(0/1/1)";
      return(sumtime(result, fetch_imm_EA[src-op]));

   case	T_ADDQ:
   case	T_SUBQ:
      if (dst_op == Dn || dst_op == An)
         result = "0(0/0/1)	2(0/0/0)	3(0/1/0)";
      else
	 {
         result = "3(0/0/1)	4(0/0/1)	6(0/1/1)";
	 return(sumtime(result, fetch_EA[dst_op]));
	 }
      break;

   case	T_ADDX:
   case	T_SUBX:
      if (src_op == Dn)
	 result = "2(0/0/0)	2(0/0/0)	3(0/1/0)";
      else if (src_op == AIdecr)
	 result = "10(2/0/1)	12(2/0/1)	13(2/1/1)";
      else
	 {
	 result = "*** Illegal Addressing Mode ***";
	 time_error(result);
	 }
      break;

   case T_AND:
   case T_EOR:
   case T_OR:
      if (dst_op == Dn)
	 {
	 result = "0(0/0/0)	2(0/0/0)	3(0/1/0)";
	 return(sumtime(result, fetch_EA[src_op]));
	 }
      else if (src_op == Dn)
	 {
	 result = "3(0/0/1)	4(0/0/1)	6(0/1/1)";
	 return(sumtime(result, fetch_EA[dst_op]));
	 }
      else
	 {
	 result = "*** Illegal Addressing Mode ***";
	 time_error(result);
	 }
      break;

   case	T_ANDI:
   case T_EORI:
   case T_ORI:
   case T_SUBI:
      if (dst_op == Dn)
         result = "0(0/0/1)	2(0/0/0)	3(0/1/0)";
      else
         result = "3(0/0/1)	4(0/0/1)	6(0/1/1)";
      return(sumtime(result, fetch_imm_EA[dst_op]));

   case	T_ASL:
   case	T_ASR:
   case	T_Bcc:
   case	T_BCHG:
   case	T_BCLR:
   case	T_BFCHG:
   case	T_BFCLR:
   case	T_BFEXTS:
   case	T_BFEXTU:
   case	T_BFFFO:
   case	T_BFINS:
   case	T_BFSET:
   case	T_BFTST:
   case	T_BKPT:
   case	T_BRA:
   case	T_BSET:
   case	T_BSR:
   case	T_BTST:
   case	T_CALLM:
   case	T_CAS:
   case	T_CAS2:
   case	T_CHK:
   case	T_CHK2:
   case	T_CLR:
   case	T_CMP:
   case	T_CMPA:
   case	T_CMPI:
   case	T_CMPM:
   case	T_CMP2:
   case	T_cpBcc:
   case	T_cpDBcc:
   case	T_cpGEN:
   case	T_cpRESTORE:
   case	cpSAVE:
   case	cpScc:
   case	cpTRAPcc:
   case	T_DBcc:
   case	T_DIVS:
   case	T_DIVSL:
   case	T_DIVU:
   case	T_DIVUL:
   case	T_EXG:
   case	T_EXT:
   case	T_EXTB:
   case	T_ILLEGAL:
   case	T_JMP:
   case	T_JSR:
   case	T_LEA:
   case	T_LINK:
   case	T_LINK_L:
   case	T_LSL:
   case T_LSR:
   case	T_MOVE:
   case	T_MOVEA:
   case	T_MOVEC:
   case	T_MOVEI:
   case	T_MOVEM:
   case	T_MOVEP:
   case	T_MOVEQ:
   case	T_MOVES:
   case	T_MULS:
   case	T_MULU:
   case	T_NBCD:
   case	T_NEG:
   case	T_NEGX:
   case	T_NOP:
   case	T_NOT:
   case	T_PACK:
   case	T_PEA:
   case	T_RESET:
   case	T_ROL:
   case	T_ROR:
   case	T_ROXL:
   case	T_ROXR:
   case	T_RTD:
   case	T_RTE:
   case	T_RTM:
   case	T_RTR:
   case	T_RTS:
   case	T_Scc:
   case	T_STOP:
   case	T_SWAP:
   case	T_TAS:
   case	T_TRAP:
   case	T_TRAPcc:
   case	T_TRAPV:
   case	T_TST:
   case	T_UNLK:
   default:
      result = "*** Unknown Instruction ***";
      time_error(result);
      break;
      };
   
   return(result);
}
