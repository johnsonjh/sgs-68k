#ifndef SYS_MFS_H
#define SYS_MFS_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mfs.h: version 2.1 created on 5/22/89 at 19:08:40	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mfs.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*
 * mfs.h
 *
 *	defines for the multi-processor file system 
 *
 */

#ident	"@(#)uts/head/sys:mfs.h	2.1"


/* synchronization defines */

#define exit_short_cr_2000(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	&0x3000, %sr	"); \
}

#define exit_short_cr_x000(x)	\
{ \
	*x = 0;			\
	asm("	and.w	&0xf0ff, %sr	"); \
}

#define exit_short_cr_2100(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	&0x3400, %sr	"); \
}

#define exit_short_cr_x100(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	%sr, %d0	"); \
	asm("	and.w	&0xf0ff, %d0	"); \
	asm("	or.w	&0x0400, %d0	"); \
	asm("	mov.w	%d0, %sr	"); \
}


#define exit_short_cr_2200(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	&0x3400, %sr	"); \
}

#define exit_short_cr_x200(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	%sr, %d0	"); \
	asm("	and.w	&0xf0ff, %d0	"); \
	asm("	or.w	&0x0400, %d0	"); \
	asm("	mov.w	%d0, %sr	"); \
}

#define exit_short_cr_2300(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	&0x3400, %sr	"); \
}

#define exit_short_cr_x300(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	%sr, %d0	"); \
	asm("	and.w	&0xf0ff, %d0	"); \
	asm("	or.w	&0x0400, %d0	"); \
	asm("	mov.w	%d0, %sr	"); \
}

#define exit_short_cr_2400(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	&0x3400, %sr	"); \
}

#define exit_short_cr_x400(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	%sr, %d0	"); \
	asm("	and.w	&0xf0ff, %d0	"); \
	asm("	or.w	&0x0400, %d0	"); \
	asm("	mov.w	%d0, %sr	"); \
}

#define exit_short_cr_2500(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	&0x3400, %sr	"); \
}

#define exit_short_cr_x500(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	%sr, %d0	"); \
	asm("	and.w	&0xf0ff, %d0	"); \
	asm("	or.w	&0x0400, %d0	"); \
	asm("	mov.w	%d0, %sr	"); \
}

#define exit_short_cr_2600(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	&0x3500, %sr	"); \
}

#define exit_short_cr_x600(x)	\
{ \
	*x = 0;			\
	asm("	mov.w	%sr, %d0	"); \
	asm("	and.w	&0xf0ff, %d0	"); \
	asm("	or.w	&0x0500, %d0	"); \
	asm("	mov.w	%d0, %sr	"); \
}

#define exit_short_cr_2700(x)	\
{ \
	*x = 0;			\
	asm("	or.w	&0x0700, %sr	"); \
}


#endif /* SYS_MFS_H */
