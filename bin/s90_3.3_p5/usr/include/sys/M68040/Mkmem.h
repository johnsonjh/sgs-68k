#ifndef	SYS_M68040_MKMEM_H
#define SYS_M68040_MKMEM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Mkmem.h: version 24.1 created on 10/28/91 at 18:30:38	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Mkmem.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * Kernel supervisor space
 */

#ifndef	SYS_TYPES_H
#include "sys/types.h"
#endif

#ifndef	SYS_SBUS_SPM_H
#include "sys/sbus_spm.h"
#endif

#ifndef	SYS_VMEM_H
#include "sys/vmem.h"
#endif

#ifndef	SYS_PM_IOMAP_H
#include "sys/pm_iomap.h"
#endif

#define is_kern_addr(x)		(((uint)x & 0x80000000))

/*
 * Own virtual segment(s) are different on every processor.
 */

#define	ADDR_OWN_SEGS	((uint) 0x80000000)


#define U_SIZE		((uint) 0x600)		/* size of user_t and ofiles */
#define U_OFFSET	(ctob(USIZE) - U_SIZE)	/* user_t's offset in page */

#define ADDR_OWN_BAD	((uint)	0x80000000)	/* page that causes buserr */
#define	ADDR_EXTRA_STK	((uint) 0x80001000)	/* per PM extended stack page */
#define	ADDR_U		((uint) 0x80002000)	/* per process vars and stack */
#define	ADDR_U_STRUCT	(ADDR_U + U_OFFSET)	/* user structure's address */
#define	ADDR_OWN_EXTRA	((uint) 0x80011000)	/* per PM swtch u extra pg */
#define	ADDR_OWN_U	((uint) 0x80012000)	/* per PM u used to swtch */
#define	ADDR_OWN_U_STR	(ADDR_OWN_U + U_OFFSET)	/* per PM u structure's addr */
/* hanna FIX: when confident in mmu, squish these to 0x200 bounds here & spm */
#define	ADDR_OWN_QTBL	((uint) 0x80020000)	/* per PM segment table */
#define	ADDR_OWN_STBL	((uint) 0x80021000)	/* per PM segment table */
#define	ADDR_OWN_PTBL	((uint) 0x80025000)	/* per PM page table	*/
#define	ADDR_OWN	((uint) 0x80026000)	/* per PM variables	*/
#define ADDR_OWN_VMAP	((uint) 0x80027000)	/* per PM vmap page */


/*
 * Kernel virtual segment(s) change as the kernel's needs change
 */

#define	ADDR_SYS_SEGS	((uint) 0x90000000)


/*
 * Memory virtual segments are set up one by the SPM and never change
 * They describe all of the main memory of the system.
 */

#define	ADDR_MEM_SEGS	((uint) 0xa0000000)

#define	MAINSTORE	ADDR_MEM_SEGS

			/* SPM communication area */
#define	ADDR_SPM_MEM		((uint) 0xa0000000)
			/* kern stbl for memory segs */
#define	ADDR_MEM_QTBL		((uint) 0xa0001000)
			/* kern stbl for memory segs-need 16tbls to cover .5G */
		/* hanna FIX: do you need 32tbl to cover 8-c ? 0x4000b */
#define	ADDR_MEM_STBL		((uint) 0xa0002000)

			/* proc 0's u area */
#define	ADDR_P0_U_AREA		((uint) 0xa0008000)
			/* proc0's u struct	*/
#define	ADDR_P0_U_STR		(ADDR_P0_U_AREA + U_OFFSET)
			/* start of kernel in memory */
#define	ADDR_KERNEL		((uint) 0xa000a000)

	/* address of page table entry used to map u */
#define ADDR_UAREA_PTBL	(ADDR_OWN_PTBL + pnum(ADDR_U - ADDR_OWN_SEGS) * sizeof(pde_t))
#define ADDR_END_U	(ADDR_U + ctob(USIZE))		/* end of u_page +1 */
#define ADDR_END_OWN_U	(ADDR_OWN_U + ctob(USIZE))	/* end of own u_page +1 */


/* address to flush out the tlb entry for ADDR_U */
#define U_TLB_FLUSH	(uint *)((((ADDR_U & MTAG_INDEX_MASK) | \
			TLB_SYS_USER_SPLIT_BIT) >> LOG_TO_TLB_OFFSET_SHIFT) | \
			(uint)MTAG_START)

/* address to flush out the tlb entry for ADDR_OWN_VMAP */
#define OWN_VMAP_FLUSH	(uint *)((((ADDR_OWN_VMAP & MTAG_INDEX_MASK) | \
			TLB_SYS_USER_SPLIT_BIT) >> LOG_TO_TLB_OFFSET_SHIFT) | \
			(uint)MTAG_START)



/* Some fixed locations in IOMAP space */

/* the interrupt dispatcher */
#define ADDR_INT_DISP	(uint *) \
			(KIO_START + \
			(Int_Disp_Index<<PM_UnmappedBits) + \
			LowBits(PM_UnmappedBits,SPM_INT_DISPATCHER))

/* addresses within the interrupt dispatcher to ack the various levels */
#define INT_DISP_ACK_LEVEL_ONE \
			(uint) \
			(KIO_START + \
			(Int_Disp_Index<<PM_UnmappedBits) + \
			LowBits(PM_UnmappedBits,SPM_ACK_LEVEL_ONE))
#define INT_DISP_ACK_LEVEL_TWO \
			(uint) \
			(KIO_START + \
			(Int_Disp_Index<<PM_UnmappedBits) + \
			LowBits(PM_UnmappedBits,SPM_ACK_LEVEL_TWO))
#define INT_DISP_ACK_LEVEL_THREE \
			(uint) \
			(KIO_START + \
			(Int_Disp_Index<<PM_UnmappedBits) + \
			LowBits(PM_UnmappedBits,SPM_ACK_LEVEL_THREE))
#define INT_DISP_ACK_LEVEL_FOUR \
			(uint) \
			(KIO_START + \
			(Int_Disp_Index<<PM_UnmappedBits) + \
			LowBits(PM_UnmappedBits,SPM_ACK_LEVEL_FOUR))
#define INT_DISP_ACK_LEVEL_FIVE \
			(uint) \
			(KIO_START + \
			(Int_Disp_Index<<PM_UnmappedBits) + \
			LowBits(PM_UnmappedBits,SPM_ACK_LEVEL_FIVE))
#define INT_DISP_ACK_LEVEL_SIX \
			(uint) \
			(KIO_START + \
			(Int_Disp_Index<<PM_UnmappedBits) + \
			LowBits(PM_UnmappedBits,SPM_ACK_LEVEL_SIX))

/*
 * macros for use by utilites
 */
	/* kv_to_kmem is used to figure out where to lseek on /dev/kmem */
#define	kv_to_kmem(addr)	((addr) - KMEM_START)
	/* kv_to_mem is used to figure out where to lseek on /dev/mem */
#define	kv_to_mem(addr)		((addr) - ADDR_MEM_SEGS)

#endif /* SYS_M68040_MKMEM_H */
