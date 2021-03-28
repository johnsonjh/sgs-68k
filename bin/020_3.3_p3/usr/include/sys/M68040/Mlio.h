#ifndef	SYS_M68040_MLIO_H
#define SYS_M68040_MLIO_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Mlio.h: version 24.1 created on 10/28/91 at 18:30:42	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Mlio.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *	lio.h - dpm local i/o
 *
 *	Diag Bag space 0xff100000-0xff1fffff is defined in bagio.h
 *	Associated FIFO's space 0xff300000-oxff3fffff is defined in fifo.h
 *
 *	SNH - 08/24/89
 */

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

/*
	The entire board is reset from the CSS BUS_RESET* bit.

	CPU A will stay in reset until the control write command ASSERT 
	MODULE ENABLE is received.

	CPU B will stay in reset until CPU A set the PCTL_CPUB bit in the 
	PM_PCTL register.

	The local CSS bus logic is disabled after a reset and remains disabled
	until the control write MODULE INTERFACE ENABLE is received.

	After reset the local I/O space 0xff000000 - 0xffffffff is replicated
	through address space.  The PCTL_DECODE bit in the PM_PCTL register 
	should be set to enable decording for all space.
/*

/*
===============================================================================
	Memory Map Entry FORMAT
===============================================================================
*/
typedef union diag_map {
	uint		mapw;		/* whole word			*/

	struct dpm40_mapf {
		uint		: 14;	/* dummy			*/
		uint	v_n	:  1;	/* valid bit - active low	*/
		uint	v	:  1;	/* valid bit - active high	*/
		uint	slot	:  4;	/* slot number			*/
		uint	ba31_20 : 12;	/* bus addr bit 31-20		*/
	} mapf;
	struct dpm40_mape {
		uint		: 14;	/* dummy			*/
		uint	v_n	:  1;	/* valid bit - active low	*/
		uint	v	:  1;	/* valid bit - active high	*/
		uint	slot	:  4;	/* slot number			*/
		uint	ba31_25	:  7;	/* base				*/
		uint	ba24_20	:  5;	/* segment number		*/
	} mape;
} DIAG_MAP;

/*
===============================================================================
	Ctag ENTRY
===============================================================================
*/
typedef union ctag {
	uint		ctagw;		/* whole thing			*/
	struct dpm40_ctagf {
		uint		: 14;	/* unused			*/
		uint	v_u	:  1;	/* valid bit for upper line	*/
		uint	v_l	:  1;	/* valid bit for lower line	*/
		uint	pa31_18	: 14;	/* PA31-18			*/
		uint		:  2;	/* unused			*/
	} ctagf;
	struct dpm40_ctage {
		uint		: 14;	/* unused			*/
		uint	v_u	:  1;	/* valid bit for upper line	*/
		uint	v_l	:  1;	/* valid bit for lower line	*/
		uint	ndx	: 12;	/* memory map index		*/
		uint	pa19_18	:  2;	/* PA19-18			*/
		uint		:  2;	/* unused			*/
	} ctage;
} CTAG;

/*
===============================================================================
	Btag ENTRY
===============================================================================
*/
typedef union btag {
	uint		btagw;		/* whole word			*/
	struct dpm40_btagf {
		uint	v_lb	:  1;	/* valid of lower line cpuB	*/
		uint	v_ub	:  1;	/* valid of upper line cpuB	*/
		uint	v_la	:  1;	/* valid of lower line cpuA	*/
		uint	v_ua	:  1;	/* valid of upper line cpuA	*/
		uint		: 10;	/* unused			*/
		uint	slot	:  4;	/* bus slot number		*/
		uint	ba31_18	: 14;	/* BA31-18			*/
	} btagf;
} BTAG;

/*
=============================================================================
	128KB ROM - shared
=============================================================================
*/
#define PM_ROM_START	(0xff000000)	/* ROM */
#define PM_ROM_SIZE	(0x20000)	/* 128KB */
#define	PM_ROM_MANY	(0x8000)	/* how many long words */

