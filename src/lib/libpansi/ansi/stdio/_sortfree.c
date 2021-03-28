/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)_sortfree.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#include "stdio.h"
#include "_stdio.h"

#ifdef DYNAMIC

extern void _sortfree (FILE *iop)
{
	FILE *tmp;

	tmp = _freeiob;
	if (tmp == NULL) {
		_freeiob = iop;
		iop->_prev = NULL;
                iop->_next = NULL;
                return;
        }
        while (iop > tmp) {
                if (tmp->_next != NULL) {
                        tmp = tmp->_next;
                } else {
                        break;
                }
        }
        if (tmp == _freeiob) {
                tmp->_prev = iop;
                iop->_next = tmp;
                iop->_prev = NULL;
		_freeiob = iop;
        } else {
                if ((iop > tmp) && (tmp->_next == NULL)) {
                        tmp->_next = iop;
                        iop->_prev = tmp;
                        iop->_next = NULL;
                } else {
                        tmp->_prev->_next = iop;
                        iop->_prev = tmp->_prev;
                        tmp->_prev = iop;
                        iop->_next = tmp;
                }
        }
}
#endif /* DYNAMIC */
