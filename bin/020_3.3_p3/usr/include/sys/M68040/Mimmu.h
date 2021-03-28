#ifndef	SYS_M68040_MIMMU_H
#define SYS_M68040_MIMMU_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Mimmu.h: version 24.1 created on 10/28/91 at 18:30:31	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Mimmu.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef LOCORE
#ifndef	SYS_TYPES_H
#include <sys/types.h>
#endif	/* SYS_TYPES_H */
#endif	/* ! LOCORE */

#define	THREE_LEVEL_MMU

/* hanna FIX: make a picture here of the table and entry stuff */
/************************************************************************/
/*			Root Descriptor Entry				*/
/************************************************************************/
#ifndef	LOCORE
typedef	union rde {	/*  root descriptor enty  */
    struct {
	uint	fn	: 23,	/* address of next segment table */
		rsvd	: 9;	/* Reserved bits: must be zero */
    } rde;

    uint	rde_all;	/* Full root descriptor entry */
} rde_t;

#define	SZOFRDE	sizeof(rde_t)	/* Sizeof of a root descriptor */
#endif	/* !LOCORE */
#ifdef	THREE_LEVEL_MMU
#define NPPRE	PNPPQT		/* Number of pages an entry describes */
#else	/* THREE_LEVEL_MMU         --- assume a two level mmu */
#define NPPRE	PNPPST		/* Number of pages an entry describes */
#endif	/* THREE_LEVEL_MMU */
#define NBPRE	NPPRE*NBPP	/* Number of bytes an entry describes */	

#ifdef	THREE_LEVEL_MMU
/************************************************************************/
/*		Q-segment Descriptor Entry				*/
/************************************************************************/

#ifndef	 LOCORE 

typedef union qde {    /*  Q-segment descriptor entry  */
    struct {
	uint	fn	: 23,	/* Address of segment table */
    		rsvd	: 5,	/* Reserved bits */
		u	: 1,	/* Used */
		wp	: 1,	/* Write protected */
		udt	: 2;	/* Upper level descriptor type */
    } qde;

    uint	qde_all;		/* Full segment descriptor entry */
} qde_t;

#endif	/* !LOCORE */

#define	SZOFQDE 4		/* sizeof(qde_t) a Q-segment Table Entry */
#define	NEPQT	128		/* Number entries per table */
#define	NBPQT	(NEPQT*SZOFQDE)	/* Number of bytes per table */
#define QTALIGN	NBPQT		/* Alignment requirement of table in memory */
#define	BQTSHFT	9		/* Table size as a power of two: log2(NBPQT) */
#define NQTPP	(NBPP/NBPQT)	/* Number of tables that fit in a page */
#define NQTPPSHFT 3		/* Shift for NQTPP. log2(NQTPP) */
#define NPPQE	PNPPST		/* Number of pages an entry describes */
#define NBPQE	(NPPQE*NBPP)	/* Number of bytes an entry describes */	
#define PNPPQT	(NPPQE*NEPQT)	/* Number of pages a table describes */
#define PNBPQT	(PNPPQT*NBPP)	/* Number of bytes a table describes */
#define	Q6XX	0xfe000000	/* Mask of bits in vaddr to index table */
#define QNUMMASK 0x7f		/* Mask for index into table after shift */
#define QNUMSHFT   25		/* No. Shifts to Right justify index to table */
#define CTOQSHFT   13		/* No. of Shifts to convert from clicks to qnum*/
#define STOQSHFT   7		/* No. of Shifts to convert from seg to Qseg */
#define	QOFFMASK 0x1ffffff	/* Mask for offset into Qsegment */
#ifndef	LOCORE
#define qnum(X)   ((uint)(X) >> QNUMSHFT)		/* Index into table */
#define ctoq(X)   (((uint)(X) + ((1 << CTOQSHFT) - 1)) >> CTOQSHFT)
#define ctoqt(X)  ((uint)(X) >> CTOQSHFT)		/* trunc click to Qseg*/
#define qtoc(X)   ((uint)(X) << CTOQSHFT)		/* Qseg to clicks */
#define stoq(X)   (((uint)(X) + QNUMMASK) >> STOQSHFT)	/* segments to Qseg */
#define stoqt(X)  ((uint)(X) >> STOQSHFT)		/* trunc seg to Qseg */
#define qtos(X)   ((uint)(X) << STOQSHFT)		/* Qseg to segments */
#define QNUM(X)   (qnum(X))				/* Index into table */
#define qoff(X)   ((uint)(X) & QOFFMASK)	      /* Offset into Qsegment */
#endif	/* THREE_LEVEL_MMU */
#endif	/* !LOCORE */

