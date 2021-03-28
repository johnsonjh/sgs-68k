#ifndef SYS_GCTIO_H
#define SYS_GCTIO_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gctio.h: version 24.1 created on 10/28/91 at 18:33:12	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gctio.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/


struct gctio {
	int		gt_tdcd;	/* dsr to dcd delay (sec)	*/
	int		gt_tact;	/* inactive delay (sec)		*/
	int		gt_tdcd_sig;	/* dsr to dcd signal		*/
	int		gt_tact_sig;	/* inactive signal		*/
	unsigned char 	gt_scc_stat;	/* scc status			*/
	unsigned char 	unused[3];	/* pad to 4-byte multiple for sparc */
};

#define	GCTIO		('G'<<8)
#define	GCTIO_GET	(GCTIO|1)
#define	GCTIO_SET	(GCTIO|2)

#endif /* SYS_GCTIO_H */
