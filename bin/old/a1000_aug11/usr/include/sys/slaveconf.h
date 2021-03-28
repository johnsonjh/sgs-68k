#ifndef SYS_SLAVECONF_H
#define SYS_SLAVECONF_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) slaveconf.h: version 2.1 created on 5/22/89 at 19:10:37	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)slaveconf.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/* slaveconf.h	1.0 */

#ident	"@(#)uts/head/sys:slaveconf.h	2.1"

struct slave_config {
	unsigned char	proc_status[4];
	int	cmd;
	int	param;
	short	slave_done;
	short	sl_response;
	short	slcpu_id;
	short	num_pages;
	short	proc_type;
	short	sys_req;
	short	extra_page;
	char	slaveslot;
	char	slave_prom_go;
} ;

extern struct slave_config slave_config;


#endif /* SYS_SLAVECONF_H */
