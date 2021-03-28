/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) prof.h: version 1.2 created on 1/26/89 at 15:30:28	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)prof.h	1.2	1/26/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#)prof.h	1.2	*/
/*	prof.h	1.3	*/

#ident	"@(#)uts/head:prof.h	1.2"

#ifndef MARK
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
		asm("	movl	#.L.,a0");\
		asm("	jbsr	mcount");\
		}
#else /* !OCC */
/*
**	The label L_F_%X should be expanded to LpMpX (p is % sign).
**	Since this file is sccs'd the pMp (p is % sign) is expanded
**	by sccs to a module name which will screw up the profiler.
**	If this label name is not hardcoded in either the compiler
**	or profiler then we can change the name to something else.
**	NOTE:	If the macro doesn't expand correctly, remove
**		the "#undef _F_" statement.
*/
#define	_F_	%M
#define MARK(X)	{\
		asm("	data");\
		asm("	even");\
		asm("X_F_:");\
		asm("	long	0");\
		asm("	text");\
		asm("L_F_%X:");\
		asm("	mov.l	&X_F_,%a0");\
		asm("	jsr	mcount%");\
		}
#undef	_F_
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
