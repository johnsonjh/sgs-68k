/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"head:signal.h	1.5"			*/

#ident	"%Z%uts/head:%M%	%I%"

/*	3.0 SID #	1.3	*/
#include<sys/signal.h>

extern	void(*signal())();
extern  void(*sigset())();
