#ifndef SYS_CSS_BD_DAT_H
#define SYS_CSS_BD_DAT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) css_bd_dat.h: version 24.1 created on 10/28/91 at 18:31:54	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)css_bd_dat.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * css_bd_dat.h -- define CSS bus board info structure used by both SPM and PMs
 */

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

/* information about boards in CSS type card cage */
typedef struct css_bd_dat {
	uchar		css_slot_id;	/* board's CSS ID		*/
	uchar		dev_board_id;	/* device board's ID if IOPM	*/
} css_bd_dat_t;

#endif /* SYS_CSS_BD_DAT_H */
