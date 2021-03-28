#ifndef SYS_PRIV_H 
#define SYS_PRIV_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) priv.h: version 24.1 created on 10/28/91 at 18:36:38	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)priv.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *
 *      priv.h  definition of privileges supported.
 * 
 */

/* system call function numbers */
#define GETPRIV         0
#define SETPRIV         1
#define GETFPRIV        2
#define SETFPRIV        3
#define SETPPRIV	4
#define SETROLE		5
#define GETROLE		6
#define SETSYSMASK	7
#define GETSYSMASK	8
#define	DELPRIV		9

/* Privilege definitions - each defined privilege is of one of two types:
 * security policy ovverrides or security policy required to perform some
 * operation. The comments will be SPO or SPR for each respectively
 */

/* MAC privileges */
#define	P_MAC_READ	0x00000001	/* SPO */
#define P_MAC_WRITE	0x00000002	/* SPO */
#define P_RLBL_OBJ	0x00000004	/* SPR - reqd to change file label */
#define P_RLBL_SUBJ	0x00000008	/* SPO - reqd to change proc label */
#define P_MAC_EXEMPT	(P_MAC_READ|P_MAC_WRITE|P_RLBL_OBJ|P_RLBL_SUBJ)


/* DAC privileges */
#define P_DAC_READ	0x00000010	/* SPO */
#define P_DAC_WRITE	0x00000020	/* SPO */
#define P_DAC_ACL	0x00000040	/* SPR - reqd to change obj acl */
#define P_DAC_EXEMPT	(P_DAC_READ|P_DAC_WRITE|P_DAC_ACL)


/* Privilege controls */
#define	P_PRIV_OBJ	0x00000100	/* SPR - reqd to change file priv */ 
#define P_PRIV_SUBJ	0x00000200	/* SPR - reqd to change proc priv */
#define P_INHERIT	0x00000400
#define P_PCNTRL	(P_PRIV_OBJ|P_PRIV_SUBJ|P_INHERIT)


/* I&A privileges */
#define P_SETUID	0x00001000	/* SPR - reqd to change proc uid */
#define P_SETGID	0x00002000	/* SPR - reqd to change proc gid */
#define P_ADDGRPS	0x00004000	/* SPR - reqd to change suppl groups */
#define P_OWNER		0x00008000	/* SPO - override owner checks */
#define P_INFO		(P_SETUID|P_SETGID|P_ADDGRPS|P_OWNER)


/* General Admin privileges */
#define P_CHROOT	0x00010000 	/* SPR - reqd to change idea of root */
#define P_LINK		0x00020000
#define P_ULIMIT	0x00040000	/* SPR - reqd to skip ulimit checks */
#define P_MKNOD		0x00080000	
#define P_UTIME		0x00100000	/* SPR - reqd to change update time */
#define P_RESOURCE	(P_CHROOT|P_LINK|P_ULIMIT|P_MKNOD|P_UTIME)


/* System Admin Relevant privileges */
#define P_ADM		0x00200000	/* SPR - reqd for stime,utime,acct */
#define P_FSYS		0x00400000	/* SPR - reqd for stripe,mirror,mount */
#define P_NICE		0x00800000
#define P_SYS    	(P_ADM|P_FSYS|P_NICE)


/* Security Relevant privileges */
#define P_SECURITY	0x01000000	/* SPR - reqd to do sec things */
#define P_KILL		0x02000000	/* SPR - reqd kill proc w/diff lbl */
#define P_AUDIT		0x04000000
#define P_SEC		(P_SECURITY|P_KILL|P_AUDIT)


/* inverse privileges - presence of these mean actions are restricted 
 * Note that P_EXEC only has meaning when MACs are used! It's a real 
 * pain in the ass when they aren't 
 */
#define P_EXECR		0x10000000	/* SPR - can only exec TCB files */
#define P_ID_EXECR	0x20000000	/* SPR - reqd to suid/gid on exer */
#define P_RESTRICT	(P_EXECR|P_ID_EXECR)


/* Default privilege role definitions - these are provided to make things
 * a little bit easier for applications. Note that P_ALL is supported but
 * should not be used in multiuser EVER!!! Thats what P_SYSADM and P_SECADM
 * are for: if P_ALL is really needed in runtime, get the privilege by starting
 * as P_SECADM (they can change priv's, but it will be audited).
 */

#define P_NULL		0x00000000 	/* no privileges */

/* traditional "super-user" - they can do it all */
#define P_ALL	(P_MAC_EXEMPT|P_DAC_EXEMPT|P_PCNTRL|P_SYS|P_INFO|P_RESOURCE|P_SEC)

/* systems admin - do everything a super-user can but bypass macs and
 * kill security processes. 
 */
