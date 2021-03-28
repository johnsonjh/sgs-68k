/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gettaent.c: version 25.1 created on 12/2/91 at 20:14:15	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gettaent.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#include <sys/types.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <sys/priv.h>
#include <sys/mls.h>
#include <sys/stat.h>
#include <sys/sakioctl.h>
#include <sys/param.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <auth.h>
#include <stdio.h>
#include <sys/synch.h>
#include <sys/security.h>
#include <sys/priv.h>
#include <sys/mls.h>
#include <sys/audit.h>

#define	BADFD	-1

static int	tafd = BADFD,
		tsfd = BADFD;

/*
 *	settaent  open and reset the tty_auth file pointer 
 *                return: fd for any further use (file locking)
 */

settaent()
{
	if (tafd == BADFD) 
		if (( tafd = open( TTYAUTH, O_RDWR )) < 0 )
			return -1;
	lseek( tafd, 0L, 0 );
	return tafd;
}

/*
 *	settsent  open and reset the tty_stat file pointer 
 *                return: fd for any further use (file locking)
 */

settsent()
{
	if (tsfd == BADFD) 
		if (( tsfd = open( TTYSTAT, O_RDWR )) < 0 )
			return -1;
	lseek( tsfd, 0L, 0 );
	return tsfd;
}

/*
 *	endtaent close the tty_auth database  
 */

endtaent()
{
int rval;

	if (tafd >= 0) {
		rval = close(tafd);
		tafd = BADFD;
	}
	return rval;
}

/*
 *	endtsent close the tty_stat database  
 */

endtsent()
{
int rval;

	if (tsfd >= 0) {
		rval = close(tsfd);
		tsfd = BADFD;
	}
	return rval;
}
	
/*
 *	gettaent   return the next tty_auth record 
 *
 */	

ttyauth_t *
gettaent()
{
static ttyauth_t ta;
struct flock lck;

	if (tafd == BADFD)
		if ( settaent() < 0 )
			return NULL;

	/* initialize the file lock structure */
	lck.l_type = F_RDLCK;
	lck.l_whence = 0;
	lck.l_start  = lseek( tafd, 0, 1 );
	lck.l_len = sizeof(ttyauth_t);

	/* if lock failed return a null */
	if ( fcntl( tafd, F_SETLKW, &lck ) < 0 ) {
		lck.l_type = F_UNLCK;
		fcntl( tafd, F_SETLK, &lck );
		return NULL;
	}

	if ( read( tafd, &ta, sizeof( ttyauth_t )) != sizeof(ttyauth_t)){
		lck.l_type = F_UNLCK;
		fcntl( tafd, F_SETLK, &lck );
		return NULL;
	}

	lck.l_type = F_UNLCK;
	fcntl( tafd, F_SETLKW, &lck );
	return &ta;
}

	
/*
 *	gettsent   return the next tty_stat record 
 *
 */	

ttystat_t *
gettsent(index)
int index;
{
static ttystat_t ts;
struct flock lck;

	if (tsfd == BADFD)
		if ( settsent() < 0 )
			return NULL;

	/* initialize the file lock structure */
	lck.l_type = F_RDLCK;
	lck.l_whence = 0;
	lck.l_start  = lseek( tsfd, index*sizeof(ttystat_t), 0 );
	lck.l_len = sizeof( ttystat_t );

	/* if lock failed return a null */
	if ( fcntl( tsfd, F_SETLKW, &lck ) < 0 ) {
		lck.l_type = F_UNLCK;
		fcntl( tsfd, F_SETLK, &lck );
		return NULL;
	}

	if ( read( tsfd, &ts, sizeof( ttystat_t )) != sizeof(ttystat_t)) {
		lck.l_type = F_UNLCK;
		fcntl( tsfd, F_SETLK, &lck );
		return NULL;
	}

	lck.l_type = F_UNLCK;
	fcntl( tsfd, F_SETLKW, &lck );
	return &ts;
}


