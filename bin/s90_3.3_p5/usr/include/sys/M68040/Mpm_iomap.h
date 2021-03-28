#ifndef	SYS_M68040_MPM_IOMAP_H
#define	SYS_M68040_MPM_IOMAP_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Mpm_iomap.h: version 24.1 created on 10/28/91 at 18:30:46	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Mpm_iomap.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifndef	SYS_LIO_H
#include <sys/lio.h>
#endif	/* SYS_LIO_H */

		/* Kernel Virtual IO Map Macros */

/*  31                     20         18   17   16     12                     0
 * +-------------------------+----------+----+----+------+---------------------+
 * |                         | RESERVED |  VALID  | Slot | Mb Offset Into Slot |
 * |            ALL ZERO     |          | LO | HI |      |                     |
 * +-------------------------+----------+----+----+------+---------------------+
 */

/* map entries and addresses are also described in lio.h */

extern	uint kv_iomap[];

#define mnum_to_km(mb)			(b_to_mb(mb))

/* transfer between magabytes and pages */
#define	MB_TO_PG_SHFT			(MNUMSHFT - PNUMSHFT)
#define pgtomb(pg)			(((uint)(pg)) >> MB_TO_PG_SHFT)
#define mbtopg(mb)			(((uint)(mb)) << MB_TO_PG_SHFT)

/* translate a kernel address (assumed in range 0x[c-f].......) to map entry */
#define KV_IOMAP_INDEX_FM_KM(km)	(mnum(km)) /* 1 MB/entry */
#define KV_IOMAP_OFFSET_FM_KM(km)	(KV_IOMAP_INDEX_FM_KM((uint)km)*PM_MAP_STEP)

#define	KV_IOMAP_NUM_MB		PM_MAP_MANY /* 4096 entries to span 4 Gb */
#define	KV_IOMAP_SLOT_MASK	MAP_SLOT
#define	KV_IOMAP_SLOT_SHIFT	12	/* from MAP_SLOT defined in lio.h */

/* the area from c to ff.. is set up by kernel */
#define KV_IOMAP_IO_INDEX	KV_IOMAP_INDEX_FM_KM(0xc0000000)

/* the area 0xff000000 to the unused portion must be set to point at self */
#define KV_IOMAP_LOCAL_INDEX	KV_IOMAP_INDEX_FM_KM(PM_ROM_START)

/* the last 32 entries are unused -- but must be set valid */
#define KV_IOMAP_UNUSED_INDEX	KV_IOMAP_INDEX_FM_KM(0xff800000)


#define slot_fm_kv_iomap(entry_ptr)	\
	     (((*entry_ptr) & KV_IOMAP_SLOT_MASK) >> KV_IOMAP_SLOT_SHIFT)

#define slot_to_kv_iomap(slot)	(KV_IOMAP_SLOT_MASK & \
				((slot) << KV_IOMAP_SLOT_SHIFT))

#define KV_IOMAP_SPFN_MASK	0x00ffff
#define spfn_fm_kv_iomap(entry) (mbtopg((entry) & KV_IOMAP_SPFN_MASK))

#define	KV_IOMAP_MB_MASK	MAP_ADDR
#define	KV_IOMAP_MB_SHIFT	0	/* from MAP_ADDR defined in lio.h */

#define mb_fm_kv_iomap(entry_ptr)	\
	(((*entry_ptr) & KV_IOMAP_MB_MASK) >> KV_IOMAP_MB_SHIFT)

#define mb_to_kv_iomap(mb)		(KV_IOMAP_MB_MASK & \
					((mb) << KV_IOMAP_MB_SHIFT))

#define	set_valid_kv_iomap(entry_ptr) ((*entry_ptr) = \
		((*entry_ptr) & ~IOMAP_VALID_BITS) | IOMAP_VALID)

#define	set_slot_kv_iomap(entry_ptr, slot) ((*entry_ptr) = \
		((*entry_ptr) & ~KV_IOMAP_SLOT_MASK) \
		| (((slot) << KV_IOMAP_SLOT_SHIFT) & KV_IOMAP_SLOT_MASK))

#define	set_mb_kv_iomap(entry_ptr, mb) ((*entry_ptr) = \
		((*entry_ptr) & ~KV_IOMAP_MB_MASK) \
		| (((mb) << KV_IOMAP_MB_SHIFT) & KV_IOMAP_MB_MASK))

#define	is_valid_kv_iomap(entry_ptr) \
	(((*entry_ptr) & IOMAP_VALID_BITS) == IOMAP_VALID)

