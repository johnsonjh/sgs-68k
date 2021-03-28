#ident	"@(#)gen.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT: ident	"libc-port:gen/makefile	1.17.1.1"
#
# makefile for libc/port/gen
#
#

.SUFFIXES: .p .n
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs
PROF=
NONPROF=
INC=$(INCRT)
INCSYS=$(INCRT)/sys
INCLIBC= ../../inc
SINC=$(ROOT)/usr/include
DEFLIST=
SDEFLIST=
INCLIST=-I$(INCLIBC) -I$(INC)

OBJECTS=\
dial.n putpwent.n dial.o      a64l.o        abort.o       assert.o	\
atof.o        atoi.o        atol.o        bsearch.o	\
calloc.o      clock.o       closedir.o	  crypt.o	\
cftime.o      ctime.o	    ctype.o       ctype_def.o   \
drand48.o     dup2.o	  ecvt.o        \
errlst.o      err_def.o	    execvp.o      \
free_def.o    frexp.o	    \
ftok.o        ftw.o         gcvt.o        getcwd.o	\
getenv.o      getgrent.o    getgrgid.o    getgrnam.o	\
getlogin.o    getopt.o      getpw.o       getpwent.o	\
getpwnam.o    getpwuid.o    \
getut.o	      grent_def.o   \
hsearch.o     isatty.o	    \
l3.o          l64a.o        ldexp.o       lfind.o	\
lsearch.o     \
malloc.o      mall_def.o    \
mktemp.o      modf.o	\
mon.o         nlist.o       opendir.o	  \
o_tolower.o o_toupper.o opt_data.o    \
perror.o      putenv.o	    putpwent.o	  \
pwent_def.o   qsort.o       rand.o 	  reall_def.o	\
readdir.o     seekdir.o     sleep.o       ssignal.o	  \
strdup.o      \
strtod.o      \
strtok.o      strtol.o      swab.o        tell.o	\
telldir.o     tfind.o	    time_comm.o		\
tolower.o     toupper.o     tsearch.o     ttyname.o	\
ttyslot.o 

POBJECTS=\
dial.p        a64l.p        abort.p       assert.p	\
atof.p        atoi.p        atol.p        bsearch.p	\
calloc.p      clock.p       closedir.o	  cftime.p	crypt.p	\
ctime.p	      ctype.p       drand48.p     \
dup2.p	      errlst.p      ecvt.p	  \
execvp.p      frexp.p	  \
ftok.p        ftw.p         gcvt.p        getcwd.p	\
getenv.p      getgrent.p    getgrgid.p    getgrnam.p	\
getlogin.p    getopt.p      getpw.p       getpwent.p	\
getpwnam.p    getpwuid.p    \
getut.p       hsearch.p     isatty.p	  \
l3.p          l64a.p        ldexp.p       lfind.p	\
lsearch.p     \
malloc.p      \
mktemp.p      modf.p	\
mon.p         nlist.p       opendir.p	  perror.p      \
putenv.p      putpwent.p    \
qsort.p       rand.p        readdir.p	  \
seekdir.p	\
sleep.p       ssignal.p	    \
strdup.p      \
strtod.p      \
strtok.p      strtol.p      swab.p        tell.p	\
telldir.p     tfind.p	    time_comm.p		\
tolower.p     toupper.p     tsearch.p     ttyname.p	\
ttyslot.p 

nonprof: $(OBJECTS)

prof: $(POBJECTS)

.c.o .c.p:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c   && mv $(*F).o $*.p
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(SDEFLIST) $(INCLIST) $(CFLAGS) -c $*.c

.c.n:
	@echo $*.c:
	$(CC) $(DEFLIST) $(NDYNFLAGS) $(SDEFLIST) $(INCLIST) $(CFLAGS) -c $*.c\
		&& mv $(*F).o $*.n
clean:
	-rm -f *.o
	-rm -f *.n

clobber:	clean
	-rm -f greplist tags

SOURCES= \
dial.c  _abs.c   _strspn.c  drand48.c    getlogin.c   malloc.c     ssignal.c \
_memccpy.c   a64l.c       dup2.c       getopt.c     mktemp.c     strdup.c \
_memchr.c    abort.c      ecvt.c       getpw.c      modf.c       strrchr.c \
_memcmp.c    assert.c     err_def.c    getpwent.c   mon.c        strtod.c \
_memcpy.c    atof.c       errlst.c     getpwnam.c   nlist.c      strtok.c \
_memset.c    atoi.c       execvp.c     getpwuid.c   opendir.c    strtol.c \
_strcat.c    atol.c       _fakcu.c      getut.c      opt_data.c   swab.c \
_strchr.c    bsearch.c    frexp.c      grent_def.c  perror.c     tell.c \
_strcmp.c    calloc.c     ftok.c       hsearch.c    putenv.c     telldir.c \
_strcpy.c    clock.c      ftw.c        isatty.c     putpwent.c   tfind.c \
_strcspn.c   closedir.c   gcvt.c       l3.c         pwent_def.c  tolower.c \
_strlen.c    crypt.c      getcwd.c     l64a.c       qsort.c      toupper.c \
_strncat.c   ctime.c      getenv.c     ldexp.c      rand.c       tsearch.c \
_strncmp.c   ctype.c      getgrent.c   lfind.c      readdir.c    ttyname.c \
_strncpy.c   ctype_def.c  getgrgid.c   lsearch.c    seekdir.c    ttyslot.c \
_strpbrk.c   getgrnam.c   mall_def.c   sleep.c \
free_def.c   cftime.c     reall_def.c  o_toupper.c o_tolower.c time_comm.c

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
