#ifndef SYS_IMMU_H
#define SYS_IMMU_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) immu.h: version 2.1 created on 5/22/89 at 19:07:41	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)immu.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/immu.h	10.5"		*/

#ident	"@(#)uts/head/sys:immu.h	2.1"

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif /* SYS_TYPES_H */

#define NUM_MMUGRP	24
#define NUM_MMUGRP25	124

struct mmugrp {
	struct mmugrp	*next;
	int		top_slot;
	int		bottom_slot;
	int		logical_zero;
	short		mmuid;
	short		procid;
};

union ptbl *getptbl();		/* page table allocator */

extern int		nptalloced;
extern int		nptfree;

/*
 * Page Descriptor (Table) Entry Definitions
 */
typedef union pde {
	struct {
		uint pg_wsimu	:  1,	/* force written bit update */
		     pg_simref	:  1,	/* force reference bit update */
		     pg_pfn	: 14,	/* Physical page frame number */
		     pg_lock	:  1,	/* Lock in core */
		     pg_pres	:  1,	/* Page is present in memory */
		     pg_cw	:  1,	/* Copy on write (fault) */
		     pg_ndref	:  1,	/* needs reference (software) */
		     pg_cp	:  1,	/* code page */
		     pg_cache	:  1,	/* page cacheable */
		     pg_mod	:  1,	/* Page has been modified */
		     pg_ref	:  1,	/* Page has been referenced */
		     pg_wp	:  1,	/* page is write protected */
		     pg_procid	:  7;	/* process id */
	} pgm;

	struct {
		uint	pg_pde;		/* Full page descriptor (table) entry */
	} pgi;
} pde_t;

#define pg_v	pg_pres		/* present bit = valid bit */

/*
 *	Page Table
 */

#define NPGPT		64		/* Nbr of pages per page table (seg). */
#define PTSIZE		256		/* page table size in bytes */
#define PTSZSHFT	8		/* page table size shift count */
typedef union ptbl {
	int page[NPGPT];
} ptbl_t;

/* Page descriptor (table) entry dependent constants */
#define	NBPP		4096		/* Number of bytes per page */
#define	NBPPT		256		/* Number of bytes per page table */
#define	BPTSHFT		8 		/* LOG2(NBPPT) if exact */
#define NDPP		4		/* Number of disk blocks per page */
#define DPPSHFT		2		/* Shift for disk blocks per page. */
#define PNUMSHFT	12		/* Shift for page number from addr. */
#define POFFMASK        0xFFF		/* Mask for offset into page. */
#define	NPTPP		16		/* Nbr of page tables per page.	*/
#define	NPTPPSHFT	4		/* Shift for NPTPP. */

/* Page descriptor (table) entry field masks */
#define	PG_WSIMU	0x80000000	/* force written bit update */
#define	PG_SIMREF	0x40000000	/* force reference bit update */
#define PG_ADDR		0x3FFF0000	/* physical page address */
#define PG_LOCK		0x00008000	/* page lock bit (software) */
#define PG_P		0x00004000	/* page present bit */
#define PG_COPYW	0x00002000	/* copy on write bit */
#define PG_NDREF	0x00001000	/* need reference bit (software) */
#define	PG_CP		0x00000800	/* page contains code only */
#define	PG_CB		0x00000400	/* page cacheable */
#define PG_M		0x00000200	/* modify bit */
#define PG_REF		0x00000100	/* reference bit */
#define PG_WP		0x00000080	/* page write protected */
#define PG_PROCID	0x0000007F	/* 7 bit process id field */



/* byte addr to virtual page */
#define pnum(X)   ((uint)(X) >> PNUMSHFT) 

/* page offset */
#define poff(X)   ((uint)(X) & POFFMASK)

/* Disk blocks (sectors) and pages  */
#define	ptod(PP)	((PP) << DPPSHFT)
#define	dtop(DD)	(((DD) + NDPP - 1) >> DPPSHFT)
#define dtopt(DD)	((DD) >> DPPSHFT)

/* Page tables (64 entries == 256 bytes) to pages  */
#define	pttopgs(X)	((X + NPTPP - 1) >> NPTPPSHFT)
#define	pttob(X)	((X) << BPTSHFT)
#define	btopt(X)	(((X) + NBPPT - 1) >> BPTSHFT)

/* Form page descriptor (table) entry from modes and page frame number */
#define	PFNUMSHFT	16	/* shift from address to page frame */
#define	mkpde(mode,pfn)	(mode | ((pfn) << PFNUMSHFT))


#define pg_setvalid(P) \
{\
			((P)->pgi.pg_pde &= ~(PG_PROCID));\
			((P)->pgi.pg_pde |= (PG_P|PG_CB|MMUVALID));\
}

