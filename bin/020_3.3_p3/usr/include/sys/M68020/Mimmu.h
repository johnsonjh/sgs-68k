#ifndef	SYS_M68020_MIMMU_H
#define	SYS_M68020_MIMMU_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Mimmu.h: version 24.1 created on 10/28/91 at 18:29:59	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Mimmu.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * 001 JPC	2/2/88	Changed pg_setndref and pg_clrndref to use dbd's
 * 002 JPC	4/6/88	Atomized lots of pg_ macros.  Added new ones.
 */

#ifndef	SYS_TYPES_H
#include "sys/types.h"
#endif
#ifndef SYS_VMEM_H
#include "sys/vmem.h"
#endif

/*
 * Page Descriptor (Table) Entry Definitions
 */

typedef union pde {    /*  page descriptor (table) entry  */
	struct {
		uint pg_pfn	: 24,	/* Physical page frame number */
		     pg_cw	:  1,	/* Copy on write (fault) */
		     pg_pres	:  1,	/* Page is present in memory */
		     pg_ref	:  1,	/* Page has been referenced */
		     pg_mod	:  1,	/* Page has been modified */
		     pg_cache	:  1,	/* page cacheable */
		     pg_rd	:  1,	/* page readable */
		     pg_wr	:  1,	/* page writeable */
		     pg_x	:  1	/* page executable */
	} pgm;

	struct {
		uint	pg_pde;		/* Full page descriptor (table) entry */
	} pgi;

	uint		pde_all;	/* Full page descriptor (table) entry */
} pde_t;

#define pg_v	pg_pres		/* present bit = valid bit */

/*
 *	Page Table
 */

#define NPGPT		512		/* Nbr of pages per page table (seg). */
#define LOG_NPGPT 	9		/* log2(NPGPT) for use in param.h */
typedef union ptbl {
	int page[NPGPT];
} ptbl_t;

/* Page descriptor (table) entry dependent constants */

#define	NBPP		4096		/* Number of bytes per page */
#define	NBPPT		2048		/* Number of bytes per page table */
#define	BPTSHFT		11 		/* LOG2(NBPPT) if exact */
#define NDPP		4		/* Number of disk blocks per page */
#define DPPSHFT		2		/* Shift for disk blocks per page. */

#define PNUMSHFT	12		/* Shift for page number from addr. */
#define PNUMMASK	0x7f		/* Mask for page number in segment. */
#define POFFMASK        0xFFF		/* Mask for offset into page. */
#define PNDXMASK	0x7FFFFF	/* Mask for page index into section. */
#define	PFGNMASK	0xFFFFF		/* Mask page frame nbr after shift. */
#define PSTARTMASK	0xFFFFF000	/* Mask off page offset bits */

#define	NPTPP		2		/* Nbr of page tables per page.	*/
#define	NPTPPSHFT	1		/* Shift for NPTPP. */

/* Page descriptor (table) entry field masks */

#define PG_ADDR		0xFFFFFF00	/* physical page address */
#define PG_COPYW	0x00000080	/* copy on write bit */
#define PG_P		0x00000040	/* page present bit */
#define PG_REF		0x00000020	/* reference bit */
#define PG_M		0x00000010	/* modify bit */
#define	PG_CB		0x00000008	/* page cacheable */
#define PG_R		0x00000004	/* page readable */
#define PG_W		0x00000002	/* page writeable */
#define	PG_X		0x00000001	/* page contains code only */

/*
 * page attribute bit fields defined in DBD structure
 */
#define PG_NDREF	0x00400000 	/* this page was touched during
					   last "vhand" cycle */
#define PG_LOCK		0x00800000 	/* do not swap this page */


	/* misc */
#define PG_UNINIT	0		/* uninitialized pde value */

#define PTSIZE		2048		/* page table size in bytes */
#define PTSZSHFT	11		/* page table size shift count */


/* byte addr to virtual page */

#define pnum(X)   ((uint)(X) >> PNUMSHFT) 

/* page offset */

#define poff(X)   ((uint)(X) & POFFMASK)

/* round address down to nearest page boundary */
#define pstart(X)	((uint)(X) & PSTARTMASK)

/*	The page number within a section.
*/

#define	pgndx(X)	(((X) >> PNUMSHFT) & PNDXMASK)

/* Round up page table address */

#define ptround(p)	((uint *) (((uint)p + PTSIZE-1) & ~(PTSIZE-1)))

/* Round down page table address */

#define ptalign(p)	((int *) ((int)p & ~(PTSIZE-1)))

/*	Disk blocks (sectors) and pages.
*/

#define	ptod(PP)	((PP) << DPPSHFT)
#define	dtop(DD)	(((DD) + NDPP - 1) >> DPPSHFT)
#define dtopt(DD)	((DD) >> DPPSHFT)

/*	Disk blocks (sectors) and bytes.
*/

#define	dtob(DD)	((DD) << SCTRSHFT)
#define	btod(BB)	(((BB) + NBPSCTR - 1) >> SCTRSHFT)
#define	btodt(BB)	((BB) >> SCTRSHFT)