/*
=============================================================================
	1MB DIAGBAG SPACE 0xff100000-0xff1fffff - shared (see bagio.h)
=============================================================================
*/

/*
=============================================================================
	32KB SCRATCH SRAM - shared
=============================================================================
*/
#define	PM_RAM_START	(0xff200000)	/* scratch RAM */
#define PM_RAM_SIZE	(0x8000)	/* 32KB */
#define	PM_RAM_MANY	(0x2000)	/* how many long words */

/*
=============================================================================
	1MB ASSOCIATED FIFO'S SPACE 0xff300000-0xff3fffff - shared (see fifo.h)
=============================================================================
*/

/*
=============================================================================
	16KB MEMORY MAP - S H A R E D , LONG WORD access only

	The upper most 16 map entries are not used.

	Both the CPU's cache and the secondary cache should be disabled 
	and invalidated before changing a cacheable map entry.

	Both valid bits must be programmed correctly before any off board
	access that uses that entry.

	An access to an invalid map entry will cause a level 7 interrupt
	if the access was a write and a bus error on a read.
=============================================================================
*/
#define	PM_MAP_START	(0xff400000)	/* memory map */
#define PM_MAP_STEP	(0x4)		/* address step */
#define	PM_MAP_SIZE	(0x4000)	/* 16KB */
#define	PM_MAP_MANY	(0x1000)	/* how many entries */
#define PM_MAP_MASK	(0x000fffff)	/* implemented bits */
	/* MAP ENRTY DEFINES */
#define MAP_ADDR	(0x00000fff)	/* CSS bus address bit31-20 */
#define MAP_SLOT	(0x0000f000)	/* CSS slot number */
#define MAP_VBIT	(0x00010000)	/* valid bit - active high */
#define MAP_VBIT_N	(0x00020000)	/* valid bit - active low */
#define MAP_RSV		(0x000c0000)	/* reserved bits */

/*
=============================================================================
	32KB SECONDARY CACHE TAG - private, LONG WORD access only

	The secondary cache and bus snoop should be disabled before
	accessing the secondary cache tag, secondary cache data store
	and bus tag.
=============================================================================
*/
#define	PM_CTAG_START	(0xff500000)	/* cpu tag */
#define PM_CTAG_STEP	(0x20)		/* address step - 32 bytes per line */
#define PM_CTAG_SIZE	(0x40000)	/* 256KB space */
#define PM_CTAG_MANY	(0x2000)	/* how many entries */
#define PM_CTAG_MASK	(0x000ffffe)	/* implemented bits */
	/* CTAG ENTRY DEFINES */
#define CTAG_INVALID	(0x0000fffe)	/* pattern to invalidate ctag entry */
#define CTAG_ADDR	(0x0000fffe)	/* phy address bits 31-17 */
#define CTAG_ADDR_SHT	(16)		/* address field shift */
#define CTAG_VBIT_U	(0x00010000)	/* valid bit of upper cache line */
#define CTAG_VBIT_L	(0x00020000)	/* valid bit of lower cache line */
#define CTAG_RSV	(0x000c0000)	/* reserved bits */

/*
=============================================================================
	256KB SECONDARY CACHE DATA STORE - private, LONG WORD access only

	The secondary cache and bus snoop should be disabled before
	accessing the secondary cache tag, secondary cache data store
	and bus tag.

	When CPU does a locked operation the read portion will always get
	data from main memory, even if the address hits in the SC.
	A locked operation that misses the SC will cause a fill if the
	address is cacheable. If a locked write hits the secondary cache
	the data store will be updated.  Note that this is different
	from the on chip primary cache that a locked access is implicitly
	considered noncacheable.

	Note that if the cpu gets a bus error both the primary and the 
	secondary caches are disabled in hardware until a clear bus error
	interrupt is issued by setting the CLR_BUSERR bit in the PM_CLRINT 
	register.
=============================================================================
*/
#define	PM_CDATA_START	(0xff800000)	/* secondary cache data */
#define PM_CDATA_SIZE	(0x40000)	/* 256 KB */
#define PM_CDATA_MANY	(0x10000)	/* how many long words */
#define PM_CDATA_MASK	(0xffffffff)	/* implemented bits */

