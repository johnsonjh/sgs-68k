#ifndef	SYS_LPM_H
#define	SYS_LPM_H

/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) lpm.h: version 24.1 created on 10/28/91 at 18:35:15	*/
/*							*/
/*	Copyright (c) 1990 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)lpm.h	24.1	10/28/91 Copyright (c) 1990 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/

/*
 *	lpm.h - struct for lpmod.c - stream printer module
 *
 *	gil hunt - 10/16/89
 */

#define MAX_OUT_SIZE	200	/* max size of output line */
struct lpm
{
	unchar	myname[4];
	queue_t	*rq;		/* addr of read queue for upstream */
	queue_t	*wq;		/* queue for downstream */
	mblk_t	*bp_lp;		/* msg containing lp struct*/
	mblk_t	*out_bp;	/* msg for output if 'allocb' returns NULL */
	int	state;		/* current state flags */
	int	col_count;	/* software col count */
	int	line_count;	/* line count */
	dev_t	t_device;	/* device number passed to open routine */
	ushort	min_dev;	/* minor device number */
	ushort	ind;		/* indentation for each line */
	ushort	max_col;	/* col after which no more data is printed */
	ushort	line;		/* line after which a FF is output. 0 = no FF */
	ushort	flag;		/* see 'sys/lprio.h' for flags (e.g., CAP) */
	int	out_index;	/* pointer into outbuf */
	unchar	outbuf[MAX_OUT_SIZE]; /* temp outbuf buffer until entire
					 line accumulated */
};
/*--------------------------------------------------------------------*/
#define WAITING_FOR_MSG		1	/* waiting for msg to become available*/
/*--------------------------------------------------------------------*/
#define	LP_FORM		0xc		/* top of form character */
/*--------------------------------------------------------------------*/
/*	types of characters that can be sent downstream
 *
 *	the following values reside within the output character table
 *	and are referenced by indexing into this table
 *	using the value of the received character as the index
 */
#define LPM_Ox08	2	/* Backspace */
#define LPM_Ox09	3	/* tab */
#define LPM_Ox0a	4	/* line feed */
#define LPM_Ox0c	6	/* form feed */
#define LPM_Ox0d	7	/* carriage return */
#define LPM_Oblank	8	/* blank */
#define LPM_Oa_z	9	/* Map lower to upper case if CAP set*/
#define LPM_Ox60	11	/* map '`' to "'-"	   if CAP set */
#define LPM_Ox7b	12	/* map '{' to "(-" 	   if CAP set */
#define LPM_Ox7c	13	/* map '|' to "!-" 	   if CAP set */
#define LPM_Ox7d	14	/* map '}' to ")-" 	   if CAP set */
#define LPM_Ox7e	15	/* map '~' to "^-" 	   if CAP set */
/*--------------------------------------------------------------------*/
#endif	/* SYS_LPM_H */
