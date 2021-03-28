#ifndef SYS_GCP_GCLINE_H
#define SYS_GCP_GCLINE_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gcline.h: version 2.1 created on 5/22/89 at 19:06:22	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gcline.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/gcp:gcline.h	2.1"

/*
 *	G C P  Line discipline switch.
 */

struct gc_linesw {
	int	(*gcl_open)();		/* open */
	int	(*gcl_close)();		/* close */
	int	(*gcl_ioctl)();		/* ioctl */
	int	(*gcl_input)();		/* process input chars */
	int	(*gcl_output)();	/* process output chars */
	int	(*gcl_rxint)(); 	/* receiver interrupts */
	int	(*gcl_txint)(); 	/* transmitter interrupts */
	int	(*gcl_exint)(); 	/* external interrupts */
	int	(*gcl_spint)(); 	/* special interrupts */
	int	* gcl_user;		/* user pointer */
};

extern struct gc_linesw gc_linesw[];

#endif /* SYS_GCP_GCLINE_H */
