#ifndef	SYS_OWN_H
#define SYS_OWN_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) own.h: version 24.1 created on 10/28/91 at 18:36:13	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)own.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * Own structure, one per PM
 */

#ifndef	SYS_PROC_H
#include "sys/proc.h"
#endif
#ifndef	SYS_IMMU_H
#include "sys/immu.h"
#endif
#ifndef	SYS_TDB_H
#include "sys/tdb.h"
#endif
#ifndef	SYS_UPKERN_H
#include "sys/upkern.h"
#endif
#ifndef	SYS_SPM_H
#include "sys/sbus_spm.h"
#endif

#define	CharBuffCnt			128	/* must be a power of 2 */
#define	CharBuffMask			(CharBuffCnt - 1)
#define OWN_INTERRUPT_QUEUE_SIZE	128

typedef struct	own {
	/* PM configuration information, initialized by SPM */
 	qde_t		*o_qtbl;	/* private kernel q segment table */
	sde_t		*o_stbl;	/* private kernel segment table */
	uint		o_slot;		/* slot of PM */
	unchar		o_lock_id;	/* id logged by spin_lock, etc */
 	unchar		o_pm_id;	/* my pm's index number */
 	unchar		o_base_pm_id;	/* index for base pm on board for 040 */
 	unchar		o_last_pm_id;	/* last PM ID on board
 					   (finishes the board initialization)*/

	/* PM <-> SPM interaction */
	uint		o_clock;
	uint		o_clocks_dropped;
	uint		o_printf_count;
	uint		o_spm_printf_count;
	unchar		o_char_buff[CharBuffCnt];
	unchar		o_boot_response;
	unchar		o_pad_pm_spm[3];
 	struct own_soft_int_queue {
		disp_int_t	*consumer;
		disp_int_t	*producer;
		disp_int_t	*end;
		disp_int_t	data[OWN_INTERRUPT_QUEUE_SIZE];
		uint		queue_pad;
	} o_queue_1, o_queue_4;
	tdb_t		o_tdb;	/* tdb control structure */
	uint		o_siderail;
	uint		:32;	/* reserved */

	/* PM configuration information, initialized by PM */
	unchar		o_fpu_type;
	unchar		o_cpu_type;
	unchar		o_fpu_loaded;	/* set when fpu contains fpu state */
	unchar		o_pad_pm[1];
	uint		o_cpu_same;		/* each PM with same cpu mask */
	uint		o_fpu_same;		/* each PM with same fpu mask */
	uint		o_id_bit;		/* this PM's bit, used in
						/* idle_pm_bits, user_pm_bits,
						 * cpu/fpu_same.
						 */
	upkern_t	o_upkern_init_val;	/* grab upkern value */
	upkern_t	o_upkern_dec_val;	/* upkern_dec check value */
	uint		:32;			/* reserved */

	/* ordinary unix variables that are duplicated for each PM */
	struct proc	*o_curproc;
	rde_t		o_cur_root;	/* user root pointer for tdb */
	int		o_switching;
	int		o_lticks;
	uchar 		o_curpri;
	char 		o_runrun;
 	unchar		o_bus_reset;	/*for 040 -- else just a pad */
 	unchar		o_err_lock;     /* for 040 -- else just a pad */

	/* more variables */
	uint			o_spin_lock_cnt;   /* cnts spin locks we have */
	unchar			o_trap_hap;
	unchar			o_is_idle;
	unchar			o_flush_tlb;	/*  when != 0 in clock() */
	unchar			o_upkern_proc;	/* curproc has upkern */
	unchar			o_in_int_service;
	unchar			o_disable_interrupts;
	unchar			o_reenable_interrupts;
	unchar			o_pad_more;
	uint			o_update_iomap;
	struct lock_req_array  *o_lock_req_ptr;


	/* put this at the end to avoid probems if it changes size */
	proc_t		o_proc;		/* A proc for own_u.u_procp */ 
	auth_t		o_auth;		/* user authentication structure */
} own_t;

extern	struct	own	own;

/*
 * values for o_clock
 *
 *	0 denotes clock interrupts not yet enabled.
 */
#define CLOCK_ENABLE	0x10		/* PM sets when ready for another */
#define CLOCK_LEADER	0x20		/* SPM sets for upkern clock */
#define CLOCK		0x30		/* SPM sets for non upkern clock */

#ifdef	M68040
/*
 * values for o_err_lock
 */
#define	ERR_LOCK_GOT	0x8
#define	ERR_LOCK_BERR	0x4
#define	ERR_LOCK_NMI	0x2
#define	ERR_LOCK_CPUA	0x1
#endif	/* M68040 */

/*
 * defines to allow old A1000 and ATT code to continue to work
 *
 */
#define	switching	own.o_switching
#define own_switching	own.o_switching

#define runrun		own.o_runrun
#define own_runrun	own.o_runrun

#define curproc		own.o_curproc
#define own_curproc	own.o_curproc

#define curpri		own.o_curpri
#define own_curpri	own.o_curpri

#endif /* SYS_OWN_H */
