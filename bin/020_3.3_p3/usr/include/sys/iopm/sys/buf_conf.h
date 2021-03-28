/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) buf_conf.h: version 24.1 created on 10/28/91 at 17:59:42	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)buf_conf.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef IOPM_BUF_CONF_H
#define IOPM_BUF_CONF_H

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

struct buf_config
{
	uint  bc_version;
	char  *bc_prefix;
	void  (*bc_open)();
	void  (*bc_close)();
	void  (*bc_strat)();
	void  (*bc_print)();
	void  (*bc_ioctl)();
	void  (*bc_start)();
	uint  bc_dma;
};

#define BC_VERSION  0x09111989

#endif /* IOPM_BUF_CONF_H */
