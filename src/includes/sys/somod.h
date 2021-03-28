#ifndef SOMOD_H
#define SOMOD_H

/*
 * @(#) Copyright 1989.  The Wollongong Group, Inc.  All Rights Reserved.
 */

#ident "@(#)somod.h  (TWG)      1.1     89/06/19 "

#define SOMOD_ID	0x135a
#define SOMODPRI 	PZERO+3

extern struct so_socket so_socket[];
extern int so_cnt;
extern int somsgsz;

/*
 * somod per channel structure - holds socket info   
 */
struct so_socket {
	queue_t	*so_rdq;		/* read queue */
	int 	so_domain;		/* socket domain */
	int 	so_type;		/* socket type tcp/udp/raw */
	int 	so_proto;		/* socket protocol 0  */
	ushort	so_flags;		/* see below */
	ushort	so_state;		/* see below */
	ushort 	so_options;		/* socket options */
	short	so_pgrp;		/* process group for signals */
	mblk_t  *so_iocsave;		/* pending ioctl message */
	long	so_prim;		/* pending TPI request */
	mblk_t  *so_ctlbuf;		/* reusable buffer for TLI requests */
	mblk_t 	*so_conq;		/* conn queue */
	mblk_t	*so_oob;		/* out of band data */ 
};

#define SIOC_CMD(so)	(*(int *)(so)->so_iocsave->b_rptr)

/*
 * so_flags XXX 
 */
#define USED		0x01	/* data structure in use          */
#define WAITIOCACK	0x02	/* waiting for info for ioctl act */
#define OOB		0x04	/* processing out-of-band data	  */

/*
 * so_state and sl_state. XXX check these out later.
 */
#define	SS_ISBOUND		0x001	/* socket is bound */
#define	SS_ISCONNECTED		0x002	/* socket connected to a peer */
#define	SS_ISCONNECTING		0x004	/* in process of connecting to peer */
#define	SS_ISDISCONNECTING	0x008	/* in process of disconnecting */
#define	SS_CANTSENDMORE		0x010	/* can't send more data to peer */
#define	SS_CANTRCVMORE		0x020	/* can't receive more data from peer */
#define	SS_RCVATMARK		0x040	/* at mark on input */

#define	SS_PRIV			0x080	/* privileged for broadcast, raw... */
#define	SS_NBIO			0x100	/* non-blocking ops */
#define	SS_ASYNC		0x200	/* async i/o notify */


/*
 * ioctl base cmds for all modules
 */
#define SIOC	('S' << 8)

/*
 * somod ioctls 
 */
#define		SIOC_BIND		(SIOC|1)
#define		SIOC_SOCKET		(SIOC|2)
#define		SIOC_CONNECT		(SIOC|3)
#define		SIOC_ACCEPT		(SIOC|4)
#define		SIOC_LISTEN		TCPIOC_SETQLENGTH
#define		SIOC_GETSOCKNAME	TCPIOC_GETSOCKNAME
#define		SIOC_GETPEERNAME	TCPIOC_GETPEERNAME
#define		SIOC_GETINFO		(SIOC|5)
#define		SIOC_BINDQPTR		(SIOC|6)
#define		SIOC_SEND_S		(SIOC|7)  /* Small buffer, noflags */ 
#define		SIOC_SEND_SF		(SIOC|8)  /* Small buffer, Flags   */
#define		SIOC_SEND_L		(SIOC|9)  /* Large buffer, noflags */ 
#define		SIOC_SEND_LF		(SIOC|10) /* Large buffer, Flags   */
#define		SIOC_RECV		(SIOC|11)
#define		SIOC_READOOB		(SIOC|12)
#define		SIOC_PEEKOOB		(SIOC|13)
#define		SIOC_SENDTO		(SIOC|14) /* no flag or to address */
#define		SIOC_SENDTO2		(SIOC|15) /* flag or to address    */
#define		SIOC_RECVFROM		(SIOC|16)
#define		SIOC_SHUTDOWN		(SIOC|17)
#define		SIOC_ATMARK		(SIOC|18)

