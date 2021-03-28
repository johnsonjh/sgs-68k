/*	mon.h	1.6	*/


#ifndef MON_H
#ident	"@(#)mon.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define MON_H

struct hdr {
	char	*lpc;
	char	*hpc;
	int	nfns;
};

struct cnt {
	char	*fnpc;
	long	mcnt;
};

typedef unsigned short WORD;

#define MON_OUT	"mon.out"
#define MPROGS0	(2000)
#define MSCALE0	4
#define NULL	0

#endif /* MON_H */
