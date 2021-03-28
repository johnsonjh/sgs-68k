#ifndef SYS_IOPM_MMU_H
#define SYS_IOPM_MMU_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iopm_mmu.h: version 24.1 created on 10/28/91 at 18:34:04	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iopm_mmu.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#if !defined SYS_IMMU_H
#include "sys/immu.h"
#endif

typedef union ipde {    /*  page descriptor (table) entry  */
	struct {
		uint pg_pfn	: 24;	/* Physical page frame number */
		uint 		:  1;	/* unused */
		uint pg_ci	:  1;	/* cache inhibit */
		uint 		:  1;	/* unused */
		uint pg_mod	:  1;	/* Page has been modified */
		uint pg_u	:  1;	/* page used */
		uint pg_wp	:  1;	/* page write protected */
		uint pg_dt	:  2;	/* descriptor type */
	} pgm;

	struct {
		uint	pg_pde;		/* Full page descriptor (table) entry */
	} pgi;
} ipde_t;

#define LOWER   0x80000000	/* limit defines lower limit */
#define LMASK   0x7fff0000	/* limit field */
#define DESC    0xfc00
#define S       0x100		/* supervisor only */
#define CI      0x40		/* cache inhibit this page */
#define M       0x10		/* page is modified */
#define U       0x8		/* page is used */
#define WP      0x4		/* page is write protected */
#define DTMASK  0x3		/* descriptor type mask */
#define PTDESC8 0x3		/* descriptor decribes 8 byte page tables */
#define PTDESC4 0x2		/* descriptor decribes 4 byte page tables */
#define PGDESC  0x1		/* descriptor is a page descriptor */
#define INVALID 0x0		/* descriptor describes an invalid segment */
#define PAMASK  0xffffff00	/* page descriptor page address mask */
#define TAMASK  0xfffffff0	/* table pointer address mask */

#define INVALIDTD   (LOWER | DESC | INVALID)	/* invalid table descriptor */
#define PTDESC      (LOWER | DESC | PTDESC4)	/* 4 byte page tables */
#define EARLYTERM   (LOWER | DESC | PGDESC)	/* early termination */
#define SHORTPT     (DESC | PTDESC4)

#define SEG_SHFT 20
#define SEG_SIZE (1 << SEG_SHFT)	/* 1 Mbyte segments */
#define NPDPPT (SEG_SIZE / NBPP)	/* number pde per page table */
#define MAXPDPMM 0xffff		/* max of pde per MM (256M per MM) */

#define PWINSZ   SEG_SIZE

#define U32TOADDR  4		/* shift u32 4 bits to left to get addr */
#define U32TOSLOT  28		/* shift u32 28 bits right to get slot */

#define PDETOMAP   28		/* shift pde 28 bits right to get map */
#define PDETOPFN   12		/* shift pde 12 bits right to get page # */

struct mmutbl {
	uint  limit;
	uint  tblp;
};

#endif /* SYS_IOPM_MMU_H */
