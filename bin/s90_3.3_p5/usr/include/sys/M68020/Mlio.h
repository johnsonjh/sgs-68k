#ifndef SYS_M68020_MLIO_H
#define SYS_M68020_MLIO_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Mlio.h: version 24.1 created on 10/28/91 at 18:30:08	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Mlio.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *	lio.h
 *	Kernel's IO space 0xffe00000 through 0xffefffff
 *	(the optional Diag Bag 0xfff00000-0xffffffff is described in bagio.h)
 *	(Addresses not defined here may give unpredictable results.)
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* * * * * * * ** * * * * * * *
 *
 *	Storage test locations and quirks
 *
 * * * * * * * ** * * * * * * */

/*
 *
 *
 *		Cache as RAM.  One image of the data store is readable
 *			and writable in all sizes and alignments by
 *			the supervisor.
 */

#define SRAM_START ((unsigned char *)0xffe00000)
#define SRAM_SIZE 65536

/*
 *
 *		CPU's tag registers.  The 4096 registers
 *			read and write only as aligned long words.
 *			Part of the word is unimplemented.
 *			Four images of each register appear at four
 *			consecutive long-word locations.
 */

#define CTAG_START ((unsigned *)0xffe10000)
#define CTAG_MANY  4096
#define CTAG_GOOD  0xffffff80	/* & mask for the implemented bits	*/
#define CTAG_WASTE 0x0000007f	/* | mask covering the waste bits	*/
#define CTAG_VBIT  0x00000080	/* & mask for the "entry is valid" bit	*/
#define CTAG_CLEAR 0x00000080	/* write this value to invalidate an entry */

/*
 *
 *		Bus's tag registers.  Four images of the 4096 registers
 *			read and write only as aligned long words.
 *			Part of the word is unimplemented.
 */

#define BTAG_START ((unsigned *)0xffe20000)
#define BTAG_MANY  4096
#define BTAG_GOOD  0xfffffff8	/* & mask for the implemented bits	*/
#define BTAG_PHYS  0xffffff00	/* & mask for the phys addr 35:12 bits	*/
#define BTAG_LOGS  0x000000f0	/* & mask for the log. addr 15:12 bits	*/
#define BTAG_VBIT  0x00000008	/* & mask for the "entry is valid" bit	*/
#define BTAG_CLEAR 0x00000000	/* write this value to invalidate an entry */
#define BTAG_WASTE 0x00000007	/* | mask covering the waste bits	*/
    /*
     * The BTAG_VBIT will be cleared at the same time as CTAG_VBIT,
     * as bus hits and cache fills dictate.
     */

/*
 *
 *		TLB's tag registers.  Four images of the 1024 registers
 *			read and write only as aligned long words.
 */

#define MTAG_START      ((unsigned *)0xffe30000)
#define MTAG_MANY       1024
#define MTAG_4K         4096
#define MTAG_GOOD       0xfff00000   /* & mask for the implemented bits */
#define MTAG_VBIT       0x00100000   /* & mask for the "entry is valid" bit */
#define MTAG_LOG_ADDR   (MTAG_GOOD & (~MTAG_VBIT))
				/* & mask for logical addr bits 31-21 */
#define MTAG_NOCLEAR    0x3fc00000   /* & mask for the regular bits     */
#define MTAG_CLEARABLE  0xe0100000   /* & mask for the clearable bits   */
#define MTAG_WASTE      0x000fffff   /* | mask covering the waste bits */
    /*
     *		This long-write only location clears all 1024 MTAG_VBITs
     *		in one poke, two bits representing LA23 and LA22.
     */
#define MTAG_FLUSH	((unsigned *)0xffe63000)
#define FLUSH_TLB	MTAG_FLUSH

/*
 *
 *		Translation Lookaside Buffer (TLB).
 *			These 1024 registers
 *			read and write only as aligned long words.
 *			The MMU keeps page descriptors here.
 */

#define TLB_START ((unsigned *)0xffe40000)
#define TLB_MANY  1024
    /*
     * Miscellaneous things in the remaining 3072 registers:
     */
