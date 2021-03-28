#ident	"@(#)mach.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#
#	M68000/010/020 DISASSEMBLER MAKEFILE
#


include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

# POSIX

SVID_FLAG =

OWN=	bin		#file owner
GRP=	bin		#file group
PROT=	755		#protections
BASE=	../..

BIN = $(ROOT)/bin
PROCESSOR  = $(MACH)

# (M for Machine independent part)
# (D for Machine dependent part)
# (P for Processor dependent part)
M=      ../common
D=	$(PWD)
F=	$(PWD)/$(FAMILY)
O=$(PWD)/$(OUT)

LIBLD = -lld

LFLAGS=	-p 

ARFORMAT=	PORTAR

# Other definitions

ICOMINC=../../inc/common

ODEFLIST=	-D$(ARFORMAT) -DUNIX=4 -DMC68 -I$(ICOMINC)

CC_CMD=	cd $(OUT); $(CC) -c $(ODEFLIST) $(CFLAGS) 


OFILES=	dis_extn.o dis_main.o dis_utls.o text_dis.o lists.o extended.o names.o\
	cp_gen.o 881_gen.o

CFILES=	$(M)/dis_extn.c $(M)/dis_main.c $(M)/dis_utls.c \
	  $(M)/lists.c \
	  $(F)/text_dis.c $(F)/extended.c $(F)/names.c\
	  $(F)/cp_gen.c $(F)/881_gen.c
HFILES= \
	$(D)/dis.h $(F)/disdefs.h   $(F)/names.h \
	$(M)/structs.h

#-------------------------

all compile build:	$(O)/dis2 clean $(O)/dis4
	if [ $(MACH) = "M68020" ]; \
	then \
		ln $(O)/dis2 $(O)/dis; \
	else \
		ln $(O)/dis4 $(O)/dis; \
	fi

#-------------------------

$(O)/dis:	$(OFILES)
		$(CC) $(CFLAGS) -o $(O)/dis $(OFILES) $(LIBLD) $(LDFLAGS)

$(O)/dis2:
	$(MAKE) -$(MAKEFLAGS)f mach.mk MACH=M68020 NOTMACH=M68040 $(O)/dis
	mv $(O)/dis $(O)/dis2

$(O)/dis4:
	$(MAKE) -$(MAKEFLAGS)f mach.mk MACH=M68040 NOTMACH=M68020 $(O)/dis
	mv $(O)/dis $(O)/dis4

dis_extn.o:	dis.h \
		$(M)/structs.h \
		$(M)/dis_extn.c 
		$(CC_CMD) $(M)/dis_extn.c

lists.o:	dis.h \
		$(M)/lists.c \
		$(M)/structs.h \
		$(MCOMINC)/paths.h \
		$(MCOMINC)/sgs.h 
		$(CC_CMD) $(M)/lists.c

dis_main.o:	dis.h \
		$(M)/structs.h \
		$(M)/dis_main.c \
		$(MCOMINC)/paths.h \
		$(MCOMINC)/sgs.h 
		$(CC_CMD) $(M)/dis_main.c

dis_utls.o:	dis.h \
		$(M)/structs.h \
		$(M)/dis_utls.c \
		$(MCOMINC)/sgs.h
		$(CC_CMD) $(M)/dis_utls.c

text_dis.o:	dis.h \
		$(F)/text_dis.c \
		$(M)/structs.h \
		$(MCOMINC)/sgs.h 
		$(CC_CMD) $(F)/text_dis.c

extended.o:	dis.h $(F)/names.h \
		$(F)/extended.c
		$(CC_CMD) $(F)/extended.c

names.o:	dis.h $(F)/names.h \
		$(F)/names.c
		$(CC_CMD) $(F)/names.c

cp_gen.o:	dis.h $(F)/names.h $(F)/cp_gen.c
		$(CC_CMD) $(F)/cp_gen.c

881_gen.o:	dis.h $(F)/names.h $(F)/881_gen.c
		$(CC_CMD) $(F)/881_gen.c

#-------------------------


install:	all
		$(INS) $(O)/dis $(BIN)/$(SGS)dis
		chmod $(PROT) $(BIN)/$(SGS)dis
		chgrp $(GRP) $(BIN)/$(SGS)dis
		chown $(OWN) $(BIN)/$(SGS)dis
		$(INS) $(O)/dis2 $(BIN)/$(SGS)dis2
		chmod $(PROT) $(BIN)/$(SGS)dis2
		chgrp $(GRP) $(BIN)/$(SGS)dis2
		chown $(OWN) $(BIN)/$(SGS)dis2
		$(INS) $(O)/dis4 $(BIN)/$(SGS)dis4
		chmod $(PROT) $(BIN)/$(SGS)dis4
		chgrp $(GRP) $(BIN)/$(SGS)dis4
		chown $(OWN) $(BIN)/$(SGS)dis4

#--------------------------

clean:
	-rm -f $(OFILES)

#--------------------------

clobber:	clean
		-rm -f $(O)/dis
		-rm -f $(O)/dis2
		-rm -f $(O)/dis4

		-rm -f greplist tags
SOURCES = $(HFILES) $(CFILES)
ctags:	
	ctags	$(SOURCES)
	echo 'grep $$* \\' > greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
