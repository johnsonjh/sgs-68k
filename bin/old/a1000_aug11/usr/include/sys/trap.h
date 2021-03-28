#ifndef SYS_TRAP_H
#define SYS_TRAP_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) trap.h: version 2.1 created on 5/22/89 at 19:12:13	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)trap.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#)trap.h	2.1	*/
/* trap.h	6.1 */

#ident	"@(#)uts/head/sys:trap.h	2.1"

/*
 * Trap type values
 */

/*
 *	G R O U P    0    E X C E P T I O N S
 */

#define BUSERR		0	/* bus error */
#define ADDRESS		1	/* address error */

/*
 *	G R O U P    1    E X C E P T I O N S
 */

#define TRACE		2	/* trace */
#define SPURIOUS	3	/* spurious interrupt */
#define AUTO1		4	/* autovector 1 */
#define E_AUTO1		4	/* stray int on autovector 1 */
#define AUTO2		5	/* autovector 2 */
#define E_AUTO2		5	/* stray int on autovector 2 */
#define AUTO3		6	/* autovector 3 */
#define E_AUTO3		6	/* stray int on autovector 3 */
#define AUTO4		7	/* autovector 4 */
#define E_AUTO4		7	/* stray int on autovector 4 */
#define AUTO5		8	/* autovector 5 */
#define E_AUTO5		8	/* stray int on autovector 5 */
#define AUTO6		9	/* autovector 6 */
#define E_AUTO6		9	/* stray int on autovector 6 */
#define AUTO7		10	/* autovector 7 */
#define RANDOM		11	/* random */
#define ILLEGAL		12	/* illegal instruction */
#define PRIV		13	/* privilege violation */

/*
 *	G R O U P    2    E X C E P T I O N S
 */

#define ZERODIV		14	/* zero divide */
#define CHK		15	/* chk instruction */
#define TRAPV		16	/* trapv instruction */
#define TRAP0		17	/* trap 0 instruction */
#define TRAP1		18	/* trap 1 instruction */
#define TRAP2		19	/* trap 2 instruction */
#define TRAP3		20	/* trap 3 instruction */

#define RESCHED		33	/* reschedule request */
#define UNIMPL		34	/* unimplemented instruction */
#define SLAVE_SIG	35	/* slave kernel telling master about signal */
#define FLOAT		36	/* f line floating point trap */
#define COPROC		37	/* coprocessor protocol violation */
#define FORMAT		38	/* stack format error */
#define FPEXCEPT	39	/* fpp generated exception */
#define FPEUNORD	40	/* fpp generated exception */


#endif /* SYS_TRAP_H */
