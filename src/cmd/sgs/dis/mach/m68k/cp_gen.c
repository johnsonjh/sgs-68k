#ident	"@(#)cp_gen.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.2	89/09/26 16:43:57 hossein
	added support for M68040.

    1.1	89/06/08 00:39:14 root
	date and time created 89/06/08 00:39:14 by root

*/


/* MC68020 Disassembler - Generalized Coprocessor Support */

/* MOT: cp_gen.c	7.1		85/09/18 */

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

void
cp_gen(idsp)
int	idsp;
{
    int		    cp_id, cp_cmd;
    unsigned long   val;
    char	    *conv_temp[100];

    cp_id = BITS11_9(curinstr);
    sprintf(mneu,"%x",cp_id);
#ifndef	M68040

    switch( cp_cmd = BITS8_6(curinstr) ) {

    case 0:			/* cpGEN */
	GET2BYTES;
	convert(cur2bytes,conv_temp,LEAD);
	sprintf(mneu,"%sGEN   CpCommand=%s,EA=%s", mneu,
		conv_temp, std_eff_add(NULL,NULL) );
	return;

    case 1:
	GET2BYTES;
	if( BITS5_3(curinstr)==1) {
	    sprintf(mneu,"%sDB%x", mneu, BITS5_0(cur2bytes) );
	    PAD(mneu);
	    sprintf(mneu,"%s%s,", mneu, reg_names[BITS3_0(curinstr)] );
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

	if( BITS5_3(curinstr)==7 ) {
	    sprintf(mneu,"%sTRAP%x", mneu, BITS5_0(cur2bytes) );
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

	sprintf(mneu,"%sS%x", mneu, BITS5_0(cur2bytes) );
	PAD(mneu);
	strcat(mneu,std_eff_add(NULL,NULL) );
	return;

    case 2:
    case 3:				/* cpBcc.w or cpBcc.l */
	sprintf(mneu,"%sB%x%s", mneu, BITS5_0(cur2bytes) );
   
	if( cp_cmd == 2 )	/* word disp */
	{
		strcat(mneu,opt_size[WORD]);
		PAD(mneu);
		GET2BYTES ;
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
	strcat(mneu,"SAVE  ");
	strcat(mneu,std_eff_add(NULL,NULL));
	return;
    
    case 5:
	strcat(mneu,"RESTORE  ");
	strcat(mneu,std_eff_add(NULL,NULL));
	return;

    };
#else	/* M68040 defined	*/
	switch (cp_id) {
	case 2:
	    	if (BIT8(curinstr) == 1) 
		{
			if (BIT6(curinstr) == 1)
				{
				if (BIT5(curinstr) == 1)
					strcpy(mneu, "ptestr");
				else	strcpy(mneu, "ptestw");
				PAD(mneu);
				sprintf(mneu,"%s%s",mneu,
				addregs[REGISTER(curinstr)]);
				}
			else	
			{
			switch (OP_MODE(curinstr)) {
			case 0:
				strcpy(mneu, "pflushn");
				PAD(mneu);
				sprintf(mneu,"%s%s",mneu,
				addregs[REGISTER(curinstr)]);
				break;
			case 1:
				strcpy(mneu, "pflush");
				PAD(mneu);
				sprintf(mneu,"%s%s",mneu,
				addregs[REGISTER(curinstr)]);
				break;
			case 2:
				strcpy(mneu, "pflushan");
				break;
			case 3:
				strcpy(mneu, "pflusha");
				break;
			default:
				/** error condition **/
				break;
				}
			}
		}
		else
		{
			if (BIT5(curinstr) == 1)
			{
				switch (SCOPE(curinstr)) {
				case 1:
					strcpy(mneu, "cpushl");
					PAD(mneu);
					sprintf(mneu,"%s%s,%s",mneu,
					(CACHE(curinstr)==0?"&0":(CACHE(curinstr)==1?"&1":(CACHE(curinstr)==2?"&2":"&3"))),
					addregs[REGISTER(curinstr)]);
					break;
				case 2:
					strcpy(mneu, "cpushp");
					PAD(mneu);
					sprintf(mneu,"%s%s,%s",mneu,
					(CACHE(curinstr)==0?"&0":(CACHE(curinstr)==1?"&1":(CACHE(curinstr)==2?"&2":"&3"))),
					addregs[REGISTER(curinstr)]);
					break;
				case 3:
					strcpy(mneu, "cpusha");
					PAD(mneu);
					sprintf(mneu,"%s%s",mneu,
					(CACHE(curinstr)==0?"&0":(CACHE(curinstr)==1?"&1":(CACHE(curinstr)==2?"&2":"&3"))));
					break;
				default:
					/** error condition **/
					break;
				}
			}
			else	
			{
				switch (SCOPE(curinstr)) {
				case 1:
					strcpy(mneu, "cinvl");
					PAD(mneu);
					sprintf(mneu,"%s%s,%s",mneu,
					(CACHE(curinstr)==0?"&0":(CACHE(curinstr)==1?"&1":(CACHE(curinstr)==2?"&2":"&3"))),
					addregs[REGISTER(curinstr)]);
					break;
				case 2:
					strcpy(mneu, "cinvp");
					PAD(mneu);
					sprintf(mneu,"%s%s,%s",mneu,
					(CACHE(curinstr)==0?"&0":(CACHE(curinstr)==1?"&1":(CACHE(curinstr)==2?"&2":"&3"))),
					addregs[REGISTER(curinstr)]);
					break;
				case 3:
					strcpy(mneu, "cinva");
					PAD(mneu);
					sprintf(mneu,"%s%s",mneu,
					(CACHE(curinstr)==0?"&0":(CACHE(curinstr)==1?"&1":(CACHE(curinstr)==2?"&2":"&3"))));
					break;
				default:
					/** error condition **/
					break;
					}
			}
		}
		return;
	case 3:
		switch (BIT5(curinstr)) {
		case 0:
			strcpy(mneu, "move16");
			PAD(mneu);
			switch (BITS4_3(curinstr)) {
			case 0:
				strcat(mneu,eff_add(3,REGISTER(curinstr),NULL,NULL));
				strcat(mneu,",&");
				GET2BYTES;
				convert(cur2bytes,conv_temp,LEAD);
				strcat(mneu,conv_temp);
				GET2BYTES;
				convert(cur2bytes,conv_temp,NOLEAD);
				strcat(mneu,conv_temp);
				break;
			case 1:
				{
				char * reg_temp;
				reg_temp = eff_add(3,REGISTER(curinstr),NULL,NULL);
				GET2BYTES;
				strcat(mneu,"&");
				convert(cur2bytes,conv_temp,LEAD);
				strcat(mneu,conv_temp);
				GET2BYTES;
				convert(cur2bytes,conv_temp,NOLEAD);
				strcat(mneu,conv_temp);
				strcat(mneu,",");
				strcat(mneu,reg_temp);
				break;
				}
			case 2:
				sprintf(mneu,"%s(%s)",mneu, addregs[REGISTER(curinstr)]);
				strcat(mneu,",&");
				GET2BYTES;
				convert(cur2bytes,conv_temp,LEAD);
				strcat(mneu,conv_temp);
				GET2BYTES;
				convert(cur2bytes,conv_temp,NOLEAD);
				strcat(mneu,conv_temp);
				break;
			case 3:
				{
				GET2BYTES;
				strcat(mneu,"&");
				convert(cur2bytes,conv_temp,LEAD);
				strcat(mneu,conv_temp);
				GET2BYTES;
				convert(cur2bytes,conv_temp,NOLEAD);
				strcat(mneu,conv_temp);
				strcat(mneu,",");
				sprintf(mneu,"%s(%s)",mneu, addregs[REGISTER(curinstr)]);
				break;
				}

			}
			break;
	
		case 1:
			strcpy(mneu, "move16");
			PAD(mneu);
			strcat(mneu,eff_add(3,REGISTER(curinstr),0,0));
			strcat(mneu,",");
			GET2BYTES;
			strcat(mneu,eff_add(3,BITS14_12(cur2bytes),0,0));
			break;
		
		}
		return;
	default:
		/** error condition **/
		return;
		}
#endif
}
