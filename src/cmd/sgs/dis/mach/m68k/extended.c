#ident	"@(#)extended.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
    1.1	89/06/08 00:39:15 root
	date and time created 89/06/08 00:39:15 by root

*/

/* MC68000 Disassembler */

/* MOT : extended.c	7.1		85/09/18	";*/

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


extern unsigned long	get_disp();


fullformat(regno,ext_word,address_fld,idsp)
int	regno;
int	ext_word;
char	*address_fld;
int	idsp;
{
	int have_index,index_num,index_scale;
	int index_size, id_size;
	int have_base, bd_size;
	unsigned long	outer_disp,base_disp;
	int ind_type;
	char conv_temp[100];

	index_num = BITS15_12(ext_word);
	index_size= BIT11(ext_word);
	index_scale=BITS10_9(ext_word);
	ind_type = BITS2_0(ext_word);

	bd_size = BITS5_4(ext_word);
	base_disp = get_disp( bd_size, idsp );
	
	id_size = BITS1_0(ext_word);
	outer_disp = get_disp( id_size, idsp );
	
	convert(base_disp,conv_temp,LEAD);

	strcat(address_fld,"(");

	if( ind_type != 0 ) strcat(address_fld,"[");

	if( (base_disp!=0) || 			/* non zero base disp */
	   ( (BIT7(ext_word)==1) && ((BIT6(ext_word)==1)||BIT2(ext_word)==1) )
	  )
	  {
		strcat(address_fld,conv_temp);
		strcat(address_fld,opt_size[bd_size]);
		if( (BIT7(ext_word)==0) ||
		    ( (BIT6(ext_word)==0)&&BIT2(ext_word)==0) )
		    strcat(address_fld,",");
	  };

	if ( BIT7(ext_word)==0 )
		strcat(address_fld, reg_names[regno+8]);
	
	if ( (BIT2(ext_word)==1) && ( ind_type != 0 ) )	/* post indexing */
		strcat(address_fld,"]");

	if ( BIT6(ext_word)==0 ) {			/* have index. */
		if((BIT2(ext_word)==1)||(BIT7(ext_word)==0))
			strcat(address_fld,",");
		sprintf(address_fld,"%s%s%s",address_fld,
			reg_names[BITS15_12(ext_word)],
			scale_string[BITS11_9(ext_word)] );
	    };

	if ( (BIT2(ext_word)==0) && ( ind_type != 0 ) )	/* pre indexing */
		strcat(address_fld,"]");

	if ( id_size != 0 && outer_disp!=0 ) {
		convert(outer_disp,conv_temp,LEAD);
		sprintf(address_fld,"%s,%s%s", address_fld,
			conv_temp, opt_size[id_size] );
		}

	sprintf(address_fld,"%s)",address_fld);

}



unsigned long
get_disp(size,idsp)
int	size;
int	idsp;
{
	unsigned long	save_val;

	switch(size)
	{
	case 0:
	case BYTE: return(0);
	case WORD:
		GET2BYTES;
		return(cur2bytes);
	case LONG:
		GET4BYTES;
		return( cur4bytes );
#ifndef SDB
	default:
		fatal("Internal ERROR: get_disp of invalid size");
#endif
	};
}
