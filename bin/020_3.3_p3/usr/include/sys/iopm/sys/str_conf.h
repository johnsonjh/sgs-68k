/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) str_conf.h: version 24.1 created on 10/28/91 at 18:00:09	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)str_conf.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ifndef IOPM_STR_CONF_H
#define IOPM_STR_CONF_H

struct str_config
{
	uint              sc_version;
	struct streamtab  *sc_strtab;
	void              (*sc_start)();
};

struct str_mod_config
{
	uint              smc_version;
	struct streamtab  *smc_strtab;
};

#define SC_VERSION   0x09111989
#define SMC_VERSION  0x09111989

#endif /* IOPM_STR_CONF_H */