#define	clr_valid_kv_iomap(entry_ptr) ((*entry_ptr) = \
	((*entry_ptr) & ~IOMAP_VALID_BITS) | IOMAP_INVALID)



/* 
 * Defines for the pm_iomap portion of the kv_iomap.  This portion maps
 * the locations 0xc0000000 - 0xffffffff.  These defines are set to mimic
 * the 68020 iomap.
 * There are 1024 entries that span this space, but according to the hardware
 * spec, 1016 are available.  The last are local registers, etc.
 */

extern uint	*pm_int_req_regs[];

/* Defines for routines using the IO_SPACE of the DPM40 */

#define	UintBits	32	/* number of bits in a uint */

/* How mapper works on addresses */ 
#define	PM_UnmappedBits	20	/* lower 20 bits don't get mapped */
#define	PM_MapInBits	12	/* next 12 bits get mapped */
#define	PM_MapOutBits	0	/* to 16 bits */

/* Layout of mapper register */
#define	PM_MapOutShft	0	/* offset of MapOut in map register */

/* offset of sbus slot in map register */
#define PM_MapSlotShft	KV_IOMAP_SLOT_SHIFT


#define	PM_IOMAP_SIZE	(KIO_SIZE / IOMAP_MANY) /* size of a mapped region */


#define	PM_IO_MAPPER	((uint *)PM_IOMAP_BASE)	/* array of mapping registers */
#define	PM_IO_SPACE	((unsigned char *)KIO_START)	/* mapped region */

#define Upper_Mbyte	0x0fff		/* value to place in map register to
					 * set upper megabyte as offset
					 */

#define	PM_MyMap		16	/* map that iomap uses */

#define PM_Int_Req_Reg_Index	17	/* map for use in clearing pending
					 * interrupts.  see iomap_init().
					 */

#define Int_Disp_Index		18	/* map entry dedicated to addressing
					 * the interrupt dispatcher, both for
					 * writing and ack'ing
					 */
#define PIOMAP_START_INDEX	200

#define	LowBits(nbits, value)	((value) & ((1 << (nbits)) - 1))
#define	HighBits(nbits, value)	((value) >> (nbits))

#define ICB_MAP_ENTRY	68	/* map entry number reserved for first ICB */
#define NEXT_ICB_MAP	16	/* offset in iomap to next ICB map pair */


#define	IOMAP_BASE		((uint *)PM_MAP_START + 0xc00)
#define PM_IOMAP_BASE		IOMAP_BASE
#define IOMAP_MANY		1024
#define	USEABLE_IOMAPS		(IOMAP_MANY - KV_IOMAP_UNUSED_INDEX)
#define	IOMAP_SECTION_SIZE 	0x100000
#define	IOMAP_VALID_BITS	(MAP_VBIT_N|MAP_VBIT)
#define	IOMAP_VALID		(MAP_VBIT)
#define	IOMAP_INVALID		(MAP_VBIT_N)


/* hanna FIX -- make these nicer by giving base addr, not index number */
#define	iomap_is_valid(index) \
	((PM_IO_MAPPER[(index)] & IOMAP_VALID_BITS) == IOMAP_VALID)
#define	iomap_set_valid(index) \
	((PM_IO_MAPPER[(index)] & ~IOMAP_VALID_BITS) | IOMAP_VALID)
#define	iomap_clr_valid(index) \
	((PM_IO_MAPPER[(index)] & ~IOMAP_VALID_BITS) | IOMAP_INVALID)

			/* LPFN Macros */
/*  31                                       20               8               0
 * +-------------------------------------------+---------------+---------------+
 * |                                           | Master IO Map |  Page  Offset |
 * |                ALL ZERO                   |  Index (Mb #) |  Into that Mb |
 * +-------------------------------------------+---------------+---------------+
 */

#define	LPFN_INDEX_MASK		0xfff00
#define	LPFN_INDEX_SHIFT	8
#define	index_fm_lpfn(lpfn)	(((lpfn) & LPFN_INDEX_MASK) >> LPFN_INDEX_SHIFT)
#define	index_to_lpfn(index)	(((index)<< LPFN_INDEX_SHIFT) & LPFN_INDEX_MASK)

#define	LPFN_PG_MASK		0x000ff
#define	LPFN_PG_SHIFT		0
#define	pg_fm_lpfn(lpfn)	(((lpfn) & LPFN_PG_MASK) >> LPFN_PG_SHIFT)
#define	pg_to_lpfn(pg)		(((pg) << LPFN_PG_SHIFT) & LPFN_PG_MASK)



			/* SPFN Macros */
