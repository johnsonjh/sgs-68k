#ifndef SYS_PWRF_H
#define SYS_PWRF_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) pwrf.h: version 24.1 created on 10/28/91 at 18:36:54	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)pwrf.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/* pwrf.h */
/* 
   contains external definitions and literal definitions for the power fail
   handling code of the kernel
*/

extern uint stop_tape_io, power_fail;
extern time_t power_fail_time;

#define UPS_PRESENT     8

#endif /* SYS_PWRF_H */