#define TLB_TOTAL 4096
#define UROOT     ((unsigned *)0xffe41000)	/* User's root pointer    */
#define KROOT     ((unsigned *)0xffe41004)	/* Kernel's root pointer  */
#define ROOTMASK  0xffffff800	/* & mask for useful bits in a root ptr	*/

/*
 *	A second image of the MMU store appears.
 *	Writes to this image (which must of course be long-aligned)
 *	have the side effect of discarding all 1024 cached descriptors
 *	by clearing all the valid-bits in the TLB's tag.
 *	By writing URXXT, a new user map is selected in main memory.
 */

#define URXXT ((unsigned *)0xffe61000)	/* User's root pointer: flushes TLB */
#define KRXXT ((unsigned *)0xffe61004)	/* Kernel's root pointer: flushes TLB */

/*		I/O Map.  The map is used by the kernel to defeat
 *	translation caching, data caching and write buffering.
 *	Each descriptor represents a 1MB
 *	section of the 64GB physical space.
 *	The mapped i/o space is in the second 1GB of kernel's space.
 */

#define IOMAP_BASE		((unsigned *)0xffe42000)
#define PM_IOMAP_BASE  		IOMAP_BASE
#define IOMAP_MANY		1024
#define IOMAP_SECTION_SIZE	0x00100000
#define IOMAP_VALID		0x00000040
#define USEABLE_IOMAPS		1016		/* local i/o space overlaps
						 * with the last 8 locations
						 */
    /*
     * The last eight slots in the io map are not used because
     * the local io uses the last 8 MB of io space.
     */
/*
 * --> end of the 5 RAM blocks !
 */

/* * * * * * * * * * * * * * *
 *
 *	Cache control register
 *
 * * * * * * * * * * * * * * */

/* write only, read is ignored */
#define CACHE_CTL_REG    ((unsigned char *)0xffe5b000)
#define CACHE_CTL_ON     0xff
#define CACHE_CTL_OFF    0x00
#define CACHE_CTL_FILL   0x80
#define CACHE_CTL_HIT    0x40
#define CACHE_CTL_SPYIN  0x20
#define CACHE_CTL_SPYOUT 0x10
#define CACHE_CTL_WABON  0x04
    /*
     * bits:
     *	31	30	29	28	27	26	25	24
     *	fillenb	hitenb	spyin	spyout		wab
     */

/* * * * * * * * * * * * * * *
 *
 *	Bus mail byte
 *
 * * * * * * * * * * * * * * */

/* write byte only; read destroys */
#define BUS_MAIL_BYTE	((unsigned char *)0xffe5b800)
    /* reads back on the CSS Bus */

/* * * * * * * * * * * * * * *
 *
 *	General status register
 *
 *
 * * * * * * * * * * * * * * */

/* read long only; writes ignored    */
#define STATUS_REG     ((unsigned *)0xffe52000)
#define ST_IMASK       0xfff8ffff	/* Implemented status bits	     */
#define ST_MASK        0x01701f7e	/* Interesting status bits	     */
#define ST_SLOT_MASK   0xf0000000	/* What slot are we?		     */
#define ST_FPU_TOUCHED 0x08000000	/* Somebody selected it		     */
#define ST_XMT_READY   0x04000000	/* WAB empty			     */
#define ST_BAG_DTECT   0x02000000	/* 0 = Diag Bag is attached	     */
#define ST_CMD_PENDG   0x01000000	/* Your NMI is from command rcvd     */
#define ST_FIFO_BUSY   0x00800000	/* The spy's FIFO is not empty	     */
#define ST_IPL_REG     0x00700000
				/* Your IPL input pins: 111=nothing pending  */
#define ST_SLOT_BUSY   0x00080000	/* This slot is busy		     */
#define ST_BAD_IO      0x00008000	/* Attempted I/O but valid was off   */
#define ST_TIMEOUT     0x00004000	/* Timeout			     */
#define ST_EXECUTE     0x00002000	/* Executing when cycle busted	     */
#define ST_WRONG_SPACE 0x00001000	/* Tried to access wrong space:
		moves to undefined function code, or user's pointer > 2GB     */
#define ST_WRONG_COP   0x00000800	/* Tried to access wrong coprocessor:
		an f-line instruction with (cpid != 2) */
