#ifndef SYS_PWRF_H
#define SYS_PWRF_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) pwrf.h: version 2.1 created on 5/22/89 at 19:09:47	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)pwrf.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys:pwrf.h	2.1"

/* pwrf.h */
/* 
   contains external definitions and literal definitions for the power fail
   handling code of the kernel
*/

/* 
 * icb.h contains the two interrupt macros ICBSTOP_TAPE and ICBSTOP_IO
 * that will be sent to the disk/tape controllers
 */

#ifndef SYS_ICB_H
#include "sys/icb.h"
#endif /* SYS_ICB_H */


extern char stop_tape_io, power_fail, power_fail_ups;
extern time_t power_fail_time;

#define UPS_PRESENT     8
#define EXT_AC_FAIL     4


#endif /* SYS_PWRF_H */
