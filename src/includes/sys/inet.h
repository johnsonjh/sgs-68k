
#ifdef  NO_SCCS_ID
#ident "@(#)inet.h  (TWG)      1.5     89/09/07 "
#endif  /*NO_SCCS_ID*/

/*
 * @(#) Copyright 1986.  The Wollongong Group, Inc.  All Rights Reserved.
 */

#ifndef SYS_INET_H
#define SYS_INET_H

/*
 * local definitions for this implementation
 * catch all situation
 */

#ifndef ARIX
#define ARIX 1
#endif

#ifndef minor
#include <sys/sysmacros.h>
#endif

#ifdef DEBUG_TRACE
#define TRACE(mask,fmt,arg) 	{ extern int _trmask ; \
				if (_trmask & mask) printf(fmt, arg); }
#define TRACE2(mask,fmt,arg1, arg2) 	{ extern int _trmask ; \
				if (_trmask & mask) printf(fmt, arg1,arg2); }
#else
#define TRACE(mask, fmt, arg) {}
#define TRACE2(mask, fmt, arg, a2) {}
#endif /* DEBUG_TRACE */

#define HALFWORD_ALIGNED(ptr) (!((int)ptr & 1))
#define ODD_BOUNDARY(ptr) 	((int)ptr & 1)

/*
 * these probably belong to stream.h
 */
#define BLEN(bp) ((bp)->b_wptr - (bp)->b_rptr)
#define MLEN(bp) ((bp)->b_wptr - (bp)->b_rptr)
#define DLEN(bp) ((bp)->b_datap->db_lim - (bp)->b_datap->db_base)
#define	MTYPE(bp) (bp)->b_datap->db_type
#define	TTYPE(x)  ((x)->type)
#define	mtod(bp, type)	((type)((bp)->b_rptr))

#define SIGCHLD SIGCLD
/*
 * random selection
 */
#define	IP_ID		0x1351		/* 4945 decimal */
#define	TCP_ID		0x1352
#define	UDP_ID		0x1353
#define	ARP_ID		0x1354
#define	ICMP_ID		0x1355
#define	LOOP_ID		0x1356
#define	UI_ID		0x1357
#define	RAW_ID		0x1358
#define CIRC_ID		0x1359		/* AFUN Decimal 4953 */

/* sadlog/SADLOG level */
#define	DPRI_HI		20
#define	DPRI_MED	30
#define	DPRI_LO		40
#define	SL_te		SL_TRACE | SL_ERROR

typedef unsigned long u_long;
typedef unsigned long unlong;
typedef unsigned long n_long;
#ifdef XENIX
typedef unsigned long ulong;
#endif
typedef unsigned short u_short;
typedef unsigned short unshort;
typedef unsigned short n_short;
typedef unsigned char u_char;
#ifdef XENIX
typedef unsigned char uchar;	/* NBB */
typedef unsigned char unchar;
#endif
typedef unsigned int u_int;
typedef unsigned int unint;
typedef unsigned long inaddr_t;
typedef	unsigned long n_time;

#define	NBBY	8		/* number of bits in a byte */
/*
 * Select uses bit masks of file descriptors in longs.
 * These macros manipulate such bit fields (the filesystem macros use chars).
 * FD_SETSIZE may be defined by the user, but the default here
 * should be >= NOFILE (param.h).
 */
#ifndef	FD_SETSIZE
#define	FD_SETSIZE	256
#endif

typedef long	fd_mask;
#define NFDBITS	(sizeof(fd_mask) * NBBY)	/* bits per mask */
#ifndef howmany
#define	howmany(x, y)	(((x)+((y)-1))/(y))
#endif

