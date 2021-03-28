#ifndef	SYS_PERFEXT_H
#define	SYS_PERFEXT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) perfext.h: version 24.1 created on 10/28/91 at 18:36:24	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)perfext.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifdef	PERF
extern	long	glob_quantum_copy_flag;	/* copy value from parent dir	*/
extern	long	glob_quantum_size;	/* global quantum size		*/

extern	long	glob_slice_copy_flag;	/* copy value from parent dir	*/
extern	long	glob_slice_clamp_cnt;	/* global usage count stat	*/
extern	long	glob_slice_clamp_ovfl;	/* overflow count for above	*/
extern	long	glob_slice_clamp;	/* global slice size clamp	*/

extern	long	glob_pri_copy_flag;	/* copy value from parent dir	*/
extern	long	glob_pri_clamp_cnt;	/* global usage count stat	*/
extern	long	glob_pri_clamp_ovfl;	/* overflow count for above	*/
extern	long	glob_pri_clamp;		/* global pri clamp		*/

extern	long	glob_usrpri_copy_flag;	/* copy value from parent dir	*/
extern	long	glob_usrpri_clamp_cnt;	/* global usage count stat	*/
extern	long	glob_usrpri_clamp_ovfl;	/* overflow count for above	*/
extern	long	glob_usrpri_clamp;	/* global usrpri clamp		*/

extern	long	glob_cpu_copy_flag;	/* copy value from parent dir	*/
extern	long	glob_cpu_clamp_cnt;	/* global usage count stat	*/
extern	long	glob_cpu_clamp_ovfl;	/* overflow count for above	*/
extern	long	glob_cpu_clamp;		/* global cpu clamp		*/
#endif	/* PERF */

extern	long	glob_pri_adj_copy_flag;	/* copy value from parent dir	*/
extern	long	glob_pri_adj;		/* global pri adj value		*/

extern	long	glob_perf_flags_copy_flag;/* copy value from parent dir	*/

extern	uchar	pri_PPIPE;		/* pipe PPIPE priority value	*/
extern	uchar	pri_PSEMZ;		/* sem  PSEMZ priority value	*/
extern	uchar	pri_PSEMN;		/* sem  PSEMN priority value	*/

extern	int	quantum_size;
#ifdef	PERF
extern	int	max_quantum_size;
extern	int	max_slice_clamp;
extern	int	max_pri_clamp;
extern	int	max_usrpri_clamp;
extern	int	max_cpu_clamp;
#endif	/* PERF */
extern	int	max_pri_adj;
#endif	/* SYS_PERFEXT_H */
