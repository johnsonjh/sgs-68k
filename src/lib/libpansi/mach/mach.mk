#ident	"@(#)mach.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:ident	"libc-m32:makefile	1.43"


include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs

# Create POSIX binaries
SVID_FLAG=

#
#
# The variable PROF is null by default, causing profiled object to be
# maintained.  If profiled object is not desired, the reassignment
# PROF=@# should appear in the make command line.
#

.SUFFIXES: .p

M4=m4 m4.def
SYSTRAP_H=$(IROOT)/systrap
PROF=
NONPROF=
INCLIBC=../inc
DEFLIST=
#SDEFLIST=
INCLIST=-I$(INCRT)

OBJECTS=\
fp/fpgetrnd.o	fp/fpsetrnd.o	fp/fpgetmask.o	fp/fpsetmask.o	\
fp/isnand.o	fp/fpgetsticky.o	fp/fpsetsticky.o	\
crt/zmcrt_def.o    sys/getppid.o      sys/rmdir.o  \
sys/zdvect_def.o   sys/getuid.o       sys/rmount.o  \
sys/gtty.o         sys/rumount.o  \
gen/sysm68k.o      \
sys/semsyscall.o   sys/setgroups.o \
sys/_sysm68k.o     sys/setgid.o       sys/getpgrp.o \
sys/acct.o         sys/mkdir.o        sys/setuid.o  \
sys/advfs.o        sys/mknod.o        sys/shmsyscall.o   sys/setpgid.o \
sys/alarm.o        sys/mount.o        \
sys/chdir.o        sys/msgsyscall.o   sys/statfs.o  \
sys/chmod.o        sys/netboot.o      sys/stime.o  \
gen/gen_def.o      sys/chown.o        \
sys/chgperf.o	sys/chgsched.o	\
sys/netunboot.o    sys/stty.o  \
sys/chroot.o       sys/nexect.o       sys/sync.o  \
sys/nice.o         sys/syscall.o  \
sys/creat.o        sys/sysfs.o  \
sys/dup.o          sys/pause.o \
sys/locking.o      sys/sysarix.o      sys/sysconf.o      sys/sysconfa.o \
sys/pipe.o         \
sys/plock.o        sys/uadmin.o  \
sys/poll.o         sys/ulimit.o  \
sys/profil.o       sys/umask.o  \
sys/ptrace.o       sys/umount.o  \
sys/putmsg.o       sys/unadvfs.o  \
sys/getdents.o     sys/rdebug.o       sys/uname.o  \
sys/getegid.o      sys/rdump.o        sys/pathconf.o \
sys/geteuid.o      sys/ustat.o	      \
sys/getgid.o       sys/rfstart.o      sys/utime.o  \
sys/getmsg.o       sys/rfstop.o       \
sys/rfsys.o        \
sys/setsid.o	   sys/mkfifo.o 	  gen/sigsetjmp.o \
sys/sec_dev.o	   gen/inode_size.o   gen/getfstyp.o \
sys/facl.o	   sys/acl.o	\
sys/setitimer.o sys/getitimer.o gen/exec_state.o \
sys/lstat.o        sys/lchown.o       sys/symlink.o     sys/readlink.o \
sys/getrlimit.o    sys/setrlimit.o

POBJECTS=\
fp/fpgetrnd.o	fp/fpsetrnd.o	fp/fpgetmask.o	fp/fpsetmask.o	\
fp/isnand.p	fp/fpgetsticky.o	fp/fpsetsticky.o	\
crt/zmcrt_def.p    sys/getppid.p      sys/rmdir.p  \
sys/zdvect_def.p   sys/getuid.p       sys/rmount.p  \
sys/gtty.p         sys/rumount.p  \
gen/sysm68k.p      \
sys/semsyscall.p   sys/setgroups.o \
sys/_sysm68k.p     sys/setgid.p  \
sys/acct.p         sys/mkdir.p        sys/setuid.p  \
sys/advfs.p        sys/mknod.p        sys/shmsyscall.p   sys/setpgid.o \
sys/alarm.p        sys/mount.p        \
sys/chdir.p        sys/msgsyscall.p   sys/statfs.p  \
sys/chmod.p        sys/netboot.p      sys/stime.p  \
gen/gen_def.p      sys/chown.p        \
sys/chgperf.p	sys/chgsched.p	\
sys/netunboot.p    sys/stty.p  \
sys/chroot.p       sys/nexect.p       sys/sync.p  \
sys/nice.p         sys/syscall.p  \
sys/creat.p        sys/sysfs.p  \
sys/dup.p          sys/pause.p \
sys/locking.p 	   sys/sysarix.p      sys/sysconf.p      sys/sysconfa.p \
sys/pipe.p         \
sys/plock.p        sys/uadmin.p  \
sys/poll.p         sys/ulimit.p  \
sys/profil.p       sys/umask.p  \
sys/ptrace.p       sys/umount.p  \
sys/putmsg.p       sys/unadvfs.p  \
sys/getdents.p     sys/rdebug.p       sys/uname.p  \
sys/getegid.p      sys/rdump.p        sys/pathconf.p \
sys/geteuid.p      sys/ustat.p	   \
sys/getgid.p       sys/rfstart.p      sys/utime.p  \
sys/getmsg.p       sys/rfstop.p       \
sys/rfsys.p        \
sys/setsid.p	      sys/mkfifo.p	 gen/sigsetjmp.p \
sys/sec_dev.p	   gen/inode_size.p   gen/getfstyp.p \
sys/acl.p	   sys/facl.p	\
sys/setitimer.p sys/getitimer.p gen/exec_state.p \
sys/lstat.p        sys/lchown.p       sys/symlink.p     sys/readlink.p \
sys/getrlimit.p    sys/setrlimit.p

