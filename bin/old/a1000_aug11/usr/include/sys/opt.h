#ifndef SYS_OPT_H
#define SYS_OPT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) opt.h: version 2.1 created on 5/22/89 at 19:09:13	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)opt.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	opt.h	3.13	*/
/* opt.h	6.1 */

#ident	"@(#)uts/head/sys:opt.h	2.1"

#if MESG==0
msgsys(){nosys();}
msginit(){return(0);}
#endif

#if SEMA==0
semsys(){nosys();}
seminit(){}
semexit(){}
#endif

#if SHMEM==0
shmsys(){nosys();}
shmexec(){}
shmexit(){}
shmfork(){}
shmreset(){}
shminit(){}
#endif

#endif /* SYS_OPT_H */
