#ifndef SYS_SYSARIX_H
#define SYS_SYSARIX_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sysarix.h: version 2.1 created on 5/22/89 at 19:11:39	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sysarix.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * Commands for sysarix system call:  sysarix(cmd, arg1, arg2, ... argN);
 */

#define SETNAME		0	/* rename the system			*/
#define	ARIXSNAME	0	/* rename the system			*/
/* arg1 = pointer to null terminated string that will fit (including the null)
 *	  within the nodename field of struct utsname (see uname(2)), returns
 *	  0 if nodename was changed, else -1
 */
#define	ARIXSWPI	1	/* General swap interface		*/
/* arg1 = pointer to struct swapint (see sys/swap.h for details)
 *	Subcommands:
 *	SI_LIST:  copies current swaptab into provided array, returns 0 if ok
 *	SI_ADD:   adds the swap device, returns swap index (0 to 15) or -1
 *	SI_DEL:   removes the swap device, returns 0 on success, else -1
 */
#define	ARIXDUMP	2	/* Generate system dump			*/
/* arg1 = integer dump flag
 *	Cause a kernel core image dump to either a floppy, tape or
 *	winchester's swap area.  Argument digit 1 to do_dump tells
 *	it to actually do a dump, otherwise it will just fake thru
 *	the user interface and drop down to monitor without dumping.
 */
#define POWERMARG	3	/* Power margin the power supply - S90 only */
#define ARIXPMARG	3	/* Power margin the power supply - S90 only */
/*
 * System 90 specific
 * POWERMARG system call has two arguments:
 *	arg1 = power supply number (0..4)
 *	arg2 = command to power supply
 *		0 - turn off margining, 1 - margin high, 2 - margin low
 */
#define	ARIXPHMAT	4	/* Physical memory address space attach	*/
/* Attach physical memory region
 *	arg1 = physical address
 *	arg2 = virtual address
 *	arg3 = size
 *	arg4 = flag
 */
#define	ARIXPHMDT	5	/* Physical memory address space dettach*/
/* Detach physical memory region
 *	arg1 = virtual address
 */
#define	ARIXGETHZ	6	/* Get current HZ value			*/
/* Returns the number of clock ticks per second (same as HZ in sys/param.h)
 */
#define	ARIXGETLA	7	/* Get current loadavg table		*/
/* Returns the load average table. (If averaging is in progress, returns -1
 * and sets errno to EAGAIN, else returns 0.)
 *	arg1 = pointer to the load average table (20 bytes long)
 */
#define	ARIXFPHW	8	/* what (if any?) flting-pnt hardware	*/
#define	ARIXKSTR	9	/* make a copy of a kernel string	*/
/* Copy a null-terminated string from a given kernel address to a user buffer.
 *	arg1 = kernel address
 *	arg2 = user buffer address
 *	arg3 = (uint) string length (including '\0')
 */
#define	ARIXMEMSIZE	10	/* return a system memory value		*/
/* Returns the value of the selected kernel memory variable:
 *	arg1 = one of the following XMEM_ defines
 */
#define  XMEM_AVAILRMEM		0	/* available lockable pages	*/
#define  XMEM_AVAILSMEM		1	/* available virtual pages	*/
#define  XMEM_FREEMEM		2	/* currently free physical pages */
#define  XMEM_MAXMEM		3	/* maximum available physical pages */
#define  XMEM_PHYSMEM		4	/* total physical pages		*/
/* */
#define	ARIXTODEBUG	11	/* Transfer control to firmware/debugger*/
#define	ARIXCCDEBUG	12	/* Control character access to debugger	*/
/* Modify the control character which gets into or out of debug mode.
 * Returns the previous control character.
 *	arg1 = flag (get if 0, else set)
 *	arg2 = new control char (if set)
 */
#define	ARIXDELMEM	13	/* Delete available memory for testing	*/
#define	ARIXADDMEM	14	/* Add back deleted memory		*/

#endif /* SYS_SYSARIX_H */
