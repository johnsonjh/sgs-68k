#ifndef SYS_SBUS_PM_H
#define SYS_SBUS_PM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sbus_pm.h: version 24.1 created on 10/28/91 at 18:37:39	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sbus_pm.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * sbus_pm.h
 *
 *	This describes the appearance of the processor module
 *	as seen by other modules on the CSS bus.
 *
 *	See Cameron's PM Document "slotfn".
 */

/*
 * PM_STATUS_REGISTER
 *
 *	Intereseting things to read from a PM on the SBUS:
 */

#define PM_STATUS_REGISTER	(unsigned)0xfffffffc

/* minimum address bits needed to talk to PM status register */
#define PM_STAT_MIN_BITS	(unsigned)0xfc
/* minimum address bits needed to talk to PM command register */
#define PM_CMD_MIN_BITS		(unsigned)0x80

/* active low bits describing highest pending interrupt.  */
#define PM_IPL_REQ_LEVEL_MASK	0x00d00000

/* PM has doubled bus faulted, or has not run yet */
#define PM_FAULTED		0x00100000

#define PM_BOARD_ID_MASK	0x000000ff

/* Command register is full.  The cmd-pend bit goes to 0 when the processor
 * resets it by writing at CLR_NMI.  Separately, a write to INC_READY is
 * required to allow more commands.
 */
#define PM_COMMAND_PENDING	0x00080000


/*
 * Command register:
 *
 */

#define PM_COMMAND_REGISTER	0xffffff80

/*
 * Interrupt Requests:
 *
 *	The interrupt request register contains seven individual request bits.
 *	An aligned long write command to can change the state of any one request
 *	bit.
 *
 *	Interrupt acknowledge by the cpu chip is auto-vectored, and has no
 *	effect on any request bits.
 */

	/* TO CLEAR A PM INTERRUPT */
#ifdef	M68020
#define PM_INT_REQ_REG		(unsigned)0xfffffffc
#define PM_CLEAR_LEVEL_ONE	0x01000000
#define PM_CLEAR_LEVEL_TWO	0x02000000
#define PM_CLEAR_LEVEL_THREE	0x03000000
#define PM_CLEAR_LEVEL_FOUR	0x04000000
#define PM_CLEAR_LEVEL_FIVE	0x05000000
#define PM_CLEAR_LEVEL_SIX	0x06000000
#define PM_CLEAR_LEVEL_SEVEN	0x07000000
#endif /* M68020 */

#ifdef	M68040
/* Some of the interrupts come in as hardware ints, some as software ints */
#define PM_INT_CLEAR_REG	(PM_CLRINT)
#define PM_CLEAR_LEVEL_ONE	CLR_SW1
#define PM_CLEAR_LEVEL_TWO	CLR_HW2	/* to pm A only */
#define PM_CLEAR_LEVEL_THREE	CLR_HW3	/* to pm A only */
#define PM_CLEAR_LEVEL_FOUR	CLR_SW4
#define PM_CLEAR_LEVEL_FIVE	CLR_SW5
#define PM_CLEAR_LEVEL_SIX	CLR_SW6
#define PM_CLEAR_LEVEL_SEVEN	CLR_SW7 /* nmi's dealt with elsewhere */
#define clear_pm_interrupt(pattern)	*(PM_INT_CLEAR_REG) = (pattern)
#endif /* M68040 */


#ifdef	M68020
#define PM_LEVEL_ONE_INT_REQ	0x09000000
#define PM_LEVEL_TWO_INT_REQ	0x0a000000
#define PM_LEVEL_THREE_INT_REQ	0x0b000000
#define PM_LEVEL_FOUR_INT_REQ	0x0c000000
#define PM_LEVEL_FIVE_INT_REQ	0x0d000000
#define PM_LEVEL_SIX_INT_REQ	0x0e000000
#define PM_LEVEL_SEVEN_INT_REQ	0x0f000000
#define send_pm_interrupt(pm_id, pattern) *(pm_int_req_regs[(pm_id)]) = pattern
#endif /* M68020 */

#ifdef	M68040
#define PM_INT_REQ_REG		PM_INTR
/* for the 68040, these patterns always send a software interrupt */
#define PM_LEVEL_ONE_INT_REQ	0x09000000
#define PM_LEVEL_TWO_INT_REQ	0x0a000000
#define PM_LEVEL_THREE_INT_REQ	0x0b000000
#define PM_LEVEL_FOUR_INT_REQ	0x0c000000
#define PM_LEVEL_FIVE_INT_REQ	0x0d000000
#define PM_LEVEL_SIX_INT_REQ	0x0e000000
#define PM_LEVEL_SEVEN_INT_REQ	0x0f000000
#define send_pm_interrupt(pm_id, pattern) *(pm_int_req_regs[(pm_id)]) = \
	    ((pattern) | ((spm_mem.pm_own[(pm_id)]->o_base_pm_id == pm_id)<<29))
#endif	/* M68040 */

#endif /* SYS_SBUS_PM_H */
