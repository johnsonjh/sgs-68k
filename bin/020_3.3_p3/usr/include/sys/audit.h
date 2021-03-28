#ifndef SYS_AUDIT_H
#define SYS_AUDIT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) audit.h: version 24.1 created on 10/28/91 at 18:31:11	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)audit.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *	audit.h   header files for kernel audit probes, audit drivers etc.
 */

#ifndef SYS_PRIV_H
#include "sys/priv.h"
#endif

#ifndef SYS_MLS_H
#include "sys/mls.h"
#endif

#ifndef SYS_SYNCH_H
#include "sys/synch.h"
#endif

#ifndef SYS_PARAM_H
#include "sys/param.h"
#endif

#define AUDIT_TRAIL_VERSION	0x02061991

#ifndef NGROUPS_MAX 
#define NGROUPS_MAX	16
#endif

#ifndef DIRSIZ 
#define DIRSIZ		NAME_MAX
#endif

#define INITPID		1			/* process id of init */
#define SATDEV		"/dev/sat"		/* name of node for sat drvr */
#define SATCOMM		"/dev/satcomm"		/* IPC to sat deamon */
#define SATCOMX		"/dev/satcomx"		/* auditc read side */
#define AUD_PATH 	"/usr/audit/"		/* default audit dir */
#define AUD_UIDS	"/dev/uids"		/* node to dwnld uid/roles */
#define AUD_ROLES	"/dev/roles"		/* node to dwnld roles id */
#define AUDMAXDIR	8			/* maximum number of dirs */


/*
 *	auditc and auditd signals  these signals are used to sync comm 
 *	at the shared pipes between these two guys 
 */ 
#define NEWSESS		SIGINT			
#define GETSESS		SIGILL
#define KILLSESS	SIGTERM
#define NEWFILE		SIGQUIT
#define GETFILE		SIGHUP

/* state contoles for the sat internal structures */
#define	SAT_ENABLE	0x01
#define SAT_OPEN	0x02
#define SAT_SLEEP	0x04
#define SAT_TIMER	0x08
#define SAT_TEMP	0x10

/*
 *	audit system call functions 
 *
 * audit system call functions. One system entry point is provided 
 * and function code are passwd to indicated which particular action the 
 * audit system call should perform. One particular function, the AUDIT_XXXCONF
 * functions requires a third sub-function specifier
 */
#define	AUDIT_START	0			/* start the audit trail */
#define AUDIT_STOP	1			/* stop the audit trail */
#define AUDIT_SET    	2			/* get audit configuration */
#define AUDIT_GET    	3			/* get audit configuration */
#define AUDIT_MASK      4			/* set the audit event mask */
#define AUDIT_STAT	5			/* get audit driver stats */
#define AUDIT_EVTSTAT	6			/* get the audit event stats */
#define AUDIT_WRITE	7			/* write a audit record */
#define AUDIT_SUMASK	8			/* set user audit mask */
#define AUDIT_GUMASK 	9			/* get user audit mask */
#define AUDIT_AUID  	10		
#define AUDIT_MAX	11

/* 			
 *	AUDCONF fucntion code definitions 
 *
 * The following are the allowed sub-functions for the audit system call
 * AUDIT_XXXCONF function.  Note that AUDCONF_VERS is only valid with the 
 * AUDIT_GETCONF function (ie: it's not settable).
 */

#define AUDCONF_HWM	0		/* get/set high water mark for sat */
#define AUDCONF_HZ	1		/* get/set HZ before write */
#define AUDCONF_MASK	2		/* get/set event mask */
#define AUDCONF_VERS	3		/* get version of audit trail */

/*
 *	audit event definitions 
 * 
 */

/* process oriented events */
#define	AUD_FORK	0	/* process creation */
#define AUD_EXEC	1	/* image execution */
#define AUD_EXIT	2	/* process termination */
#define AUD_SETUID	3	/* process uid changes */
#define AUD_SETGID	4	/* process group changes */
#define AUD_SETSID	5	/* process session/pgrp changes */
#define AUD_SUPPL	6	/* process suplemntary group changes */
#define AUD_ROLE	7	/* process role changes */
#define AUD_PRIV	8	/* process privilege changes */
#define AUD_LABEL	9 	/* process label changes */
#define AUD_CHROOT	10	/* process change root */
#define AUD_CTTY	11 	/* change a process ctty */
#define AUD_KILL	12	/* send signal to process */	

