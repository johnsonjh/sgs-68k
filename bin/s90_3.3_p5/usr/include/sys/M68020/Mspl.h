#ifndef SYS_M68020_MSPL_H
#define SYS_M68020_MSPL_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Mspl.h: version 24.1 created on 10/28/91 at 18:30:23	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Mspl.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#define fspl0()  {asm("mov.w	&0x2000,%sr");}
#define fspl1()  {asm("mov.w	&0x2100,%sr");}
#define fspl2()  {asm("mov.w	&0x2200,%sr");}
#define fspl3()  {asm("mov.w	&0x2300,%sr");}
#define fspl4()  {asm("mov.w	&0x2400,%sr");}
#define fspl5()  {asm("mov.w	&0x2500,%sr");}
#define fspl6()  {asm("mov.w	&0x2600,%sr");}
#define fspl7()  {asm("mov.w	&0x2700,%sr");}

#define fsplhi()  fspl7()

#endif /* SYS_M68020_MSPL_H */
