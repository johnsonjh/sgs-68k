#ident	"@(#)localeconv.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#include <locale.h>
#include <string.h>
#include "_locale.h"

extern struct lconv *localeconv(void)
{
	_setmonetary(_cur_locale[LC_MONETARY]);
	_setnumeric(_cur_locale[LC_NUMERIC]);
	return _monetary_num;
}
