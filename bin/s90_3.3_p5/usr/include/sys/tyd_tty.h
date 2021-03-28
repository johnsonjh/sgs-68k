#ifndef	SYS_TYD_TTY_H
#define	SYS_TYD_TTY_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tyd_tty.h: version 24.1 created on 10/28/91 at 18:40:14	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tyd_tty.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *	tyd_tty.h - header file for streams tty driver
 *
 *	gil hunt - Nov 14, 1988
 *	gil hunt - Nov 27, 1989 - added private stats
 */
/*--------------------------------------------------------------------*/
struct tyd_rstat		/* private stats for read side */
{
	int	rcvin;		/* input character count */
	int	mdmin;	/* modem changes */
};
typedef struct tyd_rstat tyd_rstat_t;

struct tyd_wstat		/* private stats for write side */
{
	int	xmtin;		/* output char count, received from upstream */
};
typedef struct tyd_wstat tyd_wstat_t;
/*--------------------------------------------------------------------*/
struct tyd_tty
{
	unchar	myname[4];
	queue_t	*rq;		/* addr of read queue for upstream */
	queue_t	*wq;		/* queue for downstream */
	mblk_t	*delay_msg;
	struct module_stat *rq_stat; /* addr of general read side stats */
	struct module_stat *wq_stat; /* addr of general write side stats */
	int	act_id;		/* timer id - no activity */
	int	dcd_id;		/* timer id - waiting for DCD to go hi */
	int	dsr_id;		/* timer id - waiting for DSR to go lo */
	int	output_empty_id;/* timer id - waiting for output to empty */
	int	cancel_id;	/* timer id - cancel wait for output to empty */
	int	out_pending_id;	/* timer id - wait for output to half empty */
	int	timer_step_id;	/* timer id - delaying for a while */
	int	break_interval;	/* no. of milliseconds to BREAK */
	int	total_delay;
	int	state;	/* current state flags */
	struct	ms_ctl  *sram_ctl; /* input control tbl in sram */
	unchar	*sram_in_base;  /* addr of input buffer in s-ram for data */
	unchar	*sram_out_base; /* addr of output buffer in s-ram for data */
	unchar	*sram_spec_tab;	/* addr of 32-byte special char table in s-ram*/
	unchar	*uartptr;	/* addr of uart this device */
	unchar	*octart_dtr_reg;/* addr of octart dtr register for port */
	ushort	ms_out_max;	/* max no. bytes allowed in sram output buf */
	ushort	timer_enable;	/* indicates which timers are running */
	ushort	t_openf;	/* type of open - info only */
	ushort	t_closing;	/* non-zero if in middle of close */
	ushort	t_pgrp;
	unchar	min_dev;	/* minor device on board*/
	unchar	xofc;		/* default X-OFF character */
	unchar	xonc;		/* default X-ON character */
	unchar	eia_stat;	/* hardware status of uart, includes DSR & DCD*/
	unchar	dtr_reg_mask;	/* bit to toggle in dtr reg for this device */
	unchar	mode_1;		/* copy of first uart mode register */
	unchar	mode_2;		/* copy of second uart mode register */
	unchar	cmd_reg;	/* copy of uart command register */
	unchar	ms_iflag;	/* copy of ms_ctl_flg for debugging */
	unchar	output_in_progress;	/* non-zero if output not complete */
	dev_t	t_device;	/* device number passed to open routine */
	dev_t	unused;		/* pad out to 'int' in case dev_t is a short */
	struct	termio termio;	/* termio flags (see usr/include/sys/termio.h)*/
	struct	gctio t_gctio;	/* dcd and activity timeout struct */
	unchar	modpush;	/* request module push		*/
	struct	sakioctl sak;	/* secure attention key stuff */
	int	sak_count;	/* how many 'sak' keys received so far */
	tyd_rstat_t pv_rstat;	/* private stats for read side */
	tyd_wstat_t pv_wstat;	/* private stats for write side */
};
typedef struct tyd_tty tyd_tty_t;
/*--------------------------------------------------------------------*/
/*	state flags */
#define	CARR_ON			1	/* either DSR & DTR or CLOCAL */
#define	WOPEN			2	/* Waiting for open to complete */
#define	ISOPEN			4	/* Device is open */
#define	TBLOCK			8	/* input stopped by TCXONC ioctl */
#define	T_RXSTOP		0x10	/* input stopped by X-OFF to device*/
#define	TTUSE			0x20	/* set if port opened & stp allocated*/
#define	T_DTR_SET		0x40	/* DTR is set in octart DTR register*/
#define	T_TXSTOP		0x100	/* Output stopped by X-OFF from device*/
#define	T_SUSPEND		0x200	/* Output stopped by ioctl from user */
#define T_DCD			0x400	/* data carrier detect (DCD) */
#define T_DSR			0x800	/* data set ready (DSR)*/
#define	BREAK_IN_PROGRESS 	0x1000	/* break being sent to device */
#define	NO_OUTPUT_ALLOWED 	0x2000	/* trying to empty out buf in sram*/
#define	WAITING_FOR_MSG		0x4000	/* waiting for bufcall response */
#define T_NON			0x8000	/* notify on connect bit */
#define	WAIT_FOR_FLAG_ON_OPEN	0x10000	/* wait in open for spm to see flags*/
#define	WAIT_FOR_FLAG_ON_IOCTL	0x20000	/* wait for spm to process new cflag and
					   iflag before acking M_IOCTL*/
/*--------------------------------------------------------------------*/
/*	timer_enable flags */
#define	ACT_TIMER		1	/* input/output activity */
#define	DCD_TIMER		2	/* DCD hi */
#define	DSR_TIMER		4	/* DSR lo */
#define	OUTPUT_EMPTY_TIMER	8	/* wait for output in sram to empty*/
#define	OUTPUT_PENDING_TIMER	0x10	/* wait for output to half empty*/
#define	CANCEL_CHECK_TIMER	0x20	/* cancel wait for output to empty */
#define	STEP_TIMER		0x80	/* delaying for a bit */
/*--------------------------------------------------------------------*/
/*	modpush flags	*/
#define	LDTERM		1	/* ldterm module	*/
/*--------------------------------------------------------------------*/
#endif	/* SYS_TYD_TTY_H */
