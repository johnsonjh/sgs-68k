#ifndef SYS_GCP_CIO_H
#define SYS_GCP_CIO_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) cio.h: version 2.1 created on 5/22/89 at 19:06:06	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)cio.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/* @(#)head/gcp:cio.h	1.5 */
/*******************************************************************
 *
 *			C   I   O
 *
 *******************************************************************/

struct cio_reg {
#ifdef GC16
	unsigned char reg;
	unsigned char filler;
#else
	unsigned char filler;
	unsigned char reg;
#endif
};

struct cio {
	struct cio_reg mic;		/* master interrupt control */
	struct cio_reg mcc;		/* master configuration control */
	struct cio_reg pivect[2];	/* parallel port interrupt vectors */
	struct cio_reg ctivect;		/* counter timer interrupt vector */
	struct cio_reg pcdpp;		/* port C data path polarity */
	struct cio_reg pcdd;		/* port C data direction */
	struct cio_reg pcsc;		/* port C special control */
	struct cio_reg portcs[2];	/* port command status */
	struct cio_reg ctcs[3];		/* counter timer command status */
	struct cio_reg portdata[3];	/* port data */
	struct ct_cc {
		struct cio_reg ccmsb;	/* current count MSB */
		struct cio_reg cclsb;	/* current count LSB */
	} ctcc[3];
	struct ct_tc {
		struct cio_reg tcmsb;	/* terminal count MSB */
		struct cio_reg tclsb;	/* terminal count LSB */
	} cttc[3];
	struct cio_reg ctmode[3];	/* counter timer mode specification */
	struct cio_reg curvect;		/* current interrupt vector */
	struct cio_port {
		struct cio_reg pmode;	/* port mode specification */
		struct cio_reg phndshk;	/* port handshake specification */
		struct cio_reg dppolar;	/* data path polarity */
		struct cio_reg dpdir;	/* data path direction */
		struct cio_reg spcntrl;	/* special i/o control */
		struct cio_reg patpolar;/* pattern polarity */
		struct cio_reg pattrans;/* pattern transition */
		struct cio_reg patmask;	/* pattern mask */
	} cioport[2];
};

/******************************************************************************
 *
 * mic --- master interrupt control register (r/w)
 *
 ******************************************************************************/
#define C_RESET    0x01  /* reset the cio chip */
#define C_RJUST    0x02  /* right justified addresses */
#define C_CTVIS    0x04  /* counter timer vector includes status */
#define C_PBVIS    0x08  /* port B vector includes status */
#define C_PAVIS    0x10  /* port A vector includes status */
#define C_NVECT    0x20  /* no vector */
#define C_DLC      0x40  /* disable lower chain */
#define C_MIE      0x80  /* master interrupt enable */

/******************************************************************************
 *
 * mcc --- master configuration control register (r/w)
 *
 ******************************************************************************/
#define C_DISALL   0x00  /* disable all */
#define C_LCMSK    0x03  /* link control mask */
#define C_LCCTI    0x00  /* link control counter timers independant */
#define C_LC1G2    0x01  /* link control counter ct1 out gates ct2 */
#define C_LC1T2    0x02  /* link control counter ct1 out triggers ct2 */
#define C_LC1I2    0x03  /* link control counter ct1 out is ct2 count input */
#define C_PAE      0x04  /* port A enable */
#define C_PLC      0x08  /* port link control */
#define C_PCCT3E   0x10  /* port C and counter timer 3 enable */
#define C_CT2E     0x20  /* counter timer 2 enable */
#define C_CT1E     0x40  /* counter timer 1 enable */
#define C_PBE      0x80  /* port B enable */

/******************************************************************************
 *
 * portcs --- port command status registers (read/partial write)
 *
 ******************************************************************************/
#define P_IOE      0x01  /* interrupt on error */
#define P_PMF      0x02  /* pattern match flag (read only) */
#define P_IRF      0x04  /* input register full (read only) */
#define P_ORE      0x08  /* output register empty (read only) */
#define P_IERR     0x10  /* interrupt error (read only) */
#define P_ICMSK    0xe0  /* interrupt control mask (see interrupt control) */

