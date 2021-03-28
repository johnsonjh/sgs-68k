#ifndef SYS_SBUS_SPM_H
#define SYS_SBUS_SPM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sbus_spm.h: version 24.1 created on 10/28/91 at 18:37:43	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sbus_spm.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#define SPM_CMD_REG		((unsigned)0xffffff80)
#define SPM_INT_DISPATCHER	((unsigned)0xffffffa0)

/* 
 *		INTERRUPT DATA received from a DISPATCHER ACK
 *
 *	  31 - 24   23 22-18 17 16   15 - 12    11 - 8   7     6 - 0
 *	+----------+-+------+-----+----------+----------+-+-------------+
 *	| Vector # |D|   x  |level|Dest. Slot| Src. Slot|I|I/O Src. Slot| 
 *	+----------+-+------+-----+----------+----------+-+-------------+
 */

typedef union {
	struct {
		uint	vector		: 8;	/* software controlled */
		uint	directed	: 1;	/* directed/non-directed */
		uint			: 5;	/* not propagated by spm */
		uint	level		: 2;	/* modified by directed */
		uint	dest_slot	: 4;	/* for directed ints */
		uint	src_slot	: 4;	/* css source of int */
		uint	from_io_bus	: 1;	/* Not set for IOA int */
		uint	io_slot		: 7;	/* slot behind an IOA */
	} fields;

	uint	entire_32_bits;
} disp_int_t;

/*
 * values for disp_int.level
 *
 *	disp_int.directed must be set for levels FIVE and FOUR
 *	disp_int.directed must be clear for levels THREE, TWO, ONE, ZERO
 */

#define SPM_DISP_LEVEL_FIVE	1		/* ASSERT(int_data.directed) */
#define SPM_DISP_LEVEL_FOUR	0		/* ASSERT(int_data.directed) */
#define SPM_DISP_LEVEL_THREE	3		/* ASSERT(! int_data.directed */
#define SPM_DISP_LEVEL_TWO	2		/* ASSERT(! int_data.directed */
#define SPM_DISP_LEVEL_ONE	1		/* ASSERT(! int_data.directed */
#define SPM_DISP_LEVEL_ZERO	0		/* ASSERT(! int_data.directed */

#define NON_DIRECTED	0
#define DIRECTED	1

/*
 * These are Motorola/CSS interrupt levels, which are
 * refered to as level - 1 internally on the SPM
 */

#define MOT_LEVEL_SIX	SPM_DISP_LEVEL_FIVE	/* ASSERT(int_data.directed) */
#define MOT_LEVEL_FIVE	SPM_DISP_LEVEL_FOUR	/* ASSERT(int_data.directed) */
#define MOT_LEVEL_FOUR	SPM_DISP_LEVEL_THREE	/* ASSERT(! int_data.directed */
#define MOT_LEVEL_THREE	SPM_DISP_LEVEL_TWO	/* ASSERT(! int_data.directed */
#define MOT_LEVEL_TWO	SPM_DISP_LEVEL_ONE	/* ASSERT(! int_data.directed */
#define MOT_LEVEL_ONE	SPM_DISP_LEVEL_ZERO	/* ASSERT(! int_data.directed */


/*
 * Addresses to read in the interrupt dispatcher to recieve
 * interrupt data, and to reset the dispatcher for sending
 * interrupts at that level to the PM reading.
 */

#define SPM_ACK_LEVEL_ONE	((unsigned)0xffffff0c)
#define SPM_ACK_LEVEL_TWO	((unsigned)0xffffff14)
#define SPM_ACK_LEVEL_THREE	((unsigned)0xffffff1c)
#define SPM_ACK_LEVEL_FOUR	((unsigned)0xffffff24)
#define SPM_ACK_LEVEL_FIVE	((unsigned)0xffffff2c)
#define SPM_ACK_LEVEL_SIX	((unsigned)0xffffff34)

#endif /* SYS_SBUS_SPM_H */