/*	Page tables (512 entries == 2048 bytes) to pages.
*/

#define	pttopgs(X)	((X + NPTPP - 1) >> NPTPPSHFT)
#define	pttob(X)	((X) << BPTSHFT)
#define	btopt(X)	(((X) + NBPPT - 1) >> BPTSHFT)

union ptbl *getptbl();		/* page table allocator */

extern int		nptalloced;
extern int		nptfree;

/* Form page descriptor (table) entry from modes and page frame
** number
*/

#define	PFNUMSHFT	8	/* shift from page frame to pt format */

uint	mkpde();

/*	The following macros are used to check the value
 *	of the bits in a page descriptor (table) entry 
 */

#define pg_isvalid(P)	 	((P)->pgm.pg_pres)
#define pg_islocked(D)		((D)->dbd_pglock)

#define pg_isndref(D)		((D)->dbd_pgndref)
#define pg_ismod(P)		((P)->pgm.pg_mod && (P)->pgm.pg_wr)
#define pg_iswp(P)		(!(P)->pgm.pg_wr)
#define pg_iscode(P)		((P)->pgm.pg_x)
#define pg_iscw(P)		((P)->pgm.pg_cw)
#define pg_iscacheable(P)	((P)->pgm.pg_cache)	
#define pg_isref(P)		((P)->pgm.pg_ref)
#define pg_isuninit(P)		(((P)->pgi.pg_pde & ~(PG_REF | PG_M)) == 0)

#define pg_valpfn(P)		((P)->pgm.pg_pfn)
#define pg_sbusaddr(P)		((P)->pgi.pg_pde & PG_ADDR)
#define pg_all(P)		((P)->pgi.pg_pde)

/*	The following macros are used to set the value
 *	of the bits in a page descrptor (table) entry 
 *
 *	Atomic instructions (atom_and, atom_or, and atom_set in ml/cas.s)
 *	set or clear pdes, while ordinary instructions are OK for dbds.
 */

					/* Set valid bit, and cacheable	*/
#define pg_setvalid(P)	(atom_or((P), (PG_P|PG_CB)))

#define pg_clrvalid(P)	(atom_and((P), ~PG_P))	/* Clear valid bit.	*/

#define pg_setndref(D)	((D)->dbd_pgndref = 1)	/* Set need ref bit.	*/
#define pg_clrndref(D)	((D)->dbd_pgndref = 0)	/* Clr need ref bit.	*/

#define pg_setlock(D)	((D)->dbd_pglock = 1)	/* Set lock bit.	*/
#define pg_clrlock(D)	((D)->dbd_pglock = 0) 	/* Clear lock bit.	*/

#define pg_setmod(P)	(atom_or((P), PG_M))	/* Set modify bit.	*/
#define pg_clrmod(P)	(atom_and((P), ~PG_M))	/* Clear modify bit.	*/
#define pg_clrwp(P)	(atom_or((P), PG_W))	/* allow writing	*/
#define pg_setwp(P)	(atom_and((P), ~PG_W))	/* prevent writing	*/
#define pg_setcp(P)	(atom_or((P), PG_R|PG_X))

#define pg_clrcb(P)	(atom_or((P), PG_CB))	/* set? cacheable	*/

#define pg_setcw(P)	(atom_or((P), PG_COPYW)) /* Set copy-on-write	*/
#define pg_clrcw(P)	(atom_and((P), ~PG_COPYW)) /* Clr copy-on-write	*/

#define pg_setref(P)	(atom_or((P), PG_REF)) 	/* Set ref bit.		*/
#define pg_clrref(P)	(atom_and((P), ~PG_REF)) /* Clear ref bit.	*/

#define pg_setrw(P)	(atom_or((P), PG_R|PG_W)) /* Set read and write	*/
#define pg_setrwx(P)	(atom_or((P), PG_R|PG_W|PG_X)) /* Set r, w, and x */

#define pg_setuninit(P)	(atom_set((P), 0))	/* make uninitialized	*/
#define pg_setall(P,v)	(atom_set((P), (v)))	/* set pde value	*/

/*
 * Segment Descriptor (Table) Entry Definitions
 *	Arix calls this an entry in the PAGE POINTER TABLE
 */

typedef	union sde {	/*  segment descriptor table enty  */
/*
 *   +------------------------------------------------+------------+
 *   |         sfn (segment frame number)             | unused     |
 *   +------------------------------------------------+------------+
 *               25                                       7
 */
	struct {
		uint	sg_sfn  	: 25;	/* address of page table */
		uint	sg_unused	: 7;	/* unused bits */
	} sgm;

	uint	sg_sge;		/* Full segment descriptor entry */

	uint	sde_all;	/* Full segment descriptor entry */
} sde_t;

/* 
 * definition for changing protection fields. (Uses sg_unused)
 */

#define SEG_RO		0
#define SEG_RW		1
#define SEG_TEXT	2