typedef	struct fd_set {
	fd_mask	fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;

#define	FD_SET(n, p)	((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define	FD_CLR(n, p)	((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define	FD_ISSET(n, p)	((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)	bzero((char *)(p), sizeof(*(p)))

/*
 * Define Qhead
 */
struct qhead {
	struct	qhead *b_next;
	struct	qhead *b_prev;
	struct	qhead *b_cont;	/* ??? Is this needed */
};

/*
 * address format for tcp and udp: was two longs before
 * may have to change to character string
 */
struct tsap {
	short ta_family;
	unsigned short ta_port;		/* port number */
	ulong ta_addr;			/* internet address */
};
#define BINDLEN	sizeof(struct tsap)	/* kernel address length for tcp/udp */

#ifdef	u3b2
/*
 * These assembler directives are recognized only by the 3B2's
 * as far as we know.
 *	pam daniel
 */
#define LABEL(X)\
	asm("	.globl	X");\
	asm("X:");
#else
#ifdef ARIX

#define LABEL(X)\
	THING( global X );\
	THING( X: );

#define THING(X)\
        asm(#X);

#else
#define LABEL(X)
#endif	/* ARIX */
#endif	/* u3b2 */

#define	REUSEABLE(BP, SIZE) \
 (((BP->b_datap->db_ref == 1) && \
 (BP->b_datap->db_lim - BP->b_datap->db_base >= SIZE)) ? \
 freemsg(BP->b_cont), BP->b_cont = NULL,	/* XXX */ \
 BP->b_rptr = BP->b_wptr = BP->b_datap->db_base, 1 : 0)

/*
 * XXX: in sys/log.h, each routine should include log.h
 * STRLOG(mid,sid,level,flags,fmt,args) should be used for those trace
 * calls that are only to be made during debugging.
 */
#ifdef DEBUG
#define STRLOG	strlog
#else
#define STRLOG
#endif

#ifdef DEBUG_TRACE
#define STRlog strlog
#else
#define STRlog
#endif

/*
 * error code: should have been in errno.h
 * XXX: cleanup later
 * These are defined in P8 errno.h 
 */
#ifndef ARIX

#define	EWOULDBLOCK	200		/* Operation would block */
#define	EINPROGRESS	201		/* Operation now in progress */
#define	EALREADY	202		/* Operation already in progress */

/* ipc/network software */
	/* argument errors */
#define	ENOTSOCK	203		/* Socket operation on non-socket */
#define	EDESTADDRREQ	204		/* Destination address required */
#define	EMSGSIZE	205		/* Message too long */
#define	EPROTOTYPE	206		/* Protocol wrong type for socket */
#define	ENOPROTOOPT	207		/* Protocol not available */
#define	EPROTONOSUPPORT	208		/* Protocol not supported */
#define	ESOCKTNOSUPPORT	209		/* Socket type not supported */
#define	EOPNOTSUPP	210		/* Operation not supported on socket */
#define	EPFNOSUPPORT	211		/* Protocol family not supported */
#define	EAFNOSUPPORT	212		/* Address family not supported by protocol family */
#define	EADDRINUSE	213		/* Address already in use */
#define	EADDRNOTAVAIL	214		/* Can't assign requested address */

	/* operational errors */
#define	ENETDOWN	215		/* Network is down */
#define	ENETUNREACH	216		/* Network is unreachable */
#define	ENETRESET	217		/* Network dropped connection on reset */
#define	ECONNABORTED	218		/* Software caused connection abort */
#define	ECONNRESET	219		/* Connection reset by peer */
#define	ENOBUFS		220		/* No buffer space available */
#define	EISCONN		221		/* Socket is already connected */
#define	ENOTCONN	222		/* Socket is not connected */
#define	ESHUTDOWN	223		/* Can't send after socket shutdown */
#define	ETIMEDOUT	224		/* Connection timed out */
#define	ECONNREFUSED	225		/* Connection refused */

#define	EHOSTDOWN	226		/* Host is down */
#define	EHOSTUNREACH	227		/* No route to host */

#endif

#if defined( ARIX ) && ! defined( S90_REL_40 )
/* BSD Networking Software */
	/* argument errors */
#define	ENOTSOCK	95		/* Socket operation on non-socket */
#define	EDESTADDRREQ	96		/* Destination address required */
#define	EMSGSIZE	97		/* Message too long */
#define	EPROTOTYPE	98		/* Protocol wrong type for socket */
#define	ENOPROTOOPT	99		/* Protocol not available */
#define	EPROTONOSUPPORT	120		/* Protocol not supported */
#define	ESOCKTNOSUPPORT	121		/* Socket type not supported */
#define	EOPNOTSUPP	122		/* Operation not supported on socket */
#define	EPFNOSUPPORT	123		/* Protocol family not supported */
#define	EAFNOSUPPORT	124		/* Address family not supported by 
					   protocol family */
#define	EADDRINUSE	125		/* Address already in use */
#define	EADDRNOTAVAIL	126		/* Can't assign requested address */
	/* operational errors */
#define	ENETDOWN	127		/* Network is down */
#define	ENETUNREACH	128		/* Network is unreachable */
#define	ENETRESET	129		/* Network dropped connection because
					   of reset */
#define	ECONNABORTED	130		/* Software caused connection abort */
#define	ECONNRESET	131		/* Connection reset by peer */
#define	ENOBUFS		132	       	/* No buffer space available */
#define	EISCONN		133		/* Socket is already connected */
#define	ENOTCONN	134		/* Socket is not connected */
/* XENIX has 135 - 142 */
#define	ESHUTDOWN	143		/* Can't send after socket shutdown */
#define	ETOOMANYREFS	144		/* Too many references: can't splice */
#define	ETIMEDOUT	145		/* Connection timed out */
#define	ECONNREFUSED	146		/* Connection refused */
#define	EHOSTDOWN	147		/* Host is down */
#define	EHOSTUNREACH	148		/* No route to host */
#define EWOULDBLOCK	EAGAIN
#define EALREADY	149		/* operation already in progress */
#define EINPROGRESS	150		/* operation now in progress */
#endif

/*
 * LLC stuff
 */
#define	MACSIZE		6	/* 6 octets, or 48 bits */
#define	LC_REQSIZE	(sizeof(struct DL_unitdata_req) + EMDSAP_SIZE)
#define	LC_INDSIZE	(sizeof(struct DL_unitdata_ind)+EMDSAP_SIZE+EMDSAP_SIZE)

/* These should really be in /usr/include/sys/lihdr.h 
 * but are not as yet.
 */
#define DL_IPX25	5	/* required for ACC changes */
#define DL_PTOP		8	/* Missing in lihdr.h */
#define DL_LOOP        10	/* TWG definition. XXX danger of collosion */

/*
 * llc 802.2 address format
 */
struct llc_a {
	uchar	l_addr[MACSIZE];
	uchar	l_sap;
};
#define	LSAP_SIZE	7

/*
 * llc ethernet address format
 */
struct llc_aemd {
	uchar	l_addr[MACSIZE];
	ushort	l_prot;
};
#define	EMDSAP_SIZE	8

/*
 * llc ip address format
 */
struct llc_ip {
	ulong	l_addr;
	ushort	l_type;
	ushort	l_flag;
};

/*
 * 802.2 SAP numbers
 */
#define	IP_SAP		6
#define	ARP_SAP		7			/* illegal! */
#define SNAP_SAP	170
#define	URP_SAP		254

/*
 * ethernet protocol type
 */
#define	ETHERPUP_PUPTYPE	0x0200		/* PUP protocol */
#define	ETHERPUP_IPTYPE		0x0800		/* IP protocol */
#define	ETHERPUP_ARPTYPE	0x0806		/* ARP protocol */
#define	ETHERPUP_RARPTYPE	0x8035		/* RARP protocol */


/*
 * MAchine specific stuff
 */
#if defined(u3b2) || defined(ns32000) || defined(i386) 

#define NOFLOAT 1
#ifdef XENIX
#define splnet spl5
#else
#define splnet splstr	/* Keep the timein routines out	*/
#endif

#endif /* u3b2 */

#if defined( ARIX )
#define NOFLOAT 1
#define splnet	splstr
#define splni	splstr
#define spltime	splstr
#endif /* ARIX */

#ifndef i386
#ifdef INKERNEL
#ifdef u3b2
#define bzero(a,b)	w_bzero(a,b)  
#endif /* u3b2 */
#endif /* INKERNEL */
#endif /* i386 */

/*
 * The winsopt structure is used by socket option code
 * at the user and kernel levels.
 */
struct winsopt {
	int level;	/* protocol level */
	int name;	/* option name */
	int flags;	/* option flags */
};

#ifdef INKERNEL
#ifndef M_DATA
#include <sys/stream.h>
#endif /* M_DATA */
/*
 * We need these defined here as they are no longer macros on SVR3.2 systems
 * (they are functions) and would cause a panic if our product was compiled on
 * a SVR3.2 system and then installed on a SVR3.1 system!
 * This is really AT&T's problem.
 *
 * noenable - set queue so that putq() will not enable it.
 * enableok - set queue so that putq() can enable it.
 */
#ifndef ARIX
#ifndef noenable
#define noenable(q)	{ \
			register int s; \
			s = splstr(); \
			(q)->q_flag |= QNOENB; \
			splx(s); \
			}
#endif
#ifndef enableok
#define enableok(q)	{ \
			register int s; \
			s = splstr(); \
			(q)->q_flag &= ~QNOENB; \
			splx(s); \
			}
