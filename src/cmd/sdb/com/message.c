/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) message.c: version 23.1 created on 11/15/90 at 13:18:40	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)message.c	23.1	11/15/90 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)sdb/com:message.c	23.1"

/*		Copyright (c) 1985 AT&T		*/
/*		All Rights Reserved		*/
	/*	OLD MOT:message.c	6.1		*/

/*
 *	UNIX debugger
 *		Can put this file in text space !
 */



#include	"mode.h"
#include	"machdep.h"	/* for NSIG in signal.h */


MSG		BADMOD	=  "Bad modifier";
MSG		NOBKPT	=  "No breakpoint set";
MSG		NOPCS	=  "No process";
MSG		BADTXT	=  "Text address not found";
MSG		BADDAT	=  "Data address not found";
MSG		EXBKPT	=  "Too many breakpoints";
MSG		ENDPCS	=  "Process terminated";
MSG		SZBKPT	=  "Bkpt: Command too long";
MSG		BADWAIT	=  "Wait error: Process disappeared!";
MSG		NOFORK	=  "Try again";
MSG		BADMAGN =  "Bad core magic number";

STRING		signals[NSIG] = {
		"",
		"Hangup (1)",
		"Interrupt (2)",
		"Quit (3)",
		"Illegal Instruction (4)",
		"Trace/BPT (5)",
		"IOT (6)",
		"EMT (7)",
		"Floating Exception (8)",
		"Killed (9)",
		"Bus Error (10)",
		"Memory Fault (11)",
		"Bad System Call (12)",
		"Broken Pipe (13)",
		"Alarm Call (14)",
		"Terminated (15)",
		"User Signal 1 (16)",
		"User Signal 2 (17)",
		"Death of a Child (18)",
		"Power-Fail Restart (19)",
};
