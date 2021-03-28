#ifndef SYS_TDB_H
#define SYS_TDB_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tdb.h: version 24.1 created on 10/28/91 at 18:39:20	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tdb.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif	/* SYS_TYPES_H */

#ifndef SYS_STATE_H
#include "sys/state.h"
#endif	/* SYS_STATE_H */


typedef	struct {
	/* information passed between PM and SPM */
	uint		done;	/* true if cmd completed by PM */

	/* information passed from SPM to PM */
	uint		cmd;	/* cmd for PM to perform */
	ulong		arg;	/* parameter passed to SPM from PM */

	/* information passed from PM to SPM */
	uint		error;	/* result of PM doing cmd */
	ulong		value;	/* parameter passed from PM to SPM */

	/* internal values needed by PM */
#if defined(M68020) || defined(M68040)
	uint		pm_fault_reg;
	state_t		*state;
#endif /* M68020 | M68040 */

	label_t		jbuf;
	uint		in_tdb;		/* used to prevent re-entrancy */

} tdb_t;

/* commands */
#define	TDB_CMD_NONE		0	/* do nothing */ 
#define	TDB_CMD_GO		1	/* go, ignore trace */
#define	TDB_CMD_STEP		2	/* go, using trace to single step */

#define	TDB_CMD_GET_CHAR	0xb
#define	TDB_CMD_PUT_CHAR	0xc
#define	TDB_CMD_GET_SHORT	0xd
#define	TDB_CMD_PUT_SHORT	0xe
#define	TDB_CMD_GET_LONG	0xf
#define	TDB_CMD_PUT_LONG	0x10
#define	TDB_CMD_GET_REG		0x11
#define	TDB_CMD_PUT_REG		0x12

#define TDB_CMD_SHOW_TRAP	0x14
#define TDB_CMD_BACKTRACE	0x15

#define TDB_CMD_CHECK_TAGS	0x1e


/* register space */

#if defined(M68020) || defined(M68040)

#define	TDB_REG_D0	0xd0
#define	TDB_REG_D1	0xd1
#define	TDB_REG_D2	0xd2
#define	TDB_REG_D3	0xd3
#define	TDB_REG_D4	0xd4
#define	TDB_REG_D5	0xd5
#define	TDB_REG_D6	0xd6
#define	TDB_REG_D7	0xd7

#define	TDB_REG_A0	0xa0
#define	TDB_REG_A1	0xa1
#define	TDB_REG_A2	0xa2
#define	TDB_REG_A3	0xa3
#define	TDB_REG_A4	0xa4
#define	TDB_REG_A5	0xa5
#define	TDB_REG_A6	0xa6
#define	TDB_REG_A7	0xa7

#define	TDB_REG_PC	0x00
#define	TDB_REG_SR	0x01
#define	TDB_REG_VECTOR	0x02
#define	TDB_REG_FORMAT	0x03
#define	TDB_REG_VBR	0x04

#endif /* M68020 | M68040 */

#endif /* SYS_TDB_H */
