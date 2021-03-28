#ifndef SYS_GCP_TCB_H
#define SYS_GCP_TCB_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tcb.h: version 2.1 created on 5/22/89 at 19:07:07	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tcb.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)head/gcp:tcb.h	1.6"

struct tcb {
	struct tcb *	tcb_forward;		/* forward link pointer	*/
	struct tcb *	tcb_backward;		/* back link pointer	*/
	struct tcb *	tcb_event_wait;		/* event wait chain	*/
	int * 		tcb_stack;		/* current stack	*/
	unsigned short	tcb_wait;		/* wait flags		*/
	unsigned char	tcb_pri_max;		/* max priority		*/
	unsigned char	tcb_pri_min;		/* min priority		*/
	unsigned char	tcb_priority;		/* current priority	*/
	unsigned char	tcb_eidi;		/* disable depth	*/
	unsigned char	tcb_slice;		/* time slice		*/
};

#define W_SEMA		0x1000			/* semaphore wait id	*/
#define W_TEB		0x2000			/* timed event wait	*/
#define W_ICB		0x4000			/* timed event wait	*/
#define W_BLOCK		0x8000			/* wait forever		*/

#define STACK_SIZE	256

#define WAIT_EVENT(head, wait_flag, booster){\
	sys_tcb->tcb_event_wait = head;\
	head = sys_tcb;\
	WAIT(wait_flag, booster);\
}

#define WAIT(wait_flag, booster){\
	sys_tcb->tcb_wait |= wait_flag;\
	if ((sys_tcb->tcb_priority += booster) > sys_tcb->tcb_pri_max)\
		sys_tcb->tcb_priority = sys_tcb->tcb_pri_max;\
	else if (sys_tcb->tcb_priority < sys_tcb->tcb_pri_min)\
		sys_tcb->tcb_priority = sys_tcb->tcb_pri_min;\
	resched();\
}

#define WAIT_EIDI(wait_flag, booster){\
	sys_tcb->tcb_wait |= wait_flag;\
	if ((sys_tcb->tcb_priority += booster) > sys_tcb->tcb_pri_max)\
		sys_tcb->tcb_priority = sys_tcb->tcb_pri_max;\
	else if (sys_tcb->tcb_priority < sys_tcb->tcb_pri_min)\
		sys_tcb->tcb_priority = sys_tcb->tcb_pri_min;\
	ENABLE0();\
	resched();\
	DISABLE0();\
}

#define WAIT_EI(wait_flag, booster){\
	sys_tcb->tcb_wait |= wait_flag;\
	if ((sys_tcb->tcb_priority += booster) > sys_tcb->tcb_pri_max)\
		sys_tcb->tcb_priority = sys_tcb->tcb_pri_max;\
	else if (sys_tcb->tcb_priority < sys_tcb->tcb_pri_min)\
		sys_tcb->tcb_priority = sys_tcb->tcb_pri_min;\
	ENABLE0();\
	resched();\
}

#define RESCHED(n){\
	if ((sys_tcb->tcb_priority - n) < sys_tcb->tcb_pri_min)\
		resched();\
	else {\
		sys_tcb->tcb_priority -= n;\
		resched();\
		sys_tcb->tcb_priority += n;\
	}\
}

#define POST_EVENT(head){\
	DISABLE();\
	while (head) {\
		post(head);\
		head = head->tcb_event_wait;\
	}\
	ENABLE();\
}

#define POST(wait_flag, tcbp){\
	DISABLE();\
	if (tcbp->tcb_wait == wait_flag)\
		post(tcbp);\
	else\
		tcbp->tcb_wait &= ~wait_flag;\
	ENABLE();\
}

#endif /* SYS_GCP_TCB_H */
