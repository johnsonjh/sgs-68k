#ident	"@(#)_setcoll.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <locale.h>
#include "_locale.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int _setcollate(char *new_collate)
{

	char pathname[128];
	register int ret = -1;
	int pcnt, i;
	char *mycoll;
	char *p, *np;
	short *sp;
	FILE *fd;
	size_t size;

	if ((fd = fopen(_fullocale(new_collate,"LC_COLLATE"),"r")) != NULL) {
	
		if ((mycoll = malloc(_SZ_COLLATE+(5*_SZ_SUBS))) ==  NULL) 
		return -1;
		if ((size = fread(mycoll, _SZ_COLLATE + 5*_SZ_SUBS,1,fd)) != 0)
		{
			/*
			 * Update _colltbl only after everything works.
			 */
			if (size <= (_SZ_COLLATE + 5*_SZ_SUBS)) {
				memcpy((void *) _colltbl,(void *) mycoll, _SZ_COLLATE);
				p = mycoll + _SZ_COLLATE; 
				while ( *p != '\0') 
					p += strlen(p) + 1;
				size = p - &mycoll[_SZ_COLLATE];
				memcpy((void *) _1_to_n,(void *) (mycoll + _SZ_COLLATE), size);
				_1_to_n[size] = '\0'; /* Ensure null after sequence */
				p++; pcnt = 0;
				do {
					/* Avoid odd byte boundaries */
					_2_to_1[pcnt] = *p++ << 8;
					_2_to_1[pcnt] |= *p++;
					pcnt++;
					} 
				while (*p != '\0');
				p++;
				_2_to_1[pcnt] = '\0';

				/* Now move the final array: _val_2_to_1 */

				i = pcnt; pcnt= 0;
				while (i-- != 0)  {
					_val_2_to_1[pcnt] = (*p++ << 8);
					_val_2_to_1[pcnt] |= *p++;
					pcnt++;
				}
 
			}
		}
		free((void *) mycoll);
		fclose(fd);
		return 1;
	};
	return -1;
}