/* file audit events */
#define AUD_ACCSR	13	/* file access success read */
#define AUD_ACCSW	14	/* file acess sucess write */
#define AUD_ACCFR	15	/* file access fail read   */
#define AUD_ACCFW	16	/* file access fail write */
#define	AUD_MKNOD	17	/* mknod/mkfifo  */
#define AUD_PIPE	18	/* create a pipe */
#define AUD_CREAT	19	/* reserved for file creation events */
#define AUD_LINK	20	/* link new name to a file */
#define AUD_RENAME	21	/* file rename */
#define AUD_UNLINK 	22	/* file unlink */
#define AUD_CHMOD	23	/* change mode of a file */
#define AUD_CHOWN	24	/* change owner of a file */
#define AUD_CHGRP	25	/* change files group */
#define AUD_FILEPRIV	26	/* change effective privlige */
#define AUD_FILELABEL	27	/* change a files label */
#define AUD_ACL		28	/* change files acl */

/* file system/system call oriented events */
#define AUD_MOUNT	29	/* mount */
#define AUD_UMOUNT	30	/* audit unmount request */
#define AUD_SYSERR	31	/* failed system call */
#define AUD_CLOCK	32	/* system clock changes */

/* IPC oriented events */
#define	AUD_IPCREATE	33	/* ipcreate function */
#define AUD_IPCACCESS	34	/* ipc access record */
#define AUD_IPCRM	35	/* ipc remove function */
#define AUD_IPCLABEL	36  	/* reserved for B1 */
#define AUD_IPCDAC	37	/* used for any mode/owner/groups */
#define AUD_IPCACL	38	/* change ipc acl */

/* ULI's  (user level audit events */
#define AUD_LOGIN	39	/* login/logout events */
#define AUD_AUDIT	40	/* audit control events */
#define AUD_USRINFO  	41	/* user account info changes */
#define	AUD_DEVINFO	42	/* device info changes */
#define AUD_LP		43	/* lp subsystem events */
#define AUD_CRON	44	/* cron events */
#define AUD_UUCP	45	/* uucp events */
#define AUD_ULI		46	/* user specific audit events */

/*  network oriented events as sub-events...for now, reserve space. */
#define AUD_TCP    	47	/* network events  */
#define AUD_NFS		48	/* nfs events */
#define AUD_RFS		49	/* rfs generated events */
#define AUD_SLINK	50	/* symbolic link */
#define	AUD_MAX_DEF	51	/* maximum defined events */
/* JTOF:
 *        gap for extra events has been intentionally left! These event numbers
 *	  should only be assigned to events that must be first level only. The
 *	  total number of first level events is 63...not 64...the last event 
 *	  number is to be used to denote extended events. These events will 
 *	  be named AUDX_xxxx in the future 
 */
#define AUD_EXTEND	64	
#define AUD_MAX_EVT	64

/* 
 *	audit record mode definitions  
 */

/* sat_access mode definitions */
#define	ACCFAIL 	0x80		/* flag indicating pass/fail stat */
#define ACCMODE		0x01		/* pass/fail on mode bits */
#define ACCPRIV		0x02		/* pass/fail on privileges */
#define ACCACL		0x04		/* pass/fail on acl override */
#define ACCMAC		0x08		/* fail on invalid label */
#define ACCTIME		0x10		/* fail on bad time */
#define ACCOWNER	0x20		/* pass on owner */


/*
 *	sat_login flag bits, tell audit trail whether the record was 
 *		  written bu su or login and whether the attempts was
 *		  sucessfull or failed  
 */
#define	LOGIN_ID	0x01
#define SU_ID		0x02
#define	LOGIN_FAIL	0x80

/* sat_login mode bits tell us about good and bad logins. If a login is
 * fails, the other bits tell us why it failed.
 */
#define	LOGIN_BADTIME	0x01
#define LOGIN_BADPWD	0x02
#define LOGIN_BADUID	0x04
#define LOGIN_LOCKED	0x10
#define LOGIN_RETIRED	0x20
#define LOGIN_TTY_LOCK	0x40
#define LOGIN_TTY_NOPERM 0x80

/* sat_usrinfo mode bits tell us whether an attempt to change the user_auth
 * was sucessfull or not. It marks why a passwd attempt was denied if it
 * was denied 
 */
