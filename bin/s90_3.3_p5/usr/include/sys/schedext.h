#ifndef	SYS_SCHEDEXT_H
#define	SYS_SCHEDEXT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) schedext.h: version 24.1 created on 10/28/91 at 18:37:50	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)schedext.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

extern	schedsw_t	schedsw[];
extern	int		schedswcnt;
extern	int		schedsw_lock;
extern	int		sched_debug;

extern	int		sched_type;
extern	int		sched_mode;
extern	int		which_loadavg;
extern	int		s5enh_cpulimit;
extern	int		s5bsd_cpulimit;
extern	int		bsd_cpulimit;
extern	int		mach_cpulimit;
extern	int		s5enh_cpudecay;
extern	int		s5enh_cpuattack;
extern	int		bsd_cpudecay;
extern	int		mach_cpudecay;
extern	int		*sched_idxarray[];

extern	int		clkcpu_idx;
extern	int		clkpri_idx;
extern	int		ukclkcpu_idx;
extern	int		ukclkpri_idx;
extern	int		ukclkslp_idx;
extern	int		wakeuppri_idx;
extern	int		setrunpri_idx;
extern	int		userpri_idx;
extern	int		exitcpu_idx;
extern	int		loadavg_idx;

extern	long		*AvenPtr;	/* ptr to one of the load arrays */
extern	long		Avenrun[];	/* load averages		 */
extern	long		Cexp[];		/* constants for above array	 */
extern	long		MachAvenrun[];	/* load averages		 */
extern	long		MachCexp[];	/* constants for above array	 */

extern	int		noschedcpu();
extern	int		noschedpri();
extern	int		noschedslp();
extern	int		noschedload();
extern	int		sched_loadavg();

extern	int		s5_clkcpu();
extern	int		s5_ukclkcpu();
extern	int		s5_ukclkpri();
extern	int		s5_userpri();
extern	int		s5_exitcpu();
extern	int		s5_loadavg();

extern	int		s5enh_clkcpu();
extern	int		s5enh_ukclkcpu();
extern	int		s5enh_ukclkpri();
extern	int		s5enh_userpri();
extern	int		s5enh_exitcpu();

extern	int		s5bsd_ukclkcpu();

extern	int		bsd_clkcpu();
extern	int		bsd_clkpri();
extern	int		bsd_ukclkcpu();
extern	int		bsd_ukclkpri();
extern	int		bsd_ukclkslp();
extern	int		bsd_wakeuppri();
extern	int		bsd_setrunpri();
extern	int		bsd_userpri();
extern	int		bsd_loadavg();

extern	int		bsd_setpri();
extern	int		bsd_updatepri();

extern	int		mach_clkcpu();
extern	int		mach_clkpri();
extern	int		mach_ukclkcpu();
extern	int		mach_ukclkpri();
extern	int		mach_ukclkslp();
extern	int		mach_wakeuppri();
extern	int		mach_setrunpri();
extern	int		mach_userpri();
extern	int		mach_loadavg();

extern	int		mach_setpri();
extern	int		mach_updatepri();

extern	RUNQ		runqs, upkern_runqs;
extern	uint		num_idle_pms;
extern	long		lbolt;
#endif	/* SYS_SCHEDEXT_H */
