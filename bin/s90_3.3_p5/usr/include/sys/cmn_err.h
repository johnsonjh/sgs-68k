#ifndef SYS_CMN_ERR_H
#define SYS_CMN_ERR_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) cmn_err.h: version 24.1 created on 10/28/91 at 18:31:40	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)cmn_err.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/cmn_err.h	10.5"	*/


/* Common error handling severity levels */

#define CE_CONT  0	/* continuation				*/
#define CE_NOTE  1	/* notice				*/
#define CE_WARN	 2	/* warning				*/
#define CE_PANIC 3	/* panic				*/

/*	Codes for where output should go.
*/

#define	PRW_BUF		0x01	/* Output to putbuf.		*/
#define	PRW_CONS	0x02	/* Output to console.		*/

extern short	prt_where;

#endif /* SYS_CMN_ERR_H */