#define BADCHANGE	0x0001		/* attempt to change failed */
#define NEW_UNAME	0x0002		/* new users added */         
#define NEW_UID		0x0004
#define NEW_GID		0x0008
#define NEW_PASSWD	0x0010		/* passwd changes */
#define NEW_DIR		0x0020
#define NEW_SHELL	0x0040
#define NEW_ROLE	0x0080
#define NEW_PWDPARM	0x0100		/* min/max and gen changes */
#define NEW_MAXTRY	0x0400
#define NEW_AMASK	0x0800
#define NEW_TWIN	0x1000
#define ACCTLOCKED	0x2000		/* account status changes */
#define ACCTRETIRED	0x4000
#define ACCTUNLOCKED	0x8000

/* sat_devinfo mode bits - these bits define the fields that have 
 * changed in the dev_auth files
 */

#define NEW_TTY_DEV		0x0002
#define NEW_TTY_MAXTRY		0x0004
#define NEW_TTY_DELAY		0x0008
#define NEW_TTY_MINLBL		0x0010
#define NEW_TTY_MAXLBL		0x0020
#define NEW_TTY_USRS		0x0040
#define NEW_TTY_GROUPS		0x0080
#define TTY_LOCKED		0x0100
#define TTY_UNLOCKED		0x0200


/* sat_audit mode bits - these bit define the actvities performed on the
 * audit trail 
 */
#define	AUDMASK		0x01		/* mask was changed */
#define AUDSTART	0x02		/* audit trail was started */
#define AUDSTOP		0x04		/* audit trail was stopped */


/* the evt_state strucutre contains per event information. This
 * information will be stored for SEC queries etc. 
 */
typedef struct evt_state {
	time_t		edtime;		/* time event last enable/dis */
	uint		tot_recs;	/* total records since startup */
	uint		cur_recs;	/* total records since last ena/dis */
	uchar		seq;		/* sequence number */
	uchar		state;		/* current event state */
	} evt_state_t;

/* the aud_stat structure contains information about the current state of
 * the security audit driver. This included per-event information as well
 * as general usage statistics. This routine is set/retrieved by the audit
 * system call
 */	
typedef struct aud_state {
	ushort	state;
	ushort	hwm;			/* high water mark */
	ushort	pid;			/* process that opened device */
	ushort	callout;		/* HZ * seconds */
	uint	mask[2];		/* mask stores up to 256 events */
	char	*h;			/* head of buffer */
	char	*r;			/* read pointer */
	char	*w;			/* write pointer */
	char	*e;			/* end of buffer */
	uint	rcnt;			/* byte written in buffer */
	uint	bcnt;			/* free bytes in buffer */
	uint	tot_recs;		/* total sat records recorded 	
				           from boot_time */
	uint	cur_recs;		/* total sat record recorded 
				           from last start_time */
	uint	buf_recs;		/* buffered records */
	time_t	start_time;		/* time audit started */
	time_t	open_time;		/* time device opened */
	time_t	end_time;		/* end time */
	time_t	boot_time;		/* time system was booted */
	time_t	close_time;		/* time the driver was last closed */
	uchar	version;		/* audit version number */
	evt_state_t evts[AUD_MAX_EVT];	/* one structure per event */
	spin_lock_t	lock;
	} aud_state_t;
/*
 *
 * 	audit record definitions 
 * 
 */

/* generic security audit trail header */
typedef struct sat_hdr {
	uchar	event;		/* event id for the rest of the record */
	uchar	size;		/* size of the audit record */
	uchar	cpu;		/* cpu that the event occured upon */
	uchar 	seq;		/* sequence of the event */
	ushort	pid;		/* process id event generated for */
	time_t	time;		/* time event occured */
	} sat_hdr_t;

/* AUD_FORK event record */
typedef struct sat_fork { 
	priv_t	priv;		/* keep privileges in case of masking */
	ushort 	ppid;		/* parent of newly created proc */
	} sat_fork_t;

/* AUD_EXEC event record - we have to define the role and the real
 * privileges to handle the case of suid-exec privilege attentuation and
 * effective privileges set in files 
 */
