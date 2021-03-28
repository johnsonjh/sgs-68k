#ifndef SYS_GCP_SYS_H
#define SYS_GCP_SYS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sys.h: version 2.1 created on 5/22/89 at 19:07:03	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sys.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)head/gcp:sys.h	1.6"

extern struct tcb *	sys_tcb;		/* current tcb		*/
extern unsigned char	sys_eidi;		/* disable depth	*/
extern unsigned char	sys_slice;		/* current time slice	*/
extern unsigned char	sys_notrun;		/* system not run flag	*/
extern struct tcb 	sys_run_queue;		/* system run queue	*/

extern int		sys_icb_base;		/* icb base address	*/
extern int		sys_icb_lomem;		/* icb low mem pointer	*/
extern int		sys_icb_himem;		/* icb high mem pointer	*/
extern int		sys_icb_mtos;		/* master to slave cmds	*/
extern int		sys_icb_stom;		/* slave to master cmds	*/

extern struct teb *	sys_teb_free;		/* free teb chain	*/
extern struct teb *	sys_teb_head;		/* head teb chain	*/

extern unsigned short	dtr_reg;		/* bit control for DTR	*/
extern int		dlcb[];			/* interrupt data link	*/
extern int		(*dltxint[])();		/* interrupt handler	*/
extern int		(*dlrxint[])();		/* interrupt handler	*/
extern int		(*dlexint[])();		/* interrupt handler	*/
extern int		(*dlspint[])();		/* interrupt handler	*/
extern int		(*dltxdma[])();		/* interrupt handler	*/
extern int		(*dlrxdma[])();		/* interrupt handler	*/

extern struct scc	*scc_table[];

extern long		time;
extern char		ports[];
extern unsigned short	sync_ctrl;
extern int		(*mtos_service[])();

#endif /* SYS_GCP_SYS_H */
