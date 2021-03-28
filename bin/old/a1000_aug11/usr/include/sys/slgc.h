#ifndef SYS_SLGC_H
#define SYS_SLGC_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) slgc.h: version 2.1 created on 5/22/89 at 19:10:40	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)slgc.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys:slgc.h	2.1"

#define SL_GT_WRITE_MAX_BYTES	256
#define SL_GT_READ_MAX_BYTES	256

typedef struct _gcbuf {
	struct gctty	*tp;
	char		data[SL_GT_WRITE_MAX_BYTES];
	short		count;
	short		mode;
	short		in_use;
} SL_GC_BUF;

/* this is taken from gt0.c */
#define	t_dev	t_eye_catcher[3]

#endif /* SYS_SLGC_H */
