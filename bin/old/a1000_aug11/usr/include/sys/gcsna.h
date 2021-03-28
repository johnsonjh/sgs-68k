#ifndef SYS_GCSNA_H
#define SYS_GCSNA_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gcsna.h: version 2.1 created on 5/22/89 at 19:07:13	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gcsna.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys:gcsna.h	2.1"

/*
 * GCP board SNA driver
 */

/*
	master to slave commands
*/

#define	MS_SNA_OPEN		(0|GCSNA_COMMANDS)
#define	MS_SNA_CLOSE		(1|GCSNA_COMMANDS)
#define	MS_SNA_PARAM		(2|GCSNA_COMMANDS)
#define MS_SNA_BTU		(3|GCSNA_COMMANDS)
#define MS_SNA_MU		(4|GCSNA_COMMANDS)
#define	MS_SNA_WORK		(5|GCSNA_COMMANDS)

/*
	slave to master commands
*/

#define SM_SNA_BTU		(0|GCSNA_COMMANDS)
#define	SM_SNA_HLFLGS		(1|GCSNA_COMMANDS)
#define	SM_SNA_WAKELU		(2|GCSNA_COMMANDS)
#define SM_SNA_MU		(3|GCSNA_COMMANDS)
#define SM_SNA_TRACE		(4|GCSNA_COMMANDS)
#define SM_SNA_WAKE_ALL_LU	(5|GCSNA_COMMANDS)

#endif /* SYS_GCSNA_H */
