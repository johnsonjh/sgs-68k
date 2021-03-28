
# SCCS_ID - "@(#)Makefile (TWG)       1.7     89/08/31 "

SOURCES	= arp.h dlpi.h icmp_var.h if.h in.h in_var.h \
	  inet.h inetioctl.h ip.h ip_icmp.h ip_var.h \
	  raw.h resource.h route.h socket.h socketvar.h somod.h \
	  tcp.h tty_pty.h udp.h un.h unpcb.h \
	  nb.h nb_module.h nb_msg.h nb_name.h

OBJECTS	= 

BINDIST	= ../../bindist
SRCDIST	= ../../srcdist
INSTALL =

include	../../make.pre

SHELL	= /bin/sh

OPTS	= 
DEFINES = 

$(DRIVER) all: $(SOURCES)

bindist : $(SOURCES)
	cp $(SOURCES) $(BINDIST)/user/usr/include/tcp/sys

install : $(SOURCES)
	-mkdir $(INSTALL)/usr/include/tcp/sys
	cp $(SOURCES) $(INSTALL)/usr/include/tcp/sys

srcdist : 
	cp $(SOURCES) Makefile $(SRCDIST)/user/include/sys

clean :
	rm -f *.o core a.out

clobber : clean
