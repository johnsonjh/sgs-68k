#ifndef SYS_OPT_H
#define SYS_OPT_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) opt.h: version 24.1 created on 10/28/91 at 18:36:10	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)opt.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

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