/******************************************************************************
 *
 * ctcs --- counter timer command status registers (read/partial write)
 *
 ******************************************************************************/
#define CT_CIP     0x01  /* count in progress */
#define CT_TCB     0x02  /* trigger command bit (write only/read 0) */
#define CT_GCB     0x04  /* gate command bit */
#define CT_RCC     0x08  /* read counter control */
#define CT_IERR    0x10  /* interrupt error (read only) */
#define CT_ICMSK   0xe0  /* interrupt control mask (see interrupt control) */

/******************************************************************************
 *
 * ctmode --- counter timer mode specification registers (read/write)
 *
 ******************************************************************************/
#define CT_ODCMSK  0x03  /* output duty cycle */
#define CT_PULSE   0x00  /* pulse output */
#define CT_1SHOT   0x01  /* one-shot output */
#define CT_SQWAV   0x02  /* square wave output */
#define CT_NSPEC   0x03  /* do not specify output */
#define CT_REB     0x04  /* retrigger enable bit */
#define CT_EGE     0x08  /* external gate enable bit */
#define CT_ETE     0x10  /* external trigger enable bit */
#define CT_ECE     0x20  /* external count enable bit */
#define CT_EOE     0x40  /* external output enable bit */
#define CT_CONT    0x80  /* continuous/single cycle* operation */

/******************************************************************************
 *
 * pmode --- port mode specification registers (read/write)
 *
 ******************************************************************************/
#define P_LPM      0x01  /* latch on pattern match */
#define P_PMMSK    0x06  /* pattern mode mask */
#define P_DISPM    0x00  /* disable pattern match */
#define P_ANDMD    0x02  /* and mode */
#define P_ORMD     0x04  /* or mode */
#define P_OPEVM    0x06  /* or-priority encoded vector mode */
#define P_IMO      0x08  /* interrupt on match only */
#define P_SBM      0x10  /* single buffered mode */
#define P_ITB      0x20  /* interrupt on two bytes */
#define P_TYPSEL   0xc0  /* port type select */
#define P_BIT      0x00  /* bit port */
#define P_INPUT    0x40  /* input port */
#define P_OUTPUT   0x80  /* output port */
#define P_BIDIR    0xc0  /* bidirectional port */

/******************************************************************************
 *
 * phndshk --- port handshake specification registers (read/write)
 *
 ******************************************************************************/
#define P_DTSMSK   0x07  /* deskew time specification mask */
#define P_RWSMSK   0x38  /* request/wait* specification mask */
#define P_DISRW    0x00  /* disable request/wait */
#define P_OUTW     0x08  /* output wait */
#define P_INPW     0x18  /* input wait */
#define P_SPRQ     0x20  /* special request */
#define P_OUTRQ    0x28  /* output request */
#define P_INPRQ    0x38  /* input request */
#define P_HTS      0xc0  /* handshake type specification */
#define P_HINTL    0x00  /* interlocked handshake */
#define P_HSTROBD  0x40  /* strobed handshake */
#define P_HPULSED  0x80  /* pulsed handshake */
#define P_H3WIRE   0xc0  /* three wire handshake */

/******************************************************************************
 *
 * interrupt control (read and write)
 *
 ******************************************************************************/
/* read */
#define ICR_IP     0x20  /* interrupt pending */
#define ICR_IED    0x40  /* interrupt enabled */
#define ICR_IUS    0x80  /* interrupt under service */
/* write */
#define ICW_NULL   0x00  /* null code */
#define ICW_CPUS   0x20  /* clear interrupt pending and int under service */
#define ICW_SIUS   0x40  /* set interrupt under service */
#define ICW_CIUS   0x60  /* clear interrupt under service */
#define ICW_SIP    0x80  /* set interrupt pending */
#define ICW_CIP    0xa0  /* clear interrupt pending */
#define ICW_SIE    0xc0  /* set interrupt enable */
#define ICW_CIE    0xe0  /* clear interrupt enable */

#endif /* SYS_GCP_CIO_H */
