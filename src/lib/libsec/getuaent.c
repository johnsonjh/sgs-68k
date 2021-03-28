/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) getuaent.c: version 25.1 created on 12/2/91 at 20:14:19	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)getuaent.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include <sys/types.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/sakioctl.h>
#include <sys/param.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <time.h>
#include "auth.h"
#include "stdio.h"
#include "sys/synch.h"
#include "sys/mls.h"
#include "sys/priv.h"
#include "sys/security.h"
#include "sys/audit.h"

#define BADFD	-1

static int	uafd = BADFD,
		uast = BADFD;

#define PRINTABLE_CHARS	0x20

/*
 *	setuaent  open and reset the user_auth file pointer 
 *                return: fd for any further use (file locking)
 */

setuaent()
{
	if (uafd < 0) 
		if (( uafd = open( USRAUTH, O_RDWR | O_CREAT,0600 )) < 0 )
			return -1;
	lseek( uafd, 0L, 0 );
	return uafd;
}

setustatent()
{
	if (uast < 0) 
		if (( uast = open( USRSTAT, O_RDWR | O_CREAT,0600 )) < 0 )
			return -1;
	lseek( uast, 0L, 0 );
	return uast;
}
/*
 *	enduaent close the user_auth database  
 */

enduaent()
{
int rval;

	if (uafd >= 0) {
		rval = close(uafd);
		uafd = BADFD;
	}
	return rval;
}
	
endustatent()
{
int rval;

	if (uast >= 0) {
		rval = close(uast);
		uast = BADFD;
	}
	return rval;
}

/*
 *	getuaent   return the next ua_auth record 
 *
 */	

usrauth_t *
getuaent()
{
	static usrauth_t ua;
	struct flock lck;

	if (uafd < 0)
		if ( setuaent() < 0 )
			return NULL;

	/* initialize the file lock structure */
	lck.l_type = F_RDLCK;
	lck.l_whence = 0;
	lck.l_start  = lseek( uafd, 0, 1 );
	lck.l_len = sizeof(usrauth_t);

	/* if lock failed return a null */
	if ( fcntl( uafd, F_SETLKW, &lck ) < 0 ) {
		lck.l_type = F_UNLCK;
		fcntl( uafd, F_SETLK, &lck );
		return NULL;
	}	
	if ( read( uafd, &ua, sizeof( usrauth_t)) != sizeof(usrauth_t)) {
		lck.l_type = F_UNLCK;
		fcntl( uafd, F_SETLKW, &lck );
		return NULL;
	}

	lck.l_type = F_UNLCK;
	fcntl( uafd, F_SETLKW, &lck );
	return &ua;
}

/*
 *	getusent   get user_stat entry for uid
 *
 */	


static usrstat_t *
get_next_usent()
{
	static usrstat_t us;
	struct flock lck;

	if (uast < 0)
		if ( setustatent() < 0 )
			return NULL;

	/* initialize the file lock structure */
	lck.l_type = F_RDLCK;
	lck.l_whence = 0;
	lck.l_start  = lseek( uast, 0, 1 );
	lck.l_len = sizeof(usrstat_t);

	/* if lock failed return a null */
	if ( fcntl( uast, F_SETLKW, &lck ) < 0 ) {
		lck.l_type = F_UNLCK;
		fcntl( uast, F_SETLK, &lck );
		return NULL;
	}	
	if ( read( uast, &us, sizeof( usrstat_t)) != sizeof(usrstat_t)) {
		lck.l_type = F_UNLCK;
		fcntl( uast, F_SETLKW, &lck );
		return NULL;
	}

	lck.l_type = F_UNLCK;
	fcntl( uast, F_SETLKW, &lck );
	return &us;
}

usrstat_t *
getusent(uid)
uid_t uid;
{
	static	usrstat_t *us;

	setustatent();

	while (us = get_next_usent() ) {
		if ( us->us_uid == uid )
			return(us);
	}
	endustatent();
	return NULL;
}

/*
 *	getuanam  search the database for the named item 
 *
 */

usrauth_t *
getuanam(name)
char *name;
{
	usrauth_t *ua, *getuaent();
	uint	found = 0;

	setuaent();
	/* read each entry until we find the proper one */
	while ( ua = getuaent() )
		if ( !strncmp( ua->ua_name, name, NAME_MAX )) {
			found++;
			break;
		}
	enduaent();
	if ( found )
		return ua;

	return NULL;
}

/*
 *	putuaent  write the user_auth record in the current position 
 *                in the user_auth database.
 *
 */