#endif
#endif /* ARIX */
#endif /* INKERNEL */

/*
 * AFUN ioctl structures for unix domain
 */
struct afun {
	short	afun_dev;		/* major minor dev #s for afun  */
	ushort	afun_ino;		/* inode # 			*/
};

#define SUNPATHLEN	108
#define LOOPBACK	0x7f000001

struct afuid {
	union {
		ino_t	afuino;
		ushort	afuport;
	} afutag;
	dev_t	afu_dev;
	int	afu_flen;
	unchar	afu_file[SUNPATHLEN];
};
#define afu_ino		afutag.afuino
#define afu_port	afutag.afuport
#define AFULEN		sizeof(struct afuid) 

/*  End of AFUN strucures */
#define TS_NOCHANGE	TS_NOSTATES + 1

struct timezone {
	int	tz_minuteswest;	/* minutes west of Greenwich */
	int	tz_dsttime;	/* type of dst correction */
};
#define	DST_NONE	0	/* not on dst */
#define	DST_USA		1	/* USA style dst */
#define	DST_AUST	2	/* Australian style dst */
#define	DST_WET		3	/* Western European dst */
#define	DST_MET		4	/* Middle European dst */
#define	DST_EET		5	/* Eastern European dst */

#ifndef	UTIME_H
#define	UTIME_H

struct	utimbuf {
	time_t	actime;
	time_t	modtime;
};

#endif	/* UTIME_H */

#endif /* SYS_INET_H */
