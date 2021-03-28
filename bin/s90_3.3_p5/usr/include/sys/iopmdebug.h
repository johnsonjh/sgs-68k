#ifndef SYS_IOPMDEBUG_H
#define SYS_IOPMDEBUG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) iopmdebug.h: version 24.1 created on 10/28/91 at 18:34:15	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)iopmdebug.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

extern iopm_dbg;

#define DBG      0x00000001	/* space holder (define iopm_dbg w/o printing)*/
/* streams debug flags */
#define SSEND    0x00000002
#define SRECV    0x00000004
#define SROUT    0x00000008
#define SOPEN    0x00000010
#define SCLOSE   0x00000020
#define SREAD    0x00000040
#define SWRITE   0x00000080
#define SIOCTL   0x00000100
#define SRESRC   0x00000200
#define SFLOW    0x00000400
#define SSRV     0x00000800
/* bufs debug flags */
#define SEND     0x00020000
#define INTR     0x00040000
#define ROUT     0x00080000
#define OPEN     0x00100000
#define CLOSE    0x00200000
#define READ     0x00400000
#define WRITE    0x00800000
#define IOCTL    0x01000000
#define DATA     0x02000000
#define STRAT    0x04000000
#define CONFIG   0x08000000
#define RESOURCE 0x10000000
#define FLOW     0x20000000

#ifdef IOPMDEBUG
#define PRINT(mask, s)  if ( iopm_dbg & (mask) ) printf(s)
#define PRINT1(mask, s, a)  if ( iopm_dbg & (mask) ) printf(s, a)
#define PRINT2(mask, s, a,b)  if ( iopm_dbg & (mask) ) printf(s, a,b)
#define PRINT3(mask, s, a,b,c)  if ( iopm_dbg & (mask) ) printf(s, a,b,c)
#define PRINT4(mask, s, a,b,c,d)  if ( iopm_dbg & (mask) ) printf(s, a,b,c,d)
#define PRINT5(mask, s, a,b,c,d,e)  if ( iopm_dbg & (mask) ) printf(s, a,b,c,d,e)

#define BPRINT_TYPE(mask,bp) \
    if ( iopm_dbg & (mask) ) \
	switch( bp->b_driver_flags & IOPM_DRIVER_FLAGS ) \
	{ \
	    case I_OPEN: printf("open"); break; \
	    case I_CLOSE: printf("close"); break; \
	    case I_IOCTL: printf("ioctl"); break; \
	    case I_STRAT: printf("strat "); \
		if ( bp->b_flags & B_PHYS ) \
		    printf("P "); \
		else \
		    printf("B "); \
		if ( bp->b_flags & B_READ ) \
		    printf("R"); \
		else \
		    printf("W"); \
		break; \
	    case I_DATA: printf("data "); \
		if ( bp->b_driver_flags & B_READ ) \
		    printf("R"); \
		else \
		    printf("W"); \
		break; \
	    case I_ERROR: printf("error"); break; \
	    case I_PRINT: printf("print"); break; \
	    default: printf("req=%x", bp->b_driver_flags ); break; \
	}

#define SPRINT_TYPE(mask,mp) \
    if ( iopm_dbg & (mask) ) \
	switch( mp->b_datap->db_type ) \
	{ \
	    case M_OPEN: printf("open"); break; \
	    case M_CLOSE: printf("close"); break; \
	    case M_DATA: printf("data"); break; \
	    case M_IOCTL: case M_IOCTL | QREMCMD: printf("ioctl"); \
		switch( ((struct iocblk *)mp->b_rptr)->ioc_cmd ) \
		{ \
		    case I_PUSH: printf("-I_PUSH"); break; \
		    case I_POP: printf("-I_POP"); break; \
		    case I_FIND: printf("-I_FIND"); break; \
		    case I_LOOK: printf("-I_LOOK"); break; \
		    case I_STR: printf("-I_STR"); break; \
		    case I_LINK: printf("-I_LINK"); break; \
		    case I_UNLINK: printf("-I_UNLINK"); break; \
		    case I_FDINSERT: printf("-I_FDINSERT"); break; \
		    default: printf("-%x", \
			       ((struct iocblk *)mp->b_rptr)->ioc_cmd); \
		} \
		break; \
	    case M_HICOPY: printf("hicopy"); break; \
	    case M_LOCOPY: printf("locopy"); break; \
	    case M_WFLOW: printf("wflow"); break; \
	    case M_RFLOW: printf("rflow"); break; \
	    case M_ERROR: printf("err"); break; \
	    case M_FLUSH: printf("flush"); break; \
	    case M_IOCACK: case M_IOCACK | QREMCMD: printf("iocack"); break; \
	    case M_IOCNAK: case M_IOCNAK | QREMCMD: printf("iocnak"); break; \
	    default: printf("req=%x", mp->b_datap->db_type ); break; \
	}

#else
#define PRINT(mask,s)
#define PRINT1(mask,s,a)
#define PRINT2(mask,s,a,b)
#define PRINT3(mask,s,a,b,c)
#define PRINT4(mask,s,a,b,c,d)
#define PRINT5(mask,s,a,b,c,d,e)
#define BPRINT_TYPE(mask,mp)
#define SPRINT_TYPE(mask,mp)
#endif

#endif /* SYS_IOPMDEBUG_H */
