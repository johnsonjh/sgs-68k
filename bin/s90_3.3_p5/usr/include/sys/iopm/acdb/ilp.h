/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ilp.h: version 24.1 created on 10/28/91 at 17:38:58	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ilp.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)ilp.h   24.1"
/*
 *	ilp.h - header file for streams lp driver on the ASYNC IOPM board
 *
 *	gil hunt - April 1, 1989
 */
struct ilp
{
	unchar	myname[4];
	queue_t	*rq;		/* addr of read queue for upstream */
	queue_t	*wq;		/* queue for downstream */
	mblk_t	*first_mp;	/* addr of first msg to be moved to print buf*/
	mblk_t	*last_mp;	/* addr of last msg to be moved to print buf*/
	mblk_t	*delay_msg;	/* msg to be processed after output complete */
	int	output_empty_id;/* timer id - waiting for output to empty */
	int	cancel_id;	/* timer id - cancel wait for output to empty */
	int	output_in_progress; /* non-zero while dma output in progress */
	int	bytes_so_far;	/* no. bytes ready to be moved to print buf */
	int	state;		/* current state flags */
	int	max_size;	/* max no. bytes allowed in output buffer */
	unchar	*ms_control_reg; /* addr of control reg this lp */
	unchar	*ms_status_reg;	/* addr of status register this lp */
	unchar	*ms_clear_done; /* addr of 'clear done' this lp */
	unchar	*end_of_lp_buf_hi; /* addr of end of lp buf this lp */
	dev_t	t_device;	/* device number passed to open routine */
	ushort	t_openf;	/* type of open - info only */
	ushort	t_closing;	/* non-zero if in middle of close */
	ushort	t_pgrp;
	ushort	timer_enable;	/* indicates which timers are running */
	unchar	min_dev;	/* minor device on board*/
	unchar	status;		/* copy of printer status */
};
typedef struct ilp ilp_t;
/*--------------------------------------------------------------------*/
/*	t_state flags */
#define	ISOPEN		4		/* Device is open */
#define	TTUSE		0x20		/* set if port opened */
#define	NO_OUTPUT_ALLOWED 0x2000	/* trying to empty out buf in sram*/
/*--------------------------------------------------------------------*/
/*	timer_enable flags */
#define	CANCEL_CHECK_TIMER	4	/* cancel check for output to empty */
#define	OUTPUT_EMPTY_TIMER	8	/* wait for output in sram to empty*/
/*--------------------------------------------------------------------*/
