#ifndef	SYS_SBUS_H
#define	SYS_SBUS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sbus.h: version 24.1 created on 10/28/91 at 18:37:29	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sbus.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 * sbus.h -- system bus defines
 */

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#define	SBUS_SLOT_BITS		4	/* number of bits to describe slot */
#define SBUS_OFFSET_BITS	32	/* bits behind each slot */
#define	SBUS_NUM_SLOT		16	/* number of slots in main bus */
#define	SBUS_MAX_NUM_PM		16	/* the most pm's supported per machine*/

#define	SBUS_ADDR_ID	0xFFFFFFFC	/* long read yields ID or timeout */
#define	SBUS_ID_MASK	0xFF		/* mask to yield only ID bits */

/* valid board ID values */
#define	SBUS_SPM	0	/* service processor module */
#define	SBUS_MM		1	/* memory module */
#define	SBUS_MM2	0x11	/* memory module type 2 */
#define	SBUS_PM20	2	/* processor module 68020 */
#define	SBUS_DPM40_SGL	0x22	/* duo processor module 68040. one pm onboard */
#define	SBUS_DPM40_DBL	0x26	/* duo processor module 68040. two pm's */
#define	SBUS_IOM	3	/* i/o module */
#define SBUS_VAM	5	/* VME Adatper Module */
#define	SBUS_NIOM	0x13	/* new i/o module */
#define SBUS_IOPM	0x84	/* IO Processing Module */
#define	SBUS_NO_BOARD	0xFF	/* reserved for no board present in slot */

/*
 * Note that the bottom three bits of a board ID reveals its type:
 *	SPM 0, MEM 1, CPU 2, IOM 3, IOP 4, VAM 5
 */
    /* cpu type processor modules always "end in '2'" */
#define	SBUS_IS_PM(bd_id)	(((bd_id) & 0x3) == 0x2)

/* FIX MSS, in the long run this next line should be removed */
#define	Sbus_Num_Id	ERROR	/* This define no longer works */

#ifndef SYS_SBUS_IOM_H
#define NUM_IOM_PER_SYS		4
#endif

typedef	struct	sbus	{
	uint	sbus_slot;
	uint	sbus_offset;
} sbus_t;

sbus_t	km_to_sbus();		/* convert kernel memory to system bus */
sbus_t	kv_to_sbus();		/* convert kernel virtual to system bus */

/*	CSS transmission TYPE field

	BUS_TYPE [5:0] = [3 bit type field], [3 bit modifier field]
*/

/* type field */

#define SBUS_TYPE_RESPONSE	0	/* response */
#define SBUS_TYPE_READ		3	/* read */
#define SBUS_TYPE_WRITE		5	/* write */
#define SBUS_TYPE_CWRITE	6	/* control */ 

/* read or write modifier field */
	
#define SBUS_4BYTES_DATA	0	/* size is 4 bytes */
#define SBUS_1BYTE_DATA		1	/* size is 1 byte */
#define SBUS_2BYTES_DATA	2	/* size is 2 bytes */
#define SBUS_3BYTES_DATA	3	/* size is 3 bytes */
#define SBUS_8BYTES_DATA	4	/* size is 8 bytes */
#define SBUS_16BYTES_DATA	5	/* size is 16 bytes */
#define SBUS_32BYTES_DATA	6	/* size is 32 bytes */

/* response modifier field */

#define SBUS_RESPONSE_ERR	0	/* the response to a READ command that
					is somehow recognized as having failed
					to read anything. The data field of the
					response is undefined */ 
#define SBUS_RESPONSE_ERR_DATA0	2	/* indicates that the error was detected
					by the device responding to the READ */
#define SBUS_RESPONSE_ERR_DATA1	3	/* indicates that the error was a trans-
				      mission error detected by the IO MODULE */
#define SBUS_RESPONSE_DATA	4	/* normal response to a READ command. A
					READ command for more than 8 bytes
					requires more than one RESPONSE trans-
					mission */ 

/* control write modifier field */


#define SBUS_MODULE_ENABLE_OFF	   2	/* module enable off */
#define SBUS_MODULE_ENABLE_ON	   3	/* module enable on */
#define SBUS_MODULE_INTERFACE_OFF  4	/* module interface enable off */
#define SBUS_MODULE_INTERFACE_ON   5	/* module interface enable on */


#endif	/* SYS_SBUS_H */
