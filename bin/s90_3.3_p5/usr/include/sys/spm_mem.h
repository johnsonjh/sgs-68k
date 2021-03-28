#ifndef SYS_SPM_MEM_H
#define SYS_SPM_MEM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) spm_mem.h: version 24.1 created on 10/28/91 at 18:38:17	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)spm_mem.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/* spm_mem.h */
/*
 * Shared memory area in main memory maintained by service processor
 */

#ifndef SYS_SBUS_H
#include "sys/sbus.h"
#endif

#ifndef SYS_IMMU_H
#include "sys/immu.h"
#endif

#ifndef SYS_OWN_H
#include "sys/own.h"
#endif

#ifndef SYS_TYD_SRAM_H
#include "sys/tyd_sram.h"
#endif

#ifndef SYS_SPM_LP_H
#include "sys/spm_lp.h"
#endif

#ifndef SYS_STREAM_H
#include "sys/stream.h"
#endif

#ifndef SYS_SYNCH_H
#include "sys/synch.h"
#endif

#ifndef SYS_STRSTAT_H
#include "sys/strstat.h"
#endif

#ifndef SYS_INTS_H
#include "sys/ints.h"
#endif

#ifndef SYS_IOM_CONFIG_H
#include "sys/iom_config.h"
#endif

#ifndef SYS_IUNIT_H
#include "sys/iunit.h"
#endif

#define KERN_NAME_LEN	64		/* length of kernel pathname	*/

#define NUM_K2S_ARGS	3		/* number of kernel to SPM arguments */
#define NUM_S2K_ARGS	4		/* number of SPM to kernel arguments */

/*
 * SPM_MEM_VERSION should be updated after any change is made to the
 * way the PMs and SPM communicate, in particular to own_t or spm_mem_t.
 */
#define SPM_MEM_VERSION	0x05301991

/* structure for kernel symbol table */
typedef struct kern_sym {
	uint	value;			/* symbol's value (address)	*/
	char	*name;			/* name string in kern virtual	*/
} kern_sym_t;

