/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) llvl_msgs.h: version 24.1 created on 10/28/91 at 17:51:19	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)llvl_msgs.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/* file scsi_msg.h	*/

/* message definition */
#define MSG_COMP	0x00   /* i  ;     command complete     	*/
#define MSG_EXTN	0x01   /* i,o;	   extended message 		*/
#define MSG_SYNC	0x01   /* i, ;	   sync data transfer request	*/
#define MSG_SAVE	0x02   /* i  ;     save data pointer		*/
#define MSG_REST	0x03   /* i  ;     restore pointers		*/
#define MSG_DISC	0x04   /* i  ;     disconnect			*/
#define MSG_ERRD	0x05   /*   o;     initiator detected error	*/
#define MSG_ABRT	0x06   /*   o;     abort			*/
#define MSG_RJCT	0x07   /* i,o; 	   message reject		*/
#define MSG_NOP		0x08   /*   o;	   no operation			*/
#define MSG_PERR	0x09   /*   o;     message parity error		*/
#define MSG_LNKC	0x0A   /* i  ;     linked command complete	*/
#define MSG_LNKF	0x0B   /* i  ;     linked command complete w/f  */
#define MSG_DRST   	0x0C   /*   o;     bus device reset		*/
#define MSG_IDFY	0x80   /* i,o;	   identify message 		*/

/* bit 6 definition of identify message	*/
#define DISC_RECONN	0x40		/* ability to accomodate disc/re */
