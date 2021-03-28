#ifndef SYS_IOPMCOMM_H
#define SYS_IOPMCOMM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iopmcomm.h: version 24.1 created on 10/28/91 at 18:34:12	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iopmcomm.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef SYS_STREAM_H
#include "sys/stream.h"
#endif

#ifndef SYS_STRSTAT_H
#include "sys/strstat.h"
#endif

#ifndef SYS_BUFSTAT_H
#include "sys/bufstat.h"
#endif

#ifndef SYS_TDB_H
#include "sys/tdb.h"
#endif

#ifndef SYS_SBUS_IOPM_H
#include "sys/sbus_iopm.h"
#endif

#define PRINTFBUFFLEN  128
#define PREFIXLEN      16

/*
** version number to coordinate versions of IOPM os with the kernel and spm.
*/
#define IOPM_VERSION  0x04271990

typedef struct iopm_comm
{
	uint             iopm_version;
	uint             o_boot_response;
	uint             o_reprime_clock;/* set when IOPM will take clk intr */
	uint             o_clocks_dropped;

	struct buf       *ktoib;	/* head of kernel to iopm block req's */
	struct buf       **itokbp;	/* ptr to head of iopm blk responses */
	mblk_t           *ktois;	/* head of kernel to iopm stream reqs */
	mblk_t           **itoksp;	/* ptr to head of iom str responses */

	/* configuration */
	uint             *stblp;	/* pointer to segment table */
	uint             memsize;	/* size of IOPM memory in bytes */
	uint             freemem;	/* unused memory in bytes */
	uint             slot;		/* slot of this IOPM */
	uint             dbtype;	/* device board type */

	/* printf & tdb stuf for SPM */
	uint             o_printf_count;
	uint             o_spm_printf_count;
	uchar            o_char_buff[PRINTFBUFFLEN];
	tdb_t            o_tdb;

	/* io/iopm.c device interaction */
	uint             iopm_cmd;
	uint             iopm_arg[9];
	uchar            iopm_arg_str[PREFIXLEN];
	uint             iopm_response;

	struct sysinfo   *ksysinfop;	/* pointer to kernel sysinfo */

	/*
	**	information for crash and similar utilities
	*/
	struct strstat   istrst;	/* IOPM streams statistics */

	queue_t          *queuep;	/* IOPM queue array */
	uint             nqueue;	/* number of queues in array */

	mblk_t           *mblkp;	/* IOPM mblk array */
	uint             nblk[NCLASS];
	mblk_t           *imbfreelist;

	caddr_t          dblkp;		/* IOPM dblk array */
	struct dbalcst   idballoc[NCLASS];
	dblk_t           *idbfreelist[NCLASS];

	struct iqueue    *qihead;	/* head of queues to run */
	struct iqueue    *qitail;	/* last queue */

	struct use_stat  itcbst;	/* statistics for tcb's */
	struct tcb       *tcbarray;
	uint             numtcb;	/* number of tasks */

	struct bufstat   ibufstat;	/* statistics for bufs drivers */
	caddr_t          bufarray;
	uint             numbuf;
	struct i_buf     *bparray;
	uint             numbp;

	struct icallo    *callo_head;	/* head of linked callout list */

	/* FIX THIS, CMW: move the next five fields to appropriate
	 * positions within the struct next time we roll version
	 */
	struct iopminfo  *iopminfop;
	uint             numeblk;

	uint                   dizzy;
	struct lock_req_array  *lock_req_ptr;
	uint                   lock_id;
} iopm_comm_t;

#define IOPM_COMM  IOPM_RAM_START

/* Used by IOPM. Not used by the kernel since there are many IOPMs */
extern struct iopm_comm  iopmcomm;
extern struct buf        *ktoib;
extern queue_t           *ktois;

#endif /* SYS_IOPMCOMM_H */
