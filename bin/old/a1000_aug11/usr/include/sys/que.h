#ifndef SYS_QUE_H
#define SYS_QUE_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) que.h: version 2.1 created on 5/22/89 at 19:09:50	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)que.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	ATT: #ident	"kern-port:sys/que.h	10.3"		*/

#ident	"@(#)uts/head/sys:que.h	2.1"

/*********************************************************************

	following structures declares the data structures used for
	queue manipulation.

*********************************************************************/

/*****

	Queues consist simply of one-way list.  Each queue is
	defined by a 'queue control element' made up of head
	and tail pointers.  Each item held on a queue contains
	a pointer word used to link successive item together.
	The head pointer contains the address of the first
	item in the queue and the tail pointer contains the
	address of the last item.

*****/



/*
 *	Queue Item
 */

#define	QITEM	struct qitem_st

struct	qitem_st
	{
	QITEM	*qi_next;	/* next queue item pointer */
	char	data[1];	/* data */
	};



/*
 *	Queue Control Element
 */

#define	QCTRL	struct qctrl_st

struct	qctrl_st
	{
	QITEM	*qc_head;		/* head pointer */
	QITEM	*qc_tail;		/* tail pointer */
	};






#endif /* SYS_QUE_H */