#define ST_X_PERM      0x00000400	/* State of the X_PERMIT bit	     */
#define ST_R_PERM      0x00000200	/* State of the R_PERMIT bit	     */
#define ST_W_PERM      0x00000100	/* State of the W_PERMIT bit	     */
#define ST_ARB_REQ     0x00000080	/* waiting for CSS grant	*/
#define ST_BERR        0x00000040	/* bus error since CLR_FAULTS	*/
#define ST_PAGE_FAULT  0x00000020	/* Page fault			     */
#define ST_SINCE       0x00000010	/* This is not the first bus error   */
					/*  since CLR_FAULTS		     */
#define ST_NAK         0x00000008	/* NAK received or bad parity	     */
#define ST_WALK_ERR    0x00000004	/* table walk failed		*/
#define ST_BAD_DATA    0x00000002	/* Bad data recvd		     */
#define ST_VIOLATION   0x00000001	/* The cycle had insufficient permits */

#define ST_ACTIVE_LOW	(ST_BAD_IO | ST_WRONG_SPACE | ST_WRONG_COP | \
			 ST_IPL_REG | ST_BAG_DTECT)

/* ST_FPU_TOUCHED, ST_BERR, ST_SINCE,
 ST_WALK_ERR, ST_NAK, and ST_BAD_DATA
 are cleared by writing (data ignored, aligned-long) to CLR_FAULTS.
 These bits set when the appropriate condition exists.
 The delayed bus error signal (due to instruction prefetch
 or move multiple from faulty memory locations) sets them,
 as well as the bus error exception taken.
 This fault-accumulation feature is intended to allow the operating system to
 detect bus error signals to user programs which are delayed or not taken:
 the "unused prefetch across page boundary" phenomenon.

 ST_BAD_IO, ST_TIMEOUT, ST_EXECUTE, ST_WRONG_SPACE, ST_WRONG_COP,
 ST_X_PERM, ST_RPERM, and ST_W_PERM reflect the conditions of
 the most recent bus error signal, whether the exception was taken or delayed.
 Conditions of earlier bus errors are lost.

 ST_BAD_IO, ST_WRONG_SPACE, ST_WRONG_COP, ST_IPL_REG, and ST_BAG_DTECT
 are low-true, that is, zero indicates the true sense and one
 indicates the false sense of these bits.

 ST_WRONG_COP may indicate a hardware fault, because the machine instruction
 0xffff sets it.  This value can indicate incorrect hardware operation,
 because it's the "TTL float" nothing-driving-the-bus level.

 ST_WALK_ERR indicates table walk in progress when the bus error occurred.
 ST_TIMEOUT, ST_NAK and ST_BAD_DATA apply.  If a pointer table entry was
 acquired correctly before the fault occurred, it will appear in the TLB
 location used by the faulted address (unless the fault handler also
 uses that location).

 ST_BAD_DATA is meaningless if ST_TIMEOUT because you can't time out
 once BAD DATA arrives, and if you time out you aren't waiting for data
 any more.  If BAD DATA arrives in the same cpu's clock period
 as TIMEOUT, then pigs can fly and hell is freezing over.
 */



/* * * *
 *
 *	Receive funnel registers
 *	(readable any size, writes are ignored)
 *
 *		The receive funnel unit has 4 64-bit registers.
 *
 *	The first, RCVN1-RCVN2, captures each READ DATA RESPONSE (RDR)
 *	(defined in the css bus spec) out of the bus receiver
 *	and holds it while the cpu, cache, or mmu uses it.
 *
 *	Each time a new RDR arrives, the old one moves into
 *	RCVN3-RCVN4.  So the receive funnel retains the last 16 bytes
 *	received as RDRs from memory at all times.
 *	
 *	The third 64 bit register, RCV_WCMD-RCV_CMD, captures
 *	READ COMMANDs and WRITE COMMANDs (see css bus spec),
 *	which any module may send into this module's command register.
 *	The DATA part of a WRITE COMMAND appears in RCV_WCMD.
 *	The ADDRESS part of either COMMAND appears in RCV_CMD.
 *	(The ADDRESS part is the slot-offset bits 31-0.
 *	The command's originating slot number is NOT CAPTURED.)
 *
 *	When a COMMAND arrives, it generates a non-maskable interrupt,
 *	and no more commands of any kind can be accepted.
 *	Software must write INC_READY to re-enable commands.
 *
 *	The command register will not work if loopback mode is on.
 *	(see RCV_LOOP_ENB.)
 *	In this mode, RCV_WCMD-RCV_CMD captures this module's
 *	every bus transmission: RDRs and COMMANDs.
 *	
 *	Bytes ordering in these longs works
 *	in the usual Motorola big-endian way. 
 *	
 * * * */