/************************************************************************/
/*			Segment Descriptor Entry			*/
/************************************************************************/

#ifndef	LOCORE

typedef union sde {    /*  segment descriptor entry  */
    struct {
	uint	fn	: 24,	/* Physical page frame number */
		rsvd	: 4,	/* Reserved bits */
		u	: 1,	/* Used */
		wp	: 1,	/* Write protected */
		udt	: 2;	/* Upper level descriptor type */
    } sde;

    uint	sde_all;	/* Full segment descriptor entry */
} sde_t;

#endif	/* !LOCORE */

#define	SZOFSDE	4		/* sizeof(sde_t)--Sizeof of segmenttable entry*/
#define	NEPST	128		/* Number entries per table */
#define	NBPST	(NEPST*SZOFSDE)	/* Number of bytes per table */
#define STALIGN		NBPST	/* Alignment requirement of table in memory */
#define	BSTSHFT	9		/* Table size as a power of two: log2(NBPST) */
#define NSTPP	(NBPP/NBPST)	/* Number of tables that fit in a page */
#define NSTPPSHFT 3		/* Shift for NSTPP. log2(NSTPP) */
#define NPPSE	NPGPT		/* Number of pages an entry describes */
#define NBPSE	(NPPSE*NBPP)	/* Number of bytes an entry describes */	
#define PNPPST	(NPPSE*NEPST)	/* Number of pages a table describes */
#define PNBPST	(PNPPST*NBPP)	/* Number of bytes a table describes */
#define	NBPS	PNBPST
#define	S6XXX	0x01fc0000	/* Mask of bits in vaddr to index table */
#define SNUMMASK 0x3f		/* Mask for index into table ??? */
#define SNUMSHFT   18		/* No. Shifts to Right justify index to table */
#define	SOFFMASK 0x3ffff	/* Mask for offset into segment */
#ifndef	LOCORE
#define snum(X)   ((uint)(X) >> SNUMSHFT) /* Index into contiguous tables */
#define SNUM(X)   (((uint)(X) >> SNUMSHFT) & SNUMMASK) /* Index into table */
#define soff(X)   ((uint)(X) & SOFFMASK)	/* Offset into segment */
#define segidx(s) ((uint)(s) & (NEPST - 1))	/* snum index within seg tbl */
#endif	/* !LOCORE */


/************************************************************************/
/*			Page Descriptor Entry				*/
/************************************************************************/

#ifndef	LOCORE

typedef union pde {    /*  page descriptor (table) entry  */
    struct {
	uint	fn	: 20,	/* Physical page frame number */
		ndref	:  1,	/* needs reference (from 020 dbd) (was UR) */
		g	:  1,	/* Global page -- prevents flushing of page */
		lock	:  1,	/* Don't swap page (from 020 dbd) (was U1) */
		cw	:  1,	/* Copy On Write (from 020 dbd) (was U0) */
		s	:  1,	/* Sys access only */
				/* CM field is actually 2 bits broken up */
		nocache	:  1,	/* Page cacheable==0 */
		cache0	:  1,	/* Should (currently) always be 0 */
		mod	:  1,	/* Page has been modified (was M) */
		ref	:  1,	/* Page has been referenced (was U) */
		wp	:  1,	/* Page is write protected -- note polarity */
				/* PDT field is actually 2 bits broken up */
		pres0	:  1,	/* Should (currently) always be 0 */
		pres	:  1;	/* Page is present */
    } pde;
    uint	pde_all;	/* Full page descriptor (table) entry */
} pde_t;

#endif	/* !LOCORE */

