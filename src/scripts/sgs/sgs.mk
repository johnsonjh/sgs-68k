#	START NEW ARIX SCCS HEADER
#
#	@(#) sgs.mk: version 25.1 created on 12/2/91 at 20:25:51
#
#	Copyright (c) 1990 by Arix Corporation
#	All Rights Reserved
#
#	@(#)sgs.mk	25.1	12/2/91 Copyright (c) 1990 by Arix Corporation
#
#	END NEW ARIX SCCS HEADER
#

#ident	"@(#)scripts/sgs:sgs.mk	25.1"

IROOT =
include $(IROOT)/macdefs

INSDIR=$(ROOT)/usr/options
FILES = sgs.un sgs.name sgs.rlst

all clean clobber:

install: $(FILES)
	 $(INS) sgs.un $(INSDIR)/sgs.un
	 $(INS) sgs.name $(INSDIR)/sgs.name
	 $(INS) sgs.rlst $(INSDIR)/sgs.rlst
	chmod 500 $(INSDIR)/sgs.un
	chgrp bin $(INSDIR)/sgs.un
	chown bin $(INSDIR)/sgs.un
	chmod 444 $(INSDIR)/sgs.name
	chgrp bin $(INSDIR)/sgs.name
	chown bin $(INSDIR)/sgs.name
	chmod 444 $(INSDIR)/sgs.rlst
	chgrp bin $(INSDIR)/sgs.rlst
	chown bin $(INSDIR)/sgs.rlst

