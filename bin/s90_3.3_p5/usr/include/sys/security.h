#ifndef	SYS_SECURITY_H
#define	SYS_SECURITY_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) security.h: version 24.1 created on 10/28/91 at 18:37:53	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)security.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *
 *	security.h   security relevant headers for the kernel and 
 *	             specific user programs
 *
 */

#ifndef SYS_PRIV_H
#include "sys/priv.h"
#endif

#ifndef SYS_MLS_H
#include "sys/mls.h"
#endif

#define MACSINH		(1<<3)
#define MACSON		(1<<2)
#define	PRIVON		(1<<1)
#define SATON		1

/* bit definitions for kern_uidmap and kern_gidmap */
#define	IDLOCK		0x01		/* uid/gid is locked  */
#define IDRETIRE	0x02		/* uid/gid is retired */

/* kern_role - this structure represents the internal role table 
 * downloaded by trusted utilities, and maintained by the kernel. 
 * This table is used to validate roles and to assign privs and 
 * macs on setuid and suid-on-exec events. 
 */
typedef struct {
	char		role;			/* role number */
	priv_t		priv;			/* real privilege of role */
	slabel_t 	minlabel;		/* minimum label for role */
	slabel_t	maxlabel;		/* maximum label for role */
	char		status;			/* status of the role */
	} kern_role_t;

/* kern_uidmap - all utilities that add users to the system must download
 * the new uid/role information to the kernel before the user is really 
 * known to the system. The information downloaded must be sorted!!!
 */
typedef struct {
	ushort	uid;			/* usr's id */
	uchar	role;			/* usr role as def'd in kern_role */    
	uchar	status;			/* state of uid locked/retired etc. */
	} kern_uidmap_t;

/* kern_gidmap - trusted utilities will update the kernels idea of active
 * groups so the kernel may prohibit sgid-on-exec to gid's that no longer 
 * exist
 */
typedef struct {
	ushort  gid;
	ushort	status;
	} kern_gidmap_t;




#endif	/* SYS_SECURITY_H */
