#ifndef	SYS_PERF_H
#define	SYS_PERF_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) perf.h: version 24.1 created on 10/28/91 at 18:36:20	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)perf.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *	IF INVOKED AS ...
 *
 *	chgperf(fname,cmd,pid,nval,oval)
 *		fname	is uap->arg0	(user virtual address of file name)
 *
 *	OR ...
 *
 *	fchgperf(fdes,cmd,pid,nval,oval)
 *		fdes	is uap->arg0	(user file descriptor)
 *
 *	THEN ...
 *
 *		cmd	is uap->arg1	(user virtual address of cmd value)
 *					which perf value to affect (sub-command)
 *		pid	is uap->arg2	NOT USED
 *		nval	is uap->arg3	new value to set (accepts any value)
 *					Upon fork, child inherits parent's
 *					quantum, and all clamps.
 *					For quantum, upon exec, child will use
 *					default time slice size if inode value
 *					is zero or will keep parent's value if
 *					inode value is negative, or will accept
 *					new value from the inode if it is within
 *					a valid range.
 *					For clamps, upon exec, child will
 *					turn off clamps if inode value is
 *					zero, or will keep parent's value if
 *					inode value is negative, or will accept
 *					new value from the inode if it is within
 *					a valid range.
 *		oval	is uap->arg4	return old value here
 *
 *	ELSE ...
 *
 *		if (fname is 0) OR (fdes < 0), then affect the
 *					process identified by pid
 *		cmd	is uap->arg1	(user virtual address of cmd value)
 *					which perf value to affect (sub-command)
 *		pid	is uap->arg2	process id of process to affect
 *					if 0, do it to current process
 *		nval	is uap->arg3	new value to set
 *					if 0, then use a default for quantum or
 *						turn off clamps
 *					if -1, then just return the old value
 *						in oval
 *		oval	is uap->arg4	return old value here
 *
 *	SPECIAL CASE ...
 *
 *		For getting or setting statistics, the following override
 *		the above description:
 *
 *		cmd	is uap->arg1	(user virtual address of cmd value)
 *					which perf stat to get/set
 *		pid	is uap->arg2	process id of process to affect
 *					if -1, get global stats, else
 *					if 0, get stats from current process
 *					else, get stats from process 'pid'
 *		nval	is uap->arg3	stat overflow field get/set
 *		oval	is uap->arg4	stat count field get/set
 *
 *	RETURN:
 *		0	- on success
 *		-1	- on failure and set errno to:
 *			EINVAL	- if cmd not supported or in conflict
 *				  or if values are way out of range
 *			EFAULT	- if copying of perf structure failes
 *			ESRCH	- if 'pid' is not in proc table
 *			EBADF	- if passed bad file descriptor
 *			ENOSYS	- if bad file system type of this operation
 *			EACCES	- if not superuser and tries to change
 *				  someone else's values
 */

/*
 *	Possible values for p_perf_flags
 */
#ifdef	PERF
#define	PERF_QUANTUM_LEADER	0x00000001	/* quantum leader flag	*/
#define	PERF_QUANTUM_MASK	0x0000000f	/* quantum flags  mask	*/
#define	PERF_SLICE_LEADER	0x00000010	/* slice   leader flag	*/
#define	PERF_SLICE_MASK		0x000000f0	/* slice   flags  mask	*/
#define	PERF_PRI_LEADER		0x00000100	/* pri     leader flag	*/
#define	PERF_PRI_MASK		0x00000f00	/* pri     flags  mask	*/
#define	PERF_USRPRI_LEADER	0x00001000	/* usrpri  leader flag	*/
#define	PERF_USRPRI_MASK	0x0000f000	/* usrpri  flags  mask	*/
#define	PERF_CPU_LEADER		0x00010000	/* cpu     leader flag	*/
#define	PERF_CPU_MASK		0x000f0000	/* cpu     flags  mask	*/
#endif	/* PERF */
#define	PERF_PRIADJ_LEADER	0x00100000	/* pri adj leader flag	*/
#define	PERF_PRIADJ_MASK	0x00f00000	/* pri adj flags  mask	*/

/*
 *	Possible values for perf.cmd
 */
#ifdef	PERF
#define	PERF_GETQUANTUM		0	/* get current quantum size	      */
#define	PERF_SETQUANTUM		1	/* set current quantum size	      */
#define	PERF_GETSLICE		2	/* get current time slice clamp	      */
#define	PERF_SETSLICE		3	/* set current time slice clamp	      */
#define	PERF_GETPRI		4	/* get current pri clamp	      */
#define	PERF_SETPRI		5	/* set current pri clamp	      */
#define	PERF_GETUSRPRI		6	/* get current usrpri clamp	      */
#define	PERF_SETUSRPRI		7	/* set current usrpri clamp	      */
#define	PERF_GETCPU		8	/* get current cpu clamp	      */
#define	PERF_SETCPU		9	/* set current cpu clamp	      */