/*
 *	gettanam  search the database for the named item 
 *
 */

ttyauth_t *
gettanam(name)
char *name;
{
	ttyauth_t *ta, *gettaent();
	uint	found = 0;

	settaent();
	/* read each entry until we find the proper one */

	while ( ta = gettaent() ) {
		if ( !strncmp( ta->ta_ttyn, name, NAME_MAX )) {
			found++;
			break;
		}
	}
	endtaent();

	if (found)
		return ta;

	return NULL;
}

/*
 *	puttaent  write the tty_auth record in the current position 
 *                in the tty_auth database.
 *
 */

puttaent( fd, ta )
int fd;
ttyauth_t *ta;
{
struct flock lck;
int rval;

	lck.l_whence = 0;
	lck.l_start = lseek( fd, 0, 1 );
	lck.l_type = F_WRLCK;
	lck.l_len = sizeof( ttyauth_t );

	if ( fcntl( fd, F_SETLKW, &lck ) < 0 ) {
		lck.l_type = F_UNLCK;
		fcntl( fd, F_SETLK, &lck );
		return -1;
	}	
	rval = write( fd, ta, sizeof(ttyauth_t));
	lck.l_type = F_UNLCK;
	fcntl( fd, F_SETLK, &lck );
	return rval;
}


puttsent( fd, ts )
int fd;
ttystat_t *ts;
{
struct flock lck;
int rval;

	lck.l_whence = 0;
	lck.l_start = lseek( fd, 0, 1 );
	lck.l_type = F_WRLCK;
	lck.l_len = sizeof( ttystat_t );

	if ( fcntl( fd, F_SETLKW, &lck ) < 0 ) {
		lck.l_type = F_UNLCK;
		fcntl( fd, F_SETLK, &lck );
		return -1;
	}	
	rval = write( fd, ts, sizeof(ttystat_t));
	lck.l_type = F_UNLCK;
	fcntl( fd, F_SETLK, &lck );
	return rval;
}


/*
 *	updatetaent   changes the record for a given tty
 *
 */

updatetaent( ta , ts, mode)  
ttyauth_t *ta;
ttystat_t *ts;
int mode;
{

	ttyauth_t  *gettaent();
	ttyauth_t new_ta;
	struct flock lck;
	int rval,i;

	if( settaent() < 0 )
		return (-1);

	new_ta = *ta;

	/* read each entry until we find the proper one */
	i = 0;
	while ( ta = gettaent() ) {
		if ( !strncmp( new_ta.ta_ttyn, ta->ta_ttyn, NAME_MAX )) {
			break;
		}
		i++;
	}
	new_ta.ta_tsentry = i;

	if(mode & JUST_TA) {

		lck.l_whence = 0;
		lck.l_start = lseek( tafd, i*sizeof( ttyauth_t ), 0 );
		lck.l_type = F_WRLCK;
		lck.l_len = sizeof( ttyauth_t );

		if ( fcntl( tafd, F_SETLKW, &lck ) < 0 ) {
			lck.l_type = F_UNLCK;
			fcntl( tafd, F_SETLK, &lck );
			return -1;
		}

		rval = write( tafd, &new_ta, sizeof( ttyauth_t ));
		lck.l_type = F_UNLCK;
		fcntl( tafd, F_SETLK, &lck );
		endtaent();
		if ( rval < 0 )
			return;
		else
			audit_tachange( ta, &new_ta );
	}

	/* Now update the tty_stat file */

	if(mode & JUST_STAT) {

		if( settsent() < 0 )
			return (-1);
		lck.l_whence = 0;
		lck.l_start = lseek( tsfd, i*sizeof( ttystat_t ), 0 );
		lck.l_type = F_WRLCK;
		lck.l_len = sizeof( ttystat_t );

		if ( fcntl( tsfd, F_SETLKW, &lck ) < 0 ) {
			lck.l_type = F_UNLCK;
			fcntl( tsfd, F_SETLK, &lck );
			return -1;
		}
 
		rval = write( tsfd, ts, sizeof( ttystat_t ));
		lck.l_type = F_UNLCK;
		fcntl( tsfd, F_SETLK, &lck );
		endtsent();
	
	}

	return rval;
}



