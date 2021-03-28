#ifndef	SYS_LOGIN_EXT_H
#define	SYS_LOGIN_EXT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) login_ext.h: version 24.1 created on 10/28/91 at 18:35:12	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)login_ext.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#define TTY struct tty_list 
#define TIME struct time_list 

struct tty_list {
		char tty[10];			/* Hold tty value */
		struct tty_list *next;	/* Point to the next tty value */
};

struct time_list {
		int	day;				/* Hold day value */
		int time[2];			/* Hold time values */
		struct time_list *next;	/* Point to the next time value */
};

struct login_ext {
       int 		pw_uid;			/* Uid from the password file */
       int		p_vect;			/* Privilege vector */
       TIME		*times;			/* Date, start and stop time */
       TTY		*ttys;			/* Link list of tty(s) */
       int		passwd_gen;		/* Type of password generation */	
       int		disable_acct;	/* Disable login account */
};

struct login_ext *getlogext();	/* Returns full structure */
struct login_ext *getlogid();	/* Given uid, returns full structure*/
struct login_ext *putlogent();	/* Writes full structure */

struct time_list *td_split();	/* Split time, date and test values */

struct tty_list *tty_split();	/* Split tty and test values */

char *strtoken();				/* Given a string, return string */

/* Defined for the p-vect field */

#define NO_PRIV	0	/* Default - No privilege vector */
#define SSO	1	/* System Security Officer */
#define SA	2	/* System Administrator */
#define SO	3	/* System Operator */

/* Defined for the disable-acct field */

#define ACTIVE	0	/* Default - Account active */
#define DISABLE	1	/* Account has been disabled */

/* Defined to name the login extension file */

#define TOS_FILE	"/etc/security/loginfo"

#endif	/* SYS_LOGIN_EXT_H */
