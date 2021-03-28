#ifndef SYS_TUNEABLE_H
#define SYS_TUNEABLE_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tuneable.h: version 24.1 created on 10/28/91 at 18:40:04	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tuneable.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"kern-port:sys/tuneable.h	10.1"	*/

typedef struct tune {
	int	t_gpgslo;	/* If freemem < t_getpgslow, then start	*/
				/* to steal pages from processes.	*/
	int	t_gpgshi;	/* Once we start to steal pages, don't	*/
				/* stop until freemem > t_getpgshi.	*/
	int	t_gpgsmsk;	/* Mask used by getpages to determine	*/
				/* whether a page is stealable.  The	*/
				/* page is stealable if pte & t_gpgsmsk	*/
				/* is == 0.  Possible values for this	*/
				/* mask are:				*/
				/* 0		- steal any valid page.	*/
				/* PG_REF	- steal page if not	*/
				/*		  referenced in 	*/
				/*		  t_vhandr seconds.	*/
				/*		  mask = 0x00000100	*/
				/* PG_REF|PG_NDREF - steal page if not	*/
				/*		  referenced in 2 *	*/
				/*		  t_vhandr seconds.	*/
				/*		  mask = 0x00001100	*/
	int	t_vhandr;	/* Run vhand once every t_vhandr seconds*/
				/* if freemem < t_vhandl.		*/
	int	t_vhandl;	/* Run vhand once every t_vhandr seconds*/
				/* if freemem < t_vhandl.		*/
	int	t_maxsc;	/* The maximum number of pages which	*/
				/* will be swapped out in a single	*/
				/* operation.  Cannot be larger than	*/
				/* MAXSPGLST in getpages.h.		*/
	int	t_maxfc;	/* The maximum number of pages which	*/
				/* will be saved up and freed at once.	*/
				/* Cannot be larger than MAXFPGLST in	*/
				/* getpages.h.				*/
	int	t_maxumem;	/* The maximum size of a user's virtual	*/
				/* address space in pages.		*/
	int	t_bdflushr;	/* The rate at which bdflush is run in	*/
				/* seconds.  May be changed by powerfail*/
	int	t_bdflushr_df;	/* The default value of t_bdflushr.	*/
	int	t_minarmem;	/* The minimum available resident (not	*/
				/* swapable) memory to maintain in 	*/
				/* order to avoid deadlock.  In pages.	*/
	int	t_minasmem;	/* The minimum available swapable	*/
				/* memory to maintain in order to avoid	*/
				/* deadlock.  In pages.			*/

	int	t_bdirtypct;	/* percentage of 1k disk buffer cache	*/
				/* being dirty which will trigger	*/
	int	t_basynchi;	/* flushing */
	int	t_basynclo;
	int	t_s54kbdirtypct;/* percentage of 4k disk buffer cache	*/
				/* being dirty which will trigger	*/
	int	t_s54kbasynchi;	/* flushing */
	int	t_s54kbasynclo;
} tune_t;

extern tune_t	tune;


#endif /* SYS_TUNEABLE_H */
