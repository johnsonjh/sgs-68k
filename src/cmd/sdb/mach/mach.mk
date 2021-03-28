#	START NEW ARIX SCCS HEADER
#
#	@(#) mach.mk: version 25.1 created on 12/2/91 at 17:15:23
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)mach.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#

#ident	"@(#)sdb/mach:mach.mk	25.1"

#		Copyright (c) 1985 AT&T		#
#		All Rights Reserved		#
#	OLD MOT:makefile	6.8	
include $(IROOT)/macdefs

# This is a posix binary.  Turn off the SVID flag
SVID_FLAG=

#FPS=M68881
FPS=NOFPU

PROCESSOR=M68020

#LOCAL_CFLAGS=	-Xstrwrite -DDEBUG
#LOCAL_CFLAGS=	-Xstrwrite -DDEBUG=2
LOCAL_CFLAGS =	-Xstrwrite

DEFINES= -Dm68k -DFLEXNAMES -I$(INCSRC)/inc -I$(INCSRC)/inc/common \
	 -D$(PROCESSOR) -DSDB -D$(FPS) -DDEBUG=4 -D$(FLAVOR) -U$(UFLAVOR) \
	`/bin/echo "$(CFLAGS)"|sed -e 's/-[DU]M680[24]0//g'` 

UBIN = $(ROOT)/usr/bin
TESTDIR = ../$(FLAVOR)
INSDIR =
INCSRC = $(SROOT)/cmd/sgs
DISDIR = $(INCSRC)/dis/mach
DISDIRP = $(DISDIR)/$(FAMILY)
DISINC = -I$(DISDIR) -I$(DISDIRP) -I$(INCSRC)/inc -I$(INCSRC)/common

DIS.H=	$(DISDIR)/dis.h	$(DISDIRP)/names.h
DIS.C=	$(DISDIRP)/text_dis.c	$(DISDIRP)/names.c	$(DISDIRP)/881_gen.c \
	$(DISDIRP)/cp_gen.c	$(DISDIRP)/extended.c	dis_utils.c

DIS.O=	$(TESTDIR)/text_dis.o $(TESTDIR)/names.o $(TESTDIR)/881_gen.o \
	$(TESTDIR)/cp_gen.o $(TESTDIR)/extended.o $(TESTDIR)/dis_utils.o

OBJS=	$(TESTDIR)/main.o $(TESTDIR)/re.o $(TESTDIR)/fio.o \
	$(TESTDIR)/bio.o $(TESTDIR)/decode.o $(TESTDIR)/docomm.o \
	$(TESTDIR)/sub.o  \
	$(TESTDIR)/access.o $(TESTDIR)/pcs.o $(TESTDIR)/runpcs.o \
	$(TESTDIR)/setup.o $(TESTDIR)/message.o \
	$(TESTDIR)/symt.o $(TESTDIR)/display.o $(TESTDIR)/prvar.o \
	$(TESTDIR)/xeq.o $(TESTDIR)/version.o \
	$(TESTDIR)/opset.o $(TESTDIR)/optab.o $(TESTDIR)/machdep.o \
	$(TESTDIR)/udef.o \
	$(TESTDIR)/debug.o

COM= ../com
SOURCE=	$(COM)/main.c $(COM)/re.c $(COM)/fio.c $(COM)/bio.c $(COM)/decode.c  \
	$(COM)/docomm.c $(COM)/sub.c $(COM)/access.c $(COM)/pcs.c  \
	$(COM)/runpcs.c $(COM)/setup.c $(COM)/message.c  \
	$(COM)/symt.c $(COM)/display.c $(COM)/prvar.c $(COM)/xeq.c  \
	$(COM)/version.c $(COM)/opset.c $(COM)/optab.c  \
	$(COM)/machdep.c $(COM)/udef.c $(COM)/debug.c 