/*	Segment descriptor (table) dependent constants.	*/

#define NBPS		0x200000 /* Number of bytes per segment */
#define SNUMSHFT	21	/* Shift for segment number from address. */
#define SNUMMASK	0x3FF	/* Mask for segment number after shift. */
#define SOFFMASK	0x1FFFFF/* Mask for offset into segment. */
#define PPSSHFT		9	/* Shift for pages per segment. */

#define snum(X)   ((uint)(X) >> SNUMSHFT) /* Index into contiguous tables */
#define SNUM(X)   (((uint)(X) >> SNUMSHFT) & SNUMMASK) /* Index into table */
#define soff(X)   ((uint)(X) & SOFFMASK)
#define segidx(s) ((uint)(s) & SNUMMASK)	/* snum index within seg tbl */

typedef	union rde {	/*  root descriptor enty  */
	struct {
		uint	rde_rfn  	: 24;	/* address of segment table */
		uint	rde_unused	: 8;	/* unused bits */
	} rdm;

	uint	rde_all;	/* Full root descriptor entry */
} rde_t;

/*
 * Dummy qde structure to keep the size of own_t and spm_mem_t the same
 * for two and three level MMUs (i.e. SPARC, 68040, i486, etc.).
 * Craig hates this style with a passion.  (So don't show it to him.)
 */

typedef union qde {
	uint	qde_all;
} qde_t;



/*	The following variables describe the memory managed by
**	the kernel.  This includes all memory above the kernel
**	itself.
*/

extern uint	kpbase;		/* The address of the start of	*/
				/* the first virtual page of	*/
				/* memory above the kernel.	*/
				/* Memory from here to the end	*/
				/* of mapped physical memory	*/
				/* is represented in the pfdat.	*/
extern int	syssegs[];	/* Start of the system segment	*/
				/* from which kernel space is	*/
				/* allocated.  The actual value	*/
				/* is defined in the vuifile.	*/
extern int	win_ublk[];	/* A window into which a	*/
				/* u-block can be mapped.	*/

extern pde_t	own_ptbl[];	/* Per processor page table.	*/
				/* Used for u and own*		*/
extern pde_t	uarea_ptbl[];	/* A subset of own_ptbl		*/
extern pde_t	*kptbl;		/* Kernel page table.  Used to	*/
				/* map sysseg.			*/

extern sde_t	own_stbl[];	/* Per processor segment table	*/
extern sde_t	mem_stbl[];	/* Prototype kernel segment table */
extern sde_t	pt_invalid;	/* invalid value in mem_stbl	*/
extern int	maxmem;		/* Maximum available free	*/
				/* memory.			*/
extern int	freemem;	/* Current free memory.		*/
extern int	availrmem;	/* Available resident (not	*/
				/* swapable) memory in pages.	*/
extern int	availsmem;	/* Available swapable memory in	*/
				/* pages.			*/
extern 	int *flush_tlb_reg;	/* for flush tlb purpose */

/*	Conversion macros
*/

/*	Get virtual page number from system virtual address.  */

#define	svtovp(X)	((uint)(X) >> PNUMSHFT) 

/*	Get system virtual address from virtual page number.  */

#define	vptosv(X)	((uint)(X) << PNUMSHFT)


/*	Between kernel virtual addresses and the kernel
**	segment table entry.
*/

#define	kvtokstbl(vaddr) (&mem_stbl[snum(vaddr)])

/*	Between kernel virtual addresses and the kernel page
**	table.
*/

#define	kvtokptbl(X)	(&kptbl[pgndx((uint)(X) - (uint)syssegs)])

/*	The following routines are involved with the pfdat
**	table described in pfdat.h
*/

#define SpfnToSlotShft		20		/* shift from spfn to slot id */

#define SpfnSlotMask		0x00f00000	/* mask out all but slot id */
#define SpfnSlotToU32Shft	8		/* shift to uppermost nibble */

#define SPFNTOFULLSLOT		16		/* shift 16 to get slot/sub */


/* flags used in ptmemall() call
*/

#define PHYSCONTIG 02
#define NOSLEEP    01

/* this is defined in vmem.h */
/* #define UMEM_SIZE	(unsigned)0x80000000	/* 2 GB user space */

#define MAXUSEG		((unsigned)UMEM_SIZE >> SNUMSHFT)
					/* maximum user segments */

#define MTAG_INDEX_MASK		0x001ff000
#define LOG_TO_TLB_OFFSET_SHIFT	10		/* log2(NBPP/sizeof(pde)) */
#define TLB_SYS_USER_SPLIT_BIT	0x00200000

struct pfdat	*kvtopfdat();
struct pfdat	*pfn_to_pfdat();
uint	pfdattopfn();

uint	pde_to_pfn();
struct pfdat	*pde_to_pfdat();
uint	pde_to_km();
uint	sde_to_km();
uint	rde_to_km();
sde_t	km_to_sde();
rde_t	km_to_rde();

uint	svtopfn();

#endif	/* SYS_M68020_MIMMU_H */
