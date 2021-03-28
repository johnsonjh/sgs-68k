#ifndef SYS_TYD_SRAM_H
#define SYS_TYD_SRAM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) tyd_sram.h: version 24.1 created on 10/28/91 at 18:40:11	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)tyd_sram.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

#define NUM_SPM_TTYS	56		/* number of SPM tty ports */

/*
 *	layout of microsequencer (uSeq) ram
 */
/*---------------------------------------------------------------------------*/
struct ms_ctl
{
	unchar ms_status;   /*set by msq - status */
/*## copy status from SRAM to PM memory each clock tick */

	unchar ms_event;    /*set by msq - flags eia, input, overrun, 2nd event
			      clr by SPM - after processing event */
			/* set by SPM, clr by PM (in spm_mem struct) */
/*## copy event from SRAM to PM memory each clock tick */

	unchar ms_ctl_flag; /*set by SPM - control bits */
/*## set by SPM in response to cflag or iflag change */

	unchar ms_command;  /*set by SPM - commands
			      clr by msq - after command executed */
			/* set by PM, clr by SPM (in spm_mem struct) */

	unchar ms_in_limit; /*set by SPM - loc where input buffer almost full*/
	unchar ms_in_head;  /*set by msq - next loc in input buf msq puts data*/
	unchar ms_in_tail;  /*set by SPM - next loc in input buf SPM gets data*/
	unchar ms_out_head; /*set by msq - next byte in output buf to write*/
	unchar ms_out_tail; /*set by SPM - next loc in output buf to put data*/
	unchar ms_char;	    /*set by msq - received data character */
	unchar ms_spec_char;/*set by SPM - byte ms sends if MS_SEND_SPEC_CHAR*/
/*## copy from SRAM to PM when command is MS_SEND_SPEC_CHAR */
	unchar ms_start;    /*set by SPM - X-ON  character (same as out_start)*/
	unchar ms_stop;	    /*set by SPM - X-OFF character (same as out_stop)*/
	unchar ms_err;	    /*used by msq only - error status */
	unchar ms_err_count;/*used by msq only - error count for no error */
	unchar dev;	    /* minor device no. for debugging dumps */
	uint   cflag;	    /*## copy of c_cflag */
	uint   iflag;	    /*## copy of c_iflag */
	unchar  eia_stat;	/* copied from msq to PM; DSR, DCD, etc */
	unchar break_duration;	/* duration of break in clock ticks */
	unchar unused[6];	/* keep total struct size at 32 bytes */
};/*--------------------------------------------------------------------------*/
typedef struct ms_ctl ms_ctl_t;
/* unchar ms_spec_tbl[32]		set by SPM - character recognition table */
/*---------------------------------------------------------------------------*/
/*	ms_command values - set by SPM */

#define	MS_FLUSH_XMIT_BUF	1	/* set output buf head ptr = tail ptr */
/*## set head & tail pointer equal in PM ram
  ## set same bit in SRAM struct  */

#define	MS_SEND_SPEC_CHAR	2	/* if set, send byte in special_char */
/*## copy special character from PM struct to SRAM struct
  ## set same bit in SRAM struct */

/* NEW: special PM-SPM interface bit - not valid in mseq RAM */
#define	MS_FLUSH_RCV_BUF	4	/* set input buf head ptr = tail ptr */
/*## set head & tail pointer equal in PM ram
 *## set same bit in SRAM struct  */
#define MS_NEW_FLAG		8	/*## if set, cflag or iflag changed */

#define	MS_STOP_OUTPUT		0x10	/* if set, stop sending data */
/*## set same bit in SRAM struct */

#define	MS_START_OUTPUT		0x20	/* if set, start sending data */
/*## set same bit in SRAM struct
  ## turn on transmitter */
#define MS_START_BREAK		0x40	/*## tell SPM to start a break */
#define MS_STOP_BREAK		0x80
/*---------------------------------------------------------------------------*/
/*	ms_status values - set by uSeq */

#define MSS_OUTPUT_PENDING	1	/* set when output buffer is non-empty*/
#define MSS_INPUT_STOPPED	2	/* set when input limit reached and
					   x-off is sent to the SPM */
#define MSS_OUTPUT_STOPPED	4	/* set if output stopped because either
					   x-off received or driver said stop */
#define	MSS_SEND_STOP		0x10	/* if set, send ms_stop character */
#define	MSS_DISABLED		0x80	/* if set, no ace board for this port */
/*---------------------------------------------------------------------------*/
/*	ms_control_flag values - set by SPM */

#define	MS_IGNBRK		1	/* ignore breaks from terminal */
#define	MS_BRKINT		2	/* signal drivr if break from terminal*/
#define	MS_IGNPAR		4	/* ignore parity errors from terminal */
#define	MS_PARMRK		8	/* mark parity errors */
#define	MS_INPCK		0x10	/* enable input parity check */
#define	MS_ISTRIP		0x20	/* mask X-ON & X-OFF with 0x7f */
#define MS_IXANY		0x40	/* any char restarts suspended output */
#define MS_IXOFF		0x80	/* send X-OFF if input buffer full */
/*---------------------------------------------------------------------------*/
/*	ms_event flag - set by uSeq *//* set by SPM in kmem struct */

#define MSE_OVERRUN		1	/* set when input buffer wraps */
/* NEW: special flag for SPM-PM interface, not set in uSeq ram */
#define MSE_INPUT_STOPPED	4	/* set if X-OFF sent */
#define MSE_INPUT_STARTED	8	/* set if X-ON  sent */
#define MSE_OUTPUT_STOPPED	0x10	/* set if X-OFF received */
#define MSE_OUTPUT_START_TRY	0x20	/* set if X-ON received (or any char
				if IXANY) and output stopped used to ask SPM
				driver to tell uSeq to start up output */
#define MSE_INPUT_PENDING	0x40	/* set when data in input buffer */
#define MSE_BREAK		0x80	/* set if break occurred */
/*---------------------------------------------------------------------------*/
#define	MS_BUF_SIZE		256	/* size of input and output data bufs*/
#define	MS_HALF_BUF_SIZE	128	/* size of half of input or output buf*/
#define	MS_LIMIT_SIZE		200	/* dont fill output buf up all the way*/
/*---------------------------------------------------------------------------*/

typedef struct {
	caddr_t		tty_buffer_space;
	struct  ms_ctl	spm_tty_ctl[NUM_SPM_TTYS];
} spm_tty_t;

#endif	/* SYS_TYD_SRAM_H */