isttyauth( ta, uname )
ttyauth_t *ta;
char *uname;
{
struct passwd *pwd, *getpwnam();
struct group *gp, *getgrnam(), *getgrgid();
int i, j;

	if ( ta == NULL )
		return 0;
	/* check the user list first */
	if( (pwd = getpwnam(uname) ) == NULL )
		return 0;

	for ( i = 0; ta->ta_users[i] != (uid_t)-1; i++ ) {
		if ( ta->ta_users[i] ==  pwd->pw_uid )
			return 1;

		if ( ta->ta_users[i] ==  (uid_t)-2 )
			return 1;

	}
	for ( i = 0; ta->ta_groups[i] != (gid_t)-1 ; i++ ) {
		if ( ta->ta_groups[i] == (gid_t)-2 )
			return 1;

		if ( ta->ta_groups[i] == pwd->pw_gid)
			return 1;

		gp = getgrnam( ta->ta_groups[i] );
		if ( gp == NULL )
			return 0;

		/* search all members of the group */
		for ( j = 0; gp->gr_mem[j]; j++ ) 
			if ( ! strcmp( gp->gr_mem[j], uname ))
				return 1;
	}

	return 0;
}
	
/*
 *	ta_btoa   convert the contents of a ta strucuture to a ':' formated
 *	          record suitable for output 
 *
 * 	Fields are as follows:
 *        1       2      3    4     5       6       7        8       9     10   
 *	ttyname:maxtry:delay:sak:inactive:status:minlabel:maxlabel:users:groups
 *
 */
ta_btoa( ta, buf )
ttyauth_t *ta;
char *buf;
{
struct passwd *pwd, *getpwuid();

	/* 
 	 * room for largest possible list of names and the commas between them.
 	 */
char tmp[MAXUG * (NAME_MAX + 1)];  


	sprintf( buf, "%s:%d:%d:", ta->ta_ttyn, ta->ta_maxtry, ta->ta_delay);
	sak_ascii( &ta->ta_sak, buf );	/* no terminating colon */
	/*
	 *	NOTE:	the terminating colon for above field is prepended
	 *		in the following sprintf string.
	 */
	sprintf(&buf[strlen(buf)],":%d:%d:%d:",
		ta->ta_inactive,ta->ta_status,ta->ta_tsentry);
	mac_name(&ta->ta_minlabel,tmp);
	strcat( buf, tmp);
	strcat( buf, ":");
	mac_name(&ta->ta_maxlabel,tmp);
	strcat( buf, tmp);
	strcat( buf, ":");
	*tmp = 0;
	gulist( ta, tmp, 0 );
	strcat( buf, tmp );
	strcat( buf, ":" );
	*tmp = 0;
	gulist( ta, tmp, 1 );
	strcat( buf, tmp );
}

/*
 *	ta_atob  translate ttyauth acsii record to binary
 *
 * 	Fields are as follows:
 *        1       2      3    4     5       6       7        8       9     10   
 *	ttyname:maxtry:delay:sak:inactive:status:minlabel:maxlabel:users:groups
 */
