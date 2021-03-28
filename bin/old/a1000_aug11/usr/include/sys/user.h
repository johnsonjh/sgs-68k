#ifndef SYS_USER_H
#define SYS_USER_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) user.h: version 2.1 created on 5/22/89 at 19:12:32	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)user.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/user.h	10.15"		*/

#ident	"@(#)uts/head/sys:user.h	1.3"

#ifndef SYS_FS_S5DIR_H
#include "sys/fs/s5dir.h"
#endif

#ifndef SYS_PARAM_H
#include "sys/param.h"
#endif

#ifndef SYS_SIGNAL_H
#include "sys/signal.h"
#endif

#ifndef SYS_IMMU_H
#include "sys/immu.h"
#endif

#define USIZE		2	/* # of 4k pages in ublock		*/
#define	INTVEC_START	0	/* mmu slot offset of interrupt vecs	*/
#define	UPAGE1_START	1	/* mmu slot offset of 1st page of ublock*/
#define	UPAGE2_START	2	/* mmu slot offset of 2nd page of ublock*/
#define	KCODE_START	3	/* mmu slot offset of kernel code start	*/

/*
**	extra pages offsets
*/
#define	EXTRA_IVEC	0	/* interrupt vectors per slave		*/
#define	EXTRA_AVAIL	1	/* available for future use		*/
#define	EXTRA_OWN1	2	/* 1st page of own area per slave	*/
#define	EXTRA_OWN2	3	/* 2nd page of own area per slave	*/
#define	EXTRA_UPAGE1	4	/* 1st page of U area per slave		*/
#define	EXTRA_UPAGE2	5	/* 2nd page of U area per slave		*/
#define	EXTRA_PSIZE	6	/* # of phys pages for extra per slave	*/

#define NEXECSAVENTRY 16
#define	PSARGSZ	80	/* Space in u-block for exec arguments.	*/
			/* Used by ps command.			*/

#define	PSCOMSIZ	DIRSIZ	/* For the time being set PSCOMSIZ */
				/* to DIRSIZ until we can get rid of */
				/* struct direct u_dent */

/*
 * The user structure.
 * One allocated per process.
 * Contains all per process data that doesn't need to be referenced
 * while the process is swapped.
 *
 */
