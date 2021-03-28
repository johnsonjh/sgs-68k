#ident	"@(#)mach.mk	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."
#	Copyright (c) 1984 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#	ATT:ident	"libc-m32:makefile	1.43"


include	$(IROOT)/macdefs
include	$(IROOT)/sgsdefs

#
#  makefile for libc/m32
#
#
# The variable PROF is null by default, causing profiled object to be
# maintained.  If profiled object is not desired, the reassignment
# PROF=@# should appear in the make command line.
#

.SUFFIXES: .p

M4=m4 m4.def
PROF=
NONPROF=
INCLIBC=../inc
DEFLIST=
SDEFLIST=
INCLIST=-I$(INCRT)
OBJECTS=\
fp/fpgetrnd.o	fp/fpsetrnd.o	fp/fpgetmask.o	fp/fpsetmask.o	\
fp/isnand.o	fp/fpgtstcky.o	fp/fpststcky.o	\
crt/zmcrt_def.o        crt/access81.o     gen/strpbrk.o      sys/getppid.o      sys/rmdir.o  \
sys/zdvect_def.o   crt/cerror.o       gen/strrchr.o      sys/getuid.o       sys/rmount.o  \
crt/mcount.o       gen/strspn.o       sys/gtty.o         sys/rumount.o  \
csu/crt0.o	   gen/sysm68k.o      sys/ioctl.o        sys/sbrk.o  \
csu/crtn.o	   csu/crt5.o         sys/kill.o         sys/semsyscall.o  \
csu/fpdprec.o   csu/fpeprec.o   csu/fpprec.o    csu/fpsprec.o   csu/fpstart.o \
csu/mcrt0.o	   sys/_sysm68k.o     sys/link.o         sys/setgid.o  \
sys/access.o       sys/lseek.o        sys/setpgrp.o  \
csu/mcrt5.o        sys/acct.o         sys/mkdir.o        sys/setuid.o  \
sys/advfs.o        sys/mknod.o        sys/shmsyscall.o  \
gen/abs.o          sys/alarm.o        sys/mount.o        sys/stat.o  \
gen/cuexit.o       sys/chdir.o        sys/msgsyscall.o   sys/statfs.o  \
gen/fakcu.o        sys/chmod.o        sys/netboot.o      sys/stime.o  \
gen/gen_def.o      sys/chown.o        sys/netunboot.o    sys/stty.o  \
sys/chgperf.o	sys/chgsched.o	\
gen/mach_data.o    sys/chroot.o       sys/nexect.o       sys/sync.o  \
gen/memccpy.o      sys/close.o        sys/nice.o         sys/syscall.o  \
gen/memchr.o       sys/creat.o        sys/open.o         sys/sysfs.o  \
gen/memcmp.o       sys/dup.o          sys/pause.o        sys/time.o  \
sys/locking.o      sys/sysarix.o\
gen/memcpy.o       sys/execve.o       sys/pipe.o         sys/times.o  \
gen/memset.o       sys/exit.o         sys/plock.o        sys/uadmin.o  \
gen/setjmp.o       sys/fcntl.o        sys/poll.o         sys/ulimit.o  \
gen/strcat.o       sys/fork.o         sys/profil.o       sys/umask.o  \
gen/strchr.o       sys/fstat.o        sys/ptrace.o       sys/umount.o  \
gen/strcmp.o       sys/fstatfs.o      sys/putmsg.o       sys/unadvfs.o  \
gen/strcpy.o       sys/getdents.o     sys/rdebug.o       sys/uname.o  \
gen/strcspn.o      sys/getegid.o      sys/rdump.o        sys/unlink.o  \
gen/strlen.o       sys/geteuid.o      sys/read.o         sys/ustat.o  \
gen/strncat.o      sys/getgid.o       sys/rfstart.o      sys/utime.o  \
gen/strncmp.o      sys/getmsg.o       sys/rfstop.o       sys/wait.o  \
gen/strncpy.o      sys/getpid.o       sys/rfsys.o        sys/write.o   \
sys/signal.o       gen/inode_size.o   gen/getfstyp.o	 sys/getitimer.o \
sys/setitimer.o    gen/exec_state.o   sys/readlink.o     sys/lstat.o   \
sys/symlink.o      sys/lchown.o       sys/getrlimit.o    sys/setrlimit.o

