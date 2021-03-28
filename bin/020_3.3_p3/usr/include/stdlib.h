/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


#if !defined __STRICT_STDC__
#include "sys/types.h"
#endif

#ifndef _STDLIB_H
#ident	"@(#)stdlib.h	24.1	10/28/91 Copyright (c) 1991 by Arix Corp."
#define _STDLIB_H
#pragma SYS

/*****************************************************************************
                                  M A C R O S
 ****************************************************************************/

#define NULL			0	/* Also defined in locale, stddef, stdio, */
							/* string and time						  */
#define EXIT_FAILURE	0
#define EXIT_SUCCESS	1
#define MB_CUR_MAX		1
#define MB_LEN_MAX		1
#define RAND_MAX		32767


/*****************************************************************************
                                  T Y P E S
 ****************************************************************************/

#if  !defined _STDDEF_H && !defined _STDIO_H && !defined _STRING_H && !defined _TIME_H && !defined SYS_TYPES_H
typedef unsigned int size_t;	/* Also defined in stddef, stdio, string */
								/* and time								 */
#endif

#if !defined _STDDEF_H
typedef int wchar_t;			/* Also defined in stddef				 */
#endif

typedef struct {int quot;int rem; } div_t;
typedef struct {long quot;long rem; } ldiv_t;


/*****************************************************************************
                   E X T E R N A L    D E C L A R A T I O N S 
 ****************************************************************************/

#if __STDC__ > 0
#define _PARMS(x) x
#else
#define _PARMS(x) ()
#endif

void abort _PARMS((void));
int abs _PARMS((int));
int atexit _PARMS((void(*)(void)));
double atof _PARMS((const char *));
int atoi _PARMS((const char *));
long int atol _PARMS((const char *));
void *bsearch _PARMS((const void *,const void *,size_t,size_t,int(*)(const void *,const void *)));
div_t div _PARMS((int,int));
void exit _PARMS((int));
void free _PARMS((void *));
char *getenv _PARMS((const char *));
long int labs _PARMS((long int));
ldiv_t ldiv _PARMS((long int,long int));
int mblen _PARMS((const char *,size_t));
size_t mbstowcs _PARMS((wchar_t *,const char *,size_t));
int mbtowc _PARMS((wchar_t *,const char *,size_t));
void qsort _PARMS((void *,size_t,size_t,int(*)(const void *,const void *)));
int rand _PARMS((void));
void srand _PARMS((unsigned int));
double strtod _PARMS((const char *,char **));
long int strtol _PARMS((const char *,char **,int));
unsigned long strtoul _PARMS((const char *,char **,int));
int system _PARMS((const char *));
size_t wcstombs _PARMS((char *,const wchar_t *,size_t));
int wctomb _PARMS((char *,wchar_t));

#if defined _SYSV
char *malloc();
char *calloc();
char *realloc();
#else
void *malloc _PARMS((size_t));
void *calloc _PARMS((size_t,size_t));
void *realloc _PARMS((void *,size_t));
#endif	/* _SYSV */

#if !defined __STRICT_STDC__

#include "dirent.h"
#include "grp.h"
#include "stdio.h"