#define pg_clrvalid(P) \
{\
			((P)->pgi.pg_pde &= ~(PG_P|PG_PROCID));\
			((P)->pgi.pg_pde |= MMUINVALID);\
}
#define pg_issimuid(P)	(((P)->pgi.pg_pde & MMUSIMUID) == MMUSIMUID)
#define pg_isvalid(P) 	((P)->pgi.pg_pde & PG_P)
#define pg_islocked(P)	((P)->pgi.pg_pde & PG_LOCK)
#define pg_isndref(P)	((P)->pgi.pg_pde & PG_NDREF)
#define pg_ismod(P)	((P)->pgi.pg_pde & PG_M)
#define pg_iswp(P)	((P)->pgi.pg_pde & PG_WP)
#define pg_iscode(P)	((P)->pgi.pg_pde & PG_CP)
#define pg_iscw(P)	((P)->pgi.pg_pde & PG_COPYW)
#define pg_iscacheable(P) ((P)->pgi.pg_pde & PG_CB)	
#define pg_isref(P)	((P)->pgi.pg_pde & PG_REF)
#define pg_issimref(P)	((P)->pgi.pg_pde & PG_SIMREF)
#define pg_iswsimu(P)	((P)->pgi.pg_pde & PG_WSIMU) 	
#define pg_setndref(P)	((P)->pgi.pg_pde |= PG_NDREF)
#define pg_setsimref(P)	((P)->pgi.pg_pde |= PG_SIMREF)
#define pg_clrndref(P)	((P)->pgi.pg_pde &= ~PG_NDREF)
#define pg_clrsimref(P)	((P)->pgi.pg_pde &= ~PG_SIMREF)
#define pg_setlock(P)	((P)->pgi.pg_pde |= PG_LOCK)	
#define pg_clrlock(P)	((P)->pgi.pg_pde &= ~PG_LOCK) 
#define pg_setmod(P)	((P)->pgi.pg_pde |= PG_M)	
#define pg_clrmod(P)	((P)->pgi.pg_pde &= ~PG_M)	
#define pg_setwp(P)	((P)->pgi.pg_pde |= PG_WP)
#define pg_clrwp(P)	((P)->pgi.pg_pde &= ~(PG_WP|PG_CP))	
#define pg_setcode(P)	((P)->pgi.pg_pde |= (PG_WP|PG_CP))	
#define pg_setcacheable(P) ((P)->pgi.pg_pde |= PG_CB)	
#define pg_setcw(P)	((P)->pgi.pg_pde |= (PG_COPYW | PG_WP))
#define pg_clrcw(P)	((P)->pgi.pg_pde &= ~(PG_COPYW | PG_WP))
#define pg_setref(P)	((P)->pgi.pg_pde |= PG_REF)
#define pg_clrref(P)	((P)->pgi.pg_pde &= ~PG_REF)
#define pg_setwsimu(P)	((P)->pgi.pg_pde |= PG_WSIMU) 	
#define pg_clrwsimu(P)	((P)->pgi.pg_pde &= ~PG_WSIMU) 
#define pg_setprot(P,b)	

#define phystokv(paddr) (paddr)
#define	pfntopfdat(pfn)	(&pfdat[pfn - kpbase_btoc])
#define	pfdattopfn(pfd)	(pfd - pfdat + kpbase_btoc)
#define	phystopfn(addr)	((uint)(addr) >> PNUMSHFT)


/*	Segment descriptor (table) dependent constants.	*/
#define NBPS		0x40000 /* Number of bytes per segment */
#define SNUMSHFT	18	/* Shift for segment number from address. */
#define SOFFMASK	0x3FFFF	/* Mask for offset into segment. */
#define PPSSHFT		6	/* Shift for pages per segment. */

#define snum(X)   ((uint)(X) >> SNUMSHFT)
#define soff(X)   ((uint)(X) & SOFFMASK)



/*	The following variables describe the memory managed by
**	the kernel.  This includes all memory above the kernel
**	itself.
*/

extern int	kpbase;		/* The address of the start of	*/
				/* the first physical page of	*/
				/* memory above the kernel.	*/
				/* Physical memory from here to	*/
				/* the end of physical memory	*/
				/* is represented in the pfdat.	*/
extern int	kpbase_btoc;
extern int	syssegs[];	/* Start of the system segment	*/
				/* from which kernel space is	*/
				/* allocated.  The actual value	*/
				/* is defined in the vuifile.	*/
extern int	win_ublk[];	/* A window into which a	*/
				/* u-block can be mapped.	*/
extern pde_t	*kptbl;		/* Kernel page table.  Used to	*/
				/* map sysseg.			*/
extern int	maxmem;		/* Maximum available free	*/
				/* memory.			*/
extern int	freemem;	/* Current free memory.		*/
extern int	availrmem;	/* Available resident (not	*/
				/* swapable) memory in pages.	*/
extern int	availsmem;	/* Available swapable memory in	*/
				/* pages.			*/
extern unsigned int	maxuseg;/* max num of user segments     */


/* flags used in ptmemall() call */
#define PHYSCONTIG 	02
#define NOSLEEP    	01

#define SEG_RO		0xc2
#define SEG_RW		0xc3

#define	MAXUSEG	64
#define	MAPTOKV(pt) \
{\
*(short *)own_srcptpslot = phystopfn(pt); \
pt = (pde_t *)((uint)own_srcptpbase | poff(pt));\
}


/* MASK used for updating statistics from hardware mapper */
#define	MASKUSED	0x0FFF0FFF


#endif /* SYS_IMMU_H */
