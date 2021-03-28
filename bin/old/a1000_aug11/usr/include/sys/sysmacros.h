#ifndef SYS_SYSMACROS_H
#define SYS_SYSMACROS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) sysmacros.h: version 2.1 created on 5/22/89 at 19:11:45	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)sysmacros.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/sysmacros.h	10.5"	*/

#ident	"@(#)uts/head/sys:sysmacros.h	2.1"
/*
	Modifications:
  001 12/05/88 djs	Added SALIGN, IALIGN, and LALIGN for ARIX
 */

/*
 * Some macros for units conversion
 */

/* Core clicks to segments and vice versa */
#define ctos(x)		(((x) + (NCPS-1)) >> CPSSHIFT)
#define	ctost(x)	((x) >> CPSSHIFT)
#define	stoc(x)		((x) * NCPS)

/* byte address to segment and vice versa  */
#define sgnum(x)	(((unsigned)(x) >> 18) & 0x3fff)
#define stob(x)		((((unsigned)(x)) & 0x3fff) << 18)
#define	btos(x)		((unsigned)(x) >> 18)

/* Core clicks to immu max offset and vice versa */
#define ctomo(x)	((x) * 256 -1)
#define motoc(x)	((((x)+1)*8) >> BPCSHIFT)
#define motob(x)	(8*((x)+1)-1)

/* Core clicks to disk blocks */
#define	ctod(x) ((x)<<2)

/* disk sectors to bytes */
#define dtob(x) (x<<10)
#define btod(x) ((x+1023) >> 10)
#define btodt(x) (x >> 10)

/* inumber to disk address */
#define	itod(x)	(daddr_t)(((unsigned)(x)+(2*INOPB-1))>>INOSHIFT)
/* inumber to disk offset */
#define	itoo(x)	(int)(((unsigned)(x)+(2*INOPB-1))&(INOPB-1))

/* clicks to bytes */
#define	ctob(x)	((x)<<BPCSHIFT)

/* bytes to clicks */
#define	btoc(x)	(((unsigned)(x)+(NBPC-1))>>BPCSHIFT)
#define	btoct(x)	((unsigned)(x)>>BPCSHIFT)

#ifdef notdef

/* major part of a device internal to the kernel */

extern char MAJOR[128];
#define	major(x)	(int)(MAJOR[(unsigned)((x)>>8)&0x7F])
#define	bmajor(x)	(int)(MAJOR[(unsigned)((x)>>8)&0x7F])
/* minor part of a device internal to the kernel */
extern char MINOR[128];
#define	minor(x)	(int)(MINOR[(unsigned)((x)>>8)&0x7F]+((x)&0xFF))

#else
#define	major(x)	(int)(((unsigned)x>>8)&0x7F)
#define	bmajor(x)	(int)(((unsigned)x>>8)&0x7F)

/* minor part of a device external from the kernel */
#define	minor(x)	(int)((unsigned)x&0xFF)
#endif /* notdef */

/* make a device number */
#define	makedev(x,y)	(dev_t)((((unsigned)x)<<8) | ((unsigned)y))

/*
 *   emajor() allows kernel/driver code to print external major numbers
 *   eminor() allows kernel/driver code to print external minor numbers
 */

#define emajor(x)	(int)(((unsigned)(x)>>8)&0x7F)
#define eminor(x)	(int)((x)&0xFF)

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

#ifdef	ARIX							/* 001 */
#define	SALIGN(p)		(char *)(((int)p+1) & ~1)
#define	IALIGN(p)		(char *)(((int)p+3) & ~3)
#define LALIGN(p)		(char *)(((int)p+3) & ~3)
#endif
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

#define SNEXT(p)		(char *)((int)p + sizeof (short))
#define INEXT(p)		(char *)((int)p + sizeof (int))
#define LNEXT(p)		(char *)((int)p + sizeof (long))


#endif /* SYS_SYSMACROS_H */
