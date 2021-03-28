/*
 *	auth.h    definition of the authorization structures
 *	(C) 1989, 1990 by ARIX Corp.
 *
 */

#ifndef	AUTH_H
#ident	"@(#)auth.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#define	AUTH_H

#ifndef SYS_TYPES_H
#include <sys/types.h>
#endif

#ifndef SYS_PRIV_H
#include <sys/priv.h>
#endif

#ifndef SYS_MLS_H
#include <sys/mls.h>
#endif

#ifndef SYS_SAKIOCTL
#include <sys/sakioctl.h>
#endif

#ifndef LIMITS_H
#include <limits.h>
#endif

#define TTYAUTH_A	"/etc/security/dev_auth.a"
#define	TTYAUTH		"/etc/security/dev_auth"
#define TTYSTAT_A	"/etc/security/dev_stat.a"
#define TTYSTAT		"/etc/security/dev_stat"
#define USRAUTH_A	"/etc/security/user_auth.a"
#define USRAUTH		"/etc/security/user_auth"
#define USRSTAT_A	"/etc/security/user_stat.a"
#define USRSTAT		"/etc/security/user_stat"
#define ROLES		"/etc/security/roles"

#define MAXCOMMENT	40
#define MAXUG		64
#define MAXPATH		40

#define GRPFLE		"/etc/group"
#define OGRPFLE		"/etc/ogroup"
#define GRPTMP		"/etc/gtmp"
#define SGRPFLE		"/etc/security/grp_auth"
#define OSGRPFLE	"/etc/security/ogrp_auth"
#define SGRPTMP		"/etc/security/grp_atmp"

#define	DAY		(24L * 60 * 60) /* 1 day in seconds */
#define DAY_NOW		(long)time((long *)0) / DAY
#define TTY_TIMEOUT	30

/* twin - defines the access times a user is allowed on the system on a 
 *        per day basis. Format is 00:00 to 24:00
 */
typedef struct twin {
	char 	st_hr;			/* starting hour */
	char	st_min;			/* starting min */
	char	et_hr;			/* ending hour */
	char	et_min;			/* ending minute */
	} twin_t;


/* usrauth  - defines the user authorization structure for the 
 *            user. All information necessary to identify and authenticate
 *            the user, plus all privilege information can be found 
 *            in this structure. This structure also defines the 
 *            record format of the user_auth database!
 */

typedef struct usrauth {
	char		ua_name[NAME_MAX];	/* user name */
	ushort		ua_uid;			/* users identification */
	ushort		ua_gid;			/* default group */
	char		ua_passwd[NAME_MAX];	/* encrypted passwd */
	char		ua_gecos[MAXCOMMENT];	/* comment field */
	char		ua_defdir[MAXPATH];	/* default login dir */ 
	char		ua_defshell[MAXPATH];	/* default login shell */
	int		ua_role;		/* role number */
	char		ua_mode;		/* is passwd required? */
	int		ua_min_pwdlife;		/* min password life */
	int		ua_max_pwdlife;		/* max password life */
	int 		ua_maxtries;		/* maximum login attempts*/
	time_t		ua_acctstat;		/* account status */
	int		ua_amask[2];		/* audit mask */
	twin_t		ua_times[7];		/* access times */
	} usrauth_t;

#define NOLOCK 0
#define LOCKED	1	/*entry for acctstat field */
#define RETIRE	2    

/* bit definitions for the ua_mode field */
#define NO_PASS_NEED	0x80	/* bit for password required */
#define NO_PASS_CHANGE	0x40	/* bit for password change by user */
#define MODE_MASK	0xc0	/* mask for mode bits */
#define MODE_SHIFT	6	/* shift value for mode bits */
#define NO_CHANGE(x)	(NO_PASS_CHANGE & (x))
#define PASS_NEED(x)	(!(NO_PASS_NEED & (x)))
#define PWGEN_MASK	0x07
#define AUTO_PASS(x)	((x) & PWGEN_MASK)
#define	AUTO_MODE(x)	(((x) & MODE_MASK) >> MODE_SHIFT)

/*
 *  usrstat - this structure defines a users login and general account 
 *	      status information. It is updated on every good and bad
 *            attempt to access the users account. This structure also
 *            defines the record structure of the usr_stat database.
 */