/*
=============================================================================
	32KB BUS TAG - S H A R E D, LONG WORD access only

	The secondary cache and bus snoop should be disabled before
	accessing the secondary cache tag, secondary cache data store
	and bus tag.
=============================================================================
*/
#define	PM_BTAG_START	(0xff600000)	/* bus tag */
#define PM_BTAG_STEP	(0x4)		/* address step */
#define PM_BTAG_SIZE	(0x8000)	/* 32KB */
#define PM_BTAG_MANY	(0x2000)	/* how many entries*/
#define PM_BTAG_MASK	(0xf007ffff)	/* implemented bits */
	/* BTAG ENTRY DEFINES */
#define BTAG_ADDR	(0x00007fff)	/* bus address bits 31-17 */
#define BTAG_SLOT	(0x00078000)	/* bus slot */
#define BTAG_VBIT_UA	(0x10000000)	/* valid bit of upper cacheline cpuA */
#define BTAG_VBIT_LA	(0x20000000)	/* valid bit of lower cacheline cpuA */
#define BTAG_VBIT_UB	(0x40000000)	/* valid bit of upper cacheline cpuB */
#define BTAG_VBIT_LB	(0x80000000)	/* valid bit of lower cacheline cpuB */

/*
=============================================================================
	LOCAL STATUS REGISTER - private, LONG WORD READ only

	STS_SWINT, STS_HWINT, STS_DUAL and STS_CSSERR bits are defined for
	cpu A only.
=============================================================================
*/
#define PM_STS		(0xff700000)	/* local status register */
#define	STATUS_REG	PM_STS

#define STS_SWINT	(0x0000003f)	/* cpuA only - software interrupt */
#define STS_HWINT	(0x00003f00)	/* cpuA only - hardware interrupt */
#define STS_HWINT_SFT	(0x8)		/* hardware interrupt right shift */
#define STS_DUAL	(0x00008000)	/* cpuA only - cpuB present */
#define STS_GOTIPC	(0x00010000)	/* got ipc command */ 
#define STS_NORSP	(0x00020000)	/* response time out */
#define STS_NOGRANT	(0x00040000)	/* bus grant time out */
#define STS_FRZ		(0x00080000)	/* bus freeze */
#define STS_NONRDERR	(0x00100000)	/* xmit write/response command error */
#define STS_SFIFOFULL	(0x00200000)	/* snoop fifo overflow */
#define STS_SWINT7	(0x00400000)	/* software interrupt 7 */
#define STS_MAPFAULT	(0x00800000)	/* map fault */
#define STS_WHO		(0x01000000)	/* 1=cpuA 0=cpuB */
#define STS_CSSERR	(0x02000000)	/* cpuA only */
					/* bus snoop error for the followings */
					/* wrong type,type parity,source err */
					/* dest err,data parity */
#define STS_SNOOPERR	(STS_CSSERR)
#define STS_IF		(0x04000000)	/* bus interface enabled */
#define STS_DIAGBAG	(0x08000000)	/* diagbag present */
#define STS_MYSLOT	(0xf0000000)	/* my slot ID */
#define STS_MYSLOT_SHFT	28		/* slot ID shift val */

/*
=============================================================================
	LATCHED STATUS REGISER for first - S H A R E D, LONG WORD READ only

	This register contains the state of the error bits when the first
	one happens.  It stays until all the error sources are cleared.
=============================================================================
*/
#define	PM_FST		(0xff720000)	/* first bus error register */

