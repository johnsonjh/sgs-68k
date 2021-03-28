/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) kdb.h: version 25.1 created on 12/2/91 at 17:42:45	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)kdb.h	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* this is kdb.h, ken's debugging routines and associated structures */

int kdebug=0;

char *db_inop[143]={
"OOPS!" /* 0 my bogus */
,"ERROR" /* 1 */
,"NAME" /* 2 */
,"STRING" /* 3 */
,"ICON" /* 4 */
,"FCON" /* 5 */
,"PLUS" /* 6 */
,"PLUSEQ?" /* 7 */
,"MINUS" /* 8 */
,"MINUSEQ?" /* 9 */
,"OOPS!" /* 10 */
,"MUL" /* 11 */
,"OOPS!" /* 12 */
,"STAR" /* 13 */
,"AND" /* 14 */
,"OOPS!" /* 15 */
,"OOPS!" /* 16 */
,"OR" /* 17 */
,"OOPS!" /* 18 */
,"ER" /* 19 */
,"OOPS!" /* 20 */
,"QUEST" /* 21 */
,"COLON" /* 22 */
,"ANDAND" /* 23 */
,"OROR" /* 24 */
,"ASOP" /* 25 */
,"RELOP" /* 26 */
,"EQUOP" /* 27 */
,"DIVOP" /* 28 */
,"SHIFTOP" /* 29 */
,"INCOP" /* 30 */
,"UNOP" /* 31 */
,"STROP" /* 32 */
,"TYPE" /* 33 */
,"CLASS" /* 34 */
,"STRUCT" /* 35 */
,"RETURN" /* 36 */
,"GOTO" /* 37 */
,"IF" /* 38 */
,"ELSE" /* 39 */
,"SWITCH" /* 40 */
,"BREAK" /* 41 */
,"CONTINUE" /* 42 */
,"WHILE" /* 43 */
,"DO" /* 44 */
,"FOR" /* 45 */
,"DEFAULT" /* 46 */
,"CASE" /* 47 */
,"SIZEOF" /* 48 */
,"ENUM" /* 49 */
,"LP" /* 50 */
,"RP" /* 51 */
,"LC" /* 52 */
,"RC" /* 53 */
,"LB" /* 54 */
,"RB" /* 55 */
,"CM" /* 56 */
,"SM" /* 57 */
,"ASSIGN" /* 58 */
,"ASM or COMOP" /* 59 */
,"DIV" /* 60 */
,"OOPS!" /* 61 */
,"MOD" /* 62 */
,"OOPS!" /* 63 */
,"LS" /* 64 */
,"OOPS!" /* 65 */
,"RS" /* 66 */
,"OOPS!" /* 67 */
,"DOT" /* 68 */
,"STREF" /* 69 */
,"CALL" /* 70 */
,"OOPS!" /* 71 */
,"OOPS!" /* 72 */
,"FORTCALL" /* 73 */
,"OOPS!" /* 74 */
,"OOPS!" /* 75 */
,"NOT" /* 76 */
,"COMPL" /* 77 */
,"INCR" /* 78 */
,"DECR" /* 79 */
,"EQ" /* 80 */
,"NE" /* 81 */
,"LE" /* 82 */
,"LT" /* 83 */
,"GE" /* 84 */
,"GT" /* 85 */
,"ULE" /* 86 */
,"ULT" /* 87 */
,"UGE" /* 88 */
,"UGT" /* 89 */
,"SETBIT" /* 90 */
,"TESTBIT" /* 91 */
,"RESETBIT" /* 92 */
,"ARS" /* 93 */
,"REG" /* 94 */
,"TEMP" /* 95 */
,"CCODES" /* 96 */
,"FREE" /* 97 */
,"STASG" /* 98 */
,"STARG" /* 99 */
,"STCALL" /* 100 */
,"OOPS!" /* 101 */
,"OOPS!" /* 102 */
,"FLD" /* 103 */
,"CONV" /* 104 */
,"PMUL" /* 105 */
,"PDIV" /* 106 */
,"FORCE" /* 107 */
,"GENLAB" /* 108 */
,"CBRANCH" /* 109 */
,"GENBR" /* 110 */
,"CMP" /* 111 */
,"GENUBR" /* 112 */
,"INIT" /* 113 */
,"CAST" /* 114 */
,"FUNARG" /* 115 */
,"VAUTO" /* 116 */
,"VPARAM" /* 117 */
,"RNODE" /* 118 */
,"SNODE" /* 119 */
,"QNODE" /* 120 */
,"UOP0 " /* 121 */
,"UOP1 " /* 122 */
,"UOP2 " /* 123 */
,"UOP3 " /* 124 */
,"UOP4 " /* 125 */
,"UOP5 " /* 126 */
,"UOP6 " /* 127 */
,"UOP7 " /* 128 */
,"UOP8 " /* 129 */
,"UOP9 " /* 130 */
#ifdef IN_LINE
,"INCALL" /* 131 */
#ifdef ITREES
,"PROLOG" /* 132 */
,"EPILOG" /* 133 */
,"SWBEG" /* 134 */
,"SWCASE" /* 135 */
,"SWDEF" /* 136 */
,"SWEND" /* 137 */
#ifdef HLOPTIM
,"EXPRNO" /* 138   beginning of an expression */
,"SBLOCK" /* 139   start of a block */
,"EBLOCK" /* 140 end of a block */
,"MANY" /* 141 */
#ifdef  M68881
,"INTCALL" /* 142 */
#endif
# else
,"MANY" /* 138 */
#ifdef  M68881
,"INTCALL" /* 139 */
#endif
#endif
#else
#ifdef HLOPTIM
,"EXPRNO" /* 132   beginning of an expression */
,"SBLOCK" /* 133   start of a block */
,"EBLOCK" /* 134   end of a block */
,"MANY" /* 135 */
#ifdef  M68881
,"INTCALL" /* 136 */
#endif
#else
,"MANY" /* 132 */
#ifdef  M68881
,"INTCALL" /* 133 */
#endif
#endif
#endif
/**********************/
# else
/**********************/
#ifdef ITREES 
,"PROLOG" /* 131 */
,"EPILOG" /* 132 */
,"SWBEG" /* 133 */
,"SWCASE" /* 134 */
,"SWDEF" /* 135 */
,"SWEND" /* 136 */
#ifdef HLOPTIM
,"EXPRNO" /* 137   beginning of an expression */
,"SBLOCK" /* 138   start of a block */
,"EBLOCK" /* 139   end of a block */
,"MANY" /* 140 */
#ifdef  M68881
,"INTCALL" /* 141 */
#endif
#else 
,"MANY" /* 137 */
#ifdef  M68881
,"INTCALL" /* 138 */
#endif
#endif
#else
#ifdef HLOPTIM
,"EXPRNO" /* 131   beginning of an expression */
,"SBLOCK" /* 132   start of a block */
,"EBLOCK" /* 133   end of a block */
,"MANY" /* 134 */
#ifdef  M68881
,"INTCALL" /* 135 */
#endif
# else
,"MANY" /* 131 */
#ifdef  M68881
,"INTCALL" /* 132 */
#endif
#endif
#endif
/***************************/
#endif
};


char *db_intype[18] = {
"TNULL", /* 0 */
"FARG", /* 1 */
"CHAR", /* 2 */
"SHORT", /* 3 */
"INT", /* 4 */
"LONG", /* 5 */
"FLOAT", /* 6 */
"DOUBLE", /* 7 */
"STRTY", /* 8 */
"UNIONTY",  /* 9 */
"ENUMTY", /* 10 */
"MOETY", /* 11 */
"UCHAR", /* 12 */
"USHORT", /* 13 */
"UNSIGNED", /* 14 */
"ULONG", /* 15 */
"VOID", /* 16 */
"UNDEF" /* 17 */
};
