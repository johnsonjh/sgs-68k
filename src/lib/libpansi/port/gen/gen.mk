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

#POSIX
SVID_FLAG=

PROF=
NONPROF=
INC=$(INCRT)
INCSYS=$(INCRT)/sys
INCLIBC= ../../inc
SINC=$(ROOT)/usr/include
DEFLIST=
SDEFLIST=
INCLIST=-I$(INCLIBC) 


#
# following four lines are necessary because
# SCCS rejects file names exceeding 14 characters -- hr 3-29-1989
#
old_tolower.c:	oldtolower.c
	-ln	oldtolower.c	old_tolower.c

old_toupper.c:	oldtoupper.c
	-ln	oldtoupper.c	old_toupper.c

OBJECTS=\
dial.n filemacros.n dial.o	      a64l.o \
attr.o \
baud.o \
cfree.o      closedir.o	  crypt.o	\
cftime.o      ctype_def.o   \
drand48.o     dup2.o	    \
err_def.o	    execvp.o      \
free_def.o \
ftok.o        ftw.o         gcvt.o        getcwd.o	\
getgrent.o    getgrgid.o    getgrnam.o	\
getlogin.o    getopt.o      getpw.o       getpwent.o	\
getpwnam.o    getpwuid.o    \
getut.o	      grent_def.o   \
hsearch.o     \
l3.o          l64a.o        lfind.o	\
lsearch.o     \
mall_def.o    \
mon.o         nlist.o       opendir.o	  \
old_tolower.o old_toupper.o opt_data.o    \
putenv.o	    putpwent.o	  \
pwent_def.o   reall_def.o	\
readdir.o     seekdir.o     sleep.o       ssignal.o	  \
strdup.o      rewinddir.o   \
swab.o        tell.o	\
telldir.o     tfind.o	    \
tsearch.o     ttyname.o	\
ttyslot.o     iswhat.o      filemacros.o

POBJECTS=\
dial.p        a64l.p        \
attr.p \
baud.p \
cfree.p      closedir.o	  cftime.p	crypt.p	\
drand48.p     \
dup2.p	      \
execvp.p \
ftok.p        ftw.p         gcvt.p        getcwd.p	\
getgrent.p    getgrgid.p    getgrnam.p	\
getlogin.p    getopt.p      getpw.p       getpwent.p	\
getpwnam.p    getpwuid.p    \
getut.p       hsearch.p     \
l3.p          l64a.p        lfind.p	\
lsearch.p     \
mon.p         nlist.p       opendir.p \
putenv.p      putpwent.p    \
readdir.p	  \
seekdir.p     rewinddir.p   \
sleep.p       ssignal.p	    \
strdup.p      \
swab.p        tell.p	\
telldir.p     tfind.p	    \
tsearch.p     ttyname.p	\
ttyslot.p     iswhat.p      filemacros.p

nonprof: $(OBJECTS)

prof: $(POBJECTS)

.c.p:
	$(PROF)@echo $*.c:
	$(PROF)$(CC) $(DEFLIST) $(INCLIST) $(CFLAGS) -c -p $*.c   && mv $(*F).o $*.p
.c.o:
	$(NONPROF)@echo $*.c:
	$(NONPROF)$(CC) $(DEFLIST) $(SDEFLIST) $(INCLIST) $(CFLAGS) -c $*.c

.c.n:
	@echo $*.c:
	$(CC) $(DEFLIST) $(NDYNFLAGS) $(INCLIST) $(CFLAGS) -c $*.c   && \
		mv $(*F).o $*.n

clean:
	-rm -f *.o
	-rm -f *.n

clobber:	clean
	-rm -f greplist tags old_toupper.c old_tolower.c

SOURCES= \
dial.c       drand48.c    getlogin.c   ssignal.c \
a64l.c       dup2.c       getopt.c     strdup.c \
getpw.c      strrchr.c \
err_def.c    getpwent.c   mon.c \
getpwnam.c   nlist.c \
execvp.c     getpwuid.c   opendir.c \
_fakcu.c     getut.c      opt_data.c   swab.c \
grent_def.c  tell.c \
cfree.c     ftok.c       hsearch.c    putenv.c     telldir.c \
ftw.c        putpwent.c   tfind.c \
closedir.c   gcvt.c       l3.c         pwent_def.c \
crypt.c      getcwd.c     l64a.c \
tsearch.c \
getgrent.c   lfind.c      readdir.c    ttyname.c \
ctype_def.c  getgrgid.c   lsearch.c    seekdir.c    ttyslot.c \
getgrnam.c   mall_def.c   sleep.c \
free_def.c   cftime.c     reall_def.c  oldtoupper.c oldtolower.c \
attr.c	     baud.c  \
rewinddir.c  iswhat.c     filemacros.c

ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