#define FST_NORSP_N	(0x00010000)	/* response timeout - LOW TRUE */
#define FST_NOGRANT_N	(0x00020000)	/* grant timeout - LOW TRUE */
#define FST_CSSERR_N	(0x00040000)	/* CSS bus error - LOW TRUE */
#define FST_NONRDERR_NA	(0x00080000)	/* cpuA nonread bus error - LOW TRUE */
#define FST_MF_NA	(0x00100000)	/* cpuA map fault - LOW TRUE */
#define FST_MF_NB	(0x00200000)	/* cpuB map fault - LOW TRUE */
#define FST_WRERR_NB	(0x00400000)	/* cpuB write error - LOW TRUE */

/*
=============================================================================
	BUS ERROR REGISER - S H A R E D, LONG WORD READ only

	This register contains the information for the first STS_NONRDERR
	or STS_CSSERR error.

	Issue a write to the PM_CLRINT register with CLR_BUSERR bit cleared
	to reenable the error logging function.
=============================================================================
*/
#define	PM_RCV		(0xff740000)	/* receieve bus error register */

#define RCV_NACK	(0x00000040)	/* rcv NACK - bus signal */
#define RCV_ACK_N	(0x00000080)	/* rcv ACK! - bus signal - LOW TRUE */
#define RCV_TYP		(0x00003f00)	/* rcv type - bus signal */
#define RCV_CSSERR	(0x00004000)	/* bus snoop error */
#define RCV_SNOOPERR	(RCV_CSSERR)
#define RCV_OUTDRV	(0x00008000)	/* bus output driver enbale delay 1 */
					/* set on xmitting a command */
#define RCV_DST		(0x000f0000)	/* rcv destination slot - bus signal */
#define RCV_SRC		(0x00f00000)	/* rcv source slot - bus signal */
#define RCV_WXRSP	(0x01000000)	/* wait for xmitting response */
#define RCV_WHO		(0x02000000)	/* cpu delay 1: 1=cpuA 0=cpuB */
#define	RCV_PARVALID	(0x04000000)	/* rcv data parity valid - bus signal */
#define RCV_DATERR	(0x08000000)	/* receive data parity error */
#define RCV_TYPERR	(0x10000000)	/* receive type error */
#define RCV_DSTERR	(0x20000000)	/* receive destination error */
#define RCV_SRCERR	(0x40000000)	/* receive source error */
#define RCV_ACT_N	(0x80000000)	/* rcv ACTIVE! - bus signal LOW TRUE */

/*
=============================================================================
	BUS TIME OUT REGISER - S H A R E D, LONG WORD READ only

	This register contains the information for the first STS_NOGRANT
	or STS_NORSP error.

	A non-read bus error NMI interrupt is generated for a grant time out 
	on write operations.

	A bus error interrupt is generated for a grant time out or a response
	time out on read operations.

	Bits 0-7 reflect information on the first response time out.

	Bits 16-31 reflect information on the first grant time out.

	Issue a write to the PM_CLRINT register with CLR_BUSERR bit cleared
	to reenable the error logging function.
=============================================================================
*/
#define	PM_TMOUT	(0xff760000)	/* bus time out register */

#define	TM_RSRCCMPL	(0x0000000f)	/* response source complement */
#define	TM_RSRC		(0x000000f0)	/* response source */
#define TM_WHO		(0x00000100)	/* 1=cpuA; 0=cpuB */
#define TM_XDSTCMPL	(0x000f0000)	/* xmit destination complement */
#define TM_XDST		(0x00f00000)	/* xmit destination */
#define TM_ADST		(0x0f000000)	/* arbiter destination */
#define TM_XTYP		(0x70000000)	/* xmit type bit 3-5 */
#define TM_CFIFO	(0x80000000)	/* CFIFO cpu: 1=cpuA  0=cpuB */

/*
=============================================================================
	PRIVATE CONTROL REGISTER - private, LONG WORD access only

	The PCTL_CPUB bit is for cpu A only.
=============================================================================
*/
#define	PM_PCTL		(0xff780000)	/* private control register */

