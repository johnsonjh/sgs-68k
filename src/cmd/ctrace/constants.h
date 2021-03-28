/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) constants.h: version 25.1 created on 12/2/91 at 14:09:35	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)constants.h	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT:#ident	"ctrace:constants.h	1.3"	*/

#ident	"@(#)ctrace:constants.h	25.1"

/*	ctrace - C program debugging tool
 *
 *	preprocessor constant definitions
 *
 */

#define IDMAX		8	/* maximum significant identifier chars */
#define INDENTMAX	40	/* maximum left margin indentation chars */
#define LOOPMAX		20	/* statement trace buffer size */
#define SAVEMAX		80	/* parser function header and brace text storage size */
#define	STMTMAX		400	/* maximum statement text length */
#define TOKENMAX	30	/* maximum non-string token length */
#define	TRACE_DFLT	10	/* default number of traced variables */
#define TRACEMAX	20	/* maximum number of traced variables */
#undef	YYLMAX		
#define YYLMAX		STMTMAX + TOKENMAX	/* scanner line buffer size */
#define YYMAXDEPTH	300	/* yacc stack size */

#define NO_LINENO	0
#define	PP_COMMAND	"/lib/cpp -C -DCTRACE"	/* preprocessor command */
#define	RUNTIME		LIB/runtime.c"	/* run-time code package file */
