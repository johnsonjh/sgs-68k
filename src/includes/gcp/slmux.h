#ifndef SYS_GCP_SLMUX_H
#define SYS_GCP_SLMUX_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) slmux.h: version 2.1 created on 5/22/89 at 19:06:56	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)slmux.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys/gcp:slmux.h	2.1"

#define SLMUX_H

#define SL_MUX_WRITE_MAX_BYTES	256
#define SL_MUX_READ_MAX_BYTES	256

typedef struct _muxbuf {
	struct gcttm	*tp;
	char		data[SL_MUX_WRITE_MAX_BYTES];
	short		count;
	short		mode;
} SL_MUX_BUF;


#endif /* SYS_GCP_SLMUX_H */
