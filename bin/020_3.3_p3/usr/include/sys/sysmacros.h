#ifndef SYS_SYSMACROS_H
#define SYS_SYSMACROS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sysmacros.h: version 24.1 created on 10/28/91 at 18:39:12	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sysmacros.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/sysmacros.h	10.5"	*/

#ifndef SYS_TYPES_H
#include "sys/types.h"
#endif

#ifndef SYS_IMMU_H
#include "sys/immu.h"
#endif	/* SYS_IMMU_H */
/*
 * Some macros for units conversion
 */

/* Core clicks to segments and vice versa */
#define ctos(x)		(((x) + (NCPS-1)) >> CPSSHIFT)
#define	ctost(x)	((x) >> CPSSHIFT)
#define	stoc(x)		((x) * NCPS)

/* byte address to segment and vice versa  */
/* Ravi: be careful */
#define sgnum(x)	(((unsigned)(x) >> SNUMSHFT) & SNUMMASK)
#define stob(x)		(((unsigned)(x)) << SNUMSHFT)
#define	btos(x)		((unsigned)(x) >> SNUMSHFT)

/* Core clicks to immu max offset and vice versa */
#define ctomo(x)	((x) * 256 -1)
#define motoc(x)	((((x)+1)*8) >> BPCSHIFT)
#define motob(x)	(8*((x)+1)-1)

/* Core clicks to disk blocks */
#define	ctod(x) ((x)<<2)

/* inumber to disk address */
#define	itod(x)	(daddr_t)(((unsigned)(x)+(2*INOPB-1))>>INOSHIFT)
/* inumber to disk offset */
#define	itoo(x)	(int)(((unsigned)(x)+(2*INOPB-1))&(INOPB-1))

#define sec_itod(x, isize)    (daddr_t)(((unsigned)(x)+(2*SECINOPB(isize)-1)) \
	>>SECINOSHIFT(isize))
#define sec_itoo(x, isize)    (daddr_t)(((unsigned)(x)+(2*SECINOPB(isize)-1)) \
	&(SECINOPB(isize)-1))




/* clicks to bytes */
#define	ctob(x)	((x)<<BPCSHIFT)

/* bytes to clicks */
#define	btoc(x)	(((unsigned)(x)+(NBPC-1))>>BPCSHIFT)
#define	btoct(x)	((unsigned)(x)>>BPCSHIFT)

#ifdef notdef
/* AT&T's version of major and minor */
/* major part of a device internal to the kernel */

extern char MAJOR[128];
#define	major(x)	(int)(MAJOR[(unsigned)((x)>>8)&0x7F])
#define	bmajor(x)	(int)(MAJOR[(unsigned)((x)>>8)&0x7F])
/* minor part of a device internal to the kernel */
extern char MINOR[128];
#define	minor(x)	(int)(MINOR[(unsigned)((x)>>8)&0x7F]+((x)&0xFF))

#else

/*
 * Arix 9/88 JPC
 * 
 * The major, minor, and makedev macros were changed to allow extended minor
 * devices.  That is, easy extension of the number of minor devices per driver
 * to more than 256 by using more than one major device number for a driver.
 * The full device number is converted to a minor device number by subtracting
 * a value stored in the MINOR array, indexed by the major number.
 *
 * Problem:  If a minor device number can be > 255, then how does the minor
 * macro decide between a full device number with a small major component, and
 * a previously minored number?
 *
 * Solution:  Set the high bit (bit 15) of all full device numbers before
 * using minor.  This means that the largest major device number is 127,
 * and that there can be at most 32K devices.
 *
 * NOTE:  The only device driver functions that are given a full device
 * number are the open and close routines of streams drivers and the strategy
 * functions for block drivers (except when called for raw I/O).  The operating
 * system will already have set the "unminored" bit for the device number
 * passed to these functions.  All other driver functions will only get a minor
 * device number.
 *
 * As always, a device number should only be manipulated using these macros.
 * Any other use is implementation-dependent and will probably fail in future
 * releases of the operating system.
 */

#ifdef INKERNEL

/* major part of a device number */
#define	major(x)	(int)(((dev_t)(x) >> 8) & 0x7F)
#define	bmajor(x)	major(x)
#define gmajor(x)	GMAJOR[major(x)]
#define next_major(maj)	NMAJOR[maj]	/* note that maj is already a major */

#define UNMINOR_BIT	0x8000		/* a dev_t with this bit is unminored */
#define UNMINOR_MASK	0x7fff		/* removes the unminored bit	*/

#define notminored(x)	((x) | UNMINOR_BIT)  /* add unminored bit to full dev */
#define denotminored(x)	((x) & UNMINOR_MASK) /*unzombie? dezombie? antizombie?*/

/* minor part of a device number */
#define minor(x)	(int)(((x) & UNMINOR_BIT) ? \
			  (dev_t)(x) - MINOR[major(x)] : (dev_t)(x))

extern unsigned short	MINOR[];	/* the offsets for extended minors */
extern char		GMAJOR[];	/* extended major group leader	*/
extern char		NMAJOR[];	/* next major number in group or -1 */

#else /* INKERNEL */

#define	major(x)	(int)((dev_t)(x) >> 8)
#define	bmajor(x)	major(x)
#define minor(x)	(int)((dev_t)(x) & 0xFF)

#endif /* INKERNEL */

#endif /* notdef */

/*
 * make a device number -- assumes adjacent major numbers if min > 255
 */
#define	makedev(maj, min)	(dev_t)(((dev_t)(maj) << 8) + (dev_t)(min))

