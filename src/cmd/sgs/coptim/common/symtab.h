#ident	"@(#)symtab.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
/*	@(#)symtab.h	7.1		*/
/*	Copyright 1985 Motorola Inc.	*/

/* flag settings for opn.flags */
#define SIMPLE	1
#define ARRAY	2
#define LCSEFLAG 4
#define UNIQUE	8

/* flag settings for symble table */
#define ADDRESSTAKEN 1
#define USED	2
#define COMPLEX	4

#define ISSIMPLE( p ) ( p->opn.flags & SIMPLE && ( ! ((symtab + p->opn.sourceno)->flags & ADDRESSTAKEN)))

#define ISCOMPLEX(p) ( p->opn.sourceno\
		     && p->opn.parent\
	 	     && ((    p->opn.type != INT\
			      && p->opn.type != UNSIGNED\
	 		      && p->opn.type != LONG\
			      && p->opn.type != ULONG\
			 )\
			 ||(  optype(p->opn.parent->opn.op) == BITYPE\
			   && p->opn.parent->in.right->in.op == CONV\
			   && (p->opn.parent->in.right->in.left->in.type==FLOAT\
			      || p->opn.parent->in.right->in.left->in.type==DOUBLE\
			   )  )\
			 || p->opn.flags & (LCSEFLAG|UNIQUE)\
			 || (   p->opn.parent->opn.op != ASSIGN\
				&& p->opn.parent->opn.op != PLUS\
				&& p->opn.parent->opn.op != ASG PLUS\
				&& p->opn.parent->opn.op != MINUS\
				&& p->opn.parent->opn.op != ASG MINUS\
				&& p->opn.parent->opn.op != INCR\
				&& p->opn.parent->opn.op != DECR\
				&& !( logop(p->opn.parent->opn.op)\
			  	    && p->opn.parent->opn.op != NOT\
	   			    && p->opn.parent->opn.op != ANDAND\
	   			    && p->opn.parent->opn.op != OROR\
	   		)  )   )    )

/* The following data structure, SYMENT (SYMbol table ENTry), is used to 
   describe the declaration of a simple local variable or parameter.   
   The symbols are identified in the Ctrees by VAUTO or PARAM
   nodes whose offsets match a offset in the symbol table. The "sourceno"
   associated with each Ctree node will be the index into the symbol
   table. 
   The symbols declared within the same scope are linked together in
   a linked list order from the positive (parameters) to negative (locals)
   offset.
*/                        

typedef struct Tlevel SCOPE;
typedef struct syment SYMENT;

struct Tlevel
   {                 
   struct Tlevel *higher;       /* pointer to higher scope level */
   struct Tlevel *same;         /* pointer to same scope level */
   struct Tlevel *lower;        /* pointer to lower scope level */
   int scopehead;               /* index into symbol table of first variable
                                ** declared in this block
                                */
   SYMENT *highsym;		/* highest used variable (or struct) in scope */
   int maxoffset;               /* max offset of VAUTOs in block */
   int regexport;               /* exported register mask */
   int regavail;                /* available register mask */
   NODE *sblock;                /* pointer to '[' node which starts block */
   NODE *eblock;                /* pointer to ']' node which ends block */
   int qnode;                   /* offset of temporary used as qnode */
   };

typedef struct syment
   {
   int offset;		/* stack offset for a simple variable in bytes */
   int newoffset;	/* stack offset for a variable in bytes */
   int size;		/* size of variable in bytes */
   short flags;		/* bit flags */
   char *name;		/* name of variable - for debug purposes only*/
   TWORD type;          /* type of variable */
   char	 stypeq;	/** type qualifier information **/
   char sclass;		/* storage class */
   int reg;		/* bit mask designating register used by variable */ 
   int weight;		/* dependent upon static usage and loop depth */
   int frmwgt;		/* weighted frame usage of all local variables including
			 * Structures, Unions, and Enumerations.
			 * dependent upon static usage and loop depth */
   ENTRY *valueno;	/* current value number for variable */
   int scopendx;	/* index of next variable in same scope level */
   SCOPE *scope;	/* pointer to scope where the variable was declared */
   NODE  *p;		/* pointer to a node for a constant (ICON or FCON) */
   };

typedef struct Tsorted
   {
   int weight;		/* weight of the variable */
   int ndx;		/* index into the symbol table */
   } SORTED;

