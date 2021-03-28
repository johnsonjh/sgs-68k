#ifndef SYS_OWN_H
#define SYS_OWN_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) own.h: version 2.1 created on 5/22/89 at 19:09:16	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)own.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
**	NOTE	NOTE	NOTE	NOTE	NOTE	NOTE	NOTE
**
**	This file contains declarations for variables that are linked
**	into first 8k of data section.  These first two pages (4k each)
**	are mapped to different locations for each slave so that
**	each one sees only his modifications.
**
**	This file MUST follow CLOSELY every declaration in os/own_area.c.
*/

/*
**	Physical memory addresses of some dedicated slots
**	in the mmu registers.
*/
extern	short		*own_cdumpslot;
extern	short		*own_cp1slot;
extern	short		*own_cp2slot;
extern	short		*own_diskslot;
extern	short		*own_dstptpslot;
extern	short		*own_execslot;
extern	short		*own_faultslot;
extern	short		*own_genslot;
extern	short		*own_gpslot;
extern	short		*own_kdeb1slot;
extern	short		*own_kdeb2slot;
extern	short		*own_ksymslot;
extern	short		*own_nmbuf_slot;
extern	short		*own_p1slot;
extern	short		*own_p2slot;
extern	short		*own_p3slot;
extern	short		*own_p4slot;
extern	short		*own_rdmaslot;
extern	short		*own_srcptpslot;
extern	short		*own_swap1slot;
extern	short		*own_swap2slot;
extern	short		*own_swap3slot;
extern	short		*own_uarea1slot;
extern	short		*own_uarea2slot;

/*
**	Base logical address of the windows defined by the above slots.
*/
extern	char		*own_cdumpbase;
extern	char		*own_cp1base;
extern	char		*own_cp2base;
extern	char		*own_diskbase;
extern	char		*own_dstptpbase;
extern	char		*own_execbase;
extern	char		*own_faultbase;
extern	char		*own_genbase;
extern	char		*own_gpbase;
extern	char		*own_kdeb1base;
extern	char		*own_kdeb2base;
extern	char		*own_ksymbase;
extern	char		*own_nmbuf_base;
extern	char		*own_p1base;
extern	char		*own_p2base;
extern	char		*own_p3base;
extern	char		*own_p4base;
extern	char		*own_rdmabase;
extern	char		*own_srcptpbase;
extern	char		*own_swap1base;
extern	char		*own_swap2base;
extern	char		*own_swap3base;
extern	char		*own_uarea1base;
extern	char		*own_uarea2base;

/*
**	Used to restore stack when returning to system state
**	after user process gets sent to master.
*/
extern	short		own_usched1;
extern	short		own_usched2;

/*
**	Address of beginning of mmu registers.
*/
extern	int		own_mmu_addr;

extern	struct	mmureg	*own_mmufreeaddr;
extern	short		own_mmufreeslot;

/*
**	Miscellaneous variables - unique for each cpu.
*/
extern	unsigned char	*own_glob_rqsem;
extern	unsigned char	*own_glob_tas_sem_byte;
extern	unsigned char	*own_slave_status_ptr;

extern	char		*own_panicstr;
extern	char		own_cpu_type;
extern	char		own_curpri;
extern	char		own_fpp_type;
extern	char		own_idleflg;
extern	char		own_local_cpu25;
extern	char		own_local_proc;
extern	char		own_runrun;

extern	int		own_kernel_stk; /* kernel stk start in ublock	*/
extern	int		own_bus_error;
extern	int		own_cpu_id;
extern	int		own_cpuslotnum;
extern	int		own_float_except;
extern	int		own_local_fpp;
extern	int		own_look_runq;
extern	int		own_mmugrp_freeslot;
extern	int		own_num_secs;
extern	int		own_profile;
extern	int		own_savea0;	/* save area for sl_resume	*/
extern	int		own_savea5;	/* save area for ml/cswitch.s	*/
extern	unsigned int	own_sem_mask;
extern	int		own_sem_timeout;
extern	int		own_slave_go;
extern	int		own_switching;
extern	int		own_tas_save;
extern	int		own_trap_hap;

extern	short		own_lticks;
extern	short		own_tas_psr;

extern	time_t		own_lbolt;

extern	struct	proc	*own_curproc;
extern	struct	mmugrp	*own_mmugrp_next;
extern	struct	mmugrp	own_mmugrp[];

extern	int		own_int_stk[];

/*
**	Temporary stack for use by mfs_sleep() and mfs_sleep_with_sig_check().
*/
extern	int		own_mfs_tmp_stack[];
extern	int		own_mfs_tmp_stack_begin;

/*
**	Kernel's system activity data structure for each cpu.
*/
extern	struct	sysinfo	own_sysinfo;
extern	struct	syserr	own_syserr;

#endif /* SYS_OWN_H */
