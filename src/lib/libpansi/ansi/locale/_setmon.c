#ident	"@(#)_setmon.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <locale.h>
#include "_locale.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void _dostringread(char *start, char **mymonetary)
{
	char *ptr;

	ptr = start ;
	while (**mymonetary != '\0') { *ptr++ = **mymonetary++; };
	*ptr++ = '\0'; 
	return;
}

int _setmonetary(char *new_monetary_num)
{

	register FILE *fd;
	char *mymonetary,*old_mymonetary;
	size_t filelength,size;

	if ((fd = fopen(_fullocale(new_monetary_num,"LC_MONETARY"),"r")) != NULL) {
		fseek(fd,0L,SEEK_END);
		filelength = (size_t) ftell(fd);
		if ((mymonetary = (char *) malloc(filelength)) ==  NULL) 
			return (-1); 
		old_mymonetary=mymonetary;
		if ((size = fread((void *) mymonetary,filelength,1,fd)) != 0)
		{
			/*
			 * Update _monetary_num only after everything works.
			 */
			 _dostringread(_monetary_num->currency_symbol,&mymonetary);
			 _monetary_num->frac_digits=*mymonetary++;
			 _dostringread(_monetary_num->int_curr_symbol,&mymonetary);
			 _dostringread(_monetary_num->mon_decimal_point,&mymonetary);
			 _dostringread(_monetary_num->mon_grouping,&mymonetary);
			 _monetary_num->int_frac_digits=*mymonetary++;
			 _dostringread(_monetary_num->mon_thousands_sep,&mymonetary);
			 _monetary_num->n_cs_precedes=*mymonetary++;
			 _monetary_num->n_sep_by_space=*mymonetary++;
			 _monetary_num->n_sign_posn=*mymonetary++;
			 _dostringread(_monetary_num->negative_sign,&mymonetary);
			 _monetary_num->p_cs_precedes=*mymonetary++;
			 _monetary_num->p_sep_by_space=*mymonetary++;
			 _monetary_num->p_sign_posn=*mymonetary++;
			 _dostringread(_monetary_num->positive_sign,&mymonetary);
			 if (mymonetary- old_mymonetary > size) 
			 	{ free((void *) old_mymonetary); fclose(fd); return -1; };
		}
		free((void *) old_mymonetary);
		fclose(fd);
		return 1;
	};
	return -1;
}
