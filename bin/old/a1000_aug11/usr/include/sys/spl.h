#ifndef SYS_SPL_H
#define SYS_SPL_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) spl.h: version 2.1 created on 5/22/89 at 19:10:51	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)spl.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/sys:spl.h	2.1"

#define fspl0()  \
{ \
	asm("and.w	&0xf8ff,%sr"); \
}

#define fspl1()  \
{ \
	asm("mov.w	%sr,%d1		");  \
	asm("and.w	&0xf8ff,%d1		");  \
	asm("or.w	&0x0400,%d1		");  \
	asm("mov.w    %d1,%sr		"); \
}

#define fspl2()  \
{ \
	asm("mov.w	%sr,%d1		");  \
	asm("and.w	&0xf8ff,%d1		");  \
	asm("or.w	&0x0400,%d1		");  \
	asm("mov.w    %d1,%sr		"); \
}

#define fspl3()  \
{ \
	asm("mov.w	%sr,%d1		");  \
	asm("and.w	&0xf8ff,%d1		");  \
	asm("or.w	&0x0400,%d1		");  \
	asm("mov.w    %d1,%sr		"); \
}

#define fspl4()  \
{ \
	asm("mov.w	%sr,%d1		");  \
	asm("and.w	&0xf8ff,%d1		");  \
	asm("or.w	&0x0400,%d1		");  \
	asm("mov.w    %d1,%sr		");  \
}

#define fsplhi()  \
{ \
	asm("mov.w	%sr,%d1		");  \
	asm("and.w	&0xf8ff,%d1		");  \
	asm("or.w	&0x0400,%d1		");  \
	asm("mov.w    %d1,%sr		"); \
}

#define fspl5()  \
{ \
	asm("mov.w	%sr,%d1		");  \
	asm("and.w	&0xf8ff,%d1		");  \
	asm("or.w	&0x0400,%d1		");  \
	asm("mov.w    %d1,%sr		"); \
}

#define fspl6()  \
{ \
	asm("mov.w	%sr,%d1		");  \
	asm("and.w	&0xf8ff,%d1		");  \
	asm("or.w	&0x0500,%d1		");  \
	asm("mov.w    %d1,%sr		");  \
}

#define factspl6()  \
{ \
	asm("mov.w	%sr,%d1		");  \
	asm("and.w	&0xf8ff,%d1		");  \
	asm("or.w	&0x0600,%d1		");  \
	asm("mov.w    %d1,%sr		");   \
}

#define fspl7()  \
{ \
	asm("or.w	&0x0700,%sr		"); \
}

#endif /* SYS_SPL_H */
