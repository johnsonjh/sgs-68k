#ifndef SYS_GCP_GCICB_H
#define SYS_GCP_GCICB_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gcicb.h: version 2.1 created on 5/22/89 at 19:06:16	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gcicb.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*******************************************************************
 *
 *			G  C  I  C  B
 *
 *******************************************************************/

#define ICB_SLOT	(*ICB_LOSTAT & 0x1f)
#define ICB_LOCKED	(*ICB_LOSTAT & 0x80)
#define ICB_BASE	(0x600000 + (ICB_SLOT << 16))
#define ICB_TOP		(0x60ffa0 + (ICB_SLOT << 16))

/*******************************************************************
 *
 *	G C I C B    V I R T U A L    R E G I S T E R S
 *
 *		r0  -  struct chead *chead
 *		r1  -  struct tty *tty0
 *		r2  -  struct tty *tty1
 *		r3  -  struct tty *tty2
 *		r4  -  struct tty *tty3
 *		r5  -  struct tty *tty4
 *		r6  -  struct tty *tty5
 *		r7  -  struct tty *tty6
 *		r8  -  struct tty *tty7
 *
 *******************************************************************/

#define VREGS	(int *)(sys_icb_base + 0xff00)

#endif /* SYS_GCP_GCICB_H */
