#ifndef	SYS_SYNCH_H
#define	SYS_SYNCH_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) synch.h: version 24.1 created on 10/28/91 at 18:39:01	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)synch.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

/* SPIN LOCKS */

typedef struct sem_lock {
	ushort	lock_psw;	/* 0 or saved psw of owner of lock */

	/* what follows is optional and intended to aid in debugging */

	unchar	lock_owner;	/* information about the owner of lock */
	ulong	ret_pc;		/* optional pc of who got lock */
	ulong	filler[6];
} spin_lock_t;

#define my_spin_lock(l)	((l)->lock_psw && (l)->lock_owner == own_lock_id)
extern uchar	own_lock_id;

extern	spin_lock_t
	runqs_lock,
	proc_lock,
	flock_sleeplist_sem,
	freelocklist_sem,
	inode_sem,
	file_locking_sem,
	region_lock_sem,
	bio_lock,
	s54kbio_lock,
	super_block_flock,
	super_block_ilock,
	callout_lock,
	swap_lock,
	swapbuf_lock,
	delay_lock,
	file_freelist_lock,
	errlog_sem,
#ifdef	M68040
	fpem_lock,	/* floating point emulation code lockout */
#endif	/* M68040 */
	sat_sem;


/* SUSPEND LOCKS */

/* #define SUSPEND_DEBUG	4	/* collect debug info on N ret pcs */

#define SUSPEND_INIT(p)	{ 0, 0, p }	/* initializes lock priority	*/


	/* Although suspend_lock_t->lock is a full 8 bits
	 * only bit7 and bit6 are used.  This allows
	 * bit0 thru but5 to be used for other purposes.
	 * However, changes to the byte should only be done
	 * using atomic instructions such as "cas".
	 */

typedef	struct suspend_lock {
	uchar		s_lock;		/* contains BUSY and WANT bits	*/
	uchar		s_filler;
	ushort		s_priority;	/* sleep priority on collision	*/
	uint		s_proc;		/* lock owner's proc pointer	*/
	uint		s_numlocks;	/* number of times lock used	*/
	uint		s_collisions;	/* number of atomic collisions	*/
	spin_lock_t	s_spin_lock;	/* spin lock used for synchronizing */
#ifdef SUSPEND_DEBUG
	uint	s_func[SUSPEND_DEBUG];	/* stack backtrace		*/
#endif /* SUSPEND_DEBUG */
} suspend_lock_t;

uint	try_suspend_lock();

/* DIZZY LOCKS */

/* Caution: hard coded offsets in dizzy_lock and dizzy_unlock */
typedef struct dizzy_lock {
	struct lock_req_array  *head_lock_req;	/* head of requesters */
				/* this is real lock & must be long aligned */
	ulong	lock_psw;	/* 0 or saved psw of owner of lock */
	ulong	lock_owner;	/* information about the owner of lock */

	/* what follows is optional and intended to aid in debugging */

	ulong   ret_pc;
	ulong	ok_cnt;
	ulong	fail_cnt;
	ulong	wait_cnt;
	ulong   max_chain;
} dizzy_lock_t;

#define LLOCK_MAX 80		/* 80 =  64 IOPMs & 16 PMs */
struct  lock_req_array {	/* hard coded offsets & size in dizzy_lock */
	struct lock_req_array  *next_lock_req;	/* previous requester */
	ulong   remote_spin;	/* 1 or 0 for spin in lockers local mem */
	ulong   slot;		/* if remote_spin, slot of locker */
	union {
		ulong   *addr;	/* offset into slot */
		ulong	spin;	/* spin element for PMs */
	} lra;
};


#endif	/* SYS_SYNCH_H */