#define SZOFPDE	4		/* sizeof(pde_t) -- Size of a table entry */
#define	NPGPT	64		/* Number entries per table */
#define LOG_NPGPT 6		/* log2(NPGPT) for use in param.h */
#define	NEPPT	NPGPT		/* Number entries per table */
#define	NBPPT	(NPGPT*SZOFPDE)	/* Number of bytes per table */
#define	PTSIZE	NBPPT		/* hanna FIX: can you make this one define? */
#define PTALIGN		NBPPT	/* Alignment requirement of table in memory */
#define	BPTSHFT	8		/* Table size as a power of two */
#define	PTSZSHFT	BPTSHFT /* hanna FIX: make this one define */
#define NPTPP	(NBPP/NBPPT)	/* Number of tables that fit in a page */
#define NPTPPSHFT 4		/* Shift for NPTPP. Must handcode */
#define NPPPE	1		/* Number of pages an entry describes */
#define NBPPE	(NPPPT*NBPP)	/* Number of bytes an entry describes */	
#define PNPPPT	(NEPPT)		/* Number of pages a table describes */
#define PNBPPT	(PNPPPT*NBPP)	/* Number of bytes a table describes */
#define	P6XXX	0x0002f000	/* Mask of bits in vaddr to index table */
#define PNUMSHFT   12		/* No. Shifts to Right justify index to table */
#define PNUMMASK 0x3f		/* Mask for index into table ??? */
#define PNDXMASK 0x7fffff	/* Mask for page index into section. ???? */
#define	POFFMASK 0x00000fff	/* Mask of offset into page, itself */
#define PSTARTMASK	~POFFMASK	/* Mask off page offset bits */
#ifndef	LOCORE
/* round address down to nearest page boundary */
#define pstart(X)	((uint)(X) & PSTARTMASK)
#define pnum(X)   ((uint)(X) >> PNUMSHFT) 
#define PNUM(X)   (((uint)(X) >> PNUMSHFT)  & PNUMMASK)
#define pfn_to_km(X)   ((uint)(X) << PNUMSHFT) 
#define poff(X)   ((uint)(X) & POFFMASK)	/* page offset */
			/* page number within a section */
#define	pgndx(X)	(((X) >> PNUMSHFT) & PNDXMASK) 
			/* Round up page table address */
#define ptround(p)	((uint *) (((uint)p + PTSIZE-1) & ~(PTSIZE-1)))
			/* Round down page table address */
#define ptalign(p)	((int *) ((int)p & ~(PTSIZE-1)))
#endif	/* !LOCORE */

/*
 * Page descriptor (table) entry field masks. 
 * WARNING: These are the true bit positions for the 68020.
 * With the exception of PG_ADDR which is actually used as a mask,
 * these may be just flags for sparc & 68040. 
 */
#define PG_ADDR		0xFFFFF000	/* physical page address */
#define PG_REF		0x00000008	/* reference bit */
#define PG_M		0x00000010	/* modify bit */

    /* The PG_CB field is 2 bits wide use as flag for now 
     * And use the higher bit as the cachable bit, inverted.
     *  *0      Meaning
     * ================================================
     *	00	Cachable, writethrough	(CACHABLE)
     *  01	Cachable, copyback
     *  10	Noncachable, serialized	(NOT CACHABLE)
     *  11	Noncachable, not serialized
     */

#define	PG_CB		0x00000040	/* page cacheable */
#define PG_CACHE_BITS   0x00000060	/* cache field mask */

    /* The PG_P field is 2 bits wide use as flag for now */
#define PG_P		0x00000001	/* page present bit,w/in entry type */

    /* For the 68040, the W bit is actually write protect */
    /*
     *    ~W S		USER	SYS
     *     0 0          rwx     rwx	(stack/data/rw:shm?)
     *     0 1          ---     rwx
     *     1 0          r-x     r-x	(text segment/ro:shm)
     *     1 1          ---     rwx
     */
    /* PG_RWX included for backward compatibility */
#define PG_R		0x00000000	/* page readable flag */
#define PG_W		0x00000004	/* page writeable flag (active low */
#define	PG_X		0x00000000	/* page contains code only  flag */

#define PG_WP		0x00000004	/* actual write protect field bit */
#define PG_S		0x00000080	/* actual supervisor field bit */

    /* Some bits moved over from the dbd structure */
#define PG_COPYW	0x00000100	/* copy on write bit */

    /* .....and the global field, unique to the 68040 MMU */
#define PG_G		0x00000400	/* global bit */

#define	PG_ACTIVE_LOW (PG_W | PG_CB)	/* invert these bits in pde_t */
/************************************************************************/
/*			Global Defines 					*/
/************************************************************************/
#define	NBPP	4096		/* Number of bytes per page */
#define PGALIGN	NBPP		/* Alignment required for a page */
#define	NDPP	4		/* Number of disk blocks per page */
#define	DPPSHFT	2		/* Shift for disk blocks per page */

