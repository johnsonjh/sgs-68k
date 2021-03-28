#ifndef	SYS_M68020_MREG_H
#define	SYS_M68020_MREG_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) Mreg.h: version 24.1 created on 10/28/91 at 18:30:19	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)Mreg.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#define SP	0
#define D0	1
#define D1	2
#define D2	3
#define D3	4
#define D4	5
#define D5	6
#define D6	7
#define D7	8
#define A0	9
#define A1	10
#define A2	11
#define A3	12
#define A4	13
#define A5	14
#define A6	15
#define A7	0

#define SR	16
#define PC	17
#define FP	A6

/* The mask of user modifiable bits in the SR */
#define NZVC	0xf
#endif	/* SYS_M68020_MREG_H */
