#ifndef SYS_GCP_GCM4_H
#define SYS_GCP_GCM4_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gcm4.h: version 2.1 created on 5/22/89 at 19:06:25	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gcm4.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#ident	"@(#)uts/head/gcp:gcm4.h	2.1"

define(`UNIQA',50000)dnl
define(`UNIQB',20000)dnl

define(ENABLE,
`{	asm("	tst.b	eidi");
	asm("	beq	L%UNIQA");
	asm("	sub.b	&1,eidi");
	asm("	bne	L%UNIQB");
	asm("L%UNIQA:");
	asm("	and.w	&0xf8ff,%sr");
	asm("L%UNIQB:");
	define(`UNIQA',decr(UNIQA))dnl
	define(`UNIQB',decr(UNIQB))dnl
}')

define(ENABLE0,
`{	asm("	mov.b	&0,eidi");
	asm("	and.w	&0xf8ff,%sr");
}')

define(DISABLE,
`{	asm("	or.w	&0x0700,%sr");
	asm("	add.b	&1,eidi");
}')

define(DISABLE0,
`{	asm("	or.w	&0x0700,%sr");
	asm("	mov.b	&1,eidi");
}')

define(BLD_TTY,
bldtty(		$1,		/* tty number */
		$2,		/* tty pointer */
		rbuf$1,		/* rx buffer */
		tbuf$1,		/* tx buffer */
		rawq$1,		/* raw queue */
		canb$1,		/* canon work buffer */
		&tcbc$1,	/* control processors tcb */
		&tcbi$1,	/* input processors tcb */
		&tcbo$1,	/* output processors tcb */
		$3,		/* scc base address */
		$4,		/* scc interrupt vector */
		$5		/* initial io state */
))

/*******************************************************************
 *
 *	T  C  B    M A C R O
 *
 *	input parameters:
 *		1.  tcb number
 *		2.  tcb name
 *		3.  forward tcb chain pointer
 *		4.  backward tcb chanin pointer
 *		5.  dispatch priority level
 *		6.  time slice
 *		7.  entry address
 *
 *******************************************************************/

define(TCB,
extern int stack$1[];
static char tcbec$1[]="TCB$1";
`struct tcb tcb$2={	$3,		/* forward chain pointer */
			$4,		/* backward chain pointer */
			0,		/* cblock wait chain */
			0,		/* tty control block */
			$7,		/* entry address */
			stack$1+STKLEN,	/* top of stack */
			stack$1+STKLEN-12, /* current stack */
			$1,		/* tcb number */
			$5,		/* dispatch prioity level */
			0,		/* wait flags */
			1,		/* disable depth */
			$6		/* time slice */
}')

define(BUF,
extern unsigned char $2[];
`struct cirbuf $1={	BUFEMPTY,	/* buffer flags */
			0,		/* character count */
			$2,		/* head of queue */
			$2,		/* end of queue */
			$2,		/* buffer base */
			$2+$3		/* buffer end */
}')

define(GCP_ICB_CMD, icboreq($1 | (tp->t_number << 8) | 0x4000))
define(GCP_LP_ICB_CMD, icboreq($1 | 0x4000))

define(WAIT,
`{cvt.tcb->wait |= $1;
	ttydisp();
}')

define(ENABLE_WAIT_DISABLE,
`{cvt.tcb->wait |= $1;
	ENABLE();
	ttydisp();
	DISABLE();
}')

define(ENABLE0_WAIT_DISABLE0,
`{cvt.tcb->wait |= $1;
	ENABLE0();
	ttydisp();
	DISABLE0();
}')

define(ENABLE_WAIT,
`{cvt.tcb->wait |= $1;
	ENABLE();
	ttydisp();
}')

define(ENABLE0_WAIT,
`{cvt.tcb->wait |= $1;
	ENABLE0();
	ttydisp();
}')

define(POST, $2->wait &= ~$1)

define(TRIGGER,
`{
	int trigger$1;
	if (trigger$1);
}')

/*****************************************************************
 *
 *	B Y T E   O U T
 *
 *	Get a byte from a "cirbuf"
 *
 *	input parameters:
 *		1.  buffer pointer
 *		2.  the byte
 *
 *	output parameters:
 *		2.  -1 if the buffer is empty
 *
 *****************************************************************/

define(BYTEOUT,
`{
	if(($1->hoq == $1->eoq) && !$1->count)
		$2 = -1;
	else {
		$2 = *$1->hoq++;
		if ($1->hoq == $1->end)
			$1->hoq = $1->base;
		$1->count--;
	}
}')

define(BYTE_OUT,
`{
	if ($1->cb_eoq == $1->cb_hoq)
		c = -1;
	else {
		c = *$1->cb_hoq++;
		if ($1->cb_hoq == $1->cb_end)
			$1->cb_hoq = $1->cb_base;
	}
}')

/*****************************************************************
 *
 *	B Y T E   I N
 *
 *	Place a byte into a "cirbuf"
 *
 *	input parameters:
 *
 *		1.  buffer pointer
 *		2.  the byte
 *		3.  return code
 *			1  -  if successful
 *			0  -  if buffer is full
 *
 *****************************************************************/

define(BYTEIN,
`{
	if(($1->hoq == $1->eoq) && $1->count)
		$3 = 0;
	else {
		*$1->eoq++ = $2;
		if ($1->eoq == $1->end)
			$1->eoq = $1->base;
		$1->count++;
		$3 = 1;
	}
}')

/*********************************************************
 *
 *	I C B   I N
 *
 *********************************************************/

define(ICBIN,
`{
	DISABLE0();
	do {
		BYTE_OUT(outq_bp);
		if ( c >= 0)
			break;
		outq_bp->cb_out_flag = 1;
		if (outq_bp->cb_in_flag)
			outq_start(tp);
		if (outq_bp->cb_hoq == outq_bp->cb_eoq)
			ENABLE0_WAIT_DISABLE0(W_ICB);
		outq_bp->cb_out_flag = 0;
	} while (1);
	if (outq_bp->cb_in_flag)
		outq_start(tp);
	ENABLE0();
}')

define(CONOUT,
`{
	DISABLE0();
	do {
		BYTEIN(tbuf_bp, $1, rc);
		if (rc)
			break;
		ENABLE0_WAIT_DISABLE0(W_TX);
	} while (1);
	if (!(tp->t_state&(BUSY|TIMEOUT|TTSTOP))) {
		tp->t_state |= BUSY;
		tp->t_scc->reg[8].reg = byteout(tbuf_bp);
	}
	ENABLE0();
}')

/*********************************************************
 *
 *	R A W   I N
 *
 *	get a byte from the raw queue
 *
 *	input parameters:
 *		1.  raw buffer pointer
 *		2.  the byte
 *
 *********************************************************/

define(RAWIN,
`{
	DISABLE0();
	BYTEOUT($1, $2);
	ENABLE0();
	if (tp->t_state&TBLOCK)
		txstart(tp);
}')

/*********************************************************
 *
 *	R A W   O U T
 *
 *	Put a byte into the raw queue
 *
 *	input parameters:
 *		1.  raw buffer pointer
 *		2.  the byte
 *		3.  return code
 *			1  -  if successful
 *			0  -  if buffer is full
 *
 *********************************************************/

define(RAWOUT,
`{
	do {
		DISABLE0();
		BYTEIN($1, $2, $3);
		ENABLE0();
		if ($3)
			break;
		while (tp->t_rawq.count > tp->t_rawq.low_water) {
			if (tp->t_canq.count)
				delay(100);
			else
				canon(tp);
		}
	} while (1);
}')

#endif /* SYS_GCP_GCM4_H */