POBJECTS=\
fp/fpgetrnd.o	fp/fpsetrnd.o	fp/fpgetmask.o	fp/fpsetmask.o	\
fp/isnand.p	fp/fpgtstcky.o	fp/fpststcky.o	\
crt/zmcrt_def.p        crt/access81.p     gen/strpbrk.p      sys/getppid.p      sys/rmdir.p  \
sys/zdvect_def.p   crt/cerror.p       gen/strrchr.p      sys/getuid.p       sys/rmount.p  \
crt/mcount.p       gen/strspn.p       sys/gtty.p         sys/rumount.p  \
csu/crt0.p	   gen/sysm68k.p      sys/ioctl.p        sys/sbrk.p  \
csu/crtn.p	   csu/crt5.p         sys/kill.p         sys/semsyscall.p  \
csu/fpdprec.p   csu/fpeprec.p   csu/fpprec.p    csu/fpsprec.p   csu/fpstart.p \
csu/mcrt0.p	   sys/_sysm68k.p     sys/link.p         sys/setgid.p  \
sys/access.p       sys/lseek.p        sys/setpgrp.p  \
csu/mcrt5.p        sys/acct.p         sys/mkdir.p        sys/setuid.p  \
sys/advfs.p        sys/mknod.p        sys/shmsyscall.p  \
gen/abs.p          sys/alarm.p        sys/mount.p        sys/stat.p  \
gen/cuexit.p       sys/chdir.p        sys/msgsyscall.p   sys/statfs.p  \
gen/fakcu.p        sys/chmod.p        sys/netboot.p      sys/stime.p  \
gen/gen_def.p      sys/chown.p        sys/netunboot.p    sys/stty.p  \
sys/chgperf.p	sys/chgsched.p	\
gen/mach_data.p    sys/chroot.p       sys/nexect.p       sys/sync.p  \
gen/memccpy.p      sys/close.p        sys/nice.p         sys/syscall.p  \
gen/memchr.p       sys/creat.p        sys/open.p         sys/sysfs.p  \
gen/memcmp.p       sys/dup.p          sys/pause.p        sys/time.p  \
sys/locking.p 	   sys/sysarix.p\
gen/memcpy.p       sys/execve.p       sys/pipe.p         sys/times.p  \
gen/memset.p       sys/exit.p         sys/plock.p        sys/uadmin.p  \
gen/setjmp.p       sys/fcntl.p        sys/poll.p         sys/ulimit.p  \
gen/strcat.p       sys/fork.p         sys/profil.p       sys/umask.p  \
gen/strchr.p       sys/fstat.p        sys/ptrace.p       sys/umount.p  \
gen/strcmp.p       sys/fstatfs.p      sys/putmsg.p       sys/unadvfs.p  \
gen/strcpy.p       sys/getdents.p     sys/rdebug.p       sys/uname.p  \
gen/strcspn.p      sys/getegid.p      sys/rdump.p        sys/unlink.p  \
gen/strlen.p       sys/geteuid.p      sys/read.p         sys/ustat.p  \
gen/strncat.p      sys/getgid.p       sys/rfstart.p      sys/utime.p  \
gen/strncmp.p      sys/getmsg.p       sys/rfstop.p       sys/wait.p  \
gen/strncpy.p      sys/getpid.p       sys/rfsys.p        sys/write.p  \
sys/signal.p       gen/inode_size.p   gen/getfstyp.p	 sys/getitimer.p \
sys/setitimer.p    gen/exec_state.p   sys/readlink.p     sys/lstat.p   \
sys/symlink.p      sys/lchown.p       sys/getrlimit.p    sys/setrlimit.p

all:	$(OBJECTS)

.c.o .c.p:
	$(NONPROF)@echo $*.c:
	cp $*.c $(*F).c   
	$(NONPROF)$(CC) $(DEFLIST) $(CFLAGS) -c $(*F).c && mv $(*F).o $*.o
	$(PROF)@echo $*.c:
	cp $*.c $(*F).c   
	$(PROF)$(CC) $(DEFLIST) $(CFLAGS) -c -p $(*F).c  && mv $(*F).o $*.p
	-rm $(*F).c

.s.o .s.p:
	$(NONPROF)@echo $*.s:
	$(NONPROF)$(M4) $(SDEFLIST) -DMCOUNT=#  $*.s   > $(*F).s   
	$(NONPROF)$(CC) $(DEFLIST) $(CFLAGS) -c $(*F).s && mv $(*F).o $*.o
	$(PROF)@echo $*.s:
	$(PROF)$(M4) mcount.def  $*.s   > $(*F).s 
	$(PROF)$(CC) $(DEFLIST) $(CFLAGS) -c -p $(*F).s  && mv $(*F).o $*.p
	-rm $(*F).s

archive:
	#
	# Note that "archive" is invoked with libc/object as current directory.
	#
	# figure out the correct ordering for all the archive modules except
	#	cuexit and fakcu, which must go at end of archive.
	mv cuexit.o cuexit.x
	mv fakcu.o fakcu.x
	mv signal.o signal.x
	mv kill.o kill.x
	mv getpid.o getpid.x
	mv cerror.o cerror.x
	mv malloc.o malloc.x
	$(LORDER) *.o | tsort >objlist
	mv cuexit.x cuexit.o
	mv fakcu.x fakcu.o
	mv signal.x signal.o
	mv kill.x kill.o
	mv getpid.x getpid.o
	mv cerror.x cerror.o
	mv malloc.x malloc.o
	#
	# build the archive with the modules in correct order.
	$(AR) q ../lib.libc `cat objlist` \
	signal.o \
	kill.o getpid.o malloc.o cerror.o cuexit.o fakcu.o 
	#now do the NONDYNAMIC lib
	for i in *.n ; do mv $$i `basename $$i .n`.o ; done
	$(AR) q ../lib.libcn `cat objlist` \
	signal.o \
	kill.o getpid.o malloc.o cerror.o cuexit.o fakcu.o 
	$(PROF)#
	$(PROF)# build the profiled library archive, first renaming the
	$(PROF)#	.p (profiled object) modules to .o
	$(PROF)for i in *.p ; do mv $$i `basename $$i .p`.o ; done
	if [ "$(PROF)" != "@#" ]; \
	then \
	$(PROF)$(AR) q ../libp.libc `cat objlist` \
	signal.o \
	kill.o getpid.o malloc.o cerror.o cuexit.o fakcu.o  ; \
	fi

clean:
	-rm -f *.o
	-rm -f *.p
	-rm -f *.s
	-rm -f *.n

clobber: clean
	-rm -f */*.o
	-rm -f */*.p
	-rm -f */*.n
