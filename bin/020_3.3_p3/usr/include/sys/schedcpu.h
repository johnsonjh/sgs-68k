#ifndef	SYS_SCHEDCPU_H
#define	SYS_SCHEDCPU_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) schedcpu.h: version 24.1 created on 10/28/91 at 18:37:46	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)schedcpu.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * Cpu scheduler switch.
 */
typedef	struct	schedsw
	{
		int	(*s_clkcpu)();
		int	(*s_clkpri)();
		int	(*s_ukclkcpu)();
		int	(*s_ukclkpri)();
		int	(*s_ukclkslp)();
		int	(*s_wakeuppri)();
		int	(*s_setrunpri)();
		int	(*s_userpri)();
		int	(*s_exitcpu)();
		int	(*s_loadavg)();
	} schedsw_t;

#define	CLKCPU(pp)		(*schedsw[clkcpu_idx].s_clkcpu)(pp)
#define	CLKPRI(pp,ps)		(*schedsw[clkpri_idx].s_clkpri)(pp,ps)
#define	UKCLKCPU(pp)		(*schedsw[ukclkcpu_idx].s_ukclkcpu)(pp)
#define	UKCLKPRI(pp)		(*schedsw[ukclkpri_idx].s_ukclkpri)(pp)
#define	UKCLKSLP(pp)		(*schedsw[ukclkslp_idx].s_ukclkslp)(pp)
#define	WAKEUPPRI(pp)		(*schedsw[wakeuppri_idx].s_wakeuppri)(pp)
#define	SETRUNPRI(pp)		(*schedsw[setrunpri_idx].s_setrunpri)(pp)
#define	USERPRI(pp)		(*schedsw[userpri_idx].s_userpri)(pp)
#define	EXITCPU(up,pp)		(*schedsw[exitcpu_idx].s_exitcpu)(up,pp)
#define	LOADAVG()		(*schedsw[loadavg_idx].s_loadavg)()

/*
 * Scheduler type index into scheduler table.
 */
#define	SCHED_NONE		0
#define	SCHED_SVR32		1	/* SVR32 standard sched	   cpu=80     */
#define	SCHED_SVR32ENH		2	/* SVR32 dyn limit, attack & decay    */
#define	SCHED_SVR32BSD		3	/* SVR32 enh limit, attack & BSD decay*/
#define	SCHED_BSD43		4	/* BSD43 standard sched    cpu=255    */
#define	SCHED_MACH		5	/* MACH  standard sched    cpu=255    */
#define	SCHED_MIXED		99	/* indexes were set individually*/

/*
 * Possible values for loadavg index cmd.
 */
#define	LOADAVG_1MIN		0	/* use  1 min  loadavg for p_cpu calc */
#define	LOADAVG_5MIN		1	/* use  5 min  loadavg for p_cpu calc */
#define	LOADAVG_15MIN		2	/* use 15 min  loadavg for p_cpu calc */
#define	LOADAVG_5SEC		3	/* use  5 sec  loadavg for p_cpu calc */
#define	LOADAVG_INST		4	/* use instant loadavg for p_cpu calc */

#define	LOADAVG_MACH		10	/* offset into loadavg index table    */
#define	LOADAVG_M1MIN		10	/* use  1 min  loadavg for p_cpu calc */
#define	LOADAVG_M5MIN		11	/* use  5 min  loadavg for p_cpu calc */
#define	LOADAVG_M30SEC		12	/* use 30 sec  loadavg for p_cpu calc */
#define	LOADAVG_M5SEC		13	/* use  5 sec  loadavg for p_cpu calc */
#define	LOADAVG_MINST		14	/* use instant loadavg for p_cpu calc */

/*
 * Fraction for digital decay to forget 90% of usage in 5*loadav seconds.
 */
#define	filter(lav)	((1000 * ((lav) << 1)) / (((lav) << 1) + 1000))

/*
 * Per function hard limits for p_cpu field.
 */
