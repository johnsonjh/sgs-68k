#ident	"@(#)protos.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#if defined __STDC__

/* Function Prototypes for "/mnt/barryk/wd/cmd/mach/../../cmd/common/cc.c" (generated Mar 15 1990, 18:20) */
extern int  main(int , char **);
extern int  idexit(void);
extern int  dexit(void);
extern int  getsuf(char *);
extern char * setsuf(char *, int );
extern int  callsys(char *, char **);
extern int  nodup(char **, char *);
extern char * savestr(char *);
extern char * strspl(char *, char *);
extern char * getpref(char *);
extern char * makename(char *, char *, char *);
extern int  getXpass(int , char *);
extern char * stralloc(int );
extern int  addopt(int , char *);
extern int  chg_pathnames(char *, char *, char *);

#endif /* defined __STDC__ */