CSOURCE= $(COM)/machdep.h $(COM)/access.c $(COM)/machdep.c $(COM)/opset.c \
	$(COM)/optab.c $(COM)/runpcs.c $(COM)/setup.c $(COM)/xeq.c

compile all: $(TESTDIR)/sdb

$(TESTDIR)/sdb:	$(OBJS) $(DIS.O)
	$(CC) $(DEFINES) -o $(TESTDIR)/sdb $(OBJS) $(DIS.O) $(LIBFLAGS) $(LDFLAGS) $(CFLAGS)


COFF.H= $(COM)/coff.h $(INCSRC)/inc/mach/sgs.h 
MACHDEP.H= $(COM)/machdep.h 
MODE.H= $(COM)/mode.h
HEAD.H= $(COM)/head.h $(COM)/bio.h $(MODE.H) $(MACHDEP.H) 


$(TESTDIR)/decode.o:	$(HEAD.H) $(COM)/decode.c
		$(CC) $(DEFINES) -c $(COM)/decode.c  && mv $(@F) $@

$(TESTDIR)/docomm.o:	$(HEAD.H) $(COFF.H) $(COM)/docomm.c
		$(CC) $(DEFINES) -c $(COM)/docomm.c  && mv $(@F) $@

$(TESTDIR)/main.o:		$(HEAD.H) $(COM)/main.c
		$(CC) $(DEFINES) -c $(COM)/main.c  && mv $(@F) $@

$(TESTDIR)/sub.o:		$(HEAD.H) $(COFF.H) $(COM)/sub.c
		$(CC) $(DEFINES) -c $(COM)/sub.c  && mv $(@F) $@

$(TESTDIR)/re.o:		$(HEAD.H) $(COM)/re.c
		$(CC) $(DEFINES) -c $(COM)/re.c  && mv $(@F) $@

$(TESTDIR)/fio.o:		$(HEAD.H) $(COM)/fio.c
		$(CC) $(DEFINES) -c $(COM)/fio.c  && mv $(@F) $@

$(TESTDIR)/bio.o:		$(COM)/bio.h $(COM)/bio.c
		$(CC) $(DEFINES) -c $(COM)/bio.c  && mv $(@F) $@

$(TESTDIR)/access.o:	$(HEAD.H) $(COM)/access.c
		$(CC) $(DEFINES) -c $(COM)/access.c  && mv $(@F) $@

$(TESTDIR)/pcs.o:		$(HEAD.H) $(COM)/pcs.c
		$(CC) $(DEFINES) -c $(COM)/pcs.c  && mv $(@F) $@

$(TESTDIR)/runpcs.o:	$(HEAD.H) $(COM)/runpcs.c
		$(CC) $(DEFINES) -c $(COM)/runpcs.c  && mv $(@F) $@

$(TESTDIR)/xeq.o:		$(HEAD.H) $(COFF.H) $(COM)/xeq.c
		$(CC) $(DEFINES) -c $(COM)/xeq.c  && mv $(@F) $@

$(TESTDIR)/setup.o:	$(HEAD.H) $(COFF.H) $(COM)/setup.c
		$(CC) $(DEFINES) -c $(COM)/setup.c  && mv $(@F) $@

$(TESTDIR)/message.o:	$(MODE.H) $(COM)/message.c
		$(CC) $(DEFINES) -Xnostrwrite -c $(COM)/message.c && mv $(@F) $@ 

$(TESTDIR)/symt.o:		$(HEAD.H) $(COFF.H) $(COM)/symt.c
		$(CC) $(DEFINES) -c $(COM)/symt.c  && mv $(@F) $@

$(TESTDIR)/display.o:	$(HEAD.H) $(COFF.H)  $(COM)/display.c
		$(CC) -g $(DEFINES) -c $(COM)/display.c  && mv $(@F) $@

$(TESTDIR)/prvar.o:	$(HEAD.H) $(COFF.H) $(COM)/prvar.c
		$(CC) $(DEFINES) -c $(COM)/prvar.c  && mv $(@F) $@