putuaent( fd, ua )
int fd;
usrauth_t *ua;
{
struct flock lck;
int rval;

	lck.l_whence = 0;
	lck.l_start = lseek( fd, 0, 1 );
	lck.l_type = F_WRLCK;
	lck.l_len = sizeof( usrauth_t );

	if ( fcntl( fd, F_SETLKW, &lck ) < 0 ) {
		lck.l_type = F_UNLCK;
		fcntl( fd, F_SETLK, &lck );
		return -1;
	}	
	rval = write( fd, ua, sizeof(usrauth_t));
	lck.l_type = F_UNLCK;
	fcntl( fd, F_SETLK, &lck );
	return rval;
}

putusent( fd, us , index)
int fd,index;
usrstat_t *us;
{
struct flock lck;
int rval;

	lck.l_whence = 0;
	lck.l_start = lseek( fd, index*sizeof(usrstat_t), 0 );
	lck.l_type = F_WRLCK;
	lck.l_len = sizeof( usrstat_t );

	if ( fcntl( fd, F_SETLKW, &lck ) < 0 ) {
		lck.l_type = F_UNLCK;
		fcntl( fd, F_SETLK, &lck );
		return -1;
	}	
	rval = write( fd, us, sizeof(usrstat_t));
	lck.l_type = F_UNLCK;
	fcntl( fd, F_SETLK, &lck );
	return rval;
}


/*
 *	updaueuaent   changes the record for a given user
 *
 */

updateuaent( ua, us, mode)  
usrauth_t *ua;
usrstat_t *us;
int mode;
{
	int ua_index = 0;
	int us_index = 0;
	usrauth_t new_ua;
	usrstat_t new_us;
	struct flock lck;
	int rval;

	if (mode & JUST_UA) {
	
		new_ua = *ua;

		setuaent();
		while (ua = getuaent()) {
			if (!strncmp(ua->ua_name, new_ua.ua_name, NAME_MAX))
				break;
			ua_index++;
		}

		lck.l_whence = 0;
		lck.l_start = lseek(uafd, ua_index * sizeof(usrauth_t), 0);
		lck.l_type = F_WRLCK;
		lck.l_len = sizeof(usrauth_t);

		if ( fcntl( uafd, F_SETLKW, &lck ) < 0 ) {
			lck.l_type = F_UNLCK;
			fcntl( uafd, F_SETLK, &lck );
			return -1;
		}
 
		rval = write( uafd, &new_ua, sizeof( usrauth_t ));
		lck.l_type = F_UNLCK;
		fcntl( uafd, F_SETLK, &lck );
		enduaent();
		if ( rval < 0 )
			return rval;
		else
			audit_uachange( ua, &new_ua );
	}


	if( mode & JUST_STAT ) {
	
		new_us = *us;

		setustatent();
		while (us = get_next_usent()) {
			if (us->us_uid == new_us.us_uid)
				break;
			us_index++;
		}

		lck.l_whence = 0;
		lck.l_start = lseek( uast, us_index * sizeof (usrstat_t), 0);
		lck.l_type = F_WRLCK;
		lck.l_len = sizeof( usrstat_t );

		if ( fcntl( uast, F_SETLKW, &lck ) < 0 ) {
			lck.l_type = F_UNLCK;
			fcntl( uast, F_SETLK, &lck );
			return(-1);
		}
 
		rval = write( uast, &new_us, sizeof( usrstat_t ));
		lck.l_type = F_UNLCK;
		fcntl( uast, F_SETLK, &lck );
		endustatent();
	}

	return(rval);
}


istimeauth( ua )
usrauth_t *ua;
{
struct tm *tim, *localtime();
time_t clock;
int wday;

	clock = time( 0 );	
	tim = localtime( &clock );
	wday = tim->tm_wday;
	
	if (( ua->ua_times[wday].st_hr == 0 ) &&
	    ( ua->ua_times[wday].et_hr == 0 ))
		return 0;

	if ( ua->ua_times[wday].st_hr == -1 )
		return 1;
	

	if (tim->tm_hour < ua->ua_times[wday].st_hr )
		return 0;
	if ((tim->tm_hour == ua->ua_times[wday].st_hr ) && 
		(tim->tm_min != -1) && (tim->tm_min < ua->ua_times[wday].st_min))
		return 0;
	if (tim->tm_hour > ua->ua_times[wday].et_hr )
		return 0;
	if ((tim->tm_hour == ua->ua_times[wday].et_hr) &&
		(tim->tm_min != -1) && (tim->tm_min > ua->ua_times[wday].et_min))
		return 0;
	return 1;
}

