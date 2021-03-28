/*	prof.h	1.3	*/


#ifndef MARK
#ident	"@(#)prof.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define MARK(L)	{}
#else
#undef MARK
#if(m68 || m68k || M68020) 
#ifdef OCC
#define MARK(L)	{\
		asm("	.data");\
		asm("	.align	2");\
		asm(".L.:");\
		asm("	.long	0");\
		asm("	.text");\
		asm("M.L:");\
		asm("	mov.l	#.L.,-(%sp)");\
		asm("	jsr	mcount%");\
		asm("	add.w	&4,%sp");\
		}
/* 		asm("	movl	#.L.,a0");\		last 3 asm lines replace these old 
		asm("	jbsr	mcount");\	    two lines (MIRCEA)		*/
#else /* !OCC */
#define MARK(X)	{\
		asm("	data");\
		asm("	even");\
		asm("X%M:");\
		asm("	long	0");\
		asm("	text");\
		asm("Lprof.hX:");\
		asm("	mov.l	&X%M,-(%sp)");\
		asm("	jsr	mcount%");\
		asm("	add.w	&4,%sp");\
		}
/*		asm("	mov.l	&X%M,%a0");\ last 3 asm lines replace these old 
		asm("	jsr	mcount%");\		 two lines (MIRCEA)		*/
#endif /* OCC */
#endif /* m68 || m68k */
#ifdef vax
#define MARK(L)	{\
		asm("	.data");\
		asm("	.align	4");\
		asm(".L.:");\
		asm("	.long	0");\
		asm("	.text");\
		asm("M.L:");\
		asm("	nop;nop");\
		asm("	movab	.L.,r0");\
		asm("	jsb	mcount");\
		}
#endif
#ifdef u3b
#define MARK(L)	{\
		asm("	.data");\
		asm("	.align	4");\
		asm(".L.:");\
		asm("	.word	0");\
		asm("	.text");\
		asm("M.L:");\
		asm("	movw	&.L.,%r0");\
		asm("	jsb	_mcount");\
		}
#endif
#ifdef pdp11
#define MARK(L)	{\
		asm("	.bss");\
		asm(".L.:");\
		asm("	.=.+2");\
		asm("	.text");\
		asm("M.L:");\
		asm("	mov	$.L.,r0");\
		asm("	jsr	pc,mcount");\
		}
#endif
#endif