#define P_SYSADM	(P_DAC_EXEMPT|P_INFO|P_SYS|P_RESOURCE) 

/* security admin - he can get to security stuff, set privilege stuff
 * etc. He can change his priv to do anything if he really needs to, 
 * but only through run-time TCB mediation!!! 
 */
#define P_SECADM	(P_SEC|P_PCNTRL|P_ID_EXECR)

/* operators can read everything but files protected by MACs 
 * they can also service simple resource needs for users 
 * but they can't modify files that are dac write protected.
 */
#define P_OPER		(P_DAC_READ|P_RESOURCE) 

/* normal user - he can execute setuid programs */
#define P_USR		(P_NULL)

/* restricted user - same as P_USR but can't execute any suid/sgid on 
 * exec stuff: he's really restricted!! */
#define P_RUSR		(P_RESTRICT)

/* developers usually control their own resources */
#define P_DEV 		(P_USR|P_SYS|P_RESOURCE)


/* priv_t - this structure defines the per-process privilege information
 *          The real privilege and effective privileges are similar in 
 *          concept to the real and effective uid's. The mask is used
 *          to limit, on a per-user or per-system basis, the inheritable
 *	    effective priv set that comes from a file or a suid on exec
 */
typedef struct priv {
	uint	real;		/* the process's real privilege set */
	uint	eff;		/* the process's effective priv set */
	uint	mask;		/* mask to limit effective priv set */
	} priv_t;

/*
 * priv_sym_t  structure used by utilities to convert priv bit maps 
 * to symbolic names and vica-versa 
 */
typedef struct priv_sym {
	char	p_name[14];	/* privilege name */
	uint	p_priv;		/* actual privilege */
	} priv_sym_t;

/* system wide privilege mask - set by the privs system call. Intially 
 * starts wide open on boot
 */
extern uint	sys_priv_mask;
extern uint	sec_mode;

/*
 *  priv_pmap - allows the mapping of priv names to priv maps. Note
 *  	       that this structure is organized with just the real 
 * 	       privileges. A second table, priv_cmap, contains the 
 * 	       maps for the combinationals 
 */

#ifdef PRIV_SYMS
priv_sym_t	priv_pmap[] = {
	"P_NULL", 0,
	"P_MAC_READ", P_MAC_READ,
	"P_MAC_WRITE", P_MAC_WRITE,
	"P_RLBL_OBJ", P_RLBL_OBJ,
	"P_RLBL_SUBJ", P_RLBL_SUBJ,
	"P_DAC_READ", P_DAC_READ,
	"P_DAC_WRITE", P_DAC_WRITE,
	"P_DAC_ACL", P_DAC_ACL,
	"P_PRIV_OBJ", P_PRIV_OBJ,
	"P_PRIV_SUBJ", P_PRIV_SUBJ,
	"P_INHERIT", P_INHERIT,
	"P_SETUID", P_SETUID,
	"P_SETGID", P_SETGID,
	"P_ADDGRPS", P_ADDGRPS,
	"P_OWNER", P_OWNER,
	"P_CHROOT", P_CHROOT,
	"P_LINK", P_LINK,
	"P_ULIMIT", P_ULIMIT,
	"P_MKNOD", P_MKNOD,
	"P_UTIME", P_UTIME,
	"P_ADM", P_ADM,
	"P_FSYS", P_FSYS,
	"P_NICE", P_NICE,
	"P_SECURITY", P_SECURITY,
	"P_SEC", P_SEC,
	"P_AUDIT", P_AUDIT,
	"P_KILL", P_KILL,
	"P_EXECR", P_EXECR,
	"P_ID_EXECR", P_ID_EXECR,
	"", 0 
	};

/* these are the combinational privileges - note they are placed in the
 * table so that the role combinationals are checked first, cause they 
 * use some of the combinationals fround later 
 */
priv_sym_t  priv_cmap[] = {
	"P_ALL", P_ALL,
	"P_SYSADM", P_SYSADM,
	"P_SECADM", P_SECADM,
	"P_OPER", P_OPER,
	"P_DEV", P_DEV,
	"P_USR", P_USR,
	"P_RUSR", P_RUSR,
	"P_MAC_EXEMPT", P_MAC_EXEMPT,
	"P_DAC_EXEMPT", P_DAC_EXEMPT,
	"P_RESTRICT", P_RESTRICT,
	"P_PCNTRL", P_PCNTRL,
	"P_INFO", P_INFO,
	"P_RESOURCE", P_RESOURCE,
	"P_SYS", P_SYS,
	"", 0
	};
#endif /* PRIV_SYMS */

#endif /* SYS_PRIV_H */
