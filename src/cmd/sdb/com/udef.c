/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) udef.c: version 23.2 created on 1/2/91 at 16:11:15	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)udef.c	23.2	1/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/com:udef.c	23.2"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
	/*	OLD MOT:udef.c	6.1		*/


#include	"head.h"

ADDR	lastknownadr = 0;
/*	#include <sys/types.h>		removed */
/*	char		u[XTOB(USIZE)]; /* struct user u: - see below */

/*	added following
 *  Space allocated for objects declared external in head.h
 *  	also machdep.h setup.c main.c runpcs.c
 *  In UNIX 5.0 cannot have objects declared in more than one file.
 */

/* file containing sdb commands - stdin default */
FILE *inputfile = stdin;

/* initial value for metaflag in outvar (prvar.c) - 0 if stdin input,
 * SCRIPTEXP otherwise (input from script)
 */
int metabase = 0;

/* input line decoding */
#ifdef FLEXNAMES
char	proc[BUFSIZ];		/* Procedure name. */
#else
char	proc[30];	/* procedure name */
#endif
long	integ;		/* count or number in input */
int	integ2;		/* count or number in input following comma */
char 	cmd;		/* command letter */
char	re[MAXPATH];	/* regular expression */	/* dd0 */
char	args[MAXPATH];	/* arguments */	/* dd0 */
char	*argsp;		/* pointer to args */
#ifdef FLEXNAMES
char	var[BUFSIZ];	/* Variable name. */
#else
char	var[60];	/* variable name */
#endif
int	scallf;		/* set to 1 iff procedure call */
int	reflag;		/* set to 1 iff re */
int	redir;		/* set to 1 iff forward search */
int	colonflag;	/* set to 1 iff colon typed */
int	ncolonflag;	/* set to 1 iff colon typed after number */
int	percentflag;	/* set to 1 iff percent symbol typed */

/* value to be assigned ('!' command) decoding */
struct rhs_struct rhs_value;

/* source file i/o */
char	curfile[MAXPATH];	/* name of file being edited */	/* dd0 */
struct stmt curstmt;	/* current statement in file */
int	maxfline;	/* maximum line number in file, 0 if unknown */
struct brbuf  fiobuf;	/* file descriptor */
extern char  fbuf[];	/* current line from file */
char 	filework[MAXPATH];	/* place to put filename */	/* dd0 */
char	sdbpath[ MAXPATH ];	/* Directory lookup path array for files. */	/* dd0 */
int	sdbpsz = sizeof( sdbpath );	/* Sizeof of sdbpath lookup array */
char 	*fwp;		/* pointer to it (or within it -- see main) */
int	nolines;	/* set to 1 iff no lines in file */

/* returned by slookup */
#ifdef FLEXNAMES
char	sl_name[BUFSIZ];
#else
char	sl_name[SYMNMLEN +1];
#endif
char	sl_class;
ushort	sl_type;
ushort	sl_stype;	/* simple type, i.e. type after dereferencing */
int	sl_size, sl_addr;
short	sl_scnum;
struct proct *sl_procp;
ushort	sl_dimen[DIMNUM];

/* procedure call information */
int scallx;			/* procedure call in progress */
ADDR fps, aps, pcs;		/* old stack frame */
ADDR	adsubx;			/* hanna: return from trap addr */
ADDR	lastknownaddr = 0;	/* hanna: pc before subroutine call */
BKPTR bkpts;			/* old breakpoint */
int flagss;			/*  and its flags */
char dschar;			/* '/' if value should be displayed */
ADDR adsubn, adsubc;		/* addresses of __dbsubn, __dbsubc */
int adargs;			/* address of __dbargs */

/* symbol table info */
long	ststart;		/* offset of symbol table in a.out */
struct brbuf sbuf;		/* buffer for symbol table entry */
long	extstart;		/* offset of first external in a.out */

/* address info */
ADDR	dot;			/* current address */

/* setup information */
extern STRING	symfil, corfil;	/* file names */
INT	fsym, fcor;		/* file descriptors */
INT	fakecor = 0;		/* 1 iff user "core" not really core file */
MAP	txtmap, datmap;		/* maps */
INT	argcount;		/* number of arguments to sdb */

/* process info */
L_INT	cntval;			/* count for c and s instructions */
INT	signo;			/* returned signal */
INT	adrflg;			/* new address for doexec */
/*ADDR	userpc = (-1);		/* program counter in user program */
ADDR	userpc = (0); /* hanna	/* program counter in user program */
INT	pid;			/* process id of user program */
BKPTR	bkpthead;		/* first breakpoint */

/* other */
int	sflag;			/* option -s specified */
int	Wflag;			/* option -W (no warnings) specified */
#if DEBUG > 1
int	debugflag;		/* option -d specified */
#endif
STRING	errflg;			/* 0 or pointer to error message */
char	odesc[10];		/* descriptor of last displayed variable */
ADDR	oaddr;			/* address of last displayed variable */
char	otype;			/* type of last displayed variable */
char	oclass;			/* class of last displayed variable */
short	oincr;			/* size of last displayed variable */
				/* need short to hold size of case stmt */
#ifndef SGTTY
  struct termio sdbttym, usrttym;	/* tty structure(s) */
  int sdbttyf, usrttyf;			/* fcntl flags */
#else
  struct sgttyb sdbttym, usrttym;	/* tty modes for sdb and user */
#endif
enum lastcom lastcom;
char	oldargs[MAXPATH];	/* dd0 */
char prname[50];		/* print name used by outvar */
jmp_buf	env;			/* environment for setjmp, longjmp */
int	debug;			/* toggled by Y command */
int	vmode;			/* verbose; toggled by 'v' command */
time_t	symtime;		/* modification time of symfil */
ADDR	exactaddr, lnfaddr;	/* set by adrtolineno() */
ADDR	firstdata;		/* lowest address of data */

struct filet *files, *badfile;

struct proct *procs, *badproc;


/* main.c and/or runpcs.c */
INT		mkfault;
INT		executing = FALSE;

#ifndef _POSIX_SOURCE
ADDR		sigint;
ADDR		sigqit;
#else
void		(*sigint)();
void		(*sigqit)();
#endif

CHAR		lastc;
extern int	errno;
L_INT		loopcnt;


/* setup.c */
INT		magic;
STRING		symfil	= "a.out";
STRING		corfil	= "core";

/* machdep.h */

user_page_t	user_page;

ADDR 	regs[NUMREGS];
ADDR	callpc, frame, argp;		/* current stack frame */
