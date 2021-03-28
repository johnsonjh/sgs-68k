#ifndef SYS_IOPMSTAT_H
#define SYS_IOPMSTAT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iopmstat.h: version 24.1 created on 10/28/91 at 18:34:25	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iopmstat.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#ifndef SYS_STREAM_H
#include "sys/stream.h"
#endif

/* cached kernel message's statistics */
struct kbstat {
	struct kab {
		uint  called;		     /* calls to kallocb */
		uint  cachit[ALLOCB_TRIES];  /* calls to kallocb that hit */
		uint  fill[ALLOCB_TRIES];    /* calls to kallocb that filled */
	} kab[NCLASS];

	struct kfb {
		uint called;		/* calls to kfreeb */
		uint flush;		/* calls to kfreeb that flushed msg */
	} kfb[NCLASS];

	uint flush_old_kdb[NCLASS];	/* flushes of old k dblks */
};

/* css interface statistics */
struct intrstat {
	uint  intr_in;
	uint  str_intr_out;
	uint  buf_intr_out;
};

struct strifstat {
	uint  new_req;
	uint  msg_in;
	uint  msg_out;
};

struct bufifstat {
	uint  new_req;
	uint  buf_in;
	uint  buf_out;
};

struct iopminfo {
	struct kbstat    *kbstatp;
	struct intrstat  *intrstatp;
	struct strifstat *strifstatp;
	struct bufifstat *bufifstatp;
};

#endif /* SYS_IOPMSTAT_H */