#define	PERF_GETMAXQUANTUM	10	/* get maximum quantum size	      */
#define	PERF_SETMAXQUANTUM	11	/* set maximum quantum size	      */
#define	PERF_GETMAXSLICE	12	/* get maximum time slice clamp	      */
#define	PERF_SETMAXSLICE	13	/* set maximum time slice clamp	      */
#define	PERF_GETMAXPRI		14	/* get maximum pri clamp	      */
#define	PERF_SETMAXPRI		15	/* set maximum pri clamp	      */
#define	PERF_GETMAXUSRPRI	16	/* get maximum usrpri clamp	      */
#define	PERF_SETMAXUSRPRI	17	/* set maximum usrpri clamp	      */
#define	PERF_GETMAXCPU		18	/* get maximum cpu clamp	      */
#define	PERF_SETMAXCPU		19	/* set maximum cpu clamp	      */

#define	PERF_GETQUANTUMCOPY	20	/* get quantum copy flag	      */
#define	PERF_SETQUANTUMCOPY	21	/* set quantum copy flag	      */
#define	PERF_GETSLICECOPY	22	/* get slice   copy flag	      */
#define	PERF_SETSLICECOPY	23	/* set slice   copy flag	      */
#define	PERF_GETPRICOPY		24	/* get pri     copy flag	      */
#define	PERF_SETPRICOPY		25	/* set pri     copy flag	      */
#define	PERF_GETUSRPRICOPY	26	/* get usrpri  copy flag	      */
#define	PERF_SETUSRPRICOPY	27	/* set usrpri  copy flag	      */
#define	PERF_GETCPUCOPY		28	/* get cpu     copy flag	      */
#define	PERF_SETCPUCOPY		29	/* set cpu     copy flag	      */

#define	PERF_GETQUANTUMSTAT	30	/* get quantum usage stats	      */
#define	PERF_SETQUANTUMSTAT	31	/* set quantum usage stats	      */
#define	PERF_GETSLICESTAT	32	/* get slice   usage stats	      */
#define	PERF_SETSLICESTAT	33	/* set slice   usage stats	      */
#define	PERF_GETPRISTAT		34	/* get pri     usage stats	      */
#define	PERF_SETPRISTAT		35	/* set pri     usage stats	      */
#define	PERF_GETUSRPRISTAT	36	/* get usrpri  usage stats	      */
#define	PERF_SETUSRPRISTAT	37	/* set usrpri  usage stats	      */
#define	PERF_GETCPUSTAT		38	/* get cpu     usage stats	      */
#define	PERF_SETCPUSTAT		39	/* set cpu     usage stats	      */

#define	PERF_GETGLOBQUANTUM	40	/* get global quantum size	      */
#define	PERF_SETGLOBQUANTUM	41	/* set global quantum size	      */
#define	PERF_GETGLOBSLICE	42	/* get global time slice clamp	      */
#define	PERF_SETGLOBSLICE	43	/* set global time slice clamp	      */
#define	PERF_GETGLOBPRI		44	/* get global pri clamp		      */
#define	PERF_SETGLOBPRI		45	/* set global pri clamp		      */
#define	PERF_GETGLOBUSRPRI	46	/* get global usrpri clamp	      */
#define	PERF_SETGLOBUSRPRI	47	/* set global usrpri clamp	      */
#define	PERF_GETGLOBCPU		48	/* get global cpu clamp		      */
#define	PERF_SETGLOBCPU		49	/* set global cpu clamp		      */
#endif	/* PERF */

#define	PERF_GETPRIPPIPE	50	/* get PPIPE priority value	      */
#define	PERF_SETPRIPPIPE	51	/* set PPIPE priority value	      */
#define	PERF_GETPRIPSEMZ	52	/* get PSEMZ priority value	      */
#define	PERF_SETPRIPSEMZ	53	/* set PSEMZ priority value	      */
#define	PERF_GETPRIPSEMN	54	/* get PSEMN priority value	      */
#define	PERF_SETPRIPSEMN	55	/* set PSEMN priority value	      */
#define	PERF_GETFLAGSCOPY	56	/* get performance flags copy flag    */
#define	PERF_SETFLAGSCOPY	57	/* set performance flags copy flag    */
#define	PERF_GETPERFFLAGS	58	/* get performance flags	      */
#define	PERF_SETPERFFLAGS	59	/* set performance flags	      */

#define	PERF_GETPRIADJ		60	/* get pri adj value		      */
#define	PERF_SETPRIADJ		61	/* set pri adj value		      */
#define	PERF_GETGLOBPRIADJ	62	/* get global pri adj value	      */
#define	PERF_SETGLOBPRIADJ	63	/* set global pri adj value	      */
#define	PERF_GETPRIADJCOPY	64	/* get pri adj value copy flag	      */
#define	PERF_SETPRIADJCOPY	65	/* set pri adj value copy flag	      */
#define	PERF_GETMAXPRIADJ	66	/* get max pri adj value	      */
#define	PERF_SETMAXPRIADJ	67	/* set max pri adj value	      */
#define	PERF_GETPRIADJSTAT	68	/* get pri adj value stat	      */
#define	PERF_SETPRIADJSTAT	69	/* set pri adj value stat	      */

