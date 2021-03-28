#ifndef SYS_GCP_PTO_H
#define SYS_GCP_PTO_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) pto.h: version 2.1 created on 5/22/89 at 19:06:44	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)pto.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)head/gcp:pto.h	1.2"

struct pto {
	struct pto *	pto_forward;
	struct pto *	pto_backward;
	int (* 		pto_callout)();
	int		pto_ticks;
	int		pto_arg;
	int		pto_time;
};

#endif /* SYS_GCP_PTO_H */
