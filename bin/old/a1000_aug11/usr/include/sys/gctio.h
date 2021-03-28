#ifndef SYS_GCTIO_H
#define SYS_GCTIO_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gctio.h: version 2.1 created on 5/22/89 at 19:07:16	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gctio.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)sys:gctio.h	1.4""
/* 9-22-88	pad out gctio to 4-byte multiple for sparc - gil hunt */


struct gctio {
	int		gt_tdcd;	/* dsr to dcd delay (sec)	*/
	int		gt_tact;	/* inactive delay (sec)		*/
	int		gt_tdcd_sig;	/* dsr to dcd signal		*/
	int		gt_tact_sig;	/* inactive signal		*/
	unsigned char 	gt_scc_stat;	/* scc status			*/
	unsigned char	unused[3]; 	/* pad out for sparc */
};

#define	GCTIO		('G'<<8)
#define	GCTIO_GET	(GCTIO|1)
#define	GCTIO_SET	(GCTIO|2)

#endif /* SYS_GCTIO_H */
