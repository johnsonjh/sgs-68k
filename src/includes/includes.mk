#ident	"@(#)includes.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
IROOT =
include $(IROOT)/macdefs

INSDIR = $(ROOT)/usr/include
DIRCTS = $(INSDIR)

all:	

dirs:
	@-for i in $(DIRCTS); \
	do \
		if [ ! -d $$i ]; \
		then \
			echo "	mkdir $$i"; \
			mkdir $$i; \
			chmod 755 $$i; \
			chown bin $$i; \
			chgrp bin $$i; \
		fi ;\
	done

install: dirs
	@-for i in *.h; \
	do \
		echo "$(INS) $$i $(INSDIR)/$$i" ; \
		$(INS) $$i $(INSDIR)/$$i ; \
		chmod 644 $(INSDIR)/$$i ; \
		chgrp bin $(INSDIR)/$$i ; \
		chown bin $(INSDIR)/$$i ; \
	done
	@echo "--->>> Copying .../system/pm/sys headers files <<<---";
	cd $(BUILD)/system/pm/sys;  \
		find . -print | cpio -pdmuva $(ROOT)/usr/include/sys

	@echo "--->>> Copying .../system/iopm/sys headers files <<<---";
	cd $(BUILD)/system/iopm/sys;  \
	find . -print | cpio -pdmuva $(ROOT)/usr/include/sys/iopm/sys

	@echo "--->>> Copying .../system/dsdb headers <<<---";
	cd $(BUILD)/system/dsdb; \
	cp */*.h $(ROOT)/usr/include/sys/iopm/dsdb

	@echo "--->>> Copying .../system/acdb headers <<<---";
	cd $(BUILD)/system/acdb/sys; \
	find . -print | cpio -pdmuva $(ROOT)/usr/include/sys/iopm/acdb

	@echo "\n--->>> ....DONE SETTING UP INCLUDE FILES"

clean:

clobber: clean