/*
 * supplementary somod ioctls
 */
#define 	SIOC_UNBLOCK		(SIOC|120)
#define 	SIOC_BLOCK		(SIOC|119)
#define 	SIOC_GETMSGSZ		(SIOC|118)

/*
 * 4.3 ioctls
 */
#define SIOCATMARK		SIOC_ATMARK
#define SIOC_NBIO		(SIOC|127)
#define SIOC_ASYNC		(SIOC|126)
#define	SIOC_NREAD		(SIOC|125)

#ifdef notyet
#define FIONBIO			SIOC_NBIO
#define FIOASYNC		SIOC_ASYNC
#define FIONREAD		SIOC_NREAD
#endif

/*
 * structure for passing various socket call parameters
 */
struct S_sock_req {
	int domain;
	int type;
	int proto;
};

struct S_snd_req {
	int cmd;		/* always SIOC_SEND */
	int flags;		
	int DATA_length;
	int DATA_offset;
};

struct S_sndto_req {
	int cmd;		/* always SIOC_SENDTO */
	int flags;
	int DEST_length;	/* dest addr length */
	int DEST_offset;	/* dest offset */
	int DATA_length;	/* data length */
	int DATA_offset;	/* data offset */
};
	
/*
 * from a pointer to so_so, find the dev #
 */
#define SOCHAN(sp)	(sp - so_socket)

#define	OKTOUSE(BP, SIZE) \
 (((BP->b_datap->db_ref == 1) && \
 (BP->b_datap->db_lim - BP->b_datap->db_base >= SIZE)) ? \
 BP->b_rptr = BP->b_wptr = BP->b_datap->db_base, 1 : 0)

/*
 * Information from somod.
 */
struct s_info {
	int	domain;
	int	type;
	int 	proto;
	ushort	flags;
	ushort 	state;
	ushort 	options;
	int	somsgsz;
};

/*
 * union of the primitives requests
 */
union T_req {
	long			type;		/* primitive type     */
	struct T_conn_req	conn_req;	/* connect request    */
	struct T_discon_req	discon_req;	/* disconnect request */
	struct T_data_req	data_req;	/* data request       */
	struct T_exdata_req	exdata_req;	/* expedited data req */
	struct T_info_req	info_req;	/* information req    */
	struct T_bind_req	bind_req;	/* bind request       */
	struct T_unbind_req	unbind_req;	/* unbind request     */
	struct T_unitdata_req	unitdata_req;	/* unitdata requset   */
	struct T_optmgmt_req	optmgmt_req;	/* manage opt req     */
	struct T_ordrel_req	ordrel_req;	/* orderly rel req    */
};

#define sowriteblock(q) \
	((q)->q_minpsz = 0x7fff)

#define sowriteunblock(q) \
	((q)->q_minpsz = (q)->q_next->q_minpsz)

/* do we have to send all at once on a socket? */
#define	sosendallatonce(so) \
    (((so)->so_state & SS_NBIO) || ((so)->so_type == SOCK_DGRAM) || \
	((so)->so_type == SOCK_RAW))

/* Control the queuing of messages by putq()*/
#define DO_Q		1
#define DONT_Q		0

#ifdef notdef
/* put this as SIGUSR1 in sigbsd.h later */
#define SIGURG	16

/* can we read something from so? */
#define	soreadable(so) \
    ((so)->so_rcv.sb_cc || ((so)->so_state & SS_CANTRCVMORE) || (so)->so_qlen)

/* can we write something to so? */
#define	sowriteable(so) \
    (sbspace(&(so)->so_snd) > 0 && \
	(((so)->so_state&SS_ISCONNECTED) || \
	  ((so)->so_proto->pr_flags&PR_CONNREQUIRED)==0) || \
     ((so)->so_state & SS_CANTSENDMORE))
#endif /* notdef */

#endif /* SOMOD_H */
