
/*	START NEW ARIX SCCS HEADER			*/
/*							*/
/*	@(#) ptem.h: version 2.1 created on 5/22/89 at 19:09:44	*/
/*							*/
/*	Copyright (c) 1988 by Arix Corporation		*/
/*	All Rights Reserved				*/
/*							*/
#ident	"@(#)ptem.h	2.1	5/22/89 Copyright (c) 1988 by Arix Corporation"
/*							*/
/*	END NEW ARIX SCCS HEADER			*/
/*							*/
/*
 * Windowing structure to support JWINSIZE/TIOCSWINSZ/TIOCGWINSZ 
 */
struct winsize {
	unsigned short ws_row;       /* rows, in characters*/
	unsigned short ws_col;       /* columns, in character */
	unsigned short ws_xpixel;    /* horizontal size, pixels */
	unsigned short ws_ypixel;    /* vertical size, pixels */
};

/*
 * The ptem data structure used to define the global data
 * for the psuedo terminal emulation  streams module
 */
struct ptem
{
	unsigned short cflags;	/* copy of c_cflags */
	unsigned short state;	/* state of ptem entry, free or not */
	mblk_t *dack_ptr;	/* pointer to preallocated message block used to ACK disconnect */
	queue_t *q_ptr;		/* pointer to the ptem read queue */
	struct winsize wsz;	/* struct to hold the windowing info. */
	short ttypgid;		/* process group for controlling tty */
};
/*
 * state flags
 * if flag is zero then free then ptem entry is free to be allocated
 */
#define INUSE 	0x1
#define RDSIDE  0x2
#define WRTSIDE  0x4