#define RCV_BUF   0xffe5c000	/*  Receive funnel registers  */

#define RCVN1     0xffe5c008	/* this long word receives normal memory
				 * RDRs from even long-words
				 */
#define RCVN2     0xffe5c00c	/* this long word receives normal memory
				 * RDRs from odd long-words
				 */
#define RCVN3     0xffe5c000	/* used for fill data bytes 0-3	*/
#define RCVN4     0xffe5c004	/* used for fill data bytes 4-7	*/

#define RCV_WCMD  0xffe5c010	/* hold 4 data bytes of a write command
				 * (causes NMI when filled from CSS Bus)
				 */
#define RCV_CMD   0xffe5c014	/* hold 4 address bytes of a command
				 * read or write
				 */
#define RCV_LOOPD RCV_WCMD	/* hold 4 bytes data from our transmitter
				 * if test mode is on
				 */
#define RCV_LOOPA RCV_CMD	/* hold 4 bytes address from our transmitter
				 * if test mode is on
				 */

#define RCV2	  0xffe5c008	/*  historical baggage define  */

/* * * * * *
 *
 *	Transmit Buffers (aka WAB, write latch)
 *		These latches shine through unless locked by a buffered
 *		write or by these test locations.
 *		They unlock when the transmitter has emptied on the CSS bus.
 *
 * * * * * */

#define WAB_DATA  0xffe50000	/* write data here; size ignored	*/
#define WAB_TYPE  0xffe50004	/* wrt moves SEND_REG into transmitter	*/
#define WAB_ADDR  0xffe50008	/* wrt to freeze addr; useful??		*/
#define WAB_CLEAR 0xffe5000c	/* wrt to unlock all three		*/
    /*
     * At this writing, the TLB control is not smart enough to guarantee an
     * address value when you write WAB_ADDR
     */

/* * * * * *
 *
 *	Transmit Parity Polarity Register
 *
 * * * * * */

/* write one byte		*/
#define CSS_PARITY ((unsigned char *)0xffe54000)

    /*
     * Each bit controls the parity sent in with that byte of the 
     * CSS Bus Transmission Data.
     * 1= send even (normal)	0= send odd (go for a NAK)
     *		There is NO power-up clear.
     */

/* * * * * *
 *
 *	Send Register
 *
 *	Defines a Transmission Type to be transmitted by SEND_BUTTON
 *
 * * * * * */

#define SEND_REG 0xffe58000	/* write one byte; bits 7,6 ignored	*/

/* * * * * *
 *
 *	Send Button
 *
 *	Transmits
 *	whatever is in WAB_DATA and WAB_ADDR and WAB_TYPE
 *
 * * * * * */

#define SEND_BUTTON 0xffe50018	/* write one byte; data ignored	*/

/* * * * * *
 *
 *	Clear NMI
 *
 *	turns off (re-arms) the command-buffer-filled interrupt request
 *
 * * * * * */

/* write one byte; data ignored	*/
#define CLR_NMI ((unsigned char *)0xffe5b400)

/* * * * * *
 *
 *	Clear FPU Touched
 *
 *	Clears the FPU has been touched bit
 *
 * * * * * */

/* write one byte; data ignored	*/
#define CLR_FPUSED ((unsigned char *)0xffe70000)
				/* Same as CLR_FAULTS */

/* * * * * *
 *
 *	Clear FPU
 *
 *	Causes a hardware reset to the 68881
 *
 * * * * * */

/* write one byte; data ignored	*/
#define CLR_FPU ((unsigned char *)0xffe5a400)

