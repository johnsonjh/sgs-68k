/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) net.c: version 25.1 created on 12/2/91 at 20:14:39	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)net.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include <sys/param.h>
#include <sys/types.h>
#include <stdio.h>
#include <limits.h>
#include <sys/priv.h>
#include <sys/mls.h>
#include <sys/sakioctl.h>
#include <auth.h>
#include <shadow.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <time.h>

char	def_net[]="network";     /*network is the name of an entry in
				 the dev_auth file that is used when
				 there is no other tty.  Such as in
				 the case of remsh
				*/


/* These are functions that must be used by the various network daemons
   in order to properly access a secure system.  When run without security,
   these are just stubs
*/


/* useraccess:  input  -  username
		output  -  true, access granted
			   false, access denied

	This routine first checks the passwd file to insure that the user
	has an entry on the system.  It then checks to see if there are
	secure databases and investigates whether there are any restrictions
	present for the given user.  If all of the tests pass, it returns
	true(access granted).

*/

useraccess(uname)
char *uname;
{
	struct passwd *pwd;
	struct passwd *getpwnam();
	char	*ttyname();	/* name of tty */
	char *ttyn;
	ttyauth_t *ta,*gettanam();
	ttystat_t *ts,*gettsent();
	usrauth_t *ua,*getuanam();
	usrstat_t *us,*getusent();
	int usr_flag,tty_flag,shad_flag;


	if( ( usr_flag = ! access(USRAUTH,0) ) )
		tty_flag = ! access(TTYAUTH,0);
	else if(( pwd = getpwnam(uname)) == NULL) 
		return(0);

	if ( usr_flag ) {
		if( ( ua = getuanam(uname)) != NULL ) {

			/* Is the user already locked out? */

			if ( ua->ua_acctstat != NOLOCK )
				update_and_exit(ua,tty_flag);

			/* Get the entry for the usr_stat file */
			us = getusent(ua->ua_uid);

			/* Is the user authorized for this time and/or day? */
			if (!istimeauth(ua) || !isdayauth(ua) )
				update_and_exit(ua,tty_flag);

			/* Has he exceeded his limit of failed logins?  */
			if( us->us_badtries > ua->ua_maxtries) 
				update_and_exit(ua,tty_flag);

			if (tty_flag) {
			/* OK so the user account was OK, 
			 * now see if the tty is OK */

				if ((ttyn = ttyname(0)) == NULL)
					ttyn = def_net;

				if( (ta = gettanam(ttyn)) == NULL ) 
				/* There wasn't even an entry in the tty 
				 * file so we cannot allow
				 * this guy to get on the system
				 */
					update_and_exit(ua,tty_flag);

				/* Is the terminal already locked ? */
				if ( ta->ta_status != NOLOCK )
					update_and_exit(ua,tty_flag);

				if( !isttyauth(ta,uname) ) 
					update_and_exit(ua,tty_flag);
	
			}
		}
		else
			return(0);

	}

	/* give them the OK */
	return(1);
}


/*    update_and_exits -- updates  the database files with failed login 
 *    information.
*/

update_and_exit(ua,tty)
usrauth_t	*ua;
int		tty;
{
usrstat_t	*us;
ttyauth_t	*ta;
ttystat_t	*ts;
char		*ttyn;

	us = getusent(ua->ua_uid);
	us->us_last_try = DAY_NOW;

	if ((ua->ua_acctstat != NOLOCK ) && 
	    (us->us_badtries++ > ua->ua_maxtries) )
		ua->ua_acctstat |= LOCKED;

	updateuaent(ua,us,DO_BOTH);


	 if (tty) {
		if ((ttyn = ttyname(0)) == NULL)
			ttyn = def_net;

	    	if( (ta = gettanam(ttyn)) == NULL ) 
			exit (0);

		ts = gettsent(ta->ta_tsentry);
		ts->ts_lasttry = DAY_NOW;
		if( ts->ts_tries++ > ta->ta_maxtry) 
			ta->ta_status |= LOCKED;

		updatetaent(ta,ts,DO_BOTH);
	}
	exit (0);

}	


passwdrequired(uname)
char *uname;
{

	usrauth_t *getuanam();
	struct passwd *pwd, *getpwnam();
	usrauth_t *upwd;	/* Shadow password structure */
	struct spwd *shpwd;
	int shad_flag,usr_flag;

	usr_flag = ! access(USRAUTH,0);

	if( usr_flag &&  ((upwd = getuanam(uname)) != NULL) ) {
		if( *(upwd->ua_passwd) )
			return (1);
		else
			return (0);
	}

	else if ( (shad_flag = ! access (SHADOW,0) ) ) {
		if( (shpwd = getspnam(uname)) != NULL ) {
			if(*(shpwd->sp_pwdp))
				return (1);
			else
				return (0);
		}
	}

	else if ((pwd = getpwnam(uname)) != NULL) {
		if( *(pwd->pw_passwd) )
			return (1);
		else
			return (0);
	}


	errno = ENOENT;
	
	return (-1);



}

/* passwdok - The purpose of this routine is to check the user's password
   for those network utilities that do not use login.

	input -  uname - name of the user
		 passwd - the unencrypted password

	output - true  - the password is ok
		 false - invalid password

*/

passwdok(uname,passwd)
char *uname,*passwd;
{
	struct passwd *pwd, *getpwnam();
	usrauth_t *upwd;	/* Shadow password structure */
	struct spwd *shpwd;	/* Shadow password structure */
	int shad_flag,usr_flag;

	usr_flag = ! access(USRAUTH,0);
	shad_flag = ! access(SHADOW,0);

	if( usr_flag &&  ((upwd = getuanam(uname)) != NULL) ) {

		if ( strcmp(crypt(passwd,upwd->ua_passwd),upwd->ua_passwd) ) 
			return (0);

		return (1);
	}
	else if( shad_flag &&  ((shpwd = getspnam(uname)) != NULL) ) {

		if ( strcmp(crypt(passwd,shpwd->sp_pwdp),shpwd->sp_pwdp) ) 
			return (0);

		return (1);
	}
	else if ((pwd = getpwnam(uname)) != NULL) {

		if ( strcmp(crypt(passwd,pwd->pw_passwd),pwd->pw_passwd) ) 
			return (0);

		return (1);
	}

	errno = ENOENT;
	
	return (-1);

}