typedef struct spm_mem {
	uint	IOPM_sp;			/* initial IOPM stack pointer */
	uint	IOPM_pc;			/* initial IOPM pc */	

		/* versions are expected to be SPM_MEM_VERSION */
	uint	spm_version;		/* spm's idea of version */
	uint	pm_version;		/* pm's idea of version */

	uchar	sbus_slot_id[SBUS_NUM_SLOT];	/* id of each slot */
	uchar	dev_board_id[SBUS_NUM_SLOT];	/* IOPM's device board */
	ulong	slot_size[SBUS_NUM_SLOT];	/* size of any memory boards */

	uint	pfn_adjust[SBUS_NUM_SLOT];	/* spfn minus lpfn */
	uint	lpfn_end[SBUS_NUM_SLOT];	/* one more than last lfn */ 

	ulong	spm_mem_used;	/* memory taken up by SPM + Kernel */

	uint	spm_slot;	/* slot of the SPM */

	rde_t	invalid_rde;	/* describes many invalid_sde */
	qde_t	invalid_qde;	/* describes many invalid_qde */
	sde_t	invalid_sde;	/* describes many invalid_pde */
	sde_t	*iopm_stbl;	/* u32 address of pointers to IOPM ipde_t's */
	pde_t	*mem_ptbl;	/* kernel's address of mem_ptbl in memory */
	pde_t	*kptbl;		/* kernel's description of syssegs */
	uint	*kv_iomap;	/* 040 map table -- hanna FIX do you want it here? */
	uint	kv_size;	/* size of syssegs in pages */
	uint	*lpfn_tbl;	/* kernel's address of lookup table for
				 * lpfn_to_pfn
				 */
	uint	num_pm;		/* how many pm's in this machine */
	own_t	*pm_own[SBUS_MAX_NUM_PM];	/* each PM's own */

	iom_config_t iom_config[NUM_IOM_PER_SYS];

	uint	kernel_green_light; 	/* SPM sets to non-zero to tell the
					 * kernel to continue execution after
					 * waiting in startc2
					 */
	upkern_t	upkern;

	uint	ksym_proc;	/* kernel address's for SPM ps */
	uint	ksym_v;
	uint	cache_off;

	iunit_t	rootdev;
	iunit_t	swapdev;

	uint	panic_flag;		/* the kernel has panic'd */
	uint	panic_pm_id;		/* pm of panic'er */
	uint	panic_slot;		/* slot of panic'er */


	/********* Start of SPM Runtime services ********/

	/*********  SPM Console ********/

	struct {
		uchar	input_buf;
		uchar	enable_input;
					/* cleared by spm to indicate that it
					 * has put a character into the 
					 * unix_console_input_buf.
					 * kernel sets to 1 to indicate
					 * that it has drained it.
					 */
		uchar	output_buf;
		uchar	enable_output;	
					/* set by kernel to indicate that it
					 * has put a character into the 
					 * unix_console_output_buf.
					 * spm sets to zero to indicate
					 * that it has drained it.
					 */

		disp_int_t	spm_output_int;	/* interrupt value to write into
				 	 	 * dispatcher to interrupt spm
						 * for console output;
					 	 */
		disp_int_t	input_int;	/* interrupt value for spm to
						 * write into the dispatcher
						 * for console input
						 */
		disp_int_t	output_int;	/* interrupt value for spm to
						 * write into the dispatcher
						 * for output done interrupt
						 */
	} console;

	/*********  SPM Clock ********/

	uint		ticks_500;	/* spm maintained 500 usec counter */
	uint		spm_time;	/* spm maintained seconds
					 * counter (kern format) */
	time_t		time;		/* kernel time in seconds since 1970 */
	uint		lbolt;		/* SPM: # of ticks since kern booted */

	/*********  kernel streams variables used by IOPM ********/
	char             qrunflag;	/* set iff queues are enabled */
	char             strbcflag;	/* bufcall functions ready to go */
	short            pad;
	dizzy_lock_t     allocb_lock;	/* lock for mbfreelist & dbfreelist */
	struct msgb      *mbfreelist;	/* message block free list */
	dblk_t           *dbfreelist[NCLASS];	/* data block free lists */
	struct dbalcst   dballoc[NCLASS];	/* data block allocation parm */
	struct strstat   strst;		/* streams statistics structure */
	int              nmblock;

	/* ACRW tty, lp support */
	spm_tty_t 	spm_tty;
	spm_pm_lp_t 	spm_lp_ctl;

	uint		kern2spm_cmd;	/* kernel to SPM command number */
	uint		kern2spm_arg[NUM_K2S_ARGS];
	disp_int_t	kern2spm_int;	/* kernel to SPM interrupt */
	disp_int_t	spm2kern_done;	/* SPM to kernel cmd done interrupt */

	uint		spm2kern_cmd;	/* SPM to kernel command number */
	uint		spm2kern_arg[NUM_S2K_ARGS];
	disp_int_t	spm2kern_int;	/* SPM to kernel interrupt (uses this
					 * with different vector for done int)*/

	kern_sym_t	*symbols;	/* kernel symbol table */
	int		num_syms;	/* number of symbol entries */

	/* FIX THIS, CMW: move below upkern */
	/* NOTE: this a pointer to a structure in kernel memory,
	 * it may or may not straddle a page boundary.
	 *
	 * if it is desired that the SPM be a member of the
	 * dizzy lock brotherhood, she will require that the
	 * data structure be contained within a single page.
	 * the simplest method would be to move it to spm_mem
	 */
	struct lock_req_array  *lock_req_arrayp;
	uint                   next_lock_id;

	/* Misc. info provided by the SPM */
	uint		num_pages;	/* total # of pages of system memory */
	uint		cpu_type;	/* kernel CPU type (e.g. 68020....) */
	char		kern_name[KERN_NAME_LEN];
} spm_mem_t;


#define	SEND_CLK_INT	1

/*
 * See sys/kmem.h for location of where this shows up in kernel virtual
 */

extern struct spm_mem	spm_mem;		/* this is for the kernel */
#define Spm_Mem	((spm_mem_t *)0x90000000)	/* this is for the spm code */

#endif /* SYS_SPM_MEM_H */