#define PCTL_MBOX	(0x000000ff)	/* mail box */
#define	PCTL_BUSY	(0x00000080)	/* I am busy in mail box */
#define	PCTL_CPUB	(0x00000100)	/* cpuA only - enable cpuB */
#define	PCTL_SELFOFF	(0x00000200)	/* DISABLE snoop on self */
#define	PCTL_DECODE	(0x00000800)	/* enable address decode */
#define PCTL_SFIFO	(0x00001000)	/* enable unload snoop fifo */
#define	PCTL_SCACHE	(0x00002000)	/* enable secondary cache */
#define	PCTL_PCACHE	(0x00004000)	/* enable primary cache */
#define	PCTL_MMU	(0x00008000)	/* was enable mmu -- now defunct! */

/*
=============================================================================
	CLEAR INTERRUPT REGISTER - private, LONG WORD access only

	Only cpu A has the CLR_CSSERR feature.
=============================================================================
*/
#define PM_CLRINT	(0xff7a0000)	/* clear interrupt register */

#define CLR_SW		(0x00000007)	/* clear software interrupts */
#define CLR_HW		(0x00000038)	/* clear hardware interrupts */
#define CLR_NMI		(0x00001f00)	/* clear NMI interrupts */
#define CLR_SW1		(0x00000001)	/* clear software interrupt 1 */
#define CLR_SW2 	(0x00000002)	/* clear software interrupt 2 */
#define CLR_SW3 	(0x00000003)	/* clear software interrupt 3 */
#define CLR_SW4 	(0x00000004)	/* clear software interrupt 4 */
#define CLR_SW5 	(0x00000005)	/* clear software interrupt 5 */
#define CLR_SW6 	(0x00000006)	/* clear software interrupt 6 */
#define CLR_SW7 	(0x00000007)	/* clear software interrupt 7 */
#define CLR_HW1		(0x00000008)	/* cpuA only - clear hardware int 1 */
#define CLR_HW2		(0x00000010)	/* cpuA only - clear hardware int 2 */
#define CLR_HW3		(0x00000018)	/* cpuA only - clear hardware int 3 */
#define CLR_HW4		(0x00000020)	/* cpuA only - clear hardware int 4 */
#define CLR_HW5		(0x00000028)	/* cpuA only - clear hardware int 5 */
#define CLR_HW6		(0x00000030)	/* cpuA only - clear hardware int 6 */
#define CLR_BUSERR	(0x00000040)	/* clear bus error interrupt */
#define CLR_NONRD	(0x00000100)	/* clear non read NMI */
#define CLR_CSSERR	(0x00000200)	/* cpuA only - clear snoop error NMI */
#define CLR_SNOOPERR	(CLR_CSSERR)
#define CLR_SFIFO	(0x00000300)	/* clear invalidator fifo ovrflow NMI */
#define CLR_IPC		(0x00000400)	/* clear ipc written NMI */
#define CLR_FRZ		(0x00000500)	/* clear bus freeze NMI */
#define CLR_MAP		(0x00000800)	/* clear invalid map entry NMI */
#define CLR_CACHE	(0x00001000)	/* clear cache inhibit bit that set */
					/* by a cpu bus error*/
					
/*
=============================================================================
	SHARED CONTROL REGISTER - shared, LONG WORD access only
=============================================================================
*/
#define	PM_SCTL		(0xff7c0000)	/* shared control register */

#define SCTL_LED_RDY_FLT (0x00000001)	/* turn on LED RDY; off LED FAULT */
#define SCTL_LED_SFT1	(0x00000002)	/* turn on LED SFT1 */
#define SCTL_LED_SFT2	(0x00000004)	/* turn on LED SFT2 */
#define SCTL_LED_SFT3	(0x00000008)	/* turn on LED SFT3 */
#define SCTL_FULL0	(0x00000010)	/* command fifo full 0 - READ only */
#define SCTL_FULL1	(0x00000020)	/* command fifo full 1 - READ only */
#define SCTL_IPCOFF	(0x00000080)	/* disable ipc function */
#define	SCTL_RFIFOTEST	(0x00000100)	/* enable response fifo test mode */
					/* always used with SCTL_LPBK */
