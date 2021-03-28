#ident	"@(#)objconv.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
include $(IROOT)/macdefs
include $(IROOT)/sgsdefs

# POSIX

SVID_FLAG =

OBJECT = objconv
#OBJECT = a.out
XOBJECT = x.a.out
INSDIR = .
#CFLAGS=-g
#LDFLAGS=-g /newroot/usr/lib/libg.a

CFILES1 = \
asname.c      decls.c       getargs.c     printsyms.c   readsyms.c \
changename.c  directive.c   main.c        printtype.c   typesym.c \
cmptbl.c      fixsyms.c     printname.c   readhead.c    writesyms.c \
printhead.c   interact.c    reloc.c       reloc1.c       fixmagic.c

CFILES2 = \
tbls.c

XOFILES1 = \
x.asname.o      x.decls.o       x.getargs.o     x.printsyms.o   \
x.readsyms.o \
x.changename.o  x.directive.o   x.main.o        x.printtype.o   \
x.typesym.o \
x.cmptbl.o      x.fixsyms.o     x.printname.o   x.readhead.o    \
x.writesyms.o \
x.printhead.o x.interact.o x.reloc.o       x.fixmagic.o

XOFILES2 = \
x.tbls.o

OFILES1 = \
asname.o      decls.o       getargs.o     printsyms.o   readsyms.o \
changename.o  directive.o   main.o        printtype.o   typesym.o \
cmptbl.o      fixsyms.o     printname.o   readhead.o    writesyms.o \
printhead.o interact.o reloc.o       reloc1.o       fixmagic.o

OFILES2 = \
tbls.o

OFILES = $(OFILES1) $(OFILES2)
XOFILES = $(XOFILES1) $(XOFILES2)

#all: $(OBJECT) $(XOBJECT)
all: $(OBJECT) 

$(OBJECT): $(OFILES)
	$(CC) -o $(OBJECT) $(OFILES) $(LDFLAGS)
$(XOBJECT): $(XOFILES)
	acc -o $(XOBJECT) $(XOFILES) lib/ninter.o $(LDFLAGS)

decls.h : switches.h

$(OFILES1): decls.h
$(OFILES2): oldtbl.h newtbl.h dirtbl.h decls.h

$(XOFILES): $(OBJECT) $(OFILES)
	@-for i in $(OFILES); \
	do \
	echo $(OBJECT) -o x.$$i $$i; \
	$(OBJECT) -o x.$$i $$i; \
	done
	
install: all
	$(INS) $(OBJECT) $(ROOT)/local/bin/$(OBJECT)
	chmod 755 $(ROOT)/local/bin/$(OBJECT)
	chgrp bin $(ROOT)/local/bin/$(OBJECT)
	chown bin $(ROOT)/local/bin/$(OBJECT)

clean:
	rm -f $(OFILES) $(XOFILES)

clobber: clean
	rm -f $(OBJECT) $(XOBJECT) greplist tags

SOURCES=$(CFILES1)
ctags:
	ctags $(SOURCES)
	echo 'grep $$* \\' >greplist
	for i in $(SOURCES)	;\
	do echo "$$i \\"	;\
	done >> greplist
	echo /dev/null >> greplist
	chmod +x greplist