ta_atob( ta, buf )
ttyauth_t *ta;
char *buf;
{
char *p[12];
int i;

	p[0] = buf;
	for ( i = 0; i < 11; i++ ) {
		if (!( p[i+1] = strchr( p[i], ':' )))
			break;
		*p[i+1]++ = 0;
	}
	strncpy( ta->ta_ttyn, p[0], NAME_MAX );
	ta->ta_maxtry = (ushort) atoi( p[1] );
	ta->ta_delay = (ushort) atoi( p[2] );
	ascii_sak( &ta->ta_sak, p[3] );
	ta->ta_inactive = (ushort) atoi( p[4] );
	ta->ta_status = (char) atoi( p[5] );
	ta->ta_tsentry = (char) atoi( p[6] );
	mac_map(p[7],&ta->ta_minlabel);
	mac_map(p[8],&ta->ta_maxlabel);
	talist( ta, p[9], 0 );
	talist( ta, p[10], 1 );
}

/*
 *	sak_ascii  convert a sak ioctl structure to ascii format
 */
sak_ascii(sak,buf)
struct	sakioctl	*sak;
char			*buf;
{
uchar			flags, key1, key2, key3;

	flags = sak->flags;
	if (flags & SAK_CARR) {
		strcat(buf,"CARRIER");
		return (0);
	}
	if (flags & SAK_BREAK) {
		strcat(buf,"BREAK");
		return (0);
	}
	if (flags & SAK_KEYS) {
		key1 = sak->sak_key1;
		key2 = sak->sak_key2;
		key3 = sak->sak_key3;
		if (key1 < ' ')
			sprintf(&buf[strlen(buf)],"^%c",key1|0100);
		else
			if (key1 == '^')
				sprintf(&buf[strlen(buf)],"\\^");
			else
				if (key1 == '\\')
					sprintf(&buf[strlen(buf)],"\\\\");
				else
					if (key1 > '~')
						sprintf(&buf[strlen(buf)],"^?");
					else
						sprintf(&buf[strlen(buf)],
							"%c",key1);
		if (flags & (SAK_2KEYS | SAK_3KEYS)) {
			if (key2 < ' ')
				sprintf(&buf[strlen(buf)],"^%c",key2|0100);
			else
				if (key2 == '^')
					sprintf(&buf[strlen(buf)],"\\^");
				else
					if (key2 == '\\')
						sprintf(&buf[strlen(buf)],
							"\\\\");
					else
						if (key2 > '~')
						    sprintf(&buf[strlen(buf)],
							"^?");
						else
						    sprintf(&buf[strlen(buf)],
								"%c",key2);
		}
		if (flags & SAK_3KEYS) {
			if (key3 < ' ')
				sprintf(&buf[strlen(buf)],"^%c",key3|0100);
			else
				if (key3 == '^')
					sprintf(&buf[strlen(buf)],"\\^");
				else
					if (key3 == '\\')
						sprintf(&buf[strlen(buf)],
							"\\\\");
					else
						if (key3 > '~')
						    sprintf(&buf[strlen(buf)],
							"^?");
						else
						    sprintf(&buf[strlen(buf)],
								"%c",key3);
		}
	}
	return (0);
}

/*
 *	ascii_sak	parse the sak keys and fill in the proper structure.
 *			Does not understand commas or hex specification but
 *			knows about control characters (preceded by '^').
 */
ascii_sak(sak,buf)
struct	sakioctl	*sak;
char			*buf;
{
int			i, val;
char			key, *uc;

	sak->flags = 0;
	sak->sak_key1 = 0;
	sak->sak_key2 = 0;
	sak->sak_key3 = 0;
	if (buf && *buf && (*buf != ':')) {
		uc = strdup(buf);	/* keep original string case */
		if (uc == NULL)
			uc = buf;
		else
			mkupperlen(uc,strlen(uc));
		if (!strncmp(uc,"CARR",4)) {
			sak->flags = SAK_CARR;
			return (0);
		}
		if (!strncmp(uc,"BREA",4)) {
			sak->flags = SAK_BREAK;
			return (0);
		}
		i = 0;
		while (*buf) {
			if ((*buf == '\\') && *(buf+1)) {
				buf++;
				key = *buf;
			} else {
				if ((*buf == '^') && *(buf+1)) {
					key = *(buf+1);
					if ((key >= 'a') && (key <= 'z'))
						key = toupper(*(buf+1));
					if ((key >= 'A') && (key <= '_')) {
						key &= ~0100;
						buf++;
					} else {
						if (key == '?') {    /* ^?  */
							key = 0177; /* DEL */
							buf++;
						} else {
							key = *buf;
						}
					}
				} else {
					key = *buf;
				}
			}
			buf++;
			i++;
			switch (i) {
				case 1:
					sak->flags = SAK_1KEY;
					sak->sak_key1 = key;
					break;
				case 2:
					sak->flags = SAK_2KEYS;
					sak->sak_key2 = key;
					break; 
				case 3:
					sak->flags = SAK_3KEYS;
					sak->sak_key3 = key;
					break; 
			}
		}
	}
	return (0);
}