#define	SCTL_IPCTEST	(0x00000200)	/* enable ipc test mode */
					/* always used with SCTL_LPBK */
#define	SCTL_LPBK	(0x00000400)	/* enable bus loop back */
					/* used together with SCTL_RFIFOTEST */
					/* or SCTL_IPCTEST */
#define	SCTL_CFIFO	(0x00000800)	/* enable unload command fifo */
#define SCTL_RDYOFF	(0x00001000)	/* disable arbiter ready */
#define	SCTL_BTAG	(0x00002000)	/* enable bus tag snooping */
#define	SCTL_BYPASS	(0x00004000)	/* enable command bypass */

/*
=============================================================================
	BUS FRTEEZE INTERRUPT REQUEST PORT - LONG WORD WRITE only
=============================================================================
*/
#define PM_FRZ		(0xff7e0000)	/* bus freeze interrupt request port */

/*
=============================================================================
	EXTERNAL CSS BUS STATUS REGISTER, LONG WORD READ only
=============================================================================
*/
#define PM_ST		(0xfffffffc)	/* PM status register */

#define ST_MBOXB	(0x00ff0000)	/* cpuB mail box */
#define ST_MBOXA	(0x0000ff00)	/* cpuA mail box */
#define ST_PMID		(0x000000ff)	/* module ID: single=0x22 dual=0x26 */
#define ST_DUAL		(0x00000026)	/* ID for dual processors board */
#define ST_SINGLE	(0x00000022)	/* ID for single processor board */
#define ST_MYID		(ST_PMID)

/*
=============================================================================
	EXTERNAL INTERRUPT REQUEST REGISTER, LONG WORD WRITE only

	An interrupt issued to cpu B is NAK'ed if cpu B is not present.
	All hardware interrupts are directed to cpu A.
	Hardware interrupts 1-4 are non-directed, 5-6 are directed.
=============================================================================
*/
#define PM_INTR		(0xfffffffc)	/* PM interrupt request register */
#define PM_INT		(PM_INTR)	/* PM interrupt request register */

#define INT_ID		(0x20000000)	/* int ID: 1=cpuA; 0=cpuB */
#define INT_WHICH	(INT_ID)
#define INT_TYP		(0x10000000)	/* int TYPE: 1=HW; 0=SW */
#define INT_CTL		(0x08000000)	/* int CTL: 1=set; 0=nop */
#define INT_LVL		(0x07000000)	/* int LEVEL 1-7 */

/*
=============================================================================
	EXTERNAL IPC REQUEST PORTS - LONG WORD WRITE only (see ipcc.h)

	The IPC request ports will not response (NAK'ed) if the SCTL_IPCOFF 
	bit in the PM_SCTL register is set.
=============================================================================
*/
#define PM_IPCA		(0x00000080)	/* cpuA interprocessor request port */
#define PM_IPCB		(0x00000084)	/* cpuB interprocessor request port */


/*
=============================================================================
       Twinkle the Idle Light on the Front of the Board Macros
=============================================================================
*/
#define dpm40_which_led() \
	((own.o_pm_id == own.o_base_pm_id) ? SCTL_LED_SFT1 : SCTL_LED_SFT2)
#define idle_led_on()		(atom_or(PM_SCTL, dpm40_which_led()))
#define idle_led_off()		(atom_and(PM_SCTL, ~dpm40_which_led()))
#define streams_led_on()	(atom_or(PM_SCTL, SCTL_LED_SFT3))
#define streams_led_off()	(atom_and(PM_SCTL, ~SCTL_LED_SFT3))

/* other machine-independent defines */
#define get_cpu_mask_type()	 /* currently only one 040 mask type */

#endif	/* SYS_M68040_MLIO_H */
