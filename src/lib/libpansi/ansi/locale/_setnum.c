#ident	"@(#)_setnum.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
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

static void _dostringread(char *start, char **mynumeric)
{
	char *ptr;

	ptr = start ;
	while (**mynumeric != '\0') { *ptr++ = **mynumeric++; };
	*ptr++ = '\0'; 
	return;
}

int _setnumeric(char *new_numeric)
{

	register FILE *fd;
	char *mynumeric,*old_mynumeric;
	size_t filelength,size;
	
	if ((fd = fopen(_fullocale(new_numeric,"LC_NUMERIC"),"r")) != NULL) {
		fseek(fd,0L,SEEK_END);
		filelength = (size_t) ftell(fd);
		if ((mynumeric = (char *) malloc(filelength)) ==  NULL) 
			return (-1);
		old_mynumeric=mynumeric;
		if ((size = fread((void *) mynumeric,filelength,1,fd)) != 0)
		{
			/*
			 * Update _numeric only after everything works.
			 */
			 _dostringread(_monetary_num->decimal_point,&mynumeric);
			 _dostringread(_monetary_num->grouping,&mynumeric);
			 _dostringread(_monetary_num->thousands_sep,&mynumeric);
			 if (mynumeric- old_mynumeric > size) 
			 	{ free((void *) old_mynumeric); fclose(fd); return -1; };
		}
		free((void *) old_mynumeric);
		fclose(fd);
		return 1;
	};
	return -1;
}