#define NOSLEEP    01

/*
 * page attribute bit fields that WERE defined in DBD structure
 */
#define PG_NDREF	0x00000800 	/* this page was touched during
					   last "vhand" cycle */
#define PG_LOCK		0x00000200 	/* do not swap this page */
	
	/* Some Upper Descriptor Table Entries */
#define	UDT_INVALID	0x0	/* also 0x1 is invalid, ignore this */
#define	UDT_PRES	0x2	/* also 0x3 is present, ignore this */

	/* misc. */
/*
 * We can't just use 0 for the uninitialized pde value, because a lpfn of 0
 * is a valid lpfn, and 0xfffff means board registers, so use 0xfefff.
 */
#define PG_UNINIT	(0xFEFFF000 | PG_ACTIVE_LOW) /* uninitialized pde */

/*
 * pde_mode_to_mask translates the page descriptor mode settings, 
 * such as cacheable, present, read, write, execute.... to the actual
 * mask that should go into the pde.  For processors that have one bit
 * for each mode, this is no problem, and the function is null.  For the
 * 68040 or sparc, whose rwx modes are encoded (see Mimmu.h) the bits
 * must be manipulated.  Also, the write bit is inverted (it is a write
 * protect bit) for the 68040.
 */
#define pde_mode_to_mask(mode) ((uint)(mode) ^ PG_ACTIVE_LOW)


/* 
 * Shift constant to put an lpfn into the proper position in an mmu
 * table entry. (that is, left justified (highest bits) in the long)
 */
#define	LPFN_XDE_SHFT	12
/* 
 * definition for changing protection fields. (Uses sg_unused)
 */

#define SEG_RO		0
#define SEG_RW		1
#define SEG_TEXT	2

#ifndef	LOCORE

typedef union ptbl {
    int page[NPGPT];
} ptbl_t;

/* Union of all mmu level types -- for ease of conversion */
typedef union {
    rde_t	rde;
#ifdef	THREE_LEVEL_MMU
    qde_t	qde;
#endif	/* THREE_LEVEL_MMU */
    sde_t	sde;
    pde_t	pde;
} xde_t;

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

/*	Page tables to pages.
*/

#define	pttopgs(X)	((X + NPTPP - 1) >> NPTPPSHFT)
#define	pttob(X)	((X) << BPTSHFT)
#define	btopt(X)	(((X) + NBPPT - 1) >> BPTSHFT)

/*	Between kernel virtual addresses and the kernel
**	segment table entry.
*/

#define kvtokstbl(vaddr) \
	(&mem_stbl[(qnum(kv_to_kmem(vaddr)) * NEPST) + SNUM(vaddr)])

/*	Between kernel virtual addresses and the kernel page
**	table.
*/

#define	kvtokptbl(X)	(&kptbl[pgndx((uint)(X) - (uint)syssegs)])



/*	The following macros are used to check the value
 *	of the bits in a page descriptor (table) entry 
 */

#define pg_isvalid(P)	 	((P)->pde.pres)
#define pg_islocked(D)		((D)->dbd_pglock)

#define pg_isndref(D)		((D)->dbd_pgndref)

/* hanna FIX: what is the best way to fix this */
#define pg_ismod(P)		((P)->pde.mod)
#define pg_iscw(P)		((P)->pde.cw)
#define pg_iswp(P)		((P)->pde.wp)
#define pg_isref(P)		((P)->pde.ref)
#define pg_isuninit(P)		(((P)->pde_all & ~PG_REF) == PG_UNINIT)

#define pg_valpfn(P)		((P)->pde.fn)
#define pg_sbusaddr(P)		((P)->pde_all & PG_ADDR) /* hanna FIX: valid? */
#define pg_all(P)		((P)->pde_all)

/*	The following macros are used to set the value
 *	of the bits in a page descrptor (table) entry 
 *
 *	Atomic instructions (atom_and, atom_or, and atom_set in ml/cas.s)
 *	set or clear pdes, while ordinary instructions are OK for dbds.
 */

				/* Set valid bit, and cacheable	*/
				/* hanna FIX: do you need to clear S bit? */
#define pg_setvalid(P)	(atom_and_or((P), ~PG_CACHE_BITS, PG_P)) 
#define pg_clrvalid(P)	(atom_and((P), ~PG_P))	/* Clear valid bit.	*/

