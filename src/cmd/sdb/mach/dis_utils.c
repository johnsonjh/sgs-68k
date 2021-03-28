/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) dis_utils.c: version 25.1 created on 12/2/91 at 17:15:19	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)dis_utils.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/mach:dis_utils.c	25.1"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/

/*	OLD MOT:dis_utils.c	6.1		*/
/* SDB Disassembly Utilities.					*/
/*	Utility routines designed to interface SDB with DIS	*/
/*	This file provides those utilities that DIS expects.	*/
/*	The prime ones are getting data (get2bytes,get4butes),	*/
/*	Data conversion to format (convert), and writing out	*/
/*	a dissassembled line (putline).				*/

/*	static char SCCSID[] = "OLD MOT:dis_utils.c	6.1 	85/05/31";*/

#include	<stdio.h>
#include	"dis.h"
#define		FPRT2		stderr

extern	int		debugflag;
extern	char		mneu[];
extern	char		object[];
extern	unsigned short	cur2bytes;
extern	unsigned long	cur4bytes;
extern	long	loc;	/* from _extn.c */

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

	lng += loc;
	sprintf(temp,"%s <$%lx>",temp,lng);
};

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
#if DEBUG && defined(SDB)
	if (debugflag)
		fprintf(FPRT2,"in get2bytes object<%s>\n",object);
#endif
	return;
}
/*
 *	get4bytes(idsp)
 *
 *	This routine will get 4 bytes, print them in the object file
 *	and place the result in 'cur4bytes'.
 */

void
get4bytes(idsp)
int idsp;
{
	char	temp[NCPS+1];
#if DEBUG
	if (debugflag)
		fprintf(FPRT2,"in get4bytes about to call chkget;loc=%d, idsp=%d\n",loc,idsp);	/* dd0 */
#endif
	cur4bytes =chkget(loc,idsp);

	loc += 4;
	convert( cur4bytes, temp, NOLEAD);
	sprintf(object,"%s%s ",object, temp);
#if DEBUG && defined(SDB)
	if (debugflag)
		fprintf(FPRT2,"in get4bytes object<%s>\n",object);
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
