#ifndef UNPCB_H
#define UNPCB_H

/*
 * @(#) Copyright 1986.  The Wollongong Group, Inc.  All Rights Reserved.
 */

#ident "@(#)unpcb.h  (TWG)      1.3     89/06/19 "

struct unpcb {
	queue_t *ud_rdq;		/* read queue */
	ushort	ud_flags;		/* see below */
	ushort	ud_tstate;		/* TLI state */
	struct	unpcb *ud_ref;		/* see note below */
	struct	unpcb *ud_nextref;	/* */
	struct	unpcb *ud_conq;		/* we are connected to this */
	mblk_t	*ud_addr;		/* the 'name' we are bound to */
	ushort	ud_ino;			/* inode if bound */
	ushort	ud_maxconn;		/* max # of indications allowed*/
	ushort	ud_numconn;		/* number of connections */
	int	ud_type;		/* socket type */
	int	ud_seqnum;		/* sequence number */
};

/* ud_flag */

#define	TD_ERROR	(1<<0)		/* fatal error occurred */
#define TD_SUPERUSER	(1<<1)		/* Super User opened it */

/* other defines */

#define	UDSCHAN(un)	(un - &Uds[0])
#define	UDDCHAN(un)	(un - &Udd[0])
#define	UNREACH		255

#define	UDS_ID	6543
#define	UDD_ID	5432

#endif /* UNPCB_H */
