#ifndef SYS_MFS_DEBUG_H
#define SYS_MFS_DEBUG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mfs_debug.h: version 2.1 created on 5/22/89 at 19:08:42	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mfs_debug.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys:mfs_debug.h	2.1"

				/*
				 * this is to aid in automatic inclusion of
				 * "sys/debug.h".  Normally, it should be
				 * included at the beginning of any source
				 * file that has any debug conditional compile
				 * flags in it.  however some of the includes
				 * may also have conditionals in them, and can
				 * check this variable to decide if debug.h
				 * was already included or not.
				 */

/* put all defines between ifndef DEBUG_H / endif DEBUG_H */

/*#define DEBUG_SYSTEM_CALLS	/*
				 * files affected:
				 *	os/sysent.c slave/sl_sysent.c
				 *	systm.h os/slave_ctrl.c
				 *	slave/sl_synch.c os/shortdelay.c
				 */


/* #define SHORT_CR_ASSERT		/* files affected: os/short.s */
/* #define	SHORT_CR_DEBUG */
/* #define	SHORT_CR_ACCT */

/* #define MFS_SLEEP_ASSERT	/* files affected: slave/mfs_slp.c */
/* #define	MFS_SLEEP_ACCT  */
/* #define	MFS_SLEEP_DEBUG  */

/* #define MFS_RDWRI_DEBUG		/* files affected os/rdwri.c */
/* #define MFS_SIG_DEBUG		/* files affected os/sig.c */
/* #define MFS_SYNCH_ASSERT		/* files affected: os/synch.c */
/* #define MFS_SLP_ASSERT			/* files affected: os/slp.c */
/* #define MFS_TRAP_ASSERT			/* files affected: os/trap.c */
/* #define MFS_EXIT_ASSERT			/* files affected: os/sys1.c */
/* #define MFS_CLOSEF_ASSERT		/* files affected: os/sys1.c */

#define CLEAR_REQUEST_STRUCTS	/* files affected: io/dk.c io/icb_int.c */
#define BP_AV_BACK_CLEAR	/* files affected: io/dk.c io/mt.c io/nt.c */

/* #define ICB_INT_ASSERT		/* files affected: io/icb_int.c */
/* #define LOCKTAS_DEBUG		/* files affected: io/icb_int.c */

/* #define DK_DEBUG		/* files affected: io/dk.c */
/* #define DK_ASSERT		/* files affected: io/dk.c */
/* #define MT_ASSERT		/* files affected: io/mt.c */
/* #define NT_ASSERT		/* files affected: io/nt.c */
/* #define NT_DEBUG_1		/* files affected: io/nt.c */

/* #define RDK_ASSERT		/* files affected: io/rdk.c */
/* #define RDK_DEBUG		/* files affected: io/rdk.c */

/* #define SL_GC_DEBUG		/* files affected: slave/slgc.c */
/* #define SL_GC_ASSERT		/* files affected: slave/slgc.c */
/* #define HIPO_GC_ASSERT		/* files affected: slave/slgc.c */

#endif /* SYS_MFS_DEBUG_H */
