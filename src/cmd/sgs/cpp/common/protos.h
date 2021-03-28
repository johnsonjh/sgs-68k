#ident	"@(#)protos.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#if defined __STDC__

/* Function Prototypes for "/mnt/barryk/wd/cpp/mach/../../cpp/common/cpp.c" (generated Apr 01 1990, 17:43) */
extern int  sayline(void);
extern int  newline_sync(void);
extern int  dump(void);
extern char * refill(char *);
extern char * cotoken(char *);
extern char * skipbl(char *);
extern char * unfill(char *);
extern char * doincl(char *);
extern int  equfrm(char *, char *, char *);
extern char * control(char *);
extern struct symtab * stsym(char *);
extern struct symtab * ppsym(char *);
extern int  err_exit(void);
extern struct symtab * lookup(char *, int );
extern struct symtab * slookup(char *, char *, int );
extern char * trmdir(char *);
extern char * copy(char *);
extern int  yywrap(void);
extern int  main(int , char **);
extern int  cppread(char *, FILE *);
extern int  cppwrite(char *, char *, FILE *);
extern int  trigraph(char *, int );


/* Function Prototypes for "/mnt/barryk/wd/cpp/mach/./cpy.c" (generated Apr 01 1990, 17:43) */
extern int  yyparse(void);


/* Function Prototypes for "/mnt/barryk/wd/cpp/mach/../../cpp/common/yylex.c" (generated Apr 01 1990, 17:44) */
extern int  yylex(void);
extern int  tobinary(char *, int );


/* Function Prototypes for "/mnt/barryk/wd/cpp/mach/../../cpp/common/mac.c" (generated Apr 01 1990, 17:44) */
extern char * subst(char *, struct symtab *);
extern int  react_chk(char *);
extern int  is_deact(char *);
extern void  deact_save(char *, char *, int );
extern void  deact_restore(char *, char *, int );
extern char * dodef(char *);
extern int  mac_flush(void);

#endif /* defined __STDC__ */