/* * * * * *
 *
 *	Yank FREEZE
 *
 *	Write this location to pulse the FREEZE line on the backplane.
 *	This pulse causes all processors to get a non-maskable interrupt,
 *	even the processor which put out the pulse.
 *	(The FREEZE line was formerly known as BUS.PWR.FAIL.WRN*.)
 *
 * * * * * */

#define YANK_FREEZE ((unsigned char *)0xffe5a000)


/* * * * * *
 *
 *	Logic analyzer
 *
 *	Otherwise ignored, write only, pulses a pin for test equipment
 *
 * * * * * */

#define LOGIC_ANALYZER ((unsigned char *)0xffe5a800)


/* * * * * *
 *
 *	Clear Faults
 *
 *	Clears accumulating bits in the fault register
 *
 * * * * * */

/* write one byte; data ignored	*/
#define CLR_FAULTS ((unsigned char *)0xffe70000)

/* * * * * *
 *
 *	Clear FIFO so L.A.15-12 will go from Btag to Ctag correctly.
 *		also renders ST_FIFO_ meaningful
 *
 * * * * * */

/* write one byte; data ignored	*/
#define CLR_FIFO ((unsigned char *)0xffe5ac00)

/* * * * * *
 *
 *	Increment Arbiter's ready count for our slot
 *
 * * * * * */

#define INC_READY 0xffe56000


/* * * * * *
 *
 *	Stretch
 *
 *	Clock stretch test location
 * * * * * */

#define STRETCH CLR_FIFO	/* write one byte; data ignored	*/

/* * * * * *
 *
 *	Bus Mail Register (visible to other modules)
 *
 *	One byte in the outside status word
 * * * * * */

#define BM_REG 0xffe5b800	/* write one byte	*/

/* * * * * *
 *
 *	Addressable latch
 *
 *	eight bits at eight addresses
 *		(see also prom.h)
 *	These are cleared by BUS.RESET (power-on) or Diag Bag RESET button.
 *
 * * * * * */

/* spare latch		0xffe5bc00	*/

#define PERMIT_FLT_ENBL ((unsigned char *)0xffe5bc80) /* 0 = ignore permission
							bits in Page Descriptors
							1 = Enforce permits  */

#define RCV_LOOP_BACK	0xffe5bd00	/*	0 = Command Register Enable
						1 = Loopback test	*/

#define FIFO_T_ENBL	0xffe5be00	/* I don't know what it does yet */


/* the label says "RDY", silkscreen says "LED2" */
#define LEDX		((unsigned char *)0xffe5be80)
#define GREEN_LED	LEDX

/* the label says "LED0", silkscreen says "LED3" */
#define LEDY		((unsigned char *)0xffe5bf00)
#define RED_LED		LEDY
#define LED0		LEDY

/* the label says "LED1", silkscreen says "LED6" */
#define LEDZ		((unsigned char *)0xffe5bd80)
#define RED_LED_B	LEDZ
#define LED1		LEDZ

#define PM_LED_ON	0
#define PM_LED_OFF	1

/* Twinkle the Idle Light on the Front of the Board Macros */
#define idle_led_on()		(*LED1 = PM_LED_ON)
#define idle_led_off()		(*LED1 = PM_LED_OFF)
#define streams_led_on()	(*LED0 = PM_LED_ON)
#define streams_led_off()	(*LED0 = PM_LED_OFF)

/* other machine-independent defines */
#define flush_all_own_tlb()	(*FLUSH_TLB = 0)
#define flush_all_user_tlb()	(*FLUSH_TLB = 0)
#define get_cpu_mask_type()	probe_byte(ADDR_OWN_BAD) /* cause bus err */
/*** #define	flush_sys_tlb_entry(X)	(*FLUSH_TLB = 0)	***/
/* Note: in os/swtch.c the line was "*U_TLB_FLUSH = ~MTAG_VBIT;" not "= 0;" */
#define	flush_sys_tlb_entry(X)	(*(uint *)(((((X) & MTAG_INDEX_MASK) | \
			TLB_SYS_USER_SPLIT_BIT) >> LOG_TO_TLB_OFFSET_SHIFT) | \
			(uint)MTAG_START) = 0)

#endif /* SYS_M68020_MLIO_H */
