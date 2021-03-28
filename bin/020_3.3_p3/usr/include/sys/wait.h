#ifndef SYS_WAIT_H
#define SYS_WAIT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) wait.h: version 24.1 created on 10/28/91 at 18:41:07	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)wait.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ifdef _POSIX_SOURCE
/* defined for posix waitpid system call */
#define WNOHANG		0x1
#define	WUNTRACED	0x2

/* Following define is used by kernel only. Applications SHOULD NOT use this */
#define	TRACED		0x4

/* Macros used by applications */
#define WIFEXITED(X)		((X & 0xFF) == 0)? 1:0
#define	WEXITSTATUS(X)		((X >> 8) & 0xFF)
#define WIFSIGNALED(X)		((((X >> 8) & 0xFF) == 0)&&(X&0xFF))? 1:0
#define	WTERMSIG(X)		(X & 0x7F)
#define WIFSTOPPED(X)		((X & 0xFF) == 0x7F)? 1:0
#define	WSTOPSIG(X)		((X >> 8) & 0xFF)
#endif /* _POSIX_SOURCE */

#endif /* SYS_WAIT_H */
