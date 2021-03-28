#ident	"@(#)proto.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/* function prototypes */

#define PNULL (t_prototype *) 0
		/* constants for parameter list header flags */
#define PVARARGS 0100				/* variable # args */
#define PDECLARE 0200				/* declared, vs defined, prototype */
#define PVOID    0400				/* function prototyped as void */
#define WARNED (unsigned short) (~0)	/* no prototype && already warned */
#define HASPROTO(x)		( x != 0 && x != WARNED )

typedef struct s_prototype {		/* prototype info (per parameter) */
	TWORD	ptype;
	short	pdim;					/* to check array types */
	short	psiz;					/* to check structure/union types */
	int 	psym;					/* symtab indx for optional id */
	int 	pnest;					/* nested prototype list */
	TWORD 	pqual;					/* to save ftnarg's qualifiers */
	char 	pclass;  				/* to save ftnarg's storage class */
	struct s_prototype *pnext;		/* next parameter type on list */
	struct s_prototype *pprev;		/* previous parameter type on list */
} t_prototype;

typedef struct s_protolist {		/* parameter type list header structure */
	t_prototype *lhead;				/* pointer to top of list */
	int lsymidx;					/* index of function symbol in stab */
	int lptr2ftn;					/* for ftn returning ptr to ftn */
	int lcnt;						/* parameter count */
	int lflag;						/* special flags */
} t_protolist;

extern t_protolist proto_lists[];	/* headers of prototype lists */