/* kc setup aging of user authorization */
/* this is broken.  login has example of good code.  login uses
that code in one instance and used this function in another.  changed
login to not call this function for now and just do the code.
*/

isdayauth( ua )
usrauth_t *ua;
{

	if( ua->ua_min_pwdlife )		/* does the user have limits */
		if((ua->ua_min_pwdlife >DAY_NOW || (ua->ua_max_pwdlife < DAY_NOW)))
			return(0);	/* authorization invalid today */
	return(1);
}


/*
 *	ua_btoa   convert the user_auth binary record to a ascii record
 *
 *	Format: 
 *	1      2   3   4    5     6      7    8    9    10    11   12  
 * 	uname:uid:gid:pwd:gecos:ddir:dshell:role:mode:pwmin:pwmax:maxtries:
 *	  13   14   15
 *	stat:mask:times
 */
ua_btoa( ua, buf )
usrauth_t *ua;
char *buf;
{
	sprintf(buf, "%s:%d:%d:%s:%s:%s:%s:%d:%d:%d:%d:%d:%d:%d,%d:", 
		ua->ua_name, ua->ua_uid, ua->ua_gid,
		ua->ua_passwd, ua->ua_gecos, ua->ua_defdir, ua->ua_defshell, 
		ua->ua_role, ua->ua_mode, ua->ua_min_pwdlife, 
		ua->ua_max_pwdlife, ua->ua_maxtries,
		ua->ua_acctstat,ua->ua_amask[0], ua->ua_amask[1] );

	times_btoa( ua, &buf[strlen(buf)] );

}

/*
 *	times_atob   convert ascii record to twin_t structure
 *
 */

times_atob( ua, buf )
usrauth_t  *ua;
char *buf;
{
char *tl, *tmp, *pl;
int i = 0;

	tl = buf;
	while ( tl ) {
		tmp = tl;
		if( tl = strchr( tl, ',') )
			*tl++ = 0;
		if ( (*tmp == NULL) || (*tmp < PRINTABLE_CHARS) ) {
			ua->ua_times[i].st_hr = 0;
			ua->ua_times[i++].et_hr = 0;
			continue;
		}
		if ( *tmp == '*' ) {
			ua->ua_times[i].st_hr = (char)-1;
			ua->ua_times[i++].et_hr = 0;
			continue;
		}

		/* split times into two halves */
		if (!(buf=strchr( tmp, '-' )))
			return NULL;
		*buf++ = 0;

		if ( (atoi(tmp) > 24) || (atoi(tmp) < 0 ) )
			return (-2);			/* Error start time (hour) */

		if(!(pl=strchr( tmp, ':' )))  {
			ua->ua_times[i].st_hr = atoi( tmp );
			ua->ua_times[i].st_min = 0;
		}
		else {
			*pl++  = 0;

			if ( (atoi(pl) > 59) || (atoi(pl) < 0 ) )
				return (-2);			/* Error start time (min) */

			ua->ua_times[i].st_hr = atoi( tmp );
			ua->ua_times[i].st_min = atoi( pl );
		}

		if ( (atoi(buf) > 24) || (atoi(buf) < 0 ) )
			return (-3);			/* Error end time (hour) */

		if (!(pl=strchr( buf, ':' ))) {
			ua->ua_times[i].et_hr = atoi( buf );
			ua->ua_times[i].et_min = 0;
		}
		else {
			*pl++ = 0;

			if ( (atoi(pl) > 59) || (atoi(pl) < 0 ) )
				return (-3);			/* Error end time (min) */

			ua->ua_times[i].et_hr = atoi( buf );
			ua->ua_times[i].et_min = atoi( pl );
		}
		i++;
			
	}
	/* make sure seven days accounted for */
	if ( i < 7)
		return -1;
	return 0; 
}
	

/*
 *	ua_atob  translate userauth acsii record to binary
 *
 *	Format: 
 *	1      2   3   4    5     6      7    8    9    10    11   12  
 * 	uname:uid:gid:pwd:gecos:ddir:dshell:role:mode:pwmin:pwmax:maxtries:
 *	  13   14   15
 *	stat:times:mask
 */