typedef	struct	user
{
	char	u_slreturn;	/* set if returning from slave */
	char	u_acflag;
	char	u_segflg;	/* IO flag: 0:user D; 1:system; 2:user I */
	int	u_error;	/* return error code */
	caddr_t	u_base;		/* base address for IO */
	unsigned u_count;	/* bytes remaining for IO */
	off_t	u_offset;	/* offset in file for IO */
	int	*u_ap;		/* pointer to arglist */
	struct proc *u_procp;	/* pointer to proc structure */
	char	u_comm[PSCOMSIZ];
	unchar	u_save_exec;	/* restore exec registers */
	unchar	u_save_window;	/* defines which registers to restore */
	int     u_nommu;	/* no need to write mmu */
	char	*u_fault_addr;	/* Ptr to function to handle user space */
				/* faults encountered in the kernel */
	char	*u_comp;	/* pointer to current component */
	char	*u_nextcp;	/* pointer to beginning of next */
	struct	mmugrp *u_mmugrp_ptr[4];/* mmu register set */
	char	u_slots_chg[4];		/* byte indicating need for rewrite */
	int	u_top_mmureg;		/* mmuregs space required text+data */
	int	u_text_top_regs;	/* mmuregs space required for text */
	int	u_datorg_snum;		/* snum(u.u_datorg) */
	int	u_datorg_soff;		/* text and data share */

					/* following for Distributed UNIX */
 	struct rem_ids {		/* for exec'ing REMOTE text */
 		ushort	ux_uid;		/* uid of exec'd file */
 		ushort	ux_gid;		/* group of exec'd file */
 		ushort	ux_mode;	/* file mode (set uid, etc. */
 	} u_exfile;
	ushort 		u_rflags;	/* flags for distribution */
	int 		u_syscall;	/* system call number */
	int		u_mntindx;	/* mount index from sysid */
	struct sndd	*u_gift;	/* gift from message      */
	struct response	*u_copymsg;	/* copyout unfinished business */
	struct msgb	*u_copybp;	/* copyin premeditated send    */
	char 		*u_msgend;	/* last byte of copymsg + 1    */
					/* end of Distributed UNIX */
	
	void(*u_signal[MAXSIG])();	/* disposition of signals */

	label_t	u_rsav;			/* save area for stack exchange */
	label_t	u_qsav;			/* save area for quits and interrupts */

	char	u_psargs[PSARGSZ];	/* arguments from exec system call */
	char	*u_tracepc;		/* Return PC if tracing enabled */

	ushort	u_uid;			/* effective user id */
	ushort	u_gid;			/* effective group id */

	ushort	u_ruid;			/* real user id */
	ushort	u_rgid;			/* real group id */

	union {				/* syscall return values */
		struct	{
			int	r_val1;
			int	r_val2;
		}r_reg;
		off_t	r_off;
		time_t	r_time;
	} u_r;

	short	u_fmode;	/* file mode for IO */
	ushort	u_pbsize;	/* Bytes in block for IO */

	ushort	u_pboff;	/* offset in block for IO */
	short	u_errcnt;	/* syscall error count */
	daddr_t	u_rablock;	/* read ahead block address */

	struct inode *u_cdir;	/* current directory */
	struct inode *u_rdir;	/* root directory */
	struct inode *u_pdir;	/* inode of parent directory of dirp */
	caddr_t	u_dirp;		/* pathname pointer */
	struct direct u_dent;	/* current directory entry */

	char	*u_pofile;	/* Ptr to open file flag array.	 */
	int	u_arg[6];	/* arguments to current system call */
	int	*u_ar0;		/* address of users saved R0 */

	time_t	u_utime;	/* this process user time */
	time_t	u_stime;	/* this process system time */
	time_t	u_cutime;	/* sum of childs' utimes */
	time_t	u_cstime;	/* sum of childs' stimes */

	struct {			/* profile arguments */
		short	*pr_base;	/* buffer base */
		unsigned pr_size;	/* buffer size */
		unsigned pr_off;	/* pc offset */
		unsigned pr_scale;	/* pc scaling */
	} u_prof;

	short  *u_ttyp;			/* pointer to pgrp in "tty" struct */
	dev_t	u_ttyd;			/* controlling tty dev */
	dev_t	u_pbdev;		/* real device for IO */
	struct inode *u_ttyip;		/* inode of controlling tty (streams) */

	struct exdata {
		struct    inode  *ip;
		long      ux_tsize;	/* text size    */
		long      ux_dsize;	/* data size    */
		long      ux_bsize;	/* bss size     */
		long      ux_lsize;  	/* lib size     */
		long      ux_nshlibs; 	/* num of shared libs needed*/
		short     ux_mag;   	/* magic number MUST be here */
		long      ux_toffset;	/* file offset to raw text   */
		long      ux_doffset;	/* file offset to raw data   */
		long      ux_loffset;	/* file offset to lib sctn   */
		long      ux_txtorg;	/* start addr. of text in mem */
		long      ux_datorg;	/* start addr. of data in mem */
		long      ux_entloc;	/* entry location             */
	} u_exdata;
	char	pad;
	char	u_reenter_tas;
	long   	u_execsz;

	time_t	u_start;
	time_t	u_ticks;
	long	u_mem;
	long	u_ior;
	long	u_iow;
	long	u_iosw;
	long	u_ioch;

	short	u_lock;			/* process/text locking flags */

	short	u_cmask;		/* mask for file creation */
	daddr_t	u_limit;		/* maximum write address */

	int	*u_regs_ptr;		/* allow slave to find trap registers */
	int	u_ssize;
	int	u_tsize;
	int	u_dsize;
	
	ulong	u_stack_top;		/* stack lower bound */
	short   mmu1index;		/* this set of variables is used to */
	short   offset1;		/* point into the mmuslots array to */
	int     length1;		/* inform the device driver which  */
	short   mmu2index;		/* memory is to be read/written to */
	short	u_page_table_pfn;	/* page number of segment table */
	int	u_page_table_off;	/* offset into page */
	pde_t	*u_page_table_alt;	/* pointer to first data page table */
	pde_t	u_execptbl[NEXECSAVENTRY]; /* page entries used by exec */
	pde_t	u_savfault;		/* saved page entry used by fault */
	pde_t	u_savvhand;		/* saved page entry used by vhand */
	pde_t	u_savsrc;		/* saved page entry used by src */
	char	u_srcsavmap;		/* save map for prev active pg table */
	pde_t	u_savrdmap;		/* saved page entry used by readmap */
	short	u_fppsave_state[92];	/* fpp stat registers save area */
	long	u_fppsave_cont[3];	/* fpp control registers save area */
	long	u_fppsave_data[24];	/* fpp data registers save area */
	caddr_t	u_datorg;		/* start of data page */
	daddr_t	u_limit_adj;		/* maximum write address */
	int	u_systrap;
	int	u_nmbuf;		/* set to pfn if nmbp mapped */
	long	u_spare1[3];
	long	u_spare2[4];
	long	u_rcstat;
		
	struct file	*u_ofile[1];	/* Start of array of pointers	*/
					/* to file table entries for	*/
					/* open files.			*/
} user_t;

