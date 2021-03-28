/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) audlib.c: version 25.1 created on 12/2/91 at 20:13:56	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)audlib.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*
 *	audlib.c     audit library routines for the ARIX C2/B1 Security
 *		     kernel and utilities
 *	
 *	Version 2:  Added new library calls to match the feature supported
 *		    in the new system. This includes calls to the audit
 *		    system call regarding configuration and writting 
 *		    audit records.o
 *
 *	Note: Due to security policy, only privileges users may use these
 *	      calls effectively
 */


#include <sys/param.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/synch.h>
#include <errno.h>

#include <sys/priv.h>
#include <sys/mls.h>
#include <sys/security.h>
#include <sys/audit.h>

/*
 *	audit_start  start the audit trail. This should be used only
 *	             after a mask has been set and the audit deamon 
 *		     started. It will fully enable the audit mechanism
 *		     in the kernel.
 */

audit_start()
{
	return kernaud( AUDIT_START, 0, (char*) 0 );
}

/*
 *	audit_stop  stop the current audit session. This doesn't change
 *	            the current audit mask, and can be used in emergencies
 *		    or to disable the audit trail in total 
 */

audit_stop()
{
	return kernaud( AUDIT_STOP, 0, (char *) 0  );
}

/*
 *	audit_getconf   get up the audit configuration parameters. This
 *			call takes a function code and a value. The 
 *			function value is AUDCONF_HWM, AUDCONF_HZ etc.
 */
audit_getconf( val, info )
int val;
char *info;
{
	return kernaud( AUDIT_GET, val, info );
}

/*
 *	audit_setconf   set up the audit configuration parameters. This
 *			call takes a function code and a value. The 
 *			function value is AUDCONF_HWM, AUDCONF_HZ etc.
 */
audit_setconf( val, info )
int val;
char *info;
{
	return kernaud( AUDIT_SET, val, info );
}

/*
 *	audit_mask  set the current audit mask to the value supplied.
 *		    Note that the value mask must be a ptr to a 64 bit
 *		    map where 1 is an event active and 0 is inactive 
 */
audit_set_mask( mask )
uint  *mask;
{
	return kernaud( AUDIT_MASK, 0, mask );
}

/*
 *	audit_status   return the internal aud_state structure to 
 *		       the user. Unfortunately, much of the information
 *		       in this structure isn't all that usefull, but
 *		       all start/stop times and configuration parms 
 *		       can be found in this guy
 *
 */

audit_status( p )
aud_state_t *p;
{
	return kernaud( AUDIT_STAT, 0, p );
}

/*
 *	audit_event_status   return the event status structure. This
 *		 	     keeps track of the total number of events
 *			     since boottime and since last startup
 *
 */
audit_event_status( event, p )
int event;
evt_state_t *p;
{
	return kernaud( AUDIT_EVTSTAT, event, p );
}

/*
 *	audit_add_event   add a formated event structure to the audit
 *			  trail. These events must be in the range as
 *			  follows  AUD_IPCACL < event < AUD_RFS     
 */
audit_add_event( event, p, size )
int event;
char *p;
int size;
{
char buf[512];

	if (size >= sizeof(buf)) {
		errno = EOVERFLOW;
		return(-1);
	}

	if (( event < AUD_IPCACL ) || ( event > AUD_RFS )) {
		errno = EINVAL;
		return -1;
	}

	/* slug the event into the first byte of the buffer */
	buf[0] = (char)event;

	/* copy the structure to the buffer */
	memcpy( &buf[1], p, size );
	return kernaud( AUDIT_WRITE, size + 1, buf );
}

/*
 *	audit_add_str   add a string to the audit record. These strings
 *		        are all written to the default event AUD_ULI
 */

audit_add_str( str )
char *str;
{
int size = strlen( str );
char buf[512];

	if (size >=sizeof(buf)){
		errno = EOVERFLOW;
		return(-1);
	}

	buf[0] = AUD_ULI;
	strncpy( &buf[1], str, size );
	return kernaud( AUDIT_WRITE, size +1 , buf );
}

/*
 *
 */
audit_get_hwm()
{
int rval;

	if ( audit_getconf( AUDCONF_HWM, &rval ) == -1 )
		return -1;
	return rval;
}

/*
 *
 */
audit_get_hz()
{
int rval;
	
	if ( audit_getconf( AUDCONF_HZ, &rval) == -1 )
		return -1;
	return rval;
}

/*
 *
 */
audit_get_vers()
{
int rval;
	
	if ( audit_getconf( AUDCONF_VERS, &rval) == -1 )
		return -1;
	return rval;
}

audit_get_mask( mask )
uint *mask;
{
	return audit_getconf( AUDCONF_MASK, &mask );
}


/*
 *
 */
audit_set_hwm( hwm )
uint hwm;
{
	return audit_setconf( AUDCONF_HWM, &hwm );
}


/*
 *
 */
audit_set_hz( hz )
uint hz;
{
	return audit_setconf( AUDCONF_HZ, &hz );
}

/*
 *
 */
audit_umask( mask, mode )
int *mask, mode;
{
	if ( mode ) 
		return kernaud( AUDIT_SUMASK, 0, mask );
	else
		return kernaud( AUDIT_GUMASK, 0, mask );
}
