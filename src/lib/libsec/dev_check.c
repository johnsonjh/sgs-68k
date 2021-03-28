/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) dev_check.c: version 25.1 created on 12/2/91 at 20:13:59	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)dev_check.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* 
   name_devcheck (name) - Verifies named user's access to current tty

   uid_devcheck (uid)   - Verifies named uid's access to current tty

   name_devinq (name, device) - Verifies named user's access to named device

   uid_devinq (uid, device) - Verifies named uid's access to named device
*/


#include <sys/types.h>
#include <pwd.h>
#include <auth.h>


#define PASSFILE	"/etc/passwd"
#define NULL 0

struct	passwd	*getpwnam (), *getpwuid ();
struct	usrauth *getuanam ();


int
name_devcheck (name)
char	*name;
{
	struct	passwd  *pwd;
	struct  usrauth *ua;

	if ( access (TTYAUTH, 0) != 0 ){

		printf ("%s\n",TTYAUTH);
		return (1);
		}

	if ( ! access (USRAUTH, 0) ){

		if ( (ua = getuanam (name)) == NULL)

			return (0);

		return ( ttyauth (ttyname (0), ua->ua_uid, getgid ()));
		}

	else if ( ! access (PASSFILE, 0) ){

		/* Fail if name not in password file */
		if ( (pwd = getpwnam (name) ) == NULL){	

			endpwent ();
			return (0);
			}

		endpwent ();
		return ( ttyauth (ttyname (0), pwd->pw_uid, getgid ()));
		}

	else

		return (1);
}


int
uid_devcheck (uid)
uid_t	uid;
{
	struct	passwd  *pwd;
	struct  usrauth *ua;

	if ( access (TTYAUTH, 0) != 0 )

		return (1);

	/* Password file is checked first for name, user authorization
	   file is checked afterwards, if present */
	if ( ! access (PASSFILE, 0) ){

		if ( (pwd = getpwuid (uid)) == NULL){

			endpwent ();
			return (0);
			}
		endpwent ();

		if ( ! access (USRAUTH, 0)  ){

			if ( (ua = getuanam (pwd->pw_name)) == NULL)

				return (0);
			}

		return ( ttyauth (ttyname (0), ua->ua_uid, getgid ()));
		}

	else		/* Allow access if no existing password file */

		return (1);
}


int
name_devinq (name, device)
char	*name, *device;
{
	struct	passwd  *pwd;
	struct  usrauth *ua;

	if ( access (TTYAUTH, 0) != 0)

		return (1);

	if ( ! access (USRAUTH, 0) ){

		if ( (ua = getuanam (name)) == NULL)

			return (0);

		return ( ttyauth (device , ua->ua_uid, getgid ()));
		}

	else if ( ! access (PASSFILE, 0) ){

		/* Fail if name not in password file */
		if ( (pwd = getpwnam (name) ) == NULL){	

			endpwent ();
			return (0);
			}

		endpwent ();
		return ( ttyauth (device, pwd->pw_uid, getgid ()));
		}

	else

		return (1);
}


int
uid_devinq (uid, device)
uid_t	uid;
char	*device;
{
	struct	passwd  *pwd;
	struct  usrauth *ua;

	if ( access (TTYAUTH, 0) != 0)

		return (1);

	/* Password file is checked first for name, user authorization
	   file is checked afterwards, if present */
	if ( ! access (PASSFILE, 0) ){

		if ( (pwd = getpwuid (uid)) == NULL){

			endpwent ();
			return (0);
			}
		endpwent ();

		if ( ! access (USRAUTH, 0) ){

			if ( (ua = getuanam (pwd->pw_name)) == NULL)

				return (0);
			}

		return ( ttyauth (device, ua->ua_uid, getgid () ));
		}

	else		/* Allow access if no existing password file */

		return (1);
}