#define u_exuid u_exfile.ux_uid
#define u_exgid u_exfile.ux_gid
#define u_exmode u_exfile.ux_mode

#define	u_rval1	u_r.r_reg.r_val1
#define	u_rval2	u_r.r_reg.r_val2
#define	u_roff	u_r.r_off
#define	u_rtime	u_r.r_time

/* rcstat values for client caching */
#define	U_RCACHE	0x1

/* ioflag values: Read/Write, User/Kernel, Ins/Data */
#define	U_WUD	0
#define	U_RUD	1
#define	U_WKD	2
#define	U_RKD	3
#define	U_WUI	4
#define	U_RUI	5

#define	EXCLOSE	01

/* distribution: values for u_rflags */
#define FREMOTE	0x0002	/* file is remote  */

#define	U_RSYS		0x0004	/* system call has gone remote */
#define	U_DOTDOT	0x0200
#define U_RCOPY		0x0400	/* used by copyout for non-delay copy */


/* bit positions of the data access and code access mmu fault enables */
#define DATAACCESSFLTENB  0x20
#define CODEACCESSFLTENB  0x10
#define MAPENABLE	  0x03		/* enable stats also */
#define IDFLTENB	  0x04
#define WRTPROTFLTENB	  0x08
#define STATSENABLE	  0x02

#define	NMMUREGS	4096		/* max mmu regs for 12.5 Mhz cpu  */
#define	NMMUREGS25	16384		/* max mmu regs for 25.0 Mhz cpu  */
#define	KMAXMMUSLOTS	1024		/* max kernel mmu slot for memory */

/* format of mmu regs */
struct mmureg  {
	unsigned   unused2     : 4;
	unsigned   pagenum     : 12;
	unsigned   unused1     : 4;
	unsigned   attributes  : 5;
	unsigned   userid      : 7;
};

/* bit positions of mmu attributes */
#define MMUWRITEPROT 1
#define MMUACCESSED  2
#define MMUCACHEABLE 8
#define MMUCODEPAGE  16

/* flag values for virtphys indicating whether the address is a 
   kernel address of user address
*/
#define KERNEL  0
#define USER    1

/* kernel id for mmu registers */
#define KERNELID 0

/* invalid indication for user id */
#define MMUINVALID 0x7f
#define MMUVALID   0x00
#define MMUSIMUID  0x7e

#define	PAGESIZE	NBPP
 
struct fpp_proc  {
	long	fppsave_cont[3];	/* fpp control registers save area */
	long	fppsave_data[24];	/* fpp data registers save area */
};

extern	struct	user	u;
extern	int	userstack;

/* constant offset used to translate kernel virtual addresses to physical */
extern	int	kerneloffset;

extern	short	*useroffsetreg;	/* address of user offset mmu register */
extern	char	*useridreg;	/* address of userid mmu register */
extern	char	*enblregmmu;	/* address of fault enable mmu register */
extern	char	enblstatemmu;	/* current contents of fault enable mmu reg */

#endif /* SYS_USER_H */
