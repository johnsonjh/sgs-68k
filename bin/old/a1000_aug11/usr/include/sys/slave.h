#ifndef SYS_SLAVE_H
#define SYS_SLAVE_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) slave.h: version 2.1 created on 5/22/89 at 19:10:34	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)slave.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/* these defines are present in os/synch.c and must be updated there
   also
*/
#define NUM_PMB_SLOTS 6
extern	unsigned  char	runqsem;	/* runqsem for master */
extern  unsigned  char	tas_sem;
extern	unsigned  char	*rqsem;		/* runqsem for slave */
extern	unsigned  char  *tas_sem_ptr;
extern  char	slave_go;
extern	struct  {
		unsigned int		int_cmd;
		unsigned char		type;
		unsigned char		priority;
		unsigned char		proc;
	}  cpu_conf[];
extern	char 			*cpu_addr[];
extern	int			cpu_list[];
extern	short			slave_own_memory[];
extern	char			cpuslotnum;
extern	short			slcpu_id;
extern  int			num_cpu;	/* number of cpus in system */
extern  int			num_cpu25;
extern  int			pure_cpu25;
extern	struct	proc		*proc_ptr_global;
extern  struct	var		*v_ptr_global;
extern	time_t			*lbolt_ptr_global;
extern	time_t			*time_ptr_global;
extern	short			*maxmem_ptr_global;
extern	struct  proc		**runq_ptr;
extern	struct	boot_config	*bt_ptr;
extern	struct	boot_config	*sl_bt_ptr;
extern	char			*strt_master_base;

#define SLAVE_HOLD 0

/* list of commands sent over icb from master to slave */
#define	SL_INTERRUPT	0
#define SL_RESCHED	1
#define SL_UPDATE_MMU	2
#define SL_RUNQ		3
#define SL_VHAND	4
#define SL_CRASH_STOP	5	/* stops slave during crash processing   */
#define SL_CRASH_CRASH	6	/* tells slave to panic			 */
#define SL_CRASH_GO	7	/* restarts slave after crash processing */
#define SL_READ_MMU_BANK	8 /* copy 4k bytes of mmu reg to master	 */

typedef	struct	slrdmmu
	{
		int	slot;		/* this slave's slot		*/
		int	cpuid;		/* this slave's cpuid		*/
		int	regcnt;		/* how many registers to copy	*/
		int	regoff;		/* where to start in mmu table	*/
		int	bufptr;		/* kernel virtual buffer addr	*/
	} slrdmmu_t;

extern char slave_continue[];

#endif /* SYS_SLAVE_H */
