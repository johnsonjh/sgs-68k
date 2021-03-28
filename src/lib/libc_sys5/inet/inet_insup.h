#ident	"@(#)inet_insup.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*
 *	Copyright 1986 Lachman Associates Inc. and The Instruction Set Ltd.
 */
/*      @(#)inet_insup.h	2.4 LAI KNFS for ARETE  source        */
/*      @(#)inet_insup.h	2.4 System V NFS  source        */
#ifdef EXOS
/* Machine defs. EXOS uses upper case, /usr/lib/cpp uses lower case */
#ifdef vax
#define VAX
#endif
#ifdef pdp11
#define PDP11
#endif
#ifdef I8086
#define i8096	/* Assuming there exists a cpp that defines 8086 this way */
#endif
#if m68k || M68020
#define M68000	/* Excelan for Arete does it this way */
#endif
#ifdef Z8000
#define z8000	/* Assuming there exists a cpp that defines z8000 this way */
#endif
/*
 * Following definitions are taken from
 * in.h 1.1 84/12/20 SMI; from UCB 4.20 83/01/17
 */
#define IN_CLASSA_NSHIFT 24
#define IN_CLASSB_NSHIFT 16
#define IN_CLASSC_NSHIFT 8
#else
/*
 * If Developer is not using a BSD in.h, then he may need to do something
 * like the above.
 */
#endif