#define pg_setndref(D)	((D)->dbd_pgndref = 1)	/* Set need ref bit.	*/
#define pg_clrndref(D)	((D)->dbd_pgndref = 0)	/* Clr need ref bit.	*/

#define pg_setlock(D)	((D)->dbd_pglock = 1)	/* Set lock bit.	*/
#define pg_clrlock(D)	((D)->dbd_pglock = 0) 	/* Clear lock bit.	*/

#define pg_setmod(P)	(atom_or((P), PG_M))	/* Set modify bit.	*/
#define pg_clrmod(P)	(atom_and((P), ~(PG_M)))/* Clear modify bit.	*/

#define pg_clrwp(P)	(atom_and((P), ~(PG_WP)))/* allow writing	*/
#define pg_setwp(P)	(atom_or((P), (PG_WP)))	/* prevent writing	*/
#define pg_setcp(P)	/* only found in chgprot, and seems to do nothing */

			/* Copy on Write	*/
#define pg_setcw(P)	(atom_or((P), PG_COPYW))
#define pg_clrcw(P)	(atom_and((P), ~(PG_COPYW)))

			/* Set ref bit.		*/
#define pg_setref(P)	(atom_or((P), (PG_REF)))
			/* Clear ref bit.	*/
#define pg_clrref(P)	(atom_and((P), ~(PG_REF))) 

			/* Set read and write	*/
#define pg_setrw(P)	(atom_and((P), ~(PG_WP|PG_S)))
#define pg_setrwx(P)	(atom_and((P), ~(PG_WP|PG_S))) /* Set r, w, and x */

/* hanna FIX: do you want an uninit that sets the inverted fields */
#define pg_setuninit(P)	(atom_set((P), PG_UNINIT)) /* make uninitialized*/
#define pg_setall(P,v)	(atom_set((P), (v)))	/* set pde value	*/


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

/* hanna : FIX ME: do you need to add own_qtbl, mem_qtbl sde_invalid ? */
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

/* counters to track number of page tables currently used/free */
extern int		nptalloced;
extern int		nptfree;


struct pfdat	*kvtopfdat();
struct pfdat	*pfn_to_pfdat();
struct pfdat	*pde_to_pfdat();
uint	pde_to_pfn();
uint	pfdattopfn();
uint	svtopfn();

uint	pde_to_km();
uint	sde_to_km();
uint	qde_to_km();
uint	rde_to_km();

uint	mkpde();
sde_t	km_to_sde();
qde_t	km_to_qde();
rde_t	km_to_rde();
#endif	/* !LOCORE */

/* hanna FIX: are these still valid for M68040 */
/* ARIX Hardware Defines */
#define SpfnToSlotShft		20		/* shift from spfn to slot id */
#define SpfnSlotMask		0x00f00000	/* mask out all but slot id */
#define SpfnSlotToU32Shft	8		/* shift to uppermost nibble */

#define SPFNTOFULLSLOT		16		/* shift 16 to get slot/sub */

/* hanna FIX: are these still valid for M68040 */
#define MTAG_INDEX_MASK		0x001ff000
#define LOG_TO_TLB_OFFSET_SHIFT	10		/* log2(NBPP/sizeof(pde)) */
#define TLB_SYS_USER_SPLIT_BIT	0x00200000

/* union for the 040's MMU Status Register (set by ptest) */
typedef union m68040_mmu_sr_u {
	struct m68040_mmu_sr_s {
		uint	msr_fn		: 20,	/* Frame number from pde */
			msr_berr	: 1,	/* Bus err on table walk */
			msr_g		: 1,	/* Global bit from pde	 */
			msr_u1		: 1,	/* U1 bit from pde	 */
			msr_cw		: 1,	/* Copy-on-write (U0)	 */
			msr_s		: 1,	/* S bit from pde	 */
			msr_nocache	: 1,	/* Page cacheable==0	 */
			msr_cache0	: 1,	/* Should always be 0	 */
			msr_mod		: 1,	/* Modified bit from pde */
			msr_unused0	: 1,	/* unused (always 0)	 */
			msr_wp		: 1,	/* Write protect bit	 */
			msr_transparent	: 1,	/* Used TTR, not pde	 */
			msr_resident	: 1	/* Hit TTR or valid pde	 */
	} msr;
	uint	msr_all;
} mmu_sr_t;

#endif /* SYS_M68040_MIMMU_H */
