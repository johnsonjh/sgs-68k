#ifndef SYS_GCTTY_H
#define SYS_GCTTY_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) gctty.h: version 2.1 created on 5/22/89 at 19:07:19	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)gctty.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
#ident "@(#)sys:gctty.h	1.15"
/*
 * A tty structure is needed for each UNIX character device that
 * is used for terminal IO.
 */
/*
 gh0 - 07/12/88 - add TXSTOP & THSTOP flags to t_gcp_state
 gh1 - 08-22-88 - add STREAM_OPEN & NON_STREAM_OPEN for streams
       09-22-88 - pad out gctty to 4-byte multiple for sparc - gil hunt 
*/

struct gctty
{
	unsigned char   t_eye_catcher[4];/* eye catcher                      */
	struct cirbuf   t_rawq;		/* raw input queue                   */
	struct cirbuf   t_canq;		/* canonical queue                   */
	struct cir_buf  t_outq;		/* output queue                      */
	struct cirbuf   t_tbuf[2];	/* tx control blocks                 */
	struct cirbuf   t_rbuf;		/* rx control block                  */
	struct cirbuf   t_icmd;		/* icb input command queue           */
	struct tcb      *t_input_tcb;	/* input tcb                         */
	struct tcb      *t_output_tcb;	/* output tcb                        */
	struct tcb      *t_control_tcb;	/* control tcb                       */
	unsigned char   *t_canwb;	/* iop's canon work buffer           */
	struct scc      *t_scc;		/* scc base address                  */
	unsigned short  t_iflag;	/* input modes                       */
	unsigned short  t_oflag;	/* output modes                      */
	unsigned short  t_cflag;	/* control modes                     */
	unsigned short  t_lflag;	/* line discipline modes             */
	short           t_state;	/* internal state                    */
	short           t_pgrp;		/* process group name                */
	unsigned short  t_gcp_state;	/* gcp state                         */
	unsigned short  t_lock_state;	/* locked state variables            */
	struct sema     t_lock;		/* lock semaphore                    */
	unsigned char   t_cc[NCC]; 	/* settable control chars            */
	struct termio   t_ioctl_set;	/* ioctl set                         */
	unsigned char   t_ioctl_cmd;	/* ioctl command                     */
	unsigned char   t_ioctl_arg;	/* ioctl argument                    */
	unsigned char   t_exst;		/* software copy external status     */
	char            t_line;		/* line discipline                   */
	char            t_term;		/* terminal type                     */
	char            t_tmflag;	/* terminal flags                    */
	char            t_col;		/* current column                    */
	char            t_row;		/* current row                       */
	char            t_vrow;		/* variable row                      */
	char            t_lrow;		/* last physical row                 */
	unsigned char   t_wr5;
	unsigned char   t_openf;	/* open flags                        */
	unsigned short  t_dlycnt;	/* number of delays pending          */
	unsigned short  t_closing;	/* wait for close                    */
	unsigned short  t_delct;	/* delimiter count                   */
	struct gctio    t_gctio;	/* extened gcp tty ioctl             */
	struct pto      t_pto_dcd;	/* dsr to dcd delay                  */
	struct pto      t_pto_active;	/* inactive delay                    */
	unsigned char   t_board;	/* board number                      */
	unsigned char   t_device;	/* device number                     */
	unsigned char   freetbuf;	/* index to t_buf for output task    */
	unsigned char   unused;		/* pad out for sparc		     */
};

#define TTYVERSION  0x31000000

#define GCP_STOM_CMD(cmd) icbcmd(cmd | tp->t_device);

#define GCTTY_CFLAG_MASK (CBAUD|CSIZE|CSTOPB|PARENB|PARODD)

/*
 *	locked state variables
 */

#define GCTTY_LS_IRDY	0x0001		/* input ready			*/
#define GCTTY_LS_ORDY	0x0002		/* output ready			*/
#define GCTTY_LS_CRDY	0x0004		/* control ready		*/

#define GCTTY_LS_SISLP	0x0010		/* slave input sleeping		*/
#define GCTTY_LS_SOSLP	0x0020		/* slave output sleeping	*/

#define GCTTY_LS_MISLP	0x0100		/* master input sleeping	*/
#define GCTTY_LS_MOSLP	0x0200		/* master output sleeping	*/
#define GCTTY_LS_MCSLP	0x0400		/* master control sleeping	*/

/*
 *	gcp state variables
 */

#define GCTTY_STATE_RTO 	0x0001	/* raw io time out		*/
#define GCTTY_STATE_DSR 	0x0002	/* waiting for dsr to drop	*/
#define GCTTY_STATE_BREAK 	0x0004	/* received break		*/
#define GCTTY_STATE_TXSTOP	0x0008	/* output stopped - X-OFF rcvd  */
#define GCTTY_STATE_THSTOP	0x0010	/* output stopped - CTS dropped */

/*
 *	slave to master commands
 */

#define GCTTY_STOM_STATE	(0x0100|GC_TTY_COMMANDS)
#define GCTTY_STOM_READ		(0x0200|GC_TTY_COMMANDS)
#define GCTTY_STOM_INTR		(0x0300|GC_TTY_COMMANDS)
#define GCTTY_STOM_QUIT		(0x0400|GC_TTY_COMMANDS)
#define GCTTY_STOM_SWITCH	(0x0500|GC_TTY_COMMANDS)
#define GCTTY_STOM_FLUSH	(0x0600|GC_TTY_COMMANDS)
#define GCTTY_STOM_IOCTL	(0x0700|GC_TTY_COMMANDS)
#define GCTTY_STOM_TXLOW	(0x0800|GC_TTY_COMMANDS)
#define GCTTY_STOM_HUP		(0x0900|GC_TTY_COMMANDS)
#define GCTTY_STOM_SWTCH	(0x0a00|GC_TTY_COMMANDS)
#define GCTTY_STOM_TACT		(0x0b00|GC_TTY_COMMANDS)

/*
 *	master to slave commands
 */

#define GCTTY_MTOS_OPEN		(0x0100|GC_TTY_COMMANDS)
#define GCTTY_MTOS_CLOSE	(0x0200|GC_TTY_COMMANDS)
#define GCTTY_MTOS_TIMEO	(0x0300|GC_TTY_COMMANDS)
#define GCTTY_MTOS_READ		(0x0400|GC_TTY_COMMANDS)
#define GCTTY_MTOS_UNBLOCK	(0x0500|GC_TTY_COMMANDS)
#define GCTTY_MTOS_IOCTL	(0x0600|GC_TTY_COMMANDS)
#define GCTTY_MTOS_WRITE	(0x0700|GC_TTY_COMMANDS)
#define GCTTY_STOS_TIMEO	(0x0a00|GC_TTY_COMMANDS)

/*
 *	task wait flags
 */

#define W_EVENT		0x0010
#define W_RX		0x0020
#define W_TX		0x0040
#define W_WORK		0x0080

/*
 *	t_openf flags (in addition to those defineded in file.h)
 */
/* gh1 */
#define	STREAM_OPEN	0x40	/* device open for streams     (e.g., tta01) */
#define	NON_STREAM_OPEN	0x80	/* device open for non-streams (e.g., tty01) */

#endif /* SYS_GCTTY_H */
