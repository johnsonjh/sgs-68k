#ifndef SYS_INITTAB_H
#define SYS_INITTAB_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) inittab.h: version 24.1 created on 10/28/91 at 18:33:39	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)inittab.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

struct inittab {
	char *init_id;			/* Id field */
	char *init_rstate;		/* Rstate field */
	char *init_action;		/* Action field */
	char *init_process;		/* Process field */
};

/* Define the location of the inittab file */

#define INITTABFILE "/etc/security/inittab"

struct inittab *getinitent();	/* Get the structure of inittab */
struct inittab *getinitid();	/* Given a id return a structure */
int	putinitent();		/* Given a structure, write to inittab */

char *strtoken();				/* Given a string, return a token */
#endif /* SYS_INITTAB_H */
