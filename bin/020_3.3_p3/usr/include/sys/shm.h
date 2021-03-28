#ifndef SYS_SHM_H
#define SYS_SHM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) shm.h: version 24.1 created on 10/28/91 at 18:38:03	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)shm.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_IMMU_H
#include	<sys/immu.h>
#endif

#ifndef SYS_SYNCH_H
#include	<sys/synch.h>
#endif

/*
**	IPC Shared Memory Facility.
*/

/*
**	Implementation Constants.
*/

#define	SHMLBA	ctob(1)	/* segment low boundary address multiple */
			/* (SHMLBA must be a power of 2) */

/*
**	Permission Definitions.
*/

#define	SHM_R	0400	/* read permission */
#define	SHM_W	0200	/* write permission */

/*
**	ipc_perm Mode Definitions.
*/

#define	SHM_INIT	001000	/* grow segment on next attach		*/
#define	SHM_DEST	002000	/* destroy segment when # attached = 0	*/
#define SHM_A1000	040000	/* A1000 style shared memory		*/
/* #define IPC_ALLOC   0100000	/* entry allocated (defined in sys/ipc.h)*/

/*
**	Message Operation Flags.
*/

#define	SHM_RDONLY	010000	/* attach read-only (else read-write) */
#define	SHM_RND		020000	/* round attach address to SHMLBA */

/*
**	Structure Definitions.
*/

/*
**	There is a shared mem id data structure for each segment in the system.
*/

struct shmid_ds {
	struct ipc_perm	shm_perm;	/* operation permission struct */
	int		shm_segsz;	/* size of segment in bytes */
	struct region	*shm_reg;	/* ptr to read-write shared region */
	/*
	 * 3/31/88 JPC
	 * The following member (shm_ro_reg) replaces the next line in the
	 * struct shmid_ds declaration in intro(2) of the 5.3.1 Prog Ref Man:
	 * char		pad[4];		/* for swap compatibility *
	 *
	 * This was moved to the end of structure for A1000 compatibillity.
	 */
/*	struct region	*shm_ro_reg;	/* ptr to read-only shared region */
	ushort		shm_lpid;	/* pid of last shmop */
	ushort		shm_cpid;	/* pid of creator */
	ushort		shm_nattch;	/* number of current attaches */
/*	ushort		shm_cattch;	/* used only for shminfo */
	time_t		shm_atime;	/* last shmat time */
	time_t		shm_dtime;	/* last shmdt time */
	time_t		shm_ctime;	/* last change time */
	struct region	*shm_ro_reg;	/* ptr to read-only shared region */
};

struct	shminfo {
	int	shmmax,	/* max shared memory segment size */
		shmmin,	/* min shared memory segment size */
		shmmni,	/* # of shared memory identifiers */
		shmseg,	/* max attached shared memory segments per process */
		shmbrk,	/* gap (in clicks) used between data and shared memory*/
		shmall;	/* max total shared memory system wide (in clicks) */
};

/*
 * Shared memory control operations
 */

#define SHM_LOCK	3	/* Lock segment in core */
#define SHM_UNLOCK	4	/* Unlock segment */

#endif /* SYS_SHM_H */