typedef struct sat_exec {
	ushort	auid;		/* audit id of the proc */
	ushort	ruid;		/* user id of the proc */
	ushort	rgid;		/* group id of the proc */
	ushort	euid;		/* effective uid of the proc */
	ushort	egid;		/* effective group of the proc */
	ushort	sessid;		/* session id of process */
	ushort	pgrp;		/* pgrp of process */
	dev_t	ctty;		/* keep track of the controlling tty */
	dev_t 	dev;		/* dev_t of the disk file inode */
	ushort	ino;		/* disk inode number */
	uchar	role;		/* role id of the process */
	priv_t	priv;		/* privilege of the process */
	slabel_t minlabel;	/* minimum label for the process */
	slabel_t maxlabel;	/* max label process may assume */
	ushort	groups[NGROUPS_MAX]; 
	char	name[DIRSIZ];	/* u_comm for process */
	} sat_exec_t;

/* AUD_EXIT event record */
typedef struct sat_exit {
	char	flag;		/* process exit flag ( as per acct.h) */
	uint  	status;		/* process status field */
	uint	pused;		/* privileges used by process */
	} sat_exit_t;

/* AUD_SETUID event record - note that setuid will cause the kernel to also
 * change the priv and labels for the uid if caller is privileged, but another
 * audit record will be generated by this call in such a case (aud_priv,
 * aud_label, or aud_setrole )
 */
typedef struct sat_setuid {
	ushort	auid;		/* audit id for process */
	ushort	ruid;	 	/* real id for process */
	ushort	euid;     	/* effective id for process */
	ushort	role;		/* role definition */
	priv_t	priv;		/* inherited privileges - result
				 * after mask. Labels not stored
			         * cause role tells us what they are!
			         */
	} sat_setuid_t;

/* AUD_SETGID event record */
typedef struct sat_setgid {
	ushort 	rgid;		/* real group id */
	ushort	egid;		/* effective group id */
	} sat_setgid_t;

/* AUD_SESSID event record - keep session id information to track
 * any fancy games sophiticated users may play with controlling tty's
 * etc. */
typedef struct sat_setsid {
	uchar	mode;		/* setpgrp = 0  or setsid  = 1*/
	ushort	old_sessid;	/* old session id */
	ushort	old_pgrp;	/* old process group */
	dev_t	old_ctty;	/* old controlling */
	dev_t	new_ctty;	/* new controlling terminal */
	} sat_setsid_t;

/* AUD_SUPP event record */
typedef struct sat_setsupp {
	ushort	groups[NGROUPS_MAX];
	} sat_setsupp_t;

/* AUD_ROLE event record - setting the role will only effect the 
 * real privilege. We also want to monitor the effective privileges
 * that come from a file so we keep track of that in this inteface also 
 * note: this record will typcally come from the setuid call, but can
 *       also come from the setrole system call
 */
typedef struct sat_setrole {
	uchar		new_role;	/* new role number */
	priv_t		priv;		/* new privilege */        
	} sat_setrole_t;

/* AUD_PRIV event record - note that a process privielege can change
 * ie: a process can give away privileges that it doesn't want. In 
 * such cases it nolonger maps to a role...also, we do support a chrpriv
 * system call and we must audit it's affects!
 */
typedef struct sat_priv {
	priv_t		priv;
	} sat_priv_t;

/* AUD_LABEL event record - as witht the aud_priv record, the labels of 
 * a process will typically change via setuid(), but a setlabel routine 
 * is available that may also be used.
 */
typedef struct sat_label {
	slabel_t	minlabel;
	slabel_t	maxlabel;
	} sat_label_t;

/* AUD_CHROOT event record - */
typedef struct sat_chroot {
	dev_t	dev;		/* device of new root */
	ushort	ino;		/* inode of new root */
	} sat_chroot_t;

/* AUD_KILL event record - keep track of inter-process signals   */
typedef struct sat_kill {
	ushort	from_pid;
	int	signal;
	} sat_kill_t;

/* AUD_ACCESS event record - keep track of all sucessfull and unsucessfull
 * access attempts to a file. Note: don't need label here cause this call
 * only records access to existing files (unlike V/MLS).  
 */
typedef struct sat_access {
	uchar 	mode;		/* mode describes access information */
	uchar	type;		/* status of access attempt */
	dev_t	dev;		/* device file resides on */
	ushort	ino;		/* inode number of file */
	} sat_access_t;

/* AUD_MKNOD event record - this is used by mknod and mkfifo */
typedef struct sat_mknod {
	ushort	mode;		/* as defined in file.h */
	slabel_t label;		/* label of file created */
	dev_t	dev;		/* device node resides on */
	ushort	ino;		/* inode of device special */
	ushort	dino;		/* inode of files directory */
	ushort	aino;		/* inode of default acl */
	char	name[DIRSIZ];	/* basename of file created */
	} sat_mknod_t;