$(TESTDIR)/version.o:	$(COM)/version.c
		$(CC) $(DEFINES) -c $(COM)/version.c  && mv $(@F) $@

$(TESTDIR)/opset.o:	$(HEAD.H) $(COM)/opset.c
		$(CC) $(DEFINES) -c $(COM)/opset.c  && mv $(@F) $@

$(TESTDIR)/optab.o:	$(MODE.H) $(MACHDEP.H)  $(COM)/optab.c
		$(CC) $(DEFINES) -Xnostrwrite -c $(COM)/optab.c  && mv $(@F) $@

$(TESTDIR)/machdep.o:	$(HEAD.H) $(COFF.H) $(COM)/machdep.c
		$(CC) $(DEFINES) -c $(COM)/machdep.c  && mv $(@F) $@

$(TESTDIR)/udef.o:		$(HEAD.H) $(COFF.H) $(COM)/udef.c
		$(CC) $(DEFINES) -c $(COM)/udef.c  && mv $(@F) $@

#
#			Build files for disassembler.
#			source is from the utility 'dis'.
#
disobjs:	$(DIS.O) $(DIS.H)
$(TESTDIR)/text_dis.o: $(DISDIRP)/text_dis.c $(DIS.H)
	$(CC) $(DISINC) $(DEFINES) -c $(DISDIRP)/text_dis.c  && mv $(@F) $@

$(TESTDIR)/names.o: $(DISDIRP)/names.c $(DIS.H)
	$(CC) $(DEFINES) -c $(DISINC) $(DISDIRP)/names.c  && mv $(@F) $@

$(TESTDIR)/extended.o: $(DISDIRP)/extended.c $(DIS.H)
	$(CC) $(DEFINES) -c $(DISINC) $(DISDIRP)/extended.c  && mv $(@F) $@

$(TESTDIR)/881_gen.o: $(DISDIRP)/881_gen.c $(DIS.H)
	$(CC) $(DEFINES) -c $(DISINC) $(DISDIRP)/881_gen.c  && mv $(@F) $@

$(TESTDIR)/cp_gen.o: $(DISDIRP)/cp_gen.c $(DIS.H)
	$(CC) $(DEFINES) -c $(DISINC) $(DISDIRP)/cp_gen.c  && mv $(@F) $@

$(TESTDIR)/dis_utils.o: dis_utils.c $(DIS.H)
	$(CC) $(DEFINES) -c $(DISINC) dis_utils.c  && mv $(@F) $@
#
#			End of disassembler related files
#

$(TESTDIR)/debug.o:	$(COM)/head.h $(COFF.H) $(COM)/debug.c
		$(CC) $(DEFINES) -c $(COM)/debug.c  && mv $(@F) $@

install:	all
		$(INS) $(TESTDIR)/sdb $(ROOT)/bin/sdb.$(FLAVOR)
		chmod 755 $(ROOT)/bin/sdb.$(FLAVOR)
		chgrp bin $(ROOT)/bin/sdb.$(FLAVOR)
		chown bin $(ROOT)/bin/sdb.$(FLAVOR)
		@-if [ "$(FLAVOR)" = "$(MACH)" ] ; \
		then \
		rm -f $(ROOT)/bin/sdb ; \
		echo "ln $(ROOT)/bin/sdb.$(MACH) $(ROOT)/bin/sdb" ; \
		ln $(ROOT)/bin/sdb.$(MACH) $(ROOT)/bin/sdb ; \
		fi

clean :
		-rm -f $(OBJS) $(DIS.O)

clobber: clean
		-rm -f $(TESTDIR)/sdb

		-rm -f greplist tags
SOURCES= $(DIS.C) $(SOURCE) $(CSOURCE)
ctags:
	ctags $(SOURCES)
	echo 'grep $$* \\' >greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