/*
 *   emajor() allows kernel/driver code to print external major numbers
 *   eminor() allows kernel/driver code to print external minor numbers
 */

#define emajor(x)	major(x)
#define eminor(x)	minor(x)

/*	Calculate user process priority.  */
#define calcppri(p)	((p->p_cpu) >> 1) +  p->p_nice + (PUSER - NZERO)

/*  Evaluate to true if the process is a server - Distributed UNIX */
#define	server()	(u.u_procp->p_sysid != 0)

extern int rcacheinit;
extern unsigned long rfs_vcode;


#ifdef RFSCACHE
#define CLOSEI(ip, flag, count, offset) \
{ \
	if (rcacheinit && (ip)->i_ftype == IFREG && (count) == 1 \
		&& !server() && (flag) & FWRITE) { \
		(ip)->i_wcnt--; \
		if ((ip)->i_flag & IWROTE && (ip)->i_wcnt == 0) { \
			(ip)->i_flag &= ~IWROTE; \
			if ((ip)->i_rcvd) \
				enable_cache((ip)->i_rcvd); \
		} \
	} \
	FS_CLOSEI(ip, flag, count, offset); \
}
#else
#define CLOSEI(ip, flag, count, offset) \
	FS_CLOSEI(ip, flag, count, offset);
#endif /* RFSCACHE */


#ifdef RFSCACHE
#define OPENI(ip, mode) \
{ \
	if(rcacheinit && (ip)->i_ftype==IFREG && !server() && (mode) & FWRITE) \
		(ip)->i_wcnt++; \
	FS_OPENI(ip, mode); \
}
#else
#define OPENI(ip, mode) \
	FS_OPENI(ip, mode); 
#endif /* RFSCACHE */


#ifdef RFSCACHE
#define WRITEI(ip) \
{ \
	if (rcacheinit && (ip)->i_ftype == IFREG) { \
		if (!server()) \
			(ip)->i_flag |= IWROTE; \
		(ip)->i_vcode = ++rfs_vcode; \
		if ((ip)->i_rcvd) \
			disable_cache(ip); \
	} \
	FS_WRITEI(ip); \
}
#else
#define WRITEI(ip) \
	FS_WRITEI(ip); 
#endif /* RFSCACHE */


#ifdef RFSCACHE
#define ITRUNC(ip) \
{ \
	if (rcacheinit && (ip)->i_ftype == IFREG) { \
		if (!server()) \
			(ip)->i_flag |= IWROTE; \
		(ip)->i_vcode = ++rfs_vcode; \
		if ((ip)->i_rcvd) \
			disable_cache(ip); \
	} \
	FS_ITRUNC(ip); \
}
#else
#define ITRUNC(ip) \
	FS_ITRUNC(ip);
#endif /* RFSCACHE */


#ifdef RFSCACHE
#define IPUT(ip) \
{ \
	if (rcacheinit && (ip)->i_nlink <= 0 && (ip)->i_count == 1) { \
		(ip)->i_vcode = ++rfs_vcode; \
	} \
	FS_IPUT(ip); \
}
#else
#define IPUT(ip) \
	FS_IPUT(ip);
#endif /* RFSCACHE */


#ifdef RFSCACHE
#define FREESP(ip, arg, flag, offset) \
{ \
	if (rcacheinit && (ip)->i_ftype == IFREG) { \
		if (!server()) \
			(ip)->i_flag |= IWROTE; \
		(ip)->i_vcode = ++rfs_vcode; \
		if ((ip)->i_rcvd) \
			disable_cache(ip); \
	} \
	FS_FCNTL(ip, F_FREESP, arg, flag, offset); \
}
#else
#define FREESP(ip, arg, flag, offset) \
	FS_FCNTL(ip, F_FREESP, arg, flag, offset);
#endif /* RFSCACHE */

/* machine dependent operations - defined for RFS and STREAMS */

/*	Add CAN_ALIGN for aligning items for canonical form.
 *	On M68000 struct members are aligned on short bounds, but
 *	canonical items must still be aligned on long bounds.
 */

#ifdef	pdp11
#define	SALIGN(p)		(char *)(((int)p+1) & ~1)
#define	IALIGN(p)		(char *)(((int)p+1) & ~1)
#define LALIGN(p)		(char *)(((int)p+1) & ~3)
#endif
#ifdef	vax
#define	SALIGN(p)		(char *)(((int)p+1) & ~1)
#define	IALIGN(p)		(char *)(((int)p+3) & ~3)
#define	LALIGN(p)		(char *)(((int)p+3) & ~3)
#endif
#ifdef	u3b2
#define	SALIGN(p)		(char *)(((int)p+1) & ~1)
#define	IALIGN(p)		(char *)(((int)p+3) & ~3)
#define	LALIGN(p)		(char *)(((int)p+3) & ~3)
#endif
#if defined(M68020) || defined(M68040)
/* Ravi: think about it */
/* FOR NOW, this is same as 020. When 040 uses long alignment, then change */
#define	SALIGN(p)		(char *)(((int)p+1) & ~1)
#define	IALIGN(p)		(char *)(((int)p+1) & ~1)
#define	LALIGN(p)		(char *)(((int)p+1) & ~1)
#define	CAN_ALIGN(p)		(char *)(((int)p+3) & ~3)
#else
#define CAN_ALIGN	LALIGN
#endif


#define SNEXT(p)		(char *)((int)p + sizeof (short))
#define INEXT(p)		(char *)((int)p + sizeof (int))
#define LNEXT(p)		(char *)((int)p + sizeof (long))

#endif /* SYS_SYSMACROS_H */
