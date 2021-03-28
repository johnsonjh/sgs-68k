#ifndef SYS_INTS_H
#define SYS_INTS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ints.h: version 24.1 created on 10/28/91 at 18:33:50	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ints.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#if !defined(SYS_SBUS_SPM_H)
#include "sys/sbus_spm.h"
#endif

/*
 * ints.h:
 *
 * 	this file contains defines for the different commands that 
 *	are placed in the VECTOR portion of an interrupt dispatcher
 *	dispatched interrupt.
 *
 *	defines for the level at which interrupts will come in.
 *
 *	and defines for software interrupts.
 */

/*
 * VECTOR portion of dispatched interrupts (bits 31 - 24, see sbus_spm.h)
 *
 */

/* interrupt types can range from 0x01 to 0x3f	*/

#define MM_INTERRUPT		0x01
#define IOM_ERROR		0x02
#define IOSBA_ERROR		0x03
#define KERNEL_TO_SPM_CMD	0x04	/* generic command from kernel to SPM*/
#define	KERNEL_TO_SPM_OUTPUT	0x05
#define	CONSOLE_INPUT		0x06
#define	CONSOLE_OUTPUT		0x07
#define UPKERN_CONSOLE_INPUT	0x08
#define UPKERN_CONSOLE_OUTPUT	0x09
/* unused			0xa */
/* unused,			0xb */
#define KERNEL_TO_SPM_DONE	0x0c	/* generic command complete */
#define TIMEIN			0x0d
#define RUNQUEUES		0x0e
#define UPKERN_STRATEGY		0x0f
#define DO_IOMAP_UPDATE		0x10
#define DO_TLB_FLUSH		0x11
#define	IOPM_IOCTL_INTR		0x12	/* IOPM has responded to PM cmd */
#define	IOPM_BUF_INTR		0x13	/* IOPM has left BUFS work for PM */
#define	IOPM_STR_INTR		0x14	/* IOPM has left STREAMS work for PM */
#define SPM_TO_KERNEL_CMD	0x15	/* generic command from SPM to kernel*/
#define SPM_TO_KERNEL_DONE	0x16	/* generic command complete */
#define UPKERN_SPM2KERN_CMD	0x17	/* upkern generic command */
#define	RESCHED			0x18
#define	DISABLE_INTERRUPTS	0x19	/* go to spl7() until told otherwise */

/* 
 * these are the different levels that the spm will
 * program the IOM to send interrupts at.
 */

#define CONSOLE_LEVEL	MOT_LEVEL_THREE
#define DEFAULT_LEVEL	MOT_LEVEL_FIVE

/* commands for SPM to kernel or kernel to SPM interrupts -- must be != 0 */

#define	K2S_SETTIME		0x01	/* set spm_time and the HW clock time */
#define K2S_SETMARGIN		0x02	/* set a power margin		*/
#define K2S_UADMIN		0x03	/* uadmin power/reset/shutdown cmds */
#define K2S_GETMARGIN		0x04	/* get a power margin		*/
#define K2S_GET_CLK_CAL		0x05	/* get the system clock cal value */
#define K2S_SET_CLK_CAL		0x06	/* set the system clock cal value */
#define K2S_CCDEBUG		0x07	/* get/set the switch-to-SPM char */
#define K2S_RESETIOPM		0x08	/* reset selected IOPM */
#define K2S_STARTIOPM		0x09	/* start (unreset) selected IOPM */
#define S2K_PT_FAST_FLUSH	0x40	/* power/temp commands		*/
#define S2K_PT_NORMAL_FLUSH	0x41
#define S2K_PT_SHUTDOWN		0x42
#define S2K_CALL_FUNC		0x43	/* call function: (*a[0])(a[1], a[2]) */
#define S2K_KILL_PROC		0x44	/* kill a proc: a[0]=sig, a[1]=pid */

#endif /* SYS_INTS_H */