/*
 *	ttyauth   return true if the users is authorized to use the 
 *		  tty else return false
 *
 */

ttyauth( ttyn, uid, gid )
uid_t uid;
gid_t gid;
char *ttyn;
{
ttyauth_t *ta, *gettanam();

	if ( ! ( ta = gettanam( ttyn ) ) )
		return 0;
	return taauth( ta, uid, gid );
}

/*
 *	taauth  determine if the user is authorized for a given
 *		tty based upon a tty_auth structure and uid/gid info
 *
 */

taauth( ta, uid, gid )
ttyauth_t *ta;
uid_t uid;
gid_t gid;
{
int i;
	for ( i = 0; i < MAXUG; i++ ) {

		if ( (ta->ta_users[i] == (uid_t) -2 ) || ( ta->ta_users[i] == 
		     uid ) )

			return 1;

		if ( ta->ta_users[i] == (uid_t) -1 )

			break;
	}

	/* check to see if any groups match */
	for ( i = 0; i < MAXUG; i++ ) {

		if ( (ta->ta_groups[i] == (gid_t) -2 ) || ( ta->ta_groups[i] == 
		     gid ) )

			return 1;
		if ( ta->ta_groups[i] == (gid_t) -1 )

			return 0;
	}
	return 0;
}

/*
 *	gulist  group/user list building routine converts uid/gids to 
 *		uname, gname lists 
 *
 */

gulist( ta, buf, mode )
ttyauth_t *ta;
char *buf;
int mode;
{
struct group *gp, *getgrgid();
struct passwd *pw, *getpwuid(); 
int i;

	for ( i = 0; i < MAXUG; i++ ) {
		if ( mode ) {
			if ( ta->ta_groups[i] == (gid_t)-1 )
				break;
			if ( ta->ta_groups[i] == (gid_t)-2 ) {
				strcat( buf, "*" );
				break;
			}
			if ( gp = getgrgid( ta->ta_groups[i] )) {
				strcat( buf, gp->gr_name );
				if ( ta->ta_groups[i+1] != (gid_t)-1 )
					strcat( buf, "," );
			}
			else
				break;
		}
		else {
			if ( ta->ta_users[i] == (uid_t)-1 )
				break;
			if ( ta->ta_users[i] == (uid_t)-2 ) {
				strcat( buf, "*" );
				break;
			}
			if ( pw = getpwuid( ta->ta_users[i] )) {
				strcat( buf, pw->pw_name );
				if ( ta->ta_users[i+1] != (uid_t)-1)
					strcat( buf, "," );
			}
			else
				break;
		}
		}
}
			
/*
 *	talist  convert a symbolic uname/group name list into a -1 terminated 
 *		  gid list
 *
 */

