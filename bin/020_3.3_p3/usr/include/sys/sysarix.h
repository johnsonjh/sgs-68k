#ifndef SYS_SYSARIX_H
#define SYS_SYSARIX_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sysarix.h: version 24.1 created on 10/28/91 at 18:39:04	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sysarix.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
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
 *		use one of the following (except PS_MARGIN_BAD):
 * Returns the previous margin state, or PS_MARGIN_BAD for an invalid supply.
 */
#define PS_MARGIN_OFF	0	/* set power supply back to normal	*/
#define PS_MARGIN_HIGH	1	/* set supply voltage higher than usual	*/
#define PS_MARGIN_LOW	2	/* set supply voltage lower than usual	*/
#define PS_MARGIN_BAD	3	/* invalid power supply number		*/

#define MAX_PS_NUMBER	4	/* highest valid power supply number	*/
/* */
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
 *	arg2 = (XMEM_ALL only) pointer to XMEM_NUM longs
 */
#define  XMEM_AVAILRMEM		0	/* available lockable pages	*/
#define  XMEM_AVAILSMEM		1	/* available virtual pages	*/
#define  XMEM_FREEMEM		2	/* currently free physical pages */
#define  XMEM_MAXMEM		3	/* maximum available physical pages */
#define  XMEM_PHYSMEM		4	/* total physical pages		*/
#define  XMEM_ALL		-1	/* all of the above, in order	*/
#define  XMEM_NUM		5	/* number of longs for XMEM_ALL	*/
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
/* */
#define ARIXGMARG	15	/* Get the power supply margin - S90 only */
/*
 * System 90 specific
 * ARIXGMARG system call has one argument:
 *	arg1 = power supply number (0..4)
 * Returns the current margin state of the specified power supply, or
 * PS_MARGIN_BAD for an invalid power supply.
 */
#define ARIXGETCLKCAL	16	/* Get the clock chip's calibration value */
/*
 * System 90 specific
 * ARIXGETCLKCAL returns the system clock chip calibration value.
 * Positive values make the clock run (val / 100.0) seconds per month faster.
 * Negative values make the clock run slower.
 * (Present clock chips cannot be calibrated nearly this accurately.  The
 *  current one is calibrated in units of 5.35 [use 535] seconds per month.
 *  Giving the value in hundredths of a second per month allows for more
 *  accurate clocks in the future.)
 */
#define ARIXSETCLKCAL	17	/* Set the clock chip's calibration value */
/*
 * System 90 specific
 * ARIXSETCLKCAL sets and returns the system clock chip calibration value.
 *	arg1 = (int) calibration value in hundredths of a second per month
 * See above for details on accuracy.  The returned value will vary based
 * on how accurately the chip can be calibrated.  The current chip will
 * always return multiples of 535.
 */
#define ARIXGETVALSYM	18	/* Return the kernel symbol <= the value */
/*
 * System 90 specific
 * ARIXGETVALSYM returns the kernel symbol with the value <= arg1
 *	arg1 = (uint) value
 *	arg2 = pointer to struct _ksym for result
 * Returns 0 on success, ENXIO if no symbols or symbol not found
 */

#define KSYM_NAME_LEN	32		/* max symbol name length (with null) */
typedef struct _ksym {
	unsigned int	value;
	char		name[KSYM_NAME_LEN];
} ksym_t;

#define ARIXGETSTRSYM	19	/* Return the kernel symbol given a string */
/*
 * System 90 specific
 * ARIXGETSTRSYM returns the kernel symbol value that matches the string arg1
 *	arg1 = symbol string (max length = KSYM_NAME_LEN including NULL)
 * Returns ENXIO on error, else an uint not equal to (uint)-1.
 */

#define ARIXPOSIX	20		/* do POSIX specific commands	 */
#define	ARIXGETPOSFLAGS		1	/* return POSIX flags field	 */
#define	ARIXSETPOSFLAGS		2	/* set new value for POSIX flags */

/*
 * System 90 specific
 * ARIXGETKERNNAME returns the pathname of the booted kernel in arg1
 *	arg1 = string (at least KERN_NAME_LEN bytes long [see sys/spm_mem.h])
 * Returns EFAULT on address error.
 */
#define ARIXGETKERNNAME	21		/* get booted kernel pathname	 */

/*
 * System 90 specific
 * ARIXGETSPM_MEM returns the spm_mem structure in arg1
 *	arg1 = pointer to spm_mem_t (see sys/spm_mem.h)
 * Returns EFAULT on address error.
 */
#define ARIXGETSPM_MEM	22		/* copy out spm_mem structure	 */

#endif /* SYS_SYSARIX_H */
