#ifndef SYS_GCP_MACROS_H
#define SYS_GCP_MACROS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) macros.h: version 2.1 created on 5/22/89 at 19:06:41	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)macros.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)head/gcp:macros.h	1.5"

#define DISABLE() {\
	asm("	or.w	&0x0600,%sr");\
	asm("	add.b	&1,sys_eidi");\
}

#define DISABLE0() {\
	asm("	or.w	&0x0600,%sr");\
	asm("	mov.b	&1,sys_eidi");\
}

#define ENABLE() {\
	if (!sys_eidi || !--sys_eidi)\
		asm("	and.w	&0xf8ff,%sr");\
}

#define ENABLE0() {\
	asm("	mov.b	&0,sys_eidi");\
	asm("	and.w	&0xf8ff,%sr");\
}

#define EIDI0() {\
	asm("	mov.b	&0,sys_eidi");\
	asm("	and.w	&0xf8ff,%sr");\
	asm("	nop");\
	asm("	or.w	&0x0600,%sr");\
	asm("	mov.b	&1,sys_eidi");\
}

/*****************************************************************
 *
 *	B Y T E   O U T
 *
 *	Get a byte from a "cirbuf"
 *
 *	input parameters:
 *		1.  buffer pointer
 *		2.  the byte
 *
 *	output parameters:
 *		2.  -1 if the buffer is empty
 *
 *****************************************************************/

#define BYTEOUT(bp, c) {\
	register unsigned char *tmp;\
	if(!bp->count)\
		c = -1;\
	else {\
		c = *bp->hoq;\
		tmp = bp->hoq + 1;\
		if (tmp == bp->end)\
			tmp = bp->base;\
		bp->hoq = tmp;\
		bp->count--;\
	}\
}

#define BYTE_OUT(bp, c) {\
	register unsigned char *tmp;\
	if (bp->cb_eoq == bp->cb_hoq)\
		c = -1;\
	else {\
		c = *bp->cb_hoq;\
		tmp = bp->cb_hoq + 1;\
		if (tmp == bp->cb_end)\
			tmp = bp->cb_base;\
		bp->cb_hoq = tmp;\
	}\
}

/*****************************************************************
 *
 *	B Y T E   I N
 *
 *	Place a byte into a "cirbuf"
 *
 *	input parameters:
 *
 *		1.  buffer pointer
 *		2.  the byte
 *		3.  return code
 *			1  -  if successful
 *			0  -  if buffer is full
 *
 *****************************************************************/

#define BYTEIN(bp, c, rc) {\
	register unsigned char *tmp;\
	if((bp->hoq == bp->eoq) && bp->count)\
		rc = 0;\
	else {\
		*bp->eoq = c;\
		tmp = bp->eoq + 1;\
		if (tmp == bp->end)\
			tmp = bp->base;\
		bp->eoq = tmp;\
		bp->count++;\
		rc = 1;\
	}\
}


#define FLUSHBUF(cb) {\
	cb.count = 0;\
	cb.hoq = cb.eoq;\
}

#endif /* SYS_GCP_MACROS_H */
