/*	%W%	*/
/*	mon.h	1.6	*/

#ident	"%Z%uts/head:%M%	%I%"

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