extern int  a64l _PARMS((char *));
extern int  cftime _PARMS((char *, char *, int *));
extern int  ascftime _PARMS((char *, char *, struct tm *));
extern int  closedir _PARMS((DIR *));
extern void  setkey _PARMS((char *));
extern void  encrypt _PARMS((char *, int ));
extern char * crypt _PARMS((char *, char *));
extern int  setchrclass _PARMS((char *));
extern double  drand48 _PARMS((void));
extern double  erand48 _PARMS((unsigned short *));
extern int  lrand48 _PARMS((void));
extern long  mrand48 _PARMS((void));
extern void  srand48 _PARMS((int ));
extern unsigned short * seed48 _PARMS((unsigned short *));
extern void  lcong48 _PARMS((unsigned short *));
extern long  nrand48 _PARMS((unsigned short *));
extern long  jrand48 _PARMS((unsigned short *));
extern int  dup2 _PARMS((int , int ));
extern char * ecvt _PARMS((double , int , int *, int *));
extern char * fcvt _PARMS((double , int , int *, int *));
extern char * cvt  _PARMS((double , int , int *, int *, int));
extern int  execlp _PARMS((char *, char *, ... ));
extern int  execvp _PARMS((char *, char **));
extern key_t  ftok _PARMS((char *, char ));
extern int  ftw _PARMS((char *, int (*)(), int ));
extern char * gcvt _PARMS((double , int , char *));
extern char * getcwd _PARMS((char *, int ));
extern void  setgrent _PARMS((void));
extern void  endgrent _PARMS((void));
extern struct group * getgrent _PARMS((void));
extern struct group * fgetgrent _PARMS((FILE *));
extern struct group * getgrgid _PARMS((int ));
extern struct group * getgrnam _PARMS((char *));
extern char * getlogin _PARMS((void));
extern int  getopt _PARMS((int , char **, char *));
extern int  getpw _PARMS((int , char *));
extern void  setpwent _PARMS((void));
extern void  endpwent _PARMS((void));
extern struct passwd * getpwent _PARMS((void));
extern struct passwd * fgetpwent _PARMS((FILE *));
extern struct passwd * getpwnam _PARMS((char *));
/* extern struct passwd * getpwuid _PARMS((int )); */
extern struct utmp * getutent _PARMS((void));
extern struct utmp * getutid _PARMS((struct utmp *));
extern struct utmp * getutline _PARMS((struct utmp *));
extern struct utmp * pututline _PARMS((struct utmp *));
extern int  setutent _PARMS((void));
extern int  endutent _PARMS((void));
extern int  utmpname _PARMS((char *));
extern int  hcreate _PARMS((int ));
extern void  hdestroy _PARMS((void));
/* extern struct entry * hsearch _PARMS((struct entry , enumty )); */
extern int  isatty _PARMS((int ));
extern void  ltol3 _PARMS((char *, int *, int ));
extern void  l3tol _PARMS((int *, char *, int ));
extern char * l64a _PARMS((int ));
extern char * lfind _PARMS((char *, char *, unsigned *, unsigned , int (*)()));
extern char * lsearch _PARMS((char *, char *, unsigned *, unsigned , int (*)()));
extern char * mktemp _PARMS((char *));
extern void  monitor _PARMS((char *, char *, unsigned short *, int , int ));
extern int  nlist _PARMS((char *, struct nlist *));
extern DIR * opendir _PARMS((char *));
extern int  putenv _PARMS((char *));
extern int  putpwent _PARMS((struct passwd *, FILE *));
extern struct dirent * readdir _PARMS((DIR *));
extern void  seekdir _PARMS((DIR *, int ));
extern unsigned  sleep _PARMS((unsigned ));
extern int (*ssignal _PARMS((int , int (*)())) ) ();
extern int  gsignal _PARMS((int ));
extern void  swab _PARMS((short *, short *, int ));
extern long  tell _PARMS((int ));
extern long  telldir _PARMS((DIR *));
extern char * tfind _PARMS((char *, struct node **, int (*)()));
extern char * tsearch _PARMS((char *, struct node **, int (*)()));
extern char * tdelete _PARMS((char *, struct node **, int (*)()));
extern void  twalk _PARMS((struct node *, void (*)()));
extern char * ttyname _PARMS((int ));
extern int  ttyslot _PARMS((void));

extern unsigned int rd881_control  _PARMS((void ));
extern unsigned int rd881_iaddr  _PARMS((void ));
extern unsigned int rd881_status  _PARMS((void ));
extern void wr881_control  _PARMS((unsigned int ));


#endif	/* ! __STRICT_STDC__ */
#pragma noSYS
#endif	/* _STDLIB_H */
