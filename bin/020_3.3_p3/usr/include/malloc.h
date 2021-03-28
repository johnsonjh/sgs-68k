/*	malloc.h	1.2	*/


#ifndef MALLOC_H
#ident	"@(#)malloc.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define MALLOC_H

/*
	Constants defining mallopt operations
*/
#define M_MXFAST	1	/* set size of blocks to be fast */
#define M_NLBLKS	2	/* set number of block in a holding block */
#define M_GRAIN		3	/* set number of sizes mapped to one, for
				   small blocks */
#define M_KEEP		4	/* retain contents of block after a free until
				   another allocation */
/*
	structure filled by 
*/
struct mallinfo  {
	int arena;	/* total space in arena */
	int ordblks;	/* number of ordinary blocks */
	int smblks;	/* number of small blocks */
	int hblks;	/* number of holding blocks */
	int hblkhd;	/* space in holding block headers */
	int usmblks;	/* space in small blocks in use */
	int fsmblks;	/* space in free small blocks */
	int uordblks;	/* space in ordinary blocks in use */
	int fordblks;	/* space in free ordinary blocks */
	int keepcost;	/* cost of enabling keep option */
};	

#if defined _SYSV
char *malloc();
char *realloc();
#else
void *malloc();
void *realloc();
#endif	/* _SYSV */
void free();
int mallopt();
struct mallinfo mallinfo();

#endif /* MALLOC_H */
