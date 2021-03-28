#ident	"@(#)decls.c	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "decls.h"
char * dummy_name_1;
char * utilname;	/* name of argv[0], the utility name */
char * infile;		/* the input file name */
char * outfile = "new.obj";/* the output file name,default new.obj */
FILE * dummy_name_2x;
FILE * symp;		/* pointer to symbol table in infile */
FILE * strp;		/* pointer to symbol string table in infile */
struct symelt * head;		/* pointer to head of linked list of syms */
struct symelt * dummy_name_3xxx;
struct symelt   dummy_name_4;
struct symelt * runner;	/* pointer to some symbol element in list */
long symptr;		/* pointer to symbol table in input file */
long   dummy_name_5xx;
long nsyms;		/* number of symbols in the input file */
long strtab;		/* beginning of string table */
long strtabexist = 0;/* nonzero if a stringtable exists */
struct filehdr aheader; 	/* the a.out header */
unsigned short localmagic;	/* holds new magic number for -m */
struct scnhdr sheader;	/* object section headers */
struct reloc reloc;	/* relocation cell */
 char   dummy_name_6xx;
char dflag=0;		/* debug flag */
char mflag=0;	/* change magic number to localmagic */
char nflag=0;	/* no write, print out symbol changes only */
char iflag=0;	/* interactive flag */
char pflag=0;	/* print only flag */
char rflag=0;	/* change relocation information only */
char vflag=0;		/* verbose flag */
char wflag=0;	/* suppress warning messages */
char buf[1025];		/* temporary storage buffer for new name */
long maxname = 8;

long conflict;