/* AUD_PIPE event record  */
typedef struct sat_pipe {	   
	dev_t	dev;		/* device node resides on */
	ushort	ino;		/* inode of file */
	} sat_pipe_t;

/* AUD_CREATE event record - currently unused ! */
typedef struct sat_create {
	ushort	mode;		/* as defined in file.h */
	slabel_t label;		/* label of file created */
	dev_t	dev;		/* device node resides on */
	ushort	ino;		/* files inode */
	ushort	dino;		/* inode of files directory */   
	ushort	aino;		/* inode of default acl */
	char	name[DIRSIZ];	/* basename of file created */
	} sat_create_t;

/* AUD_LINK event record */
typedef struct sat_link {
	dev_t	dev;		/* device file resides on */   
	ushort	ino;		/* inode of file */
	ushort	dino;		/* inode of directory */
	slabel_t label;		/* label of file created */
	char	name[DIRSIZ];	/* basename of file created */
	} sat_link_t;

/* AUD_SLINK event record */
typedef struct sat_slink {
	dev_t	dev;		/* device file resides on */   
	ushort	ino;		/* inode of link */
	ushort	dino;		/* inode of directory */
	slabel_t label;		/* label of link created */
	char	name[DIRSIZ];	/* basename of file created */
	ushort	path_length;	/* length of target path */
	char	target[PATH_MAX]/* target path */
	} sat_slink_t;

/* AUD_RENAME event record - */
typedef struct sat_rename {
	dev_t	dev;		/* device file resides on */
	ushort	ino;		/* inode file resides on */
	ushort	dino;		/* inode of directory */
	slabel_t label;		/* label of file renamed */
	char	name[DIRSIZ];	/* basename of file created */
	} sat_rename_t;

/* AUD_UNLINK event record */
typedef struct sat_unlink {
	dev_t	dev;		/* device file redied on */
	ushort	ino;		/* inode of file */
	ushort	dino;		/* inode of directory */
	slabel_t label;		/* label of file */
	char	name[DIRSIZ];	/* basename of file created */
	} sat_unlink_t;

/* AUD_CHMOD event record */
typedef struct sat_chmod {
	dev_t	dev;		/* device the file resides on */
	ushort	ino;		/* inode of the file */
	ushort	old_mode;	/* mode of the file */
	ushort	new_mode;	/* new mode of the file */
	} sat_chmod_t;

/* AUD_CHOWN event record */
typedef struct sat_chown {
	dev_t	dev;		/* device the file resides on */
	ushort	ino;		/* inode of the file */
	ushort	old_uid;	/* old user */
	ushort	new_uid;	/* new user */
	} sat_chown_t;

/* AUD_CHGRP event record */
typedef struct sat_chgrp {
	dev_t	dev;
	ushort	ino;
	ushort	old_gid;
	ushort	new_gid;
	} sat_chgrp_t;

/* AUD_FILEPRIV event record - record change in files effective privilege */
typedef struct sat_filepriv {
	dev_t	dev;
	ushort	ino;
	uint	old_priv;
	uint	new_priv;
	} sat_filepriv_t;

/* AUD_FILELABEL event record */
typedef struct sat_filelabel {
	dev_t	dev;
	ushort	ino;
	slabel_t old_label;
	slabel_t new_label;
	} sat_filelabel_t;
	

/* AUD_ACL event record - record changes of acl entries on file 
 * note that this audit record is VERY expensive...we must essentially
 * copy the contents of the acl into the audit record! Thats what the 
 * aclsize field is for...tells us how much data follows this record.
 * Becuase the whole acl is copies, no create/update differences are
 * necessary */
typedef struct sat_acl {
	uchar	mode;		/* see aud_acl mode bit definition */
	dev_t	dev;		/* files device */
	ushort	ino;		/* files inode */
	ushort	aino;		/* acl's inode */
	ushort	aclsize;	/* size of the acl */
	} sat_acl_t;

/* AUD_MOUNT event record */
typedef struct sat_mount {
	uchar	mode;		/* same as for file access */
	dev_t	mtptdev;	/* mount point device */
	ushort	mtptino;	/* mount point inode */
	dev_t	fsdev;		/* fs device */
	dev_t	fsrootdev;	/* fs root device */
	ushort	fsrootino;	/* fs root inode */
	} sat_mount_t;

