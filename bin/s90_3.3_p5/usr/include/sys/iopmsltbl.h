#ifndef SYS_IOPMSLTBL_H
#define SYS_IOPMSLTBL_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iopmsltbl.h: version 24.1 created on 10/28/91 at 18:34:22	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iopmsltbl.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

struct iopmsltbl {			/* IOPM slot table */
	uchar  iopmslot;
	uchar  valid;
	dev_t  iopmdev;
};

#define SLTBLSZ   2048			/* max minors (0x8000) / MNPG */
#define MNPGSHFT  4			/* minor numbers per group shift */
#define MNPG  (1<<MNPGSHFT)		/* min numbers per group of minors */

#endif /* SYS_IOPMSLTBL_H */