all:	$(OBJECTS)

.c.o .c.p:
	$(NONPROF)@echo $*.c:
	cp $*.c $(*F).c   
	$(NONPROF)$(CC) $(DEFLIST) $(CFLAGS) -c $(*F).c && mv $(*F).o $*.o
	$(PROF)@echo $*.c:
	-rm $(*F).c
	cp $*.c $(*F).c   
	$(PROF)$(CC) $(DEFLIST) $(CFLAGS) -c -p $(*F).c  && mv $(*F).o $*.p
	-rm $(*F).c

.s.o .s.p:
	$(NONPROF)@echo $*.s:
	$(NONPROF)$(M4) $(SDEFLIST) -DMCOUNT=#  $*.s   > $(*F).s   
	$(NONPROF)$(CC) $(DEFLIST) $(CFLAGS) -c $(*F).s && mv $(*F).o $*.o
	$(PROF)@echo $*.s:
	$(PROF)$(M4) mcount.def  $(SDEFLIST) $*.s   > $(*F).s 
	$(PROF)$(CC) $(DEFLIST) $(CFLAGS) -c -p $(*F).s  && mv $(*F).o $*.p
	-rm $(*F).s

archive:
	#
	# Note that "archive" is invoked with libc/object as current directory.
	#
	# Generic libansic .o files
	mv crt1.o crt2.o ..
	$(PROF) rm -f mcount.o mcrt1.o mcrt2.o
	$(PROF) mv crt1.p crt2.p mcrt1.p mcrt2.p mcount.p ..
	-for i in *.O; \
	do \
		mv $$i `basename $$i .O`.o ; done
	mv exit.o exit.x
	mv fakcu.o fakcu.x
	$(IROOT)/stdinlorder | tsort > objlist
	mv exit.x exit.o
	mv fakcu.x fakcu.o
	#
	# build the archive with the modules in correct order.
	split -100 objlist
	-for i in xa*; \
	do \
		$(AR) q ../lib.libc `cat $$i`; \
	done
	$(AR) q ../lib.libc exit.o fakcu.o
	# Now do the NONDYNAMIC lib
	for i in *.n ; do mv $$i `basename $$i .n`.o ; done
	-for i in xa*; \
	do \
		$(AR) q ../lib.libcn `cat $$i`; \
	done
	$(AR) q ../lib.libcn exit.o fakcu.o
	$(PROF)#
	$(PROF)# build the profiled library archive, first renaming the
	$(PROF)#	.p (profiled object) modules to .o
	$(PROF)for i in *.p ; do mv $$i `basename $$i .p`.o ; done
	$(PROF)for i in *.P ; do mv $$i `basename $$i .P`.o ; done
	$(PROF) mv ../mcount.p mcount.o
	-if [ "$(PROF)" != "@#" ]; \
	then \
		for i in xa*; \
		do \
			$(AR) q ../libp.libc `cat $$i`; \
		done; \
		$(PROF)$(AR) q ../libp.libc mcount.o exit.o fakcu.o ;\
	fi

clean:
	-rm -f *.o
	-rm -f *.p
	-rm -f *.s
	-rm -f *.n

clobber: clean
	-rm -f */*.o
	-rm -f */*.p
	-rm -f fp/fpsetsticky.s fp/fpgetsticky.s

#
# following four lines are necessary because
# SCCS rejects file names exceeding 14 characters -- hr 3-29-89
#
fp/fpsetsticky.s:	fp/fpststcky.s
	-ln	fp/fpststcky.s	fp/fpsetsticky.s

fp/fpgetsticky.s:	fp/fpgtstcky.s
	-ln	fp/fpgtstcky.s	fp/fpgetsticky.s

