/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) opset.c: version 23.1 created on 11/15/90 at 13:18:46	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)opset.c	23.1	11/15/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/com:opset.c	23.1"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
	/*	OLD MOT:opset.c	6.3		*/

/*
 *	UNIX debugger
 *
 *		Instruction printing routines.
 *		MACHINE DEPENDENT.
 *		3B: dis_dot() in "dis" subdirectory;
 *		    routines take form 3B disassembler
 */

#include "head.h"

#define SYSTAB struct systab
SYSTAB {
	int	argc;
	char	*sname;
};
extern struct systab systab[];
extern STRING	regname[];
extern STRING	fltimm[];

/*	printins() all new to disassemble for 3B-20
 *	use code from 3B disassembler -- see "dis" sub-directory
 *	New arguments; change calls in prvar.c also
 *	calls dis_dot(adr,space,fmt)
 */

printins(fmt,idsp)
char fmt;
{
	struct proct *procp;
	long value;
	unsigned short ins;
	union word word;
	long dis_dot();

#if DEBUG > 1
	if(debugflag)
		fprintf(FPRT2, "printins(fmt=%c, idsp=%d);\n", fmt,idsp);
#endif
	procp = adrtoprocp(dot);
#if DEBUG > 1
	if(debugflag > 0)
		fprintf(FPRT2, "dot=%#lx; pname=%s; paddress=%#lx;\n",
					dot, procp->pname, procp->paddress);
#endif

	/* dot (the location counter) used (but not changed) by dis_dot() */
	value = dis_dot(dot,idsp,fmt);		/* disassemble instr at dot */
	oincr = value - dot;
	printline();			/* print it out */
	/* when not too difficult, print symbolic info (separately) */
	if(fmt == 'i')
		prassym();
#if DEBUG > 1
	if(debugflag)
		fprintf(FPRT2, "dis_dot: dot=%#lx; [oincr=%#x];\n", dot,oincr);
#endif
}

