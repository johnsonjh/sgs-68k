#ifndef SYS_GCTTY_MAP_H
#define SYS_GCTTY_MAP_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gctty_map.h: version 2.1 created on 5/22/89 at 19:07:23	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gctty_map.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
struct gctty_map
{
	unsigned short num_tty_on_board;
	unsigned short first_tty_on_board;
} gctty_map[];

#endif /* SYS_GCTTY_MAP_H */