/*  31                          24     20                     8               0
 * +------------------------------+------+---------------------+---------------+
 * |                              | Slot | Mb Offset Into Slot |  Page  Offset |
 * |            ALL ZERO          |      | (fm kv_iomap entry) |  Into that Mb |
 * +------------------------------+------+---------------------+---------------+
 */
#define	SPFN_SLOT_MASK		0xf00000
#define	SPFN_SLOT_SHIFT		20
#define	slot_fm_spfn(spfn)	(((spfn) & SPFN_SLOT_MASK) >> SPFN_SLOT_SHIFT)
#define	slot_to_spfn(slot)	(((slot) << SPFN_SLOT_SHIFT) & SPFN_SLOT_MASK)

#define	SPFN_MB_MASK		0xfff00
#define	SPFN_MB_SHIFT		8
#define	mb_fm_spfn(spfn)	(((spfn) & SPFN_MB_MASK) >> SPFN_MB_SHIFT)
#define	mb_to_spfn(mb)		(((mb) << SPFN_MB_SHIFT) & SPFN_MB_MASK)

#define	SPFN_PG_MASK		0xff
#define	SPFN_PG_SHIFT		0
#define	pg_fm_spfn(spfn)	(((spfn) & SPFN_PG_MASK) >> SPFN_PG_SHIFT)
#define	pg_to_spfn(pg)		(((pg) << SPFN_PG_SHIFT) & SPFN_PG_MASK)




			/* SBUS_U32 Macros */
/*  31  28                    16               8                   0
 * +------+---------------------+---------------+-------------------+
 * | Slot | Mb Offset Into Slot |  Page  Offset |  Line             |
 * |      | (fm kv_iomap entry) |  Into that Mb | bits 4-11 fm vaddr|
 * +------+---------------------+---------------+-------------------+
 */

#define SBUS_SPFN_SHIFT		8
#define	spfn_fm_sbus_u32(sbus_u32)	(((uint) (sbus_u32)) >> SBUS_SPFN_SHIFT)
					/* note: following makes pg offset 0 */
#define	spfn_to_sbus_u32(spfn)		((spfn) << SBUS_SPFN_SHIFT)

#define	SBUS_MB_MASK		0x0fff0000
#define	SBUS_MB_SHIFT		16
#define mb_fm_sbus_u32(sbus_u32) (((sbus_u32) & SBUS_MB_MASK) >> SBUS_MB_SHIFT)
#define mb_to_sbus_u32(mb)	(((mb) << SBUS_MB_SHIFT) & SBUS_MB_MASK)

#define	SBUS_PG_MASK		0x0000ff00
#define	SBUS_PG_SHIFT		8
#define pg_fm_sbus_u32(sbus_u32) (((sbus_u32) & SBUS_PG_MASK) >> SBUS_PG_SHIFT)
#define pg_to_sbus_u32(pg)	(((pg) << SBUS_PG_SHIFT) & SBUS_PG_MASK)

#define	SBUS_LN_MASK		0x000000ff /* mask wrt sbus */
#define	SBUS_LN_SHIFT		0
#define ln_fm_sbus_u32(sbus_u32) (((sbus_u32) & SBUS_LN_MASK) >> SBUS_LN_SHIFT)
#define ln_to_sbus_u32(ln)	(((ln) << SBUS_LN_SHIFT) & SBUS_LN_MASK)

		/* various macros to translate a 16 byte line */
#define	VADDR_LN_MASK		0xff0	/* mask wrt a vaddr */
#define VADDR_LN_SHIFT		4
#define	ln_fm_vaddr(vaddr)	(((vaddr) & VADDR_LN_MASK) >> VADDR_LN_SHIFT)
#define	ln_to_vaddr(ln)		(((ln) << VADDR_LN_SHIFT) & VADDR_LN_MASK)
#define	mpfn_to_vaddr(mpfn)	((mpfn) << PNUMSHFT)

uint	km_to_sbus_u32();
void	kv_iomap_init();
void	lpfn_init();
uint	lpfn_to_mpfn();
uint	lpfn_to_spfn();
void	mem_stripe_init();
uint	mpfn_to_lpfn();
uint	mpfn_to_spfn();
uint	sbus_u32_to_km();
uint	spfn_to_lpfn();
#define	lpfn_to_pfn	lpfn_to_mpfn
#define	pfn_to_lpfn	mpfn_to_lpfn
#define	pfn_to_spfn	mpfn_to_spfn
#endif	/* SYS_M68040_MPM_IOMAP_H */