talist( ta, buf, mode )
ttyauth_t *ta;
char *buf;
int mode;
{
gid_t retgid(), retuid();
char *p, *tp, *group;
int i = 0;

	p = buf;
	while ( p && *p ) {
		group = tp = p;
		if ( (p = strchr( tp, ',' )) ||	(p = strchr( tp, '\n'))) {
			*p++ = 0;
		}

		/* wildcard will be know as -2's */
		if ( *group == '*' ) {
			if ( mode ) 
				ta->ta_groups[0] = -2;
			else
				ta->ta_users[0] = -2;
			return 0;
		}

		/* convert the groups name */
		if ( mode ) {
			if ( (ta->ta_groups[i] = retgid( group )) == -1 ) 
				return -1;
		}
		else
			if ( (ta->ta_users[i] = retuid( group )) == -1 ) 
				return -1;
		i++;
	}
	if ( mode )
		ta->ta_groups[i] = -1;
	else
		ta->ta_users[i] = -1;
	return 0;
}

/* 
 *	retgid   given a group name return the id     
 *
 */

gid_t retgid( gname )
char *gname;
{
struct group *gp, *getgrent();

	while ( gp = getgrent() ) {
		if ( ! strcmp( gp->gr_name, gname ) ) {
			endgrent();
			return gp->gr_gid;
		}
	}
	endgrent();
	return -1;
}


/*
 *	retuid   given a uname return the uid
 *
 */

uid_t retuid( uname )
char *uname;
{
struct passwd *pw, *getpwent();


	while ( pw = getpwent() ) {
		if ( ! strcmp( pw->pw_name, uname ) ) {
			endpwent();
			return pw->pw_uid;
		}
	}
	endpwent();
	return -1;
}

/*
 *	Convert a decimal number to an ascii character string
 *
 */
itoa(n,s)
int n;
char s[];
{

	int i,sign;

	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while (( n /= 10) > 0);
	if(sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse (s);
}

reverse (s)
char s[];
{
	int c, i, j;

	for ( i=0, j=strlen(s) - 1; i <j; i++, j-- )  {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

getaltaent(ta,index)
ttyauth_t *ta;
int index;
{
struct flock lck;

	if (tafd == BADFD)
		if ( settaent() < 0 )
			return (0);

	/* initialize the file lock structure */
	lck.l_type = F_RDLCK;
	lck.l_whence = 0;
	lck.l_start  = lseek( tafd, index*sizeof(ttyauth_t), 0 );
	lck.l_len = sizeof(ttyauth_t);

	/* if lock failed return a null */
	if ( fcntl( tafd, F_SETLKW, &lck ) < 0 ) {
		lck.l_type = F_UNLCK;
		fcntl( tafd, F_SETLK, &lck );
		return (0);
	}	

	if ( read( tafd, ta, sizeof( ttyauth_t )) != sizeof(ttyauth_t)) {
		lck.l_type = F_UNLCK;
		fcntl( tafd, F_SETLK, &lck );
		return (0);
	}

	lck.l_type = F_UNLCK;
	fcntl( tafd, F_SETLKW, &lck );
	return (1);
}

/*
 *
 */
audit_tachange( old_ta, new_ta )
ttyauth_t *old_ta, *new_ta;
{
sat_devinfo_t buf;

	if ( old_ta ) {
		if ( strcmp( old_ta->ta_ttyn, new_ta->ta_ttyn ) )
			buf.mode |= NEW_TTY_DEV;
		if ( old_ta->ta_status != new_ta->ta_status ) {
			if ( new_ta->ta_status == 0 )
				buf.mode |= TTY_LOCKED;
			else
				buf.mode |= TTY_UNLOCKED;
		}
	}
	
	strncpy( (char *)&buf.ttyn[0], (char *)&new_ta->ta_ttyn[0], 14 );
	
	if ( audit_add_event( AUD_DEVINFO, &buf, sizeof( sat_devinfo_t )) < 0 )
		if ( errno != ENOSYS )
			fprintf( stderr, "can't write audit trail\n" );

}

static
mkupperlen(p,len)
register char	*p;
{
	if (p) {
		while (*p && len--) {
			if ((*p >= 'a') && (*p <= 'z'))
				*p = toupper(*p);
			p++;
		}
	}
}