/* prassym(): symbolic printing of disassembled instruction */
static
prassym()
{
	int cnt, regno, jj;
	long value;
	char rnam[9];
	register char *os;
	extern	char	mneu[];		/* in dis/extn.c */

	char *regidx;
	int idxsize;
	int starflg = 0;
	int sign = 0;
	int os_bkup;

	/* depends heavily on format output by disassembler */
	printf("\t[");
	cnt = 0;
	os = mneu;	/* instruction disassembled by dis_dot() */
	while(*os != '\t' && *os != ' ' && *os != '\0')
		os++;		/* skip over instr mneumonic */
	while (*os) {
		idxsize = 0;
		while(*os == '\t' || *os == ',' || *os == ' ')
			os++;
#if DEBUG > 1
		if(debugflag)
			fprintf(FPRT2, "os=%s;\n", os);
#endif
		value = 0;
		regno = -1;
		rnam[0] = '\0';
		os_bkup = 0;
		value = 0;
	top:
		switch (*os) {
		    case '&':	/* immediate value */
			jj = 0;  /* don't print, is in mneu already */ 
			break;
		    			/* absloute value, must match	*/
					/* cvt_hex_lead in the dis file	*/
					/* $PROCESSOR/names.c		*/
		    case '$':
			jj = sscanf(os, "$0x%x", &value);
			if( *os == '.' ) {	/* .b, .w, .l */	/* dd0 */
				os += 2;	/* skip size  */
				break;
			  };
			if( *os != '(' ) break;	/* else we are a prefix */
						/* fall through */

		    case '(':	
			if( (sign==1) || (value!=0) ) {
					/* -(Rn) or value(Rn) */
				jj = sscanf(os, "(%[^)])", rnam);
				break;
			   };
			jj = sscanf(os,"(0x%lx%*[.wlb,]%[^)])", &value, rnam );
			if( jj ==2 ) break;
			regidx = os;
			idxsize = 1;
			while ( (*os!='\0') && (*os++ != ']') ) idxsize++;
			jj = sscanf(os,"[^)])", rnam );
			break;

		    case '%':	/* Rn */
			jj = sscanf(os,"%%%[^ 	,]",rnam);/* regnames doesn't */
							  /* include the % */
			break;

		    case '-':	/* -(Rn) or -number(Rn) */
			os_bkup = 1;
			sign = 1;
			os++;
			goto top;

		    default:	/*  number(Rn), number.s, name  */
			if (*os <= '9' && *os >= '0') {
				jj = sscanf(os, "%ld", &value);
				if (sign) {
					sign = 0;
					value = -value;
				}
				os_bkup++;
				while ((*os != '(') && (*++os != '\0'))
						os_bkup++;
				os_bkup--;
				os++;
				/* jj > 0 only if number and register read */
				jj *= sscanf(os, "(%[^)]", rnam);
			}
			 else if ( *os == '$' ) {
				os++;
				os_bkup++;
				jj = sscanf(os, "%x", &value);
				if (sign) {
					sign = 0;
					value = -value;
				}
				os_bkup++;
				while ((*os != '(') && (*++os != '\0'))
						os_bkup++;
				os_bkup--;
				os++;
				/* jj > 0 only if number and register read */
				jj *= sscanf(os, "(%[^)]", rnam);
			}
			else if (*os <= 'z' && *os >= 'a') {
				if ((jj = sscanf(os, "%[^, \t]", rnam)) == EOF)
					jj = sscanf(os, "%s", rnam);
			}
			else jj = -1;
			break;
		}
		if (*rnam)
			for(jj = 0; regname[jj]; jj++)
				if (eqstr(rnam,regname[jj]))
					regno = jj;
		if(jj > 0) {
			if(cnt++ > 0)
				printf(",");

			if (idxsize) {
				while (idxsize--) {
					printf("%c", *regidx++);
				}
			}
			if (starflg) {
				printf("*");
				starflg = 0;
			}
			jj = psymoff(value, regno, 'i');
		}
		if (jj == (-1)) os -= os_bkup;
		while(*os != '\t' && *os != ',' && *os != ' ' && *os != '\0') {
			if(jj == (-1))
				printf("%c", *os);   /* just as is */
			os++;
		}
	}
	printf("]");

}

/* changed 2nd arg in psymoff():  char **r --> int regno */
static
psymoff(val, regno, fmt)
L_INT val; char fmt; int regno; {
	struct proct *procp;
	register long diff = -1;

#if DEBUG
	if(debug)
		fprintf(FPRT2, "psymoff(val=%#x, regno=%#x, fmt='%c');\n",
				val, regno, fmt);
#endif
	if (fmt == 'i') {
		if (regno == APNO) {   /* parameter ("ap" in regname) */
				diff = adrtolocal((ADDR)val, adrtoprocp(dot));
	if( debug > 1) 
		fprintf(FPRT2, "return from adrtolocal, diff = %d/n", diff);
			} else if (regno == FPNO) {	/* local ("fp" in regname) */
				diff = adrtoparam((ADDR)val, adrtoprocp(dot));
		}
		else if (ISREGVAR(regno)) {
			diff = adrtoregvar((ADDR)regno, adrtoprocp(dot));
		}
		if (regno != -1 && diff < 0)
			return(-1);
		if(diff != -1) {
			printf( "%s", sl_name );
			prdiff(diff);
			return(0);
		}

		if (val < firstdata) {
			if ((procp = adrtoprocp((ADDR) val)) != badproc) {
				prlnoff(procp, val);
				return(0);
			}
		} else {
			if ((diff = adrtoext((ADDR) val)) != -1) {
				printf( "%s", sl_name );
				prdiff(diff);
				return(0);
			}
		}
	}
	prhex(val);
	return(1);
}


prdiff(diff) {
	if (diff) {
		printf("+");
		prhex(diff);
	}
}
