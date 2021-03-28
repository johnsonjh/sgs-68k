/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) mode.h: version 23.1 created on 11/15/90 at 13:18:43	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)mode.h	23.1	11/15/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/com:mode.h	23.1"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
	/*	OLD MOT:mode.h	6.1		*/

/*
 *	UNIX debugger
 */

#define MAXCOM	128
#define MAXARG	32
#define LINSIZ	256

#define REG	register

typedef	unsigned long	ADDR;
typedef	unsigned long	POS;
typedef	int	INT;
typedef	int	VOID;
typedef	long	L_INT;
/*	TYPE	float		REAL;		not used */
/*	TYPE	double		L_REAL;		not used */
typedef	char		BOOL;
typedef	char		CHAR;
typedef	char		*STRING;
typedef	char		MSG[];
typedef	struct map	MAP;
typedef	MAP		*MAPPTR;
typedef	struct bkpt	BKPT;
typedef	BKPT		*BKPTR;

/* file address maps */
struct map {
	ADDR	b1;	/* beginning (internal) address */
	ADDR	e1;	/* ending (internal) address	*/
	ADDR	f1;	/* file address corresponding to b1 */
	ADDR	b2;	/* beginning (internal) address) */
	ADDR	e2;	/* ending (internal) address	*/
	ADDR	f2;	/* file address corresponding to b2 */
	INT	ufd;	/* UNIX file descriptor		*/
};
struct bkpt {
	ADDR	loc;	/* location of breakpoint	*/
	ADDR	ins;	/* instruction at breakpoint	*/
	INT	count;	/* not used ?? 		*/
	INT	initcnt;/* not used ?? 		*/
	INT	flag;	/* non-zero if breakpoint inserted */
	CHAR	comm[MAXCOM];	/* ASCII user command at breakpoint */
	BKPT	*nxtbkpt;	/* next breakpoint in sdb table	*/
};

typedef	struct reglist	REGLIST;
typedef	REGLIST		*REGPTR;
struct reglist {
	STRING	rname;	/* name of register (AP, R0, etc.)	*/
	INT	roffs;	/* reg offset (ints) from R0 in user area */
};
