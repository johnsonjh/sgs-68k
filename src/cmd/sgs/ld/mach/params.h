#ident	"@(#)params.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

	

/*
 */
/***********************************************************************
*   This file contains system dependent parameters for the link editor.
*   There must be a different params.h file for each version of the
*   link editor (e.g. b16, n3b, mac80, etc.)
*
*   THIS COPY IS FOR M80
*********************************************************************/


/*
 * Maximum size of a section
 */
#define MAXSCNSIZE	0x20000000L
#define MAXSCNSZ	MAXSCNSIZE


/*
 * Default size of configured memory
 */
#define SEGVIOORG	0x1000L		/* start the text at page 1 */
					/* this will cause seg vio  */
					/* on deref of 0            */
#define MEMORG		0x0L
#define MEMSIZE		0xffffffffL
#define NONULLORG	0x20000L

/*
 * Size of a region. If USEREGIONS is zero, the link editor will NOT
 * permit the use of REGIONS, nor partition the address space
 * USEREGIONS is defined in system.h
 */
#define REGSIZE 	0

#define COM_ALIGN	0x3L	/* Align factor for .comm's	*/

/*
 * define boundary for use by paging
 */

/* 3b2: #define BOUNDARY	0x80000		/* 512K */
/* sw1 */
#define BOUNDARY	0x2000			/* 8K */

/*
 * define special symbol names
 */

#define _CSTART	"_start"
#define _MAIN	"main"
