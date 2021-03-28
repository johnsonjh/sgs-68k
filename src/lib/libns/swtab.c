/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) swtab.c: version 25.1 created on 12/2/91 at 19:43:34	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)swtab.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"libns:swtab.c	1.3"	*/
	
#ident	"@(#)libns:swtab.c	25.1"


#include "nserve.h"
#include "sys/cirmgr.h"
#include "pn.h"

/* these are the indicies into sw_tab.
   note that the orders must match the opcodes */

pntab_t sw_tab[NUMSWENT] = {	"RFV", RF_RF,
				"NSV", RF_NS,
				"RSP", RF_AK 
			   };

/* these are the indicies into du_tab.
   note that the orders must match the opcodes */

pntab_t du_tab[NUMDUENT] = {	"MNT", MNT
			   };
