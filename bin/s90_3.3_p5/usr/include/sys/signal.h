#ifndef SYS_SIGNAL_H
#define SYS_SIGNAL_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) signal.h: version 24.1 created on 10/28/91 at 18:38:06	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)signal.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	SYS_TYPES_H
#include <sys/types.h>
#endif

#define	SIGHUP	1	/* hangup */
#define	SIGINT	2	/* interrupt (rubout) */
#define	SIGQUIT	3	/* quit (ASCII FS) */
#define	SIGILL	4	/* illegal instruction (not reset when caught)*/
#define	SIGTRAP	5	/* trace trap (not reset when caught) */
#define	SIGIOT	6	/* TRAPV instruction */
#define SIGABRT 6	/* used by abort, replace SIGIOT in the  future */
#define	SIGEMT	7	/* EMT instruction */
#define	SIGCHK	7	/* CHK instruction */
#define	SIGFPE	8	/* floating point exception */
#define	SIGKILL	9	/* kill (cannot be caught or ignored) */
#define	SIGBUS	10	/* bus error */
#define	SIGSEGV	11	/* segmentation violation */
#define	SIGSYS	12	/* bad argument to system call */
#define	SIGPIPE	13	/* write on a pipe with no one to read it */
#define	SIGALRM	14	/* alarm clock */
#define	SIGTERM	15	/* software termination signal from kill */
#define	SIGUSR1	16	/* user defined signal 1 */
#define	SIGUSR2	17	/* user defined signal 2 */
#define	SIGCLD	18	/* death of a child */
#define	SIGPWR	19	/* power-fail restart */
#define	SIGWINCH 20	/* window change */
#define SIGURG	21	/* urgent socket condition (SVR4) */
#define SIGPOLL 22	/* pollable event occured */

#ifdef _POSIX_SOURCE
#define SIGSTOP 23	/*stop sig. can't be caught or ignored */
#define SIGTSTP 24	/*interactive stop sig from terminal */
#define SIGCONT 25	/*continue if stopped    */
#define SIGTTIN 26	/*read from control tty-from background process*/
#define SIGTTOU 27	/*write to control tty-from background process*/
#define SIGCHLD SIGCLD  /*redefined for posix; both are child-death signals */
#define SIGVTALRM 28	/* virtual timer expired (SVR4) */
#define SIGPROF 29	/* profiling timer expired (SVR4) */
#define	SIGSAK	30	/*something to do with security*/
#define	NSIG	31	/* The valid signal number is from 1 to NSIG-1 */
#else
#define SIGVTALRM 28	/* virtual timer expired (SVR4) */
#define SIGPROF 29	/* profiling timer expired (SVR4) */
#define	NSIG	31
#endif

#define MAXSIG	32	/* THIS CAN'T BE CONDITIONAL ON POSIX_SOURCE!! */

#if lint
#define SIG_ERR	(void(*)())0
#define	SIG_DFL	(void(*)())0
#define	SIG_IGN	(void (*)())0
#define SIG_HOLD (void(*)())0
#else

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

#define SIG_ERR 	(void (*) _PARMS((int)))-1
#define SIG_DFL 	(void (*) _PARMS((int)))0
#define	SIG_IGN		(void (*) _PARMS((int)))1

#define SIG_HOLD (void(*)())2
#endif

/* The following definitions are only used by kernel */
#define SIGNO_MASK	0xff
#define SIGDEFER	0x0100
#define SIGHOLD		0x0200
#define SIGRELSE	0x0400
#define SIGIGNORE	0x0800
#define SIGPAUSE	0x1000
/* more such defined for posix */
#define SIG_BLOCK	0x0001
#define SIG_UNBLOCK	0x0002
#define SIG_SETMASK	0x0004


/* SIGFPE codes */

#define	KINTOVF	1	/* integer overflow */
#define	KINTDIV	2	/* integer divide by zero */
#define	KFLTOVF	3	/* floating overflow */
#define	KFLTDIV	4	/* floating divide by zero */
#define	KDECDIV	KFLTDIV	/* decimal divide by zero */
#define	KFLTUND	5	/* floating underflow */
#define	KDECOVF	6	/* decimal overflow */
#define	KSUBRNG	7	/* subscript range */

/* The following codes correspond to new exceptions generated
** by the M68881 floating point chip.				*/
#define	KFPBSUN	18	/* FPU exc. - Branch or Set on UNordered cond.*/
#define	KFPINEX	19	/* FPU exc. - INEXact result */
#define	KFPDZ  	20	/* FPU exc. - Divide by Zero */
#define	KFPUNFL	21	/* FPU exc. - UNderFLow */
#define	KFPOPER	22	/* FPU exc. - OPerand ERror */
#define	KFPOVFL	23	/* FPU exc. - OVerFLow */
#define	KFPSNAN	24	/* FPU exc. - Signalling NAN */

#define	sigbit(n)	(1L << ((n) -1))

#ifdef _POSIX_SOURCE 
 /* since SIGCONT can be blocked, it can't be a part of CANTMASK */
#define	CANTMASK	(sigbit(SIGKILL)|sigbit(SIGSTOP)|sigbit(SIGSAK))
#define	STOPSIGMASK	(sigbit(SIGSTOP)|sigbit(SIGTSTP)| \
			sigbit(SIGTTIN)|sigbit(SIGTTOU))
#define	POSIXSIGNALS	(sigbit(SIGSTOP)|sigbit(SIGTSTP)| \
			sigbit(SIGTTIN)|sigbit(SIGTTOU)|sigbit(SIGCONT))
/*
 * Interactive stop signals (from keyboard).
 */
#define	ISTOPSIGMASK	(sigbit(SIGTSTP)|sigbit(SIGTTIN)|sigbit(SIGTTOU))

typedef struct sigaction {	/*sigaction struct defined for posix */
		void(*sa_handler)();
		sigset_t 	 sa_mask;
		int		 sa_flags;
	} sigact_t;

#define	SA_NOCLDSTOP 0x1
#define	SA_CLDSTOP   0x2
#endif

#endif /* SYS_SIGNAL_H */