/* AUD_UMOUNT event record */
typedef struct sat_umount {
	uchar	mode;		/* same as for file access */
	dev_t	mptdev;		/* mount point device */
	ushort	mptptino;	/* mount point inode */
	dev_t	fsdev;
	dev_t	fsrootdev;
	ushort	fsrootino;
	} sat_umount_t;

/* AUD_SYSERR event record */
typedef struct sat_syserr {
	uchar	sysno;		/* system call number */
	uchar	errno; 		/* errno returned */
	} sat_syserr_t;

/* AUD_CLOCK event record */
typedef struct sat_clock {
	time_t	time;		/* record the time change */
	} sat_clock_t;

/* AUD_IPCREATE event record */
typedef struct sat_ipccreat {
	int	id;		/* ipc id */
	int	key;		/* ipc key */
	ushort	uid;		/* ipc owner */
	ushort	mode;		/* ipc protection bits */
	uchar	sysno;		/* sysno to identify type of ipc */
	slabel_t label;		/* ipc label */
	} sat_ipccreat_t;

/* AUD_IPCACCESS event record */
typedef struct sat_ipcaccess {
	uchar	mode;		/* access mode (same as for a file) */
	int	id;		/* ipc id */
	ushort	ipcmode;	/* ipc mode bits */
	uchar	sysno;		/* sysno to identify type of ipc */
	slabel_t label;		/* ipc label */
	} sat_ipcaccess_t;

/* AUD_IPCRM event record */
typedef struct sat_ipcrm {
	int	id;		/* ipc id */
	int	key;		/* ipc key */
	uchar	sysno;		/* sysno for ipc type */ 
	slabel_t label;			/* IPC labels are a B1ism */
	} sat_ipcrm_t;

/* AUD_IPCLABEL */
typedef struct sat_ipclabel {
	int	id;
	int	key;
	int	sysno;
	slabel_t new_label;		/* IPC labels are a B1ism */
	slabel_t old_label;
	} sat_ipclabel_t;

/* AUD_IPCDAC  record IPC dac changes including mode, owner, group info */
typedef struct sat_ipcdac {
	ushort	old_uid;
	ushort	old_gid;
	ushort	old_mode;
	ushort 	new_uid;
	ushort	new_gid;
	ushort	new_mode;
	int	id;
	int	sysno;
	} sat_ipcdac_t;

/* AUD_LOGIN event record */
typedef struct sat_login {
	uchar	flag;		/* see aud_login mode definitions bits */
	uchar	mode;		/* see aud_login mode definition bits */
	dev_t	tty;		/* device attempted login on */
	char	uname[DIRSIZ];	/* uname of user trying to login in */
	} sat_login_t;

/* AUD_AUDIT event record  - record control over audit trail */
typedef struct sat_audit {
	uchar	mode;
	ushort	version;
	uint	mask[2];
	} sat_audit_t;


/* AUD_USRINFO */
typedef struct sat_usrinfo {
	uint	mode;		/* see aud_pwd mode definition bits */
	uchar	uname[DIRSIZ];	/* uname that has been altered */
	} sat_usrinfo_t;

/* AUD_DEVINFO */
typedef struct sat_devinfo {
	uint	mode;
	uchar	ttyn[DIRSIZ];
	} sat_devinfo_t;

/* AUD_CRON */
typedef struct sat_cron {
	uchar	mode;	
	ushort	uid;		/* the user we start job for */
	dev_t	file_dev;	/* the exec'd file's dev */
	ushort	file_ino;	/* the exec'd file's inode */
	} sat_cron_t;

	
/* AUD_LP */
typedef struct sat_lp {
	uchar	mode;		/* whether print attempts was correct */
	ushort	uid;
	ushort	gid;
	dev_t	file_dev;	/* keep information on file being printer */
	ushort	file_ino;	
	dev_t	pr_dev;		/* keep information on printer node */
	ushort	pr_ino;
	char	pname[DIRSIZ];
	} sat_lp_t;
	

/* AUD_CTTY - this record is included to catch smart processes that 
 * call setsid/setpgrp and then open a device, inheriting it as a ctty.
 * The audit parser must then find the proper session structure and 
 * update the ctty
 */
typedef struct sat_ctty {
	ushort	sessid;
	dev_t	ctty;
	} sat_ctty_t;

/* more audit tail events to come!!! particularly for the sub-systems */	
#endif /* SYS_AUDIT_H */