#define	PERF_GETDEBUG		70	/* get perf debug state		      */
#define	PERF_SETDEBUG		71	/* set perf debug state		      */

#define	WHICH_PRI_ADJ(pp)	((glob_pri_adj>0)?glob_pri_adj:(pp)->p_pri_adj)

#ifdef	PERF
#define	PERF_SLICE_CLAMP(pp,slice) \
{ \
	if ((glob_slice_clamp > 0) && (glob_slice_clamp < pp->p_slice_clamp)) {\
		if ((slice) > glob_slice_clamp) { \
			glob_slice_clamp_cnt++; \
			if (glob_slice_clamp_cnt == 0) \
				glob_slice_clamp_ovfl++; \
			pp->p_slice_cnt++; \
			if (pp->p_slice_cnt == 0) \
				pp->p_slice_ovfl++; \
			(slice) = (int) glob_slice_clamp; \
		} \
	} else	{ \
		if ((pp->p_slice_clamp > 0) && ((slice) > pp->p_slice_clamp)) {\
			glob_slice_clamp_cnt++; \
			if (glob_slice_clamp_cnt == 0) \
				glob_slice_clamp_ovfl++; \
			pp->p_slice_cnt++; \
			if (pp->p_slice_cnt == 0) \
				pp->p_slice_ovfl++; \
			(slice) = (int) pp->p_slice_clamp; \
		} \
	} \
}

#define	PERF_PRI_CLAMP(pp,pri) \
{ \
	if ((glob_pri_clamp > 0) && (glob_pri_clamp < pp->p_pri_clamp)) { \
		if ((pri) > glob_pri_clamp) { \
			glob_pri_clamp_cnt++; \
			if (glob_pri_clamp_cnt == 0) \
				glob_pri_clamp_ovfl++; \
			pp->p_pri_cnt++; \
			if (pp->p_pri_cnt == 0) \
				pp->p_pri_ovfl++; \
			(pri) = (uchar) glob_pri_clamp; \
		} \
	} else	{ \
		if ((pp->p_pri_clamp > 0) && ((pri) > pp->p_pri_clamp)) { \
			glob_pri_clamp_cnt++; \
			if (glob_pri_clamp_cnt == 0) \
				glob_pri_clamp_ovfl++; \
			pp->p_pri_cnt++; \
			if (pp->p_pri_cnt == 0) \
				pp->p_pri_ovfl++; \
			(pri) = (uchar) pp->p_pri_clamp; \
		} \
	} \
}

#define	PERF_USRPRI_CLAMP(pp,usrpri) \
{ \
	if ((glob_usrpri_clamp > 0) && \
			(glob_usrpri_clamp < pp->p_usrpri_clamp)) { \
		if ((usrpri) > glob_usrpri_clamp) { \
			glob_usrpri_clamp_cnt++; \
			if (glob_usrpri_clamp_cnt == 0) \
				glob_usrpri_clamp_ovfl++; \
			pp->p_usrpri_cnt++; \
			if (pp->p_usrpri_cnt == 0) \
				pp->p_usrpri_ovfl++; \
			(usrpri) = (uchar) glob_usrpri_clamp; \
		} \
	} else	{ \
		if ((pp->p_usrpri_clamp > 0) && \
				((usrpri) > pp->p_usrpri_clamp)) { \
			glob_usrpri_clamp_cnt++; \
			if (glob_usrpri_clamp_cnt == 0) \
				glob_usrpri_clamp_ovfl++; \
			pp->p_usrpri_cnt++; \
			if (pp->p_usrpri_cnt == 0) \
				pp->p_usrpri_ovfl++; \
			(usrpri) = (uchar) pp->p_usrpri_clamp; \
		} \
	} \
}

#define	PERF_CPU_CLAMP(pp,cpu) \
{ \
	if ((glob_cpu_clamp > 0) && (glob_cpu_clamp < pp->p_cpu_clamp)) { \
		if ((cpu) > glob_cpu_clamp) { \
			glob_cpu_clamp_cnt++; \
			if (glob_cpu_clamp_cnt == 0) \
				glob_cpu_clamp_ovfl++; \
			pp->p_cpu_cnt++; \
			if (pp->p_cpu_cnt == 0) \
				pp->p_cpu_ovfl++; \
			(cpu) = (uchar) glob_cpu_clamp; \
		} \
	} else	{ \
		if ((pp->p_cpu_clamp > 0) && ((cpu) > pp->p_cpu_clamp)) { \
			glob_cpu_clamp_cnt++; \
			if (glob_cpu_clamp_cnt == 0) \
				glob_cpu_clamp_ovfl++; \
			pp->p_cpu_cnt++; \
			if (pp->p_cpu_cnt == 0) \
				pp->p_cpu_ovfl++; \
			(cpu) = (uchar) pp->p_cpu_clamp; \
		} \
	} \
}
#endif	/* PERF */
#endif	/* SYS_PERF_H */
