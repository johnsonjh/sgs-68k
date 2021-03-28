/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) devsw.h: version 24.1 created on 10/28/91 at 17:59:49	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)devsw.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef IOP_DEVSW_H
#define IOP_DEVSW_H

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

/*
 * Declaration of block device switch. Each entry (row) is
 * the only link between the main unix code and the driver.
 */
struct bdevsw {
	void	(*d_open)();
	void	(*d_close)();
	void	(*d_strategy)();
	void	(*d_print)();
	uint	dma;
};
extern struct bdevsw bdevsw[];

/*
 * Character device switch.
 */
struct cdevsw {
	void              (*d_open)();
	void              (*d_close)();
	void              (*d_ioctl)();
	struct streamtab  *d_str;
};
extern struct cdevsw cdevsw[];

#define FMNAMESZ 8

struct fmodsw {
	char              f_name[FMNAMESZ+1];
	struct streamtab  *f_str;
};
extern struct fmodsw  fmodsw[];

extern uint	bdevcnt;
extern uint	cdevcnt;
extern int	fmodcnt;

#endif /* IOP_DEVSW_H */
