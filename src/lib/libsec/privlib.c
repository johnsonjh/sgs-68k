/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) privlib.c: version 25.1 created on 12/2/91 at 20:14:43	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)privlib.c	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*
 *	privlib.c    privilege library routines for the ARIX C2/B1 Security
 *		     kernel and utilities
 *	
 *	Version 2:  clean up the privilege library calls, and support for
 *		    new system features and provide a set of macros for
 *		    compatablity with version 1 utilities 
 */
#include <sys/param.h>
#include <sys/types.h>
#include <sys/mount.h>

/* if this library is compiled in SECURE mode then really define symbols
 * else included fake function number so the routines compile
 */

#define PRIV_SYMS
#include <sys/priv.h>

#define PRIV_COMBO	0x01
#define PRIV_PRIM	0x02

#define TRUE		1
#define FALSE		0


/*
 *	getpriv  return the privilege set of the current  process 
 *		 enter:  pointer to a priv_t structure 
 *		 return: 0 if sucessfull else -1 and errno
 *
 *	Note: This call can be used with secure and non-secure kernels
 *	      to determine if privileges are enabled or not...if not,
 *	      -1 and ENOSYS will be returned
 */
getpriv( p )
priv_t *p;
{
	/* return the privielges to the kernel */
	return kernpriv( GETPRIV, p, (char *) 0 ); 
}


/*
 *	setpriv  set the privilege set of the current process to that 
 *		 passed in. 
 *		 enter: pointer to priv_t structure intialized to the 
 *		        proper values
 *		 return: 0 if sucessfull else -1 and errno
 */
setpriv( p )
priv_t *p;
{
	/* set the privileges to those asked for */
	return kernpriv( SETPRIV, p, (char *) 0 );
}

/*
 *	del_priv   delete privileges from the current privileges 
 *		   list.
 */
delpriv( priv )
uint *priv;
{
	kernpriv( DELPRIV, priv, (char *) 0 );
}


/*
 *	get_file_priv  return the effective privilege on a file
 *		       if the file owns one. 
 *		       enter: name - pathname of file 
 *		              priv - pointer to unsigned int for priv
 *		       return: 0 if sucessfull else -1 and errno set
 *			       appropriately 
 */

uint 
get_file_priv( name, priv )
char *name;
uint *priv;
{
	return kernpriv( GETFPRIV, priv, name );
}

/*
 *	set_file_priv   set the effective privilege on a file.
 *		        enter:  name - pathname of file 
 *				uint with proper effective privileges
 *		        return: 0 is sucessfull else -1 and errno
 *			        set appropriately
 */
set_file_priv( name, priv )
char *name;
uint *priv;
{
	return kernpriv( SETFPRIV, priv, name );
}

/*
 *	get_sys_pmask  get the system privilege mask 
 *		       enter: priv - pointer to mask (unsigned int) 
 *		        return: 0 is sucessfull else -1 and errno
 *			        set appropriately
 */
get_sys_pmask( priv )
uint *priv;
{
	kernpriv( GETSYSMASK, priv, (char *) 0);
}

/*
 *	set_sys_pmask  set the system privilege mask 
 *		       enter: priv - pointer to mask (unsigned int)  
 *		        return: 0 is sucessfull else -1 and errno
 *			        set appropriately
 */
set_sys_pmask( priv )
uint *priv;
{
	kernpriv( SETSYSMASK, priv, (char *) 0);
}

/*
 *	priv    This routine accepts a privilege map and returns
 *		TRUE is the process owns every privilege in the mask
 *		else it returns FALSE. This check is done against the
 *		combined real and effective
 *		enter: priv - privilege mask
 *		return: TRUE or FALSE 
 *
 * 	Note: This funtion will operate properly in either secure or 
 *	      non-secure modes. If we are in non-secure, this call will
 *	      check effective uid against 0.
 */

priv( priv, uid )
uint priv;
ushort uid;
{
priv_t pmap;

	/* if security is on, getpriv must return good value  
         */	
	if ( getpriv( &pmap ) == -1 )  {
		return ( uid == 0 );
	}
	return ( ((pmap.real | pmap.eff) & priv ) == priv);
}


/*
 *	cvt_sym_to_priv  convert a symbol to a privilege
 *			 privilege returned will be the privilege 
 *			 request by the name. Note that the name
 *			 may only be primitive or default combinational
 *			 This routine will handle multiple privilege
 *			 name converion if there is more than on 
 *			 privilege name. 
 *			 enter: name = privilege name string to match
 *			        priv = location to deposit results
 *				sep = seperator used to parse string
 *			 return: 1 will be retunred if all went well
 *			         and everthing matched up okay.
 *				 0 will be returned if any component
 *				 in name couldn't be matched. User
 *			         must still check priv though...may
 *				 be some good stuff
 *
 *	Notes: This routine can be used to add more privileges to 
 *	        the field priv because it uses an internal OR 
 *		mechanism instead of =  on *priv.
 */
cvt_sym_to_priv( name, priv, sep )
char *name, sep;
uint *priv;
{
char *ptr, *tmp, *strchr();
priv_sym_t *p, *match_sym();
int rval;

	ptr = name;
	rval = 1;
	while ( *ptr ) {
		tmp = ptr;
		if ( ptr = strchr( tmp, sep ) )  
			*ptr++ = 0;
		else
			ptr = tmp + strlen (tmp);

		if ( p = match_sym( tmp, PRIV_COMBO | PRIV_PRIM ) ) 
			*priv |= p->p_priv;  
		else
			rval = 0;
	}
	return rval;
}

