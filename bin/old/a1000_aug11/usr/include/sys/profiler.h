#ifndef SYS_PROFILER_H
#define SYS_PROFILER_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) profiler.h: version 2.1 created on 5/22/89 at 19:09:38	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)profiler.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	profiler.h	2.1	*/
/*
 * prof.h	-		system profiler header file
 *				extended for icb profiling
 */

#ident	"@(#)uts/head/sys:profiler.h	2.1"

#define PRFMAX 1500		/* maximum number of text addresses */
#define PRF_ON    1		/* profiler collecting samples */
#define PRF_VAL   2		/* profiler contains valid text symbols */

#define PRF_SET   0		/* set prfstat */
#define PRF_CLR   1		/* clr prfstat */
#define PRF_GET	  2		/* get prfid */


/*
 * structure defining addresses and counters for
 * each board to be profiled
 */
struct prfid {
	char name[80];	/* name of a.out file running */
	unsigned stat;	/* state of profiler */
	unsigned max;	/* highest text address */
};
struct prfargs {
	struct prfid p;
	unsigned p_sym[PRFMAX];	/* list of text addresses */
	unsigned p_sys[PRFMAX+1]; /* counters for sys mode  */
};

#endif /* SYS_PROFILER_H */
