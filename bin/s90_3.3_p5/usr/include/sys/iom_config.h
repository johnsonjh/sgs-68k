#ifndef SYS_IOM_CONFIG_H
#define SYS_IOM_CONFIG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iom_config.h: version 24.1 created on 10/28/91 at 18:34:00	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iom_config.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * sys/iom_config -- IOM expansion board configuration info
 */

#ifndef SYS_CSS_BD_DAT_H
#include "sys/css_bd_dat.h"
#endif

typedef struct iom_config {
	uchar		iom_slot;
	uchar		card_cage_type;		/* IOA, IOSBA etc */
	union ex_card_cage {

		/* information about boards in IOSBA type card cage */
		css_bd_dat_t	css_bd_dat[SBUS_NUM_SLOT];
	} ex_card_cage;
} iom_config_t;

#endif /* SYS_IOM_CONFIG_H */
