/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mlslib.c: version 25.1 created on 12/2/91 at 20:14:36	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mlslib.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*
 *	mlslib      MAC and gerneral label handling routins
 *	Version 2:  clean up the label library calls, and support for
 *		    new system features and provide a set of macros for
 *		    compatablity with version 1 utilities 
 */
#include <sys/param.h>
#include <sys/types.h>
#include <sys/mount.h>

#define MLS_SYMS
#include <sys/security.h>
#include <sys/mls.h>
#include <sys/priv.h>


/*
 *	get_slabel   get the security labels (min/max) of the current
 *		     process. Return -1 on error condition, else 0
 */
get_slabel( minl, maxl )
slabel_t *minl, *maxl;
{
slabel_t tmp[2];

	if (kernmac( GETMAC, tmp, (char *) 0 ))
		return -1;
	memcpy( minl, &tmp[0], sizeof( slabel_t ));
	memcpy( maxl, &tmp[1], sizeof( slabel_t ));
	return 0;
}

/*
 *	set_slabel   set the security labels on a process. This call
 *		     is replaced in part by the new role model, and 
 *		     really is included for backward compatability for
 *		     old security utilities. Return -1 on error else 0 
 */
set_slabel( minl, maxl )
slabel_t *minl, *maxl;
{
slabel_t tmp[2];

	memcpy( &tmp[0], minl, sizeof( slabel_t ));
	memcpy( &tmp[1], maxl, sizeof( slabel_t ));
	return kernmac( SETMAC, tmp, (char *) 0 );
}

/*
 *	get_file_slabel   get the security label on a file and return
 *			  it to caller. Return -1 on error else 0
 */
get_file_slabel( fname, lp )
char *fname;
slabel_t *lp;
{
	return kernmac( GETFMAC, lp, fname );
}

/*
 *	set_file_slabel   set the security label on a file. Return -1
 *			  on error else 0
 */
set_file_slabel( fname, lp )
char *fname;
slabel_t *lp;
{
	return kernmac( SETFMAC, lp, fname );
}

/*
 *	getmac    return the label of a process. This is a compatibilty
 *		  call for old stuff
 */
getmac( lp )
slabel_t *lp;
{
slabel_t dodo;

	return get_slabel( lp, &dodo );
}
	
/*
 *	setmac   set the label of a process. This call will make sure the
 *		 min and max labels are set to the same value
 */
setmac( lp )
slabel_t *lp;
{
	return set_slabel( lp, lp );
}


/*
 *	getfmac  return the label of the file named. This is an old style
 *		 call provided for compatability  
 */
getfmac( lp, fname )
char *fname;
slabel_t *lp;
{
	return get_file_slabel( fname, lp );
}


/*
 *	setfmac  set the label on the file. This is an old style
 *		 call provided for compatability 
 */
setfmac( lp, fname )
char *fname;
slabel_t *lp;
{
	return set_file_slabel( fname, lp );
}


/*
 *
 *	mac_map  convert a symbolic name into a true mac. This routine
 *		 is provided for backward compatability and returns
 *		 error codes a little funky
 *
 */
mac_map( str, label )
char *str;
slabel_t *label;
{
	if ( cvt_sym_slabel( str, label ) )
		return 0;
	return -1;
}


/*
 *	cvt_sym_slabel   convert a symbolic name for a symbol to a
 *		         label. This call returns 0 is sucessfull
 *			 else -1.
 */
cvt_sym_slabel( str, label )
char *str;
slabel_t *label;
{
int i, tmp;
extern slabelmap_t slabel_map[];

	for (i=0; *slabel_map[i].name; i++) {
		if ( ! strcmp( slabel_map[i].name, str )) {
			tmp = i;
			label->level = slabel_map[i].label.level;
			for (i=0;i<MAXCATLST;i++)
			     label->catlst[i] = slabel_map[tmp].label.catlst[i];
			return 1;
		}
	}
	return 0;
}

/*
 *	mac_name  convert a label to a symbolic name. This routine is
 *		  included for backward compatability will older releases.
 *
 */
mac_name( label, name )
slabel_t *label;
char *name;
{
	if ( cvt_slabel_sym( name, label ))
		return 0;
	return -1;
}

/*
 *	cvt_slabel_sym   convert a security label to a symbolic name. 
 *			Return 0 if sucessfull, else -1
 *
 */
cvt_slabel_sym( name, label )
char *name;
slabel_t *label;
{
int i, j, val;

	for (i=0; *slabel_map[i].name;i++ ) {
		if ( mls_equ( label, &slabel_map[i].label )) {
			strcpy( name,slabel_map[i].name );
			return 1;
		}
	}
	return 0;
}


/*
 *	mls_equ   return TRUE is the subject label is equal to the 
 *		  object label. Equality is defined as equivelent 
 *		  label levels with the subject category list as a 
 *		  proper superset of the object category list
 */

mls_equ( subj, obj )
register slabel_t *subj, *obj;
{
register int i;

#ifdef B1
	/* JTOF - in a true B1 we must match labesl */
	if ( subj->level != obj->level )
		return 0;
#else
	/* in our hybrid C2, we either match labels or if  
 	 * the object has no label, we declare a match  (kludgy huh??)
         */
	if ( obj->level && (subj->level != obj->level) )
		return 0;
#endif

	/* note:  is a is NULL and b is NULL will also match 
	   This check will be used later when a full B1 implementation
	   is developed
	*/
#ifdef B1
	for ( i = 0; i < MAXCATLST; i++ ) {
		if ( (subj->catlst[i] & obj->catlst[i]) != (obj->catlst[i]) )
			return 0;
	}
#endif
	return 1;
}


/*
 *	mls_dom test the subject label for dominance over the object
 *		label. Dominance means that the subject label's level
 *		is greater than or eqaul, and the subject category list
 *		contains all the categories in the object.
 */

mls_dom( subj, obj )
register slabel_t *subj, *obj;
{
register int i;
/* JTOF - this will work for B1 and C2 cause no subj should execute
 *   	  with NOLABEL....if he does then he will only match objects
 *        with nolabel
 */
	if ( subj->level < obj->level )
		return 0;
#ifdef B1
	for ( i = 0; i < MAXCATLST; i++ ) {
		if ( (subj->catlst[i] & obj->catlst[i]) != (obj->catlst[i]) )
			return 0;
	}
#endif
	return 1;
}

/*
 *	nolabel - this is a definite C2ism....not to cool for B1
 *
 */
nolabel( a )
register slabel_t *a;
{
register int i;

	if ( a->level == 0 ) {
		for ( i=0; i < MAXCATLST; i++ ) {
			if ( a->catlst[i] )
				return 0;
		}
		return 1;
	}
	return 0;
}


