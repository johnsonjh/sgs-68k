#ident	"@(#)decls.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#include "switches.h"
#include <stdio.h>
extern char * utilname;	/* name of argv[0], the utility name */
extern char * infile;	/* the input file name */
extern char * outfile;	/* the output file name,default new.obj */
extern FILE * symp;	/* pointer to symbol table in infile */
extern char * malloc();
extern FILE * strp;	/* pointer to symbol string table in infile */
extern long symptr;	/* pointer to symbol table in input file */
extern long nsyms;	/* number of symbols in the input file */
extern long strtab;	/* beginning of string table */
extern long strtabexist;/* nonzero if a stringtable exists */
extern char *newtbl[];	/* table of new reserved names */
extern char *oldtbl[];	/* table of old reserved names */
extern char *dirtbl[];	/* table of direcitive reserved names */
#include <a.out.h>
extern struct filehdr aheader; /* the a.out header */
extern unsigned short localmagic; /* holds the new magic number */
extern struct scnhdr sheader;	/* object section headers */
extern struct reloc reloc;	/* relocation cell */
extern char dflag;
extern char mflag;	/* change magic number to localmagic */
extern char nflag;	/* no write, print out symbol changes only */
extern char iflag;	/* interactive flag */
extern char pflag;
extern char rflag;	/* change relocation information only */
extern char wflag;	/* suppress warning messages */
extern char vflag;
extern char buf[];		/* temporary storage buffer for new name */

extern long maxname ;

struct symelt {
	struct syment sym;		/* the symbol info from file */
	char * sname;			/* ptr to sym name if >8 char */
	long	 adj;			/* the adj to string table */
	long namelen;			/* the length of the string */
	int entrynum;			/* spot in symtable for ent */
	char auxent;			/* 1 if is auxiliary entry */
	struct symelt * next;
	};
typedef struct symelt SYMELT ;
extern SYMELT * head;	/* pointer to head of linked list of syms */
extern SYMELT * runner;	/* pointer to some symbol element in list */
/* macaroni */
#ifdef ISMAGIC
#undef ISMAGIC
#endif /* ISMAGIC */
#define ISMAGIC(x)	( (x.f_magic == 0575 ) \
			||(x.f_magic == 0570 ) \
			||(x.f_magic == 0520 ) \
			||(x.f_magic == 0610 ) \
			||(x.f_magic == 0615 ) \
			||(x.f_magic == 0620 ) \
			||(x.f_magic == 0625 ) \
			||(x.f_magic == 0525 ) )

#ifndef SAFEIO
#define FSEEK(a,b,c)		fseek(a,b,c)
#define FREAD(a,b,c,d)		fread(a,b,c,d)
#define FWRITE(a,b,c,d)		fwrite(a,b,c,d)
#define FOPEN(a,b)		fopen(a,b)
#else /* SAFEIO */
#define FSEEK(a,b,c)	if(fseek(a,b,c)) fprintf(stderr,"fseek error at [ %d ]\n",b),exit(1) 
#define FREAD(a,b,c,d)	if(!fread(a,b,c,d)) fprintf(stderr,"fread = 0\n")
#define FWRITE(a,b,c,d)	if(!fwrite(a,b,c,d)) fprintf(stderr,"fwrite = 0\n")
#define FOPEN(a,b)		fopen(a,b)
#endif

#define ZEROES(x) x->sym._n._n_n._n_zeroes
#define OFFSET(x) x->sym._n._n_n._n_offset
#define   NAME(x) x->sym._n._n_name
#define  VAL(x) x->sym.n_value
#define  SCNUM(x) x->sym.n_scnum
#define  TYPE(x) x->sym.n_type
#define  SCLASS(x) x->sym.n_sclass
#define  NUMAUX(x) x->sym.n_numaux

#define OLDNAME 0
#define NEWNAME 1
#define LONG 0
#define SHORT 1
extern long conflict;
#define NEWFILE 01
#define NEWTBL 02
#define NOCHANGE -1
#define ASNAME -2
#define CNAME -3

#include <ctype.h>
#define iscalpha(c) ((isalpha(c)) || ((c) == '_'))
#define iscalnum(c) ((iscalpha(c)) || (isalnum(c)))