ua_atob( ua, buf )
usrauth_t *ua;
char *buf;
{
char *p[16], *tmp;
int i;

	p[0] = buf;
	for ( i = 0; i < 14; i++ ) {
		if (!( p[i+1] = strchr( p[i], ':' )))
			break;
		*p[i+1]++ = 0;
	}
	strncpy( ua->ua_name, p[0], NAME_MAX );
	ua->ua_uid = (ushort) atoi( p[1] );
	ua->ua_gid = (ushort) atoi( p[2] );
	strncpy( ua->ua_passwd,p[3],NAME_MAX );
	strncpy( ua->ua_gecos,p[4],MAXCOMMENT );
	strncpy( ua->ua_defdir,p[5],MAXPATH );
	strncpy( ua->ua_defshell,p[6],MAXPATH );
	ua->ua_role = atoi( p[7] );
	ua->ua_mode = (char) atoi( p[8] );
	ua->ua_min_pwdlife = (time_t) atoi( p[9] );
	ua->ua_max_pwdlife = (time_t) atoi( p[10] );
	ua->ua_maxtries = atoi( p[11] );
	ua->ua_acctstat = (time_t) atoi( p[12] );
	mask_atob(ua->ua_amask,p[13]);
	times_atob(ua, p[14] );
}


times_btoa(ua,buf)
usrauth_t *ua;
char *buf;
{
	register i;
	char tmp[256];
	char *t;

	t = buf;

	for( i=0,t = buf; i<7; i++,t = &buf[ strlen(buf) ] ) {
		if( ua->ua_times[i].st_hr == (char)-1) {
			if( i < 6 )
				strcat ( t , "*," );
			else
				strcat ( t , "*" );
			continue;
		}
		if( ( ua->ua_times[i].st_hr == 0) && ( ua->ua_times[i].et_hr == 0) ) {
			if( i< 6 )
				strcat (t , ",");
			continue;
		}
		itoa( ua->ua_times[i].st_hr,tmp );
		strcat( t, tmp);
		strcat( t, ":" );
		itoa( ua->ua_times[i].st_min,tmp );
		if( strncmp(tmp,"0",1) == 0 )
			strcat( t, "0");
		strcat( t, tmp);
		strcat ( t, "-");
		itoa( ua->ua_times[i].et_hr,tmp );
		strcat( t, tmp);
		strcat( t, ":" );
		itoa( ua->ua_times[i].et_min,tmp );
		if( strncmp(tmp,"0",1) == 0 )
			strcat( t, "0");
		strcat( t, tmp);
		if( i < 6 )
			strcat( t, "," );

	}
	


}

mask_atob(mask,buf)
int *mask;
char *buf;
{
	char *tmp,*t1;

	t1 = buf;
	tmp = t1;
	if (t1 = strchr( t1, ',')) 
		*t1++ = 0;
	*mask = atoi(tmp);
	*(mask+1) = atoi(t1);

}

/*
 *	audit_uachange   audit any changes in the user_auth database
 */
audit_uachange( old_ua, new_ua )
usrauth_t *old_ua, *new_ua;
{
sat_usrinfo_t buf;

	if ( old_ua ) {
		if ( old_ua->ua_uid != new_ua->ua_uid )
			buf.mode |= NEW_UID;
		if ( old_ua->ua_gid != new_ua->ua_gid )
			buf.mode |= NEW_GID;
		if ( strcmp( old_ua->ua_passwd, new_ua->ua_passwd ) )
			buf.mode |= NEW_PASSWD;
		if ( strcmp( old_ua->ua_defdir, new_ua->ua_defdir ) )
			buf.mode |= NEW_DIR;
		if ( strcmp( old_ua->ua_defshell, new_ua->ua_defshell ) )
			buf.mode |= NEW_SHELL;
		if ( old_ua->ua_mode != new_ua->ua_mode ) 
			buf.mode |= NEW_PWDPARM; 
		if ( old_ua->ua_amask != new_ua->ua_amask )
			buf.mode |= NEW_AMASK;
		if ( old_ua->ua_role != new_ua->ua_role )
			buf.mode |= NEW_ROLE;
		if ( old_ua->ua_acctstat != new_ua->ua_acctstat ) {
			switch( new_ua->ua_acctstat ) {
				case 0 :
					buf.mode |= ACCTUNLOCKED;
					break;
				case 1:
					buf.mode |= ACCTLOCKED;
					break;
				default:
					buf.mode |= ACCTRETIRED;
			}
		}
	}
	else
		buf.mode |= NEW_UNAME;

	strncpy( buf.uname,  new_ua->ua_name, 14 );
	if ( audit_add_event( AUD_USRINFO, &buf, sizeof( sat_usrinfo_t)) < 0)
			if ( errno != ENOSYS )
			fprintf( stderr, "can't write audit record\n" );
}
