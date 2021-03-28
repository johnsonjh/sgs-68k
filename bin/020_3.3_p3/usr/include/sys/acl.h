#ifndef SYS_ACL_H 
#define SYS_ACL_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) acl.h: version 24.1 created on 10/28/91 at 18:31:02	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)acl.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *
 *	acl.h	definition of structures used in an access control list
 * 
 */

#ifndef FsTYPE
#define FsTYPE 2
#endif

/* ACLs are always present in the system */
#define ACLMODE		1

/* ACL types */
#define DEFAULT_ACL	  1		/* default acl (directories) */
#define NAMED_ACL	  2		/* named acl */
#define UNAMED_ACL	  3		/* un-named acl */

/* The following are values to be found in the "type" field */
#define A_USER		1
#define A_GROUP 	2
#define A_TIME 		3
#define A_DATE		4

#if FsTYPE==2              /* 1K file sys */
#define MAX_ACL_BLK	8
#endif

#if FsTYPE==4             /* 4K file sys */
#define MAX_ACL_BLK	2
#endif

#define MAX_ACL_SIZE	BSIZE * MAX_ACL_BLK 

/* used in acl_search */
#define	GROUP_SH	3
#define OWNER_SH	6
#define MODE_MASK	7

	
/*The structure of each acl! All fields are not used by all types.*/
typedef struct acl {
	char type;
	char pad1;
	ushort id;
	ushort mode;
	ushort hour_start;
	ushort hour_end;
	ushort day_start;
	ushort day_end;
	ushort pad2;
	time_t	date_start;
	time_t	date_end;
} acl_t;


/* acl() functions */

#define	GETACL		0
#define	SETACL		1
#define	ACLSTAT		2
#define	GETACL_DEFAULT	3
#define	SETACL_DEFAULT	4
#define ACLSTAT_DEFAULT	5
#define ACL_LINK	6
#define ACL_UNLINK	7
#define ACL_LINK_DEFAULT 8
#define ACL_UNLINK_DEFAULT 9
#define	SETNACL		10


#endif	/* SYS_ACL_H */