#define	S5_CPULIMIT		80	/* hard limit for p_cpu field	*/
#define	S5ENH_CPULIMIT		96	/* hard limit for p_cpu field	*/
#define	S5BSD_CPULIMIT		127	/* hard limit for p_cpu field	*/
#define	BSD_CPULIMIT		255	/* hard limit for p_cpu field	*/
#define	MACH_CPULIMIT		255	/* hard limit for p_cpu field	*/

#define	S5_CPUDECAY		2	/* default decay is by 1/x	*/
#define	S5ENH_CPUDECAY		2	/* default decay is by 1/x	*/
#define	BSD_CPUDECAY		2	/* default decay is by 1/x	*/
#define	MACH_CPUDECAY		2	/* default decay is by 1/x	*/

#define	S5_CPUATTACK		0	/* default attack rate mask	*/
#define	S5ENH_CPUATTACK		0	/* default attack rate mask	*/

/*
 * Number of scheduler indexes for get/set index array command.
 * The kernels array is really one more than this number because
 * the last entry has zero value for termination.
 */
#define	SCHED_NUMIDX		(sizeof(schedsw_t) / sizeof(int))

/*
 * Possible values for cmd.
 */
#define	SCHED_GETSCHEDTYPE	1	/* get current scheduler type	*/
#define	SCHED_SETSCHEDTYPE	2	/* set current scheduler type	*/
#define	SCHED_GETMODE		3	/* get scheduler mode		*/
#define	SCHED_SETMODE		4	/* set scheduler mode		*/
#define	SCHED_GETLOADAVG	5	/* get loadavg index for sched	*/
#define	SCHED_SETLOADAVG	6	/* set loadavg index for sched	*/
#define	SCHED_GETARRAY		7	/* get sched index array	*/
#define	SCHED_SETARRAY		8	/* set sched index array	*/
#define	SCHED_GETCPULIMIT	9	/* get sched cpu limit		*/
#define	SCHED_SETCPULIMIT	10	/* set sched cpu limit		*/
#define	SCHED_GETCPUDECAY	11	/* get sched cpu decay rate	*/
#define	SCHED_SETCPUDECAY	12	/* set sched cpu decay rate	*/
#define	SCHED_GETCPUATTACK	13	/* get sched cpu attack rate	*/
#define	SCHED_SETCPUATTACK	14	/* set sched cpu attack rate	*/

#define	IDX_GETBASE		50	/* offset into array from cmd	*/
#define	IDX_GETCLKCPU		50	/* to get individual indexes	*/
#define	IDX_GETCLKPRI		51	/* to get individual indexes	*/
#define	IDX_GETUKCLKCPU		52	/* to get individual indexes	*/
#define	IDX_GETUKCLKPRI		53	/* to get individual indexes	*/
#define	IDX_GETUKCLKSLP		54	/* to get individual indexes	*/
#define	IDX_GETWAKEUPPRI	55	/* to get individual indexes	*/
#define	IDX_GETSETRUNPRI	57	/* to get individual indexes	*/
#define	IDX_GETUSERPRI		58	/* to get individual indexes	*/
#define	IDX_GETEXITCPU		62	/* to get individual indexes	*/
#define	IDX_GETLOADAVG		63	/* to get individual indexes	*/

#define	IDX_SETBASE		70	/* offset into array from cmd	*/
#define	IDX_SETCLKCPU		70	/* to set individual indexes	*/
#define	IDX_SETCLKPRI		71	/* to set individual indexes	*/
#define	IDX_SETUKCLKCPU		72	/* to set individual indexes	*/
#define	IDX_SETUKCLKPRI		73	/* to set individual indexes	*/
#define	IDX_SETUKCLKSLP		74	/* to set individual indexes	*/
#define	IDX_SETWAKEUPPRI	75	/* to set individual indexes	*/
#define	IDX_SETSETRUNPRI	77	/* to set individual indexes	*/
#define	IDX_SETUSERPRI		78	/* to set individual indexes	*/
#define	IDX_SETEXITCPU		82	/* to set individual indexes	*/
#define	IDX_SETLOADAVG		83	/* to set individual indexes	*/

#define	SCHED_GETDEBUG		98	/* get scheduler's debug level	*/
#define	SCHED_SETDEBUG		99	/* set scheduler's debug level	*/
#endif	/* SYS_SCHEDCPU_H */