/*
 *	cvt_priv_to_sym   convert a privilege to a symbol is possible
 *		          This routine is quite sophisticated. First,
 *			  the privilege is checked against the predefined
 *			  default privileges. If a match is found and the
 *			  control parms is true, we'll return the combinational
 *			  privilege name. If the control parm is false, we
 *			  will return a space " " seperated list of privilege
 *			  names that make up this privilege. If the privilege
 *		          is a single privielege, we return the symbolic 
 *			  name, unless the buf parm is 0 ...in this case,
 *			  we just return true if the privilege has a 
 *			  symbolic name else false
 */
cvt_priv_to_sym( priv, buf, control, sep )
uint priv;
char *buf, sep;
uint control;
{
int i;
priv_sym_t *p, *match_priv();
char tbuf[2];

	/* if the buf argument is 0 just return indicator that
 	 * the actual privelege exists 
 	 */
	if ( buf == NULL )  {
		p = match_priv( priv, PRIV_COMBO | PRIV_PRIM );
		return  ( p ) ? TRUE : FALSE;
	}

	/* if the control is on, then check for combinationals
 	 *  only 
	 */
	if ( control ) {
		if ( p = match_priv( priv, PRIV_COMBO  )) {
			strcat( buf, p->p_name );
			return TRUE;
		}
	}

	*buf = 0;
	tbuf[0] = sep;
	tbuf[1] = 0;
	if (priv == 0)

		strcat (buf, "P_NULL");

	else

	    for ( i = 0; i < 32; i++ ) {

		if ( priv & (1<<i) ){

			if ( p = match_priv( priv & (1 << i), PRIV_PRIM )) {

			    if ( *buf )

				strcat( buf, tbuf );

			    strcat( buf, p->p_name );
			}
		}
	    }

	return  (*buf) ? TRUE : FALSE;
}

/*
 *	match_priv  given a privielge, return a pointer to the
 *		    table entry that describes the privilege
 *		     priv - the privilege to check for 
 * 		     mode -  PRIV_COMBO means check for default combinationals
 *			     PRIV_PRIM means check for primitives
 *		     return: ptr to valid table entry of NULL
 */

priv_sym_t *
match_priv( priv, mode )
uint priv;
int mode;
{
priv_sym_t *p;

	if ( mode & PRIV_COMBO ) {
		/* first search the combinational map */
		for ( p = priv_cmap; *p->p_name; p++ ) {
			if ( priv == p->p_priv )
				return p;
		}
	}

	if ( mode & PRIV_PRIM ) {
		/* next search the primitive map */
		for ( p = priv_pmap; *p->p_name; p++ ) {
		if ( priv == p->p_priv )
			return p;
		}
	}
	
	return NULL;
}

/*
 *	match_sym  return a pointer to the priv_sym entry that matches
 *		   name. Mode specified tables to search
 */
priv_sym_t *
match_sym( name, mode )
char *name;
int mode;
{
priv_sym_t *p;

	if ( mode & PRIV_COMBO ) {
		/* first search the combinational map */
		for ( p = priv_cmap; *p->p_name; p++ ) {
			if ( ! strcmp( name, p->p_name ))
				return p;
		}
	}

	if ( mode & PRIV_PRIM ) {
		/* next search the primitive map */
		for ( p = priv_pmap; *p->p_name; p++ ) {
			if ( ! strcmp( name, p->p_name ))
				return p;
		}
	}
	
	return NULL;
}

/*
 *	isprived   accept a symbolic privilege name and returns TRUE
 *		   if the user has that privilege. If not in priv mode
 *		   then honor the uid = 0 stuff
 */
isprived( name, uid )
char *name;
{
priv_sym_t *p, *match_sym();
priv_t priv;

	if ( getpriv( &priv ) != -1 ) {
		if (p = match_sym( name, PRIV_COMBO | PRIV_PRIM  )) 
			return ((( priv.real | priv.eff ) & p->p_priv ) ==
					p->p_priv ); 
		else
			return 0;
	}
	else
		return uid == 0;
}

/*
 *	priv_map    convert a symbolic privilege name to a priv vector
 *	
 *	Notes: Since the priv_t fields are no longer the same, the
 *	       priv->real field gets the matching privileges, the
 *	       priv->eff and priv->mask fields get set to 0
 */

priv_map( name, priv )
char *name;
priv_t *priv;
{
priv_sym_t *p, *match_sym();
uint tmp;

	if (! cvt_sym_to_priv( name, &tmp, ',' ) )
		return 0;
	else {
		priv->real = tmp;
		priv->eff = 0;
		priv->mask = 0;
		return 1;
	}
}

/*
 *	priv_name   convert a priv map to a name
 *
 *	Note: Since the original fields had different meaning than the
 *	      new field, us the union (OR) or the real and eff set
 *	      for the conversion. As usual, if we see any error, return
 *	      error code.
 */

priv_name( priv, name  )
char *name;
priv_t *priv;
{
	if (! cvt_priv_to_sym( priv->real | priv->eff , name, 1, ',' ))
		return 0;
	return 1;
}



	
 