typedef struct usrstat {
	uid_t		us_uid;	
	time_t		us_lastlogin;		/* last login time */
	time_t		us_last_try;		/* last login try */
	time_t		us_pwdchg;		/* last password change */
	time_t		us_lastsu;		/* last sucessfull su */
	time_t		us_lastbadsu;		/* last bad su attempt */
	time_t		us_lastnetlogin;	/* last network login */
	int		us_badtries;		/* actual bad logins */
	uint		us_reason_failed;	/* reason login failed */
	char		us_ttyn[NAME_MAX];	/* last port logged onto */
	char		us_ttyn_try[NAME_MAX];	/* last tty login tries on */
	char		us_ttyn_su[NAME_MAX];	/* tty of last su */
	char		us_ttyn_badsu[NAME_MAX];/* tty of last bad su attempt */
	char		us_host[NAME_MAX];	/* host of last login */
	char		us_name[NAME_MAX];	/* remote user of login */
	} usrstat_t;

/* reasons login failed */
#define	LOGIN_FAILED_BADTIME		0x01
#define	LOGIN_FAILED_BADPASWORD		0x02
#define	LOGIN_FAILED_BADUID		0x04
#define	LOGIN_FAILED_NETP		0x08
#define	LOGIN_FAILED_USER_LOCKED	0x10
#define	LOGIN_FAILED_USER_RETIRED	0x20
#define LOGIN_FAILED_TTY_LOCKED		0x40
#define LOGIN_FAILED_TTY_NOPERM		0x80

/* grpauth - Defines group authorization structure.  This structure is
	     the same as /etc/group, with the addition of acctstat, which
	     shows whether the group is locked or not */
typedef	struct	grpauth {
	char	*ga_name;
	gid_t	ga_gid;
	char	*ga_passwd;
	char	**ga_mem;
	time_t	ga_acctstat;
	} grpauth_t;

/* modes for calling the updateuaent routine */
#define JUST_STAT	1
#define JUST_TA		2
#define JUST_UA		2
#define DO_BOTH		3

/* password generator values */
#define PWGEN0		0
#define PWGEN1		1
#define PWGEN2		2
#define PWGEN3		3
#define PWGEN4		4
#define PWGEN5		5
#define PWGEN6		6
#define PWGEN7		7

/* Initial role values */
#define ROOT_ROLE	0
#define SA_ROLE		1
#define SEC_ROLE	2
#define OP_ROLE		3
#define USR_ROLE	4

#define	MAX_ROLE	100

/* roledef structure - this structure allows symbolic names to be given 
 *                     to specific combimations of privileges and macs
 */
typedef struct roledef {
	char		r_name[NAME_MAX];	
	unsigned	r_rolenumber;
	priv_t		r_priv;			/* default privileges */
	slabel_t	r_minlabel;		/* min operating label */
	slabel_t	r_maxlabel;		/* max operating label */
	} roldef_t;

/*
 *  ttyauth - this structure defines the authorization and setup information
 *	      for any port that may function as a login port
 */

typedef struct ttyauth {
	char		ta_ttyn[NAME_MAX];	/* name of tty */
	ushort		ta_maxtry;		/* bad logins max */
	ushort		ta_delay;		/* No longer in use IGNORED */
	struct sakioctl	ta_sak;			/* sak control information */
	ushort		ta_inactive;		/* max period of inactivity */
	time_t		ta_status;		/* status of tty */
	ushort		ta_tsentry;		/* stat file entry number */
	slabel_t	ta_minlabel;		/* minimum label for tty */
	slabel_t	ta_maxlabel;		/* maximum label for tty */
	ushort		ta_users[MAXUG];	/* list of users  cleared */
	ushort		ta_groups[MAXUG];	/* list of groups cleared */
	} ttyauth_t;

/*
 *   ttystat  - this struct holds status information on the state of the
 *              login port. This information includes last good login, 
 *              last bad attempt, last uid on tty and number of tries 
 *              currently oustanding on tty 
 */

typedef struct ttystat {
	time_t		ts_lastlogin;		/* last good login on tty */
	time_t		ts_lasttry;		/* last bad login on tty */
	ushort		ts_lastuid;		/* last user on tty */
	ushort		ts_tries;		/* current # bad tries */
	uint		ts_reason_failed;	/* reason bad login failed */
	} ttystat_t;
	
struct sgroup {
	char *sg_name ; /* user name */
	char *sg_pwd  ; /* user password */
	long sg_mode; /* determine passwd format */
	};

	
/* library definition files */
extern ttyauth_t	*gettaent(), *gettanam();	

extern int 		putgrent(), lckgrpf(), ulckgrpf() ;

extern int		updateuaent();
extern usrauth_t 	*getuaent(), *getuanam();
extern usrstat_t 	*getusent();

extern grpauth_t	*getgaent(), *getganam(), *getgagid();
extern void 		setgaent(), endgaent();
extern int		updategaent(), putgaent();

#endif	/* AUTH_H */
