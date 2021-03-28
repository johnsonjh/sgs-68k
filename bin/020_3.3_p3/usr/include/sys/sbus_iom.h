#ifndef SYS_SBUS_IOM_H
#define SYS_SBUS_IOM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sbus_iom.h: version 24.1 created on 10/28/91 at 18:37:32	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sbus_iom.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * sbus_iom.h -- IOM defines
 */


/* adapter board type codes */
#define	IOM_TYPE_IOA		0x00
#define	IOM_TYPE_IOSBA		0x07
#define	IOM_TYPE_NO_BOARD	0xff

#define EXTENSION_BD_ID_MASK	0x07
/* FIX joe - .....ff7 */
#define EXTENSION_BD_ID_ADDR	0xffffeff4	/* addr of extension board 
						 * ID reg - [2:0] = 7 then IOSBA
						 * 	    [2:0] = 0 then IOA
						 */

/*
 * A IOSBA maps each slot on the expansion bus into 0x10000000 bytes, where
 * address 0x41234567 on the IOM/IOSBA becomes address 0xf1234567 on subslot 4.
 * Slot 0xF in the expansion bus, and hence addresses 0xfxxxxxxx on the IOM
 * in the main bus, are reserved for IOM and IOSBA.
 */
#define IOSBA_SubSlotShift	28		/* SubSlot # is high nibble */
#define IOSBA_SubSlotMask	0xf0000000	/* see above */
#define IOSBA_OffsetMask	0x0fffffff	/* board offset mask */

#endif /* SYS_SBUS_IOM_H */
